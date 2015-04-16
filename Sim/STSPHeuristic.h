#ifndef STSPHEURISTIC_H
#define STSPHEURISTIC_H

#include "Algorithm.h"
#include "RCCalculator.h"

// Spanning-Tree-Sampling heuristic for general graph, with probability
class STSPHeuristic
{
public:
    Node infer(const Graph& rumorGraph, const Graph& underlyingGraph, Size sampleSize);
};

#endif