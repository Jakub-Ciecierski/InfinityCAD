//
// Created by jakub on 3/5/16.
//

#ifndef MG1_COLOR_H
#define MG1_COLOR_H

/*
 * The intesitity of color belongs to the interval [0,1]
 */
class Color {
private:
    void clamp();

public:
    float R;
    float G;
    float B;

    float Alpha;

    Color(float R, float G, float B);
    Color(float R, float G, float B, float Alpha);
};


#endif //MG1_COLOR_H