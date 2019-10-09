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

struct Button
{
    int key;
    int isPressed;
};

struct Input
{
    struct Button buttons[2];
};

struct RenderBuffer renderBuffer;
struct Button left = {VK_LEFT, 0};
struct Button right  = {VK_RIGHT, 0};

// button1.key = VK_LEFT;
// button2.key = VK_RIGHT;

struct Input input;
// input.buttons[0] = button1;
// input.buttons[1] = button2;

void clearScreen(uint32_t c)
{
    uint32_t *pixel = renderBuffer.memory;

    for(int y = 0; y < renderBuffer.height; y++)
    {
        for(int x = 0; x < renderBuffer.width; x++)
        {
            *pixel++ = c;
        }
    }
}

void drawRect(int x0, int y0, int x1, int y1, uint32_t color)
{
    for(int y = y0; y < y1; y++)
    {
        uint32_t *pixel = renderBuffer.memory + x0 + renderBuffer.width * y;
        for(int x = x0; x < x1; x++)
        {
            *pixel++ = color;
        }
    }
}

LRESULT CALLBACK WindowProc(
    HWND   hwnd,
    UINT   uMsg,
    WPARAM wParam,
    LPARAM lParam
)
{
    LRESULT result = 0;
    input.buttons[0] = left;
    input.buttons[1] = right;

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

        case WM_KEYDOWN:
        {
            // if(wParam == VK_LEFT)
            // {

            // }
            switch(wParam)
            {
                case VK_LEFT:
                {
                    input.buttons[0].isPressed = 1;
                } break;

                case VK_RIGHT:
                {
                    input.buttons[0].isPressed = 1;
                } break;
            }
        } break;

        default:
        {
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);  
        }
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

    int pos = 0;

    while(running)
    {
        // Input
        MSG message;
        while(PeekMessage(&message, window, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message); 
            DispatchMessage(&message); 
        }

        // OutputDebugString(message.message);

        // Drawing
        clearScreen(0x000000);
        if(input.buttons[0].isPressed)
        {
            pos += 10;
        }
        drawRect(pos, pos, 200, 300, 0xffffff);


        // Clear button states
        for(int i = 0; i < 2; i++)
        {
            input.buttons[i].isPressed = 0;
        }
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