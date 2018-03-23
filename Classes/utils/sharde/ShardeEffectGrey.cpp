//
//  ShardeEffectGrey.cpp
//  shaarm
//
//  Created by ff on 17/12/21.
//
//

#include "ShardeEffectGrey.h"

USING_NS_CC;

void ShardeEffectGrey::setTarget(cocos2d::Texture2D *texture)
{
//    if (_glprogramstate == nullptr)
//        return;
    
}

bool ShardeEffectGrey::init()
{
    initGLProgramState("Shaders/example_GreyScale.fsh");
//    _glprogramstate->setUniformFloat("factor", 2);
    return true;
}
