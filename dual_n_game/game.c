#include "math.c"

// TODO: move to "common" ?
s32
f32Tos32(f32 A)
{
  s32 Result;
  if(A >= 0.0f)
  {
    Result = (s32)(A + 0.5f);
  }
  else
  {
    Result = (s32)(A - 0.5f);
  }
  return Result;
}

#if DEV_BUILD
  #define Assert(Expr) if(!(Expr)){ *(int *)0 = 0; }
#else
  #define Assert(Expr)
#endif

struct memory_arena
{
  void *Base;
  size_t Size;
  size_t Used;
};

void
InitializeArena(struct memory_arena *Arena, u8 *Base, size_t Size)
{
  Arena->Size = Size;
  Arena->Base = Base;
  Arena->Used = 0;
}

#define PushMem(Arena, Count, Type) (Type *)PushMem_(Arena, (Count)*sizeof(Type))
void *
PushMem_(struct memory_arena *Arena, size_t Size)
{
  Assert((Arena->Used + Size) <= Arena->Size);
  // Point to the end of previous allocation
  // that is the beginning of the new one
  u8 *Result = (u8 *)Arena->Base + Arena->Used;
  // TODO: This doesn't work
  // It shouldn't work in C++ as well, but it is working in HMH code
  // why?
  // void *Result = Arena->Base + Arena->Used;
  // Next time we allocate, start with Base + Used
  Arena->Used += Size;
  // return Result;
  return (void *)Result;
}

#define TILE_MAP_ARR_X 1
#define TILE_MAP_ARR_Y 2
#define TILE_MAP_ARR_Z 9
struct tile_map
{
  u32 *Tiles;
};

struct board_timer
{
  f32 MS;
  f32 ToShowMS;
  f32 ToHideMS;
};

#define LEVEL_MAX_STEPS 101
struct level
{
  // TODO: is this used?
  s32 Count;

  s32 AvailableCount;
  s32 ShouldClearBoard;
  s32 DecreaseNLevel;
  s32 IsNew;
  // Current number of shown items within one level
  s32 Step;
  s32 MaxSteps;
  s32 NValue;
  // Tutorial levels
  s32 Tutorial;

  // 1 - position
  // 2 - position + color
  // 3 - position + color + sound
  // 4 - position + color + sound + shape
  s32 Mode;

  s32 CurrColor;
  // TODO: move score to a separate struct?
  s32 PosHit;
  s32 TotalExpectedPosHits;

  s32 ColorHit;
  s32 ColorMiss;
  s32 ColorBtnIsClicked;
  s32 TotalCorrectColorHits;

  s32 *BoardStatePosition;
  s32 *BoardStateSound;
  s32 *BoardStateImage;
  s32 *BoardStateColor;
  // Stores player's input to calculate score
  s32 *PlayerInputForPosition;
  s32 *PlayerInputForColor;
};

struct menu_button
{
  s32 Width;
  s32 Height;
  s32 Color;
  s32 StartX;
  s32 StartY;

  s32 IsClicked;
};

struct ui_board
{
  s32 Width;
  s32 Height;
  s32 RightOffsetX;
  s32 TopOffsetY;
  s32 StartX;
};

#define MENU_BUTTONS_NUM 2
struct main_menu_state
{
  struct menu_button *Buttons[MENU_BUTTONS_NUM];
};

// #define BOARD_BUTTONS_NUM 4
enum {
  BOARD_BUTTON_POSITION,
  BOARD_BUTTON_SOUND,
  BOARD_BUTTON_COLOR,
  BOARD_BUTTON_SHAPE,

  // Side menu for tutorial
  BOARD_BUTTON_RESTART,
  BOARD_BUTTON_BACK,

  BOARD_BUTTON_COUNT // should be last item
};
struct board_menu_state
{
  struct menu_button *Buttons[BOARD_BUTTON_COUNT];
};

void
DebugPrint(char *text, u32 num)
{
  char DebugBufferStr[256];
  wsprintf(
    DebugBufferStr,
    "%s %d\n",
    text,
    num
  );
  OutputDebugStringA(DebugBufferStr);
}


// Random number generator
// xorshift algorithm
struct rand_state
{
  u32 Seed;
};
struct rand_state RandNumState = {0};

u32
xorshift32(struct rand_state *State)
{
  u32 Result = State->Seed;
  Result ^= Result << 13;
  Result ^= Result >> 17;
  Result ^= Result << 5;

  State->Seed = Result;
  // char DebugBufferStr[256];
  // wsprintf(
  //   DebugBufferStr,
  //   "%u\n",
  //   Result
  // );
  // OutputDebugStringA(DebugBufferStr);
  return Result;
}

// Force 1 byte packing (it's not by default)
#pragma pack(push, 1)
// Windows 2.x, 3.x, and 4.x BMP files contain four sections:
// a file header, a bitmap information header, a color palette, and the bitmap data.
// All versions of BMP format files begin with the following 14-byte heade
struct bitmap_header {
  // File header
  u16 FileType;     /* File type, always 4D42h ("BM") */
  u32 FileSize;     /* Size of the file in bytes */
  u16 Reserved1;    /* Always 0 */
  u16 Reserved2;    /* Always 0 */
  u32 BitmapOffset; /* Starting position of image data in bytes */
  // Bitmap header
  u32 Size;            /* Size of this header in bytes */
  s32 Width;           /* Image width in pixels */
  s32 Height;          /* Image height in pixels */
  u16 Planes;          /* Number of color planes */
  u16 BitsPerPixel;    /* Number of bits per pixel */
  u32 Compression;     /* Compression methods used */
  u32 SizeOfBitmap;    /* Size of bitmap in bytes */
  s32 HorzResolution;  /* Horizontal resolution in pixels per meter */
  s32 VertResolution;  /* Vertical resolution in pixels per meter */
  u32 ColorsUsed;      /* Number of colors in the image */
  u32 ColorsImportant; /* Minimum number of important colors */
  /* Fields added for Windows 4.x follow this line */
  u32 RedMask;       /* Mask identifying bits of red component */
  u32 GreenMask;     /* Mask identifying bits of green component */
  u32 BlueMask;      /* Mask identifying bits of blue component */
  u32 AlphaMask;     /* Mask identifying bits of alpha component */
};

struct riff_header
{
  u32 RiffId;
  u32 RiffSize;
  // Always equals to "WAVE"
  u32 WaveId;
};

struct fmt_chunk
{
  // Chunk marker with trailing space 'f', 'm', 't', ' '
  u32 FmtId;
  u32 FmtChunkSize;
};

struct wav_data_chunk
{
  // 1 is PCM
  u16 FormatTag;
  u16 NumberOfChannels;
  u32 SamplesPerSec;
  u32 AvgBytesPerSec;
  u16 BlockAlign;
  u16 BitsPerSample;
  // Chunk Id "data"
  u32 DataId;
  u32 DataSize;
};
#pragma pack(pop)

struct loaded_bmp
{
  s32 Width;
  s32 Height;
  u32 *Pixels;
};

struct loaded_wav
{
  s32 SamplesCount;
  s16 *Samples[2];
};

struct playing_sound
{
  f32 Volume[2];
  s32 SamplesPlayed;
  struct playing_sound *Next;
};

enum
{
  SPRITE_NUMBER_0,
  SPRITE_NUMBER_1,
  SPRITE_NUMBER_2,
  SPRITE_NUMBER_3,
  SPRITE_NUMBER_4,
  SPRITE_NUMBER_5,
  SPRITE_NUMBER_6,
  SPRITE_NUMBER_7,
  SPRITE_NUMBER_8,
  SPRITE_NUMBER_9,

  SPRITE_NUMBER_COUNT // should be last
};

struct sprite_number
{
  s32 AtlasRow;
  s32 AtlasCol;
};

// struct sprites
// {

// };

struct game_state
{
  struct memory_arena MemoryArena;
  struct level *Level;
  struct tile_map *TileMaps;
  struct board_timer *BoardTimer;
  struct main_menu_state *MainMenu;
  struct board_menu_state *BoardMenu;
  struct ui_board *UIBoard;

  s32 IsInitialized;

  struct v2 PlayerPos;
  struct v2 dPlayer;
  s32 PlayerWidth;
  s32 PlayerHeight;
  s32 PlayerTileMapX;
  s32 PlayerTileMapY;
  s32 PlayerTileMapZ;
  // TODO: does this belong to a game state?
  s32 TutorialObjTilePosX;
  s32 TutorialObjTilePosY;

  s32 IsMainMenu;
  s32 IsDualNBackMode;
  // TODO: this is only about board state
  // should this be a part of game state
  // or a separate board state?
  s32 IsBetweenLevels;

  struct loaded_bmp SpritesAtlas;
  // struct sprites Sprites;
  struct sprite_number SpriteNumber;

  // struct loaded_wav TestWav;
  // void *SoundMixerChannel0;
  // void *SoundMixerChannel1;
  // struct playing_sound *FirstPlayingSound;
  // struct playing_sound *FirstFreePlayingSound;

  // u32 TestSoundSamplesIndex;
  // // s32 ToneHz;
};

struct temp_state
{
  s32 IsInitialized;
  struct memory_arena TempArena;
  struct loaded_wav TestWav;
  void *SoundMixerChannel0;
  void *SoundMixerChannel1;
  struct playing_sound *FirstPlayingSound;
  struct playing_sound *FirstFreePlayingSound;
};


void
ClearScreen(struct win32_screen_buffer *ScreenBuffer)
{
  // Byte order:
  // 0x 00 FF 00 00 <- Red
  // 0x 00 00 FF 00 <- Green
  // 0x 00 00 00 FF <- Blue
  // F.e. u32 Color = 0x0000FF;
  u8 *Row = (u8 *)ScreenBuffer->Memory;
  for(s32 Y = 0; Y < ScreenBuffer->Height; Y++)
  {
    u32 *Pixel = (u32 *)Row;
    for(s32 X = 0; X < ScreenBuffer->Width; X++)
    {
      *Pixel++ = 0xFFFFFFFF;
    }
    Row += ScreenBuffer->Pitch;
  }
}

void
DrawRect(struct win32_screen_buffer *ScreenBuffer, s32 StartX, s32 StartY, s32 Width, s32 Height, u32 Color)
{
  if(StartX < 0)
  {
    StartX = 0;
  }
  if(StartY < 0)
  {
    StartY = 0;
  }
  if(StartX > ScreenBuffer->Width)
  {
    StartX = ScreenBuffer->Width;
  }
  if(StartY> ScreenBuffer->Height)
  {
    StartY = ScreenBuffer->Height;
  }

  u8 *Row = (u8 *)ScreenBuffer->Memory +
  StartX*ScreenBuffer->BytesPerPixel +
  StartY*ScreenBuffer->Pitch;
  for(s32 Y = StartY; Y < StartY+Height; Y++)
  {
    u32 *Pixel = (u32 *)Row;
    for(s32 X = StartX; X < StartX+Width; X++)
    {
      *Pixel = Color;
      Pixel++;
    }
    Row += ScreenBuffer->Pitch;
  }
}

// TODO: Add border width?
void
DrawRectNoFill(struct win32_screen_buffer *ScreenBuffer, s32 StartX, s32 StartY, s32 Width, s32 Height, u32 Color)
{
  if(StartX < 0)
  {
    StartX = 0;
  }
  if(StartY < 0)
  {
    StartY = 0;
  }
  if(StartX > ScreenBuffer->Width)
  {
    StartX = ScreenBuffer->Width;
  }
  if(StartY> ScreenBuffer->Height)
  {
    StartY = ScreenBuffer->Height;
  }

  u8 *Row = (u8 *)ScreenBuffer->Memory +
  StartX*ScreenBuffer->BytesPerPixel +
  StartY*ScreenBuffer->Pitch;
  for(s32 Y = StartY; Y < StartY+Height; Y++)
  {
    u32 *Pixel = (u32 *)Row;
    for(s32 X = StartX; X < StartX+Width; X++)
    {
      if(
        ((X == StartX) || (X == (StartX+Width-1))) ||
        ((Y == StartY) || (Y == (StartY+Height-1)))
      )
      {
        *Pixel = Color;
      }
      Pixel++;
    }
    Row += ScreenBuffer->Pitch;
  }
}

void
DrawBmp(
  struct win32_screen_buffer *ScreenBuffer, u32 *BmpPixels, s32 StartX, s32 StartY, s32 Width, s32 Height
)
{
  if(StartX < 0)
  {
    StartX = 0;
  }
  if(StartY < 0)
  {
    StartY = 0;
  }
  if(StartX > ScreenBuffer->Width)
  {
    StartX = ScreenBuffer->Width;
  }
  if(StartY> ScreenBuffer->Height)
  {
    StartY = ScreenBuffer->Height;
  }

  // The image is stored in memory in the following way:
  // f.e. it's a 4 by 4 pixel image
  // Red   White
  // White Green
  // Memory window:
  //      white         green         red           white
  // | FF FF FF FF | 00 FF 00 FF | 00 00 FF FF | FF FF FF FF |
  // It's also in BGRA order for each pixel

  // BMP image is upside down, need to reverse
  // Start from the last row
  // Example: bmp image is width=4, height=3
  // ScreenBuffer->Memory is casted to a byte (points to the upper left screen corner f.e.)
  // Preadvance it by adding (Height-1)*(ScreenBuffer->Pitch)
  // where Height-1 is the last row of an image and Pitch is ScreenWidth*BytesPerPixel (f.e. 600*4)
  // representing one full row of a screen pixels
  u8 *Row = (u8 *)ScreenBuffer->Memory +
    StartX * ScreenBuffer->BytesPerPixel +
    ((StartY + Height-1)*(ScreenBuffer->Pitch));
  for(s32 Y = StartY; Y < StartY+Height; Y++)
  {
    u32 *Pixel = (u32 *)Row;
    for(s32 X = StartX; X < StartX+Width; X++)
    {
      // Colors may not correspond (in case it's in BRG f.e.), but
      // it doesn't matter here

      // In order to use these values for calculations
      // we need to shift it and then mask
      // Masking with 0x000000FF, 0x0000FF00, 0x00FF0000 won't work in this case
      f32 SourceAlpha = (f32)((*BmpPixels >> 24) & 0xFF) / 255.0f; // 0xFF is 255 (0000 0000 0000 0000 1111 1111)
      f32 SourceRed = (f32)((*BmpPixels >> 16) & 0xFF);
      f32 SourceGreen = (f32)((*BmpPixels >> 8) & 0xFF);
      f32 SourceBlue = (f32)((*BmpPixels >> 0) & 0xFF);

      f32 DestRed = (f32)((*Pixel >> 16) & 0xFF);
      f32 DestGreen = (f32)((*Pixel >> 8) & 0xFF);
      f32 DestBlue = (f32)((*Pixel >> 0) & 0xFF);

      f32 ResultRed = (1.0f - SourceAlpha) * DestRed + SourceRed * SourceAlpha;
      f32 ResultGreen = (1.0f - SourceAlpha) * DestGreen + SourceGreen * SourceAlpha;
      f32 ResultBlue = (1.0f - SourceAlpha) * DestBlue + SourceBlue * SourceAlpha;

      u32 Result = ((u32)(ResultRed + 0.5f) << 16) |
        ((u32)(ResultGreen + 0.5f) << 8) |
        ((u32)(ResultBlue + 0.5f) << 0);

      *Pixel = Result;
      Pixel++;
      BmpPixels++;
    }
    Row -= ScreenBuffer->Pitch;
  }
}

