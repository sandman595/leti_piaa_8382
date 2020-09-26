#include <iostream>
#include <limits.h>
#include <vector>
#include <stack>
#include <iomanip>
#include <algorithm>

using namespace std;
#define LINE "<)===================================(>"
struct answer {
    char from;  //�������, �� ������� ������� �����
    char to;    //�������, � ������� ������ �����
    int weight;      //��� �����
};

bool DFS(vector<vector<int> > rGraph, int s, int t, vector <int>& parent, int V, string nodeNames);  //���������� true, ���� ���������� ���� �� ������ s � ����� t � ����� rGraph, ����� ��������� ������ parent
int FFA(vector<vector<int> >& graph, vector<vector<int> >& rGraph, int s, int t, int V, string nodeNames); //���������� ������������ ����� �� ������ s � ����� t
void printGraph(vector<vector<int> > graph, int V, string nodeNames);   //��� ������ �����
bool cmpForStepik(answer a, answer b);   //���������� ��� ���������� ������

int main()
{
    setlocale(LC_ALL, "RUS");
    int N = 0;        //���������� ��������������� ���� �����
    char start;   //�����
    char finish;  //����

    string nodeNames;  //�������� �����
    string from;
    string to;
    vector <int> w;

    cin >> N;
    cin >> start;
    cin >> finish;

    if (start == finish) {
        cout << "���� � ����� �� ����� ���� ����� ��������" << endl;
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
        for (int j = 0; j < N; j++) {         //����� ���� �����, ������� �� ������� nodeNames[k]
            if (from[j] == nodeNames[k]) {
                tmp.push_back(j);
            }
        }

        vector <int> nodeTmp;
        for (size_t i = 0; i < tmp.size(); i++) {    //����� � ������ nodeNames[k] �������, � ������� ����� ����� �� ������� tmp
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
        vector <int> indices;  //�������
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

    cout << "����� ��� �������:" << endl;
    cout << maxFlow << endl;
    for (size_t i = 0; i < forStepik.size(); i++) cout << forStepik[i].from << " " << forStepik[i].to << " " << forStepik[i].weight << endl;
    cout << LINE << endl;
    cout << "������������ ����� � ���� = " << maxFlow << endl;

    return 0;
}

bool DFS(vector<vector<int> > rGraph, int s, int t, vector <int>& parent, int V, string nodeNames)   //������� ������ ���� � �������
{
    vector <bool> visited(V, 0);    //������ ���������� ������ (���� 0, �� �� ��������)

    stack <int> st;   //��������� ����, � ������� �������� �����, � ��������� ������� ���������� ��� ����������
    st.push(s);
    visited[s] = true;
    parent[s] = -1;

    cout << "����� ����..." << endl;
    while (!st.empty()) {       //���������, ���� ���� �� ����
        int i = st.top();    //��������� ������ �������
        st.pop();

        cout << "������� " << nodeNames[i] << ":" << endl;

        for (int j = 0; j < V; j++) {  //���� ������� ������� �� ���������� � ����� ����� � �������������� ��������
            if (visited[j] == false && rGraph[i][j] > 0) {
                st.push(j);
                parent[j] = i;
                visited[j] = true;
                cout << "\t���������� ������� ������� " << nodeNames[j] << " � ����" << endl;
            }
        }

        cout << "���������� ���������� ��� ������� " << nodeNames[i] << endl;
    }

    cout << endl;
    if (visited[t] == true) {
        cout << "������ ���� ";

        string str;
        for (int i = t; i != s; i = parent[i])
            str += nodeNames[i];

        str += nodeNames[s];

        for (int i = str.length() - 1; i >= 0; i--)
            cout << str[i];

        cout << endl;
    } else cout << "���� �� ��� ������" << endl;

    return (visited[t] == true);   //���� ��� ��������� ����, �� ������������ true, ����� - false
}

int FFA(vector<vector<int> >& graph, vector<vector<int> >& rGraph, int s, int t, int V, string nodeNames)
{
    int u, v;

    for (u = 0; u < V; u++)
        for (v = 0; v < V; v++) {
            rGraph[u][v] = graph[u][v];
            graph[u][v] = 0;
        }

    vector <int> parent(V, 0);  //���� ������ ����������� �������� BFS � ������ ��� �������� ����

    int max_flow = 0;  //���������� ����� = 0

    while (DFS(rGraph, s, t, parent, V, nodeNames)) {  //������������� �����, ���� ���� ���� �� ������ � �����

        int path_flow = INT_MAX;
        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            path_flow = min(path_flow, rGraph[u][v]);
        }

        cout << "����� �� ����: " << path_flow << endl;
        cout << LINE << endl;

        for (v = t; v != s; v = parent[v]) {		//���������� ���������� ����������� ������� �����
            u = parent[v];
            rGraph[u][v] -= path_flow;
            rGraph[v][u] += path_flow;

            graph[u][v] += path_flow;
            graph[v][u] -= path_flow;
        }

        max_flow += path_flow;

    }

    cout << "���������� ���������..." << endl;
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
