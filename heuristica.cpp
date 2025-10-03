#include "heuristica.hpp"
#include <cmath>

using namespace std;

int nodosTotais = 0;
long heuristicaAcumulada = 0;



int getGridSize(const PuzzleState &state)
{
    return state.gridSize;
}

int distanciaManhattan(PuzzleState state)
{
    int valor = 0;
    int grid_size = state.gridSize;
    if (grid_size == 0)
        return 0; // Evita divisão por zero

    int total_tiles = grid_size * grid_size;

    for (int i = 0; i < total_tiles; i++) {
        int piece_value = (state.board >> (i * 4)) & 0xF;  // extrai o nibble na posição i
        if (piece_value == 0)
            continue;

        int linhaAtual = i / grid_size;
        int colunaAtual = i % grid_size;

        int linhaEsperada = piece_value / grid_size;
        int colunaEsperada = piece_value % grid_size;

        valor += std::abs(linhaAtual - linhaEsperada) + std::abs(colunaAtual - colunaEsperada);
    }

    heuristicaAcumulada += valor;
    nodosTotais++;
    return valor;
}

// Checagem de Objetivo Generalizada
bool isGoal(PuzzleState state)
{

    const uint64_t goal_15_puzzle = 0xFEDCBA9876543210;

    const uint64_t goal_8_puzzle = 0x876543210;

    if (state.gridSize == 4)
    {
        return state.board == goal_15_puzzle;
    }
    else
    {
        return state.board == goal_8_puzzle;
    }
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

uint64_t swap_positions(uint64_t board, int pos1, int pos2) {
    // Extrai nibbles
    uint64_t nib1 = (board >> (pos1 * 4)) & 0xFULL;
    uint64_t nib2 = (board >> (pos2 * 4)) & 0xFULL;

    // Limpa posições
    board &= ~(0xFULL << (pos1 * 4));
    board &= ~(0xFULL << (pos2 * 4));

    // Escreve de volta trocados
    board |= (nib1 << (pos2 * 4));
    board |= (nib2 << (pos1 * 4));

    return board;
}

PuzzleState swap(PuzzleState state, int posFree, int posNew)
{
    PuzzleState newState;
    newState.board = swap_positions(state.board, posFree, posNew);
    newState.emptyTilePos = posNew;
    newState.gridSize = state.gridSize;
    return newState;
}

// Geração de Filhos Generalizada
vector<Node> generateChildNodes(Node fatherNode)
{
    vector<Node> children;
    PuzzleState fatherState = fatherNode.state;
    int grid_size = getGridSize(fatherState);
    int emptyPos = fatherState.emptyTilePos;

    // Ação para CIMA
    if (fatherNode.action != DOWN && emptyPos >= grid_size)
    {
        Node upNode;
        upNode.action = UP;
        upNode.state = swap(fatherState, emptyPos, emptyPos - grid_size);
        upNode.valorH = distanciaManhattan(upNode.state);
        upNode.valorG = fatherNode.valorG + 1;
        upNode.valorF = upNode.valorG + upNode.valorH;
        children.push_back(upNode);
    }

    // Ação para ESQUERDA
    if (fatherNode.action != RIGHT && (emptyPos % grid_size) != 0)
    {
        Node leftNode;
        leftNode.action = LEFT;
        leftNode.state = swap(fatherState, emptyPos, emptyPos - 1);
        leftNode.valorH = distanciaManhattan(leftNode.state);
        leftNode.valorG = fatherNode.valorG + 1;
        leftNode.valorF = leftNode.valorG + leftNode.valorH;
        children.push_back(leftNode);
    }

    // Ação para DIREITA
    if (fatherNode.action != LEFT && (emptyPos % grid_size) != (grid_size - 1))
    {
        Node rightNode;
        rightNode.action = RIGHT;
        rightNode.state = swap(fatherState, emptyPos, emptyPos + 1);
        rightNode.valorH = distanciaManhattan(rightNode.state);
        rightNode.valorG = fatherNode.valorG + 1;
        rightNode.valorF = rightNode.valorG + rightNode.valorH;
        children.push_back(rightNode);
    }

    // Ação para BAIXO
    if (fatherNode.action != UP && emptyPos < static_cast<int>((grid_size * grid_size) - grid_size))
    {
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