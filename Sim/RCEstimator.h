#ifndef RCESTIMATOR_H
#define RCESTIMATOR_H

#include "Algorithm.h"
#include "RCCalculator.h"

// RCEstimator infers source based on rumor centrality for tree
class RCEstimator
{
public:
    Node infer(const Graph& rumorGraph);
};

#endif