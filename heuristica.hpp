#ifndef HEURISTICA_HPP
#define HEURISTICA_HPP
#include <vector>
#include <string>
#include <iostream>
using namespace std;


extern int nodosTotais;
extern long heuristicaAcumulada;

enum Action {
    UP, DOWN, LEFT, RIGHT, NONE
};

struct PuzzleState {
    std::vector<int>board;
    int emptyTilePos;
    void print() const {
        cout << "Board: ";
        for (size_t i = 0; i < board.size(); ++i) {
            cout << board[i];
            if (i != board.size() - 1) cout << " ";
        }
        cout << endl << "EmptyTile: " << emptyTilePos << std::endl;
    }
};

struct Node {
    int valorH;
    int valorG;
    int valorF;
    unsigned int sequenceId;
    PuzzleState state;
    Action action;

    Node() : valorH(0), valorG(0), valorF(0), sequenceId(0), action(NONE) {}

    bool operator==(const Node &other) const {
        return state.board == other.state.board;
    }
};

struct VectorHasher {
    std::size_t operator()(const std::vector<int>& vec) const {
        std::size_t seed = vec.size();
        for (int i : vec) {
            seed ^= std::hash<int>{}(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

struct NodeHash {
    std::size_t operator()(const Node& n) const {
        // Custom hash for vector<int>
        std::size_t seed = n.state.board.size();
        for (auto& i : n.state.board) {
            seed ^= std::hash<int>{}(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

struct AStarCompareNode {
    bool operator()(const Node& a, const Node& b) const {
        if (a.valorF != b.valorF)
            return a.valorF > b.valorF;
        if (a.valorH != b.valorH)
            return a.valorH > b.valorH;
        return a.sequenceId < b.sequenceId;
    }
};

struct GreedyCompareNode {
    bool operator()(const Node& a, const Node& b) {
        if (a.valorH != b.valorH)
            return a.valorH > b.valorH;
        if (a.valorG != b.valorG)
            return a.valorG < b.valorG;
        return a.sequenceId < b.sequenceId;
    }
};

struct Result {
    int expandedNodes;
    int solutionLength;
    double duration;
    float averageHeuristic;
    int initialStateHeuristic;
};

int distanciaManhattan(PuzzleState state);

bool isGoal(PuzzleState state);

Node createInitialNode(PuzzleState state);

PuzzleState swap(PuzzleState state, int posFree, int posNew);

vector<Node> generateChildNodes(Node fatherNode);

void printResult(Result result);


#endif