//
//  ShardeEffectNone.cpp
//  shaarm
//
//  Created by ff on 17/12/19.
//
//

#include "ShardeEffectNone.h"

USING_NS_CC;

void ShardeEffectNone::setTarget(cocos2d::Texture2D *texture)
{
//    if (_glprogramstate == nullptr)
//        return;
//    
//    Size size = sprite->getTexture()->getContentSizeInPixels();
//    _glprogramstate->setUniformVec2("resolution", size);
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
//    _glprogramstate->setUniformFloat("blurRadius", _blurRadius);
//    _glprogramstate->setUniformFloat("sampleNum", _blurSampleNum);
//#endif
}

bool ShardeEffectNone::init()
{
    _glprogramstate = GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP);
    CC_SAFE_RETAIN(_glprogramstate);
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
//    initGLProgramState("Shaders/example_Blur.fsh");
//#else
//    initGLProgramState("Shaders/example_Blur_winrt.fsh");
//#endif
//    _blurRadius = blurRadius;
//    _blurSampleNum = sampleNum;
    
    
    return true;
}
