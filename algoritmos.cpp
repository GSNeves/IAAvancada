#include "heuristica.hpp"
#include <unordered_set>
#include <unordered_map>
#include <ctime>
#include <deque>
#include <queue>
#include <limits>
#include <memory>
#include <functional>

int nodosExpandidos = 0;
int heuristicaPrimeiro;
unsigned int nodeIdCounter = 0;

void AStar(PuzzleState& state) {
    nodosExpandidos = 0;
    nodosTotais = 0;
    heuristicaAcumulada = 0;
    int ordemInserida = 0;
    clock_t start = clock();

    std::priority_queue<Node, std::vector<Node>, AStarCompareNode> openSet;
    unordered_map<vector<int>, int, VectorHasher> distances;

    Node firstNode = createInitialNode(state);
    firstNode.sequenceId = ordemInserida;
    openSet.push(firstNode);

    heuristicaPrimeiro = firstNode.valorH;

    while (!openSet.empty()) {
        Node node = openSet.top();
        openSet.pop();

        if (!distances.count(node.state.board) || node.valorG < distances[node.state.board]) {
            distances[node.state.board] = node.valorG;

            if (isGoal(node.state)) {
                Result finalResult;
                clock_t end = clock();
                double tempo = double(end - start) / CLOCKS_PER_SEC;
                finalResult.averageHeuristic = static_cast<double>(heuristicaAcumulada) / nodosTotais;
                finalResult.duration = tempo;
                finalResult.expandedNodes = nodosExpandidos;
                finalResult.initialStateHeuristic = heuristicaPrimeiro;
                finalResult.solutionLength = node.valorG;
                //cout << "Terminou A*" << endl;
                printResult(finalResult);
                return;
            }
            nodosExpandidos++;

            vector<Node> children = generateChildNodes(node);
            for (Node child : children) {
                ordemInserida++;
                child.sequenceId = ordemInserida;

                openSet.push(child);
            }
        }
    }

    //cout << "Terminou A* sem solucao" << endl;
    return;
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
        //       cout << "Terminou BFS" << endl;
                printResult(finalResult);
                return;
            }
            if (closedSet.find(child) == closedSet.end()) {
                closedSet.insert(child);
                openSet.push_back(child);
            }
        }
    }

   // cout << "Terminou BFS sem solucao" << endl;
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

unique_ptr<Node> depthLimitedSearchIDA(unique_ptr<Node> node, int g, int threshold, int& newThreshold) {
    int f = node->valorG + node->valorH;
    if (f > threshold) {
        newThreshold = std::min(newThreshold, f);
        return nullptr;
    }
    if (isGoal(node->state))
        return node;

    nodosExpandidos++;
    vector<Node> children = generateChildNodes(*node);
    for (Node child : children) {
        auto solution = depthLimitedSearchIDA(make_unique<Node>(child), g + 1, threshold, newThreshold);
        if (solution) {
            return solution;
        }
    }
    return nullptr;
}

void idaStar(PuzzleState& state) {
    // Implementação do IDA*
    nodosExpandidos = 0;
    heuristicaAcumulada = 0;
    Node firstNode = createInitialNode(state);
    heuristicaPrimeiro = firstNode.valorH;
    clock_t start = clock();

    int threshold = firstNode.valorH;
    while (true) {
        int newThreshold = std::numeric_limits<int>::max();
        auto solution = depthLimitedSearchIDA(make_unique<Node>(firstNode), 0, threshold, newThreshold);
        if (solution) {
            Result finalResult;
            clock_t end = clock();
            double tempo = double(end - start) / CLOCKS_PER_SEC;
            finalResult.averageHeuristic = (float) heuristicaAcumulada / nodosExpandidos;
            finalResult.duration = tempo;
            finalResult.expandedNodes = nodosExpandidos;
            finalResult.initialStateHeuristic =  heuristicaPrimeiro;
            finalResult.solutionLength = solution->valorG;
       //     cout << "Terminou IDA*" << endl;
            printResult(finalResult);
            return;
        }
        if (newThreshold == std::numeric_limits<int>::max()) {
      //      cout << "Terminou IDA* sem solucao" << endl;
            return;
        }
        threshold = newThreshold; // Incrementa o limite de profundidade
    }
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
      //      cout << "Terminou Iterative Deepening" << endl;
            printResult(finalResult);
            return;
        }
    }
}

void Greedy(PuzzleState& state) {
    nodeIdCounter = 0;
    nodosExpandidos = 0;
    heuristicaAcumulada = 0;
    Node firstNode = createInitialNode(state);
    firstNode.sequenceId = nodeIdCounter++;
    heuristicaPrimeiro = firstNode.valorH;
    clock_t start = clock();

    if (isGoal(state))
        return;

    std::priority_queue<Node, std::vector<Node>, GreedyCompareNode> openSet;
    openSet.push(firstNode);

    std::unordered_set<Node, NodeHash> closedSet;
    closedSet.insert(firstNode);

    while (!openSet.empty()) {
        nodosExpandidos++;

        Node node = openSet.top();
        openSet.pop();

        vector<Node> children = generateChildNodes(node);
        for (Node child : children) {
            if (isGoal(child.state)) {
                Result finalResult;
                clock_t end = clock();
                double tempo = double(end - start) / CLOCKS_PER_SEC;
                finalResult.averageHeuristic = (float) heuristicaAcumulada / nodosTotais;
                finalResult.duration = tempo;
                finalResult.expandedNodes = nodosExpandidos;
                finalResult.initialStateHeuristic =  heuristicaPrimeiro;
                finalResult.solutionLength = child.valorG;
        //       cout << "Terminou Greedy" << endl;
                printResult(finalResult);
                return;
            }
            if (closedSet.find(child) == closedSet.end()) {
                child.sequenceId = nodeIdCounter++;
                closedSet.insert(child);
                openSet.push(child);
            }
        }
    }


}