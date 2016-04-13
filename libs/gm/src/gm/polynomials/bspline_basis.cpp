//
// Created by jakub on 4/12/16.
//

#include <gm/polynomials/bspline_basis.h>

using namespace glm;
using namespace std;

vec4 computeBSpline(const vector<ic::Point*>& deboorPoints,
                    const vector<float>& knotVector,
                    float t, int n){
    float x,y,z;
    x = y = z = 0;
    for(unsigned int i = 0; i < deboorPoints.size(); i++){
        float bsplineBasisValue = bsplineRecurive(t, n, i, knotVector);

        const vec3& pos = deboorPoints[i]->getPosition();
        x += pos.x * bsplineBasisValue;
        y += pos.y * bsplineBasisValue;
        z += pos.z * bsplineBasisValue;
    }
    return vec4(x,y,z,1);
}

float bsplineRecurive(float t, int n, int i,
                      const vector<float>& knotVector){
    if (n == 0){
        if(t >= knotVector[i] && t < knotVector[i+1])
            return 1;
        else
            return 0;
    }
    float leftRecursion = (t - knotVector[i]);
    float leftDenominator = (knotVector[i+n] - knotVector[i]);
    if(leftRecursion == 0 || leftDenominator == 0)
        leftRecursion = 1;
    else
        leftRecursion /= leftDenominator;

    float rightRecursion = knotVector[i+1+n] - t;
    float rightDenominator = (knotVector[i+1+n] - knotVector[i+1]);
    if(rightRecursion == 0 || rightDenominator == 0)
        rightRecursion = 1;
    else
        rightRecursion /= rightDenominator;

    leftRecursion *= bsplineRecurive(t, n-1, i, knotVector);
    rightRecursion *= bsplineRecurive(t, n-1, i+1, knotVector);

    return leftRecursion + rightRecursion;
}
