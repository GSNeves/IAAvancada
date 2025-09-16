#ifndef HEURISTICA_HPP
#define HEURISTICA_HPP
#include <vector>
#include <string>
#include <iostream>
using namespace std;

enum Action {
    UP, DOWN, LEFT, RIGHT, NONE
};

struct PuzzleState {
    string board;
    int emptyTilePos;
    void print() const {
        cout << "Board: " << board << endl << "EmptyTile: " << emptyTilePos << std::endl;
    }
};

struct Node {
    int valorH;
    int valorG;
    int valorF;
    unsigned int sequenceId;
    PuzzleState state;
    Action action;

    bool operator==(const Node &other) const {
        return state.board == other.state.board;
    }
};

struct NodeHash {
    std::size_t operator()(const Node& n) const {
        return std::hash<std::string>()(n.state.board); // usa hash da string
    }
};

struct Result {
    int expandedNodes;
    int solutionLength;
    double duration;
    float averageHeuristic;
    int initialStateHeuristic;
};

int distanciaManhattan8Puzzle(PuzzleState state);

bool isGoal(PuzzleState state);

Node createInitialNode(PuzzleState state);

PuzzleState swap(PuzzleState state, int posFree, int posNew);

vector<Node> generateChildNodes(Node fatherNode);

void printResult(Result result);


#endif