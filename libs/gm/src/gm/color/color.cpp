//
// Created by jakub on 3/5/16.
//


#include <gm/color/color.h>
#include <gm/color/color_convertor.h>

Color::Color(float R, float G, float B) :
    R(R), G(G), B(B){
    clamp();
}

Color::Color(float R, float G, float B, float Alpha) :
        R(R), G(G), B(B), Alpha(Alpha){
    clamp();
}

void Color::clamp() {
    if(R < 0) R = 0;
    if(R > 1.0) R = 1.0;

    if(G < 0) G = 0;
    if(G > 1.0) G = 1.0;

    if(B < 0) B = 0;
    if(B > 1.0) B = 1.0;
}