//
// Created by jakub on 1/30/16.
//

#include "pso/console/info_printer.h"

using namespace std;

namespace pso {

InfoPrinter::InfoPrinter() {
    numberOfLinesToReset = 0;
}

void InfoPrinter::printInfo(TimeMeasures& timeMeasures,
                            int currentIteration,
                            int maxIterations,
                            int swarmSize,
                            Particle* bestParticle,
                            Particle* globalBestParticle) {
    resetConsole();

    printHeader();

    printPSOInfo(currentIteration, maxIterations, swarmSize);
    printTimeInfo(timeMeasures);
    printFitnessInfo(bestParticle, globalBestParticle);

}

    void InfoPrinter::printBug(){
        cout << prefix << "" << endl;
        cout << prefix << "            .-. " << endl;
        cout << prefix << "           o   \\     .-." << endl;
        cout << prefix << "               .----.\'   \\" << endl;
        cout << prefix << "              .\'o)  / `.   o" << endl;
        cout << prefix << "             /         |" << endl;
        cout << prefix << "             \\_)       /-." << endl;
        cout << prefix << "              \'_.`    \\  \\" << endl;
        cout << prefix << "               `.      |  \\" << endl;
        cout << prefix << "               |       \\ |" << endl;
        cout << prefix << "           .--/`-.     / /" << endl;
        cout << prefix << "          .'.-/`-. `.  .\\|" << endl;
        cout << prefix << "         /.' /`._ `-    '-." << endl;
        cout << prefix << "    ____(|__/`-..`-   '-._ \\" << endl;
        cout << prefix << "   |`------.'-._ `      ||\\ \\" << endl;
        cout << prefix << "   || #   /-.   `   /   || \\|" << endl;
        cout << prefix << "   ||   #/   `--'  /  /_::_|)__" << endl;
        cout << prefix << "   `|____|-._.-`  /  ||`--------`" << endl;
        cout << prefix << "         \\-.___.` | / || #      |" << endl;
        cout << prefix << "          \\       | | ||   #  # |" << endl;
        cout << prefix << "          /`.___.'\\ |.`|________|" << endl;
        cout << prefix << "          | /`.__.'|'.`" << endl;
        cout << prefix << "        __/ \\    __/ \\" << endl;
        cout << prefix << "       /__.-.)  /__.-.)" << endl;
        cout << prefix << endl << endl;
        numberOfLinesToReset += 26;
    }

    void InfoPrinter::printHeader() {
        cout << endl;
        numberOfLinesToReset++;
        cout << prefixIcons << " ___  ___  ___" << endl;
        cout << prefixIcons << "| _ \\/ __|/ _ \\" << endl;
        cout << prefixIcons << "|  _/\\__ \\ (_) |" << endl;
        cout << prefixIcons << "|_|  |___/\\___/" << endl;
        cout << prefixIcons << "                    " << endl;
        numberOfLinesToReset += 5;
    }

    void InfoPrinter::resetConsole() {
        // Clean previous entry
        for (int i = 0; i < numberOfLinesToReset; i++) {
            cout << "\e[A\r";
        }
        numberOfLinesToReset = 0;
    }

    void InfoPrinter::printPSOInfo(int currentIteration,
                                   int maxIterations, int swarmSize) {
        cout << prefix << "[Iteration              ]: "
        << currentIteration << "/" << maxIterations << endl;
        numberOfLinesToReset++;

        cout << endl;
        numberOfLinesToReset++;

        cout << prefix << "[Swarm Size             ]: "
        << swarmSize << endl;
        numberOfLinesToReset++;

        cout << endl;
        numberOfLinesToReset++;
    }

    void InfoPrinter::printTimeInfo(TimeMeasures& timeMeasures) {
        cout << prefix << "[Overall Time           ]: "
        << timeMeasures.overallTime << endl;
        numberOfLinesToReset++;

        cout << prefix << "[ETA Time               ]: "
        << timeMeasures.ETA << endl;
        numberOfLinesToReset++;

        cout << endl;
        numberOfLinesToReset++;

        cout << prefix << "[Last Fitness Time      ]: "
        << timeMeasures.lastFitnessTime << endl;
        numberOfLinesToReset++;

        cout << prefix << "[Last Neigh Time        ]: "
        << timeMeasures.lastNeighbourTime << endl;
        numberOfLinesToReset++;

        cout << prefix << "[Last Particle Time     ]: "
        << timeMeasures.lastParticleUpdateTime << endl;
        numberOfLinesToReset++;

        cout << endl;
        numberOfLinesToReset++;
    }

    void InfoPrinter::printFitnessInfo(Particle* bestParticle,
                                       Particle* globalBestParticle) {
        double bestFitness = 0;
        if (bestParticle != NULL) {
            bestFitness = bestParticle->getBestFitness();
        }
        cout << prefix << "[Best Fitness Cur Gen   ]: "
        << bestFitness << endl;
        numberOfLinesToReset++;

        cout << prefix << "[Best Fitness All Gen   ]: "
        << globalBestParticle->getBestFitness() << endl;
        numberOfLinesToReset++;
    }
}