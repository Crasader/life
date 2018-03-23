//
//  RotateAction.h
//  son
//
//  Created by ff on 15-5-28.
//
//

#ifndef __son__RotateAction__
#define __son__RotateAction__

#include "cocos2d.h"

class RotateAction
{
public:
    RotateAction(float ang, float rad, float x, float y);
    
    inline void addAngle(float add) { angle += add; }
    inline void addRadius(float add) { radius += add; }
    
    void resetOriginXY(float x, float y);
    
    void resetScreenXY();
private:
    float ox;
    float oy;
    
    float sina;
    float cosa;
    float angle;
    CC_SYNTHESIZE_READONLY(float, sx, DrawX);
    CC_SYNTHESIZE_READONLY(float, sy, DrawY);
    CC_SYNTHESIZE_READONLY(float, radius, Radius);
};

#endif /* defined(__son__RotateAction__) */
