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

struct Input
{
    int left;
    int right;
};

struct RenderBuffer renderBuffer;
struct Input input;

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

    while(running)
    {
        // Input

        // Clear button states
        input.left = 0;
        input.right = 0;

        MSG message;
        while(PeekMessage(&message, window, 0, 0, PM_REMOVE))
        {
            switch(message.message)
            {
                case WM_KEYDOWN:
                {
                    switch(message.wParam)
                    {
                        case VK_LEFT:
                        {
                            input.left = 1;
                        } break;

                        case VK_RIGHT:
                        {
                            input.right = 1;
                        } break;
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
        gameUpdateAndRender(&input);
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
    }
}