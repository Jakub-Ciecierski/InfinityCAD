//
// Created by jakub on 12/29/15.
//

#ifndef BACHELOR_THESIS_PARTICLE_T_H
#define BACHELOR_THESIS_PARTICLE_T_H

#include <pso/pso_common.h>

namespace pso {
class Particle {
private:
    //-----------------------------------------------------------//
    //  PRIVATE FIELDS
    //-----------------------------------------------------------//

    double posIntervalMin;
    double posIntervalMax;

    double velIntervalMin;
    double velIntervalMax;

    double maxVelocity;

    Point<double> position;
    Point<double> velocity;

    Point<double> pbest;
    Point<double> lbest;

    double fitness;
    double bestFitness;

    //-----------------------------------------------------------//
    //  PRIVATE METHODS
    //-----------------------------------------------------------//

public:
    //-----------------------------------------------------------//
    //  CONSTRUCTORS
    //-----------------------------------------------------------//

    Particle(double posIntervalMin, double posIntervalMax,
                double velIntervalMin,double velIntervalMax,
                double maxVelocity,
               Point<double> position, Point<double> velocity);

    ~Particle();

    //-----------------------------------------------------------//
    //  PUBLIC METHODS
    //-----------------------------------------------------------//

    void saveCurrentConfigAsBest();

    //-----------------------------------------------------------//
    //  GETTERS
    //-----------------------------------------------------------//

    double getPosIntervalMin() const;
    double getPosIntervalMax() const;

    double getVelIntervalMin() const;
    double getVelIntervalMax() const;

    double getMaxVelocity() const;

    const PointDouble& getPosition() const;
    const PointDouble& getVelocity() const;

    const PointDouble& getPbest() const;
    const PointDouble& getLbest() const;

    double getFitness() const;
    double getBestFitness() const;

    //-----------------------------------------------------------//
    //  SETTERS
    //-----------------------------------------------------------//

    void setFitness(double fitness);
    void setBestFitness(double fitness);

    void setPosition(PointDouble position);
    void setVelocity(PointDouble velocity);

    void setPbest(PointDouble pbest);
    void setLbest(PointDouble lbest);

    /*
     * Set value of dim-th dimension of position vector
     */
    void setPositionDim(double value, int dim);

    /*
     * Set value of dim-th dimension of velocity vector
     */
    void setVelocityDim(double value, int dim);
};
}

#endif //BACHELOR_THESIS_PARTICLE_T_H
