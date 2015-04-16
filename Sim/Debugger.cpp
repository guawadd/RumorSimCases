#include "Debugger.h"

void Debugger::testRCCalculator(const Graph& G)
{
    RCCalculator rcCalc;
    ScoreMap rc;
    //rcCalc.calcAll(G, rc);
    rcCalc.calcRegCyc(G, rc);

    //std::ofstream output(".\\test\\test_output_rcCalc.txt");
    std::ofstream output(".\\test\\test_output_regCyc.txt");
    for(ScoreMap::const_iterator it=rc.begin(); it!=rc.end(); ++it)
        output << it->first << ": " << it->second << std::endl;
    
    output.close();
}

void Debugger::testBFSTree(const Graph& G) 
{
    Graph bfsT;

    Alg::BFSTree(G, bfsT, G.NodeAt(0));
    Alg::PrintGraph(bfsT, ".\\test\\test_output_bfsTree.txt");
}

void Debugger::testRST(const Graph& G) 
{
    Graph RST;

    Alg::RST(G, RST);
    Alg::PrintGraph(RST, ".\\test\\test_output_RST.txt");
}

void Debugger::testSpreadRumor(const Graph& G)
{
    Graph rumorGraph;
    Size rumorSize = G.NodeSize()-1;
    Node source = G.NodeAt(0);

    Alg::SpreadRumor(G, rumorGraph, source, rumorSize);
    Alg::PrintGraph(rumorGraph, ".\\test\\test_output_SpreadRumor.txt");
}

void Debugger::testProbOfPerm(const Graph& G)
{
    Graph T;

    Alg::BFSTree(G, T, G.NodeAt(0));
    std::cout << "Probability for ";
    for(Index i=0; i<T.NodeSize(); ++i)
    {
        std::cout << T.NodeAt(i) << " ";
    }
    //std::cout << "considering cycle is " << Alg::ProbOfPerm(G, T.GetNodeList()) << std::endl;
    std::cout << "not considering cycle is " << Alg::ProbOfPermS(G, T.GetNodeList()) << std::endl;
}

void Debugger::testEstimator(const Graph& G, int trial, int rumorSize) 
{
    std::ofstream output;
    output.open(".\\result.txt");
    output << "True source";
    output << ", BFS_Heuristic(error hop)";
    output << ", Levelwise_Heuristic(error hop)";
        
    output << std::endl;

    BfsHeuristic bfsHeu;
    LwHeuristic lwHeu;
        
    double errorHopBFS[10] = {0};
    double errorHopLW[10] = {0};
        
    std::cout << "-------------------------------------\n";
    std::cout << "Trial: number of edges in rumor graph\n";
    for(int i=1;i<=trial;) 
    {
        Graph rumorGraph;
        Node source = G.NodeAt(rand()%G.NodeSize());
        Alg::SpreadRumor(G, rumorGraph, source, rumorSize);
        std::string graphname = ".\\rumor_graph" + std::to_string((int long long)i) + ".txt";
        Alg::PrintGraph(rumorGraph, graphname.c_str());

        std::cout << i << ": " << rumorGraph.EdgeSize() << std::endl;

        if( rumorGraph.EdgeSize()>rumorSize*1.5 )
            continue;

        bool skip = false;
        Node centerLW  = lwHeu.infer(rumorGraph, G, skip);
        if(skip)
            continue;
        Node centerBFS = bfsHeu.infer(rumorGraph, G); 
            
        Size distBFS = Alg::Distance(rumorGraph, centerBFS, source);
        Size distLW  = Alg::Distance(rumorGraph, centerLW, source);

        ++errorHopBFS[distBFS];
        ++errorHopLW[distLW];
            
        output << rumorGraph.NodeAt(0);

        output << ", " << centerBFS << "(" << distBFS << ")";
        output << ", " << centerLW << "(" << distLW << ")";

        output << std::endl;

        ++i;
    }
    output << "==========================\n";
    output << "Error hop frequency: 0(correct), 1, 2, 3,...\n";

    output << "BFS: ";
    for(int i=0;i<10;++i)
    {
        output << errorHopBFS[i]/trial << ", ";
    }
    output  << std::endl;
    output << "LV: ";
    for(int i=0;i<10;++i)
    {
        output << errorHopLW[i]/trial << ", ";
    }
    output  << std::endl;
    output.close();
}
