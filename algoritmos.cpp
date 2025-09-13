#include "heuristica.hpp"
#include <unordered_set>
#include <ctime>
#include <deque>
#include <memory>

int nodosExpandidos = 0;
long heuristicaAcumulada = 0;
int heuristicaPrimeiro;

void AStar(PuzzleState& state) {
    cout << distanciaManhattan8Puzzle(state);
}

void BFSGraph(PuzzleState& state) {
    nodosExpandidos = 0;
    heuristicaAcumulada = 0;
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
            if (isGoal(child.state)) {
                Result finalResult;
                clock_t end = clock();
                double tempo = double(end - start) / CLOCKS_PER_SEC;
                finalResult.averageHeuristic = (float) heuristicaAcumulada / nodosExpandidos;
                finalResult.duration = tempo;
                finalResult.expandedNodes = nodosExpandidos;
                finalResult.initialStateHeuristic =  heuristicaPrimeiro;
                finalResult.solutionLength = child.valorG;
                cout << "Terminou BFS" << endl;
                printResult(finalResult);
                return;
            }
            if (closedSet.find(child) == closedSet.end()) {
                closedSet.insert(child);
                openSet.push_back(child);
            }
        }
    }

    cout << "Terminou BFS sem solucao" << endl;
    return;
}

unique_ptr<Node> depthLimitedSearch(unique_ptr<Node> node, int depthLimit) {
    if (isGoal(node->state))
        return node;
    if (depthLimit> 0) {
        nodosExpandidos++;
        vector<Node> children = generateChildNodes(*node);
        for (Node child : children) {
            auto solution = depthLimitedSearch(make_unique<Node>(child), depthLimit-1);
            if (solution) {
                return solution;
            }
        }
    }
    return nullptr;
}


void IterativeDeepening(PuzzleState& state) {
    nodosExpandidos = 0;
    heuristicaAcumulada = 0;
    Node firstNode = createInitialNode(state);
    heuristicaPrimeiro = firstNode.valorH;
    clock_t start = clock();

    for (int i = 1; true; i++) {
        auto solution = depthLimitedSearch(make_unique<Node>(firstNode), i);
        if (solution) {
            Result finalResult;
            clock_t end = clock();
            double tempo = double(end - start) / CLOCKS_PER_SEC;
            finalResult.averageHeuristic = (float) heuristicaAcumulada / nodosExpandidos;
            finalResult.duration = tempo;
            finalResult.expandedNodes = nodosExpandidos;
            finalResult.initialStateHeuristic =  heuristicaPrimeiro;
            finalResult.solutionLength = solution->valorG;
            cout << "Terminou Iterative Deepening" << endl;
            printResult(finalResult);
            return;
        }
    }
}

