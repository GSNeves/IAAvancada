#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <stdexcept> // Para std::invalid_argument
#include "heuristica.hpp"
#include "algoritmos.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Erro: Argumentos insuficientes." << endl;
        cerr << "Uso: ./main <algoritmo> <estado1>[,<estado2>,...]" << endl;
        return 1;
    }

    string algorithm = argv[1];
    vector<PuzzleState> initialStates;

    // --- Início da Lógica de Leitura Definitiva ---

    // 1. Ler TODOS os números de todos os estados em um único vetor plano.
    vector<int> all_numbers;
    for (int i = 2; i < argc; ++i) {
        string arg = argv[i];
        // Remove a vírgula do argumento, caso o shell a preserve (segurança extra).
        arg.erase(remove(arg.begin(), arg.end(), ','), arg.end());
        
        // Se o argumento estiver vazio após remover a vírgula, pule-o.
        if (arg.empty()) {
            continue;
        }

        try {
            all_numbers.push_back(stoi(arg));
        } catch (const std::invalid_argument& e) {
            cerr << "Erro: Argumento inválido não pôde ser convertido para número: " << arg << endl;
            return 1;
        }
    }

    // 2. Determinar o tamanho do bloco (9 para 8-puzzle, 16 para 15-puzzle).
    size_t chunk_size = 0;
    if (all_numbers.size() % 9 == 0 && all_numbers.size() > 0) {
        chunk_size = 9;
    } else if (all_numbers.size() % 16 == 0 && all_numbers.size() > 0) {
        chunk_size = 16;
    } else {
        cerr << "Erro: O número total de peças (" << all_numbers.size() 
             << ") não é um múltiplo de 9 ou 16." << endl;
        return 1;
    }

    // 3. Dividir o vetor plano em blocos, criando um PuzzleState para cada um.
    for (size_t i = 0; i < all_numbers.size(); i += chunk_size) {
        PuzzleState state;
        // Cria um sub-vetor para o estado atual
        state.board.assign(all_numbers.begin() + i, all_numbers.begin() + i + chunk_size);

        auto it = find(state.board.begin(), state.board.end(), 0);
        // A validação de '0' já é garantida se o input estiver correto, mas mantemos por segurança.
        state.emptyTilePos = distance(state.board.begin(), it);
        
        initialStates.push_back(state);
    }

    // --- Fim da Lógica de Leitura ---

    // Loop de execução (inalterado)
    for (auto& state : initialStates) {
        if (algorithm == "-astar") {
            AStar(state);
        } else if (algorithm == "-bfs") {
            BFSGraph(state);
        } else if (algorithm == "-idfs") {
            IterativeDeepening(state);
        } else if (algorithm == "-gbfs") {
            Greedy(state);
        } else if (algorithm == "-idastar") {
            idaStar(state);
        }
    }

    return 0;
}