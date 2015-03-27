#ifndef LWHEURISTIC_H
#define LWHEURISTIC_H

#include "Algorithm.h"
#include "RCCalculator.h"

#include <algorithm>

// LwHeuristic infer source node based on Gavin's sum of level-wised trees heuristic for general graph
class LwHeuristic
{
public:
    Node infer(const Graph& rumorGraph, const Graph& underlyingGraph, bool& skip);

private:
    void init(const Graph& rumorGraph);
    void PrepDL(const Graph& rumorGraph, Node v, NodeListVector& D_v, NodeListMap& L_v);
    void EnumTreeUtil(Node v, const NodeListMap& L_v, Graph& T, Index k);
    Score AvgProbUtil(Node v, const Graph& G, NodeListVector& D_v);

private:
    RCCalculator m_rcCalc;
    ScoreMap     m_avgProb;
    ScoreMap     m_sumRC;
    CounterMap   m_treeCnt;
};

#endif