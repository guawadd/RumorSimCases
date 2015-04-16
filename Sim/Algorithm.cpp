#include "Algorithm.h"

namespace Alg
{
    void BFSTree(const Graph& G, Graph& T, Node root)
    {
        std::queue<Node> que;
        BoolMap visited;

        que.push(root);
        T.AddNode(root);
        visited[root] = true;

        while(!que.empty()) 
        {
            Node current = que.front();
            que.pop();
            
            NodeList neib = G.NeighborsOf(current);
            std::random_shuffle(neib.begin(), neib.end());

            for(NodeList::const_iterator it=neib.begin();
                                         it!=neib.end();
                                         ++it) 
            {
                Node next = *it;
                if(!visited[next])
                {
                    que.push(next);

                    T.AddNode(next);
                    T.AddEdge(current, next);

                    visited[next] = true;
                }
            }
        }
    }

    void BFSPerm(const Graph& G, NodeList& perm, Node root)
    {
        std::queue<Node> que;
        BoolMap visited;

        que.push(root);
        perm.push_back(root);
        visited[root] = true;

        while(!que.empty()) 
        {
            Node current = que.front();
            que.pop();
            
            NodeList neib = G.NeighborsOf(current);
            std::random_shuffle(neib.begin(), neib.end());

            for(NodeList::const_iterator it=neib.begin();
                                         it!=neib.end();
                                         ++it)
            {
                Node next = *it;
                if(!visited[next])
                {
                    que.push(next);

                    perm.push_back(next);

                    visited[next] = true;
                }
            }
        }
    }

    void RST(const Graph& G, Graph& RST)
    {
        // root node
        Node current = G.NodeAt(rand() % G.NodeSize());

        RST.AddNode(current);
        
        while( RST.NodeSize() != G.NodeSize() )
        {
            const NodeList& neib = G.NeighborsOf(current);
            Node next = neib[rand() % neib.size()];
            if( !RST.HasNode(next) )
            {
                RST.AddNode(next);
                RST.AddEdge(current, next);
            }

            current = next;
        }
    }

    long double ProbOfPerm(const Graph& g, const NodeList& perm, Score base) 
    {
        long double prob=base;
        BoolMap infected;
        int boundary_size=0;
        CounterMap count;
        for(unsigned i=0;i<perm.size();++i)
            count[perm[i]] = 0;
        for(NodeList::const_iterator it=g.NeighborsOf(perm[0]).begin();
                                     it!=g.NeighborsOf(perm[0]).end();
                                     ++it) 
        {
            count[*it]++;
            boundary_size++;
        }
        infected[perm[0]] = true;
        count[perm[0]] = 1;

        int perm_size = perm.size();
        for(int i=1;i<perm_size;++i) 
        {
            prob *= count[perm[i]];
            prob /= boundary_size;
            //std::cout << count[perm[i]] << "/" << boundary_size << std::endl;
            boundary_size -= count[perm[i]];
            count[perm[i]] = 0;

            for(NodeList::const_iterator it=g.NeighborsOf(perm[i]).begin();
                                         it!=g.NeighborsOf(perm[i]).end();
                                         ++it) 
            {
                if(!infected[*it]) 
                {
                    count[*it]++;
                    boundary_size++;
                }
            }

            infected[perm[i]] = true;
        }
        if(prob <= 0) 
        {
            std::cout << "Permutation probability overflowed!\n";
            exit(0);
        }
        return prob;
    }

    long double ProbOfPermS(const Graph& g, const NodeList& perm)
    {
        long double prob=1;
        BoolMap infected;
        int boundary_size=0;
        CounterMap count;
        for(unsigned i=0;i<perm.size();++i)
            count[perm[i]] = 0;
        for(NodeList::const_iterator it=g.NeighborsOf(perm[0]).begin();
                                     it!=g.NeighborsOf(perm[0]).end();
                                     ++it) 
        {
            count[*it]++;
            boundary_size++;
        }
        infected[perm[0]] = true;
        count[perm[0]] = 1;

        int perm_size = perm.size();
        for(int i=1;i<perm_size;++i) 
        {
            prob /= boundary_size;
            //std::cout << count[perm[i]] << "/" << boundary_size << std::endl;
            boundary_size -= count[perm[i]];
            count[perm[i]] = 0;

            for(NodeList::const_iterator it=g.NeighborsOf(perm[i]).begin();
                                         it!=g.NeighborsOf(perm[i]).end();
                                         ++it) 
            {
                if(!infected[*it]) 
                {
                    count[*it]++;
                    boundary_size++;
                }
            }

            infected[perm[i]] = true;
        }
        if(prob <= 0) 
        {
            std::cout << "Permutation probability overflowed!\n";
            exit(0);
        }
        return prob;
    }

