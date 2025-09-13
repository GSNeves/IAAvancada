#include "heuristica.hpp"
#include <unordered_set>
#include <ctime>
#include <deque>

void AStar(PuzzleState& state) {
    cout << distanciaManhattan8Puzzle(state);
}

int nodosExpandidos = 0;
long heuristicaAcumulada = 0;
int heuristicaPrimeiro;

void BFSGraph(PuzzleState& state) {
    clock_t start = clock();

    if (isGoal(state))
        return;
    deque<Node> openSet;
    Node firstNode = createInitialNode(state);
    openSet.push_back(firstNode);

    unordered_set<Node, NodeHash> closedSet;
    closedSet.insert(firstNode);

    heuristicaPrimeiro = firstNode.valorH;

    while (!openSet.empty()) {
        nodosExpandidos++;
        Node node = openSet.front();
        node.valorH = 0;
        heuristicaAcumulada += node.valorH;
        openSet.pop_front();
        vector<Node> children = generateChildNodes(node);
        for (Node child : children) {
            child.valorH = 0;
            clock_t end = clock();
            double tempo = double(end - start) / CLOCKS_PER_SEC;
            if (isGoal(child.state)) {
                Result finalResult;
                finalResult.averageHeuristic = (float) heuristicaAcumulada / nodosExpandidos;
                finalResult.duration = tempo;
                finalResult.expandedNodes = nodosExpandidos;
                finalResult.initialStateHeuristic =  heuristicaPrimeiro;
                finalResult.solutionLength = child.valorG;
                cout << "Terminou BFS: " << nodosExpandidos << endl;
                printResult(finalResult);
                return;
            }
            if (closedSet.find(child) == closedSet.end()) {
                closedSet.insert(child);
                openSet.push_back(child);
            }
        }
    }

    cout << "Terminou BFS: " << nodosExpandidos << endl;
    return;
}

