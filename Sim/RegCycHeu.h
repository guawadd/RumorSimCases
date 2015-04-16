#ifndef REGCYCHEU_H
#define REGCYCHEU_H

#include "Algorithm.h"
#include "RCCalculator.h"

// Rumor centrality based heuristic for regular graph with separate cycles
class RegCycHeu
{
public:
    Node infer(const Graph& rumorGraph);
};

#endif