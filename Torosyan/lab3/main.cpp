#include <iostream>
#include <limits.h>
#include <vector>
#include <stack>
#include <iomanip>
#include <algorithm>

using namespace std;
#define LINE "<)===================================(>"
struct answer {
    char from;  //вершина, из которой выходит ребро
    char to;    //вершина, в которую входит ребро
    int weight;      //вес ребра
};

bool DFS(vector<vector<int> > rGraph, int s, int t, vector <int>& parent, int V, string nodeNames);  //возвращает true, если существует путь от истока s к стоку t в графе rGraph, также заполн€ет массив parent
int FFA(vector<vector<int> >& graph, vector<vector<int> >& rGraph, int s, int t, int V, string nodeNames); //возвращает максимальный поток от истока s к стоку t
void printGraph(vector<vector<int> > graph, int V, string nodeNames);   //дл€ печати графа
bool cmpForStepik(answer a, answer b);   //компаратор дл€ сортировки вершин

int main()
{
    setlocale(LC_ALL, "RUS");
    int N = 0;        //количество ориентированных рЄбер графа
    char start;   //исток
    char finish;  //сток

    string nodeNames;  //названи€ узлов
    string from;
    string to;
    vector <int> w;

    cin >> N;
    cin >> start;
    cin >> finish;

    if (start == finish) {
        cout << "—ток и исток не могут быть одной вершиной" << endl;
        return 0;
    }

    char tmpFrom;
    char tmpTo;
    int tmpW;
    nodeNames += start;
    for (int i = 0; i < N; i++) {
        cin >> tmpFrom >> tmpTo >> tmpW;
        from += tmpFrom;
        to += tmpTo;
        w.push_back(tmpW);
        if (nodeNames.length() == 0) nodeNames += tmpTo;
        else if (nodeNames.find(tmpTo) == string::npos) nodeNames += tmpTo;
    }

    sort(nodeNames.begin(), nodeNames.end());

    int V = nodeNames.length();

    vector<vector<int> > graph(V, vector<int>(V, 0));
    string findInNodeNames;

    for (size_t k = 0; k < nodeNames.length(); k++) {
        vector <int> tmp;
        for (int j = 0; j < N; j++) {         //поиск всех ребер, ведущих из вершины nodeNames[k]
            if (from[j] == nodeNames[k]) {
                tmp.push_back(j);
            }
        }

        vector <int> nodeTmp;
        for (size_t i = 0; i < tmp.size(); i++) {    //поиск в строке nodeNames[k] вершины, в которую ведут ребра из вектора tmp
            for (size_t j = 0; j < nodeNames.length(); j++) {
                if (nodeNames[j] == to[tmp[i]])
                    nodeTmp.push_back(j);
            }
        }

        for (size_t i = 0; i < tmp.size(); i++) {
            graph[k][nodeTmp[i]] = w[tmp[i]];
        }
    }

    int startIndex = 0;
    int finishIndex = 0;
    for (int i = 0; i < V; i++) {
        if (nodeNames[i] == start) startIndex = i;
        else if (nodeNames[i] == finish) finishIndex = i;
    }

    vector<vector<int> > rGraph(V, vector<int>(V, 0));
    int maxFlow = FFA(graph, rGraph, startIndex, finishIndex, V, nodeNames);

    vector <answer> forStepik;

    for (int i = 0; i < V; i++) {
        vector <int> indices;  //индексы
        for (int j = 0; j < N; j++) {
            if (nodeNames[i] == from[j]) indices.push_back(j);
        }

        for (size_t j = 0; j < indices.size(); j++) {
            answer ans;
            ans.from = from[indices[j]];
            ans.to = to[indices[j]];

            int tmpF = 0;
            int tmpT = 0;
            for (int k = 0; k < V; k++) {
                if (nodeNames[k] == from[indices[j]]) tmpF = k;
                else if (nodeNames[k] == to[indices[j]]) tmpT = k;
            }

            if (graph[tmpT][tmpF] >= 0) ans.weight = 0;
            else ans.weight = abs(graph[tmpT][tmpF]);
            forStepik.push_back(ans);
        }
    }

    sort(forStepik.begin(), forStepik.end(), cmpForStepik);

    cout << "ќтвет дл€ степика:" << endl;
    cout << maxFlow << endl;
    for (size_t i = 0; i < forStepik.size(); i++) cout << forStepik[i].from << " " << forStepik[i].to << " " << forStepik[i].weight << endl;
    cout << LINE << endl;
    cout << "ћаксимальный поток в сети = " << maxFlow << endl;

    return 0;
}

