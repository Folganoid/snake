#include <iostream>
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>

using namespace std;

bool gameOver;
const int width = 20;
const int height = 20;
int x,y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

unsigned int microseconds = 300000;

static struct termios initial_settings, new_settings;
 
static int peek_character = -1;
 
void init_keyboard()
{
    tcgetattr(0,&initial_settings);
    new_settings = initial_settings;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;
    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &new_settings);
}
 
void close_keyboard()
{
    tcsetattr(0, TCSANOW, &initial_settings);
}
 
int _kbhit()
{
    unsigned char ch;
    int nread;
 
    if (peek_character != -1) return 1;
    new_settings.c_cc[VMIN]=0;
    tcsetattr(0, TCSANOW, &new_settings);
    nread = read(0,&ch,1);
    new_settings.c_cc[VMIN]=1;
    tcsetattr(0, TCSANOW, &new_settings);
    if(nread == 1)
    {
        peek_character = ch;
        return 1;
    }
    return 0;
}
 
int _getch()
{
    char ch;
 
    if(peek_character != -1)
    {
        ch = peek_character;
        peek_character = -1;
        return ch;
    }
    read(0,&ch,1);
    return ch;
}
 
int _putch(int c) {
    putchar(c);
    fflush(stdout);
    return c;
}

void Setup() {
    gameOver = false;
    dir = STOP;
    x = width / 2 - 1;
    y = height / 2 - 1;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
}

void Draw() {

    system("clear");

    //top
    for (int i = 0 ; i < width; i++) { 
        cout << "#";
    }
    cout << endl;

    bool Prt;

    for (int i = 0 ; i < height; i++) { 
        for (int j = 0 ; j < width; j++) {
            if (j == 0 || j == width - 1)
            {
                cout << "#";
            } else {

                if (i==y && j == x) {
                    cout << "0";
                    Prt = true;
                } else if (i == fruitY && j == fruitX) {
                    cout << "F";
                    Prt = true;
                } else {

                    Prt = false;

                    for (int k = 0; k < nTail; k++) {
                        if (tailX[k] == j && tailY[k] == i) {
                            Prt = true;
                            cout << "o";
                        }
                    }
                }
                    if (!Prt) cout << " ";
            }
        }
        cout << endl;
    }

    //bottom
    for (int i = 0 ; i < width + 1; i++) { 
        cout << "#";
    }
    cout << endl;
    cout << "Score: ";

    // std::ostringstream ostr;
    // ostr << score;
    // std::string str = ostr.str();
    // const char* cstr = str.c_str();

    cout << score << endl;
}

void Input() {

if (_kbhit()) {
            int ch = _getch();
            _putch(ch);
            switch (ch)
            {
            case 97:
                dir = LEFT;
                break;
            case 100:
                dir = RIGHT;
                break;
            case 119:
                dir = UP;
                break;
            case 115:
                dir = DOWN;
                break;
            case 120:
                gameOver = true;
                break;
            default:
                break;
        }
        }
}

void Logic() {

    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir)
    {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
    }
    //dir = STOP;

    if (x > width || x < 0 || y > height || y < 0)
    {
        gameOver = true;
    }

    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y) {
            gameOver = true;
            break;
        }
    }

    if (x==fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
    }
}

int main()
{
    Setup();
    init_keyboard();
    while (!gameOver) {

        usleep(microseconds);

        Draw();
        Input();
        Logic();
   }
   close_keyboard();

    return 0;
}