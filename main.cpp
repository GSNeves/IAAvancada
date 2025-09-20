#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <stdexcept> // Para std::invalid_argument
#include <sstream>
#include "heuristica.hpp"
#include "algoritmos.hpp"

using namespace std;


PuzzleState create_puzzle_state(vector<int>& board_data) {
    PuzzleState state;
    state.board = pack_board(board_data);

    if (board_data.size() == 16) {
        state.gridSize = 4;
    } else if (board_data.size() == 9) {
        state.gridSize = 3;
    } else {
        throw invalid_argument("Tamanho do tabuleiro inválido. Deve ser 9 ou 16.");
    }

    auto it = find(board_data.begin(), board_data.end(), 0);
    if (it != board_data.end()) {
        state.emptyTilePos = distance(board_data.begin(), it);
    } else {
        // Tratar o caso em que o '0' não é encontrado, se necessário
        // Por simplicidade, assumimos que a entrada é sempre válida.
        state.emptyTilePos = -1; 
    }
    return state;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Erro: Algoritmo não especificado." << endl;
        cerr << "Uso 1: ./main <algoritmo> <estado1>[,<estado2>,...]" << endl;
        cerr << "Uso 2: ./main <algoritmo> < arquivo_de_instancias.txt" << endl;
        return 1;
    }

    string algorithm = argv[1];
    vector<PuzzleState> initialStates;

    // --- Início da Lógica de Leitura Definitiva ---

// CASO 1: Os estados são passados como argumentos na linha de comando.
    if (argc > 2) {
        vector<int> all_numbers;
        for (int i = 2; i < argc; ++i) {
            string arg = argv[i];
            // Lógica para remover vírgulas, se um estado for passado como "1,2,3"
            stringstream ss(arg);
            string segment;
            while(getline(ss, segment, ',')) {
                if (!segment.empty()) {
                    try {
                        all_numbers.push_back(stoi(segment));
                    } catch (const std::invalid_argument& e) {
                        cerr << "Erro: Argumento inválido '" << segment << "' não pôde ser convertido para número." << endl;
                        return 1;
                    }
                }
            }
        }

        size_t chunk_size = 0;
        if (all_numbers.size() > 0 && all_numbers.size() % 9 == 0) {
            chunk_size = 9;
        } else if (all_numbers.size() > 0 && all_numbers.size() % 16 == 0) {
            chunk_size = 16;
        } else {
            cerr << "Erro: O número total de peças (" << all_numbers.size() 
                 << ") não é um múltiplo válido de 9 ou 16." << endl;
            return 1;
        }

        for (size_t i = 0; i < all_numbers.size(); i += chunk_size) {
            vector<int> board_data(all_numbers.begin() + i, all_numbers.begin() + i + chunk_size);
            initialStates.push_back(create_puzzle_state(board_data));
        }

    // CASO 2: Nenhum estado foi passado como argumento, então lemos do standard input.
    } else {
        string line;
        while (getline(cin, line)) {
            if (line.empty()) continue;

            vector<int> board_data;
            stringstream ss(line);
            int number;
            while (ss >> number) {
                board_data.push_back(number);
            }
            
            // Valida o tamanho do puzzle lido da linha
            if (board_data.size() == 9 || board_data.size() == 16) {
                 initialStates.push_back(create_puzzle_state(board_data));
            } else if (!board_data.empty()) {
                 cerr << "Aviso: Linha ignorada com número de peças inválido (" << board_data.size() << ")." << endl;
            }
        }
    }

    // --- Fim da Lógica de Leitura ---

    //cout << "Número de estados iniciais lidos: " << initialStates.size() << endl;

    // Loop de execução (inalterado)
    for (auto& state : initialStates) {
        if (algorithm == "-astar") {
            //cout << "Executando A*..." << endl;
            AStar(state);
            //cout << "----------------------------------------" << endl;
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

    //cout << "Execução concluída." << endl;

    return 0;
}