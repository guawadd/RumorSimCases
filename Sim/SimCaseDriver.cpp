#include "SimCaseDriver.h"

int SimCaseDriver::RegularTreeSim(const char file[])
{
    Graph underlyingGraph(file);
    RCEstimator rcEstimator;

    const int numRumorSize = 50; // number of different rumor graph sizes
    const int step = 3; // step between two size
    const int numExp = 5000; // number of experiments per size

    int rumorSize[numRumorSize]; // e.g. 10, 20, 30, 40, ...
    double corrFreq[numRumorSize]; // correct detection frequency of different sizes
    for( int i=0; i<numRumorSize; ++i )
        rumorSize[i] = (i+1)*step;
    
    const clock_t begin_time = clock();
    for(int i=0; i<numRumorSize; ++i)
    {
        double detectedCnt = 0;

        for(int j=0; j<numExp; ++j)
        {
            Graph rumorGraph;
            //Node source = underlyingGraph.NodeAt(rand()%underlyingGraph.NodeSize());
            Node source = underlyingGraph.NodeAt(0);
            
            Alg::SpreadRumor(underlyingGraph, rumorGraph, source, rumorSize[i]);
            //Alg::PrintGraph(rumorGraph, ".\\testgraph.txt");

            Node inferedSource = rcEstimator.infer(rumorGraph);

            if( source == inferedSource )
                ++detectedCnt;
        }

        //std::cout << "Rumor size " << rumorSize[i] << " completed!\n";
        corrFreq[i] = detectedCnt/numExp;
    }
    std::cout << "running time: " << double( clock () - begin_time ) /  CLOCKS_PER_SEC << std::endl;

    /*std::string outPath(".\\result_");
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

    std::cout << "All done!\n";*/
    return 0;
}

int SimCaseDriver::RegularCyclicSim(const char file[])
{
    Graph underlyingGraph(file);
    RegCycHeu regCycHeu;

    const int numRumorSize = 50; // number of different rumor graph sizes
    const int step = 3; // step between two size
    const int numExp = 5000; // number of experiments per size

    int rumorSize[numRumorSize]; // e.g. 10, 20, 30, 40, ...
    double corrFreq[numRumorSize]; // correct detection frequency of different sizes
    for( int i=0; i<numRumorSize; ++i )
        rumorSize[i] = (i+1)*step;
    
    const clock_t begin_time = clock();
    for(int i=0; i<numRumorSize; ++i)
    {
        double detectedCnt = 0;

        for(int j=0; j<numExp; ++j)
        {
            Graph rumorGraph;
            //Node source = underlyingGraph.NodeAt(rand()%underlyingGraph.NodeSize());
            Node source = underlyingGraph.NodeAt(0);

            Alg::SpreadRumor(underlyingGraph, rumorGraph, source, rumorSize[i]);
            //Alg::PrintGraph(rumorGraph, ".\\testgraph.txt");

            Node inferedSource = regCycHeu.infer(rumorGraph);

            if( source == inferedSource )
                ++detectedCnt;
        }

        //std::cout << "Rumor size " << rumorSize[i] << " completed!\n";
        corrFreq[i] = detectedCnt/numExp;
    }
    std::cout << "cycle running time: " << double( clock () - begin_time ) /  CLOCKS_PER_SEC << std::endl;

    /*std::string outPath(".\\result_");
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

    std::cout << "All done!\n";*/
    return 0;
}

