//
//  Guardian.cpp
//  son
//
//  Created by ff on 15-5-18.
//
//

#include "Guardian.h"
//#include "view/scene/GameScene.h"

USING_NS_CC;

bool Guardian::init()
{
    if (!Node::init()) {
        return false;
    }
    return true;
}

void Guardian::setupData(int idx)
{
    initFightData(idx);
    initViewRes();
    initFightState();
    
    setDir(NONE);

}

void Guardian::initFightData(int idx)
{
    blackActor = new BlackActor();
    blackActor->readFile(idx, FIGHTER_CONFIG_FILE);
    blackActor->initSelSkill();
    blackActor->initSelSkillTag();
    blackActor->setGhost(0);
    float maxGhost = blackActor->selSkills.begin()->second->getConsumeGhost();
    blackActor->setMaxGhost(maxGhost);
    currSkill = nullptr;
    currSkillTag = nullptr;
    
    flipX = false;
    
    walkSpeedX = blackActor->getMoveSpeed();
    rushSpeedX = blackActor->getRushSpeed();
    
    atkStiff = hurtStiff = false;
    
    localBonePoint = Point::ZERO;
    
    bHero = true;
}

void Guardian::exitState(int currState)
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

void Guardian::enterState(int nextState)
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
            setDir(man->getFlipX()?LEFT:RIGHT);
        }
            break;
        default:
            break;
    }

}

void Guardian::updateAI()
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
            float sx = blackActor->getScaleRate();
            armature->setScaleX((flipX) ? -sx : sx);
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
    
    updateGhostPoint();
}

void Guardian::updateState()
{
    bool needChange = false;
    switch (state) {
        case IDLE:
        {
            if (pressSkill()) {
                event = stateMachine->findEventVal("EVENT_SKILLA");
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
            if (pressSkill()) {
                event = stateMachine->findEventVal("EVENT_SKILLA");
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
                event = stateMachine->findEventVal("EVENT_ACTIONOVER");
                needChange = true;
            }
        }
            break;
    }
    
    if(event == stateMachine->action_undefined){
        currKey = 0;
        needChange = false;
        return;
    }
    
    currKey = 0;
    if (needChange) {
        switch2NextState(state, event);
        
        needChange = false;
        event = StateMachine::action_undefined;
    }
}

void Guardian::doFlag(int nextFlag)
{
    
}

bool Guardian::lostHero()
{
    if (man == nullptr) {
        return false;
    }
    if (typeid(*man) != typeid(FightActor)) {
        return false;
    }
    
    return dynamic_cast<FightActor*>(man)->getDieLie();
}

bool Guardian::farFromHero()
{
    if (man == nullptr) {
        return false;
    }
    
    if (abs(getPosition().getDistance(man->getPosition())) > 130) {
        return true;
    }
    
    return false;
}

bool Guardian::pressSkill()
{
    if (currKey == GAMEKEY::BUTTON_Y) {
        if (man == nullptr) {
            return false;
        }
    
        if (getBlackActor()->getGhost() < getBlackActor()->selSkills[getBlackActor()->equipSkills[SKILLA_KEY]]->getConsumeGhost()) {
            return false;
        }
        return true;
    }
    
    return false;
}

void Guardian::setGhost(float val)
{
    val = MAX(0, val);
    val = MIN(getBlackActor()->selSkills[getBlackActor()->equipSkills[SKILLA_KEY]]->getConsumeGhost(), val);
    getBlackActor()->setGhost(val);
    //todo 界面更新
//    GameScene::updateGhost(val, getBlackActor()->selSkills[getBlackActor()->equipSkills[SKILLA_KEY]]->getConsumeGhost());
}

void Guardian::updateGhostPoint()
{
    for (auto it = ghostList.begin(); it != ghostList.end(); ) {
        auto ghost = *it;
        ghost->updatePos();
        if (ghost->getAct() == ABSORB) {
            ghost->setOriginPos(getPositionX(), getPositionY()+80);
            if (ghost->hit()) {
                if (man != nullptr) {
                    setGhost(getBlackActor()->getGhost() + 1);
                }
                ghostList.erase(it);
                ghost->removeFromParent();
                continue;
            }
        }
        ++it;
    }
}

void Guardian::add2GhostList(ItemSpirit *ghost)
{
    ghostList.push_back(ghost);
}

