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
    if( argc==3 && std::strcmp(args[1], "r")==0 )
        return SimCaseDriver::RegularTreeSim(args[2]);

    // run simulatio case for general graph
    else if( argc==5 && std::strcmp(args[1], "g")==0 )
        return SimCaseDriver::GeneralGraphSim(args[2], std::atoi(args[3]), std::atoi(args[4]));

    else
        return printUsage();
}