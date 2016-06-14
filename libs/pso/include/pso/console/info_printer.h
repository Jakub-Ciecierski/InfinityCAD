//
// Created by jakub on 1/30/16.
//

#ifndef BACHELOR_THESIS_INFO_PRINTER_H
#define BACHELOR_THESIS_INFO_PRINTER_H

#include <pso/pso.h>

namespace pso {

class InfoPrinter {
private:
    //-----------------------------------------------------------//
    //  PRIVATE FIELDS
    //-----------------------------------------------------------//

    std::string prefix = "          ";
    std::string prefixIcons = "               ";

    int numberOfLinesToReset;

    //-----------------------------------------------------------//
    //  PRIVATE METHODS
    //-----------------------------------------------------------//

    void resetConsole();

    void printPSOInfo(int currentIteration,
                      int maxIterations, int swarmSize);

    void printTimeInfo(TimeMeasures& timeMeasures);

    void printFitnessInfo(Particle* bestParticle,
                          Particle* globalBestParticle);

    void printHeader();

    void printBug();


public:
    //-----------------------------------------------------------//
    //  CONSTRUCTORS
    //-----------------------------------------------------------//

    InfoPrinter();

    //-----------------------------------------------------------//
    //  PUBLIC METHODS
    //-----------------------------------------------------------//

    void printInfo(TimeMeasures& timeMeasures,
                   int currentIteration, int maxIterations,
                   int swarmSize,
                   Particle* bestParticle, Particle* globalBestParticle);
};
}

#endif //BACHELOR_THESIS_INFO_PRINTER_H