    void SpreadRumor(const Graph& underlyingGraph, Graph& rumorGraph, Node source, Size rumorSize) 
     {
        if(rumorSize<=0 || rumorSize>underlyingGraph.NodeSize())
            throw std::runtime_error("Rumor size overflow!");

        BoolMap infected;
        NodeList susNode;

        // init rumor source
        infected[source] = true;
        rumorGraph.AddNode(source);

        for(NodeList::const_iterator it=underlyingGraph.NeighborsOf(source).begin(); 
                                     it!=underlyingGraph.NeighborsOf(source).end();
                                     ++it) 
        {
            susNode.push_back(*it);
        }

        for(Index i=1;i<rumorSize;++i) {
            if(susNode.empty())
            {
                std::cout << "Rumor graph not fully generated!\n";
                exit(0);
            }

            std::random_shuffle(susNode.begin(), susNode.end());
            Node victim = susNode[rand()%susNode.size()];

            for(NodeList::iterator it=susNode.begin();it!=susNode.end();) {
                if(*it == victim) {
                    it = susNode.erase(it);
                }
                else {
                    ++it;
                }
            }

            infected[victim] = true;
            rumorGraph.AddNode(victim);

            for(NodeList::const_iterator it=underlyingGraph.NeighborsOf(victim).begin();
                                         it!=underlyingGraph.NeighborsOf(victim).end();
                                         ++it) 
            {
                if(!infected[*it]) 
                {
                    susNode.push_back(*it);
                }
                else {
                    rumorGraph.AddEdge(victim, *it);
                }
            }
        }
    }

     void PrintGraph(const Graph& G, const std::string& name) 
     {
        std::ofstream output(name);
        Graph temp(G);
        output << temp.NodeSize() << std::endl;
        output << temp.EdgeSize() << std::endl;

        for(Index i=0;i<temp.NodeSize();++i) {
            NodeList v_adj(temp.NeighborsOf(temp.NodeAt(i)));
            for(Index j=0; j<v_adj.size(); ++j) 
            {
                output << temp.NodeAt(i) << " " << v_adj[j] << std::endl;
                temp.RemoveEdge(temp.NodeAt(i), v_adj[j]);
            }
        }
        
        output.close();
    }

     Node MaxScoreNode(const ScoreMap& score)
     {
         Score max=0;
         for(ScoreMap::const_iterator it=score.begin(); it!=score.end(); ++it)
             if( it->second > max )
                 max = it->second;

         NodeList maxNodes;
         for(ScoreMap::const_iterator it=score.begin(); it!=score.end(); ++it)
             if( it->second == max )
                 maxNodes.push_back(it->first);

         return maxNodes[rand()%maxNodes.size()];
     }

    Hop Distance(const Graph& G, Node v, Node u) 
    {
        // distance between v and itself is 0
        if( v == u )
            return 0;

	    Hop distance = 1;
	    BoolMap visited;
	    NodeList currentlayer;
	    currentlayer.push_back(v);
	    visited[v] = true;

        while( !currentlayer.empty() ) 
        {
		    NodeList nextlayer;
            for(NodeList::const_iterator itc=currentlayer.begin();
                                         itc!=currentlayer.end();
                                         ++itc) 
            {
                for(NodeList::const_iterator itn=G.NeighborsOf(*itc).begin();
                                             itn!=G.NeighborsOf(*itc).end();
                                             ++itn) 
                {
                    // find 
				    if(u == *itn)
					    return distance;
				    if(!visited[*itn]) 
                    {
					    visited[*itn] = true;
					    nextlayer.push_back(*itn);
				    }
			    }
		    }

		    distance++;
		    currentlayer = nextlayer;
	    }

        // No path between v,u found
        return InfDist;
    }
};