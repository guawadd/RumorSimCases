#include "LwHeuristic.h"

Node LwHeuristic::infer(const Graph& rumorGraph, const Graph& underlyingGraph, bool& skip)
{
    init(rumorGraph);
    ScoreMap score;

    for(Index i=0; i<rumorGraph.NodeSize(); ++i)
    {
       Node v = rumorGraph.NodeAt( i );
       NodeListVector D_v;
       NodeListMap    L_v;
       PrepDL(rumorGraph, v, D_v, L_v);

       // handle data over flow, need to fix
       for(NodeListVector::const_iterator it=D_v.begin(); it!=D_v.end(); ++it)
           if(it->size()>11)
           {
               skip = true;
               return DummyNode;
           }

       m_avgProb[ v ] = AvgProbUtil(v, underlyingGraph, D_v);

       Graph T(rumorGraph, v);
       EnumTreeUtil(v, L_v, T, 1);
       
       score[v] = m_avgProb[v]*m_sumRC[v]/m_treeCnt[v];
    }

    Node source = Alg::MaxScoreNode( score );
    
    return source;
    //Node v = rumorGraph.NodeAt( 0 );
    //NodeListVector D_v;
    //NodeListMap    L_v;
    //PrepDL(rumorGraph, v, D_v, L_v);

    //Graph T(rumorGraph, v);
    //EnumTreeUtil(v, L_v, T, 1);
    //std::cout << "nooed: " << v << std::endl;
    //std::cout << "avg prob: " << AvgProbUtil(v, underlyingGraph, D_v) << std::endl;
    /*for(int i=0; i!=D_v.size(); ++i)
    {
        std::cout << "D_" << i << "(v): ";
        for(int j=0; j<D_v[i].size(); ++j)
            std::cout << D_v[i][j] << " ";
        std::cout << std::endl;
    }
    for(NodeListMap::const_iterator it=L_v.begin(); it!=L_v.end(); ++it)
    {
        std::cout << "L_v(" << it->first << "): ";
        for(int i=0; i<it->second.size(); ++i)
            std::cout << it->second[i] << " ";
        std::cout << std::endl;
    }*/

    return DummyNode;
}

void LwHeuristic::init(const Graph& rumorGraph)
{
    for(Index i=0; i<rumorGraph.NodeSize(); ++i)
    {
        m_avgProb[ rumorGraph.NodeAt(i) ] = 0;
        m_sumRC  [ rumorGraph.NodeAt(i) ] = 0;
        m_treeCnt[ rumorGraph.NodeAt(i) ] = 0;
    }
}

void LwHeuristic::PrepDL(const Graph& rumorGraph, Node v, NodeListVector& D_v, NodeListMap& L_v)
{
    Size    visitCnt=0;
    BoolMap visited;
    NodeList crtLevel, nextLevel;
    
    visited[ v ] = true;
    crtLevel.push_back( v );
    D_v.push_back( crtLevel );
    ++visitCnt;

    while( visitCnt != rumorGraph.NodeSize() )
    {
        for(NodeList::const_iterator u=crtLevel.begin(); u!=crtLevel.end(); ++u)
        {
            for(NodeList::const_iterator w=rumorGraph.NeighborsOf(*u).begin();
                                         w!=rumorGraph.NeighborsOf(*u).end(); 
                                         ++w)
            {
                if( !visited[*w] )
                {
                    nextLevel.push_back(*w);
                    L_v[*w].push_back(*u);

                    ++visitCnt;
                    visited[*w] = true;
                }
                else if( std::find(nextLevel.begin(), nextLevel.end(), *w) != nextLevel.end() )
                {
                    L_v[*w].push_back(*u);
                }
                else
                    ;
            }
        }
        D_v.push_back( nextLevel );
        crtLevel = nextLevel;
        nextLevel.clear();
    }
}

Score LwHeuristic::AvgProbUtil(Node v, const Graph& G, NodeListVector& D_v)
{
    Score ret = 1;
    BoolMap infected;
    CounterMap multiplicity;
    Score cardinality = 0;

    // init
    for(NodeList::const_iterator it=G.NeighborsOf(v).begin();
                                 it!=G.NeighborsOf(v).end();
                                 ++it )
    {
        ++multiplicity[*it];
        ++cardinality;
    }
    infected[v] = true;

    // Calculate average probability per level, starting from level 1 (level 0 is v)
    for(NodeListVector::iterator Di=D_v.begin()+1; Di!=D_v.end(); ++Di)
    {
        Score sumProb = 0;
        Size permCnt = 0;
        
        // keep track within each level
        CounterMap extraMul;
        BoolMap tempInfected;
        Score tempCard;

        // For each possible permutation, calculate probability
        std::sort(Di->begin(), Di->end());
        do
        {
            Score p = 1;

            extraMul.clear();
            tempCard = cardinality;

            for(NodeList::const_iterator it=Di->begin(); it!=Di->end(); ++it)
            {
                p *= ( multiplicity[*it] + extraMul[*it] );
                p /= tempCard;

                tempCard -= ( multiplicity[*it] + extraMul[*it] );

                for(NodeList::const_iterator neib=G.NeighborsOf(*it).begin();
                                             neib!=G.NeighborsOf(*it).end();
                                             ++neib) 
                {
                    if(!infected[*neib] && !tempInfected[*neib]) 
                    {
                        ++extraMul[*neib];
                        ++tempCard;
                    }
                }

                tempInfected[*it] = true;
            }

            sumProb += p;
            ++permCnt;

        } while( std::next_permutation(Di->begin(), Di->end()) );

        // Update for next level
        for(CounterMap::const_iterator it=extraMul.begin(); it!=extraMul.end(); ++it)
        {
            multiplicity[it->first] += it->second;
        }
        for(BoolMap::const_iterator it=tempInfected.begin(); it!=tempInfected.end(); ++it)
        {
            infected[it->first] = it->second;
        }
        cardinality = tempCard;

        ret *= sumProb/permCnt;
    }

    if(ret <= 0) 
    {
        std::cout << "Average permutation probability overflowed!";
        exit(3);
    }

    return ret;
}

// T.NodeAt(0) is assumed to be v, so the recursion can continue to end without stop
void LwHeuristic::EnumTreeUtil(Node v, const NodeListMap& L_v, Graph& T, Index k)
{
    Node current = T.NodeAt(k);
    
    // Full trees will be generated
    if( k == (T.NodeSize()-1) )
    {
        for( NodeList::const_iterator w=L_v.at(current).begin();
                                        w!=L_v.at(current).end();
                                        ++w )
        {
            T.AddEdge( current, *w );
            
            m_sumRC[ v ] += m_rcCalc.calcOne(T, v);
            ++m_treeCnt[ v ];

            // Restore to previous state
            T.RemoveEdge( current, *w );
        }
    }
    else
    {
        for( NodeList::const_iterator w=L_v.at(current).begin();
                                        w!=L_v.at(current).end();
                                        ++w )
        {
            T.AddEdge( current, *w );
        
            EnumTreeUtil(v, L_v, T, k+1);

            // Restore to previous state
            T.RemoveEdge( current, *w );
        }
    }
}