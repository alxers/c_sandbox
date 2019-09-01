#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
// #include <sys/select.h>
#include <sys/ioctl.h>
#include <stropts.h>

#define keyW 119
#define keyD 100
#define keyA 97
#define keyS 115

#define SNAKE_LENGTH 100
#define APPLE 64
#define SNAKE 36

#define WIDTH 20
#define HEIGHT 20

#define BORDER_SYM 35
#define EMPTY_SYM 32

#define SPACE 32


struct termios terminalSettings;

void input_on()
{
    struct termios newTerminalSettings;

    tcgetattr( STDIN_FILENO, &terminalSettings );

    newTerminalSettings = terminalSettings;

    newTerminalSettings.c_lflag &= ~( ICANON | ECHO );
    newTerminalSettings.c_cc[VTIME] = 0;
    newTerminalSettings.c_cc[VMIN] = 1;

    tcsetattr( STDIN_FILENO, TCSANOW, &newTerminalSettings );
}

void input_off()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &terminalSettings);
}

void clearScreen()
{
  const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
  write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
}

int _kbhit()
{
    int STDIN = 0;
    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}

void drawDesk(int map[HEIGHT][WIDTH], int *score)
{
    clearScreen();
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            // printf("%c ", map[i][j]);
            putchar(map[i][j]);
            putchar(SPACE);
        }
        printf("\n");
    }
    printf("%s\n", "Score: ");
    printf("%d\n", *score);
}

struct Node {
    int x;
    int y;
    // 119-up, 100-right, 115-down, 97-left
    int direction;
};

void addNewNode(struct Node snake[])
{
    struct Node newNode;
    int x, y, direction, currentLastNodePosition;
    for(int i = 0; i < SNAKE_LENGTH - 1; i++)
    {
        if(!snake[i+1].direction)
        {
            x = snake[i].x;
            y = snake[i].y;
            direction = snake[i].direction;
            currentLastNodePosition = i;
            break;
        }
    }
    //TODO: Adding new node looks bad.
    // Sometimes snake goes right, and new node added upward
    switch(direction)
    {
        case keyW:
            newNode.x = x + 1;
            newNode.y = y;
            break;

        case keyD:
            newNode.x = x;
            newNode.y = y - 1;
            break;

        case keyA:
            newNode.x = x -1;
            newNode.y = y;
            break;

        case keyS:
            newNode.x = x;
            newNode.y = y + 1;
            break;
    }
    newNode.direction = direction;
    snake[currentLastNodePosition + 1] = newNode;

}

void drawSnake(struct Node snake[], int arr[HEIGHT][WIDTH], int identifier)
{
    for(int i = 0; i < SNAKE_LENGTH - 1; i++)
    {
        // Snake array contains actual nodes and empty nodes
        // All the empty nodes have x, y and direction set to 0
        // Here we check if it's an actual node (direction can't be 0 in the actual node)
        if(snake[i].direction)
        {
            arr[snake[i].x][snake[i].y] = identifier;
        }
    }
}

void updatePosition(struct Node snake[])
{
    // Do not use i >= 0, we don't need to update 'head' node with out of snake array
    // boundaries values (f.e. snake[-1] is {x = 1, y = 1, direction = 1} sometimes)
    for(int i = SNAKE_LENGTH - 1; i > 0; i--)
    {
        if(snake[i].direction && snake[i-1].direction)
        {
            snake[i].x = snake[i-1].x;
            snake[i].y = snake[i-1].y;
            snake[i].direction = snake[i-1].direction;
        }
    }

    // Update head coordinate based on direction.
    switch(snake[0].direction)
    {
        case keyW:
            snake[0].x -= 1;
            break;

        case keyD:
            snake[0].y += 1;
            break;

        case keyA:
            snake[0].y -= 1;
            break;

        case keyS:
            snake[0].x += 1;
            break;
    }

}

