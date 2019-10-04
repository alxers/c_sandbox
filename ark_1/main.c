#include <windows.h>
#include <stdint.h>

// TODO: check why we need 'static'
int running = 1;

struct RenderBuffer
{
    int width;
    int height;
    uint32_t *memory;
    BITMAPINFO bitmapInfo;
};

struct RenderBuffer renderBuffer;

// TODO: move to a different file
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

// void drawRect(int x0, int y0, int x1, int y1, uint32_t color)
// {
//     // uint32_t *pixel = renderBuffer.memory;

//     uint32_t *pixel = renderBuffer.memory + x0 + renderBuffer.width * y0;
//     for(int y = y0; y < y1; y++)
//     {
//         for(int x = x0; x < x1; x++)
//         {
//             *pixel++ = color;
//         }
//     }
// }

void drawRect(int x0, int y0, int x1, int y1, uint32_t color)
{
    // cast memory to uint8 ?
    uint8_t *row = (uint8_t *)renderBuffer.memory + x1 * 4 + y1 * (renderBuffer.width * 4);
    for(int y = y0; y < y1; y++)
    {
        uint32_t *pixel = (uint32_t *) row; 
        for(int x = x0; x < x1; x++)
        {
            *pixel++ = color;
        }
        // 4 is a bytePerPixel
        row += renderBuffer.width * 4;
    }
}

// end move

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
            GetWindowRect(hwnd, &rect);
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

        // This is needed to draw from top left
        // Currently it's drawn from bottom left
        // case WM_PAINT:
        // {
        //     PAINTSTRUCT paint;
        //     HDC deviceContext = BeginPaint(hwnd, &paint);
        //     int x = paint.rcPaint.left;
        //     int y = paint.rcPaint.top;


        // } break;

        default:
        {
            result = DefWindowProcA(hwnd, uMsg, wParam, lParam);  
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

    if(!RegisterClassA(&windowClass))
    {
        return 0;
    }

    HWND window = CreateWindowExA(
                                    0,
                                    windowClass.lpszClassName,
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
        MSG message;
        while(PeekMessageA(&message, window, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message); 
            DispatchMessage(&message); 
        }

        // Drawing
        // clearScreen(0xffffff);
        drawRect(-100, -100, 50, 50, 0xffffff);

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