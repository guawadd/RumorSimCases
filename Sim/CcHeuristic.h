#ifndef CCHEURISTIC_H
#define CCHEURISTIC_H

#include "Algorithm.h"

// Closeness heuristic
class CcHeuristic
{
public:
    Node infer(const Graph& rumorGraph, const Graph& underlyingGraph);
};

#endif