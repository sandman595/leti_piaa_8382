#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

class Vertex {//vertex class

public:

    char name;
    double pathToVertex;//path to the current vertex - g
    double heuristicF;//heuristic function - f
    char nameFromT;
    std::vector<char> coupled;//vector for vertexes originating from a vertex


    Vertex(char name)//constructor1-required to fill in the initial vertex
        : name(name) {
        heuristicF = 0;
        pathToVertex = -1;//will be used for unprocessed vertexes instead of the infinity sign
        nameFromT = '-';
    }

    Vertex() {			//constructor2
        name = '!';		//
        heuristicF = 0;
        pathToVertex = -1;	//
        nameFromT = '-';
    }

};

class Path {//path class. It stores only the path: from where to where and how much the path weighs

public:

    char nameFromP;
    char nameOutP;
    double weightPath;


    Path(char nameFromP, char nameOutP, double weightPath)
        : nameFromP(nameFromP), nameOutP(nameOutP), weightPath(weightPath) {}



    char getNameFromP() const {
        return nameFromP;
    }

    char getNameOutP() const {
        return nameOutP;
    }

    double getWeightPath() const {
        return weightPath;
    }
};

bool check(std::vector<Path>& vectorPath, std::vector<Vertex>& vectorVertices, char endVertex, bool flagM, bool flagAd)
{
    std::cout << "Starting the monotony and validness function check." << std::endl;
    if (abs(endVertex - endVertex) != 0) {
        std::cout << "The heuristic estimate of the target state is not zero!" << std::endl;
        flagM = false;

    }

    for (unsigned int i = 0; i < vectorPath.size(); i++) {

        if ((abs(endVertex - vectorPath[i].nameFromP) - abs(endVertex - vectorPath[i].nameOutP)) > vectorPath[i].weightPath) {
            std::cout << "The monotony isn't as good." << std::endl;
            flagM = false;

        }
    }
    //checking for validity
    if (!flagM)
    {
        for (unsigned int i = 0; i < vectorVertices.size(); i++) {


            if ((abs(endVertex - vectorVertices[i].name) > (vectorVertices[vectorVertices.size() - 1].pathToVertex - vectorVertices[i].pathToVertex)))
            {
                std::cout << "It isn't valid." << std::endl;
                flagAd = false;
            }


        }
    }
    if (flagM)
    {
        std::cout << "The heuristic is monotonous and valid!" << std::endl;
        return true;
     }
    else if (!flagM && flagAd)
    {
        std::cout << "The heuristic is valid!" << std::endl;
        return true;
    }
    else
    {
        std::cout << "The heuristic is neither monotonous nor valid!" << std::endl;
        return false;
    }

}





int whatNumber(char a, std::vector<Vertex>& vectorVertices) {

    for (unsigned int i = 0; i < vectorVertices.size(); i++) {
        if (vectorVertices[i].name == a) {
            return i;
        }
    }
    return -1;
}

bool comp(Vertex a, Vertex b) {//comparator, used for sorting in an open list
    return a.heuristicF < b.heuristicF;
}

void answer(std::vector<Vertex>& vectorVertices, char startVertex, char endVertex)
{
    std::cout << "The answer function starts." << std::endl;
    std::vector<Vertex> answer;
    answer.reserve(0);
    Vertex temp = vectorVertices[whatNumber(endVertex, vectorVertices)];
    std::cout << "Writing down the last vertex." << endVertex << std::endl;
    answer.emplace_back(temp);
    while (temp.name != startVertex) {
        temp = vectorVertices[whatNumber(temp.nameFromT, vectorVertices)];
        std::cout << "Writing down the vertex." << temp.name << std::endl;
        answer.emplace_back(temp);
    }
    std::cout << "Reversing result..." << std::endl;
    std::reverse(answer.begin(), answer.end());//since it was filled in the reverse order, we do reverse
    std::cout << "Answer:  " << std::endl;
    for (Vertex ans : answer) {
        std::cout << ans.name;
    }
    std::cout << std::endl;
}

void changeInfo(std::vector<Vertex>& vectorVertices, std::vector<Vertex>& openVertexes, char a, char name, double temp_G, char endVertex )
{
    std::cout << "Vertex data update:  " << std::endl;

    vectorVertices[whatNumber(a, vectorVertices)].nameFromT = name;

    vectorVertices[whatNumber(a, vectorVertices)].pathToVertex = temp_G;
    openVertexes[whatNumber(a, openVertexes)].nameFromT = name;
    openVertexes[whatNumber(a, openVertexes)].pathToVertex = temp_G;
    openVertexes[whatNumber(a, openVertexes)].heuristicF = temp_G + abs(endVertex - a);
    std::cout << "Vertex  " << a << "   is set. Its ancestor:   " << name << std::endl;
    std::cout << "Path to the  " << a << "   is   " << temp_G << std::endl;
    std::cout << "Heuristic estimation for a vertex  " << a << "   is   " << temp_G + abs(endVertex - a) << std::endl;
}

