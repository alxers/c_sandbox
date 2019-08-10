#include <stdio.h>
#include <termios.h>
#include <unistd.h>

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

void drawDesk(int map[10][10])
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
}

struct Node {
    int x;
    int y;
    struct node *next;

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
    drawDesk(arr);

    int ch;
    int keyW = 119;
    int keyD = 100;
    int keyA = 97;
    int keyS = 115;

    struct Node head;
    struct Node body;

    head.x = 4;
    head.y = 4;

    arr[head.x][head.y] = 9;

    while(1)
    {
        ch = getchar();
        if (ch == keyW)
        {
            // Upper border
            if(head.x > 1)
            {
                // Empty current pos
                arr[head.x][head.y] = 0;
                
                // Set new pos
                head.x -= 1;
                arr[head.x][head.y] = 9;
            } else
            {
                // collision
            }
        }

        if (ch == keyS)
        {
            // Upper border
            if(head.x < 8)
            {
                // Empty current pos
                arr[head.x][head.y] = 0;
                
                // Set new pos
                head.x += 1;
                arr[head.x][head.y] = 9;
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
                arr[head.x][head.y] = 0;
                
                // Set new pos
                head.y += 1;
                arr[head.x][head.y] = 9;
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
                arr[head.x][head.y] = 0;
                
                // Set new pos
                head.y -= 1;
                arr[head.x][head.y] = 9;
            } else
            {
                // collision
            }
        }

        drawDesk(arr);
        usleep(100);
    }

    input_off();

    return 0;
}
