//
// Created by jakub on 3/5/16.
//

#include <gm/color/color_convertor.h>


unsigned int colorGLToSTD(float x) {
    return COLOR_STD_MAX * x;
}

float colorSTDToGL(unsigned int x) {
    return (x / (float)COLOR_STD_MAX * COLOR_GL_MAX);
}
