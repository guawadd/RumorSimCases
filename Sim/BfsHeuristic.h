#ifndef BFSHEURISTIC_H
#define BFSHEURISTIC_H

#include "Algorithm.h"
#include "RCCalculator.h"

// BfsHeuristic infer source node based on Shah's BFS heuristic for general graph
class BfsHeuristic
{
public:
    Node infer(const Graph& rumorGraph, const Graph& underlyingGraph);
};

#endif