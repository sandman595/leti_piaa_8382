#include <iostream>
#include <string>
#include <vector>
#include <map>


struct Vertex
{
    std::map<char, int> next;   //��������� ��������� �� ������� char � ������� � ������� int
    std::map<char, int> go;     //������ ��������� (���������� �������� � ������� ��������), ������������ ��� ���������� ���������� ������
    bool isTerminal = false;    //�������� �� ������������ (��������) �������� (�� ������� ������������� �������)
    int prev;                   //����� ���������� ������� (��������)
    char prevChar;              //������, �� �������� ������ � �������
    int suffix;                 //���������� ������
    std::vector<int> number;    //����� �� ����� ��������� ������� ������������� �� ���� ������� (������ ��� ������)
    int deep;                   //������� � ����, ������ ����� ������-��������, ���������������� � ���� ������������ �������
};

int go(int index, char symb, std::vector<Vertex>& vertexArr);

void addString(const std::string& str, std::vector<Vertex>& vertexArr, int& count)  //������� ���������� ������-�������� � ���
{
    std::cout << "Adding string \"" << str << "\" in the bohr\n";
    if (str.empty())
        return;

    int current = 0;
    for (int i = 0; i < str.size(); i++) {
        std::cout << "\tCurrent symbol: \'" << str[i] << "\'\n";
        std::cout << "\tCurrent vertex: " << current << "\n";
        if (vertexArr[current].next.find(str[i]) == vertexArr[current].next.end()) {    //���� ������� �� ������� �� ��������� �
            std::cout << "\tWay through \'" << str[i] << "\' wasn't found. Adding new vertex with number " << vertexArr.size() << "\n";
            Vertex ver;                                                                 //�������� ��������� �� ����� ����, ��
            ver.suffix = -1;                                                            //������ ����� �������
            ver.prev = current;
            std::cout << "\t*previous vertex is " << current << ", the symbol of incoming path \'" << str[i] << "\'\n";
            ver.prevChar = str[i];
            vertexArr.push_back(ver);
            vertexArr[current].next[str[i]] = vertexArr.size() - 1;                     //� ���������� ������� ������� � ���
        }                                                                               //�� �������� �������
        else{
            std::cout << "The way through the symbol exist\n";
        }
        std::cout << std::endl;
        current = vertexArr[current].next[str[i]];                                      //������� � ��������� �������
    }

    std::cout << "The number of this pattern is " << count + 1 << "\n";
    std::cout << "Vertex " << current << " is terminal, deep of the vertex is " << str.size() << "\n\n";
    vertexArr[current].number.push_back(++count);       //������������� ����� ���������� ��������,
    vertexArr[current].isTerminal = true;               //������������ �������
    vertexArr[current].deep = str.size();               //�������
}


int getSuffix(int index, std::vector<Vertex>& vertexArr)    //������� ������ ���������� ������ ��� ������� index
{
    std::cout << "\t\t\tGetting suffix-link from vertex " << index << "\n";
    if (vertexArr[index].suffix == -1) {                    //���� ���������� ������ ��� �� ���� �������
        if (index == 0 || vertexArr[index].prev == 0) {     //���� ������� - ������ ��� ��� �����
            vertexArr[index].suffix = 0;
            (index == 0) ? std::cout << "\t\t\tThis is root, suffix-link vertex = 0\n" : std::cout << "\t\t\tThis is a vertex with deep = 1, suffix-link = 0\n";
        }
        else {                                              //����������� ����� ����. ������. �������� ������ �������� � ���������
            std::cout << "\t\t\tFinding suffix-link from suffix of parent-vertex (" << vertexArr[index].prev << ") through " << vertexArr[index].prevChar << "\n";
            vertexArr[index].suffix = go(getSuffix(vertexArr[index].prev, vertexArr), vertexArr[index].prevChar, vertexArr);
        }                                                   //�� �� ������� �� �������, �� �������� ������ � �������, ���
    }                                                       //������� � ������ ����. ������
    std::cout << "\t\t\tSuffix-link from vertex " << index << " is " << vertexArr[index].suffix << "\n\n";
    return vertexArr[index].suffix;
}


