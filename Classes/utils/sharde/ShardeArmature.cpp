//
//  ShardeArmature.cpp
//  shaarm
//
//  Created by ff on 17/12/19.
//
//

#include "ShardeArmature.h"
#include "ShardeEffectNone.h"
#include "ShardeEffectBlur.h"
#include "ShardeEffectGrey.h"
#include "ShardeEffectShine.h"

USING_NS_CC;
using namespace cocostudio;

bool ShardeArmature::init(const std::string &name)
{
    return Armature::init(name);
}

ShardeArmature *ShardeArmature::create(const std::string &name)
{
    ShardeArmature *armature = new ShardeArmature();
    if (armature && armature->init(name)) {
        armature->autorelease();
        return armature;
    }else{
        CC_SAFE_DELETE(armature);
        return nullptr;
    }
}

void ShardeArmature::setEffect(ShardeEffect* effect) {
    if(_defaultEffect != effect) {
        for (auto &object: _children) {
            if (Bone *bone = dynamic_cast<Bone *>(object)) {
                Node *node = bone->getDisplayRenderNode();
                
                if (nullptr == node) {
                    continue;
                }
                
                Skin *skin = static_cast<Skin *>(node);
                effect->setTarget(skin->getTexture());
                
                CC_SAFE_RELEASE(_defaultEffect);
                _defaultEffect = effect;
                CC_SAFE_RETAIN(_defaultEffect);
                
                skin->setGLProgramState(_defaultEffect->getGLProgramState());
            }
        }
    }
}

void ShardeArmature::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
{
    Armature::draw(renderer, transform, flags);
}

ShardeArmature::ShardeArmature() : _defaultEffect(nullptr)
{
    _effects.reserve(2);
}
ShardeArmature::~ShardeArmature() {
    for(auto &tuple : _effects) {
        std::get<1>(tuple)->release();
    }
    CC_SAFE_RELEASE(_defaultEffect);
}

void ShardeArmature::initEffect()
{
    _vectorIndex = 0;
    
    _spriteEffects.pushBack(ShardeEffectNone::create());
    _spriteEffects.pushBack(ShardeEffectShine::create());
    _spriteEffects.pushBack(ShardeEffectBlur::create());
    _spriteEffects.pushBack(ShardeEffectGrey::create());
    setEffect( _spriteEffects.at(_vectorIndex) );
}

void ShardeArmature::changeEffect(int index)
{
    //    _vectorIndex++;
    //    _vectorIndex %= _spriteEffects.size();
    _vectorIndex = index;
    setEffect( _spriteEffects.at(_vectorIndex) );
}
