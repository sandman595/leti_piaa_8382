
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

stack <pair<int, bool>> availableSquaresStack; // ñòåê íà êîòîðîì õðàíÿòñÿ ðàçìåðû êâàäðàòîâ,êîòîðûå ìîãóò áûòü âñòàâëåíû â òî÷êó(êîîðäèíàòû òî÷êè ïîëó÷àþòñÿ èç ñòðóêòóðû Figure) è áûë ëè îí óæå ïîñòàâëåíû â ôèãóðó.

stack <Square> currentSolutions; // ñòåê, â êîòîðîì õðàíèòñÿ òåêóùeå ìèíèìàëüíîå ðàçáèåíèå ôèãóðû

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


void maxInsert(int x, int y)// ôóíêöèè ïåðåäàåòñÿ òî÷êà
{                           // ôóíêèÿ èùåò ðàçìåðû âñåõ êâàäðàòîâ, êîòîðûå
    int size;		        // ìîæíî âñòàâèòü â òî÷êó è ïðè ýòîì íå
                            // ýòîì íå ïåðåêðûòü äðóãèå êâàäðàòû, êîòîðûå
                            //óæå ñòîÿò â ôèãóðå
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


void clear(Square a) // ôóíêèÿ, êîòîðàÿ óäàëÿåò êâàäðàò èç ôèãóðû
{
    figure.summary -= a.size * a.size;

    for (int i = a.y; i < a.y + a.size; i++)
        for (int j = a.x; j < a.x + a.size; j++)
            figure.rectangle[i][j] = 0;
}


void insert(int x, int y, int size, int color) // ôóíêöèÿ, êîòîðàÿ âñòàâëÿåò êâàäðàò â ôèãóðó
{
    figure.summary += size * size;

    for (int i = y; i < y + size; i++)
        for (int j = x; j < x + size; j++)
            figure.rectangle[i][j] = color;
}


pair<int, int> tiling() // ôóíêöèÿ, êîòîðàÿ ïåðåáèðàåò âñå âîçìîæíûå âàðèàíòû ìîùåíèÿ êâàäðàòà è èùåò ñðåäè íèõ ìèíèìàëüíîå
{
    int x = 0, y = 0;
    int color = 1;
    int numberColorings = 0;
    int numberSquares = figure.N * figure.M + 1;
    bool flag;

    maxInsert(x, y);

    do{ // öèêë, êîòîðûé ðàáîòàåò ïîêà íå áóäóò ïðîâåðåííû âñå êîìáèíàöèè êâàäðàòîâ
        flag = false;

        for (y = 0; y < figure.N; y++) { // öèêëû, êîòîðûå èùóò ïåðâóþ, ïóñòóþ êëåòêó è âñòàâëÿþò â íåå êâàäðàòû
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
        // åñëè â ôèãóðó âñòàâëåííî êâàäðàòîâ áîëüøå, ÷åì óæå â êàêîì-òî èç èçâåñòíûõ ðàçáèåíèé, òî ïðîèñõîäèòñÿ îòêàò ê äðóãèì âàðèàíòàì ðàçáèÿíèÿ
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
        // åñëè ôèãóðà áûëà ïîëíîñòü ïîêðûòà, òîãäà ïðîâåðÿåòñÿ ìèíèìàëüíîå ëè ýòî ðàçáèåíèå, åñëè äà, òî îíî çàïîìèíàåòñÿ, åñëè ðàçáèåíèå íà ñòîëüêî êâàäðàòîâ óæå ñóùåòñâóåò, òî ñ÷åò÷èê âàðèàíòîí ðàçáèåíèå óâåëè÷èâàåòñÿ
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