int collidesWithItself(struct Node snake[])
{
    for(int i = 4; i < SNAKE_LENGTH - 1; i++)
    {
        if(snake[i].direction)
        {
            if(snake[0].x == snake[i].x && snake[0].y == snake[i].y)
            {
                return 1;
            }
        }
    }

    return 0;
}

void moveSnakeOnBoard(struct Node snake[], int arr[HEIGHT][WIDTH], int *score, int *isRunning, int *gameSpeed)
{
    // Empty current pos
    drawSnake(snake, arr, EMPTY_SYM);
    
    // Set new pos
    updatePosition(snake);

    if(collidesWithItself(snake))
    {
        *isRunning = 0;
    }
    if (arr[snake[0].x][snake[0].y] == APPLE)
    {
       *score = *score + 1;
       *gameSpeed = *gameSpeed - 10000;
       arr[snake[0].x][snake[0].y] = SNAKE;
       addNewNode(snake);
    }

    drawSnake(snake, arr, SNAKE);

}

int main(int argv, char *argc[])
{
    input_on();
    // int arr[HEIGHT][WIDTH] = {
    //     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    //     {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    //     {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    //     {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    //     {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    //     {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    //     {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    //     {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    //     {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    //     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    // };

    int arr[HEIGHT][WIDTH];
    // TODO: Change borad, 0 to ., 9 to #, etc.
    for(int i = 0; i < HEIGHT; i++)
    {
        for(int j = 0; j < WIDTH; j++)
        {
            if(i == 0 || i == (HEIGHT - 1))
            {
                arr[i][j] = BORDER_SYM;
            } else {
                if(j == 0 || j == (WIDTH - 1))
                {
                    arr[i][j] = BORDER_SYM;
                } else {
                    arr[i][j] = EMPTY_SYM;
                }
            }
        }
    }

    // Prints the array that was given
    // Snake will be drawn inside arr and passed to drawDesk
    int score = 0;
    int isRunning = 1;
    int gameSpeed = 600000;

    drawDesk(arr, &score);

    struct Node head;
    struct Node tail1;
    struct Node tail2;

    head.x = 4;
    head.y = 2;
    head.direction = 115;

    tail1.x = head.x -1;
    tail1.y = head.y;
    tail1.direction = head.direction;

    tail2.x = tail1.x - 1;
    tail2.y = tail1.y;
    tail2.direction = tail1.direction;

    struct Node snake[SNAKE_LENGTH] = { 0 };
    snake[0] = head;
    snake[1] = tail1;
    snake[2] = tail2;

    while(isRunning)
    {
        // Generating position for apples
        int firstRandPositionIndex = (rand() % (HEIGHT - 2)) + 1;
        int secondRandPositionIndex = (rand() % (WIDTH - 2)) + 1;
        // Generating random time for apple to appear
        int randTiming = (rand() % 50);

        if (randTiming % 5)
        {
            arr[firstRandPositionIndex][secondRandPositionIndex] = APPLE;
        }

        // Update direction if key is pressed
        if (_kbhit())
        {
            int pressed = getchar();
            if(pressed == keyW || pressed == keyA || pressed == keyS || pressed == keyD)
            {
                // Do not allow to move in the opposite direction
                if(
                    !(snake[0].direction == keyW && pressed == keyS) &&
                    !(snake[0].direction == keyS && pressed == keyW) &&
                    !(snake[0].direction == keyA && pressed == keyD) &&
                    !(snake[0].direction == keyD && pressed == keyA)
                  )
                {
                    snake[0].direction = pressed;
                }
            }
        }

        if(snake[0].x > 1 && snake[0].x < (HEIGHT - 2) && snake[0].y < (HEIGHT - 2) && snake[0].y > 1)
        {
            moveSnakeOnBoard(snake, arr, &score, &isRunning, &gameSpeed);
        } else {
            isRunning = 0;
        }

        drawDesk(arr, &score);
        // 1000000 = 1 sec
        usleep(gameSpeed);
    }

    printf("%s\n", "Game over");

    input_off();

    return 0;
}
