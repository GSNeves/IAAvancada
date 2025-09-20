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

    vector<int> current_board = unpack_board(state.board, grid_size * grid_size);

    for (size_t i = 0; i < current_board.size(); i++)
    {
        int piece_value = current_board[i];
        if (piece_value == 0)
            continue;

        int posicaoEsperada = piece_value;

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
        vector<int> board = unpack_board(state.board, state.gridSize * state.gridSize);
        for (size_t i = 0; i < board.size(); ++i)
        {
            if (board[i] != static_cast<int>(i))
            {
                return false;
            }
        }

        return true;
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

PuzzleState swap(PuzzleState state, int posFree, int posNew)
{

    vector<int> board_vec = unpack_board(state.board, state.gridSize * state.gridSize);
    swap(board_vec[posFree], board_vec[posNew]);

    PuzzleState newState;
    newState.board = pack_board(board_vec);
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