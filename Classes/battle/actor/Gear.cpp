//
//  Gear.cpp
//  son
//
//  Created by ff on 15-4-22.
//
//

#include "Gear.h"

USING_NS_CC;

bool Gear::init()
{
    if (!Node::init()) {
        return false;
    }
    return true;
}
void Gear::setupData(int idx)
{
    initFightData(idx);
    initViewRes();
    initFightState();
}
void Gear::initFightData(int idx)
{
    blackActor = new BlackActor();
    blackActor->readFile(idx, FIGHTER_CONFIG_FILE);
    blackActor->initSelSkill();
    blackActor->initSelSkillTag();
    
    currSkill = nullptr;
    currSkillTag = nullptr;
    
    killSkill = nullptr;
    killSkillTag = nullptr;
    
    targetActor = nullptr;
//    flipX = false;
    
    deHitDownVal = currHitDownVal = blackActor->getDeHitDownVal();
    atkStiff = hurtStiff = false;
    hurtMoveSpeed = 0;
    backInAir = false;
    
    viewSize = Size(Director::getInstance()->getWinSize().width, 10000);
    
    killer = nullptr;
    killers.clear();
    bHero = false;
    dying = false;
    dieLie = false;
    waitTimeOut = true;

}

void Gear::exitState(int currState)
{
    switch (currState) {
        case ACTORSTATE::ATTACK:
        {
            waitTimeOut = false;
            auto func = CallFunc::create(CC_CALLBACK_0(FightActor::waitTimeEnough, this));
            
            stopActionByTag(WAIT_TIMEOUT_TAG);
            auto s = Sequence::create(DelayTime::create(blackActor->getWaitTime()),func, NULL);
            s->setTag(WAIT_TIMEOUT_TAG);
            runAction(s);
            
        }
            break;
            
        default:
            break;
    }
}

void Gear::enterState(int nextState)
{
    
}

void Gear::updateState()
{
    bool needChange = false;
    std::string eventName("");
    if (event != stateMachine->action_undefined) {
        eventName = stateMachine->eventName[event];
    }

    switch (state) {
        case ACTORSTATE::IDLE:
            if (canFindOpponent()) {
                if (waitTimeOut) {
                    event = stateMachine->findEventVal("EVENT_CANATTACK");
                    needChange = true;
                }
            }
            break;
        case ACTORSTATE::ATTACK:
            if (armature->getAnimation()->isComplete()) {
                event =  stateMachine->findEventVal("EVENT_ACTIONOVER");
                needChange = true;
            }
            break;
            
        default:
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

void Gear::dispatchResetPunchTag()
{
    EventCustom event(MONSTER_RESET_PUNCH);
    event.setUserData(this);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}