#define TILE_MAP_WIDTH 16
#define TILE_MAP_HEIGHT 16
#define TILE_WIDTH 32
#define TILE_HEIGHT 32
#define BMP_ATLAS_WIDTH 1024
// #define BMP_ATLAS_WIDTH 96
#define BMP_ATLAS_HEIGHT 1024

void
TestDrawBmp(
  struct win32_screen_buffer *ScreenBuffer, u32 *BmpPixels,
  s32 ImgRow, s32 ImgCol,
  s32 StartX, s32 StartY, s32 Width, s32 Height
)
{

  // Get tile from tile atlas using offset
  // 1 - first column, 1 - first row (both start from 0)
  // Bmp for N-th tile
  // f.e. N = 3
  // Then our bmp for tile is from *BmpPixels[X + Y*TILE_ATLAS_WIDTH]
  // *BmpPixels[1 + 1*1024]
  u32 OffsetPixX = ImgCol * TILE_WIDTH;
  u32 OffsetPixY = ImgRow * TILE_HEIGHT;

  u32 *BmpTilePixels = BmpPixels;
  u32 NewBmpPixels[TILE_WIDTH*TILE_HEIGHT] = {0};

  s32 PixelCounter = 0;

  for(
    u32 *CurrPixel = BmpTilePixels + (OffsetPixX + OffsetPixY * BMP_ATLAS_WIDTH);
    PixelCounter < TILE_WIDTH*TILE_HEIGHT;
    PixelCounter++
  )
  {
    // When TILE_WIDTH is a multiple of pixel counter
    // it means we reached the end of the current image row
    // then advance to the next pixel row of the image
    if(PixelCounter && ((PixelCounter % TILE_WIDTH) == 0))
    {
      // Add the whole atlas width, but compensate the tile width
      // as we were already at the end of the current image row
      // f.e. when drawing White
      //      white         green                        red           white
      // | FF FF FF FF | 00 FF 00 FF | .. .. .. ..  | 00 00 FF FF | FF FF FF FF |
      //               ^                                          ^
      //         move from here                                to here
      CurrPixel += (BMP_ATLAS_WIDTH - TILE_WIDTH);
    }
    NewBmpPixels[PixelCounter] = *CurrPixel;
    CurrPixel++;
  }

  u32 *TestBmpPixels = NewBmpPixels;

  if(StartX < 0)
  {
    StartX = 0;
  }
  if(StartY < 0)
  {
    StartY = 0;
  }
  if(StartX > ScreenBuffer->Width)
  {
    StartX = ScreenBuffer->Width;
  }
  if(StartY> ScreenBuffer->Height)
  {
    StartY = ScreenBuffer->Height;
  }

  // The image is stored in memory in the following way:
  // f.e. it's a 4 by 4 pixel image
  // Red   White
  // White Green
  // Memory window:
  //      white         green         red           white
  // | FF FF FF FF | 00 FF 00 FF | 00 00 FF FF | FF FF FF FF |
  // It's also in BGRA order for each pixel

  // BMP image is upside down, need to reverse
  // Start from the last row
  // Example: bmp image is width=4, height=3
  // ScreenBuffer->Memory is casted to a byte (points to the upper left screen corner f.e.)
  // Preadvance it by adding (Height-1)*(ScreenBuffer->Pitch)
  // where Height-1 is the last row of an image and Pitch is ScreenWidth*BytesPerPixel (f.e. 600*4)
  // representing one full row of a screen pixels
  u8 *Row = (u8 *)ScreenBuffer->Memory +
    StartX * ScreenBuffer->BytesPerPixel +
    ((StartY + Height-1)*(ScreenBuffer->Pitch));
  for(s32 Y = StartY; Y < StartY+Height; Y++)
  {
    u32 *Pixel = (u32 *)Row;
    for(s32 X = StartX; X < StartX+Width; X++)
    {
      // Colors may not correspond (in case it's in BRG f.e.), but
      // it doesn't matter here

      // In order to use these values for calculations
      // we need to shift it and then mask
      // Masking with 0x000000FF, 0x0000FF00, 0x00FF0000 won't work in this case
      f32 SourceAlpha = (f32)((*TestBmpPixels >> 24) & 0xFF) / 255.0f; // 0xFF is 255 (0000 0000 0000 0000 1111 1111)
      f32 SourceRed = (f32)((*TestBmpPixels >> 16) & 0xFF);
      f32 SourceGreen = (f32)((*TestBmpPixels >> 8) & 0xFF);
      f32 SourceBlue = (f32)((*TestBmpPixels >> 0) & 0xFF);

      f32 DestRed = (f32)((*Pixel >> 16) & 0xFF);
      f32 DestGreen = (f32)((*Pixel >> 8) & 0xFF);
      f32 DestBlue = (f32)((*Pixel >> 0) & 0xFF);

      f32 ResultRed = (1.0f - SourceAlpha) * DestRed + SourceRed * SourceAlpha;
      f32 ResultGreen = (1.0f - SourceAlpha) * DestGreen + SourceGreen * SourceAlpha;
      f32 ResultBlue = (1.0f - SourceAlpha) * DestBlue + SourceBlue * SourceAlpha;

      u32 Result = ((u32)(ResultRed + 0.5f) << 16) |
        ((u32)(ResultGreen + 0.5f) << 8) |
        ((u32)(ResultBlue + 0.5f) << 0);

      *Pixel = Result;
      Pixel++;
      TestBmpPixels++;
    }
    Row -= ScreenBuffer->Pitch;
  }
}

void
DebugDrawLine(struct win32_screen_buffer *ScreenBuffer, s32 x1, s32 y1, s32 x2, s32 y2, s32 Width, u32 Color)
{
  s32 dx = x2 - x1;
  s32 dy = y2 - y1;
  s32 Absdx = Abs(dx);
  s32 Absdy = Abs(dy);

  s32 x = x1;
  s32 y = y1;
  DrawRect(ScreenBuffer, x1, y1, Width, Width, Color);
  // Slope < 1
  if(Absdx  > Absdy)
  {
    s32 Delta = 2 * Absdy - Absdx;

    for(s32 Ind = 0; Ind < Absdx; Ind++)
    {
      x = dx < 0 ? x-1 : x+1;
      if(Delta < 0)
      {
        Delta = Delta + 2 * Absdy;
      }
      else
      {
        y = dy < 0 ? y-1 : y+1;
        Delta = Delta + (2*Absdy - 2*Absdx);
      }
      DrawRect(ScreenBuffer, x, y, Width, Width, Color);
    }
  }
  else // Slope is >= 1
  {
    s32 Delta = 2 * Absdx - Absdy;

    for(s32 Ind = 0; Ind < Absdy; Ind++)
    {
      y = dy < 0 ? y-1 : y+1;
      if(Delta < 0)
      {
        Delta = Delta + 2 * Absdx;
      }
      else
      {
        x = dx < 0 ? x-1 : x+1;
        Delta = Delta + (2 * Absdx) - (2 * Absdy);
      }
      DrawRect(ScreenBuffer, x, y, Width, Width, Color);
    }
  }
}

struct loaded_bmp
DebugLoadBmp(char *Filename)
{
  struct loaded_bmp Result = {0};
  struct debug_read_file_result File;
  File = DebugReadEntireFile(Filename);
  if(File.ContentSize)
  {
    // Treat as bitmap
    struct bitmap_header *BmpContent = (struct bitmap_header *)File.Content;
    // 42 4D (BM)
    // Each 1 hex is 4 bits 4+4 4+4 = 16bit => 2 bytes for BM
    // BitmapOffset = 138 (bytes from the beggining of the file is where the image stored)
    // 00 FF 00 FF is the image data (in case of debug.bmp)
    // (u8 *)File.Content casts to a byte, then add Offset of 138 bytes, then store it as u32*
    // because pixel value is RGBA
    Result.Pixels = (u32 *)((u8 *)File.Content + BmpContent->BitmapOffset);
    Result.Width = BmpContent->Width;
    Result.Height = BmpContent->Height;
    // GameState->LoadedBmp = LoadedBmp;
    // GameState->BmpPixels = (u32 *)((u8 *)File.Content + BmpContent->BitmapOffset);
  }
  return Result;
}

struct loaded_wav
DebugLoadWav(char *Filename)
{
  struct loaded_wav Result = {0};
  struct debug_read_file_result File;
  File = DebugReadEntireFile(Filename);
  if(File.ContentSize)
  {
    struct riff_header *RiffHeader = (struct riff_header *)File.Content;
    u32 RiffId = ((u32)('R')) | ((u32)('I') << 8) | ((u32)('F') << 16) | ((u32)('F') << 24);
    u32 WaveId = ((u32)('W')) | ((u32)('A') << 8) | ((u32)('V') << 16) | ((u32)('E') << 24);
    u32 FmtId = ((u32)('f')) | ((u32)('m') << 8) | ((u32)('t') << 16) | ((u32)(' ') << 24);
    u32 DataId = ((u32)('d')) | ((u32)('a') << 8) | ((u32)('t') << 16) | ((u32)('a') << 24);
    s32 IsPCMTag = 1;

    Assert(RiffHeader->RiffId == RiffId);
    Assert(RiffHeader->WaveId == WaveId);

    struct fmt_chunk *FmtChunk = (struct fmt_chunk *)(RiffHeader + 1);
    Assert(FmtChunk->FmtId == FmtId);

    struct wav_data_chunk *FmtChunkData = (struct wav_data_chunk *)(FmtChunk + 1);
    Assert(FmtChunkData->FormatTag == IsPCMTag);
    Assert(FmtChunkData->SamplesPerSec == 48000);
    Assert(FmtChunkData->BitsPerSample == 16);
    s16 ChannelsCount = FmtChunkData->NumberOfChannels;

    // Compensate for pad byte: 0 or 1  Padding byte if n is odd
    u32 FmtDataSize = (FmtChunk->FmtChunkSize + 1) & ~1;
    struct fmt_chunk *FmtDataHeader = (struct fmt_chunk *)((u8 *)FmtChunk + sizeof(struct fmt_chunk) + FmtDataSize);
    Assert(FmtDataHeader->FmtId == DataId);

    s16 *WavSampleData = (s16 *)((u8 *)FmtDataHeader + sizeof(struct fmt_chunk));
    u32 SampleDataSize = FmtDataHeader->FmtChunkSize;

    if(ChannelsCount == 1)
    {
      Result.Samples[0] = WavSampleData;
      Result.Samples[1] = 0;
    }
    else if(ChannelsCount == 2)
    {
      Result.Samples[0] = WavSampleData;
      Result.Samples[1] = WavSampleData + SampleDataSize;
    }
    else
    {
      Assert(!"Wrong number of channels");
    }

    // TODO: Should be deinterleaved? (to be able to SIMD it?)
    // For now just write left channel and ignore right
    s32 SamplesCount = SampleDataSize / (ChannelsCount * sizeof(s16));
    Result.SamplesCount = SamplesCount;
    // Turns L1R1 L2R2 L3R3 ...
    // into  L1L2 L3L4 ... R1R? ... (but R does not come in proper order)
    for(s32 SampleIndex = 0; SampleIndex < SamplesCount; SampleIndex++)
    {
      s16 Source = WavSampleData[2*SampleIndex];
      WavSampleData[2*SampleIndex] = WavSampleData[SampleIndex];
      WavSampleData[SampleIndex] = Source;
    }

  }
  return Result;
}

void
SaveGame(struct game_state *GameState, struct level *Level)
{
  // Backup old save
  char *ExistingName = "save.txt";
  char *NewName = "save_bak.txt";
  s32 CopyResultSuccess = CopyEntireFile(ExistingName, NewName);

  // Create new save
  if(CopyResultSuccess)
  {
    // NOTE: in case there will be more values (which there probably won't)
    // consider a struct with char* and s32* elements
    // and then have an array of pointers to this struct
    // then iterate it in a loop and call "NumToStr" and "AppendToStr" there.
    // For now it's fine to have it like this:
    char Msg1[] = "PlayerTileMapY=";
    char Msg2[] = "PlayerTileMapZ=";
    char Msg3[] = "Mode=";
    char Msg4[] = "Tutorial=";
    char Msg5[] = "AvailableLvl=";

    #define MSG_LEN 128
    char SaveMsg[MSG_LEN];
    char NumStr[ITOA_STR_LEN];
    s32 GlobalMsgInd = 0;

    s32 PlayerTileMapY = GameState->PlayerTileMapY;
    s32 PlayerTileMapZ = GameState->PlayerTileMapZ;
    s32 Mode = Level->Mode;
    s32 Tutorial = Level->Tutorial;
    s32 AvailableLvl = Level->AvailableCount;

    if(PlayerTileMapY < 0)
    {
      PlayerTileMapY = 0;
    }
    if(PlayerTileMapZ < 0)
    {
      PlayerTileMapZ = 0;
    }
    if(Mode < 0)
    {
      Mode = 0;
    }
    if(Tutorial < 0)
    {
      Tutorial = 0;
    }
    if(AvailableLvl < 0)
    {
      AvailableLvl = 0;
    }

    AppendToStr(SaveMsg, Msg1, &GlobalMsgInd);
    NumToStr(NumStr, PlayerTileMapY);
    AppendToStr(SaveMsg, NumStr, &GlobalMsgInd);
    SaveMsg[GlobalMsgInd++] = '\n';

    AppendToStr(SaveMsg, Msg2, &GlobalMsgInd);
    NumToStr(NumStr, PlayerTileMapY);
    AppendToStr(SaveMsg, NumStr, &GlobalMsgInd);
    SaveMsg[GlobalMsgInd++] = '\n';

    AppendToStr(SaveMsg, Msg3, &GlobalMsgInd);
    NumToStr(NumStr, Mode);
    AppendToStr(SaveMsg, NumStr, &GlobalMsgInd);
    SaveMsg[GlobalMsgInd++] = '\n';

    AppendToStr(SaveMsg, Msg4, &GlobalMsgInd);
    NumToStr(NumStr, Tutorial);
    AppendToStr(SaveMsg, NumStr, &GlobalMsgInd);
    SaveMsg[GlobalMsgInd++] = '\n';

    AppendToStr(SaveMsg, Msg5, &GlobalMsgInd);
    NumToStr(NumStr, AvailableLvl);
    AppendToStr(SaveMsg, NumStr, &GlobalMsgInd);
    SaveMsg[GlobalMsgInd++] = '\0';

    // Pass an actual byte size of the message
    // that is GlobalMsgInd counter, since char is 1 bit
    WriteEntireFile(ExistingName, SaveMsg, (u32)GlobalMsgInd);
  }
}

