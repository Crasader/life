//
//  ShardeEffect.h
//  shaarm
//
//  Created by ff on 17/12/19.
//
//

#ifndef ShardeEffect_h
#define ShardeEffect_h

#include "cocos2d.h"

class ShardeEffect : public cocos2d::Ref
{
public:
    cocos2d::GLProgramState* getGLProgramState() const { return _glprogramstate; }
    virtual void setTarget(cocos2d::Texture2D *texture){}
    
protected:
    bool initGLProgramState(const std::string &fragmentFilename);
    ShardeEffect();
    virtual ~ShardeEffect();
    cocos2d::GLProgramState* _glprogramstate;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    std::string _fragSource;
    cocos2d::EventListenerCustom* _backgroundListener;
#endif
};

#endif /* ShardeEffect_h */
