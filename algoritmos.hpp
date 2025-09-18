#ifndef ALGORITMOS_HPP
#define ALGORITMOS_HPP

#include <memory>
#include "heuristica.hpp"

void AStar(PuzzleState& state);
void BFSGraph(PuzzleState& state);
void IterativeDeepening(PuzzleState& state);
void Greedy(PuzzleState& state);
void idaStar(PuzzleState& state);

struct SearchResult {
    int nextLimit;       // novo limite f
    unique_ptr<Node> solution; // nullptr se não achou
};

#endif