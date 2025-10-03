#include "algoritmos.hpp"
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
int idaStarHeuristaca;
int idaStarNodos;
unsigned int nodeIdCounter = 0;

void AStar(PuzzleState& state) {
    nodosExpandidos = 0;
    nodosTotais = 0;
    heuristicaAcumulada = 0;
    int ordemInserida = 0;

    const double TEMPO_LIMITE = 30.0; // Limite de tempo em segundos
    clock_t start = clock();

    std::priority_queue<Node, std::vector<Node>, AStarCompareNode> openSet;
    unordered_map<uint64_t, int> distances;

    Node firstNode = createInitialNode(state);
    firstNode.sequenceId = ordemInserida;
    openSet.push(firstNode);
    distances[firstNode.state.board] = 0; // Adiciona o estado inicial

    heuristicaPrimeiro = firstNode.valorH;

    while (!openSet.empty()) {

        //CHECAGEM DE TEMPO

        if (nodosExpandidos % 1000 == 0) { // Verifica o tempo a cada 1000 nós expandidos
            clock_t current = clock();
            double elapsed = double(current - start) / CLOCKS_PER_SEC;
            if (elapsed > TEMPO_LIMITE) {
                cout << "-,-,-,-,-" << endl;
                return; // Sai da função se o tempo limite for excedido
            }
        }

        Node node = openSet.top();
        openSet.pop();

        // Otimização: Se encontramos um caminho mais longo para um nó já finalizado, ignore-o.
        if (node.valorG > distances[node.state.board]) {
            continue;
        }

        if (isGoal(node.state)) {
            Result finalResult;
            clock_t end = clock();
            double tempo = double(end - start) / CLOCKS_PER_SEC;
            finalResult.averageHeuristic = static_cast<double>(heuristicaAcumulada) / nodosTotais;
            finalResult.duration = tempo;
            finalResult.expandedNodes = nodosExpandidos;
            finalResult.initialStateHeuristic = heuristicaPrimeiro;
            finalResult.solutionLength = node.valorG;
            printResult(finalResult);
            return;
        }
        nodosExpandidos++;

        vector<Node> children = generateChildNodes(node);
        for (Node& child : children) { // Usar referência & para evitar cópias
            
            // Verifica se o filho já foi visitado com um custo menor ou igual
            if (distances.count(child.state.board) && distances[child.state.board] <= child.valorG) {
                continue; // Se sim, ignora este caminho
            }
            
            // Caso contrário, é um bom nó para explorar
            ordemInserida++;
            child.sequenceId = ordemInserida;
            distances[child.state.board] = child.valorG; // Atualiza a distância
            openSet.push(child);
        }
    }

    //se não achar solucao, imprime mensagem
    cout << "-,-,-,-,-" << endl;

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
                finalResult.averageHeuristic = 0; // BFS não usa heurística
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

SearchResult idaSearch(unique_ptr<Node> n, int f_limit) {
    idaStarHeuristaca+=n->valorH;
    idaStarNodos++;
    if (n->valorF > f_limit) {
        return {n->valorF, nullptr};
    }
    if (isGoal(n->state)) {
        return {INT_MAX, std::move(n)};
    }

    int nextLimit = INT_MAX;
    nodosExpandidos++;
    for (Node child : generateChildNodes(*n)) {

        auto result = idaSearch(make_unique<Node>(child), f_limit);
        if (result.solution) {
            return {INT_MAX, std::move(result.solution)};
        }
        nextLimit = min(nextLimit, result.nextLimit);
    }
    return {nextLimit, nullptr};
}


void idaStar(PuzzleState& state) {
    nodosExpandidos = 0;
    nodosTotais = 0;
    heuristicaAcumulada = 0;
    clock_t start = clock();
    idaStarHeuristaca = 0;
    idaStarNodos = 0;

    Node firstNode = createInitialNode(state);
    heuristicaPrimeiro = firstNode.valorH;
    idaStarHeuristaca+=firstNode.valorH;
    idaStarNodos++;

    int f_limit = firstNode.valorH;

    while (f_limit != INT_MAX) {
        auto result = idaSearch(make_unique<Node>(firstNode), f_limit);
        if (result.solution) {
            clock_t end = clock();
            double tempo = double(end - start) / CLOCKS_PER_SEC;

            Result finalResult;
            finalResult.averageHeuristic = static_cast<double>(idaStarHeuristaca) / idaStarNodos;
            finalResult.duration = tempo;
            finalResult.expandedNodes = nodosExpandidos;
            finalResult.initialStateHeuristic = heuristicaPrimeiro;
            finalResult.solutionLength = result.solution->valorG;
            //cout << "Terminou IDA*" << endl;
            printResult(finalResult);
            return;
        }
        f_limit = result.nextLimit;
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
            finalResult.averageHeuristic = 0; // IDFS não usa heurística
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