//
//  Colossus.cpp
//  son
//
//  Created by ff on 15-5-25.
//
//

#include "Colossus.h"
#include "../dataModule/BlackColossus.h"
#include "GameDefine.h"

USING_NS_CC;

bool Colossus::init()
{
//    initFightData(100002);
//    initViewRes();
//    initFightState();
    
    if (!Node::init()) {
        return false;
    }
    return true;
}

void Colossus::setupData(int idx)
{
    initFightData(idx);
    initViewRes();
    initFightState();
    
    setDir(NONE);

}

void Colossus::initFightData(int idx)
{
    blackCol = new BlackColossus();
    blackCol->readColossus(idx, CLS_SKILL_CONFIG_FILE);
    blackActor = new BlackActor();
    blackActor->readFile(idx, FIGHTER_CONFIG_FILE);
    blackActor->initClsSelSkill(blackCol);
    blackActor->initSelSkillTag();
    
    currSkill = nullptr;
    currSkillTag = nullptr;
    
    flipX = false;
    
    walkSpeedX = blackActor->getMoveSpeed();
    rushSpeedX = blackActor->getRushSpeed();
    
    atkStiff = hurtStiff = false;
    
    localBonePoint = Point::ZERO;
    
    bHero = true;
}

void Colossus::exitState(int currState)
{
    switch (currState) {
        case IDLE:
        {
            
        }
            break;
            
        case CHASE:
        {
            stopActionByTag(MOVE_ACTION_TAG);
        }
            break;
        case SKILL:
        {
            
        }
            break;
        default:
            break;
    }
}

void Colossus::enterState(int nextState)
{
    switch (nextState) {
        case IDLE:
        {
            
        }
            break;
            
        case CHASE:
        {
            setTargetToActor(man);
            if (man->getPositionX()>=getPositionX()) {
                targetMovePos.x = man->getPositionX() - 110;
            }else{
                targetMovePos.x = man->getPositionX() + 110;
            }
            
            Move2Target(targetMovePos);
        }
            break;
        case SKILL:
        {
            posX = man->getPosX();
            posY = man->getPosY();
            if (man->getPositionX()>=getPositionX()) {
                posX = man->getPositionX() - 110;
            }else{
                posX = man->getPositionX() + 110;
            }
            setPosition(posX, posY);
//            setDir(man->currDir);
            setDir(man->getFlipX()?LEFT:RIGHT);
        }
            break;
        default:
            break;
    }
    
}

bool Colossus::lostHero()
{
    if (man == nullptr) {
        return false;
    }
    if (typeid(*man) != typeid(FightActor)) {
        return false;
    }
    
    return dynamic_cast<FightActor*>(man)->getDieLie();
}

bool Colossus::farFromHero()
{
    if (man == nullptr) {
        return false;
    }
    
    if (abs(getPosition().getDistance(man->getPosition())) > 130) {
        return true;
    }
    
    return false;
}

void Colossus::updateAI()
{
    posX = getPositionX();
    posY = getPositionY();
    auto boneMoveVec2 = calBoneMove();
    
    updateFrontEffect();
    updateBackEffect();
    
    if (currDir != DIRECT::NONE) {
        if (((currDir & DIRECT::LEFT)!=0) != flipX)
        {
            flipX = !flipX;
            armature->setScaleX((flipX) ? -1 : 1);
        }
    }
    
    switch (state) {
        case IDLE:
        {
            
        }
            break;
            
        case CHASE:
        {
            
        }
            break;
        case SKILL:
        {
            
        }
            break;
        default:
            break;
    }
    
    setActorShowPos(boneMoveVec2);
}

void Colossus::updateState()
{
    bool needChange = false;
    switch (state) {
        case IDLE:
        {
            if (event != stateMachine->action_undefined) {
                needChange = true;
                break;
            }
            //            if (armature->getAnimation()->isComplete()) {
            if (lostHero()) {
                event = stateMachine->findEventVal("EVENT_LOSTHERO");
                needChange = true;
            }else if (farFromHero()) {
                event = stateMachine->findEventVal("EVENT_LOSTTARGET");
                needChange = true;
            }
            //            }
            
        }
            break;
        case CHASE:
        {
            if (event != stateMachine->action_undefined) {
                needChange = true;
                break;
            }
            if (arrivePos(targetMovePos)) {
                event = stateMachine->findEventVal("EVENT_ARRIVE_TARGET");
                needChange = true;
            }
        }
            break;
        case SKILL:
        {
            if (armature->getAnimation()->isComplete()) {
                if (currSkill->getLinkID() != stateMachine->action_undefined) {
                    event = stateMachine->findEventVal("EVENT_SKILLA");
                }else{
                    event = stateMachine->findEventVal("EVENT_ACTIONOVER");
                }
                
                needChange = true;
            }
        }
            break;
    }
    
    if(event == stateMachine->action_undefined){
        needChange = false;
        return;
    }
    
    if (needChange) {
        switch2NextState(state, event);
        
        needChange = false;
        event = StateMachine::action_undefined;
    }
}

void Colossus::selColSkills(SkillTag *manCurrSkillTag)
{
    if (state == SKILL) {
        return;
    }
    
    auto vec = manCurrSkillTag->coloSkill;
    if (vec.empty()) {
        return;
    }
    
    int threshold = blackCol->clsSkills[vec[0]].getRate();
    float r = abs(rand()) % 100;
    if (r > threshold) {
        return;
    }
    
    
    event = stateMachine->findEventVal("EVENT_SKILLA");
    
    int s = vec.size();
    for (int i = 0; i < s; i++) {
        int colSkillId = vec[i];
        r = abs(rand()) % 100;
        threshold = blackCol->clsSkills[colSkillId].getRate();
        if (r <= threshold) {
            blackActor->equipSkills[SKILLA_KEY] = blackCol->clsSkills[colSkillId].getSkillId();
            return;
        }
    }
    
    blackActor->equipSkills[SKILLA_KEY] = blackCol->clsSkills[vec[0]].getSkillId();
}
