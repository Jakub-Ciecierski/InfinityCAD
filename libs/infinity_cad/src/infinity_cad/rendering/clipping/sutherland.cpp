//
// Created by jakub on 4/2/16.
//

#include <infinity_cad/rendering/clipping/sutherland.h>

using namespace glm;

static int LEFT=1,RIGHT=2,BOTTOM=4,TOP=8;

int getCode(const vec4& v,
            float left, float right,
            float bottom, float top){
    int code = 0;
    //Perform Bitwise OR to get outcode
    if(v.y > top)
        code |=TOP;
    if(v.y < bottom)
        code |=BOTTOM;
    if(v.x < left)
        code |=LEFT;
    if(v.x > right)
        code |=RIGHT;
    return code;
}

bool clip(vec4& v1, vec4& v2,
          float left, float right,
          float bottom, float top){
    if(v1.z > -1.0f || v2.z > -1.0f) {
        return false;
    }

    int outcode1=getCode(v1, left, right, bottom, top);
    int outcode2=getCode(v2, left, right, bottom, top);

    bool accept = false;
    while(1){

        float m = (v2.y - v1.y) / (v2.x-v1.x);

        //std::cout << "m: " << m << std::endl;

        //Both points inside. Accept line
        if(outcode1==0 && outcode2==0){
            accept = true;
            break;
        }
            //AND of both codes != 0.Line is outside. Reject line
        else if((outcode1 & outcode2)!=0){
            break;
        }
        else{

            float x,y;
            int temp;
            //Decide if point1 is inside, if not, calculate intersection
            if(outcode1==0)
                temp = outcode2;
            else
                temp = outcode1;
            //Line clips top edge
            if(temp & TOP){
                x = v1.x;
                if (!isnan(m) && m != 0) x += (top-v1.y)/m;
                y = top;
            }
            else if(temp & BOTTOM){
                x = v1.x;
                if(!isnan(m) && m != 0) x += (bottom-v1.y)/m;

                y = bottom;

            }else if(temp & LEFT){
                x = left;
                y = v1.y;
                if(!isnan(m) && m != 0)y += m*(left-v1.x);

            }else if(temp & RIGHT){
                x = right;
                y = v1.y;
                if(!isnan(m) && m != 0) y += m*(right-v1.x);
            }
            //std::cout << "x: " << x << std::endl;
            //std::cout << "y: " << y << std::endl;
            //Check which point we had selected earlier as temp, and replace its co-ordinates
            if(temp == outcode1){
                v1.x = x;
                v1.y = y;
                outcode1 = getCode(v1, left, right, bottom, top);
            }else{
                v2.x = x;
                v2.y = y;
                outcode2 = getCode(v2, left, right, bottom, top);
            }
        }
    }
    //printvec4(v1);
    //printvec4(v2);
    return accept;
}