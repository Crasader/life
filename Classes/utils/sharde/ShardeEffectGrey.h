//
//  ShardeEffectGrey.h
//  shaarm
//
//  Created by ff on 17/12/21.
//
//

#ifndef ShardeEffectGrey_h
#define ShardeEffectGrey_h

#include "ShardeEffect.h"

class ShardeEffectGrey : public ShardeEffect {
public:
    CREATE_FUNC(ShardeEffectGrey);
    virtual void setTarget(cocos2d::Texture2D *texture) override;
protected:
    bool init();
};
#endif /* ShardeEffectGrey_h */