s32
StrCompare(char *Str1, char *Str2)
{
  while(*Str1++ == *Str2++)
  {
    if(*Str1 == '\0')
    {
      return 0;
    }
  }

  return (*Str1 - *Str2);
}

s32
StrToNum(char *Str, s32 *NumLength)
{
  s32 Result = 0;
  s32 Sign = 1;
  if(*Str == '-')
  {
    Sign = -1;
    Str++;
  }

  while((*Str >= '0') && (*Str <= '9'))
  {
    Result = Result * 10 + (*Str - '0');
    (*NumLength)++;
    Str++;
  }

  return Result;
}

s32
StrLength(char *Str)
{
  s32 Result = 0;
  while(*Str++)
  {
    Result++;
  }

  return Result;
}

void
LoadGame(struct game_state *GameState, struct level *Level)
{
  char *Filename = "save.txt";
  char *Backupname = "save_bak.txt";
  struct debug_read_file_result File;
  File = DebugReadEntireFile(Filename);
  if(!File.ContentSize)
  {
    File = DebugReadEntireFile(Backupname);
  }
  if(File.ContentSize)
  {
    char *Content = (char *)File.Content;
    s32 WriteCursor = 0;
    s32 LineNumber = 1;
    // TODO: assume there are only four values for now
    // might want to make more general solution,
    // to support any number of values
    char *PlayerTileMapY = "PlayerTileMapY=";
    s32 PlayerTileMapYLen = StrLength(PlayerTileMapY);
    s32 PlayerTileMapYVal = GameState->PlayerTileMapY;
    char *PlayerTileMapZ = "PlayerTileMapZ=";
    s32 PlayerTileMapZLen = StrLength(PlayerTileMapZ);
    s32 PlayerTileMapZVal = GameState->PlayerTileMapZ;
    char *Mode = "Mode=";
    s32 ModeLen = StrLength(Mode);
    s32 ModeVal = Level->Mode;
    char *Tutorial = "Tutorial=";
    s32 TutorialLen = StrLength(Mode);
    s32 TutorialVal = Level->Mode;
    char *AvailableLvl = "AvailableLvl=";
    s32 AvailableLvlLen = StrLength(Mode);
    s32 AvailableLvlVal = Level->AvailableCount;

    s32 NumLength = 0;

    if(StrCompare(PlayerTileMapY, Content) == 0)
    {
      Content = Content + PlayerTileMapYLen;
      if(Content)
      {
        PlayerTileMapYVal = StrToNum(Content, &NumLength);
        Content += NumLength;
        NumLength = 0;
        while(*Content && ((*Content < 'A') || (*Content > 'z')))
        {
          Content++;
        }
      }
    }
    if(StrCompare(PlayerTileMapZ, Content) == 0)
    {
      Content = Content + PlayerTileMapZLen;
      if(Content)
      {
        PlayerTileMapZVal = StrToNum(Content, &NumLength);
        Content += NumLength;
        NumLength = 0;
        while(*Content && ((*Content < 'A') || (*Content > 'z')))
        {
          Content++;
        }
      }
    }
    if(StrCompare(Mode, Content) == 0)
    {
      Content = Content + ModeLen;
      if(Content)
      {
        ModeVal = StrToNum(Content, &NumLength);
        Content += NumLength;
        NumLength = 0;
        while(*Content && ((*Content < 'A') || (*Content > 'z')))
        {
          Content++;
        }
      }
    }
    if(StrCompare(Tutorial, Content) == 0)
    {
      Content = Content + TutorialLen;
      if(Content)
      {
        TutorialVal = StrToNum(Content, &NumLength);
        Content += NumLength;
        NumLength = 0;
        while(*Content && ((*Content < 'A') || (*Content > 'z')))
        {
          Content++;
        }
      }
    }
    if(StrCompare(AvailableLvl, Content) == 0)
    {
      Content = Content + AvailableLvlLen;
      if(Content)
      {
        AvailableLvlVal = StrToNum(Content, &NumLength);
        Content += NumLength;
        NumLength = 0;
        while(*Content && ((*Content < 'A') || (*Content > 'z')))
        {
          Content++;
        }
      }
    }

    // Ensure this level is available, if not move player
    // to the last available
    if(PlayerTileMapZVal >= Level->AvailableCount)
    {
      if(!Level->AvailableCount)
      {
        PlayerTileMapZVal = 0;
      }
      else
      {
        PlayerTileMapZVal = Level->AvailableCount - 1;
      }
    }

    GameState->PlayerTileMapY = PlayerTileMapYVal;
    GameState->PlayerTileMapZ = PlayerTileMapZVal;
    Level->NValue = GameState->PlayerTileMapZ + 1;
    Level->Mode = ModeVal;
    Level->Tutorial = TutorialVal;
    Level->AvailableCount = AvailableLvlVal;
    // TODO: Also save player's x, y coords?
    // for now always use default
    if(PlayerTileMapZVal || !PlayerTileMapYVal)
    {
      GameState->PlayerPos.X = (f32)TILE_WIDTH * 5.0f;
      GameState->PlayerPos.Y = (f32)TILE_HEIGHT * 5.0f;
    }

    DebugFreeFileMemory(File.Content);
  }
}

void
DrawTile(struct win32_screen_buffer *ScreenBuffer, struct game_state *GameState, u32 *CurrentTileMap)
{
  u32 Walkable = 0xffffff;
  u32 NonWalkable = 0x444e6b;
  u32 StairsUp = 0x435d91;
  u32 StairsDown = 0x428ab3;
  u32 Door = 0xc69fa5;
  u32 Npc = 0xf2d3ab;
  for(s32 Row = 0; Row < TILE_MAP_HEIGHT; Row++)
  {
    for(s32 Col = 0; Col < TILE_MAP_WIDTH; Col++)
    {
      s32 StartX = TILE_WIDTH * Col;
      s32 StartY = TILE_HEIGHT * Row;

      switch(CurrentTileMap[Col + TILE_MAP_WIDTH * Row])
      {
        case -3:
        {
          // Tree02
          TestDrawBmp(
            ScreenBuffer, GameState->SpritesAtlas.Pixels,
            1, 2,
            StartX, StartY, TILE_WIDTH, TILE_HEIGHT
          );
        } break;
        case -2:
        {
          // Tree01
          TestDrawBmp(
            ScreenBuffer, GameState->SpritesAtlas.Pixels,
            1, 1,
            StartX, StartY, TILE_WIDTH, TILE_HEIGHT
          );
        } break;
        case -1:
        {
          // Grass
          TestDrawBmp(
            ScreenBuffer, GameState->SpritesAtlas.Pixels,
            1, 0,
            StartX, StartY, TILE_WIDTH, TILE_HEIGHT
          );
        } break;
        case 1:
        {
          DrawRect(ScreenBuffer, StartX, StartY, TILE_WIDTH, TILE_HEIGHT, NonWalkable);
        } break;
        case 2:
        {
          DrawRect(ScreenBuffer, StartX, StartY, TILE_WIDTH, TILE_HEIGHT, StairsDown);
        } break;
        case 3:
        {
          DrawRect(ScreenBuffer, StartX, StartY, TILE_WIDTH, TILE_HEIGHT, StairsUp);
        } break;
        case 4:
        {
          DrawRect(ScreenBuffer, StartX, StartY, TILE_WIDTH, TILE_HEIGHT, Door);
        } break;
        case 9:
        {
          // Orb
          TestDrawBmp(
            ScreenBuffer, GameState->SpritesAtlas.Pixels,
            0, 1,
            StartX, StartY, TILE_WIDTH, TILE_HEIGHT
          );
        } break;
        default:
        {
          DrawRect(ScreenBuffer, StartX, StartY, TILE_WIDTH, TILE_HEIGHT, Walkable);
        }
      }
    }
  }
}

// void
// DebugDrawTile(
//   struct win32_screen_buffer *ScreenBuffer, struct game_state *GameState, u32 *CurrentTileMap,
//   s32 TileMapSize
// )
// {
//   u32 Walkable = 0xffffff;
//   u32 NonWalkable = 0x444e6b;
//   u32 StairsUp = 0x435d91;
//   u32 StairsDown = 0x428ab3;
//   u32 Door = 0xc69fa5;
//   u32 Npc = 0xf2d3ab;
//   for(s32 Row = 0; Row < TileMapSize; Row++)
//   {
//     for(s32 Col = 0; Col < TileMapSize; Col++)
//     {
//       s32 StartX = TILE_WIDTH * Col;
//       s32 StartY = TILE_HEIGHT * Row;

//       switch(CurrentTileMap[Col + TileMapSize * Row])
//       {
//         case -3:
//         {
//           DrawBmp(ScreenBuffer, GameState->Tree02Bmp.Pixels, StartX, StartY, TILE_WIDTH, TILE_HEIGHT);
//         } break;
//         case -2:
//         {
//           DrawBmp(ScreenBuffer, GameState->Tree01Bmp.Pixels, StartX, StartY, TILE_WIDTH, TILE_HEIGHT);
//         } break;
//         case -1:
//         {
//           DrawBmp(ScreenBuffer, GameState->GrassBmp.Pixels, StartX, StartY, TILE_WIDTH, TILE_HEIGHT);
//         } break;
//         case 1:
//         {
//           DrawRect(ScreenBuffer, StartX, StartY, TILE_WIDTH, TILE_HEIGHT, NonWalkable);
//         } break;
//         case 2:
//         {
//           DrawRect(ScreenBuffer, StartX, StartY, TILE_WIDTH, TILE_HEIGHT, StairsDown);
//         } break;
//         case 3:
//         {
//           DrawRect(ScreenBuffer, StartX, StartY, TILE_WIDTH, TILE_HEIGHT, StairsUp);
//         } break;
//         case 4:
//         {
//           DrawRect(ScreenBuffer, StartX, StartY, TILE_WIDTH, TILE_HEIGHT, Door);
//         } break;
//         case 9:
//         {
//           DrawBmp(ScreenBuffer, GameState->OrbBmp.Pixels, StartX, StartY, TILE_WIDTH, TILE_HEIGHT);
//         } break;
//         default:
//         {
//           DrawRect(ScreenBuffer, StartX, StartY, TILE_WIDTH, TILE_HEIGHT, Walkable);
//         }
//       }
//     }
//   }
// }

s32
IsButtonClicked(
  struct win32_screen_buffer *ScreenBuffer, struct game_input *Input, struct menu_button *Btn,
  s32 BtnId
)
{
  s32 IsHoverOver = (
    (Input->MousePosX >= Btn->StartX) &&
    (Input->MousePosX <= (Btn->StartX + Btn->Width)) &&

    (Input->MousePosY >= Btn->StartY) &&
    (Input->MousePosY <= (Btn->StartY + Btn->Height))
  );

  s32 IsOnBtn = (Input->MouseButtons[0].X >= Btn->StartX) &&
  (
    Input->MouseButtons[0].X <= (Btn->StartX + Btn->Width)
  ) &&
  (
    Input->MouseButtons[0].Y >= Btn->StartY
  ) &&
  (
    Input->MouseButtons[0].Y <= (Btn->StartY + Btn->Height)
  );

  if(IsHoverOver)
  {
    // Highlight button
    // TODO: Use sprite
    DrawRectNoFill(
      ScreenBuffer, Btn->StartX, Btn->StartY,
      Btn->Width, Btn->Height, 0x000000
    );
  }

  return (Input->Buttons[BtnId].IsDown || (IsOnBtn && Input->MouseButtons[0].IsDown));
}

void
DrawMainMenu(struct win32_screen_buffer *ScreenBuffer, struct game_state *GameState)
{
  struct main_menu_state *MainMenu = GameState->MainMenu;
  struct menu_button *StartBtn = MainMenu->Buttons[0];
  StartBtn->Width = 200;
  StartBtn->Height = 50;
  StartBtn->Color = 0x00FF00;
  StartBtn->StartX = ScreenBuffer->Width/2 - StartBtn->Width/2;
  StartBtn->StartY = ScreenBuffer->Height/2 - StartBtn->Height/2;

  struct menu_button *ExitBtn = GameState->MainMenu->Buttons[1];
  ExitBtn->Width = 200;
  ExitBtn->Height = 50;
  ExitBtn->Color = 0xFF0000;
  ExitBtn->StartX = StartBtn->StartX;
  ExitBtn->StartY = StartBtn->StartY + ExitBtn->Height + 10;

  u8 *Row = (u8 *)ScreenBuffer->Memory;
  for(s32 Y = 0; Y < ScreenBuffer->Height; Y++)
  {
    u32 *Pixel = (u32 *)Row;
    for(s32 X = 0; X < ScreenBuffer->Width; X++)
    {
      *Pixel = 0x000000;
      Pixel++;
    }
    Row += ScreenBuffer->Pitch;
  }

  // Start btn
  DrawRect(
    ScreenBuffer,
    StartBtn->StartX, StartBtn->StartY,
    StartBtn->Width, StartBtn->Height, StartBtn->Color
  );
  // Exit btn
  DrawRect(
    ScreenBuffer,
    ExitBtn->StartX, ExitBtn->StartY,
    ExitBtn->Width, ExitBtn->Height, ExitBtn->Color
  );
}

