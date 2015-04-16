#include "EcHeuristic.h"

Node EcHeuristic::infer(const Graph& rumorGraph, const Graph& underlyingGraph)
{
    ScoreMap score;
    
    for(Index i=0;i<rumorGraph.NodeSize();++i)
    {
        Node current = rumorGraph.NodeAt(i);
		NodeListVector layers;
		NodeList layer0;
        layer0.push_back( current );
		layers.push_back( layer0 );

		BoolMap marked;
		marked[rumorGraph.NodeAt(i)] = true;

		for(Index j=0;j<layers.size() && marked.size()!=rumorGraph.NodeSize();++j)
        {
			NodeList nextlayer;
			for(Index k=0;k<layers[j].size();++k)
            {
                const NodeList& neib = rumorGraph.NeighborsOf(layers[j][k]);
                for(Index l=0;l<neib.size();++l)
                {
					if(!marked[neib[l]])
                    {
						nextlayer.push_back(neib[l]);
						marked[neib[l]]=true;
					}
				}
			}
			layers.push_back( nextlayer );
		}

        score[current] = 1.0/(layers.size()-1);
	}
    
    Node source = Alg::MaxScoreNode(score);

    std::cout << "EC" << std::endl;
    for(ScoreMap::iterator it=score.begin(); it!=score.end(); ++it)
    {
        std::cout << it->first << ' ' << it->second << std::endl;
    }

    return source;
}