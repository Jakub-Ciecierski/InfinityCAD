//
// Created by jakub on 12/29/15.
//

#ifndef BACHELOR_THESIS_PSO_OBJECT_H
#define BACHELOR_THESIS_PSO_OBJECT_H

namespace pso {
/*
 * Provides Interface for object which will be decoded as a particle.
 *
 * The class that is needed to be optimized, must be inheriting from
 * this abtract class.
 */
class PSOObject {
private:
    //-----------------------------------------------------------//
    //  PRIVATE FIELDS
    //-----------------------------------------------------------//

    //-----------------------------------------------------------//
    //  PRIVATE METHODS
    //-----------------------------------------------------------//

public:
    //-----------------------------------------------------------//
    //  CONSTRUCTORS
    //-----------------------------------------------------------//

    PSOObject();

    virtual ~PSOObject() = 0;
};
}

#endif //BACHELOR_THESIS_PSO_OBJECT_H
