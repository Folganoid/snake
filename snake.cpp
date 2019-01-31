#include <iostream>
#include <curses.h>
#include <stdio.h>
#include <string>
#include <sstream>

using namespace std;

bool gameOver;
const int width = 20;
const int height = 20;
int x,y, fruitX, fruitY, score;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

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

    //system("clear");

    //top
    for (int i = 0 ; i < width; i++) { 
        printw("#");
    }
    printw("\n");

    for (int i = 0 ; i < height; i++) { 
        for (int j = 0 ; j < width +1 ; j++) {
            if (j == 0 || j == width - 1)
            {
                printw("#");
            } else {

                if (i==y && j == x)
                    printw("0");
                else if (i == fruitY && j == fruitX)    
                    printw("F");
                else 
                    printw(" ");
            }
        }
        printw("\n");
    }

    //bottom
    for (int i = 0 ; i < width + 1; i++) { 
        printw("#");
    }
    printw("\n");
    printw("Score: ");

    std::ostringstream ostr;
    ostr << score;
    std::string str = ostr.str();
    const char* cstr = str.c_str();

    printw(cstr);
    printw("\n");
}

void Input() {

        switch (getch())
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

void Logic() {
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
    dir = STOP;

    if (x > width || x < 0 || y > height || y < 0)
    {
        gameOver = true;
    }
    if (x==fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
    }
}

int main()
{

    initscr();
    cbreak();
    noecho();
    scrollok(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    Setup();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
   }

    return 0;
}