int go(int index, char symb, std::vector<Vertex>& vertexArr)      //������� �������� �� ������� index �� ������� symb. ���� ������ �������
{                                                                 //����������, ������� �� ������
    std::cout << "\t\t\t*Finding the way from " << index << " through \'" << symb << "\'\n";
    if (vertexArr[index].go.find(symb) == vertexArr[index].go.end()) {            //���� ���� � ������� ��������� ��� �� ��� ������
        if (vertexArr[index].next.find(symb) != vertexArr[index].next.end()) {    //���� ������ ������ ������� �� ������� � ����
            vertexArr[index].go[symb] = vertexArr[index].next[symb];              //��������� � ��������� ��������� ���������
        }
        else {                                                                //���� ������� �������� ���, �������� ����. ������
            if (index == 0)                                                   //� ���� ������� �� ���������� ������ �� ��������� �������
                std::cout << "\t\t\t*This is root\n";
            else
                std::cout << "\t\t\t*No straight path. Finding the way from suffix-link of this vertex through \'" << symb << "\'\n";
            vertexArr[index].go[symb] = (index == 0 ? 0 : go(getSuffix(index, vertexArr), symb, vertexArr));
        }
    }
    std::cout << "\t\t\t*Found way from " << index << " through \'" << symb << "\' is " << vertexArr[index].go[symb] << "\n";
    return vertexArr[index].go[symb];
}


void search(const std::string& text, std::vector<Vertex>& vertexArr, std::vector<int>& res, const std::vector<int>& patternOffsetArr, int patternLen, const std::vector<std::string>& patternArr)
{
    std::cout << "Searching begin\n";
    int curr = 0;
    for (int i = 0; i < text.size(); i++) {                                   //���������� ��� ������� ������
        std::cout << "\tCurrent symbol is \'" << text[i] <<  "\' from text...\n";
        std::cout << "\tCurrent vertex is " << curr << "\n";
        curr = go(curr, text[i], vertexArr);                               //������������ ������� � �������� �� ���������� �������
        std::cout << "\tAchieved vertex " << curr << "\n";
        std::cout << "\tFinding possible entrance with end suffix-links:\n";
        for (int tmp = curr; tmp != 0; tmp = getSuffix(tmp, vertexArr)) {  //��� ������������� ����� ����� ����. ������
            std::cout << "\t\tCurrent suffix-link vertex: " << tmp <<"\n";
            if (vertexArr[tmp].isTerminal) {                                  //���� �����-�� �� ��� ��������,
                for (int j = 0; j < vertexArr[tmp].number.size(); j++) {      //����������� ��� �������� ������ ����� ��������� ���������
                    if (i + 1 - patternOffsetArr[vertexArr[tmp].number[j] - 1] - vertexArr[tmp].deep >= 0 &&
                        i + 1 - patternOffsetArr[vertexArr[tmp].number[j] - 1] - vertexArr[tmp].deep <= text.size() - patternLen){
                        res[i + 1 - patternOffsetArr[vertexArr[tmp].number[j] - 1] - vertexArr[tmp].deep]++;
                        std::cout << "\t\tThe vertex is terminal (end suffix-link). The entrance found, index = " <<
                            i + 1 - patternOffsetArr[vertexArr[tmp].number[j] - 1] - vertexArr[tmp].deep << " (pattern = \"" << patternArr[vertexArr[tmp].number[j] - 1] <<
                            "\"). Count of entrance is " << res[i + 1 - patternOffsetArr[vertexArr[tmp].number[j] - 1] - vertexArr[tmp].deep] <<
                            " from " << patternOffsetArr.size() << " possible\n\n";
                    }
                }
            } else
                std::cout << "\t\tIt's not terminal vertex, getting suffix-link from this vertex\n\n";
        }
        std::cout << "\t\tRoot is arrived, reading new symbol from the text\n";
        std::cout << "\t---------------------------------------------------------------------------------\n";
        std::cout << "\t---------------------------------------------------------------------------------\n";
    }
    std::cout << "-------------------------------------------------------------------------------------\n";
    std::cout << "-------------------------------------------------------------------------------------\n";
}


void printRes(const std::vector<int>& res, int patternCount, std::string& textRest, int patternLen, const std::string& text)
{
    std::cout << "Total indexes of entrance (beginning from 1):\n";
    std::vector<bool> cutStr(text.size());          //������� �������� � ������, ������� ����� ��������

    for (int i = 0; i < res.size(); i++) {
        if (res[i] == patternCount) {                 //���� ��� ������� �������� ������ ������� ��� ��������,
            std::cout << i + 1 << "\n";             //�� ��������� �������
            for (int j = 0; j < patternLen; j++)    //���������� ��� ������� ������, ���������� �������
                cutStr[i + j] = true;               //�������� ������� �������� � ������, ���������� ��������
        }
    }

    for (int i = 0; i < cutStr.size(); i++){
        if (!cutStr[i])
            textRest.push_back(text[i]);            //��������� ������ ���������� �������
    }
}

