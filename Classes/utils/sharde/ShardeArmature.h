//
//  ShardeArmature.h
//  shaarm
//
//  Created by ff on 17/12/19.
//
//

#ifndef ShardeArmature_h
#define ShardeArmature_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ShardeEffect.h"

//static int tuple_sort( const std::tuple<ssize_t,Effect*,cocos2d::QuadCommand> &tuple1, const std::tuple<ssize_t,Effect*,cocos2d::QuadCommand> &tuple2 )
//{
//    return std::get<0>(tuple1) < std::get<0>(tuple2);
//}

class ShardeArmature : public cocostudio::Armature {
public:
    static ShardeArmature *create(const std::string& filename);
    
    bool init(const std::string &name);
    
    void initEffect();
    
    void setEffect(ShardeEffect* effect);
    
    void changeEffect(int index);
    
    void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
protected:
    ShardeArmature();
    ~ShardeArmature();
    
    ShardeEffect* _defaultEffect;
    //public:
    std::vector<std::tuple<ssize_t,ShardeEffect*,cocos2d::QuadCommand>> _effects;
    
    ssize_t _vectorIndex;
    cocos2d::Vector<ShardeEffect*> _spriteEffects;

};

#endif /* EffectArmature_h */
