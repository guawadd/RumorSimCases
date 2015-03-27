#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
#include <fstream>
#include <map>
#include <iostream>
#include <vector>

// type definitions
typedef unsigned Size;
typedef unsigned Index;

typedef unsigned                 Node;
typedef std::vector<Node>        NodeList;
typedef std::vector<NodeList>    NodeListVector;
typedef std::map<Node, NodeList> NodeListMap;

const Node DummyNode = 9999999;

typedef long double Score;
typedef std::map<Node, Score>    ScoreMap;
typedef std::map<Node, bool>     BoolMap;
typedef std::map<Node, unsigned> CounterMap;

typedef int Hop;
const Hop InfDist = -1;

// Graph stores graph data and implements graph operations
class Graph {
public:
    // Constructors
    Graph();

    // Special copy constructor, copy nodes only
    // Node v in G will be the first node in the new graph
    Graph(const Graph& G, Node v);

    // For data input from file
    Graph(const char file[]);
    
    // Graph operations
    // no validations for efficiency
    void AddNode(Node v);
    void RemoveNode(Node v);
    void AddEdge(Node v, Node u);
    void RemoveEdge(Node v, Node u);
    bool HasEdge(Node v, Node u) const;

    // Accessors
    Size NodeSize() const;
    Size EdgeSize() const;
    Node NodeAt(Index i)   const;
    const NodeList& GetNodeList() const;
    const NodeList& NeighborsOf(Node v) const;

private:
    // Graph data
    Size m_nodeSize;
    Size m_edgeSize;
    NodeList m_nodeList; 
    NodeListMap m_adjacencyList;
};

#endif