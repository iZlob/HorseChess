#include <iostream>
#include <iomanip>
#include <time.h>
#include "windows.h"
#include <conio.h>
using namespace std;

int get_x_step() {
    int x = rand() % 4;
    if (x == 0)
        x = -2;
    else if (x == 1)
        x = -1;
    else if (x == 2)
        x = 1;
    else if (x == 3)
        x = 2;
    return x;
}

int get_y_step(int x) {
    int y;
    if (x == 2 || x == -2) {
        y = rand() % 2;
        if (y == 0)
            y = -1;
        else
            y = 1;
    }
    else
        if (x == 1 || x == -1) {
            y = rand() % 2;
            if (y == 0)
                y = -2;
            else
                y = 2;
        }
    return y;
}

int search_point(int x, int y, int chessboard[][8], int s, int step) {//ищем точку разворота

    int xx = get_x_step();
    int yy = get_y_step(xx);

    if (((x + xx) < s && (x + xx) >= 0) && ((y + yy) < s && (y + yy) >= 0) && chessboard[x + xx][y + yy] != step - 1) {
        for (int i = 0; i < s; i++) {
            for (int j = 0; j < s; j++) {
                if (chessboard[i][j] == chessboard[x + xx][y + yy] + 1) {
                    if ((i + 1 < s && j + 2 < s && chessboard[i + 1][j + 2] == 0) || (i + 2 < s && j + 1 < s && chessboard[i + 2][j + 1] == 0) ||
                        (i - 1 >= 0 && j - 2 >= 0 && chessboard[i - 1][j - 2] == 0) || (i - 2 >= 0 && j - 1 >= 0 && chessboard[i - 2][j - 1] == 0) ||
                        (i + 1 < s && j - 2 >= 0 && chessboard[i + 1][j - 2] == 0) || (i + 2 < s && j - 1 >= 0 && chessboard[i + 2][j - 1] == 0) ||
                        (i - 1 >= 0 && j + 2 < s && chessboard[i - 1][j + 2] == 0) || (i - 2 >= 0 && j + 1 < s && chessboard[i - 2][j + 1] == 0))
                        return chessboard[i][j];
                    else
                        return chessboard[i][j];
                }
            }
        }
    }
    else
        search_point(x, y, chessboard, s, step);
}

void turn(int chessboard[][8], int s, int step, int point) {//разворачиваем ходы по методу Эйлера
    for (int i = 0; i < s; i++)
        for (int j = 0; j < s; j++)
            if (chessboard[i][j] >= point && chessboard[i][j] <= step)
                chessboard[i][j] = step - chessboard[i][j] + point;
}

void horse_move(int x, int y, int chessboard[][8], int s, int step) {//шагаем по полю
    if (step < 64) {
        int xx = get_x_step();
        int yy = get_y_step(xx);

        if (((x + xx) < s && (x + xx) >= 0) && ((y + yy) < s && (y + yy) >= 0) && chessboard[x + xx][y + yy] == 0) {
            x += xx;
            y += yy;
            chessboard[x][y] = ++step;
            horse_move(x, y, chessboard, s, step);
        }
        else
            if ((x + 1 < s && y + 2 < s && chessboard[x + 1][y + 2] == 0) || (x + 2 < s && y + 1 < s && chessboard[x + 2][y + 1] == 0) ||
                (x - 1 >= 0 && y - 2 >= 0 && chessboard[x - 1][y - 2] == 0) || (x - 2 >= 0 && y - 1 >= 0 && chessboard[x - 2][y - 1] == 0) ||
                (x + 1 < s && y - 2 >= 0 && chessboard[x + 1][y - 2] == 0) || (x + 2 < s && y - 1 >= 0 && chessboard[x + 2][y - 1] == 0) ||
                (x - 1 >= 0 && y + 2 < s && chessboard[x - 1][y + 2] == 0) || (x - 2 >= 0 && y + 1 < s && chessboard[x - 2][y + 1] == 0))
                horse_move(x, y, chessboard, s, step);
            else
                //если оказались в тупике, используем метод Эйлера, разворачивая массив                    
            {
                int point = search_point(x, y, chessboard, s, step);
                if (point != 0) {
                    for (int i = 0; i < s; i++)
                        for (int j = 0; j < s; j++)
                            if (chessboard[i][j] == point) {
                                x = i;
                                y = j;
                            }
                    //cout << "point = " << point << "   x= " << x << "   y= " << y << "   step = " << step << endl;
                    turn(chessboard, s, step, point);
                    horse_move(x, y, chessboard, s, step);
                }
                else
                    horse_move(x, y, chessboard, s, step);
            }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand(time(NULL));
    const int s = 8;
    int chessboard[s][s]{};
    int x, y;
    int step = 1;

    cout << "Задайте положение коня на шахматной доске. (введите координаты от 1 до 8): " << endl;
    cin >> x >> y;

    if (x <= 8 && x >= 1 && y <= 8 && y >= 1) {
        x -= 1;
        y -= 1;
    }
    else {
        cout << "Вы задали не верные координаты. Попробуйте снова!" << endl;
        return 0;
    }

    chessboard[x][y] = step;
    horse_move(x, y, chessboard, s, step);
    system("cls");

    for (int i = 0; i < s; i++) {
        for (int j = 0; j < s; j++)
            cout << setw(4) << chessboard[i][j];
        cout << endl;
    }
    cout << "\nНажмите на любую клавишу, что бы увидеть ходы..." << endl;
    cout << "\nESC - если не хотите видеть..." << endl;
    if (_getch() == 27)
        return 0;
    else {
        system("cls");
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        step = 1;
        int showchess[s][s]{};
        while (step <= 64) {
            for (int i = 0; i < s; i++) {
                for (int j = 0; j < s; j++)
                    if (chessboard[i][j] == step) {
                        showchess[i][j] = chessboard[i][j];
                        SetConsoleTextAttribute(hConsole, 9);
                        cout << setw(4) << showchess[i][j];
                        SetConsoleTextAttribute(hConsole, 15);
                    }
                    else cout << setw(4) << showchess[i][j];
                cout << endl;
            }
            step++;
            Sleep(500);
            system("cls");
        }
    }

    return 0;
}

