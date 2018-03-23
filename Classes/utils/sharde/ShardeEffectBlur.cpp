//
//  ShardeEffectBlur.cpp
//  shaarm
//
//  Created by ff on 17/12/19.
//
//

#include "ShardeEffectBlur.h"


USING_NS_CC;

void ShardeEffectBlur::setTarget(Texture2D *texture)
{
    if (_glprogramstate == nullptr)
        return;
    
    Size size = texture->getContentSizeInPixels();
    _glprogramstate->setUniformVec2("resolution", size);
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
    _glprogramstate->setUniformFloat("blurRadius", _blurRadius);
    _glprogramstate->setUniformFloat("sampleNum", _blurSampleNum);
#endif
}

bool ShardeEffectBlur::init(float blurRadius, float sampleNum)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
    initGLProgramState("Shaders/example_Blur.fsh");
#else
    initGLProgramState("Shaders/example_Blur_winrt.fsh");
#endif
    _blurRadius = blurRadius;
    _blurSampleNum = sampleNum;
    
    return true;
}

void ShardeEffectBlur::setBlurRadius(float radius)
{
    _blurRadius = radius;
}

void ShardeEffectBlur::setBlurSampleNum(float num)
{
    _blurSampleNum = num;
}
