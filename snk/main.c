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
#define APPLE 5
#define SNAKE 9


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

void drawDesk(int map[10][10], int *score)
{
    clearScreen();
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            printf("%d ", map[i][j]);
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

void drawSnake(struct Node snake[], int arr[10][10], int identifier)
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

void moveSnakeOnBoard(struct Node snake[], int arr[10][10], int *score, int *isRunning)
{


    // Empty current pos
    drawSnake(snake, arr, 0);
    
    // Set new pos
    updatePosition(snake);

    if(collidesWithItself(snake))
    {
        *isRunning = 0;
    }
    if (arr[snake[0].x][snake[0].y] == APPLE)
    {
       *score = *score + 1;
       arr[snake[0].x][snake[0].y] = 9;
       addNewNode(snake);
    }

    drawSnake(snake, arr, 9);

}

int main(int argv, char *argc[])
{
    input_on();
    int arr[10][10] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };

    // Prints the array that was given
    // Snake will be drawn inside arr and passed to drawDesk
    int score = 0;
    int isRunning = 1;

    drawDesk(arr, &score);

    int ch;

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
        ch = snake[0].direction;

        // Generating position for apples
        int firstRandPositionIndex = (rand() % 8) + 1;
        int secondRandPositionIndex = (rand() % 8) + 1;
        // Generating random time for apple to appear
        int randTiming = (rand() % 50);

        if (randTiming % 5)
        {
            arr[firstRandPositionIndex][secondRandPositionIndex] = 5;
        }

        if (ch == keyW)
        {
            // Upper border
            if(snake[0].x > 1)
            {
                moveSnakeOnBoard(snake, arr, &score, &isRunning);
            } else
            {
                // collision
            }
        }

        if (ch == keyS)
        {
            // Lower border
            if(snake[0].x < 8)
            {
                moveSnakeOnBoard(snake, arr, &score, &isRunning);
            } else
            {
                // collision
            }
        }

        if (ch == keyD)
        {
            // Right border
            if(snake[0].y < 8)
            {
                moveSnakeOnBoard(snake, arr, &score, &isRunning);
            } else
            {
                // collision
            }
        }

        if (ch == keyA)
        {
            // Left border
            if(snake[0].y > 1)
            {
                moveSnakeOnBoard(snake, arr, &score, &isRunning);
            } else
            {
                // collision
            }
        }

        drawDesk(arr, &score);
        // 1000000 = 1 sec
        usleep(500000);

        // Update direction if key is pressed
        if (_kbhit())
        {
            snake[0].direction = getchar();
        }
    }

    printf("%s\n", "Game over");

    input_off();

    return 0;
}