#define BOARD_WIDTH 3
#define BOARD_HEIGHT 3

#define BOARD_TILE_WIDTH 96
#define BOARD_TILE_HEIGHT 96

#define MENU_TILE_WIDTH 64
#define MENU_TILE_HEIGHT 64

void
DrawBoard(struct win32_screen_buffer *ScreenBuffer, struct game_state *GameState, s32 *Board, u32 Mode)
{
  // Offset between main board tiles
  // and menu buttons
  s32 OffsetBoardTiles = 1;
  s32 OffsetMenuButtons = 5;

  // Screen offset
  // Place the whole board with some offset to the screen
  s32 MainBoardTilesWidth = (BOARD_TILE_WIDTH * BOARD_WIDTH) + (OffsetBoardTiles * 2);
  s32 OffsetX = (ScreenBuffer->Width - MainBoardTilesWidth) / 6;
  s32 OffsetY = 32;

  // Mode
  // 1 - one n back
  // 2 - two n back, etc.

  u32 Color = 0xf0f6e8;
  // Board is always 3x3 ?
  // Might change for other game options
  for(s32 Row = 0; Row < BOARD_HEIGHT; Row++)
  {
    for(s32 Col = 0; Col < BOARD_WIDTH; Col++)
    {
      u32 CurrVal = Board[Col + Row*BOARD_WIDTH];
      s32 StartX = (Col * BOARD_TILE_WIDTH) + OffsetX;
      s32 StartY = (Row * BOARD_TILE_HEIGHT) + OffsetY;

      //
      switch(CurrVal)
      {
        case 1:
        {
          // Red
          Color = 0xFF0000;
        } break;
        case 2:
        {
          // Green
          Color = 0x00FF00;
        } break;
        case 3:
        {
          // Blue
          Color = 0x0000FF;
        } break;
        case 4:
        {
          // Yellow
          Color = 0xFFFF00;
        } break;
        case 5:
        {
          // Fuchsia
          Color = 0xFF00FF;
        } break;
        case 6:
        {
          // Aqua
          Color = 0x00FFFF;
        } break;
        case 7:
        {
          // Black
          Color = 0x000000;
        } break;
        default:
        {
          Color = 0xf0f6e8;
        } break;
      }
      DrawRect(
        ScreenBuffer,
        StartX+OffsetBoardTiles, StartY+OffsetBoardTiles,
        BOARD_TILE_WIDTH-OffsetBoardTiles, BOARD_TILE_HEIGHT-OffsetBoardTiles, Color
      );
    }
  }

  // Mode = 4;

  // Draw menu under the board
  // Depending on a game mode
  // There could be 2 to 4 (or more?) buttons
  struct board_menu_state *BoardMenu = GameState->BoardMenu;
  struct menu_button *PositionBtn = BoardMenu->Buttons[BOARD_BUTTON_POSITION];
  PositionBtn->Width = MENU_TILE_WIDTH;
  PositionBtn->Height = MENU_TILE_HEIGHT;
  PositionBtn->Color = 0xf0f6e8;
  PositionBtn->StartX = OffsetMenuButtons + OffsetX;
  PositionBtn->StartY = (BOARD_TILE_HEIGHT * BOARD_HEIGHT + OffsetMenuButtons) + OffsetY;

//   #define BOARD_TILE_WIDTH 100
// #define BOARD_TILE_HEIGHT 100

// #define MENU_TILE_WIDTH 50
// #define MENU_TILE_HEIGHT 50

  // u32 MenuColor = 0xf0f6e8;
  // s32 Menu1X = 5;
  // s32 Menu1Y = BOARD_TILE_HEIGHT * BOARD_HEIGHT + 5;

  DrawRect(
    ScreenBuffer, PositionBtn->StartX, PositionBtn->StartY,
    PositionBtn->Width, PositionBtn->Height, PositionBtn->Color
  );

  struct menu_button *SoundBtn = BoardMenu->Buttons[BOARD_BUTTON_SOUND];
  SoundBtn->Width = MENU_TILE_WIDTH;
  SoundBtn->Height = MENU_TILE_HEIGHT;
  SoundBtn->Color = 0xf0f6e8;
  SoundBtn->StartX = PositionBtn->StartX + MENU_TILE_WIDTH + OffsetMenuButtons;
  SoundBtn->StartY = PositionBtn->StartY;

  if(Mode >= 2)
  {
    DrawRect(
      ScreenBuffer, SoundBtn->StartX, SoundBtn->StartY,
      SoundBtn->Width, SoundBtn->Height, SoundBtn->Color
    );
  }

  struct menu_button *ColorBtn = BoardMenu->Buttons[BOARD_BUTTON_COLOR];
  ColorBtn->Width = MENU_TILE_WIDTH;
  ColorBtn->Height = MENU_TILE_HEIGHT;
  ColorBtn->Color = 0xf0f6e8;
  ColorBtn->StartX = SoundBtn->StartX + MENU_TILE_WIDTH + OffsetMenuButtons;
  ColorBtn->StartY = SoundBtn->StartY;

  if(Mode >= 3)
  {
    DrawRect(
      ScreenBuffer, ColorBtn->StartX, ColorBtn->StartY,
      ColorBtn->Width, ColorBtn->Height, ColorBtn->Color
    );
  }

  struct menu_button *ShapeBtn = BoardMenu->Buttons[BOARD_BUTTON_SHAPE];
  ShapeBtn->Width = MENU_TILE_WIDTH;
  ShapeBtn->Height = MENU_TILE_HEIGHT;
  ShapeBtn->Color = 0xf0f6e8;
  ShapeBtn->StartX = ColorBtn->StartX + MENU_TILE_WIDTH + OffsetMenuButtons;
  ShapeBtn->StartY = ColorBtn->StartY;

  if(Mode >= 4)
  {
    DrawRect(
      ScreenBuffer, ShapeBtn->StartX, ShapeBtn->StartY,
      ShapeBtn->Width, ShapeBtn->Height, ShapeBtn->Color
    );
  }
}

void
DrawSideMenu(
  struct win32_screen_buffer *ScreenBuffer, struct game_state *GameState,
  struct ui_board *SideMenu, s32 NValue
)
{
  SideMenu->StartX = ScreenBuffer->Width - (SideMenu->Width + SideMenu->RightOffsetX);
  DrawRect(
    ScreenBuffer, SideMenu->StartX, SideMenu->TopOffsetY,
    SideMenu->Width, SideMenu->Height, 0x00ff00
  );
  // TODO: It will only draw up to N=9 number
  // should be able to draw any number?
  TestDrawBmp(
    ScreenBuffer,
    GameState->SpritesAtlas.Pixels,
    GameState->SpriteNumber.AtlasRow, NValue,
    SideMenu->StartX, SideMenu->TopOffsetY,
    TILE_WIDTH, TILE_HEIGHT
  );
}

s32
GetCurrentTileValue(u32 *TileMap, s32 X, s32 Y)
{
  s32 Result = TileMap[X + TILE_MAP_WIDTH*Y];
  return Result;
}

s32
DebugGetCurrentTileValue(u32 TileMapWidth, u32 *TileMap, s32 X, s32 Y)
{
  s32 Result = TileMap[X + TileMapWidth*Y];
  return Result;
}

void
SetCurrentTileValue(
  struct tile_map *TileMap, s32 CurrTileVal, s32 X, s32 Y
)
{
  TileMap->Tiles[X + Y*TILE_MAP_WIDTH] = CurrTileVal;
}

// u32 *
// GetTileMapArrByXYZ(struct tile_map *TileMaps, u32 X, u32 Y, u32 Z)
// {
//   u32 *Result = 0;
//   u32 Ind = X + Y*TILE_MAP_ARR_X + Z*TILE_MAP_ARR_X*TILE_MAP_ARR_Y;
//   Result = TileMaps[Ind].Tiles;
//   // Result = (&TileMaps[Ind])->Tiles;

//   // return Result;
//   return TileMaps[Ind].Tiles;
// }

struct tile_map *
GetTileMapByXYZ(struct tile_map *TileMaps, u32 X, u32 Y, u32 Z)
{
  struct tile_map *Result = 0;
  u32 Ind = X + Y*TILE_MAP_ARR_X + Z*TILE_MAP_ARR_X*TILE_MAP_ARR_Y;
  Result = &TileMaps[Ind];

  return Result;
}


void
CopyTilesArray(s32 *Source, s32 *Dest, u32 Width, u32 Height)
{
  for(u32 Y = 0; Y < Height; Y++)
  {
    for(u32 X = 0; X < Width; X++)
    {
      Dest[X + Y*Width] = Source[X + Y*Width];
    }
  }
}

s32 *
GenerateTowerLevel(s32 *Result, s32 IsLastLevel)
{
  // s32 Example[TILE_MAP_HEIGHT][TILE_MAP_WIDTH] = {
  //   {1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1},
  //   {1, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  1},
  //   {1, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  1},
  //   {1, 0, 0, 0,  0, 0, 9, 0,  0, 0, 0, 0,  0, 0, 0, 0,  1},

  //   {1, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 2, 0, 0,  1},
  //   {1, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 4, 0, 0,  1},
  //   {1, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  1},
  //   {1, 0, 0, 0,  0, 0, 1, 0,  1, 0, 0, 0,  0, 0, 0, 0,  1},

  //   {1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1}
  // };
  // s32 Result[TILE_MAP_HEIGHT][TILE_MAP_WIDTH] = {0};
  for(s32 Y = 0; Y < TILE_MAP_HEIGHT; Y++)
  {
    for(s32 X = 0; X < TILE_MAP_WIDTH; X++)
    {
      // Borders
      if(Y == 0 || Y == (TILE_MAP_HEIGHT - 1))
      {
        // Result[Y][X] = 1;
        Result[X + Y*TILE_MAP_WIDTH] = 1;
      }
      if(X == 0 || X == (TILE_MAP_WIDTH - 1))
      {
        // Result[Y][X] = 1;
        Result[X + Y*TILE_MAP_WIDTH] = 1;
      }
      // Dual game start tile
      if(Y == 3 && X == 6)
      {
        // Result[Y][X] = 9;
        Result[X + Y*TILE_MAP_WIDTH] = 9;
      }
      // Stairs
      if(Y == 4 && X == 13)
      {
        // Result[Y][X] = 2;
        Result[X + Y*TILE_MAP_WIDTH] = 2;
      }
      // Stairs up are blocked at the beginning
      // Last level doesn't have it at all
      if((Y == 5 && X == 13) && !IsLastLevel)
      {
        // Result[Y][X] = 4;
        Result[X + Y*TILE_MAP_WIDTH] = 4;
      }
    }
  }
  // return *Result;
  return Result;
}

s32
IsMSPassed(f32 MS, f32 MSLimit)
{
  if(MS <= MSLimit)
  {
    return 0;
  }
  else
  {
    return 1;
  }
}

// s32
// IsColliding(u32 *TileMap, s32 TileX, s32 TileY)
// {
//   s32 Result = 0;
//   // TODO: 5 is tile map widht/height
//   s32 TileVal = DebugGetCurrentTileValue(5, TileMap, TileX, TileY);
//   if((TileX > -1) && (TileX < 5) && (TileY > -1) && (TileY < 5) && (TileVal != 0))
//   {
//     Result = 1;
//   }

//   return Result;
// }

s32
IsColliding(u32 *TileMap, s32 TileX, s32 TileY)
{
  s32 Result = 0;

  s32 TileVal = GetCurrentTileValue(TileMap, TileX, TileY);
  if((TileX > -1) && (TileX < TILE_MAP_WIDTH) && (TileY > -1) && (TileY < TILE_MAP_HEIGHT))
  {
    // TODO: names instead of numbers
    if((TileVal == 1) || (TileVal == -3) || (TileVal == 4))
    {
      Result = 1;
    }
  }

  return Result;
}

s32
s32PercentOfNum(s32 Percent, s32 Num)
{
  s32 Result;
  Result = (Percent * Num) / 100;

  return Result;
}

struct playing_sound *
PlayOneSound(struct temp_state *TempState)
{
  if(!TempState->FirstFreePlayingSound)
  {
    TempState->FirstFreePlayingSound = PushMem(&TempState->TempArena, 1, struct playing_sound);
    TempState->FirstFreePlayingSound->Next = 0;
  }

  struct playing_sound *PlayingSound = TempState->FirstFreePlayingSound;
  TempState->FirstFreePlayingSound = PlayingSound->Next;

  PlayingSound->SamplesPlayed = 0;
  PlayingSound->Volume[0] = 1.0f;
  PlayingSound->Volume[1] = 1.0f;

  PlayingSound->Next = TempState->FirstPlayingSound;
  TempState->FirstPlayingSound = PlayingSound;

  return PlayingSound;
}

void
GameGetSoundSamples(struct game_memory *Memory, struct game_sound_buffer *SoundBuffer)
{
  struct temp_state *TempState = (struct temp_state *)Memory->TempStorage;

  // Create and clear mixer channels (as floats)
  f32 *Channel0 = (f32 *)TempState->SoundMixerChannel0;
  f32 *Channel1 = (f32 *)TempState->SoundMixerChannel1;
  f32 *Dest0 = Channel0;
  f32 *Dest1 = Channel1;
  for(s32 SampleIndex = 0; SampleIndex < SoundBuffer->SamplesCount; SampleIndex++)
  {
    *Dest0++ = 0.0f;
    *Dest1++ = 0.0f;
  }

  // Sum all playing sounds
  for(struct playing_sound **PlayingSoundPtr = &TempState->FirstPlayingSound; *PlayingSoundPtr;)
  {
    struct playing_sound *PlayingSound = *PlayingSoundPtr;
    s32 SoundFinished = 0;

    struct loaded_wav *LoadedWav = &TempState->TestWav;
    if(LoadedWav)
    {
      f32 Volume0 = PlayingSound->Volume[0];
      f32 Volume1 = PlayingSound->Volume[1];
      Dest0 = Channel0;
      Dest1 = Channel1;

      u32 SamplesToMix = SoundBuffer->SamplesCount;
      u32 SamplesRemainingInSound = LoadedWav->SamplesCount - PlayingSound->SamplesPlayed;
      if(SamplesToMix > SamplesRemainingInSound)
      {
        SamplesToMix = SamplesRemainingInSound;
      }

      for(s32 SampleIndex = PlayingSound->SamplesPlayed;
        SampleIndex < ((s32)SamplesToMix + PlayingSound->SamplesPlayed);
        SampleIndex++)
      {
        f32 SampleValue = LoadedWav->Samples[0][SampleIndex];
        *Dest0++ += Volume0*SampleValue;
        *Dest1++ += Volume1*SampleValue;
      }

      SoundFinished = (LoadedWav->SamplesCount == PlayingSound->SamplesPlayed);
      PlayingSound->SamplesPlayed += (s32)SamplesToMix;

      if(SoundFinished)
      {
        *PlayingSoundPtr = PlayingSound->Next;
        PlayingSound->Next = TempState->FirstFreePlayingSound;
        TempState->FirstFreePlayingSound = PlayingSound;
      }
      else
      {
        PlayingSoundPtr = &PlayingSound->Next;
      }
    }
  }

  // Write summed sounds to the buffer (to be sent to OS)
  f32 *Source0 = Channel0;
  f32 *Source1 = Channel1;

  s16 *SampleOut = SoundBuffer->Samples;
  for(s32 SampleIndex = 0;
    SampleIndex < SoundBuffer->SamplesCount;
    ++SampleIndex)
  {
    // u32 TestSoundSamplesIndex = (GameState->TestSoundSamplesIndex + SampleIndex) % GameState->TestWav.SamplesCount;
    // s16 SampleValue = GameState->TestWav.Samples[0][TestSoundSamplesIndex];
    *SampleOut++ = (s16)(*Source0++ + 0.5f);
    *SampleOut++ = (s16)(*Source1++ + 0.5f);
  }
  // GameState->TestSoundSamplesIndex += SoundBuffer->SamplesCount;
}

