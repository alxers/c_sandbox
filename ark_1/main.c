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
void draw(uint32_t c)
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
                MEM_COMMIT, PAGE_READWRITE
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
                                    1024, 640, 0, 0, 0, 0
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
        draw(0xff4400);

        // Rendering
        StretchDIBits(
                        deviceContext,
                        0, 0,
                        renderBuffer.width, renderBuffer.height,
                        0, 0,
                        renderBuffer.width, renderBuffer.height,
                        &renderBuffer.memory, &renderBuffer.bitmapInfo,
                        DIB_RGB_COLORS,
                        SRCCOPY
                    );
    }
}