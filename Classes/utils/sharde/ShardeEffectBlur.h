//
//  ShardeEffectBlur.h
//  shaarm
//
//  Created by ff on 17/12/19.
//
//

#ifndef ShardeEffectBlur_h
#define ShardeEffectBlur_h

#include "cocos2d.h"
#include "ShardeEffect.h"

class ShardeEffectBlur : public ShardeEffect
{
public:
    CREATE_FUNC(ShardeEffectBlur);
    virtual void setTarget(cocos2d::Texture2D *texture) override;
    void setBlurRadius(float radius);
    void setBlurSampleNum(float num);
    
protected:
    bool init(float blurRadius = 10.0f, float sampleNum = 5.0f);
    
    float _blurRadius;
    float _blurSampleNum;
};


#endif /* ShardeEffectBlur_h */