// Dual board initialization
s32 DualBoard[BOARD_HEIGHT][BOARD_WIDTH] = {
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0}
};
s32 RandRow;
s32 RandCol;

void
RunTutorial(
  struct game_state *GameState,
  struct board_timer *BoardTimer,
  struct win32_screen_buffer *ScreenBuffer,
  struct game_input *Input,
  struct rand_state *RandNumSt,
  f32 ElapsedMS
)
{
  #define TUTORIAL_LENGTH 12
  #define MAX_TUTORIAL_STEPS 10

  // struct v2 TutorialLevels[TUTORIAL_LENGTH][MAX_TUTORIAL_STEPS] = {
  //   { {1, 1}, {2, 2},   {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} },
  // };
  // DEBUG
    // s32 test1 = s32PercentOfNum(100, 13);
    // s32 test2 = s32PercentOfNum(100, 0);
    // s32 test3 = s32PercentOfNum(0, 0);
    // s32 test4 = s32PercentOfNum(0, 100);
    // s32 test5 = s32PercentOfNum(80, 100);

    // s32 test6 = s32PercentOfNum(50, 66);
    // s32 test7 = s32PercentOfNum(1, 10);
  // end

  // { Row, Col } board coordinates, 0-based.
  #define ROW_COL_COUNT 2
  s32 TutorialLevels[TUTORIAL_LENGTH][MAX_TUTORIAL_STEPS][ROW_COL_COUNT] = {
    // First level, show that repeating position should be clicked
    { {1, 1}, {1, 1},   {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} },
    { {0, 2}, {0, 2},   {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} },
    { {0, 0}, {0, 0},   {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} },

    // Second level, increase steps to 3
    { {1, 1}, {1, 1}, {0, 0},   {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} },
    { {0, 1}, {0, 2}, {0, 2},   {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} },
    { {0, 0}, {1, 0}, {1, 0},   {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} },

    // Third level, increase steps to 5
    { {1, 1}, {2, 1}, {1, 2}, {1, 1}, {1, 1},   {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} },
    { {2, 2}, {0, 1}, {0, 2}, {0, 2}, {0, 0},   {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} },
    // Here {0, 0}, {1, 1}, {0, 0} setup for the N=3 case, where it's going to be valid
    { {2, 1}, {2, 1}, {0, 0}, {1, 1}, {0, 0},   {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} },

    // Fourth level, increase N level to 2
    { {0, 0}, {2, 2}, {1, 1}, {0, 1}, {1, 1},   {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} },
    { {1, 0}, {0, 1}, {2, 2}, {0, 1}, {0, 0},   {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} },
    { {2, 1}, {2, 1}, {0, 0}, {1, 1}, {0, 0},   {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} },

  };
  // TODO: This is wrong now, adjust according to the TutorialLevels
  s32 ScoreArr[TUTORIAL_LENGTH] = {
    1, 1, 1,  1, 1, 1,  1, 1, 1,   1, 1, 1
  };
  s32 MaxStepsArr[TUTORIAL_LENGTH] = {
    2, 2, 2,  3, 3, 3,  5, 5, 5,   5, 5, 5
  };
  s32 NLevelsArr[TUTORIAL_LENGTH] = {
    1, 1, 1,  1, 1, 1,  1, 1, 1,   2, 2, 2
  };

  struct level *Level = GameState->Level;
  Level->MaxSteps = MaxStepsArr[Level->Count];

  s32 CurrNValue = NLevelsArr[Level->Count];

  // Get tile map, modify orb position each time one tutorial level is passed
  // TODO: Board1Pos is also defined inside Initialize
  // should be somewhere in a State?
  struct tile_map *TileMap010 = GetTileMapByXYZ(GameState->TileMaps, 0, 1, 0);
  s32 *Tiles010 = (s32 *)TileMap010->Tiles;

  struct menu_button *PosBtn = GameState->BoardMenu->Buttons[BOARD_BUTTON_POSITION];

  // DEBUG print info
  DebugPrint("==================\n", 0);
  DebugPrint("Current level: \n", Level->Count);
  DebugPrint("Current step: \n", Level->Step);
  DebugPrint("Current N value: \n", CurrNValue);
  for(s32 PosInd = 0; PosInd < MAX_TUTORIAL_STEPS; PosInd++)
  {
    DebugPrint("Player input for position: \n", Level->PlayerInputForPosition[PosInd]);
  }
  DebugPrint("Positions hit: \n", Level->PosHit);
  DebugPrint("==================\n", 0);
  //

  // Always draw a board even if it is between levels
  DrawBoard(ScreenBuffer, GameState, (s32 *)DualBoard, Level->Mode);

  struct ui_board *SideMenu = GameState->UIBoard;
  struct menu_button *BackBtn = GameState->BoardMenu->Buttons[BOARD_BUTTON_BACK];
  struct menu_button *RestartBtn = GameState->BoardMenu->Buttons[BOARD_BUTTON_RESTART];

  DrawSideMenu(ScreenBuffer, GameState, SideMenu, Level->NValue);

  DrawRect(
    ScreenBuffer, BackBtn->StartX, BackBtn->StartY,
    BackBtn->Width, BackBtn->Height, 0xff0000
  );
  if(IsButtonClicked(ScreenBuffer, Input, BackBtn, BUTTON_B))
  {
    // When "Back" button is clicked, we need to reset the current steps
    // but keep current level
    BoardTimer->MS = 0;
    Level->Step = 0;
    PosBtn->IsClicked = 0;
    Level->IsNew = 1;
    Level->ShouldClearBoard = 1;

    // TODO: check if should clear at Level->Step index or Level->Step-1 ???
    Level->PlayerInputForPosition[Level->Step] = -2;
    Level->BoardStatePosition[Level->Step] = -1;

    for(s32 Row = 0; Row < BOARD_HEIGHT; Row++)
    {
      for(s32 Col = 0; Col < BOARD_WIDTH; Col++)
      {
        DualBoard[Row][Col] = 0;
      }
    }

    GameState->IsBetweenLevels = 0;
    GameState->IsDualNBackMode = 0;
  }

  if(GameState->IsBetweenLevels)
  {
    DrawRect(
      ScreenBuffer, RestartBtn->StartX, RestartBtn->StartY,
      RestartBtn->Width, RestartBtn->Height, 0x0000ff
    );
    if(IsButtonClicked(ScreenBuffer, Input, RestartBtn, BUTTON_R))
    {
      GameState->IsBetweenLevels = 0;
    }
  }

  // Dual n back mode
  if(!GameState->IsBetweenLevels)
  {
    // Skip first step
    if(Level->Step > 0)
    {
      // TODO: add controls for other buttons
      // Position
      // Store player's input if button was clicked
      // struct menu_button *PosBtn = GameState->BoardMenu->Buttons[BOARD_BUTTON_POSITION];
      if(IsButtonClicked(ScreenBuffer, Input, PosBtn, BUTTON_1))
      {
        if(!PosBtn->IsClicked)
        {
          PosBtn->IsClicked = 1;
          // DebugPrint("Current step (inside Position btn click): \n", Level->Step);
        }
      }
    }

    // Short delay at the beginning of the new level
    if(IsMSPassed(BoardTimer->MS, 200.0f) && Level->IsNew)
    {
      // RandCol = TutorialColsArr[Level->Count][Level->Step];
      // RandRow = TutorialRowsArr[Level->Count][Level->Step];
      RandRow = TutorialLevels[Level->Count][Level->Step][0];
      RandCol = TutorialLevels[Level->Count][Level->Step][1];
      s32 PosValue = RandCol + RandRow * BOARD_WIDTH;
      Level->BoardStatePosition[Level->Step] = PosValue;
      // Color is currently set in DrawBoard function
      // it is a value that we set at a certain position
      // 1 - red
      // 2 - green
      // 3 - blue
      DualBoard[RandRow][RandCol] = 1;
      Level->IsNew = 0;
    }
    if(IsMSPassed(BoardTimer->MS, BoardTimer->ToShowMS) && Level->ShouldClearBoard)
    {
      for(s32 Row = 0; Row < BOARD_HEIGHT; Row++)
      {
        for(s32 Col = 0; Col < BOARD_WIDTH; Col++)
        {
          DualBoard[Row][Col] = 0;
        }
      }
      Level->ShouldClearBoard = 0;
    }
    if(IsMSPassed(BoardTimer->MS, (BoardTimer->ToShowMS + BoardTimer->ToHideMS)))
    {
      // TODO: test with N level 3 (or more)!!!
      if(PosBtn->IsClicked)
      {
        Level->PlayerInputForPosition[Level->Step] = RandCol + RandRow * BOARD_WIDTH;
      }
      BoardTimer->MS = 0;
      Level->Step += 1;
      PosBtn->IsClicked = 0;
      Level->IsNew = 1;
      Level->ShouldClearBoard = 1;
    }

    BoardTimer->MS += ElapsedMS;

    // TODO: Go to the next level
    // if current step > max steps
    // and player CORRECTLY passed current tutorial level
    // f.e. on the first tutorial level the score should be
    // equal to 1
    // Otherwise restart current level (by resetting all the level values)
    if(Level->Step >= Level->MaxSteps)
    {
      for(s32 i = CurrNValue; i < Level->MaxSteps; i++)
      {
        // Only compare starting from current N value
        // Example:
        // Current N = 2
        // Compare Board[0] with Player[2]
        // Board : 4 0 4 2 4
        // Player: - - 4 6 -
        // Board[0] == Board[2] && Board[2] == Player[2] => hit
        s32 NBackInd = i - CurrNValue;
        if(NBackInd >= 0)
        {
          s32 BoardVal = Level->BoardStatePosition[i];
          s32 BoardNBackVal = Level->BoardStatePosition[NBackInd];
          s32 PlayerInputVal = Level->PlayerInputForPosition[i];
          if((BoardNBackVal == BoardVal) && (PlayerInputVal == BoardVal))
          {
            Level->PosHit += 1;
            DebugPrint("PosHit: \n", Level->PosHit);
          }
          // Doesn't matter for tutorial!
          if((BoardNBackVal == BoardVal))
          {
            Level->TotalExpectedPosHits += 1;
          }
        }
      }

      s32 CorrectScore = ScoreArr[Level->Count] == Level->PosHit;
      if(CorrectScore)
      {
        // Remove old orb, add new
        switch(Level->Count)
        {
          case 0:
          case 1:
          case 2:
          {
            Tiles010[GameState->TutorialObjTilePosX + GameState->TutorialObjTilePosY*TILE_MAP_WIDTH] = 0;
            GameState->TutorialObjTilePosX--;
            Tiles010[GameState->TutorialObjTilePosX + GameState->TutorialObjTilePosY*TILE_MAP_WIDTH] = 9;
            break;
          }
          case 3:
          {
            Tiles010[GameState->TutorialObjTilePosX + GameState->TutorialObjTilePosY*TILE_MAP_WIDTH] = 0;
            GameState->TutorialObjTilePosX++;
            GameState->TutorialObjTilePosY--;
            Tiles010[GameState->TutorialObjTilePosX + GameState->TutorialObjTilePosY*TILE_MAP_WIDTH] = 9;
            break;
          }
          case 4:
          case 5:
          {
            Tiles010[GameState->TutorialObjTilePosX + GameState->TutorialObjTilePosY*TILE_MAP_WIDTH] = 0;
            GameState->TutorialObjTilePosX++;
            Tiles010[GameState->TutorialObjTilePosX + GameState->TutorialObjTilePosY*TILE_MAP_WIDTH] = 9;
            break;
          }
          case 6:
          {
            Tiles010[GameState->TutorialObjTilePosX + GameState->TutorialObjTilePosY*TILE_MAP_WIDTH] = 0;
            GameState->TutorialObjTilePosX--;
            GameState->TutorialObjTilePosY--;
            Tiles010[GameState->TutorialObjTilePosX + GameState->TutorialObjTilePosY*TILE_MAP_WIDTH] = 9;
            break;
          }
          case 7:
          case 8:
          {
            Tiles010[GameState->TutorialObjTilePosX + GameState->TutorialObjTilePosY*TILE_MAP_WIDTH] = 0;
            GameState->TutorialObjTilePosX--;
            Tiles010[GameState->TutorialObjTilePosX + GameState->TutorialObjTilePosY*TILE_MAP_WIDTH] = 9;
            break;
          }
          case 9:
          {
            Tiles010[GameState->TutorialObjTilePosX + GameState->TutorialObjTilePosY*TILE_MAP_WIDTH] = 0;
            GameState->TutorialObjTilePosX--;
            GameState->TutorialObjTilePosY--;
            Tiles010[GameState->TutorialObjTilePosX + GameState->TutorialObjTilePosY*TILE_MAP_WIDTH] = 9;
            break;
          }
          case 10:
          case 11:
          {
            Tiles010[GameState->TutorialObjTilePosX + GameState->TutorialObjTilePosY*TILE_MAP_WIDTH] = 0;
            GameState->TutorialObjTilePosX--;
            Tiles010[GameState->TutorialObjTilePosX + GameState->TutorialObjTilePosY*TILE_MAP_WIDTH] = 9;
            break;
          }
          default:
          {
            break;
          }
        }

        Level->Count += 1;
        Level->MaxSteps = MaxStepsArr[Level->Count];
        // Exit dual n back mode
        GameState->IsDualNBackMode = 0;
      }
      else
      {
        // NOTE: We only show buttons "Restart", "Back" in tutorial mode?
        // in regular mode there is no need for that, as it should be clear
        // to the user if they passed or not
        // also we should show statistics in regular mode?

        // When score is incorrect, stop and show side menu
        // User should be able to restart current level or exit dual n back mode
        // TODO: check which side menu button was clicked
        // GameState->IsDualNBackMode = 0;

        GameState->IsBetweenLevels = 1;
      }

      Level->Step = 0;
      // Reset all scores
      Level->PosHit = 0;
      Level->TotalExpectedPosHits = 0;
      // Clear input records
      for(s32 i = 0; i < LEVEL_MAX_STEPS; i++)
      {
        Level->PlayerInputForPosition[i] = -2;
        Level->BoardStatePosition[i] = -1;
      }
      // Clear boad and show menu (next level or quit option)
      for(s32 Row = 0; Row < BOARD_HEIGHT; Row++)
      {
        for(s32 Col = 0; Col < BOARD_WIDTH; Col++)
        {
          DualBoard[Row][Col] = 0;
        }
      }
      // TODO: Should we also reset all the arrays?
    }
  }

//DEBUG (turn off tutorial)
if(Level->Count)
// Player has passed the tutorial
// if(Level->Count == TUTORIAL_LENGTH)
  {
    Level->PosHit = 0;

    Level->Tutorial = 0;
    // Setup for the regular game
    // Level->MaxSteps = 10;
    Level->MaxSteps = 3;
    Level->NValue = 1;
  }

}

