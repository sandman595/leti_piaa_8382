
#include <iostream>
#include <algorithm>
#include <stack>
#ifdef _WIN32
#include <windows.h>
#define cls() system("cls")
#else
#define cls() ;
#endif // _WIN32
using namespace std;

struct Square{
    int x;
    int y;
    int size;
};

struct Figure {
    int** rectangle;
    int summary;
    int N, M;
    stack <Square> squareStack;
} figure;

stack <pair<int, bool>> availableSquaresStack; // стек на котором хранятся размеры квадратов,которые могут быть вставлены в точку(координаты точки получаются из структуры Figure) и был ли он уже поставлены в фигуру.

stack <Square> currentSolutions; // стек, в котором хранится текущeе минимальное разбиение фигуры

void print()
{
    for (int i = 0; i < figure.N; i++) {
        for (int j = 0; j < figure.M; j++) {
            cout << figure.rectangle[i][j] << ' ';
        }
        cout << endl;
    }
    //getchar();
    //cls();
}


void maxInsert(int x, int y)// функции передается точка
{                           // функия ищет размеры всех квадратов, которые
    int size;		        // можно вставить в точку и при этом не
                            // этом не перекрыть другие квадраты, которые
                            //уже стоят в фигуре
    for (size = 1; size <= figure.N - 1; size++)
    {
        if (y + size > figure.N)
            return;

        if (x + size > figure.M)
            return;

        for (int i = y; i < y + size; i++)
            for (int j = x; j < x + size; j++)
            {
                if (figure.rectangle[i][j] != 0)
                    return;
            }
        availableSquaresStack.push(make_pair(size, false));
    }
}


void clear(Square a) // функия, которая удаляет квадрат из фигуры
{
    figure.summary -= a.size * a.size;

    for (int i = a.y; i < a.y + a.size; i++)
        for (int j = a.x; j < a.x + a.size; j++)
            figure.rectangle[i][j] = 0;
}


void insert(int x, int y, int size, int color) // функция, которая вставляет квадрат в фигуру
{
    figure.summary += size * size;

    for (int i = y; i < y + size; i++)
        for (int j = x; j < x + size; j++)
            figure.rectangle[i][j] = color;
}


pair<int, int> tiling() // функция, которая перебирает все возможные варианты мощения квадрата и ищет среди них минимальное
{
    int x = 0, y = 0;
    int color = 1;
    int numberColorings = 0;
    int numberSquares = figure.N * figure.M + 1;
    bool flag;

    maxInsert(x, y);

    do{ // цикл, который работает пока не будут проверенны все комбинации квадратов
        flag = false;

        for (y = 0; y < figure.N; y++) { // циклы, которые ищут первую, пустую клетку и вставляют в нее квадраты
            for (x = 0; x < figure.M; x++)
            {
                if (figure.rectangle[y][x] == 0)
                {
                    flag = true;

                    if (availableSquaresStack.top().second)
                        maxInsert(x, y);

                    insert(x, y, availableSquaresStack.top().first, color);
                    figure.squareStack.push(Square{x, y, availableSquaresStack.top().first});
                    availableSquaresStack.top().second = true;

                    color++;

                    //cout << "Insert a new square into the Figure"<< endl;
                    //print();

                    break;
                }
            }
            if (flag)
                break;
        }
        // если в фигуру вставленно квадратов больше, чем уже в каком-то из известных разбиений, то происходится откат к другим вариантам разбияния
        if (  color - 1 == numberSquares && figure.summary != figure.M * figure.N)
        {
            //cout << "This can be done with less amount of squares. Backtracking...\n";
            while ( !availableSquaresStack.empty() && availableSquaresStack.top().second)
            {
                availableSquaresStack.pop();
                clear(figure.squareStack.top());
                figure.squareStack.pop();
                color--;

            }
            //print();
        }
        // если фигура была полность покрыта, тогда проверяется минимальное ли это разбиение, если да, то оно запоминается, если разбиение на столько квадратов уже сущетсвует, то счетчик вариантон разбиение увеличивается
        if ( !availableSquaresStack.empty() && figure.summary == figure.M * figure.N)
        {
            //cout << "The figure is tiled with the number of squares less than or equal to the current split. Saving the alignment of squares and wiping out.\n";
            if (numberSquares == color - 1)
            {
                numberColorings++;
            } else{
                currentSolutions = figure.squareStack;
                numberSquares = color - 1;
                numberColorings = 1;
            }


            while ( !availableSquaresStack.empty() && availableSquaresStack.top().second )
            {
                availableSquaresStack.pop();
                clear(figure.squareStack.top());
                figure.squareStack.pop();
                color--;
            }
            //print();

        }
    }while (!availableSquaresStack.empty());

    return make_pair(numberSquares, numberColorings);
}


int main() {
    pair <int, int> ans;
    cout << "Enter the rectangle size:\n";
    cin >> figure.N >> figure.M;

    if (figure.N > figure.M)
    {
        swap(figure.N, figure.M);
    }

    figure.rectangle = new int * [figure.N];

    for (int i = 0; i < figure.N; i++)
    {
        figure.rectangle[i] = new int[figure.M];
    }

    for (int i = 0; i < figure.N; i++)
        for (int j = 0; j < figure.M; j++)
        {
            figure.rectangle[i][j] = 0;
        }

    ans = tiling();

    cout << "Minimum number of squares: " << ans.first << endl
         << "Number of minimum alignments: " << ans.second << endl;
    cout << "The squareStack of the inserted squares and their size:\n";
    for (int i = 0; i < ans.first; i++)
    {
        cout << currentSolutions.top().x << ' ' << currentSolutions.top().y << ' ' << currentSolutions.top().size << endl;
        currentSolutions.pop();
    }

    return 0;
}
