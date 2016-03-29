//
// Created by jakub on 3/29/16.
//

#ifndef MG1_CLOUDFITTER_H
#define MG1_CLOUDFITTER_H


#include <gm/rigid_body.h>
#include <gm/rendering/render_bodies/cloud.h>

class CloudFitter {
private:
    RigidBody* rigidBody;
    Cloud* cloud;

public:

    CloudFitter(RigidBody* rigidBody, Cloud* cloud);

    ~CloudFitter();

};


#endif //MG1_CLOUDFITTER_H
