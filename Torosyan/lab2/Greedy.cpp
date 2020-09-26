#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>      // std::setw

using namespace std;

class Edge {
private:
    char from;
    char to;
    double weight;

public:
    Edge(char from, char to, double weight)
        : from(from), to(to), weight(weight) {}

    char getFrom() const {
        return from;
    }

    char getTo() const {
        return to;
    }

    double getweight() const {
        return weight;
    }
};

bool comp(Edge a, Edge b) {
    return a.getweight() < b.getweight();
}

bool func(std::vector<Edge>* vector, char curChar, char endChar, std::vector<char>* answer, int depth) {
    depth++;

    std::cout << setw(depth + 1) << ' ' << "Current vertex:    " << curChar << std::endl;

    if (curChar == endChar) {    //exit from recursion
        std::cout << setw(depth + 1) << ' ' << "Reached the final vertex - " << endChar << "." << std::endl;
        return true;
    }



    std::cout << setw(depth + 1) << ' ' << "Lookin for edges coming from this vertex." << std::endl;
    std::vector<Edge> temporaryVector;
    temporaryVector.reserve(0);
    for (Edge Edge : *vector) {// all vertexes in the vector will be passed
        if (Edge.getFrom() == curChar) {//selects all Edges from the desired vertex
            std::cout << setw(depth + 1) << ' ' << "Since the vertex   " << Edge.getTo() << "   comes from the current vertex, writing this edge to the vector." << std::endl;
            temporaryVector.emplace_back(Edge);//written to a vector
        }
    }

    //since we need the cheapest way we will cohabit

    std::cout << setw(depth + 1) << ' ' << "Sorting vertices to find minimum weight." << std::endl;

    std::sort(temporaryVector.begin(), temporaryVector.end(), comp);


    for (Edge Edge : temporaryVector) {//going through all the vertexes
        if (func(vector, Edge.getTo(), endChar, answer, depth)) {//new variable
            depth--;
            std::cout << setw(depth + 1) << ' ' << "Writing a vertex    " << Edge.getTo() <<    "    into result vector" << std::endl;
            answer->emplace_back(Edge.getTo());
            return true;
        }
    }

    return false;
}

int main() {
    setlocale(LC_ALL, "rus");
    int depth = 0;
    int flag = 1;

    std::vector<Edge> vector;
    vector.reserve(0);

    std::vector<char> answer;
    answer.reserve(0);

    char startChar;
    char endChar;
    std::cout << "Input: ";

    std::cin >> startChar;
    std::cin >> endChar;


    char start, end;
    double weight;


    while (std::cin >> start >> end >> weight) {
        vector.emplace_back(Edge(start, end, weight));
    }

    std::cout << "Starting Mr. Greedy..." << std::endl;

    if (!func(&vector, startChar, endChar, &answer, depth))
    {
        std::cout << "Unexpected error!" << std::endl;
        flag = 0;
    }

    if (flag)
    {
        std::cout << "The Greedy is going down." << std::endl;
        std::cout << "The initial vertex is added to the result." << std::endl;
        answer.emplace_back(startChar);
        std::cout << "Reversing the result vector." << std::endl;
        std::reverse(answer.begin(), answer.end());

        std::cout << "Answer:    " << std::endl;
        for (char sym : answer) {
            std::cout << sym;
        }
    }


    return 0;
}
