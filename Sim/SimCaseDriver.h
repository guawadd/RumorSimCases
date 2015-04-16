#ifndef SIMCASEDRIVER_H
#define SIMCASEDRIVER_H

#include "RCEstimator.h"
#include "BfsHeuristic.h"
#include "CcHeuristic.h"
#include "EcHeuristic.h"
#include "LwHeuristic.h"
#include "RegCycHeu.h"
#include "STSHeuristic.h"
#include "STSPHeuristic.h"

#include <fstream>
#include <iostream>
#include <string>
#include <time.h>
#include <vector>

// This class is for various simulation cases
// General steps:
// 1. Prepare an underying graph, normally large enough to avoid boundary effect.
// 2. Set a rumor source in the underlying graph and desired rumor size (number of nodes to be infected).
// 3. Infer the rumor source using estimators/heuristics
// 4. Repeat 2-3 large enough times, to calculate correct and/or error (hops from the true source) detection frequency
class SimCaseDriver
{
public:
    // Test detection probabilty on regular trees using rumor centrality.
    // Run on several rumor graph size (preset in program), e.g. 10, 20, 30, 40, 50, ...
    // For each size, repeat experiment multiple times (preset in program), e.g. 5000
    static int RegularTreeSim(const char file[]);
    
    // Test detection probability on regular cyclic graph using rumor centrality
    // Run on several rumor graph size (preset in program), e.g. 10, 20, 30, 40, 50, ...
    // For each size, repeat experiment multiple times (preset in program), e.g. 5000
    static int RegularCyclicSim(const char file[]);

    // Test detection probability on general graph using different heuristic
    // Run experiment [numExpon] times on rumor graphs of size [rumorSize] 
    static int GeneralGraphSim(const char file[], int rumorSize, int numExp);
};

#endif
