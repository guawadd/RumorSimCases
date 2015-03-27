#include "RCEstimator.h"

Node RCEstimator::infer(const Graph& rumorGraph)
{
    RCCalculator rcCalctor;
    ScoreMap score;

    rcCalctor.calcAll(rumorGraph, score);

    Node source = Alg::MaxScoreNode( score );

    return source;
}