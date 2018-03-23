//
//  HitEffectLayer.cpp
//  cosoul
//
//  Created by ff on 14-10-23.
//
//

#include "HitEffectLayer.h"

USING_NS_CC;

bool HitEffectLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    return true;
}

void HitEffectLayer::addHurtNum(bool hero, int num, Vec2 pos)
{
    auto hurtNum = Effect::create();
    if (num < 0) {
        hurtNum->initWithNumber("ui/battle/hpNum.png", -num, 41, 44);
    }else{
        if (!hero) {
            hurtNum->initWithNumber("ui/battle/hurtNum.png", num, 41, 44);
        }else{
            hurtNum->initWithNumber("ui/battle/MhurtNum.png", num, 41, 44);
        }
        
    }
    
    hurtNum->setPosition(Vec2(pos.x, pos.y+210));
    addChild(hurtNum);
    
    auto scale = ScaleTo::create(0.1, 1.5);
    auto scale2 = ScaleTo::create(0.1, 0.6);
    
    auto scale3 = ScaleTo::create(0.8, 0.3);
    auto fade = FadeOut::create(0.5);
    auto move = MoveBy::create(0.8, Vec2(0, 100));
    auto eMove = EaseExponentialOut::create(move);
    
    auto phase = Spawn::create(scale3, fade, eMove, NULL);
    auto func = CallFunc::create(CC_CALLBACK_0(HitEffectLayer::delEffect, this, hurtNum));
    hurtNum->runAction(Sequence::create(scale, scale2, phase, func, NULL));
}

void HitEffectLayer::addCritHurtNum(bool hero, int num, Vec2 pos)
{
    auto critNum = Effect::create();
    if (!hero) {
        critNum->initWithNumber("ui/battle/critNum.png", num, 73, 77);
    }else{
        critNum->initWithNumber("ui/battle/McritNum.png", num, 73, 77);
    }
    
    critNum->setPosition(Vec2(pos.x, pos.y+210));
    addChild(critNum);
    
    
    auto scale = ScaleTo::create(0.1, 2);
    auto scale2 = ScaleTo::create(0.1, 0.8);
    
    auto scale3 = ScaleTo::create(0.8, 0.5);
    auto fade = FadeOut::create(0.5);
    auto move = MoveBy::create(0.8, Vec2(0, 100));
    auto eMove = EaseExponentialOut::create(move);
    
    auto phase = Spawn::create(scale3, fade, eMove, NULL);
    auto func = CallFunc::create(CC_CALLBACK_0(HitEffectLayer::delEffect, this, critNum));
    critNum->runAction(Sequence::create(scale, scale2, phase, func, NULL));
}

void HitEffectLayer::addCritFlag(Vec2 pos)
{
    auto crit = Effect::create();
    crit->initWithImg("ui/battle/crit.png");
    crit->setPosition(Vec2(pos.x+20, pos.y+200));
    addChild(crit);
    
    auto scale = ScaleTo::create(0.1, 2);
    auto scale2 = ScaleTo::create(0.1, 1);
    auto fade = FadeOut::create(0.2);
    auto func = CallFunc::create(CC_CALLBACK_0(HitEffectLayer::delEffect, this, crit));
    crit->runAction(Sequence::create(scale, scale2, fade, func, NULL));
}

void HitEffectLayer::addMissFlag(Vec2 pos)
{
    auto miss = Effect::create();
    miss->initWithImg("ui/battle/miss.png");
    miss->setPosition(Vec2(pos.x-20, pos.y+200));
    addChild(miss);
    
    auto scale = ScaleTo::create(0.1, 2);
    auto scale2 = ScaleTo::create(0.1, 0.5);
    auto fade = FadeOut::create(0.2);
    auto func = CallFunc::create(CC_CALLBACK_0(HitEffectLayer::delEffect, this, miss));
    miss->runAction(Sequence::create(scale, scale2, fade, func, NULL));
}

void HitEffectLayer::addArmatureEff(std::string resPath, cocos2d::Vec2 pos)
{
    auto eff = Effect::create();
    eff->initWithArmature(resPath, 1);
    eff->setPosition(Vec2(pos.x, pos.y));
    addChild(eff);
    
    eff->getArmature()->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(HitEffectLayer::delArmEffect, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void HitEffectLayer::delArmEffect(cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string &movementID)
{
    if (movementType == cocostudio::MovementEventType::COMPLETE)
    {
        removeChild(armature->getParent());
    }
}

void HitEffectLayer::delEffect(Effect *effect)
{
    effect->stopAllActions();
    removeChild(effect);
}
