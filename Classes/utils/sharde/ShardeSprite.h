//
//  ShardeSprite.h
//  shaarm
//
//  Created by ff on 17/12/19.
//
//

#ifndef ShardeSprite_h
#define ShardeSprite_h

#include "cocos2d.h"
#include "ShardeEffect.h"

static int tuple_sort( const std::tuple<ssize_t,ShardeEffect*,cocos2d::QuadCommand> &tuple1, const std::tuple<ssize_t,ShardeEffect*,cocos2d::QuadCommand> &tuple2 )
{
    return std::get<0>(tuple1) < std::get<0>(tuple2);
}

class ShardeSprite : public cocos2d::Sprite
{
public:
    static ShardeSprite *create(const std::string& filename);
    void initEffect();
    void setEffect(ShardeEffect* effect);
    void addEffect(ShardeEffect *effect, ssize_t order);
    void changeEffect(int index);
    
    void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
protected:
    ShardeSprite();
    ~ShardeSprite();
    
    ShardeEffect* _defaultEffect;
//public:
    std::vector<std::tuple<ssize_t,ShardeEffect*,cocos2d::QuadCommand>> _effects;

    ssize_t _vectorIndex;
    cocos2d::Vector<ShardeEffect*> _spriteEffects;

};

#endif /* ShardeSprite_h */
