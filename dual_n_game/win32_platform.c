#include <stdint.h>

typedef int8_t s8;
typedef uint8_t u8;
typedef int16_t s16;
typedef uint16_t u16;
typedef int32_t s32;
typedef uint32_t u32;
typedef int64_t s64;
typedef uint64_t u64;
typedef float f32;
typedef double f64;

// Try to include all windows stuff
// below the game code if possible
// windows defines a lot of macros
// and other things, also since we
// compile everything into one big file
// game doesn't need to know about
// platform specific things
#include <windows.h>
#include <dsound.h>

#define Kilobytes(Val) ((Val)*1024LL)
#define Megabytes(Val) (Kilobytes(Val)*1024LL)
#define Gigabytes(Val) (Megabytes(Val)*1024LL)

s32 GlobalRunning = 1;
LPDIRECTSOUNDBUFFER GlobalSecondaryBuffer;
s64 GlobalPerformanceCounterFreq;

struct win32_screen_buffer
{
  BITMAPINFO BMInfo;
  void *Memory;
  s32 Width;
  s32 Height;
  s32 BytesPerPixel;
  s32 Pitch;
};

struct game_memory
{
  void *PermanentStorage;
  u64 PermanentStorageSize;
  void *TempStorage;
  u64 TempStorageSize;
  u32 IsInitialized;
};

struct sound_output
{
  u16 SamplesPerSecond;
  u32 ToneHz;
  u32 BytesPerSample;
  u32 SecondaryBufferSize;
  u32 RunningSampleIndex;
  s32 IsSoundValid;
  s32 SafetyBytes;
};

// TODO: move out of platform layer?
struct game_sound_buffer
{
  s32 SamplesPerSecond;
  s32 SamplesCount;
  s16 *Samples;
};
void GameGetSoundSamples(struct game_memory *Memory, struct game_sound_buffer *SoundBuffer);

// TODO: use struct for all buttons
enum {
  BUTTON_UP,
  BUTTON_DOWN,
  BUTTON_LEFT,
  BUTTON_RIGHT,
  BUTTON_ENTER,
  BUTTON_ESCAPE,
  BUTTON_1,
  BUTTON_2,

  BUTTON_R,
  BUTTON_B,

  BUTTON_COUNT // should be last item
};
struct button_state
{
  s32 IsDown;
  s32 Changed;
};
struct mouse_button_state
{
  s32 IsDown;
  s32 Changed;
  s32 X;
  s32 Y;
};
struct game_input
{
  struct button_state Buttons[BUTTON_COUNT];
  struct mouse_button_state MouseButtons[2];
  s32 MousePosX;
  s32 MousePosY;
};

LARGE_INTEGER
Win32GetWallClock()
{
  LARGE_INTEGER Result;
  QueryPerformanceCounter(&Result);
  return Result;
}

f32
Win32GetSecondsElapsed(LARGE_INTEGER Start, LARGE_INTEGER End)
{
  f32 Result = ((f32)(End.QuadPart - Start.QuadPart) / (f32)GlobalPerformanceCounterFreq);
  return Result;
}

s64
Win32GetSecondsElapsedS64(LARGE_INTEGER Start, LARGE_INTEGER End)
{
  s64 Result = ((End.QuadPart - Start.QuadPart) / GlobalPerformanceCounterFreq);
  return Result;
}

