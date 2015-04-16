#include "BfsHeuristic.h"

Node BfsHeuristic::infer(const Graph& rumorGraph, const Graph& underlyingGraph)
{
    RCCalculator calculator;
    ScoreMap score;
    
    // calculate (probability of one its BFS permutation)*(rumor centrality on one its BFS tree) for every node in rumor graph
    for(Index i=0; i<rumorGraph.NodeSize(); ++i)
    {
        // current node to calculate on
        Node current = rumorGraph.NodeAt(i);
        
        // generate a BFS tree rooted at current node
        Graph bfsTree;
        Alg::BFSTree(rumorGraph, bfsTree, current);
        
        // rumor centrality of current node in the BFS tree
        Score rc = calculator.calcOne(bfsTree, current);
        
        // probability of one BFS permutation for current node (reuse the discovery order in the BFS tree )
        //Score prob = Alg::ProbOfPerm(underlyingGraph, bfsTree.GetNodeList());
        
        // product of the two quantities
        //score[current] = rc*prob;
        score[current] = Alg::ProbOfPerm(underlyingGraph, bfsTree.GetNodeList(), rc);
    }
    
    // find the most likily source, which is with the largest score
    Node source = Alg::MaxScoreNode(score);

    std::cout << "BFS" << std::endl;
    for(ScoreMap::iterator it=score.begin(); it!=score.end(); ++it)
    {
        std::cout << it->first << ' ' << it->second << std::endl;
    }

    return source;
}