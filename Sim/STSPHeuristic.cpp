#include "STSPHeuristic.h"

Node STSPHeuristic::infer(const Graph& rumorGraph, const Graph& underlyingGraph, Size sampleSize)
{
    RCCalculator rcCalctor;
    ScoreMap score;
    const NodeList& nodeList = rumorGraph.GetNodeList();

    for(Index i=0; i<nodeList.size(); ++i)
        score[nodeList[i]] = 0;

    for(Index i=0; i<sampleSize; ++i)
    {
        Graph RST;
        Alg::RST(rumorGraph, RST);

        ScoreMap rc;
        rcCalctor.calcAll(RST, rc);
        
        //Score prob = Alg::ProbOfPerm(rumorGraph, RST.GetNodeList());
        for(Index j=0; j<nodeList.size(); ++j)
        {
            NodeList bfsPerm;

            score[nodeList[j]] +=  Alg::ProbOfPerm(rumorGraph, RST.GetNodeList(), rc[nodeList[j]]);
    
            if( score[nodeList[j]] == std::numeric_limits<Score>::infinity() )
            {
                std::cout << "STS heuristic data over flow!\n";
                std::exit(3);
            }
        }
    }

    Node source = Alg::MaxScoreNode( score );

    std::cout << "STSP" << std::endl;
    for(ScoreMap::iterator it=score.begin(); it!=score.end(); ++it)
    {
        std::cout << it->first << ' ' << it->second << std::endl;
    }

    return source;
}