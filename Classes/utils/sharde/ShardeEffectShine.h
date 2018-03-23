//
//  ShardeEffectShine.h
//  life
//
//  Created by ff on 17/12/22.
//
//

#ifndef ShardeEffectShine_h
#define ShardeEffectShine_h

#include "ShardeEffect.h"

class ShardeEffectShine : public ShardeEffect {
public:
    CREATE_FUNC(ShardeEffectShine);
    virtual void setTarget(cocos2d::Texture2D *texture) override;
protected:
    bool init();
};

#endif /* ShardeEffectShine_h */
