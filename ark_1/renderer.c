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

#include <math.h>
void drawCirclePix(float m, float n, float radius, uint32_t color)
{

    // works
    // for(int i = 0; i < 360; i++)
    // {
    //     x = radius * cosf(i);
    //     y = radius * sinf(i);
    //     drawRect(x, y, 0.1, 0.1, color);
    // }

    // works
    // for(int y = -radius; y <= radius; y++)
    // {
    //     for(int x = -radius; x <= radius; x++)
    //     {
    //         if(x*x + y*y <= radius*radius + radius*0.8f)
    //         {
    //             drawRect((float)x, (float)y, 1.0, 1.0, color);
    //         }
    //     }
    // }

    float x = radius, y = 0.0;

    int P = 1 - radius;
    while(x > y)
    {
        y++;

        if(P <= 0)
        {
            P = P + 2*y + 1;
        }
        else
        {
            x--;
            P = P + 2*y - 2*x + 1;
        }

        if(x < y)
        {
            break;
        }
        drawRect(x, y, 1.0, 1.0, color);
        drawRect(-x, y, 1.0, 1.0, color);
        drawRect(x, -y, 1.0, 1.0, color);
        drawRect(-x, -y, 1.0, 1.0, color);

        if(x != y)
        {
            drawRect(y, x, 1.0, 1.0, color);
            drawRect(-y, x, 1.0, 1.0, color);
            drawRect(y, -x, 1.0, 1.0, color);
            drawRect(-y, -x, 1.0, 1.0, color);
        }
    }
}

// Game
#define isDown(btn) (input->buttons[btn].isDown)

float player1PosY = 0.0;
float player1Velocity = 0.0;

float player2PosY = 0.0;
float player2Velocity = 0.0;

float arenaHalfSizeX = 65;
float arenaHalfSizeY = 45;
float playerHalfSizeX = 2.5;
float playerHalfSizeY = 12;

// Ball
float ballPosX, ballPosY, ballVelocityX = 100, ballVelocityY;
float ballHalfSize = 1;

void playerUpdate(float *playerPosY, float *playerVelocity, float playerAcceleration ,float dt)
{
    // Add friction
    playerAcceleration -= *playerVelocity * 10.0;

    *playerPosY = *playerPosY + *playerVelocity * dt + (playerAcceleration * dt * dt) / 2.0;
    *playerVelocity = *playerVelocity + playerAcceleration * dt;

    if(*playerPosY + playerHalfSizeY > arenaHalfSizeY)
    {
        *playerPosY = arenaHalfSizeY - playerHalfSizeY;
        *playerVelocity *= -0.7;
    }
    else if(*playerPosY - playerHalfSizeY < -arenaHalfSizeY)
    {
        *playerPosY = -arenaHalfSizeY + playerHalfSizeY;
        *playerVelocity *= -0.7;
    }
}

// void gameUpdateAndRender(struct Input *input, float dt)
// {
//     float player1Acceleration = 0.0;
//     float player2Acceleration = 0.0;
//     if(isDown(DOWN))
//     {
//         player1Acceleration -= 2000;
//     }

//     if(isDown(UP))
//     {
//         player1Acceleration += 2000;
//     }

//     if(isDown(S_BTN))
//     {
//         player2Acceleration -= 2000;
//     }

//     if(isDown(W_BTN))
//     {
//         player2Acceleration += 2000;
//     }

//     playerUpdate(&player1PosY, &player1Velocity, player1Acceleration, dt);
//     playerUpdate(&player2PosY, &player2Velocity, player2Acceleration, dt);

//     //
//     ballPosX += ballVelocityX * dt;
//     ballPosY += ballVelocityY * dt;
//     //

//     // Ball collisions
//     if(ballPosX + ballHalfSize > 50 - playerHalfSizeX &&
//        ballPosX - ballHalfSize < 50 + playerHalfSizeX &&
//        ballPosY + ballHalfSize > player1PosY - playerHalfSizeY &&
//        ballPosY + ballHalfSize < player1PosY + playerHalfSizeY)
//     {
//         ballPosX = 50 - playerHalfSizeX - ballHalfSize;
//         ballVelocityX *= -1;
//         ballVelocityY = player1Velocity * 0.75;
//     }
//     else if(ballPosX + ballHalfSize > -50 - playerHalfSizeX &&
//             ballPosX - ballHalfSize < -50 + playerHalfSizeX &&
//             ballPosY + ballHalfSize > player2PosY - playerHalfSizeY &&
//             ballPosY + ballHalfSize < player2PosY + playerHalfSizeY)
//     {
//         ballPosX = -50 + playerHalfSizeX + ballHalfSize;
//         ballVelocityX *= -1;
//         ballVelocityY = player2Velocity * 0.75;
//     }
//     // End Ball collisions


//     // Wall collisions
//     if(ballPosY + ballHalfSize > arenaHalfSizeY)
//     {
//         ballPosY = arenaHalfSizeY - ballHalfSize;
//         ballVelocityY *= -1;
//     }
//     else if(ballPosY - ballHalfSize < -arenaHalfSizeY)
//     {
//         ballPosY = -arenaHalfSizeY + ballHalfSize;
//         ballVelocityY *= -1;
//     }

//     if(ballPosX + ballHalfSize > arenaHalfSizeX)
//     {
//         ballPosX = 0;
//         ballPosY = 0;
//         ballVelocityY = 0;
//         ballVelocityX *= -1;
//     }
//     else if(ballPosX - ballHalfSize < -arenaHalfSizeX)
//     {
//         ballPosX = 0;
//         ballPosY = 0;
//         ballVelocityY = 0;
//         ballVelocityX *= -1;
//     }
//     // End Wall collisions

//     clearScreen(0x000000);

//     // Arena boundaries
//     drawRect(0, 0, arenaHalfSizeX, arenaHalfSizeY, 0x5c5c5c);

//     // Ball
//     drawRect(ballPosX, ballPosY, 1, 1, 0xffffff);

//     drawRect(50, player1PosY, 2.5, 12, 0xffffff);
//     drawRect(-50, player2PosY, 2.5, 12, 0xffffff);
// }

void gameUpdateAndRender(struct Input *input, float dt)
{
    clearScreen(0x000000);
    // Do midpoint line first?
    drawCirclePix(0.0, 0.0, 10.0, 0x5c5c5c);
}