//������� ��������� ������-������� � ��������� �� ������ �����-��������� ��� ��� � ���������� �� ������� � �������������� ������
void split(std::string str, char joker, std::vector<std::string>& patternArr, std::vector<int>& patternOffsetArr){
    std::cout << "Begin splitting\n";
    std::string buf = "";
    for (int i=0; i<str.size(); i++){
        if (str[i] == joker){
            if (buf.size() > 0) {                   //���������� ������ ������ (���� ������� ���� ������)
                patternArr.push_back(buf);            //��������� �������
                std::cout << "\tWas found new pattern: " << buf << "\n";
                patternOffsetArr.push_back(i - buf.size());   //� ��� ������ ��������� � ������ � ���������
                std::cout << "\tIndex of entrance in total pattern: " << i - buf.size() << "\n";
                buf = "";
            }
        }
        else {
            buf.push_back(str[i]);                  //��������� ������-�������� ��� ��������
            if (i == str.size() - 1){               //���� ��������� ����� ��������
                patternArr.push_back(buf);            //��������� ��������� ���������� ������� ��� �������
                std::cout << "\tWas found new pattern: " << buf << "\n";
                patternOffsetArr.push_back(i - buf.size() + 1);
                std::cout << "\tIndex of entrance in total pattern: " << i - buf.size() + 1 << "\n";
            }
        }
    }
}

void readPattern(std::vector<Vertex>& vertexArr, char& joker, std::vector<int>& patternOffsetArr, int& patternLen, std::vector<std::string>& patternArr)
{
    Vertex root;                            //������������� �����
    root.prev = -1;
    root.suffix = -1;
    vertexArr.push_back(root);
    int count = 0;

    std::cout << "Enter pattern:\n";
    std::string patternStr;                                    //������-�������
    std::cin >> patternStr;
    std::cout << "Enter joker:\n";
    std::cin >> joker;
    patternLen = patternStr.size();                            //����� ��������
    std::cout << "-------------------------------------------------------------------------------------\n";

    split(patternStr, joker, patternArr, patternOffsetArr);
    std::cout << "-------------------------------------------------------------------------------------\n";
    std::cout << "-------------------------------------------------------------------------------------\n";
    std::cout << "Begin bohr building\n";
    for (auto pattern : patternArr) {
        addString(pattern, vertexArr, count);    //��������� ���
    }
    std::cout << "-------------------------------------------------------------------------------------\n";
    std::cout << "-------------------------------------------------------------------------------------\n";
}

//������� ������ ������������� ����� ��������� ��� �� ����� ������� ����
int findMaxSons(std::vector<Vertex> vertexArr)
{
    int max = vertexArr[0].next.size();

    for(int i = 1; i < vertexArr.size(); i++){
        if (vertexArr[i].next.size() > max)
            max = vertexArr[i].next.size();
    }
    return max;
}

void automatePrint(std::vector <Vertex> vertexArr)
{
    std::cout << "-------------------------------------------------------------------------------------\n";
    std::cout << "Total automate:\n";

    for (int i = 0; i < vertexArr.size(); i++){
        std::cout << "Connections from vertex " << i << ":\n";
        auto iter = vertexArr[i].go.begin();
        for (int j = 0; j < vertexArr[i].go.size(); j++){
            std::cout << "\t" << i << "  --" << iter->first << "->  " << iter->second << "\n";
            iter++;
        }
    }
}

int main() {
    std::cout << "-------------------------------------------------------------------------------------\n";
    std::cout << "Enter text:\n";
    std::string text, textRest;
    std::cin >> text;

    std::vector<Vertex> vertexArr;      //������ ������
    std::vector<std::string> patternArr;
    std::vector<int> res(110000);    //������ ����� ���������� ��������� ��� ������ �������� ������
    std::vector<int> patternOffsetArr;
    int patternLen;                     //����� ��������

    for (int i = 0; i < 110000; i++){
        res[i] = 0;
    }

    char joker;

    readPattern(vertexArr, joker, patternOffsetArr, patternLen, patternArr);
    search(text, vertexArr, res, patternOffsetArr, patternLen, patternArr);
    printRes(res, patternArr.size(), textRest, patternLen, text);

    std::cout << "Rest string from text after cutting patterns from it: " << textRest << "\n";

    int maxSonsCount = findMaxSons(vertexArr);
    std::cout << "Max count of sons: " << maxSonsCount << "\n\n";

    automatePrint(vertexArr);

    return 0;
}