#if DEV_BUILD
  struct debug_read_file_result
  {
    u32 ContentSize;
    void *Content;
  };

  void
  DebugFreeFileMemory(void *Memory)
  {
    if(Memory)
    {
      VirtualFree(Memory, 0, MEM_RELEASE);
    }
  }

  struct debug_read_file_result
  DebugReadEntireFile(char *Filename)
  {
    struct debug_read_file_result Result = {0};
    // Create handle
    HANDLE FileHandle = CreateFile(
      Filename, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0
    );
    if(FileHandle != INVALID_HANDLE_VALUE)
    {
      LARGE_INTEGER FileSize;
      if(GetFileSizeEx(FileHandle, &FileSize))
      {
        DWORD BytesRead;
        // TODO: QuadPart is s64, but ReadFile only accepts 32-bit values
        // truncate safely (using assert?)
        // For now just cast to s32
        u32 FileSize32 = (u32)FileSize.QuadPart;
        Result.Content = VirtualAlloc(0, FileSize32, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
        if(Result.Content)
        {
          if(ReadFile(FileHandle, Result.Content, FileSize32, &BytesRead, 0) && (FileSize32 == BytesRead))
          {
            Result.ContentSize = BytesRead;
          }
          else
          {
            DebugFreeFileMemory(Result.Content);
          }
        }
      }
      CloseHandle(FileHandle);
    }
    return Result;
  }

  #define ITOA_STR_LEN 32
  // This is not meant to be a general purpose itoa
  void
  NumToStr(char Str[ITOA_STR_LEN], s32 Num)
  {
    if(Num == 0)
    {
      Str[0] = '0';
      Str[1] = '\0';
      return;
    }

    s32 Sign = 1;
    if(Num < 0)
    {
      Sign = -1;
      Num = -Num;
    }

    char TmpStr[ITOA_STR_LEN];
    s32 TmpCharInd = 0;
    // Num in Temp Str is going to be in reversed order
    while(Num)
    {
      TmpStr[TmpCharInd] = (Num % 10) + '0';
      TmpCharInd++;
      Num = Num / 10;
    }

    // Reverse it back
    s32 CharInd = 0;
    if(Sign == -1)
    {
      Str[CharInd] = '-';
      CharInd++;
    }
    while(TmpCharInd)
    {
      Str[CharInd] = TmpStr[TmpCharInd-1];
      CharInd++;
      TmpCharInd--;
    }
    Str[CharInd] = '\0';
  }

  void
  AppendToStr(char First[], char Second[], s32 *FirstStrInd)
  {
    s32 SecondStrInd = 0;
    while(Second[SecondStrInd] != '\0')
    {
      First[*FirstStrInd] = Second[SecondStrInd];
      SecondStrInd++;
      (*FirstStrInd)++;
    }
  }

  u32
  WriteEntireFile(char *FileName, char *StrToSave, u32 StrToSaveSize)
  {
    u32 Result = 0;
    // Create handle
    HANDLE FileHandle = CreateFile(
      FileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0
    );
    if(FileHandle != INVALID_HANDLE_VALUE)
    {
      LARGE_INTEGER FileSize;
      if(GetFileSizeEx(FileHandle, &FileSize))
      {
        DWORD BytesWritten;
        if(
          WriteFile(FileHandle, StrToSave, StrToSaveSize, &BytesWritten, 0) &&
          (StrToSaveSize == BytesWritten)
        )
        {
          DWORD ErrorMsg = GetLastError();
          // TODO: Check error message?
        }
      }
      CloseHandle(FileHandle);
    }
    return Result;
  }

  s32 CopyEntireFile(char *ExistingName, char *NewName)
  {
    u32 Result = CopyFile(ExistingName, NewName, 0);
    DWORD ErrorMsg = GetLastError();

    return (Result || (ErrorMsg == ERROR_FILE_NOT_FOUND));
  }

#endif

typedef HRESULT WINAPI direct_sound_create(LPGUID lpGuid, LPDIRECTSOUND* ppDS, LPUNKNOWN  pUnkOuter);

void
Win32LoadDirectSound(HWND hwnd, u16 SamplesPerSecond, u32 BufferSize)
{
  HMODULE DirectSoundLib = LoadLibraryA("dsound.dll");
  if(DirectSoundLib)
  {
    LPDIRECTSOUND DirectSound;
    direct_sound_create *DirectSoundCreate = (direct_sound_create *)GetProcAddress(DirectSoundLib, "DirectSoundCreate");
    if(DirectSoundCreate && SUCCEEDED(DirectSoundCreate(0, &DirectSound, 0)))
    {
      WAVEFORMATEX WaveFormat = {0};
      WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
      WaveFormat.nChannels = 2;
      WaveFormat.nSamplesPerSec = SamplesPerSecond;
      WaveFormat.wBitsPerSample = 16;
      WaveFormat.nBlockAlign = (WaveFormat.nChannels * WaveFormat.wBitsPerSample) / 8;
      WaveFormat.nAvgBytesPerSec = WaveFormat.nBlockAlign * WaveFormat.nSamplesPerSec;

      // Both should work: #define IDirectSound_SetCooperativeLevel(p,a,b)  (p)->lpVtbl->SetCooperativeLevel(p,a,b)
      if(SUCCEEDED(IDirectSound_SetCooperativeLevel(DirectSound, hwnd, DSSCL_PRIORITY)))
      {
        DSBUFFERDESC PrimaryBufferDesc = {0};
        PrimaryBufferDesc.dwSize = sizeof(DSBUFFERDESC);
        PrimaryBufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER;

        LPDIRECTSOUNDBUFFER PrimaryBuffer;
        if(SUCCEEDED(IDirectSound_CreateSoundBuffer(DirectSound, &PrimaryBufferDesc, &PrimaryBuffer, 0)))
        {
          if(SUCCEEDED(IDirectSoundBuffer_SetFormat(PrimaryBuffer, &WaveFormat)))
          {
          }
        }
      }
      DSBUFFERDESC SecondaryBufferDesc = {0};
      SecondaryBufferDesc.dwSize = sizeof(DSBUFFERDESC);
      SecondaryBufferDesc.dwFlags = 0;
      SecondaryBufferDesc.dwBufferBytes = BufferSize;
      SecondaryBufferDesc.dwReserved = 0;
      SecondaryBufferDesc.lpwfxFormat = &WaveFormat;

      if(SUCCEEDED(IDirectSound_CreateSoundBuffer(DirectSound, &SecondaryBufferDesc, &GlobalSecondaryBuffer, 0)))
      {
      }
    }
  }
}

void
Win32ClearSoundBuffer(struct sound_output *SoundOutput)
{
  void *Region1;
  DWORD Region1Size;
  void *Region2;
  DWORD Region2Size;

  if(SUCCEEDED(IDirectSoundBuffer_Lock(
    GlobalSecondaryBuffer, 0, SoundOutput->SecondaryBufferSize, &Region1, &Region1Size, &Region2, &Region2Size, 0
  )))
  {
    // u32 Region1SampleSize = Region1Size / SoundOutput->BytesPerSample;
    u8 *DestSample = (u8 *)Region1;

    for(DWORD SampleIndex = 0; SampleIndex < Region1Size; SampleIndex++)
    {
      *DestSample++ = 0;
    }

    // u32 Region2SampleSize = Region2Size / SoundOutput->BytesPerSample;
    DestSample = (u8 *)Region2;

    for(DWORD SampleIndex = 0; SampleIndex < Region2Size; SampleIndex++)
    {
      *DestSample++ = 0;
    }
  }

  IDirectSoundBuffer_Unlock(GlobalSecondaryBuffer, Region1, Region1Size, Region2, Region2Size);
}

void
Win32FillInSoundBuffer(
  struct sound_output *SoundOutput, struct game_sound_buffer *SourceBuffer,
  DWORD StartWriteFromByte, DWORD AmountOfBytesToWrite
)
{
  void *Region1;
  DWORD Region1Size;
  void *Region2;
  DWORD Region2Size;

  if(SUCCEEDED(IDirectSoundBuffer_Lock(
    GlobalSecondaryBuffer, StartWriteFromByte, AmountOfBytesToWrite, &Region1, &Region1Size, &Region2, &Region2Size, 0
  )))
  {
    DWORD Region1SampleSize = Region1Size / SoundOutput->BytesPerSample;
    s16 *DestSample = (s16 *)Region1;
    s16 *SourceSample = SourceBuffer->Samples;

    for(DWORD SampleIndex = 0; SampleIndex < Region1SampleSize; SampleIndex++)
    {
      *DestSample++ = *SourceSample++;
      *DestSample++ = *SourceSample++;
      SoundOutput->RunningSampleIndex++;
    }

    DWORD Region2SampleSize = Region2Size / SoundOutput->BytesPerSample;
    DestSample = (s16 *)Region2;

    for(DWORD SampleIndex = 0; SampleIndex < Region2SampleSize; SampleIndex++)
    {
      *DestSample++ = *SourceSample++;
      *DestSample++ = *SourceSample++;
      SoundOutput->RunningSampleIndex++;
    }
  }

  IDirectSoundBuffer_Unlock(GlobalSecondaryBuffer, Region1, Region1Size, Region2, Region2Size);
}

#include "game.c"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wPraram, LPARAM lParam);

int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
  LARGE_INTEGER PerfCountFrequencyResult;
  QueryPerformanceFrequency(&PerfCountFrequencyResult);
  GlobalPerformanceCounterFreq = PerfCountFrequencyResult.QuadPart;

  WNDCLASS wc = {0};

  char wTitle[] = "Game 1";

  wc.lpfnWndProc = WindowProc;
  wc.hInstance = hInstance;
  wc.lpszClassName = wTitle;
  wc.style = CS_HREDRAW|CS_VREDRAW;
  wc.hCursor = LoadCursor(0, IDC_ARROW);

  RegisterClass(&wc);

  // HWND hwnd = CreateWindowEx(0, wTitle, "Game title1", WS_OVERLAPPEDWINDOW|WS_VISIBLE,
  //   CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
  //   0, 0, hInstance, 0);
  s32 ClientWindowWidth = 512;
  s32 ClientWindowHeight = 512;

  RECT ClientRect;
  ClientRect.left = 0;
  ClientRect.top = 0;
  ClientRect.right = ClientWindowWidth;
  ClientRect.bottom = ClientWindowHeight;

  AdjustWindowRectEx(&ClientRect, WS_OVERLAPPEDWINDOW|WS_VISIBLE, 0, 0);

  s32 AdjustedWindowWidth = ClientRect.right - ClientRect.left;
  s32 AdjustedWindowHeight = ClientRect.bottom - ClientRect.top;

  RECT ParentWindowRect;
  GetWindowRect(GetDesktopWindow(), &ParentWindowRect);
  s32 CenterX = (ParentWindowRect.right / 2) - (AdjustedWindowWidth / 2);
  s32 CenterY = (ParentWindowRect.bottom / 2) - (AdjustedWindowHeight / 2);

  HWND hwnd = CreateWindowEx(0, wTitle, "Game title1", WS_OVERLAPPEDWINDOW|WS_VISIBLE,
    CenterX, CenterY, AdjustedWindowWidth, AdjustedWindowHeight,
    0, 0, hInstance, 0);

  if(hwnd == 0)
  {
    return 0;
  }

  s32 MonitorRefreshRate = 60;
  s32 GameUpdateHz = MonitorRefreshRate / 2;
  f32 TargetSecondsPerFrame = 1.0f / (f32)GameUpdateHz;

  // TODO: init sound HMH 007, 008, 009
  // Then 132 asset streaming, 137, 138 play sound
  // Sound test
  struct sound_output SoundOutput = {0};
  SoundOutput.SamplesPerSecond = 48000;
  SoundOutput.BytesPerSample = sizeof(s16)*2;
  SoundOutput.SecondaryBufferSize = SoundOutput.SamplesPerSecond * SoundOutput.BytesPerSample;
  SoundOutput.RunningSampleIndex = 0;
  SoundOutput.IsSoundValid = 0;
  SoundOutput.SafetyBytes = ((SoundOutput.SamplesPerSecond * SoundOutput.BytesPerSample) / GameUpdateHz) / 3;

  Win32LoadDirectSound(hwnd, SoundOutput.SamplesPerSecond, SoundOutput.SecondaryBufferSize);
  Win32ClearSoundBuffer(&SoundOutput);
  IDirectSoundBuffer_Play(GlobalSecondaryBuffer, 0, 0, DSBPLAY_LOOPING);

  s16 *SoundSamples = (s16 *)VirtualAlloc(0, SoundOutput.SecondaryBufferSize,
                  MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);

  HDC hdc = GetDC(hwnd);

  // RECT ClientRect;
  // GetClientRect(hwnd, &ClientRect);

  // Allocate memory for the screen pixels
  struct win32_screen_buffer ScreenBuffer = {0};
  ScreenBuffer.Width = ClientWindowWidth;
  ScreenBuffer.Height = ClientWindowHeight;
  // ScreenBuffer.Width = ClientRect.right - ClientRect.left;
  // ScreenBuffer.Height = ClientRect.bottom - ClientRect.top;

  ScreenBuffer.BytesPerPixel = sizeof(s32);
  ScreenBuffer.Pitch = ScreenBuffer.Width * ScreenBuffer.BytesPerPixel;

  // Bytes per pixel is 4 (so as sizeof(s32))
  s32 AllocSize = ScreenBuffer.Width * ScreenBuffer.Height * ScreenBuffer.BytesPerPixel;
  if(ScreenBuffer.Memory)
  {
    VirtualFree(ScreenBuffer.Memory, 0, MEM_RELEASE);
  }
  // Memory is zeroed!
  ScreenBuffer.Memory = VirtualAlloc(0, AllocSize, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);

  ScreenBuffer.BMInfo.bmiHeader.biSize = sizeof(ScreenBuffer.BMInfo);
  ScreenBuffer.BMInfo.bmiHeader.biWidth = ScreenBuffer.Width;
  // Negative Height means coord start at Top left corner
  ScreenBuffer.BMInfo.bmiHeader.biHeight = -ScreenBuffer.Height;
  ScreenBuffer.BMInfo.bmiHeader.biPlanes = 1;
  ScreenBuffer.BMInfo.bmiHeader.biBitCount = 32;
  ScreenBuffer.BMInfo.bmiHeader.biCompression = BI_RGB;

  // Allocate persitant storage
  struct game_memory GameMemory = {0};
  u64 PermanentStorageSize = Megabytes(64);
  GameMemory.PermanentStorageSize = PermanentStorageSize;
  u64 TempStorageSize = Megabytes(128);
  GameMemory.TempStorageSize = TempStorageSize;

  void *MemoryBlock = VirtualAlloc(
    0, (PermanentStorageSize + TempStorageSize), MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE
  );
  GameMemory.PermanentStorage = MemoryBlock;
  GameMemory.TempStorage = ((u8 *)MemoryBlock + PermanentStorageSize);

  if(!SoundSamples && !GameMemory.PermanentStorage && !GameMemory.TempStorage && !ScreenBuffer.Memory)
  {
    return 0;
  }

  // TODO: does this belong here?
  // TODO: https://guide.handmadehero.org/code/day013/#449
  // Consider using Transition counter for button press. (timecode: 23:43)
  struct game_input Input = {0};
  // struct button ButtonUp = {0};
  // struct button ButtonDown = {0};
  // struct button ButtonLeft = {0};
  // struct button ButtonRight = {0};
  // struct button ButtonEnter = {0};
  // struct button ButtonEscape = {0};
  // struct button ButtonBoardMenu1 = {0};
  // struct button ButtonBoardMenu2 = {0};
  // struct mouse_button ButtonMouseLeft = {0};
  // struct mouse_button ButtonMouseRight = {0};
  // Input.Buttons[0] = ButtonUp;
  // Input.Buttons[1] = ButtonDown;
  // Input.Buttons[2] = ButtonLeft;
  // Input.Buttons[3] = ButtonRight;
  // Input.Buttons[4] = ButtonEnter;
  // Input.Buttons[5] = ButtonEscape;
  // Input.Buttons[6] = ButtonBoardMenu1;
  // Input.Buttons[7] = ButtonBoardMenu2;
  // Input.MouseButtons[0] = ButtonMouseLeft;
  // Input.MouseButtons[1] = ButtonMouseRight;

  UINT DesiredSchedulerMS = 1;
  s32 SleepIsGranular = (timeBeginPeriod(DesiredSchedulerMS) == TIMERR_NOERROR);
  // LARGE_INTEGER StartTickCount;
  // LARGE_INTEGER EndTickCount;
  // LARGE_INTEGER FreqTickPerSec;
  // Large interger is a signed 64-bit integer
  // QuadPart s64 too
  // LARGE_INTEGER ElapsedMicroseconds;
  // f32 ElapsedMS = 0;

  // // Ticks per second
  // // Set once, never changes after OS boot
  // QueryPerformanceFrequency(&FreqTickPerSec);
  // // Ticks
  // QueryPerformanceCounter(&StartTickCount);

  f32 SecondsElapsedPerFrame = 0;
  LARGE_INTEGER LastCounter = Win32GetWallClock();
  LARGE_INTEGER FlipWallClock = Win32GetWallClock();

  s32 SoundIsValid = 0;

  while(GlobalRunning)
  {
    for(s32 ButtonInd = 0; ButtonInd < BUTTON_COUNT; ButtonInd++)
    {
      Input.Buttons[ButtonInd].Changed = 0;
    }

    // Get input from the user
    MSG msg = {0};
    while(PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE))
    {
      switch(msg.message)
      {
        // Mouse movement
        case WM_MOUSEMOVE:
        {
          Input.MousePosX = LOWORD(msg.lParam);
          Input.MousePosY = HIWORD(msg.lParam);
          break;
        }
        // Mouse keys
        case WM_LBUTTONDOWN:
        {
          Input.MouseButtons[0].IsDown = 1;
          Input.MouseButtons[0].X = (s32)msg.lParam & 0x0000FFFF;
          Input.MouseButtons[0].Y = (s32)msg.lParam >> 16;
        } break;
        case WM_RBUTTONDOWN:
        {
          Input.MouseButtons[1].IsDown = 1;
          Input.MouseButtons[1].X = (s32)msg.lParam & 0x0000FFFF;
          Input.MouseButtons[1].Y = (s32)msg.lParam >> 16;
        } break;
        case WM_LBUTTONUP:
        {
          Input.MouseButtons[0].IsDown = 0;
          Input.MouseButtons[0].X = (s32)msg.lParam & 0x0000FFFF;
          Input.MouseButtons[0].Y = (s32)msg.lParam >> 16;
        } break;
        case WM_RBUTTONUP:
        {
          Input.MouseButtons[1].IsDown = 0;
          Input.MouseButtons[1].X = (s32)msg.lParam & 0x0000FFFF;
          Input.MouseButtons[1].Y = (s32)msg.lParam >> 16;
        } break;
        // Keyboard keys
        case WM_KEYUP:
        case WM_KEYDOWN:
        {
          u32 VkCode = (u32)msg.wParam;
          s32 IsDown = (msg.lParam & (1 << 31)) == 0;
          switch(VkCode)
          {
            case VK_UP:
            {
              Input.Buttons[BUTTON_UP].Changed = IsDown != Input.Buttons[BUTTON_UP].IsDown;
              Input.Buttons[BUTTON_UP].IsDown = IsDown;
            } break;
            case VK_DOWN:
            {
              Input.Buttons[BUTTON_DOWN].Changed = IsDown != Input.Buttons[BUTTON_DOWN].IsDown;
              Input.Buttons[BUTTON_DOWN].IsDown = IsDown;
            } break;
            case VK_LEFT:
            {
              Input.Buttons[BUTTON_LEFT].Changed = IsDown != Input.Buttons[BUTTON_LEFT].IsDown;
              Input.Buttons[BUTTON_LEFT].IsDown = IsDown;
            } break;
            case VK_RIGHT:
            {
              Input.Buttons[BUTTON_RIGHT].Changed = IsDown != Input.Buttons[BUTTON_RIGHT].IsDown;
              Input.Buttons[BUTTON_RIGHT].IsDown = IsDown;
            } break;
            case VK_RETURN:
            {
              Input.Buttons[BUTTON_ENTER].Changed = IsDown != Input.Buttons[BUTTON_ENTER].IsDown;
              Input.Buttons[BUTTON_ENTER].IsDown = IsDown;
            } break;
            case VK_ESCAPE:
            {
              Input.Buttons[BUTTON_ESCAPE].Changed = IsDown != Input.Buttons[BUTTON_ESCAPE].IsDown;
              Input.Buttons[BUTTON_ESCAPE].IsDown = IsDown;
            } break;
            // 1 key
            case 0x31:
            {
              Input.Buttons[BUTTON_1].Changed = IsDown != Input.Buttons[BUTTON_1].IsDown;
              Input.Buttons[BUTTON_1].IsDown = IsDown;
            } break;
            // 2 key
            case 0x32:
            {
              Input.Buttons[BUTTON_2].Changed = IsDown != Input.Buttons[BUTTON_2].IsDown;
              Input.Buttons[BUTTON_2].IsDown = IsDown;
            } break;
            // B button
            case 0x42:
            {
              Input.Buttons[BUTTON_B].Changed = IsDown != Input.Buttons[BUTTON_B].IsDown;
              Input.Buttons[BUTTON_B].IsDown = IsDown;
            } break;
            // R button
            case 0x52:
            {
              Input.Buttons[BUTTON_R].Changed = IsDown != Input.Buttons[BUTTON_R].IsDown;
              Input.Buttons[BUTTON_R].IsDown = IsDown;
            } break;

            // Mouse buttons
            // case VK_LBUTTON:
            // {
            //   Input.Buttons[8].IsDown = 1;
            // } break;
            // case VK_RBUTTON:
            // {
            //   Input.Buttons[9].IsDown = 1;
            // } break;
          }
        } break;

        default:
        {
          TranslateMessage(&msg);
          DispatchMessage(&msg);
        }
      }
    }

    // Simulate game
    // TODO: create a new struct "game screen buffer"
    // and pass it to the game, because game
    // doesn't need to know about platform specific
    // things.
    // It just needs a memory, inputs and timer
    f32 ElapsedMS = SecondsElapsedPerFrame * 1000.0f;
    GameUpdateAndRender(&GameMemory, &ScreenBuffer, &Input, ElapsedMS);

    // Sound test
    LARGE_INTEGER AudioWallClock = Win32GetWallClock();
    f32 FromBeginToAudioSecond = Win32GetSecondsElapsed(FlipWallClock, AudioWallClock);
    // TODO: move below Stretch di bits ?
    DWORD PlayCursor;
    DWORD WriteCursor;
    if(SUCCEEDED(IDirectSoundBuffer_GetCurrentPosition(GlobalSecondaryBuffer, &PlayCursor, &WriteCursor)))
    {
      if(!SoundOutput.IsSoundValid)
      {
        // Index in samples
        SoundOutput.RunningSampleIndex = WriteCursor / SoundOutput.BytesPerSample;
        SoundOutput.IsSoundValid = 1;
      }

      // Since buffer is circular, when write cursor is before play cursor
      // we need to "unwrap" it by adding buffer size
      DWORD SafeWriteCursor = WriteCursor;
      if(SafeWriteCursor < PlayCursor)
      {
        SafeWriteCursor += SoundOutput.SecondaryBufferSize;
      }

      SafeWriteCursor += SoundOutput.SafetyBytes;

      f32 SecondsUntilFlip = TargetSecondsPerFrame - FromBeginToAudioSecond;
      DWORD ExpectedSoundBytesPerFrame = (SoundOutput.SamplesPerSecond * SoundOutput.BytesPerSample) /
            GameUpdateHz;
      DWORD ExpectedBytesUntilFlip = (DWORD)((SecondsUntilFlip / TargetSecondsPerFrame) * (f32)ExpectedSoundBytesPerFrame);
      DWORD ExpectedFrameBoundaryByte = PlayCursor + ExpectedSoundBytesPerFrame;

      s32 IsLowLatencyAudio = SafeWriteCursor < ExpectedFrameBoundaryByte;

      DWORD TargetCursor = 0;
      if(IsLowLatencyAudio)
      {
        // Move target cursor to the beginning of the next frame (good video/audio sync)
        // TargetCursor = ExpectedFrameBoundaryByte + ExpectedSoundBytesPerFrame;
        TargetCursor = WriteCursor;
      }
      else
      {
        // Bad latency, move target cursor where write cursor is plus some margin (video/audio slightly out of sync)
        TargetCursor = WriteCursor + ExpectedSoundBytesPerFrame + SoundOutput.SafetyBytes;
      }
      TargetCursor = (TargetCursor % SoundOutput.SecondaryBufferSize);

      DWORD AmountOfBytesToWrite = 0;
      DWORD StartWriteFromByte = (SoundOutput.RunningSampleIndex * SoundOutput.BytesPerSample) % SoundOutput.SecondaryBufferSize;
      if(TargetCursor > StartWriteFromByte)
      {
        AmountOfBytesToWrite = TargetCursor - StartWriteFromByte;
      }
      else
      {
        AmountOfBytesToWrite = SoundOutput.SecondaryBufferSize - StartWriteFromByte;
        AmountOfBytesToWrite += TargetCursor;
      }

      struct game_sound_buffer GameSoundBuffer = {0};
      GameSoundBuffer.SamplesPerSecond = SoundOutput.SamplesPerSecond;
      GameSoundBuffer.SamplesCount = AmountOfBytesToWrite / SoundOutput.BytesPerSample;
      GameSoundBuffer.Samples = SoundSamples;
      GameGetSoundSamples(&GameMemory, &GameSoundBuffer);
      Win32FillInSoundBuffer(&SoundOutput, &GameSoundBuffer, StartWriteFromByte, AmountOfBytesToWrite);
    }
    // GetCurrentPosition failed
    else
    {
      SoundOutput.IsSoundValid = 0;
    }

    // Draw the buffer that we populated in game simulation
    StretchDIBits(hdc, 0, 0, ScreenBuffer.Width, ScreenBuffer.Height,
      0, 0, ScreenBuffer.Width, ScreenBuffer.Height, ScreenBuffer.Memory,
      &ScreenBuffer.BMInfo, DIB_RGB_COLORS, SRCCOPY
    );

    FlipWallClock = Win32GetWallClock();
    LARGE_INTEGER WorkCounter = Win32GetWallClock();
    f32 WorkSecondsElapsed = Win32GetSecondsElapsed(LastCounter, WorkCounter);
    SecondsElapsedPerFrame = WorkSecondsElapsed;

    if(SecondsElapsedPerFrame < TargetSecondsPerFrame)
    {
      if(SleepIsGranular)
      {
        DWORD SleepMS = (DWORD)((TargetSecondsPerFrame - SecondsElapsedPerFrame) * 1000.0f);
        if(SleepMS > 0)
        {
          Sleep(SleepMS);
        }
      }

      f32 TestSecondsElapsedPerFrame = Win32GetSecondsElapsed(LastCounter, Win32GetWallClock());
      if(TestSecondsElapsedPerFrame < TargetSecondsPerFrame)
      {
        // Missed sleep
      }

      while(SecondsElapsedPerFrame < TargetSecondsPerFrame)
      {
        SecondsElapsedPerFrame = Win32GetSecondsElapsed(LastCounter, Win32GetWallClock());
      }
    }


    LARGE_INTEGER EndCounter = Win32GetWallClock();
    // ElapsedMS = Win32GetSecondsElapsedS64(LastCounter, EndCounter);
    // ElapsedMS = (s64)(SecondsElapsedPerFrame * 1000.0f);
    LastCounter = EndCounter;
  }


  return 0;
}

LRESULT CALLBACK
WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT Result = 0;
  switch(uMsg)
  {
    case WM_DESTROY:
    {
      GlobalRunning = 0;
    } break;
    // If case WM_PAINT exists, then
    // it must have BeginPaint and EndPaint
    // otherwise StretchDIBits won't draw anything
    case WM_PAINT:
    {
      PAINTSTRUCT Paint;
      HDC DeviceContext = BeginPaint(hwnd, &Paint);
      // TODO: should call StretchDIBits here as well?
      // see HMH day005 ~59minute
      EndPaint(hwnd, &Paint);
    } break;

    default:
    {
      Result = DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

  }
  return Result;
}
