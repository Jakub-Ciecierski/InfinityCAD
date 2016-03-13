//
// Created by jakub on 3/5/16.
//

#ifndef MG1_COLOR_CONVERTOR_H
#define MG1_COLOR_CONVERTOR_H

/*
 * Converts GL to standart color intervals
 * GL color interval =  [0,1]
 * Standard =           [0, 255]
 */

const unsigned int COLOR_STD_MIN = 0;
const unsigned int COLOR_STD_MAX = 255;

const float COLOR_GL_MIN = 0.0f;
const float COLOR_GL_MAX = 1.0f;

unsigned int colorGLToSTD(float x);

float colorSTDToGL(unsigned int x);


#endif //MG1_COLOR_CONVERTOR_H