bool DFS(vector<vector<int> > rGraph, int s, int t, vector <int>& parent, int V, string nodeNames)   //функци€ поиска пути в глубину
{
    vector <bool> visited(V, 0);    //вектор посещенных вершин (если 0, то не посещена)

    stack <int> st;   //создаетс€ стек, в который кладетс€ исток, и начальна€ вершина помечаетс€ как посещенна€
    st.push(s);
    visited[s] = true;
    parent[s] = -1;

    cout << "ѕоиск пути..." << endl;
    while (!st.empty()) {       //обработка, пока стек не пуст
        int i = st.top();    //обработка первой вершины
        st.pop();

        cout << "¬ершина " << nodeNames[i] << ":" << endl;

        for (int j = 0; j < V; j++) {  //если смежна€ вершина не обработана и имеет ребро с обрабатываемой вершиной
            if (visited[j] == false && rGraph[i][j] > 0) {
                st.push(j);
                parent[j] = i;
                visited[j] = true;
                cout << "\tƒобавление смежной вершины " << nodeNames[j] << " в стек" << endl;
            }
        }

        cout << "«авершение вычислений дл€ вершины " << nodeNames[i] << endl;
    }

    cout << endl;
    if (visited[t] == true) {
        cout << "Ќайден путь ";

        string str;
        for (int i = t; i != s; i = parent[i])
            str += nodeNames[i];

        str += nodeNames[s];

        for (int i = str.length() - 1; i >= 0; i--)
            cout << str[i];

        cout << endl;
    } else cout << "ѕуть не был найден" << endl;

    return (visited[t] == true);   //если был достигнут сток, то возвращаетс€ true, иначе - false
}

int FFA(vector<vector<int> >& graph, vector<vector<int> >& rGraph, int s, int t, int V, string nodeNames)
{
    int u, v;

    for (u = 0; u < V; u++)
        for (v = 0; v < V; v++) {
            rGraph[u][v] = graph[u][v];
            graph[u][v] = 0;
        }

    vector <int> parent(V, 0);  //этот массив заполн€етс€ функцией BFS и создан дл€ хранени€ пути

    int max_flow = 0;  //изначально поток = 0

    while (DFS(rGraph, s, t, parent, V, nodeNames)) {  //увеличиваетс€ поток, пока есть путь от истока к стоку

        int path_flow = INT_MAX;
        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            path_flow = min(path_flow, rGraph[u][v]);
        }

        cout << "ѕоток на пути: " << path_flow << endl;
        cout << LINE << endl;

        for (v = t; v != s; v = parent[v]) {		//обновление пропускной способности каждого ребра
            u = parent[v];
            rGraph[u][v] -= path_flow;
            rGraph[v][u] += path_flow;

            graph[u][v] += path_flow;
            graph[v][u] -= path_flow;
        }

        max_flow += path_flow;

    }

    cout << "«авершение алгоритма..." << endl;
    cout << LINE << endl;
    return max_flow;
}

void printGraph(vector<vector<int> > graph, int V, string nodeNames)
{
    cout << " ";
    for (int i = 0; i < V; i++) {
        cout << setw(3) << nodeNames[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < V; i++) {
        cout << nodeNames[i];
        for (int j = 0; j < V; j++) cout << setw(3) << graph[i][j] << " ";
        cout << endl;
    }
    cout << endl;
}

bool cmpForStepik(answer a, answer b)
{
    if (a.from < b.from) return true;
    else if (a.from == b.from) {
        if (a.to < b.to) return true;
    }

    return false;
}
