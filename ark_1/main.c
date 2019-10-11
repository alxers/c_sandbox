#include <windows.h>
#include <stdint.h>
#include <stdio.h>

// TODO: check why we need 'static'
int running = 1;

struct RenderBuffer
{
    int width;
    int height;
    uint32_t *memory;
    BITMAPINFO bitmapInfo;
};

struct ButtonState
{
    int isDown;
    int changed;
};

enum btns
{
    LEFT,
    RIGHT,

    BTNCOUNT
};

struct Input
{
    struct ButtonState buttons[BTNCOUNT];
};

struct RenderBuffer renderBuffer;
struct Input input = {0};

#define processBtn(btn, vk)\
case vk:\
{\
input.buttons[btn].changed = isDown != input.buttons[btn].isDown;\
input.buttons[btn].isDown = isDown;\
} break;

#include "renderer.c"

LRESULT CALLBACK WindowProc(
    HWND   hwnd,
    UINT   uMsg,
    WPARAM wParam,
    LPARAM lParam
)
{
    LRESULT result = 0;

    switch(uMsg)
    {
        case WM_CLOSE:
        case WM_DESTROY:
        {
            running = 0;
        } break;

        case WM_SIZE:
        {
            // Get dimensions of a rectangle
            RECT rect;
            GetClientRect(hwnd, &rect);
            renderBuffer.width = rect.right - rect.left;
            renderBuffer.height = rect.bottom - rect.top;

            // Allocate memory
            if(renderBuffer.memory)
            {
                VirtualFree(0, 0, MEM_RELEASE);
            }

            renderBuffer.memory = VirtualAlloc(
                0,
                sizeof(uint32_t) * renderBuffer.width * renderBuffer.height,
                MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE
            );

            // Fill in bitmapinfo
            renderBuffer.bitmapInfo.bmiHeader.biSize = sizeof(renderBuffer.bitmapInfo.bmiHeader);
            renderBuffer.bitmapInfo.bmiHeader.biWidth = renderBuffer.width;
            renderBuffer.bitmapInfo.bmiHeader.biHeight = renderBuffer.height;
            renderBuffer.bitmapInfo.bmiHeader.biPlanes = 1;
            renderBuffer.bitmapInfo.bmiHeader.biBitCount = 32;
            renderBuffer.bitmapInfo.bmiHeader.biCompression = BI_RGB;
        } break;

        default:
        {
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        } break;
    }
    return result;
}

int WinMain(
  HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  LPSTR     lpCmdLine,
  int       nShowCmd
)
{
    WNDCLASSA windowClass = {0};
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = &WindowProc;
    windowClass.hInstance = hInstance;
    windowClass.lpszClassName = "main_window_class";

    if(!RegisterClass(&windowClass))
    {
        return 0;
    }

    HWND window = CreateWindow(windowClass.lpszClassName,
                                "TestWindow1",
                                WS_VISIBLE | WS_OVERLAPPEDWINDOW,
                                CW_USEDEFAULT,
                                CW_USEDEFAULT,
                                640, 480, 0, 0, 0, 0
                                );

    HDC deviceContext = GetDC(window);

    // Time
    float deltaTime = 0.016666f; // can be just 0.0 ?
    LARGE_INTEGER frameBeginTime;
    QueryPerformanceCounter(&frameBeginTime);

    float performanceFreq;
    {
        LARGE_INTEGER perf;
        QueryPerformanceFrequency(&perf);
        performanceFreq = (float)perf.QuadPart;
    }

    while(running)
    {
        // Input

        // Clear button states
        for(int i = 0; i < BTNCOUNT; i++)
        {
            input.buttons[i].changed = 0;
        }

        MSG message;
        while(PeekMessage(&message, window, 0, 0, PM_REMOVE))
        {
            switch(message.message)
            {
                case WM_KEYUP:
                case WM_KEYDOWN:
                {
                    uint32_t vkCode = (uint32_t)message.wParam;
                    int isDown = ((message.lParam & (1 << 31)) == 0);
                    switch(vkCode)
                    {
                        processBtn(LEFT, VK_LEFT);
                        processBtn(RIGHT, VK_RIGHT);
                        // case VK_LEFT:
                        // {
                        //     input.buttons[LEFT].changed = isDown != input.buttons[LEFT].isDown;
                        //     input.buttons[LEFT].isDown = isDown;
                        // } break;

                        // case VK_RIGHT:
                        // {
                        //     input.buttons[RIGHT].changed = isDown != input.buttons[RIGHT].isDown;
                        //     input.buttons[RIGHT].isDown = isDown;
                        // } break;
                    }
                } break;

                default:
                {
                    TranslateMessage(&message); 
                    DispatchMessage(&message);   
                }
            }
        }
        // End Input

        // Drawing
        gameUpdateAndRender(&input, deltaTime);
        // End Drawing

        // Rendering
        StretchDIBits(
                        deviceContext,
                        0, 0,
                        renderBuffer.width, renderBuffer.height,
                        0, 0,
                        renderBuffer.width, renderBuffer.height,
                        renderBuffer.memory, &renderBuffer.bitmapInfo,
                        DIB_RGB_COLORS,
                        SRCCOPY
                    );

        LARGE_INTEGER frameEndTime;
        QueryPerformanceCounter(&frameEndTime);
        deltaTime = (float)(frameEndTime.QuadPart - frameBeginTime.QuadPart) / performanceFreq;
        frameBeginTime = frameEndTime;
    }
}