//
//  RotateAction.cpp
//  son
//
//  Created by ff on 15-5-28.
//
//

#include "RotateAction.h"

RotateAction::RotateAction(float ang, float rad, float x, float y)
{
    angle = ang;
    radius = rad;
    ox = x;
    oy = y;
    resetScreenXY();
}

void RotateAction::resetOriginXY(float x, float y)
{
    ox = x;
    oy = y;
    float dx = sx - ox;
    float dy = sy - oy;
    radius = sqrt(pow(dx, 2) + pow(dy, 2));
    sina = CC_RADIANS_TO_DEGREES(asinf(dy/radius));
    cosa = CC_RADIANS_TO_DEGREES(acos(dx/radius));
    if (radius == 0) {
        angle = 0;
        return;
    }
    
    if (dx >= 0 && dy >= 0) {
        angle = sina;
    }else if(dx <= 0 && dy >= 0){
        angle = cosa;
    }else if(dx <= 0 && dy <= 0){
        angle = -sina + 180;
    }else if(dx <= 0 && dy <= 0){
        angle = sina + 360;
    }
}

void RotateAction::resetScreenXY()
{
    sx = cos(angle * M_PI / 180)*radius + ox;
    sy = sin(angle * M_PI / 180)*radius + oy;
}
