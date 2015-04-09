#ifndef ECHEURISTIC_H
#define ECHEURISTIC_H

#include "Algorithm.h"

// Eccentricity heuristic
class EcHeuristic
{
public:
    Node infer(const Graph& rumorGraph, const Graph& underlyingGraph);
};

#endif