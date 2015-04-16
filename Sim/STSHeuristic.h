#ifndef STSHEURISTIC_H
#define STSHEURISTIC_H

#include "Algorithm.h"
#include "RCCalculator.h"

// Spanning-Tree-Sampling heuristic for general graph, without probability
class STSHeuristic
{
public:
    Node infer(const Graph& rumorGraph, const Graph& underlyingGraph, Size sampleSize);
};

#endif