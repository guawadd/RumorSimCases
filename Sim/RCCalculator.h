#ifndef RCCALCULATOR_H
#define RCCALCULATOR_H

#include "Graph.h"

#include <iostream>

// RCCalculator is able to
// 1. calculate rumor centrality of given node in given graph
// 2. calculate rumor centrality of all nodes in given graph
//
// The input graph is assumed to be a tree, otherwise causing infinite loop.
class RCCalculator
{
public:
    RCCalculator();

    // calculate rumor centrality for all nodes in rumorGraph, store the result in rumorCentrality
    void calcAll(const Graph& rumorGraph, ScoreMap& rumorCentrality);

    // calculate rumor centrality for node v in rumorGraph
    Score calcOne(const Graph& rumorGraph, Node v);

    // calculate rumor centrality for all nodes on regular cyclic graph
    void calcRegCyc(const Graph& regCyc, ScoreMap& rumorCentrality);

private:
    void init(const Graph& rumorGraph);

    Score t_up(const Graph& rumorGraph, Node parent, Node child);
    
    Score p_up(const Graph& rumorGraph, Node parent, Node child);
    
    void  r_down(const Graph& rumorGraph, Node parent, Node child, ScoreMap& rc);

    void  r_down_reg(const Graph& rumorGraph, Node parent, Node child, ScoreMap& rc);

    void  markCyc(const Graph& G, NodeListVector& cycEdgeList);

    Score cycRootSize(Node root, Node one) const;

private:
    ScoreMap    m_t;
    ScoreMap    m_p;
    CounterMap  m_cyc;
    unsigned    m_magicCnt;
};

#endif