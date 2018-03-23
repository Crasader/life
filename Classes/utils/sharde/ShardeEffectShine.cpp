//
//  ShardeEffectShine.cpp
//  life
//
//  Created by ff on 17/12/22.
//
//

#include "ShardeEffectShine.h"

USING_NS_CC;

void ShardeEffectShine::setTarget(cocos2d::Texture2D *texture)
{
    //    if (_glprogramstate == nullptr)
    //        return;
    
}

bool ShardeEffectShine::init()
{
    initGLProgramState("Shaders/efx_shine.fsh");
    _glprogramstate->setUniformFloat("factor", 2);
    return true;
}
