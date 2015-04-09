#include "RegCycHeu.h"

Node RegCycHeu::infer(const Graph& rumorGraph)
{
    RCCalculator rcCalctor;
    ScoreMap score;
    
    rcCalctor.calcRegCyc(rumorGraph, score);

    Node source = Alg::MaxScoreNode(score);

    return source;
}