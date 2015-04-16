#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "Algorithm.h"
#include "BfsHeuristic.h"
#include "LwHeuristic.h"
#include "RCCalculator.h"

#include <fstream>
#include <string>

// This class is for debug purpose
class Debugger {
public:
    void testRCCalculator(const Graph& G);
    
    void testBFSTree(const Graph& G);

    void testRST(const Graph& G);

    void testSpreadRumor(const Graph& G);

    void testProbOfPerm(const Graph& G);

    void testEstimator(const Graph& G, int trial, int rumorSize);
};

#endif