int SimCaseDriver::GeneralGraphSim(const char file[], int rumorSize, int numExp)
{
    //Graph underlyingGraph(file);
    Graph underlyingGraph("toy.txt");
    Graph rumorGraph("toytoy.txt");

    std::string outPath(".\\result_");
    outPath.append(file);
    std::ofstream output(outPath);
    output << "True source";
    output << ", Closeness center";
    output << ", Random BFS center";
    output << ", Jordan center";
    output << ", STS center";
    output << ", STSP center";
        
    output << std::endl;

    CcHeuristic ccHeu;
    BfsHeuristic bfsHeu;
    EcHeuristic ecHeu;
    STSHeuristic stsHeu;
    STSPHeuristic stspHeu;
    //LwHeuristic lwHeu;

    double errorHopCC[10] = {0};
    double errorHopBFS[10] = {0};
    double errorHopEC[10] = {0};
    double errorHopSTS[10] = {0};
    double errorHopSTSP[10] = {0};
    //double errorHopLW[10] = {0};
        
    std::cout << "-------------------------------------\n";
    std::cout << "Experiment: number of edges in rumor graph\n";

    Node source = 1;
    if( std::strcmp(file,"as.txt")==0 )
        source = 194;
    else if( std::strcmp(file,"facebook.txt")==0 )
        source = 108;
    else if( std::strcmp(file,"lattice.txt")==0 )
        source = 25199;
    else if(std::strcmp(file,"power_grid.txt")==0 )
        source = 1309;
    else if( std::strcmp(file,"scale_free.txt")==0 )
        source = 1;
    else if( std::strcmp(file,"small_world.txt")==0 )
        source = 28563;
    else
    {
        std::cout << "holy shit\n";
        source = underlyingGraph.NodeAt(rand()%underlyingGraph.NodeSize());
    }

    std::cout << "fuck\n";
    for(int i=1; i<=1; ++i) 
    {
        //Graph rumorGraph;
        //Node source = underlyingGraph.NodeAt(rand()%underlyingGraph.NodeSize());
        //Alg::SpreadRumor(underlyingGraph, rumorGraph, source, rumorSize);
        //std::string graphname = ".\\rumor_graph" + std::to_string((int long long)i) + ".txt";
        //Alg::PrintGraph(rumorGraph, graphname.c_str());

        if(i%10==0)
            std::cout << i << " experiments done!\n";

        //if( rumorGraph.EdgeSize()>rumorSize*1.5 )
        //    continue;

        //bool skip = false;
        //Node centerLW  = lwHeu.infer(rumorGraph, underlyingGraph, skip);
        //if(skip)
        //    continue;
        Node centerBFS = bfsHeu.infer(rumorGraph, underlyingGraph); 
        Node centerCC = ccHeu.infer(rumorGraph, underlyingGraph);
        Node centerEC = ecHeu.infer(rumorGraph, underlyingGraph);
        Node centerSTS = stsHeu.infer(rumorGraph, underlyingGraph, 100);
        Node centerSTSP = stspHeu.infer(rumorGraph, underlyingGraph, 100);

        Size distCC = Alg::Distance(rumorGraph, centerCC, source);
        Size distBFS = Alg::Distance(rumorGraph, centerBFS, source);
        Size distEC = Alg::Distance(rumorGraph, centerEC, source);
        Size distSTS = Alg::Distance(rumorGraph, centerSTS, source);
        Size distSTSP = Alg::Distance(rumorGraph, centerSTSP, source);
        //Size distLW  = Alg::Distance(rumorGraph, centerLW, source);
        
        ++errorHopCC[distCC];
        ++errorHopBFS[distBFS];
        ++errorHopEC[distEC];
        ++errorHopSTS[distSTS];
        ++errorHopSTSP[distSTSP];
        //++errorHopLW[distLW];
            
        output << rumorGraph.NodeAt(0);

        output << ", " << centerCC << "(" << distCC << ")";
        output << ", " << centerBFS << "(" << distBFS << ")";
        output << ", " << centerEC << "(" << distEC << ")";
        output << ", " << centerSTS << "(" << distSTS << ")";
        output << ", " << centerSTSP << "(" << distSTSP << ")";
        //output << ", " << centerLW << "(" << distLW << ")";

        output << std::endl;
    }
    output << "==========================\n";
    output << "Error hop frequency: 0(correct detection), 1, 2, 3,...\n";

    output << "EC: ";
    for(int i=0;i<10;++i)
    {
        output << errorHopEC[i]/numExp << ", ";
    }
    output  << std::endl;
    output << "CC: ";
    for(int i=0;i<10;++i)
    {
        output << errorHopCC[i]/numExp << ", ";
    }
    output  << std::endl;
    output << "BFS: ";
    for(int i=0;i<10;++i)
    {
        output << errorHopBFS[i]/numExp << ", ";
    }
    output  << std::endl;
    output << "STS: ";
    for(int i=0;i<10;++i)
    {
        output << errorHopSTS[i]/numExp << ", ";
    }
    output  << std::endl;
    output << "STSP: ";
    for(int i=0;i<10;++i)
    {
        output << errorHopSTSP[i]/numExp << ", ";
    }
    output  << std::endl;
    /*output << "LV: ";
    for(int i=0;i<10;++i)
    {
        output << errorHopLW[i]/numExp << ", ";
    }*/
    output  << std::endl;

    output.close();

    std::cout << "All done!\n";
    return 0;
}
