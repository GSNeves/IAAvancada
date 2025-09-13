#ifndef HEURISTICA_HPP
#define HEURISTICA_HPP
#include <vector>
#include <string>
#include <iostream>
using namespace std;

enum Action {
    UP, DOWN, LEFT, RIGHT
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
    PuzzleState state;
    Action action;
};

int distanciaManhattan8Puzzle(PuzzleState state);

bool isGoal(PuzzleState state);

PuzzleState swap(PuzzleState state, int posFree, int posNew);

vector<Node> generateChildNodes(Node fatherNode);


#endif