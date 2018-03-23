//
//  ShardeEffectNone.h
//  shaarm
//
//  Created by ff on 17/12/19.
//
//

#ifndef ShardeEffectNone_h
#define ShardeEffectNone_h

#include "cocos2d.h"
#include "ShardeEffect.h"

class ShardeEffectNone : public ShardeEffect
{
public:
    CREATE_FUNC(ShardeEffectNone);
    virtual void setTarget(cocos2d::Texture2D *texture) override;
    
protected:
    bool init();
};

#endif /* ShardeEffectNone_h */
