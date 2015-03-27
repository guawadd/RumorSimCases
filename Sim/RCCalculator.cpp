#include "RCCalculator.h"

RCCalculator::RCCalculator()
: m_t         ( ),
  m_p         ( ),
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
                std::cout << "Rumor centrality data over flow!";
                std::exit(3);
            }
            r_down(rumorGraph, child, rumorGraph.NeighborsOf(child)[i], rc);
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