void
RunDualNBack(
  struct game_state *GameState,
  struct board_timer *BoardTimer,
  struct win32_screen_buffer *ScreenBuffer,
  struct game_input *Input,
  struct rand_state *RandNumSt,
  f32 ElapsedMS
)
{
  //DEBUG
  // Level->MaxSteps = 2;
  // Level->NValue = 1;
  // Dual n back mode
  struct level *Level = GameState->Level;
  struct menu_button *PosBtn = GameState->BoardMenu->Buttons[BOARD_BUTTON_POSITION];

  // DEBUG print info
  DebugPrint("==================\n", 0);
  DebugPrint("Current level: \n", Level->Count);
  DebugPrint("Current step: \n", Level->Step);
  DebugPrint("Current N value: \n", Level->NValue);
  for(s32 PosInd = 0; PosInd < MAX_TUTORIAL_STEPS; PosInd++)
  {
    DebugPrint("Player input for position: \n", Level->PlayerInputForPosition[PosInd]);
  }
  DebugPrint("Positions hit: \n", Level->PosHit);
  DebugPrint("==================\n", 0);
  //

  DrawBoard(ScreenBuffer, GameState, (s32 *)DualBoard, Level->Mode);

  struct ui_board *SideMenu = GameState->UIBoard;
  struct menu_button *BackBtn = GameState->BoardMenu->Buttons[BOARD_BUTTON_BACK];
  struct menu_button *RestartBtn = GameState->BoardMenu->Buttons[BOARD_BUTTON_RESTART];

  DrawSideMenu(ScreenBuffer, GameState, SideMenu, Level->NValue);

  DrawRect(
    ScreenBuffer, BackBtn->StartX, BackBtn->StartY,
    BackBtn->Width, BackBtn->Height, 0xff0000
  );

  if(IsButtonClicked(ScreenBuffer, Input, BackBtn, BUTTON_B))
  {
    // When "Back" button is clicked, we need to reset the current steps
    // but keep current level
    BoardTimer->MS = 0;
    Level->Step = 0;
    PosBtn->IsClicked = 0;
    Level->IsNew = 1;
    Level->ShouldClearBoard = 1;

    // TODO: check if should clear at Level->Step index or Level->Step-1 ???
    Level->PlayerInputForPosition[Level->Step] = -2;
    Level->BoardStatePosition[Level->Step] = -1;

    for(s32 Row = 0; Row < BOARD_HEIGHT; Row++)
    {
      for(s32 Col = 0; Col < BOARD_WIDTH; Col++)
      {
        DualBoard[Row][Col] = 0;
      }
    }

    GameState->IsBetweenLevels = 0;
    GameState->IsDualNBackMode = 0;
  }

  if(GameState->IsBetweenLevels)
  {
    DrawRect(
      ScreenBuffer, RestartBtn->StartX, RestartBtn->StartY,
      RestartBtn->Width, RestartBtn->Height, 0x0000ff
    );
    if(IsButtonClicked(ScreenBuffer, Input, RestartBtn, BUTTON_R))
    {
      GameState->IsBetweenLevels = 0;
    }
  }

  if(!GameState->IsBetweenLevels)
  {
    // Skip first step
    if(Level->Step > 0)
    {
      // Position
      // Store player's input if button was clicked
      if(IsButtonClicked(ScreenBuffer, Input, PosBtn, BUTTON_1))
      // if(Input->Buttons[6].IsDown)
      {
        if(!PosBtn->IsClicked)
        {
          PosBtn->IsClicked = 1;
        }
      }

      if(Level->Mode >= 2)
      {
        // Color
        if(Input->Buttons[7].IsDown)
        {
          if(!Level->ColorBtnIsClicked)
          {
            Level->ColorBtnIsClicked = 1;
          }
        }
      }
    }

    // Short delay at the beginning of the new level
    if(IsMSPassed(BoardTimer->MS, 200.0f) && Level->IsNew)
    {
      RandNumSt->Seed += (u32)(ElapsedMS + 0.5f);
      u32 RandNum1;
      u32 RandNum2;

      // TODO: ensure at least M repeating sequences?
      // sometimes random gives none
      RandNum1 = xorshift32(RandNumSt);
      RandNum2 = xorshift32(RandNumSt);
      RandRow = RandNum1 % 3;
      RandCol = RandNum2 % 3;
      // Saves the position on each step
      // So then we could check it N steps back
      Level->BoardStatePosition[Level->Step] = RandCol + RandRow * BOARD_WIDTH;
      DualBoard[RandRow][RandCol] = 1;

      if(Level->Mode >= 2)
      {
        u32 RandNum3 = xorshift32(RandNumSt);
        // Should be from 1 to 7
        Level->CurrColor = (RandNum3 % 6) + 1;
        Level->BoardStateColor[Level->Step] = Level->CurrColor;
        DualBoard[RandRow][RandCol] = Level->CurrColor;
      }
      Level->IsNew = 0;
    }
    if(IsMSPassed(BoardTimer->MS, BoardTimer->ToShowMS) && Level->ShouldClearBoard)
    {
      for(u32 Row = 0; Row < BOARD_HEIGHT; Row++)
      {
        for(u32 Col = 0; Col < BOARD_WIDTH; Col++)
        {
          DualBoard[Row][Col] = 0;
        }
      }
      Level->ShouldClearBoard = 0;
    }
    if(IsMSPassed(BoardTimer->MS, (BoardTimer->ToShowMS + BoardTimer->ToHideMS)))
    {
      // TODO: test with N level 3 (or more)!!!
      if(PosBtn->IsClicked)
      {
        Level->PlayerInputForPosition[Level->Step] = RandCol + RandRow * BOARD_WIDTH;
      }
      if(Level->ColorBtnIsClicked)
      {
        Level->PlayerInputForPosition[Level->Step] = Level->CurrColor;
      }
      BoardTimer->MS = 0;
      Level->Step += 1;
      PosBtn->IsClicked = 0;
      Level->ColorBtnIsClicked = 0;
      Level->IsNew = 1;
      Level->ShouldClearBoard = 1;
    }

    BoardTimer->MS += ElapsedMS;

    // TODO: Go to the next level
    // if current step > max steps
    // and player CORRECTLY passed current tutorial level
    // f.e. on the first tutorial level the score should be
    // equal to 1
    // Otherwise restart current level (by resetting all the level values)
    if(Level->Step >= Level->MaxSteps)
    {
      for(s32 CurrNValue = Level->NValue; CurrNValue < Level->MaxSteps; CurrNValue++)
      {
        // Only compare starting from current N value
        // Example:
        // Current N = 2
        // Compare Board[0] with Player[2]
        // Board : 4 0 4 2 4
        // Player: - - 4 6 -
        // Board[0] == Board[2] && Board[2] == Player[2] => hit
        s32 NBackInd = CurrNValue - Level->NValue;
        if(NBackInd >= 0)
        {
          // TODO: Make PosHit go negative?
          s32 BoardVal = Level->BoardStatePosition[CurrNValue];
          s32 BoardNBackVal = Level->BoardStatePosition[NBackInd];
          s32 PlayerInputVal = Level->PlayerInputForPosition[CurrNValue];
          if((BoardNBackVal == BoardVal) && (PlayerInputVal == BoardVal))
          {
            Level->PosHit += 1;
            DebugPrint("PosHit: \n", Level->PosHit);
          }
          if((BoardNBackVal == BoardVal))
          {
            Level->TotalExpectedPosHits += 1;
          }
        }
      }

      // Player goes to the next level if their score is >= than given percentage
      s32 PassPercent = 80;
      s32 ScoreToPass = s32PercentOfNum(PassPercent, Level->TotalExpectedPosHits);
      s32 CorrectScore = Level->PosHit >= ScoreToPass;

      // If it's lower than some percentage, we should decrease N level and move player
      // to the previous floor
      s32 DecreasePercent = 30;
      s32 ScoreToDecrease = s32PercentOfNum(DecreasePercent, Level->TotalExpectedPosHits);
      s32 DecreaseNLevel = Level->PosHit <= ScoreToDecrease;

      // Note: N value should always correspond to the floor
      // player is currently in (floor is a 0-based Z coord)
      s32 CurrFloorNum = GameState->PlayerTileMapZ + 1;

      if(CorrectScore)
      {
        Level->AvailableCount = CurrFloorNum + 1;
      }

      DebugPrint("Total expected pos hits: \n", Level->TotalExpectedPosHits);
      DebugPrint("Actual pos hits: \n", Level->PosHit);
      DebugPrint("\n\n", 0);
      DebugPrint("Score to pass: \n", ScoreToPass);
      DebugPrint("Score to decrease: \n", ScoreToDecrease);


      Level->Step = 0;
      // Reset all scores
      Level->PosHit = 0;
      Level->TotalExpectedPosHits = 0;
      // Clear boad and show menu (next level or quit option)
      for(u32 Row = 0; Row < BOARD_HEIGHT; Row++)
      {
        for(u32 Col = 0; Col < BOARD_WIDTH; Col++)
        {
          DualBoard[Row][Col] = 0;
        }
      }
      // TODO: Should we also reset all the arrays?
      // TODO: commented out until "Between level" menu is done
      // GameState->IsBetweenLevels = 1;
      GameState->IsDualNBackMode = 0;
    }
  }
}

// struct game_state GameState = {0};
// !!! It's not possible to initialize struct globally
// like that:
// struct game_state GameState;
// GameState->Something = 1;
// such global values has to be known at compile time?
// so it's similar to defining variables for array sizes?
// ex. we can't do:
// int ArrSize = 3;
// in global space
// instead it should be:
// #define ArrSize 3

