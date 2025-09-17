#include "heuristica.hpp"
#include <cmath>

using namespace std;

int nodosTotais = 0;
long heuristicaAcumulada = 0;

int getGridSize(const PuzzleState& state) {
    return static_cast<int>(sqrt(state.board.size()));
}

int distanciaManhattan(PuzzleState state) {
    int valor = 0;
    int grid_size = getGridSize(state);
    if (grid_size == 0) return 0; // Evita divisão por zero

    for (size_t i = 0; i < state.board.size(); i++) {
        int piece_value = state.board[i];
        if (piece_value == 0) continue;

        int posicaoEsperada = piece_value;
        // Para o objetivo 0,1,2..., a posição esperada do '0' seria 0.
        // Se o seu objetivo for 1,2,3...0, ajuste a 'posicaoEsperada'.
        // Assumindo objetivo 0,1,2,3...
        if (posicaoEsperada == 0) posicaoEsperada = state.board.size(); // '0' no final

        int linhaAtual = i / grid_size;
        int colunaAtual = i % grid_size;

        int linhaEsperada = posicaoEsperada / grid_size;
        int colunaEsperada = posicaoEsperada % grid_size;

        valor += abs(linhaAtual - linhaEsperada) + abs(colunaAtual - colunaEsperada);
    }
    
    heuristicaAcumulada += valor;
    nodosTotais++;
    return valor;
}

// Checagem de Objetivo Generalizada
bool isGoal(PuzzleState state) {
    for (size_t i = 0; i < state.board.size(); ++i) {
        if (state.board[i] != static_cast<int>(i)) {
            return false;
        }
    }
    return true;
}

Node createInitialNode(PuzzleState state)
{
    Node node;
    node.state = state;
    node.valorH = distanciaManhattan(state);
    node.valorG = 0;
    node.valorF = node.valorH;
    node.action = NONE;

    return node;
}

PuzzleState swap(PuzzleState state, int posFree, int posNew)
{
    PuzzleState newState;
    newState.board = state.board;
    newState.emptyTilePos = posNew;
    swap(newState.board[posFree], newState.board[posNew]);
    return newState;
}

// Geração de Filhos Generalizada
vector<Node> generateChildNodes(Node fatherNode) {
    vector<Node> children;
    PuzzleState fatherState = fatherNode.state;
    int grid_size = getGridSize(fatherState);
    int emptyPos = fatherState.emptyTilePos;

    // Ação para CIMA
    if (fatherNode.action != DOWN && emptyPos >= grid_size) {
        Node upNode;
        upNode.action = UP;
        upNode.state = swap(fatherState, emptyPos, emptyPos - grid_size);
        upNode.valorH = distanciaManhattan(upNode.state);
        upNode.valorG = fatherNode.valorG + 1;
        upNode.valorF = upNode.valorG + upNode.valorH;
        children.push_back(upNode);
    }

    // Ação para ESQUERDA
    if (fatherNode.action != RIGHT && (emptyPos % grid_size) != 0) {
        Node leftNode;
        leftNode.action = LEFT;
        leftNode.state = swap(fatherState, emptyPos, emptyPos - 1);
        leftNode.valorH = distanciaManhattan(leftNode.state);
        leftNode.valorG = fatherNode.valorG + 1;
        leftNode.valorF = leftNode.valorG + leftNode.valorH;
        children.push_back(leftNode);
    }

    // Ação para DIREITA
    if (fatherNode.action != LEFT && (emptyPos % grid_size) != (grid_size - 1)) {
        Node rightNode;
        rightNode.action = RIGHT;
        rightNode.state = swap(fatherState, emptyPos, emptyPos + 1);
        rightNode.valorH = distanciaManhattan(rightNode.state);
        rightNode.valorG = fatherNode.valorG + 1;
        rightNode.valorF = rightNode.valorG + rightNode.valorH;
        children.push_back(rightNode);
    }

    // Ação para BAIXO
    if (fatherNode.action != UP && emptyPos < static_cast<int>(fatherState.board.size() - grid_size)) {
        Node downNode;
        downNode.action = DOWN;
        downNode.state = swap(fatherState, emptyPos, emptyPos + grid_size);
        downNode.valorH = distanciaManhattan(downNode.state);
        downNode.valorG = fatherNode.valorG + 1;
        downNode.valorF = downNode.valorG + downNode.valorH;
        children.push_back(downNode);
    }

    return children;
}

void printResult(Result result)
{
    cout << result.expandedNodes << ",";
    cout << result.solutionLength << ",";
    cout << result.duration << ",";
    cout << result.averageHeuristic << ",";
    cout << result.initialStateHeuristic << endl;
}