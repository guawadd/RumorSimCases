#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "Graph.h"

#include <algorithm>
#include <fstream>
#include <queue>

// This namespace contains some common algorithms
namespace Alg
{
    // Find a BFS tree rooted at given node of given graph
    void BFSTree(const Graph& G, Graph& T, Node root);
    
    // Calculate probability of a permitted permutation (Gavin's method of calculation)
    long double ProbOfPerm(const Graph& g, const NodeList& perm);

    // Same as ProbOfPerm, except let all nominators be 1 (Shah's method of calculation)
    long double ProbOfPermS(const Graph& g, const NodeList& perm);

    // Spread a rumor starting from given source in given graph as per SI model, until given number of nodes are infected
    void SpreadRumor(const Graph& underlyingGraph, Graph& rumorGraph, Node source, Size rumorSize);

    // Calculate distance in terms of hops between two nodes in given graph
    Hop Distance(const Graph& G, Node v, Node u);

    // Find maximum value in a container, ties broken uniformly at random
    Node MaxScoreNode(const ScoreMap& score);

    // Output graph data with edge pairs format
    void PrintGraph(const Graph& G, const std::string& name);    
    
};
#endif