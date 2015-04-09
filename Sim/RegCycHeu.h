#ifndef REGCYCHEU_H
#define REGCYCHEU_H

#include "Algorithm.h"
#include "RCCalculator.h"

// Closeness heuristic
class RegCycHeu
{
public:
    Node infer(const Graph& rumorGraph);
};

#endif