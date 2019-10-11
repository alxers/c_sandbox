int clamp(int min, int val, int max)
{
    if(val < min)
    {
        return min;
    }

    if(val > max)
    {
        return max;
    }

    return val;
}

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

void drawRectPix(int x0, int y0, int x1, int y1, uint32_t color)
{

    x0 = clamp(0, x0, renderBuffer.width);
    x1 = clamp(0, x1, renderBuffer.width);
    y0 = clamp(0, y0, renderBuffer.height);
    y1 = clamp(0, y1, renderBuffer.height);

    for(int y = y0; y < y1; y++)
    {
        uint32_t *pixel = renderBuffer.memory + x0 + renderBuffer.width * y;
        for(int x = x0; x < x1; x++)
        {
            *pixel++ = color;
        }
    }
}

static float scale = 0.01f;

void drawRect(float x, float y, float halfSizeX, float halfSizeY, uint32_t color)
{
    x *= renderBuffer.height * scale;
    y *= renderBuffer.height * scale;
    halfSizeX *= renderBuffer.height * scale;
    halfSizeY *= renderBuffer.height * scale;

    // Move coordinates start point in the middle of the screen
    x += renderBuffer.width / 2.0f;
    y += renderBuffer.height / 2.0f;

    int x0 = x - halfSizeX;
    int x1 = x + halfSizeX;
    int y0 = y - halfSizeY;
    int y1 = y + halfSizeY;

    drawRectPix(x0, y0, x1, y1, color);
}

// Game
#define isDown(btn) (input->buttons[btn].isDown)

float playerPosY = 0.0;

void gameUpdateAndRender(struct Input *input, float dt)
{
    float speed = 50.0; // units per second
    if(isDown(LEFT))
    {
        playerPosY -= speed * dt;
    }

    if(isDown(RIGHT))
    {
        playerPosY += speed * dt;
    }

    clearScreen(0x000000);
    drawRect(0, 0, 1, 1, 0xffffff);
    drawRect(50, playerPosY, 2.5, 12, 0xffffff);
    drawRect(-50, 0, 2.5, 12, 0xffffff);
}