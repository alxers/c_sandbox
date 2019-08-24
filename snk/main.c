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
    struct Node *previous;

};

int ateAnApple(int arr[10][10], int x, int y, int *score)
{
    if (arr[x][y] == 5)
    {
        // TODO: Add new tail each time
        *score = *score + 1;
        printf("%s\n", "Snake ate an apple");
        printf("%d\n", *score);
        // usleep(1000000);
        arr[x][y] = 9;

        return 1;
    }

    return 0;
}

// TODO: fix segfault on adding new node
struct Node addNewNode(struct Node *node)
{
    struct Node newNode;
    newNode.previous = node;

    switch((*node).direction)
    {
        case keyW:
            newNode.x = node->x + 1;
            newNode.y = node->y;
            break;

        case keyD:
            newNode.x = node->x;
            newNode.y = node->y - 1;
            break;

        case keyA:
            newNode.x = node->x -1;
            newNode.y = node->y;
            break;

        case keyS:
            newNode.x = node->x;
            newNode.y = node->y + 1;
            break;
    }
    newNode.direction = node->direction;
    return newNode;
}

void drawSnake(struct Node *node, int arr[10][10], int identifier)
{
    struct Node tmpNode = *node;
    while(tmpNode.previous)
    {
        arr[tmpNode.x][tmpNode.y] = identifier;
        tmpNode = *(tmpNode.previous);
    }
    // Do this one more time for the last piece
    arr[tmpNode.x][tmpNode.y] = identifier;
}

void updatePosition(struct Node *last, struct Node *previous)
{
    if(!(*last).previous)
    {

    } else {
        (*last).x = (*previous).x;
        (*last).y = (*previous).y;
        (*last).direction = (*previous).direction;
        updatePosition((*last).previous, (*previous).previous);
    }

}

void copyStruct(struct Node *from, struct Node *to)
{
    while(from != NULL)
    {
        (*to).x = (*from).x;
        (*to).y = (*from).y;
        (*to).direction = (*from).direction;

        if((*to).previous != NULL)
        {
            (*to).previous = to;
            (*from).previous = from;
        }
    }
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
    drawDesk(arr, &score);

    int ch;
    // int keyW = 119;
    // int keyD = 100;
    // int keyA = 97;
    // int keyS = 115;

    struct Node head;
    struct Node tail1;
    struct Node tail2;

    head.x = 4;
    head.y = 2;
    head.direction = 115;
    head.previous = NULL;

    tail1.x = head.x -1;
    tail1.y = head.y;
    tail1.direction = head.direction;
    tail1.previous = &head;

    tail2.x = tail1.x - 1;
    tail2.y = tail1.y;
    tail2.direction = tail1.direction;
    tail2.previous = &tail1;

    struct Node newTail;

    while(1)
    {
        ch = head.direction;

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
            if(head.x > 1)
            {
                // Empty current pos
                drawSnake(&tail2, arr, 0);
                
                // Set new pos
                updatePosition(&tail2, tail2.previous);
                head.x -= 1;

                if (ateAnApple(arr, head.x, head.y, &score))
                {
                    // tail2 = addNewNode(&tail2);
                    newTail = addNewNode(&tail2);
                    // tail2 = newTail;
                    copyStruct(&newTail, &tail2);
                }
                drawSnake(&tail2, arr, 9);
            } else
            {
                // collision
            }
        }

        if (ch == keyS)
        {
            // Lower border
            if(head.x < 8)
            {
                // Empty current pos
                drawSnake(&tail2, arr, 0);
                
                // Set new pos
                updatePosition(&tail2, tail2.previous);
                head.x += 1;

                if (ateAnApple(arr, head.x, head.y, &score))
                {
                    // tail2 = addNewNode(&tail2);
                    newTail = addNewNode(&tail2);
                    newTail.previous = &tail2;
                    tail2 = newTail;
                }
                drawSnake(&tail2, arr, 9);
            } else
            {
                // collision
            }
        }

        if (ch == keyD)
        {
            // Right border
            if(head.y < 8)
            {
                // Empty current pos
                drawSnake(&tail2, arr, 0);
                
                // Set new pos
                updatePosition(&tail2, tail2.previous);
                head.y += 1;

                if (ateAnApple(arr, head.x, head.y, &score))
                {
                    // tail2 = addNewNode(&tail2);
                    newTail = addNewNode(&tail2);
                    newTail.previous = &tail2;
                    tail2 = newTail;
                }
                drawSnake(&tail2, arr, 9);
            } else
            {
                // collision
            }
        }

        if (ch == keyA)
        {
            // Left border
            if(head.y > 1)
            {
                // Empty current pos
                drawSnake(&tail2, arr, 0);
                
                // Set new pos
                updatePosition(&tail2, tail2.previous);
                head.y -= 1;

                if (ateAnApple(arr, head.x, head.y, &score))
                {
                    // tail2 = addNewNode(&tail2);
                    newTail = addNewNode(&tail2);
                    newTail.previous = &tail2;
                    tail2 = newTail;
                }
                drawSnake(&tail2, arr, 9);
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
            head.direction = getchar();
        }
    }

    input_off();

    return 0;
}
