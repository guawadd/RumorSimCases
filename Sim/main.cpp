#include <iostream>
#include <time.h>
#include "Graph.h"
#include "Debugger.h"
#include "SimCaseDriver.h"

int printUsage()
{
    std::cout << "Usage: sim.exe r file                                          (for regular tree simulation)\n";
    std::cout << "       sim.exe g file size_of_rumor_graph number_of_experiment (for general graph simulation)";
    return 1;
}

int main(int argc, char* args[]) {
    // set seed for random number, required only once throughout the program
    srand( (unsigned)time(0));

    // run simulation case for regular tree
    //if( argc==3 && std::strcmp(args[1], "r")==0 )
    //    return SimCaseDriver::RegularTreeSim(args[1]);

    // run simulatio case for general graph
    //else if( argc==5 && std::strcmp(args[1], "g")==0 )
      //  return SimCaseDriver::GeneralGraphSim(args[2], std::atoi(args[3]), std::atoi(args[4]));
    //return SimCaseDriver::GeneralGraphSim(args[1], 100, 1);
    //else
      //  return printUsage();

    //return SimCaseDriver::RegularCyclicSim(args[1]);
    /*Graph g(args[1]);
    Debugger debugger;
    debugger.testBFSTree(g);*/

         SimCaseDriver::RegularTreeSim("1000000_reg6_tree.txt");
    SimCaseDriver::RegularCyclicSim("1000000_regcyc6_tree.txt");
}