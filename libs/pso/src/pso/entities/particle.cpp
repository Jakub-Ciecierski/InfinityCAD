//
// Created by jakub on 12/29/15.
//

#include <pso/entities/particle.h>

//-----------------------------------------------------------//
//  CONSTRUCTORS
//-----------------------------------------------------------//

using namespace pso;

Particle::Particle(double posIntervalMin, double posIntervalMax,
                            double velIntervalMin, double velIntervalMax,
                            double maxVelocity,
                            Point<double> position, Point<double> velocity) :
                posIntervalMin(posIntervalMin), posIntervalMax(posIntervalMax),
                velIntervalMin(velIntervalMin), velIntervalMax(velIntervalMax),
                maxVelocity(maxVelocity),
                position(position), velocity(velocity),
                pbest(position), lbest(position){
    fitness = 99999;
    bestFitness = 99999;
}

Particle::~Particle() {

}

//-----------------------------------------------------------//
//  PUBLIC METHODS
//-----------------------------------------------------------//

void Particle::saveCurrentConfigAsBest(){
    this->bestFitness = this->fitness;
    this->pbest = this->position;
}

//-----------------------------------------------------------//
//  GETTERS
//-----------------------------------------------------------//

double Particle::getPosIntervalMin() const {
    return posIntervalMin;
}

double Particle::getPosIntervalMax() const {
    return posIntervalMax;
}

double Particle::getVelIntervalMin() const {
    return velIntervalMin;
}

double Particle::getVelIntervalMax() const {
    return velIntervalMax;
}

double Particle::getMaxVelocity() const {
    return maxVelocity;
}

const PointDouble& Particle::getPosition() const {
    return position;
}

const PointDouble& Particle::getVelocity() const {
    return velocity;
}

const PointDouble& Particle::getPbest() const {
    return pbest;
}

const PointDouble& Particle::getLbest() const {
    return lbest;
}

double Particle::getFitness() const {
    return fitness;
}

double Particle::getBestFitness() const {
    return bestFitness;
}

//-----------------------------------------------------------//
//  SETTERS
//-----------------------------------------------------------//

void Particle::setFitness(double fitness) {
    this->fitness = fitness;
}

void Particle::setBestFitness(double fitness) {
    this->bestFitness = fitness;
}

void Particle::setPosition(PointDouble position) {
    this->position = position;
}

void Particle::setVelocity(PointDouble velocity) {
    this->velocity = velocity;
}

void Particle::setPbest(PointDouble pbest) {
    this->pbest = pbest;
}

void Particle::setLbest(PointDouble lbest) {
    this->lbest= lbest;
}

void Particle::setPositionDim(double value, int dim){
    if(dim < 0 || dim >= this->position.size()){
        throw std::invalid_argument("Particle::setVelocityDimension");
    }
    this->position[dim] = value;
}

void Particle::setVelocityDim(double value, int dim){
    if(dim < 0 || dim >= this->velocity.size()){
        throw std::invalid_argument("Particle::setVelocityDimension");
    }
    this->velocity[dim] = value;
}
