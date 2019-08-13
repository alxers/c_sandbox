#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
// #include <sys/select.h>
#include <sys/ioctl.h>
#include <stropts.h>

// #include <stdlib.h>

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

void drawDesk(int map[10][10], int score)
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
    printf("%d \nScore: ", score);
}

struct Node {
    int x;
    int y;
    // 119-up, 100-right, 115-down, 97-left
    int direction;
    struct Node *next;

};

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
    drawDesk(arr, score);

    int ch;
    int keyW = 119;
    int keyD = 100;
    int keyA = 97;
    int keyS = 115;

    struct Node *head;
    struct Node *body;

    head = malloc(sizeof(struct Node));
    body = malloc(sizeof(struct Node));

    head->x = 4;
    head->y = 2;
    head->direction = 115;

    body->x = head->x;
    body->y = head->y;
    head->next = body;


    arr[head->x][head->y] = 9;
    arr[body->x][body->y] = 2;

    while(1)
    {
        ch = head->direction;

        // Generating position for apples
        int randPositionIndex = (rand() % 8) + 1;
        // Generating random time for apple to appear
        int randTiming = (rand() % 50);

        if (randTiming % 5)
        {
            arr[randPositionIndex][randPositionIndex] = 5;
        }
        // printf("%d r->", randPositionIndex);

        //TODO: Fix score (it doesn't update properly)
        printf("%d score->", score);
        if (head->x == randPositionIndex && head->y == randPositionIndex)
        {
            printf("%s\n", "Snake ate an apple");
            // usleep(1000000);
            arr[head->x][head->y] = 9;
            score++;
        }

        if (ch == keyW)
        {
            // Upper border
            if(head->x > 1)
            {
                // Empty current pos
                arr[head->x][head->y] = 0;
                arr[body->x][body->y] = 0;
                
                // Set new pos
                body->x = head->x;
                body->y = head->y;
                head->x -= 1;
                arr[head->x][head->y] = 9;
                arr[body->x][body->y] = 2;
            } else
            {
                // collision
            }
        }

        if (ch == keyS)
        {
            // Lower border
            if(head->x < 8)
            {
                // Empty current pos
                arr[head->x][head->y] = 0;
                arr[body->x][body->y] = 0;
                
                // Set new pos
                body->x = head->x;
                body->y = head->y;
                head->x += 1;
                arr[head->x][head->y] = 9;
                arr[body->x][body->y] = 2;
            } else
            {
                // collision
            }
        }

        if (ch == keyD)
        {
            // Right border
            if(head->y < 8)
            {
                // Empty current pos
                arr[head->x][head->y] = 0;
                arr[body->x][body->y] = 0;
                // Set new pos
                body->x = head->x;
                body->y = head->y;
                head->y += 1;
                arr[head->x][head->y] = 9;
                arr[body->x][body->y] = 2;
            } else
            {
                // collision
            }
        }

        if (ch == keyA)
        {
            // Left border
            if(head->y > 1)
            {
                // Empty current pos
                arr[head->x][head->y] = 0;
                arr[body->x][body->y] = 0;
                
                // Set new pos
                body->x = head->x;
                body->y = head->y;
                head->y -= 1;
                arr[head->x][head->y] = 9;
                arr[body->x][body->y] = 2;
            } else
            {
                // collision
            }
        }

        drawDesk(arr, score);
        // 1000000 = 1 sec
        usleep(1000000);

        // Update direction if key is pressed
        if (_kbhit())
        {
            head->direction = getchar();
        }
    }

    input_off();

    return 0;
}
