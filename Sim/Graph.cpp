#include "Graph.h"

Graph::Graph()
: m_nodeSize      ( 0 ),
  m_edgeSize      ( 0 ),
  m_nodeList      (   ),
  m_adjacencyList (   )
{
}

Graph::Graph(const Graph& G, Node v)
: m_nodeSize      ( 0 ),
  m_edgeSize      ( 0 ),
  m_nodeList      (   ),
  m_adjacencyList (   )
{
    for(Index i=0; i<G.NodeSize(); ++i)
        AddNode( G.NodeAt(i) );

    for(NodeList::iterator it=m_nodeList.begin(); it!=m_nodeList.end(); ++it)
        if( *it == v )
        {
            *it = *m_nodeList.begin();
            *m_nodeList.begin() = v;
            break;
        }
}

Graph::Graph(const Graph& G)
{
    m_nodeSize = G.m_nodeSize;
    m_edgeSize = G.m_edgeSize;
    m_nodeList = G.m_nodeList;
    m_adjacencyList = G.m_adjacencyList;
}

Graph& Graph::operator = (const Graph& G)
{
    m_nodeSize = G.m_nodeSize;
    m_edgeSize = G.m_edgeSize;
    m_nodeList = G.m_nodeList;
    m_adjacencyList = G.m_adjacencyList;
}

Graph::~Graph()
{
}

Graph::Graph(const char file[])
: m_nodeSize      ( 0 ),
  m_edgeSize      ( 0 ),
  m_nodeList      (   ),
  m_adjacencyList (   )
{
    
    std::ifstream input( file );
    if( !input.good() )
    {
        std::cout << "Input file does not exist!";
        std::exit(2);
    }

    std::cout << "Load graph data...\n";

    // load graph to buffer
    Size numV, numE;
    NodeListVector edgeBuf;
    
    input >> numV >> numE;
    edgeBuf.resize(numE);

    for(unsigned i=0;i<numE;++i) {
        edgeBuf[i].resize(2);
        input >> edgeBuf[i][0] >> edgeBuf[i][1];
    }

    input.close();

    // construct actual graph structure from buffer
    BoolMap added;
    for(Index i=0;i<numE;++i) {
        if(edgeBuf[i][0] != edgeBuf[i][1]) {
            if(!added[edgeBuf[i][0]]) {
                added[edgeBuf[i][0]] = true;
                AddNode(edgeBuf[i][0]);
            }
            if(!added[edgeBuf[i][1]]) {
                added[edgeBuf[i][1]] = true;
                AddNode(edgeBuf[i][1]);
            }
            if(!HasEdge(edgeBuf[i][0], edgeBuf[i][1])) {
                AddEdge(edgeBuf[i][0], edgeBuf[i][1]);
            }
        }
    }
        
    std::cout << "Graph loaded!\n";
    std::cout << "Number of nodes: " << NodeSize() << std::endl;
    std::cout << "Number of edges: " << EdgeSize() << std::endl;
    std::cout << "----------------------------\n";
}

void Graph::AddNode(Node v)
{
    m_nodeList.push_back(v);
    m_adjacencyList[v];
    ++m_nodeSize;
}

void Graph::RemoveNode(Node v) 
{
    // remove node v
    m_nodeSize--;
    for(NodeList::iterator it=m_nodeList.begin();it!=m_nodeList.end();it++) {
        if(*it == v) {
            m_nodeList.erase(it);
            break;
        }
    } 
    // remove edges
    NodeList v_adj = m_adjacencyList[v];
    for(Index i=0;i<v_adj.size();++i) {
        RemoveEdge(v, v_adj[i]);
    }
    // remove m_adjacencyList[v]
    for(NodeListMap::iterator it=m_adjacencyList.begin();it!=m_adjacencyList.end();++it) {
        if(it->first==v) {
            m_adjacencyList.erase(it);
            break;
        }
    }
}

void Graph::AddEdge(Node v, Node u) 
{
    m_adjacencyList[v].push_back(u);
    m_adjacencyList[u].push_back(v);
    m_edgeSize++;
}

void Graph::RemoveEdge(Node v, Node u) 
{
    for(NodeList:: iterator it=m_adjacencyList[v].begin();it!=m_adjacencyList[v].end();++it) {
        if(*it == u) {
            m_adjacencyList[v].erase(it);
            break;
        }
    }
    for(NodeList:: iterator it=m_adjacencyList[u].begin();it!=m_adjacencyList[u].end();++it) {
        if(*it == v) {
            m_adjacencyList[u].erase(it);
            break;
        }
    }
    m_edgeSize--;
}

bool Graph::HasNode(Node v) const
{
    for(NodeList::const_iterator it=m_nodeList.begin(); it!=m_nodeList.end(); ++it)
    {
        if(*it == v)
            return true;
    }
    return false;
}

bool Graph::HasEdge(Node v, Node u) const
{
    for(NodeList::const_iterator it=m_adjacencyList.at(v).begin(); it!=m_adjacencyList.at(v).end();++it)
        if(*it == u)
            return true;
    return false;
}

Size Graph::NodeSize() const
{
    return m_nodeSize;
}

Size Graph::EdgeSize() const
{
    return m_edgeSize;
}

Node Graph::NodeAt(Index i)   const
{
    return m_nodeList[i];
}

const NodeList& Graph::GetNodeList() const
{
    return m_nodeList;
}

const NodeList& Graph::NeighborsOf(Node v) const
{
    return m_adjacencyList.at(v);
}
