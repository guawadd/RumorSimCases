#include "RCCalculator.h"
#include "Algorithm.h"

RCCalculator::RCCalculator()
: m_t         ( ),
  m_p         ( ),
  m_cyc       ( ),
  m_magicCnt  ( 0 )
{
}

void RCCalculator::calcAll(const Graph& rumorGraph, ScoreMap& rumorCentrality)
{
    // randomly select a root node
    Node root = rumorGraph.NodeAt(rand()%rumorGraph.NodeSize());

    // fisrt calculate rumor centrality for root node
    rumorCentrality[root] = calcOne(rumorGraph, root);

    // then calculate rumor centrailty for all other nodes
    // message down phase
    r_down(rumorGraph, DummyNode, root, rumorCentrality);
}

Score RCCalculator::calcOne(const Graph& rumorGraph, Node v)
{
    // init
    init(rumorGraph);

    // message up phase
    m_t[v] = t_up(rumorGraph, DummyNode, v);
    m_p[v] = p_up(rumorGraph, DummyNode, v);

    Score c = 1.0/m_p[v];
    
    // Check data over limits
    if( c == std::numeric_limits<Score>::infinity() )
    {
        std::cout << "Rumor centrality data over flow!";
        std::exit(3);
    }
    return c;
}

void RCCalculator::calcRegCyc(const Graph& regCyc, ScoreMap& rumorCentrality)
{   
    // store cycles of length 3
    NodeListVector cycles;

    // mark all cycles in regCyc
    markCyc(regCyc, cycles);
    
    Graph tree(regCyc);
    Node root = tree.NodeAt(0);

    // cut the cycles by the first two nodes, set the third as root, assume only 1 cycle
    if( !cycles.empty() )
    {
        tree.RemoveEdge(cycles[0][0], cycles[0][1]);
        root = cycles[0][2];
    }

    init(tree);

    m_t[root] = t_up(tree, DummyNode, root);
    m_p[root] = p_up(tree, DummyNode, root);

    rumorCentrality[root] = 1.0/m_p[root];

    r_down_reg(tree, DummyNode, root, rumorCentrality);
}


Score RCCalculator::t_up(const Graph& rumorGraph, Node parent, Node child)
{
    Size child_degree = rumorGraph.NeighborsOf(child).size();
    for(Index i=0;i<child_degree;++i) {
        if(parent != rumorGraph.NeighborsOf(child)[i])
            m_t[child] += t_up(rumorGraph, child, rumorGraph.NeighborsOf(child)[i]);
    }

    return m_t[child];
}

Score RCCalculator::p_up(const Graph& rumorGraph, Node parent, Node child)
{
    Size child_degree = rumorGraph.NeighborsOf(child).size();
    for(Index i=0;i<child_degree;++i) {
        if(parent != rumorGraph.NeighborsOf(child)[i]) {
            m_p[child] *= p_up(rumorGraph, child, rumorGraph.NeighborsOf(child)[i]);
        }
    }
    m_magicCnt += 1;
    m_p[child] *= m_t[child]/m_magicCnt;

    return m_p[child];
}

void RCCalculator::r_down(const Graph& rumorGraph, Node parent, Node child, ScoreMap& rc)
{
    Size child_degree = rumorGraph.NeighborsOf(child).size();
    for(Index i=0;i<child_degree;++i) {
        if(parent != rumorGraph.NeighborsOf(child)[i]) {
            Node grandchild = rumorGraph.NeighborsOf(child)[i];
            rc[grandchild] = rc[child]*m_t[grandchild]/(rumorGraph.NodeSize()-m_t[grandchild]);
            
            if( rc[grandchild] == std::numeric_limits<Score>::infinity() )
            {
                std::cout << "Rumor centrality data over flow!\n";
                std::exit(3);
            }
            
            r_down(rumorGraph, child, rumorGraph.NeighborsOf(child)[i], rc);
        }
    }
}

void RCCalculator::r_down_reg(const Graph& rumorGraph, Node parent, Node child, ScoreMap& rc)
{
    Size child_degree = rumorGraph.NeighborsOf(child).size();
    for(Index i=0;i<child_degree;++i) {
        if(parent != rumorGraph.NeighborsOf(child)[i]) {
            Node grandchild = rumorGraph.NeighborsOf(child)[i];
        
            if( m_cyc[grandchild] == 0 )
                rc[grandchild] = rc[child]*m_t[grandchild]/(rumorGraph.NodeSize()-m_t[grandchild]);
            else
                rc[grandchild] = rc[child]*m_t[grandchild]/cycRootSize(child, grandchild);
           
            if( rc[grandchild] == std::numeric_limits<Score>::infinity() )
            {
                std::cout << "Rumor centrality data over flow!\n";
                std::exit(3);
            }

            r_down_reg(rumorGraph, child, rumorGraph.NeighborsOf(child)[i], rc);
        }
    }
}

void RCCalculator::init(const Graph& rumorGraph)
{
    m_t.clear();
    m_p.clear();
    for(Index i=0;i<rumorGraph.NodeSize();++i) 
    {
        m_t[rumorGraph.NodeAt(i)] = 1;
        m_p[rumorGraph.NodeAt(i)] = 1;
    }

    m_magicCnt = 0;
}

void RCCalculator::markCyc(const Graph& G, NodeListVector& cycEdgeList)
{
    for(unsigned i=0; i<G.NodeSize(); ++i)
        m_cyc[G.NodeAt(i)] = 0;

    std::map<Node, Node> parent;
	NodeList currentlayer;
    currentlayer.push_back(G.NodeAt(0));
	parent[G.NodeAt(0)] = DummyNode;

    Size cycIdx=1;
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
				if( parent[*itn]==0 ) 
                {
					parent[*itn] = *itc;
					nextlayer.push_back(*itn);
				}
                // neighbor at same layer, cycle found
                if( parent[*itn]==parent[*itc] )
                {
                    if( m_cyc[*itn]==0 && m_cyc[*itc]==0 )
                    {
                        m_cyc[*itn] = cycIdx;
                        m_cyc[*itc] = cycIdx;
                        m_cyc[parent[*itc]] = cycIdx;

                        ++cycIdx;

                        NodeList cyc;
                        cyc.push_back(parent[*itn]);
                        cyc.push_back(*itn);
                        cyc.push_back(*itc);

                        cycEdgeList.push_back(cyc);
                    }
                }
			}
		}

		currentlayer = nextlayer;
	}
}

Score RCCalculator::cycRootSize(Node root, Node one) const
{
    // assume only one cycle currently
    for(CounterMap::const_iterator it=m_cyc.begin(); it!=m_cyc.end(); ++it)
    {
        // find the other node in the cycle and return root part size
        if(it->second==1 && it->first!=root && it->first!=one)
            return m_t.at(root)-m_t.at(one)-m_t.at(it->first);
    }

    std::cout << "wtf??????????\n";
    return 0;
}