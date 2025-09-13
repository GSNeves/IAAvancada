#include "heuristica.hpp"

using namespace std;

int distanciaManhattan8Puzzle(PuzzleState state) {
    int valor = 0;
    for (int i = 0; i < state.board.size(); i++) {
        char c = state.board[i];
        if (c == '0')
            continue;

        int posicaoEsperada = stoi(string(1, c)); //bloco 1 deve estar na posição 1, o bloco 2 na posição 2 e etc.
        int linhaAtual = i / 3;
        int colunaAtual = i % 3;

        int linhaEsperada = posicaoEsperada / 3;
        int colunaEsperada = posicaoEsperada % 3;

        valor += abs(linhaAtual - linhaEsperada) + abs(colunaAtual - colunaEsperada);

        //cout << "valor: " << valor << " linhaAtual: " << linhaAtual << " linhaEsperada: " << linhaEsperada << " colunaAtual: " << colunaAtual << " colunaEsperada: " << colunaEsperada << endl;
    }

    return valor;
}

bool isGoal(PuzzleState state) {
    if (state.board == "012345678")
        return true;
    return false;
}

PuzzleState swap(PuzzleState state, int posFree, int posNew) {
    PuzzleState newState;
    newState.board = state.board;
    newState.emptyTilePos = posNew;
    swap(newState.board[posFree], newState.board[posNew]);
    return newState;
}

vector<Node> generateChildNodes(Node fatherNode) {
    vector<Node> children;
    PuzzleState fatherState = fatherNode.state;

    //Gera nodo com ação pra cima
    if (fatherNode.action != DOWN && fatherNode.state.emptyTilePos > 2) {
        Node upNode;
        upNode.action = UP;
        upNode.state = swap(fatherNode.state, fatherState.emptyTilePos, fatherState.emptyTilePos-3);
        upNode.valorH = distanciaManhattan8Puzzle(upNode.state);
        upNode.valorG = fatherNode.valorG + 1;
        upNode.valorF = upNode.valorH + upNode.valorG;

        children.push_back(upNode);
    }

    //Gera nodo com ação pra esquerda
    if (fatherNode.action != RIGHT && (fatherNode.state.emptyTilePos % 3) != 0) {
        Node leftNode;
        leftNode.action = UP;
        leftNode.state = swap(fatherNode.state, fatherState.emptyTilePos, fatherState.emptyTilePos-1);
        leftNode.valorH = distanciaManhattan8Puzzle(leftNode.state);
        leftNode.valorG = fatherNode.valorG + 1;
        leftNode.valorF = leftNode.valorH + leftNode.valorG;

        children.push_back(leftNode);
    }

    //Gera nodo com ação pra direita
    if (fatherNode.action != LEFT && (fatherNode.state.emptyTilePos % 3) != 2) {
        Node rightNode;
        rightNode.action = UP;
        rightNode.state = swap(fatherNode.state, fatherState.emptyTilePos, fatherState.emptyTilePos+1);
        rightNode.valorH = distanciaManhattan8Puzzle(rightNode.state);
        rightNode.valorG = fatherNode.valorG + 1;
        rightNode.valorF = rightNode.valorH + rightNode.valorG;

        children.push_back(rightNode);
    }

    //Gera nodo com ação pra baixo
    if (fatherNode.action != UP && fatherNode.state.emptyTilePos < 6) {
        Node downNode;
        downNode.action = UP;
        downNode.state = swap(fatherNode.state, fatherState.emptyTilePos, fatherState.emptyTilePos+3);
        downNode.valorH = distanciaManhattan8Puzzle(downNode.state);
        downNode.valorG = fatherNode.valorG + 1;
        downNode.valorF = downNode.valorH + downNode.valorG;

        children.push_back(downNode);
    }

    return children;
}