#include <iostream>
#include <vector>
#include <string>
#include "heuristica.hpp"
#include <sstream>
#include "algoritmos.hpp"

using namespace std;



int main(int argc, char* argv[]) {
    string algorithm = argv[1];
    vector<PuzzleState> initialStates;
    //POSICAO VAI DO 0 ATE O 8
    int j = 0;
    PuzzleState state;
    state.board = "";
    for (int i = 2; i < argc; ++i){
        if (argv[i] == " ")
            continue;
        if (argv[i] == ","){
            j=0;
            initialStates.push_back(state);
            continue;
        }

        int number = stoi(argv[i]);
        state.board = state.board + argv[i];
        if (number == 0)
            state.emptyTilePos = j;
        if (i == argc - 1) {
            initialStates.push_back(state);
        }
        j++;
    }
    for (const auto& initial_state : initialStates) {
        //initial_state.print();
        if (algorithm == "-astar") {
            //IMPLEMENTA ASTAR
            AStar(state);
        }
        if (algorithm == "-bfs") {
            //IMPLEMENTA BFSGRAPH
            BFSGraph(state);
        }
    }

}