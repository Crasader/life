//
//  Effect.cpp
//  ssoul
//
//  Created by ff on 14-10-10.
//
//

#include "Effect.h"
#include "utils/GameUtils.h"
#include "EventDefine.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;

bool Effect::init()
{
    if (!Node::init()) {
        return false;
    }
    scaleRate = 1;
    return true;
}
void Effect::onEnter()
{
    Node::onEnter();
    
    pauseListener = EventListenerCustom::create(PAUSE_FIGHT, CC_CALLBACK_0(Effect::pauseFight, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(pauseListener, -1);
    resumeListener = EventListenerCustom::create(RESUME_FIGHT, CC_CALLBACK_0(Effect::resumeFight, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(resumeListener, -1);
    
}
void Effect::onExit()
{
    Node::onExit();
    Director::getInstance()->getEventDispatcher()->removeEventListener(pauseListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(resumeListener);

}
bool Effect::initWithRes(std::string boneName, float scale)
{
//    std::string resPath = GameUtils::format(ACTOR_DIR,
//                                            boneName.c_str(),
//                                            boneName.c_str());
    std::string resPath = GameUtils::format(ACTOR_DIR,
                                            boneName.c_str());
    //初始化和资源载入
    ArmatureDataManager::getInstance()->addArmatureFileInfo(resPath);
    armature = Armature::create(boneName);
    scaleRate = scale;
    armature->setScale(scaleRate);
    armature->getAnimation()->playWithIndex(0);
    addChild(armature);
    
    return true;
}
bool Effect::initWithArmature(std::string boneName, float scale)
{
//    std::string resPath = GameUtils::format(ACTOR_DIR,
//                                            boneName.c_str(),
//                                            boneName.c_str());
    std::string resPath = GameUtils::format(ACTOR_DIR,
                                            boneName.c_str());
    //初始化和资源载入
    ArmatureDataManager::getInstance()->addArmatureFileInfo(resPath);
    armature = Armature::create(boneName);
    armature->setScale(scale/scaleRate);
    scaleRate = scale;
    armature->getAnimation()->playWithIndex(0);
    addChild(armature);
    
    return true;
}

bool Effect::initWithArmatureAR(std::string boneName, float scale)
{
//    std::string resPath = GameUtils::format(ACTOR_DIR,
//                                            boneName.c_str(),
//                                            boneName.c_str());
    std::string resPath = GameUtils::format(ACTOR_DIR,
                                            boneName.c_str());
    //初始化和资源载入
    ArmatureDataManager::getInstance()->addArmatureFileInfo(resPath);
    armature = Armature::create(boneName);
    armature->setScale(scale/scaleRate);
    scaleRate = scale;
    armature->getAnimation()->playWithIndex(0);
    armature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(Effect::animationEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    addChild(armature);
    
    return true;
 
}

bool Effect::initWithNumber(std::string numImgName, int num, int width, int height)
{
    hurtText = TextAtlas::create("0123456789", numImgName, width, height, "0");
    hurtText->setString(Convert2String(num));
    addChild(hurtText);
    return true;
}

bool Effect::initWithImg(std::string imgName)
{
    flagSpt = Sprite::create(imgName);
    addChild(flagSpt);
    return true;
}

void Effect::animationEvent(Armature *armature, MovementEventType movementType, const std::string& movementID)
{
    if (movementType == MovementEventType::COMPLETE)
    {
//        armature->stopAllActions();
        removeAllChildren();
        removeFromParent();
    }
}

void Effect::pauseFight()
{
    pause();
    if (armature) {
        armature->getAnimation()->pause();
    }
}

void Effect::resumeFight()
{
    resume();
    if (armature) {
        armature->getAnimation()->resume();
    }
}
