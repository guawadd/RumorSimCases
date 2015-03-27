#include "SimCaseDriver.h"

int SimCaseDriver::RegularTreeSim(const char file[])
{
    Graph underlyingGraph(file);
    RCEstimator rcEstimator;

    const int numRumorSize = 60; // number of different rumor graph sizes
    const int step = 3; // step between two size
    const int numExp = 5000; // number of experiments per size

    int rumorSize[numRumorSize]; // e.g. 10, 20, 30, 40, ...
    double corrFreq[numRumorSize]; // correct detection frequency of different sizes
    for( int i=0; i<numRumorSize; ++i )
        rumorSize[i] = (i+1)*step;
    
    for(int i=0; i<numRumorSize; ++i)
    {
        double detectedCnt = 0;

        for(int j=0; j<numExp; ++j)
        {
            Graph rumorGraph;
            Node source = underlyingGraph.NodeAt(rand()%underlyingGraph.NodeSize());
            Alg::SpreadRumor(underlyingGraph, rumorGraph, source, rumorSize[i]);
            Alg::PrintGraph(rumorGraph, ".\\testgraph.txt");

            Node inferedSource = rcEstimator.infer(rumorGraph);

            if( source == inferedSource )
                ++detectedCnt;
        }

        std::cout << "Rumor size " << rumorSize[i] << " completed!\n";
        corrFreq[i] = detectedCnt/numExp;
    }

    std::string outPath(".\\result_");
    outPath.append(file);
    std::ofstream output(outPath);
    
    output << "Input \n----------------------------------------\n";
    output << file << std::endl;
    output << "Number of nodes in underlying graph: " << underlyingGraph.NodeSize() << std::endl;
    output << "Number of edges in underlying graph: " << underlyingGraph.EdgeSize() << std::endl;
    output << "Number of different rumor graph sizes: " << numRumorSize << std::endl;
    output << "Step between sizes: " << step << std::endl;
    output << "Number of experiments per size: " << numExp << std::endl;
    output << "----------------------------------------\n\n";
    
    output << "Output \n----------------------------------------\n";
    output << "rumor graph size, correct detection frequency\n";
    for(int i=0; i<numRumorSize; ++i)
        output << rumorSize[i] << ", " << corrFreq[i] << std::endl;
    "----------------------------------------\n";
    
    output.close();

    std::cout << "All done!\n";
    return 0;
}

int SimCaseDriver::GeneralGraphSim(const char file[], int rumorSize, int numExp)
{
    Graph underlyingGraph(file);

    std::string outPath(".\\result_");
    outPath.append(file);
    std::ofstream output(outPath);
    output << "True source";
    output << ", BFS_Heuristic(error hop)";
    output << ", Levelwise_Heuristic(error hop)";
        
    output << std::endl;

    BfsHeuristic bfsHeu;
    LwHeuristic lwHeu;
        
    double errorHopBFS[10] = {0};
    double errorHopLW[10] = {0};
        
    std::cout << "-------------------------------------\n";
    std::cout << "Experiment: number of edges in rumor graph\n";
    for(int i=1;i<=numExp;) 
    {
        Graph rumorGraph;
        Node source = underlyingGraph.NodeAt(rand()%underlyingGraph.NodeSize());
        Alg::SpreadRumor(underlyingGraph, rumorGraph, source, rumorSize);
        //std::string graphname = ".\\rumor_graph" + std::to_string((int long long)i) + ".txt";
        //Alg::PrintGraph(rumorGraph, graphname.c_str());

        if(i%10==0)
            std::cout << i << " experiments done!\n";

        if( rumorGraph.EdgeSize()>rumorSize*1.5 )
            continue;

        bool skip = false;
        Node centerLW  = lwHeu.infer(rumorGraph, underlyingGraph, skip);
        if(skip)
            continue;
        Node centerBFS = bfsHeu.infer(rumorGraph, underlyingGraph); 
            
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
    output << "Error hop frequency: 0(correct detection), 1, 2, 3,...\n";

    output << "BFS: ";
    for(int i=0;i<10;++i)
    {
        output << errorHopBFS[i]/numExp << ", ";
    }
    output  << std::endl;
    output << "LV: ";
    for(int i=0;i<10;++i)
    {
        output << errorHopLW[i]/numExp << ", ";
    }
    output  << std::endl;
    output.close();

    std::cout << "All done!\n";
    return 0;
}