bool A(std::vector<Path>& vectorPath, std::vector<Vertex>& vectorVertices, char startVertex, char endVertex) {

    Vertex temp;
    double temp_G;
    std::vector<Vertex> closedVertexes;
    closedVertexes.reserve(0);
    std::vector<Vertex> openVertexes;
    openVertexes.reserve(0);

    std::cout << "Adding a vertex to the open vertexes vector:   " << vectorVertices[0].name << std::endl;

    openVertexes.emplace_back(vectorVertices[0]);

    while (!openVertexes.empty()) {
        Vertex min = openVertexes[0];
        std::cout << "Open vertices sort: " << std::endl;
        std::sort(openVertexes.begin(), openVertexes.end(), comp);
        temp = openVertexes[0]; //minimum f from openVertexes
        std::cout << "Current vertex:    "<< temp.name << std::endl;

        if (temp.name == endVertex) {
            std::cout << "The current vertex is equal to the one you are looking for, so we call the answer function." << std::endl;
            answer(vectorVertices, startVertex, endVertex);
            return true;
        }
        std::cout << "Moving " << openVertexes[0].name << " to the vector of closed vertexes." << std::endl;
        closedVertexes.emplace_back(temp); //adding the processed vertex
        openVertexes.erase(openVertexes.begin()); //deleting the processed vertex

        for (unsigned int i = 0; i < temp.coupled.size(); i++) { //for each neighbor
            if (whatNumber(temp.coupled[i], closedVertexes) != -1) { //if the neighbor is in closedVertexes (already processed)
                continue;
            }
            int j = 0;
            while (true) {
                if (vectorPath[j].nameFromP == temp.name && vectorPath[j].nameOutP == temp.coupled[i]) {
                    std::cout << "Evaluating the shortest path to the " << vectorPath[j].nameOutP << std::endl;
                    temp_G = vectorPath[j].weightPath + temp.pathToVertex;
                    std::cout << "It's about " << temp_G << std::endl;
                    break;
                }
                j++;
            }

            if (whatNumber(temp.coupled[i], openVertexes) == -1) { //if the neighbor is not in openVertexes
                std::cout << temp.coupled[i] << " is not in the open vtcs vector, putting it into..." << std::endl;
                openVertexes.emplace_back(vectorVertices[whatNumber(temp.coupled[i], vectorVertices)]); //adding a neighbor
                std::cout << "Calling the information update function."  << std::endl;
                changeInfo(vectorVertices, openVertexes, temp.coupled[i], temp.name, temp_G, endVertex);
            }
            else {
                if (temp_G < openVertexes[whatNumber(temp.coupled[i], openVertexes)].pathToVertex) {
                    std::cout << "A shorter path up to  "   << temp.coupled[i] <<   " was found (" << temp_G <<").  Update..."<< std::endl;
                    changeInfo(vectorVertices, openVertexes, temp.coupled[i], temp.name, temp_G, endVertex);
                }
            }


        }
    }
     return false;
 }


int main() {

    setlocale(LC_ALL, "Russian");
    bool flag = true;
    bool flagM = true;
    bool flagAd = true;
    std::vector<Path> vectorPath;//vector paths
    vectorPath.reserve(0);
    std::vector<Vertex> vectorVertices;//vector Vertices
    vectorVertices.reserve(0);

    char startVertex;
    char endVertex;
    std::cout << "Input: " << std::endl;

    std::cin >> startVertex;
    std::cin >> endVertex;

    char start, end;
    double weight;


    while (std::cin >> start >> end >> weight) {
        vectorPath.emplace_back(Path(start, end, weight));
    }

    std::set<char> set;

    set.insert(startVertex);//inserting the first vertex

    vectorVertices.emplace_back(Vertex(startVertex));//creating the initial vertex and putting it in the vector

    int number;

    for (Path path : vectorPath) {//going through the path vector
        char from = path.getNameFromP();//
        char out = path.getNameOutP();

        if (set.find(from) == set.end()) {//checks that there is no from in the set
            set.insert(from);
            vectorVertices.emplace_back(Vertex(from));
        }
        if (set.find(out) == set.end()) {
            set.insert(out);
            vectorVertices.emplace_back(Vertex(out));

        }
    }
    //the path vector is full, but the neighbor vector is not =>
    //performing a pass through the path vector again
    for (Path path : vectorPath) {//going through the path vector
        char from = path.getNameFromP();//
        char out = path.getNameOutP();

        if (set.find(from) != set.end()) {//checks that the set has from
            number = whatNumber(from, vectorVertices);
            vectorVertices[number].coupled.emplace_back(out);//adding a vertex neighbor
        }

    }

    vectorVertices[0].pathToVertex = 0;
    vectorVertices[0].heuristicF = abs(endVertex - startVertex);
    std::cout << "Starting A*!" << std::endl;

    if (!A(vectorPath, vectorVertices, startVertex, endVertex)) {
        flag = false;
        std::cout << "Unexpected error!" << std::endl;
    }


    if (flag)
    {
        check(vectorPath, vectorVertices, endVertex, flagM, flagAd);
    }

}