void
GameUpdateAndRender(
  struct game_memory *GameMemory,
  struct win32_screen_buffer *ScreenBuffer,
  struct game_input *Input,
  f32 ElapsedMS
)
{
  // Assert(sizeof(struct game_state) <= GameMemory->PermanentStorageSize);
  // TODO: should this be called each frame?
  struct game_state *GameState = (struct game_state *)GameMemory->PermanentStorage;
  // Initialize
  if(!GameState->IsInitialized)
  {
    GameState->PlayerPos.X = 33.0f * 13.0f;
    GameState->PlayerPos.Y = 33.0f * 13.0f;
    GameState->dPlayer.X = 0;
    GameState->dPlayer.Y = 0;
    GameState->PlayerWidth = 24;
    GameState->PlayerHeight = 12;
    GameState->PlayerTileMapX = 0;
    GameState->PlayerTileMapY = 1;
    GameState->PlayerTileMapZ = 0;

    GameState->TutorialObjTilePosX = 13;
    GameState->TutorialObjTilePosY = 13;

    GameState->IsMainMenu = 1;
    GameState->IsDualNBackMode = 0;
    GameState->IsBetweenLevels = 0;

    RandNumState.Seed = 1234;

    struct memory_arena Arena = {0};
    GameState->MemoryArena = Arena;
    InitializeArena(
      &GameState->MemoryArena,
      (u8 *)GameMemory->PermanentStorage + sizeof(struct game_state),
      GameMemory->PermanentStorageSize - sizeof(struct game_state)
    );

    GameState->SpritesAtlas = DebugLoadBmp("atlas01.bmp");
    // Atlas sprite coordinates
    // GameState->Sprites = PushMem(&GameState->MemoryArena, 1, struct sprites);
    // for(s32 NumIndex = 0; NumIndex < SPRITE_NUMBERS_COUNT; NumIndex++)
    // {
    //   GameState->Sprites->SpriteNumbers[NumIndex] = PushMem(&GameState->MemoryArena, 1, struct sprite_number);
    // }
    GameState->SpriteNumber.AtlasRow = 2; // always in row 2

    GameState->MainMenu = PushMem(&GameState->MemoryArena, 1, struct main_menu_state);
    for(s32 BtnIndex = 0; BtnIndex < MENU_BUTTONS_NUM; BtnIndex++)
    {
      GameState->MainMenu->Buttons[BtnIndex] = PushMem(&GameState->MemoryArena, 1, struct menu_button);
    }

    GameState->BoardMenu = PushMem(&GameState->MemoryArena, 1, struct board_menu_state);
    for(s32 BtnIndex = 0; BtnIndex < BOARD_BUTTON_COUNT; BtnIndex++)
    {
      GameState->BoardMenu->Buttons[BtnIndex] = PushMem(&GameState->MemoryArena, 1, struct menu_button);
    }

    GameState->UIBoard = PushMem(&GameState->MemoryArena, 1, struct ui_board);
    GameState->UIBoard->Width = 96;
    GameState->UIBoard->Height = 64;
    GameState->UIBoard->RightOffsetX = 48;
    GameState->UIBoard->TopOffsetY = 32;

    struct menu_button *BackBtn = GameState->BoardMenu->Buttons[BOARD_BUTTON_BACK];
    BackBtn->StartX = ScreenBuffer->Width - (GameState->UIBoard->Width + GameState->UIBoard->RightOffsetX);
    BackBtn->StartY = 32 + 32 + GameState->UIBoard->Height;
    BackBtn->Width = GameState->UIBoard->Width;
    BackBtn->Height = GameState->UIBoard->Height;

    struct menu_button *RestartBtn = GameState->BoardMenu->Buttons[BOARD_BUTTON_RESTART];
    RestartBtn->StartX = ScreenBuffer->Width - (GameState->UIBoard->Width + GameState->UIBoard->RightOffsetX);
    RestartBtn->StartY = BackBtn->StartY + GameState->UIBoard->Height + 5;
    RestartBtn->Width = 96;
    RestartBtn->Height = 64;

    GameState->BoardTimer = PushMem(&GameState->MemoryArena, 1, struct board_timer);
    struct board_timer *BoardTimer = GameState->BoardTimer;
    BoardTimer->MS = 0;
    BoardTimer->ToShowMS = 1500;
    BoardTimer->ToHideMS = 500;
    // TODO: why doesn't this work
    // GameState->BoardTimer->MS = 0;
    // GameState->BoardTimer->ToShowMS = 1500;
    // GameState->BoardTimer->ToHideMS = 500;

    GameState->Level = PushMem(&GameState->MemoryArena, 1, struct level);
    struct level *Level = GameState->Level;
    Level->Count = 0;
    Level->AvailableCount = 0;
    Level->Step = 0;
    Level->IsNew = 1;
    Level->ShouldClearBoard = 1;
    Level->DecreaseNLevel = 0;
    // 0 - regular game
    // 1, 2, 3, etc. - levels of tutorial
    Level->Tutorial = 1;
    // Level->MaxSteps = 5;
    Level->Mode = 1;

    Level->PosHit = 0;
    Level->TotalExpectedPosHits = 0;

    Level->NValue = 1;
    Level->CurrColor = 0;
    Level->BoardStatePosition = PushMem(&GameState->MemoryArena, LEVEL_MAX_STEPS, s32);
    Level->BoardStateColor = PushMem(&GameState->MemoryArena, LEVEL_MAX_STEPS, s32);

    Level->PlayerInputForPosition = PushMem(&GameState->MemoryArena, LEVEL_MAX_STEPS, s32);
    // Initialize with -1, because when calculating score, 0 could be a valid position
    // on the board. So -1 indicates that player didn't click a button
    // Same for Board state, but should be different from player's input initial values
    for(u32 i = 0; i < LEVEL_MAX_STEPS; i++)
    {
      Level->PlayerInputForPosition[i] = -2;
      Level->BoardStatePosition[i] = -1;
    }

    GameState->TileMaps = PushMem(
      &GameState->MemoryArena,
      TILE_MAP_ARR_X*TILE_MAP_ARR_Y*TILE_MAP_ARR_Z,
      struct tile_map
    );

    // TileMaps[1] <- width - no movement across X tile maps
    // TileMaps[1][2] <- height - only two tile maps across Y
    // TileMaps[1][2][9] <- nine tower floors - Z height

    // 3D array of tile maps (struct tile_map)
    // each Tile map consists of 1 array (a 2D array of actual tiles)
    struct tile_map *TileMaps = GameState->TileMaps;
    // Thus we need to also allocate memory for each 2D array inside
    // each Tile map
    for(u32 Z = 0; Z < TILE_MAP_ARR_Z; Z++)
    {
      for(u32 Y = 0; Y < TILE_MAP_ARR_Y; Y++)
      {
        for(u32 X = 0; X < TILE_MAP_ARR_X; X++)
        {
          // TileMaps[
          //   X + Y*TILE_MAP_ARR_X + Z*TILE_MAP_ARR_X*TILE_MAP_ARR_Y
          // ].Tiles = PushMem(
          //   &GameState->MemoryArena, TILE_MAP_WIDTH * TILE_MAP_HEIGHT, u32
          // );
          // Doesn't work
          // u32 *TileMapTiles;
          // TileMapTiles = GetTileMapArrByXYZ(TileMaps, X, Y, Z);
          // TileMapTiles = PushMem(
          //   &GameState->MemoryArena, TILE_MAP_WIDTH * TILE_MAP_HEIGHT, u32
          // );
          // Also works
          struct tile_map *TileMap = GetTileMapByXYZ(TileMaps, X, Y, Z);
          TileMap->Tiles = PushMem(
            &GameState->MemoryArena, TILE_MAP_WIDTH * TILE_MAP_HEIGHT, u32
          );
        }
      }
    }

    // TileMaps[z][y][x]
    // x - width, y - height, z - depth
    // x + y*width + z*width*height

    // u32 *Tiles001 = TileMaps[
    //   0 + 0*TILE_MAP_ARR_X + 1*TILE_MAP_ARR_X*TILE_MAP_ARR_Y
    // ].Tiles;
    // u32 *Tiles001 = GetTileMapArrByXYZ(TileMaps, 0, 0, 1);
    // CopyTilesArray((u32 *)TilesTower02, Tiles001, TILE_MAP_WIDTH, TILE_MAP_HEIGHT);

    // For tile map
    // 0 - walkable
    // 1 - wall
    // 2 - stairs down
    // 3 - stairs up
    // 4 - doors
    // 9 - NPC

    // s32 TilesOutside[TILE_MAP_HEIGHT][TILE_MAP_WIDTH] = {
    //   {1, 1, 1, 1,      1, 1, 1, 4,     4, 1, 1, 1,     1, 1, 1, 1,  1},
    //   {1, 0, 0, 0,  0, 0, 1, 0,  0, 1, 0, 0,  0, 0, 0, 0,    1},
    //   {-3, 0, -1, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, -2, 0,  -3},
    //   {-3, 0, 0, -2,  -1, 0, 9, 0,  0, 0, 0, 0,  0, 0, 0, 0,   -3},

    //   {-3, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  -3},
    //   {-3, 0, 0, 0,  0, 0, 0, 0,  0, 0, -1, 0,  0, 0, 0, 0,  -3},
    //   {-3, 0, -1, 0,  0, 0, 0, 0,  0, 0, 0, -2,  0, 0, -1, 0,  -3},
    //   {-3, 0, 0, 0,  0, 0, -1, 0,  0, 0, 0, -1,  0, 0, 0, 0,  -3},

    //   {-3, -3, -3, -3,  -3, -3, -3, -3,  -3, -3, -3, -3,  -3, -3, -3, -3,  -3}
    // };

    s32 TilesOutside[TILE_MAP_HEIGHT][TILE_MAP_WIDTH] = {
      { 1, 1, 1, 1,  1, 1, 1, 4,  4, 1, 1, 1,  1, 1, 1, 1 },
      { 1, 0, 0, 0,  0, 0, 1, 0,  0, 1, 0, 0,  0, 0, 0, -3 },
      {-3, 0, -1, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, -2, -3 },
      {-3, 0, 0, -2,  -1, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, -3 },

      {-3, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, -3 },
      {-3, 0, 0, 0,  0, 0, 0, 0,  0, 0, -1, 0,  0, 0, 0, -3 },
      {-3, 0, -1, 0,  0, 0, 0, 0,  0, 0, 0, -2,  0, 0, -1, -3 },
      {-3, 0, 0, 0,  0, 0, -1, 0,  0, 0, 0, -1,  0, 0, 0, -3 },

      {-3, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, -3 },
      {-3, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, -3 },
      {-3, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, -3 },
      {-3, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, -3 },

      {-3, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, -3 },
      {-3, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, -3 },
      {-3, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, -3 },
      {-3, -3, -3, -3,  -3, -3, -3, -3,  -3, -3, -3, -3,  -3, -3, -3, -3 }
    };

    s32 BoardPosX = 13;
    s32 BoardPosY = 13;

    TilesOutside[BoardPosY][BoardPosX] = 9;

    // s32 TilesOutside[5][5] = {
    //   {1, 1, 1, 0, 0},
    //   {1, 0, 0, 0, 0},
    //   {1, 0, 0, 0, 0},
    //   {0, 0, 0, 1, 0},
    //   {0, 0, 0, 0, 0},
    // };

    s32 TilesTower01[TILE_MAP_HEIGHT][TILE_MAP_WIDTH] = {
      {1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1 },
      {1, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 1 },
      {1, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 1 },
      {1, 0, 0, 0,  0, 0, 9, 0,  0, 0, 0, 0,  0, 0, 0, 1 },

      {1, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 1 },
      {1, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 4, 0, 1 },
      {1, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 1 },
      {1, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 1 },

      {1, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 1 },
      {1, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 1 },
      {1, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 1 },
      {1, 1, 1, 1,  1, 1, 1, 0,  0, 1, 1, 1,  1, 1, 1, 1 }
    };

    // struct tile_map *TileMap001 = GetTileMapByXYZ(TileMaps, 0, 0, 1);
    // s32 *Tiles001 = TileMap001->Tiles;
    // CopyTilesArray((s32 *)TilesTower02, Tiles001, TILE_MAP_WIDTH, TILE_MAP_HEIGHT);

    struct tile_map *TileMap000 = GetTileMapByXYZ(TileMaps, 0, 0, 0);
    u32 *Tiles000 = TileMap000->Tiles;
    CopyTilesArray((s32 *)TilesTower01, (s32 *)Tiles000, TILE_MAP_WIDTH, TILE_MAP_HEIGHT);

    struct tile_map *TileMap010 = GetTileMapByXYZ(TileMaps, 0, 1, 0);
    u32 *Tiles010 = TileMap010->Tiles;
    CopyTilesArray((s32 *)TilesOutside, (s32 *)Tiles010, TILE_MAP_WIDTH, TILE_MAP_HEIGHT);

    // The rest of the levels are the same
    for(u32 Z = 1; Z < TILE_MAP_ARR_Z; Z++)
    {
      s32 IsLastLevel = (Z == (TILE_MAP_ARR_Z - 1));
      s32 Result[TILE_MAP_HEIGHT][TILE_MAP_WIDTH] = {0};
      s32 *TowerLevel = GenerateTowerLevel(*Result, IsLastLevel);
      struct tile_map *TileMap = GetTileMapByXYZ(TileMaps, 0, 0, Z);
      s32 *Tiles = (s32 *)TileMap->Tiles;
      CopyTilesArray((s32 *)TowerLevel, Tiles, TILE_MAP_WIDTH, TILE_MAP_HEIGHT);
    }

    LoadGame(GameState, Level);

    // Set default values in case game was loaded and tutorial has been passed before
    if(!Level->Tutorial)
    {
      // Level->MaxSteps = 10;
      Level->MaxSteps = 3;
      Level->NValue = 1;
    }

    GameState->IsInitialized = 1;
  }

  struct temp_state *TempState = (struct temp_state *)GameMemory->TempStorage;
  // Initialize
  if(!TempState->IsInitialized)
  {
    struct memory_arena TempArena = {0};
    TempState->TempArena = TempArena;
    InitializeArena(
      &TempState->TempArena,
      (u8 *)GameMemory->TempStorage + sizeof(struct temp_state),
      GameMemory->TempStorageSize - sizeof(struct temp_state)
    );

    TempState->TestWav = DebugLoadWav("test.wav");

    if(TempState->TestWav.SamplesCount)
    {
      TempState->SoundMixerChannel0 = PushMem(&TempState->TempArena, TempState->TestWav.SamplesCount, f32);
      TempState->SoundMixerChannel1 = PushMem(&TempState->TempArena, TempState->TestWav.SamplesCount, f32);
    }
    // PlayOneSound(TempState);

    TempState->IsInitialized = 1;
  }

  // We have to assign it twice
  // first time inside !IsInitialized block
  // but it has to be available everywhere below
  // so do it second time (this is also how it's done in HMH)
  struct tile_map *TileMaps = GameState->TileMaps;
  struct board_timer *BoardTimer = GameState->BoardTimer;
  struct level *Level = GameState->Level;

  // TODO: should we restart tutorial once it's done, but
  // player click on it again?
  // For now, just remove it
  // Open the gates to the tower
  if(!Level->Tutorial)
  {
    struct tile_map *TilesOutside = GetTileMapByXYZ(TileMaps, 0, 1, 0);
    u32 NewTileVal = 0;
    // Set by X (col), Y (row)
    SetCurrentTileValue(TilesOutside, NewTileVal, 7, 0);
    SetCurrentTileValue(TilesOutside, NewTileVal, 8, 0);
    SetCurrentTileValue(TilesOutside, NewTileVal, 6, 3);
  }
  // Last floor level shouldn't have stairs (up)
  if(Level->AvailableCount && (Level->AvailableCount < TILE_MAP_ARR_Z))
  {
    // Example:
    // Player just finished Level 1 => Level->AvailableCount is eq to 2
    // Open stairs on the current level (Z=1)
    s32 MaxStairsLvl = Level->AvailableCount - 1;
    for(s32 CurrStairsLvl = 0; CurrStairsLvl < MaxStairsLvl; CurrStairsLvl++)
    {
      struct tile_map *TileMap00z = GetTileMapByXYZ(TileMaps, 0, 0, CurrStairsLvl);
      // 3 - upstairs
      u32 NewTileVal = 3;
      SetCurrentTileValue(TileMap00z, NewTileVal, 13, 5);
    }
  }

  // Tiles are now packed as s32*
  // To access Tile values as 2d array, we need to unpack it
  // with Tile[X + Y*MAP_WIDTH]
  struct tile_map *TileMap = GetTileMapByXYZ(
    TileMaps,
    GameState->PlayerTileMapX, GameState->PlayerTileMapY, GameState->PlayerTileMapZ
  );
  u32 *CurrentTileMap = TileMap->Tiles;

  //
  ClearScreen(ScreenBuffer);
  //

  // TODO: Make ElapsedSeconds instead?
  f32 dt = ElapsedMS / 50.0f;
  struct v2 ddPlayer = {0};
  f32 PlayerSpeed = 5.0f;
  // f32 PlayerSpeed = 1.0f;

  // Input
  if(!GameState->IsDualNBackMode)
  {
    if(Input->Buttons[BUTTON_UP].IsDown)
    {
      ddPlayer.Y -= 1.0f;
    }
    if(Input->Buttons[BUTTON_DOWN].IsDown)
    {
      ddPlayer.Y += 1.0f;
    }
    if(Input->Buttons[BUTTON_LEFT].IsDown)
    {
      ddPlayer.X -= 1.0f;
    }
    if(Input->Buttons[BUTTON_RIGHT].IsDown)
    {
      ddPlayer.X += 1.0f;
    }
    // Make diagonal movement
    if(ddPlayer.X != 0 && ddPlayer.Y != 0)
    {
      if(V2LengthSquared(ddPlayer) > 1.0f)
      {
        ddPlayer = V2Normalize(ddPlayer);
      }
    }
  }

  f32 Accel = 1.0f;
  f32 Deaccel = 0.8f;
  f32 MaxSpeed = 7.0f;

  // 0 - up
  // 1 - down
  // 2 - left
  // 3 - right
  if(!GameState->IsDualNBackMode)
  {
    if(Input->Buttons[BUTTON_UP].IsDown)
    {
      GameState->dPlayer.Y -= Accel;
      if(GameState->dPlayer.Y < -MaxSpeed)
      {
        GameState->dPlayer.Y = -MaxSpeed;
      }
    }
    else if(GameState->dPlayer.Y < 0.0f)
    {
      GameState->dPlayer.Y += Deaccel;
      if(GameState->dPlayer.Y > 0.0f)
      {
        GameState->dPlayer.Y = 0.0f;
      }
    }

    if(Input->Buttons[BUTTON_DOWN].IsDown)
    {
      GameState->dPlayer.Y += Accel;
      if(GameState->dPlayer.Y > MaxSpeed)
      {
        GameState->dPlayer.Y = MaxSpeed;
      }
    }
    else if(GameState->dPlayer.Y > 0.0f)
    {
      GameState->dPlayer.Y -= Deaccel;
      if(GameState->dPlayer.Y < 0.0f)
      {
        GameState->dPlayer.Y = 0.0f;
      }
    }

    if(Input->Buttons[BUTTON_LEFT].IsDown)
    {
      GameState->dPlayer.X -= Accel;
      if(GameState->dPlayer.X < -MaxSpeed)
      {
        GameState->dPlayer.X = -MaxSpeed;
      }
    }
    else if(GameState->dPlayer.X < 0.0f)
    {
      GameState->dPlayer.X += Deaccel;
      if(GameState->dPlayer.X > 0.0f)
      {
        GameState->dPlayer.X = 0.0f;
      }
    }

    if(Input->Buttons[BUTTON_RIGHT].IsDown)
    {
      GameState->dPlayer.X += Accel;
      if(GameState->dPlayer.X > MaxSpeed)
      {
        GameState->dPlayer.X = MaxSpeed;
      }
    }
    else if(GameState->dPlayer.X > 0.0f)
    {
      GameState->dPlayer.X -= Deaccel;
      if(GameState->dPlayer.X < 0.0f)
      {
        GameState->dPlayer.X = 0.0f;
      }
    }
  }

  // Check collisions
  if(GameState->dPlayer.X != 0.0f)
  {
    s32 MinY = f32Tos32(GameState->PlayerPos.Y) / TILE_HEIGHT;
    s32 MaxY = f32Tos32(GameState->PlayerPos.Y + GameState->PlayerHeight) / TILE_HEIGHT;
    s32 MinX = 0;
    s32 MaxX = 0;

    if(GameState->dPlayer.X < 0.0f)
    {
      MinX = f32Tos32(GameState->PlayerPos.X + GameState->dPlayer.X) / TILE_WIDTH;
      MaxX = f32Tos32(GameState->PlayerPos.X) / TILE_WIDTH;
    }
    else
    {
      MinX = f32Tos32(GameState->PlayerPos.X + GameState->PlayerWidth) / TILE_WIDTH;
      MaxX = f32Tos32(GameState->PlayerPos.X + GameState->PlayerWidth + GameState->dPlayer.X) / TILE_WIDTH;
    }

    s32 Break = 0;
    for(s32 CurrTileY = MinY; (CurrTileY <= MaxY) && !Break; CurrTileY++)
    {
      for(s32 CurrTileX = MinX; (CurrTileX <= MaxX) && !Break; CurrTileX++)
      {
        if(IsColliding(CurrentTileMap, CurrTileX, CurrTileY))
        {
          if(GameState->dPlayer.X < 0.0f)
          {
            GameState->PlayerPos.X = (f32)((CurrTileX + 1) * TILE_WIDTH);
          }
          else
          {
            GameState->PlayerPos.X = (f32)((CurrTileX * TILE_WIDTH) - GameState->PlayerWidth - 1);
          }
          GameState->dPlayer.X = 0;
          Break = 1;
          break;
        }
      }
    }
  }

  if(GameState->dPlayer.Y != 0.0f)
  {
    s32 MinX = f32Tos32(GameState->PlayerPos.X) / TILE_WIDTH;
    s32 MaxX = f32Tos32(GameState->PlayerPos.X + GameState->PlayerWidth) / TILE_WIDTH;
    s32 MinY = 0;
    s32 MaxY = 0;

    if(GameState->dPlayer.Y < 0.0f)
    {
      MinY = f32Tos32(GameState->PlayerPos.Y + GameState->dPlayer.Y) / TILE_HEIGHT;
      MaxY = f32Tos32(GameState->PlayerPos.Y) / TILE_HEIGHT;
    }
    else
    {
      MinY = f32Tos32(GameState->PlayerPos.Y + GameState->PlayerHeight) / TILE_HEIGHT;
      MaxY = f32Tos32(GameState->PlayerPos.Y + GameState->PlayerHeight + GameState->dPlayer.Y) / TILE_HEIGHT;
    }

    s32 Break = 0;
    for(s32 CurrTileY = MinY; (CurrTileY <= MaxY) && !Break; CurrTileY++)
    {
      for(s32 CurrTileX = MinX; (CurrTileX <= MaxX) && !Break; CurrTileX++)
      {
        if(IsColliding(CurrentTileMap, CurrTileX, CurrTileY))
        {
          if(GameState->dPlayer.Y < 0.0f)
          {
            GameState->PlayerPos.Y = (f32)((CurrTileY + 1) * TILE_HEIGHT);
          }
          else
          {
            GameState->PlayerPos.Y = (f32)((CurrTileY * TILE_HEIGHT) - GameState->PlayerHeight - 1);
          }
          GameState->dPlayer.Y = 0;
          Break = 1;
          break;
        }
      }
    }
  }
  GameState->PlayerPos.X += GameState->dPlayer.X;
  GameState->PlayerPos.Y += GameState->dPlayer.Y;

  s32 IsMovedToMapYZ = 0;

  // Check if we need to move to a different tile map
  if(GameState->PlayerPos.X < 0)
  {
    GameState->PlayerTileMapX -= 1;
    GameState->PlayerPos.X = TILE_MAP_WIDTH * TILE_WIDTH - 1;
  }
  if(GameState->PlayerPos.X >= TILE_MAP_WIDTH * TILE_WIDTH)
  {
    GameState->PlayerTileMapX += 1;
    GameState->PlayerPos.X = 0;
  }
  if(GameState->PlayerPos.Y < 0)
  {
    GameState->PlayerTileMapY -= 1;
    GameState->PlayerPos.Y = TILE_MAP_HEIGHT * TILE_HEIGHT - 1;
    IsMovedToMapYZ = 1;
  }
  if(GameState->PlayerPos.Y >= TILE_MAP_HEIGHT * TILE_HEIGHT)
  {
    GameState->PlayerTileMapY += 1;
    GameState->PlayerPos.Y = 0;
    IsMovedToMapYZ = 1;
  }

  s32 PlayerTileX = (f32Tos32(GameState->PlayerPos.X) + (GameState->PlayerWidth / 2)) / TILE_WIDTH;
  s32 PlayerTileY = (f32Tos32(GameState->PlayerPos.Y) + (GameState->PlayerHeight / 2)) / TILE_HEIGHT;
  s32 CurrTileVal = GetCurrentTileValue(CurrentTileMap, PlayerTileX, PlayerTileY);

  // 2 - downstaris
  if(CurrTileVal == 2)
  {
    GameState->PlayerTileMapZ -= 1;
    // When going up or down stairs,
    // always move player one tile away from the stairs
    GameState->PlayerPos.X += TILE_WIDTH;
    IsMovedToMapYZ = 1;
  }
  // 3 - upstairs
  if(CurrTileVal == 3)
  {
    GameState->PlayerTileMapZ += 1;
    GameState->PlayerPos.X -= TILE_WIDTH;
    IsMovedToMapYZ = 1;
  }

  if(IsMovedToMapYZ)
  {
    // N level always corresponds to the floor (PlayerTileMapZ is zero based)
    Level->NValue = GameState->PlayerTileMapZ + 1;
    // Save game each time player moves to a different map (Y or Z)
    SaveGame(GameState, Level);
    IsMovedToMapYZ = 0;
  }

  // Collision with NPC
  // it means we should start dual n back
  if(CurrTileVal == 9)
  {
    // DebugPrint("Btn enter is down: \n", Input->Buttons[BUTTON_ENTER].IsDown);
    if(Input->Buttons[BUTTON_ENTER].IsDown)
    {
      GameState->IsDualNBackMode = 1;
    }
    if(Input->Buttons[BUTTON_ESCAPE].IsDown)
    {
      GameState->IsDualNBackMode = 0;
    }
  }

  // Drawing
  if(GameState->IsMainMenu)
  {
    DrawMainMenu(ScreenBuffer, GameState);
    s32 IsOnBtn = (Input->MouseButtons[0].X >= GameState->MainMenu->Buttons[0]->StartX) &&
    (Input->MouseButtons[0].X <= GameState->MainMenu->Buttons[0]->StartX + GameState->MainMenu->Buttons[0]->Width) &&
    (Input->MouseButtons[0].Y >= GameState->MainMenu->Buttons[0]->StartY) &&
    (Input->MouseButtons[0].Y <= GameState->MainMenu->Buttons[0]->StartY + GameState->MainMenu->Buttons[0]->Height);
    // TODO: add exit btn?
    if(IsOnBtn && Input->MouseButtons[0].IsDown)
    {
      GameState->IsMainMenu = 0;
    }
  }
  else
  {
    if(!GameState->IsDualNBackMode)
    {
      // Regular mode, draw tiles
      DrawTile(ScreenBuffer, GameState, CurrentTileMap);
      // DebugDrawTile(ScreenBuffer, GameState, CurrentTileMap, 5);

      s32 PlayerX = f32Tos32(GameState->PlayerPos.X);
      s32 PlayerY = f32Tos32(GameState->PlayerPos.Y);
      s32 PlBmpOffsetX = 4;
      s32 PlBmpOffsetY = 20;
      s32 PlBmpAtlasRow = 1;
      s32 PlBmpAtlasCol = 0;

      // DrawBmp(
      //   ScreenBuffer,
      //   GameState->PlayerBmp.Pixels,
      //   PlayerX, PlayerY,
      //   GameState->PlayerBmp.Width, GameState->PlayerBmp.Height
      // );

      TestDrawBmp(
        ScreenBuffer,
        GameState->SpritesAtlas.Pixels,
        0, 0,
        PlayerX-PlBmpOffsetX, PlayerY-PlBmpOffsetY,
        TILE_WIDTH, TILE_HEIGHT
      );

      DrawRect(
        ScreenBuffer,
        f32Tos32(GameState->PlayerPos.X), f32Tos32(GameState->PlayerPos.Y),
        GameState->PlayerWidth, GameState->PlayerHeight, 0x00FF00);


      struct ui_board *SideMenu = GameState->UIBoard;
      DrawSideMenu(ScreenBuffer, GameState, SideMenu, (GameState->PlayerTileMapZ + 1));

      // DEBUG
      // s32 DebugLineWidth = 2;
      // s32 PlayerCenterX = f32Tos32(GameState->PlayerPos.X) + GameState->PlayerWidth/2;
      // s32 PlayerCenterY = f32Tos32(GameState->PlayerPos.Y) + GameState->PlayerHeight/2;
      // s32 PlayerDeltaX = PlayerCenterX + f32Tos32(PlayerDelta.X);
      // s32 PlayerDeltaY = PlayerCenterY + f32Tos32(PlayerDelta.Y);
      // if((PlayerDeltaX != 0) || (PlayerDeltaY != 0))
      // {
      //   DebugDrawLine(ScreenBuffer, PlayerCenterX, PlayerCenterY, PlayerDeltaX, PlayerDeltaY, DebugLineWidth, 0x0000FF);
      // }
      // s32 PlayerSpeedX = PlayerCenterX + f32Tos32(GameState->dPlayer.X);
      // s32 PlayerSpeedY = PlayerCenterY + f32Tos32(GameState->dPlayer.Y);
      // if((PlayerSpeedX != 0) || (PlayerSpeedY != 0))
      // {
      //   DebugDrawLine(ScreenBuffer, PlayerCenterX, PlayerCenterY, PlayerSpeedX, PlayerSpeedY, DebugLineWidth, 0xFF0000);
      // }
      // end DEBUG



      // Player's current tile
      // PlayerCurrentTileX
      // DrawRect(
      //   ScreenBuffer,
      //   PlayerCurrentTileX*TILE_WIDTH, PlayerCurrentTileY*TILE_HEIGHT,
      //   TILE_WIDTH, TILE_HEIGHT, 0x000000
      // );

      // Top left dot
      // s32 plLeftX = (s32)(GameState->PlayerPos.X + 0.5f);
      // s32 plTopY = (s32)(GameState->PlayerPos.Y + 0.5f);
      // DrawRect(
      //   ScreenBuffer,
      //   plLeftX, plTopY,
      //   5, 5, 0xFF0000);
    }
    else
    {
      // Run tutorial levels if they haven't been run yet
      if(Level->Tutorial)
      {
        RunTutorial(
          GameState,
          BoardTimer,
          ScreenBuffer,
          Input,
          &RandNumState,
          ElapsedMS
        );
      }
      // After that run regular levels
      else
      {
        RunDualNBack(
          GameState,
          BoardTimer,
          ScreenBuffer,
          Input,
          &RandNumState,
          ElapsedMS
        );
      }
    }
  }
}