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

void gameUpdateAndRender(struct Input *input, float dt)
{
    float player1Acceleration = 0.0;
    float player2Acceleration = 0.0;
    if(isDown(LEFT))
    {
        player1Acceleration -= 2000;
    }

    if(isDown(RIGHT))
    {
        player1Acceleration += 2000;
    }

    // Add friction
    player1Acceleration -= player1Velocity * 10.0;

    player1PosY = player1PosY + player1Velocity * dt + (player1Acceleration * dt * dt) / 2.0;
    player1Velocity = player1Velocity + player1Acceleration * dt;

    player2Acceleration -= player1Velocity * 10.0;

    player2PosY = player2PosY + player2Velocity * dt + (player2Acceleration * dt * dt) / 2.0;
    player2Velocity = player2Velocity + player2Acceleration * dt;

    //
    ballPosX += ballVelocityX * dt;
    ballPosY += ballVelocityY * dt;
    //

    // Player's collisions
    if(player1PosY + playerHalfSizeY > arenaHalfSizeY)
    {
        player1PosY = arenaHalfSizeY - playerHalfSizeY;
        player1Velocity *= -0.7;
    }
    else if(player1PosY - playerHalfSizeY < -arenaHalfSizeY)
    {
        player1PosY = -arenaHalfSizeY + playerHalfSizeY;
        player1Velocity *= -0.7;
    }

    if(player2PosY + playerHalfSizeY > arenaHalfSizeY)
    {
        player2PosY = arenaHalfSizeY - playerHalfSizeY;
        player2Velocity *= -0.7;
    }
    else if(player2PosY - playerHalfSizeY < -arenaHalfSizeY)
    {
        player2PosY = -arenaHalfSizeY + playerHalfSizeY;
        player2Velocity *= -0.7;
    }
    // End Player's collisions

    // Ball collisions
    if(ballPosX + ballHalfSize > 50 - playerHalfSizeX &&
       ballPosX - ballHalfSize < 50 + playerHalfSizeX &&
       ballPosY + ballHalfSize > player1PosY - playerHalfSizeY &&
       ballPosY + ballHalfSize < player1PosY + playerHalfSizeY)
    {
        ballPosX = 50 - playerHalfSizeX - ballHalfSize;
        ballVelocityX *= -1;
        ballVelocityY = player1Velocity * 0.75;
    }
    else if(ballPosX + ballHalfSize > -50 - playerHalfSizeX &&
            ballPosX - ballHalfSize < -50 + playerHalfSizeX &&
            ballPosY + ballHalfSize > player2PosY - playerHalfSizeY &&
            ballPosY + ballHalfSize < player2PosY + playerHalfSizeY)
    {
        ballPosX = -50 + playerHalfSizeX + ballHalfSize;
        ballVelocityX *= -1;
        ballVelocityY = player2Velocity * 0.75;
    }
    // End Ball collisions

    clearScreen(0x000000);

    // Arena boundaries
    drawRect(0, 0, arenaHalfSizeX, arenaHalfSizeY, 0x5c5c5c);

    // Ball
    drawRect(ballPosX, ballPosY, 1, 1, 0xffffff);

    drawRect(50, player1PosY, 2.5, 12, 0xffffff);
    drawRect(-50, 0, 2.5, 12, 0xffffff);
}