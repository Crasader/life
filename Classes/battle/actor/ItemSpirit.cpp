//
//  ItemSpirit.cpp
//  son
//
//  Created by ff on 15-5-28.
//
//

#include "ItemSpirit.h"

USING_NS_CC;
using namespace cocostudio;

bool ItemSpirit::init()
{
    rCount = 0;
    
    speed = 0;
    addAngle = 0;
    
    rAction = nullptr;
    return true;
}

void ItemSpirit::initPos(float x, float y)
{
//    sprite = Sprite::create("armature/ghostpoint0.png");
//    addChild(sprite);
//    armature = Armature::create("ghostpoint");
//    armature->getAnimation()->playWithIndex(0);
//    
//    addChild(armature);
    _emitter = ParticleSystemQuad::create("img/battle/soul.plist");
    _emitter->setTextureWithRect(Director::getInstance()->getTextureCache()->addImage("img/battle/soul.png"), Rect(0,0,32,32));
    addChild(_emitter);
    
    setPosition(Vec2(x, y));
    
    rAction = new RotateAction(abs(rand()%160)+10, 5, x, y -10);
    speed = abs(rand()%8)+5;
    addAngle = abs(rand()%15)/5;
}

void ItemSpirit::updatePos()
{
    rCount++;
    if (act == RELEASE) {
        rAction->addRadius(speed);
        if (speed > 0) {
            speed -= 0.5;
        }
    } else if (act == ABSORB) {
        rAction->addRadius(-speed);
        rAction->addAngle(addAngle);
        if (speed < 8) {
            speed += 0.3;
        }
    }
    
    rAction->resetScreenXY();
    setPosition(rAction->getDrawX(), rAction->getDrawY());
}

void ItemSpirit::setOriginPos(float x, float y)
{
    rAction->resetOriginXY(x, y);
}

bool ItemSpirit::hit()
{
    return (rAction->getRadius() < 10);
}

void ItemSpirit::setAct(int action)
{
    rCount = 0;
    act = action;
}
