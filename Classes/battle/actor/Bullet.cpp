//
//  Bullet.cpp
//  cocosGame
//
//  Created by ff on 14-12-15.
//
//

#include "Bullet.h"
#include "FightActor.h"
#include "utils/GameUtils.h"
#include "YNCamera.h"
#include "EventDefine.h"
#include "GameReader.h"

USING_NS_CC;
using namespace cocostudio;

bool Bullet::init()
{
    flipX = false;
    speed = 0;
    distY = GroundY;
    isAlive = false;
    armature = nullptr;
    currSkillTag = nullptr;
    plotPause = false;
    selSkillTags.clear();
    return true;
}

void Bullet::setupData(cocos2d::EventCustom *event)
{
    CREATE_BULLET_DATA data = *static_cast<CREATE_BULLET_DATA*>(event->getUserData());
//    std::string resPath = GameUtils::format(ACTOR_DIR,
//                                            data.res.c_str(),
//                                            data.res.c_str());
    std::string resPath = GameUtils::format(ACTOR_DIR,
                                            data.res.c_str());
    ArmatureDataManager::getInstance()->addArmatureFileInfo(resPath);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    armature = ShardeArmature::create(data.res);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    armature = Armature::create(data.res);
#endif
    scaleRate = data.scale;
    armature->setScale(scaleRate);
    addChild(armature);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    armature->initEffect();
#endif
    

    
    armature->getAnimation()->setFrameEventCallFunc(CC_CALLBACK_0(Bullet::onFrameEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

    setPosition(data.srcX, data.srcY);
    speed = data.speed;
    flipX = data.flipX;
    if (flipX) {
        armature->setScaleX(-scaleRate);
    }
    width= data.bulletWidth;
    height = data.bulletHeight;
    bulletType = data.bulletType;
    through = data.through;
    skillId = data.bulletSkillId;
    fillSkillTags(skillId);
    calAngel = data.calAngel;
    distPosX = data.distPosX;
    ownerType = data.ownerType;
    baseAttack = data.baseAttack;
    baseCritRatio = data.baseCritRatio;
    spurFly = false;
    slowRate = 1.0;
    comboFactor = data.comboFactor;
    if(selSkillTags.find(skillId*100+1) != selSkillTags.end()){
        spurFly = true;
    }
    if (bulletType == 3) {
        
        if (data.srcX == -10000) {
            float centerX = YNCamera::getInstance()->getPositionX();
            centerX = MAX(centerX, GameUtils::winSize.width/2);
            centerX = MIN(centerX, YNCamera::getInstance()->getMapAreaRight() - GameUtils::winSize.width/2);
            setPositionX(centerX + data.distPosX);
        }else{
            setPositionX(data.srcX + data.distPosX);
        }
        distY = data.srcY;
        setPositionY(GameUtils::winSize.height+100);
    }
    if (bulletType == 2) {
        changeState(BOOM);
    }else{
        changeState(FLY);
    }
    

    isAlive = true;
    increaceId();
}

void Bullet::fillSkillTags(int skillId)
{
    auto doc = GameReader::getDocInstance(SKILL_TAG_CONFIG_FILE);
    if(SkillTag::idTable.empty())
    {
        SkillTag::initIdTable(*doc);
    }
    
    for(auto iter = SkillTag::idTable.begin(); iter !=SkillTag::idTable.end();)
    {
        iter = std::find_if(iter, SkillTag::idTable.end(), FindSkillTag(skillId));
        if (iter != SkillTag::idTable.end()) {
            auto skillTag = new SkillTag;
            skillTag->readFile((*iter).first, SKILL_TAG_CONFIG_FILE);
            selSkillTags[(*iter).first] = skillTag;
            iter++;
        }
    }

}

bool Bullet::changeState(BULLET_STATE nextState)
{
    switch (nextState) {
        case FLY:
        {
            armature->getAnimation()->play("fly");
            if (spurFly) {
                currSkillTag = selSkillTags[skillId*100+1];
            }else{
                currSkillTag = nullptr;
            }
        }
            break;
        case BOOM:
        {
            auto movement = armature->getAnimation()->getAnimationData()->getMovement("boom");
            if (movement != nullptr) {
                armature->getAnimation()->play("boom");
            }else{
                return false;
            }
            
        }
            break;
        case LAND:
        {
            auto movement = armature->getAnimation()->getAnimationData()->getMovement("land");
            if (movement != nullptr) {
                armature->getAnimation()->play("land");
            }else{
                return false;
            }
        }
            break;

        default:
            break;
    }
    
    state = nextState;
    return true;
}

void Bullet::Logic(float dt)
{
    switch (state) {
        case FLY:
        {
            switch (bulletType) {
                case 1:
                {
                    setPosition(getPosition()+Vec2(speed*slowRate, 0));
                    if (distPosX>0) {
                        if (speed > 0 && getPositionX()>=distPosX) {
                            if(!changeState(BOOM)) {
                                isAlive = false;
                            }
                        }else if (speed < 0 && getPositionX()<=distPosX){
                            if(!changeState(BOOM)) {
                                isAlive = false;
                            }
                        }
                    }else{
                        isAlive = !isOutsideMap();
                    }
                }
                    break;
                case 3:
                {
                    setPosition(getPosition()+Vec2(0, -speed*slowRate));
                    if (distY >= getPositionY()) {
                        setPositionY(distY);
                        if(!changeState(LAND)) {
                            isAlive = false;
                        }
                    }
                }
                    break;
                    
                default:
                    break;
            }
        }
            break;
        case BOOM:
        {
            switch (bulletType) {
                case 2:
                {
                    setPosition(getPosition()+Vec2(speed*slowRate, 0));
                    isAlive = !isOutsideMap();
                }
                    break;
            }
            if (isAnimationOver()) {
                isAlive = false;
            }
        }
            break;
        case LAND:
        {
            if (isAnimationOver()) {
                isAlive = false;
            }
        }
            break;
            
        default:
            break;
    }
    
}

Rect Bullet::getAtkRect()
{
    return Rect(-width/2, 0, width, height);
}
bool Bullet::isCollide(cocos2d::Rect bRect)
{
    if (state != BULLET_STATE::FLY) {
        if (!atking) {
            return false;
        }
        
        if (currSkillTag == nullptr) {
            return false;
        }
        auto bulletPos = getPosition()+Vec2(0, armature->getPositionY());
        auto aRect = currSkillTag->getAttackRect();
        if (flipX) {
            aRect.origin.x += aRect.size.width;
            aRect.origin.x *= -1;
        }
        aRect.origin += bulletPos;
        if (aRect.intersectsRect(bRect)) {
//            isAlive = false;
            return true;
        }
        return false;
    }
    if (!spurFly) {
        return false;
    }
    auto bulletPos = getPosition()+Vec2(0, armature->getPositionY());
    auto aRect = Rect(-width/2, 0, width, height);
    aRect.origin += bulletPos;
    switch (bulletType) {
        case 1:
            if (aRect.intersectsRect(bRect)) {
                if (!through) {
                    if(!changeState(BOOM)) {
                        isAlive = false;
                    }
                }
                return true;
            }
            break;
            
        default:
            break;
    }
    
    return false;
}

bool Bullet::isOutsideMap()
{
    if (getPositionX()< YNCamera::getInstance()->getMapAreaLeft() || getPositionX() > YNCamera::getInstance()->getMapAreaRight()) {
        return true;
    }
    
    return false;
}
bool Bullet::isOutsideCamera(float cameraPosX)
{
    if (getPositionX()<-50 || getPositionX()>YNCamera::getInstance()->getMapAreaWidth() +50) {
        return true;
    }
    
//    float cameraPosX = YNCamera::getInstance()->getPositionX();
    if (abs(cameraPosX - getPositionX()) < 530) {
        return false;
    }
    
    if (cameraPosX < 480) {
        if (getPositionX() - cameraPosX > 960-cameraPosX+50 ) {
            return true;
        }
    }else if(cameraPosX > YNCamera::getInstance()->getMapAreaWidth() - 480) {
        if (cameraPosX - getPositionX() > 960 + 50 - (YNCamera::getInstance()->getMapAreaWidth() - cameraPosX)) {
            return true;
        }
    }
    return false;
}

bool Bullet::isAnimationOver()
{
    return armature->getAnimation()->isComplete();
}

void Bullet::onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
{
    
    auto *val = evt.c_str();
    if (strcmp(val, "") == 0) {
        return;
    }
    
    auto result = GameUtils::splitStr(evt, ",");
    for (int i = 0; i<result.size(); i++) {
        if (strcmp(result[i].c_str(), "startAttack") == 0) {
            //            log("start Attack @ %d", action);
            atking = true;
        }
        if (strcmp(result[i].c_str(), "endAttack") == 0) {
            atking = false;
        }
        if (strcmp(result[i].c_str(), "attackHead") == 0) {
            attackHead = true;
            attackWaist = false;
        }
        if (strcmp(result[i].c_str(), "attackWaist") == 0) {
            attackWaist = true;
            attackHead = false;
        }
        if (strcmp(result[i].c_str(), "hit2air") == 0) {
            
            hit2Air = true;
        }
        
        if (strncmp(result[i].c_str(), "attackTag", strlen("attackTag")) == 0) {
            size_t tagLen = strlen("attackTag");
            size_t sLen = result[i].length();
            attackTag = atoi(result[i].substr(tagLen, sLen - tagLen).c_str());
            attackTag += skillId*100;
            currSkillTag = selSkillTags[attackTag];
        }
        
        if (strcmp(result[i].c_str(), "bound2Air") == 0)
        {
            hit2Ground = true;
        }
        
        if (strcmp(result[i].c_str(), "bound2Wall") == 0) {
            hit2Wall = true;
        }
    }
    
}
