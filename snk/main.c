#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
// #include <sys/select.h>
#include <sys/ioctl.h>
#include <stropts.h>


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
    struct Node *next;

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

void addNewNode(struct Node *node)
{
    struct Node tmpNode = *node;
    struct Node newNode;
    while(tmpNode.next)
    {
        tmpNode = *(tmpNode.next);
    }
    newNode.x = tmpNode.x;
    newNode.y = tmpNode.y;
    newNode.next = NULL;
    newNode.direction = 888;
    tmpNode.next = &newNode;
    printf("%s\n", "TEST");
    printf("%d\n", newNode.direction);
    usleep(1000000);
}

void drawSnake(struct Node *node, int arr[10][10], int identifier)
{
    struct Node tmpNode = *node;
    while(tmpNode.next)
    {
        arr[tmpNode.x][tmpNode.y] = identifier;
        tmpNode = *(tmpNode.next);
    }
    // Do this one more time for the last piece
    arr[tmpNode.x][tmpNode.y] = identifier;
}

void updatePosition(struct Node *first, struct Node *second)
{
    if(!(*second).next)
    {
        (*second).x = (*first).x;
        (*second).y = (*first).y;

        return;
    } else {
        updatePosition((*first).next, (*second).next);
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
    int keyW = 119;
    int keyD = 100;
    int keyA = 97;
    int keyS = 115;

    struct Node head;
    struct Node tail;

    head.x = 4;
    head.y = 2;
    head.direction = 115;

    tail.x = head.x;
    tail.y = head.y;
    tail.next = NULL;
    // remove
    tail.direction = 999;

    head.next = &tail;

    arr[head.x][head.y] = 9;
    arr[tail.x][tail.y] = 2;

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
                drawSnake(&head, arr, 0);
                
                // Set new pos
                updatePosition(&head, head.next);
                head.x -= 1;

                if (ateAnApple(arr, head.x, head.y, &score))
                {
                    addNewNode(&head);
                }
                drawSnake(&head, arr, 9);
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
                drawSnake(&head, arr, 0);
                
                // Set new pos
                // tail.x = head.x;
                // tail.y = head.y;
                updatePosition(&head, head.next);
                head.x += 1;

                if (ateAnApple(arr, head.x, head.y, &score))
                {
                    addNewNode(&head);
                }
                drawSnake(&head, arr, 9);
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
                drawSnake(&head, arr, 0);
                
                // Set new pos
                updatePosition(&head, head.next);
                head.y += 1;

                if (ateAnApple(arr, head.x, head.y, &score))
                {
                    addNewNode(&head);
                }
                drawSnake(&head, arr, 9);
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
                drawSnake(&head, arr, 0);
                
                // Set new pos
                updatePosition(&head, head.next);
                head.y -= 1;

                if (ateAnApple(arr, head.x, head.y, &score))
                {
                    addNewNode(&head);
                }
                drawSnake(&head, arr, 9);
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
