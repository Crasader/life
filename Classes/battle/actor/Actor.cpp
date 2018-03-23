//
//  Actor.cpp
//  cocosGame
//
//  Created by ff on 14-11-5.
//
//

#include "Actor.h"
#include "YNCamera.h"

USING_NS_CC;

int Actor::totalId = 0;

void Actor::setDir(int dir)
{
//    if (state == ACTORSTATE::ATTACK )
//    {
//        //攻击状态下，不移动
//        return;
//    }
    currDir = dir;

}

Rect Actor::getBodyRect()
{
    return Rect(-35, 0, 70, 200);
}

void Actor::logic(float dt)
{
    updateAI();
    if (plotPause) {
        return;
    }
    updateState();
}

void Actor::switch2NextState(int state, int event)
{
    stateMachine->setCurrTactics(state);
    stateMachine->setCurrEvent(event);
    stateMachine->nextState();
    doFlag(stateMachine->getNextFlag());
    if(stateMachine->getNextAction() == StateMachine::action_undefined)
    {
        return;
    }
    if(stateMachine->getNextTactics() != this->state)
    {
        changeState(stateMachine->getNextTactics());
    }
    
//    changeAction(stateMachine->getNextAction());
    log("-------change action>>>>>>>>");
    changeActionWithName(stateMachine->getNextAction());
    log("<<<<<<<<end action--------");
}

void Actor::changeState(int nextState)
{
    exitState(state);
    enterState(nextState);
    state = nextState;
}

void Actor::stopMove()
{
    speedX = speedY = 0;
    currDir = DIRECT::NONE;
}

void Actor::startMove()
{
    if (state == JUMP)
    {
        speedX = walkSpeedX;
    }else{
        speedX = rush?rushSpeedX:walkSpeedX;
    }
    speedX *= slowRate;
    //各方向叠加速度
    if ((currDir & LEFT) != 0)
    {
        speedX *= -1;
    }
    
    //清除不需要方向的速度
    speedY = 0;
}

Point Actor::calBoneMove()
{
    auto originInfo = armature->getBone("OriginPoint")->getWorldInfo();
    auto boneInfo = armature->getBone("localPoint")->getWorldInfo();
    
    auto boneVec2 = Point(boneInfo->x, boneInfo->y)-Point(originInfo->x, originInfo->y);
    auto boneMoveVec2 = boneVec2 - localBonePoint;
    localBonePoint = Point(boneInfo->x, boneInfo->y)-Point(originInfo->x, originInfo->y);
    
    return boneMoveVec2;
}

void Actor::setActorShowPos(Point boneMoveVec2)
{
    posX += boneMoveVec2.x*((flipX) ? -1 : 1)*slowRate;
    posX = MIN(YNCamera::getInstance()->getMapAreaRight(), posX);
    posX = MAX(YNCamera::getInstance()->getMapAreaLeft(), posX);
    setPositionX(posX);
    float armY = armature->getPositionY()+boneMoveVec2.y*slowRate;
    armY = MAX(armY, 0);
    armature->setPositionY(armY);
}

void Actor::Move2Target(Point p)
{
    targetMovePos = p;
    if(targetMovePos.x>getPositionX())
    {
        setDir(DIRECT::RIGHT);
    }else{
        setDir(DIRECT::LEFT);
    }
    
    stopActionByTag(MOVE_ACTION_TAG);
    auto dis = getPosition().getDistance(targetMovePos);
    float dur = dis/(walkSpeedX*slowRate*60);
    
    auto mt = MoveTo::create(dur, targetMovePos);
    mt->setTag(MOVE_ACTION_TAG);
    runAction(mt);
}

void Actor::Run2Target(Point p)
{
    targetMovePos = p;
    if(targetMovePos.x>getPositionX())
    {
        setDir(DIRECT::RIGHT);
    }else{
        setDir(DIRECT::LEFT);
    }
    
    stopActionByTag(MOVE_ACTION_TAG);
    auto dis = getPosition().getDistance(targetMovePos);
    float dur = dis/(rushSpeedX*slowRate*60);
    
    auto mt = MoveTo::create(dur, targetMovePos);
    mt->setTag(MOVE_ACTION_TAG);
    runAction(mt);
}

bool Actor::arrivePos(Vec2 p)
{
    if (p.distance(getPosition()) < 4) {
        setPosX(p.x);
        setPositionX(p.x);
        return true;
    }
    return false;
//    return getPosition() == p;
}

bool Actor::arriveTargetPos()
{
    return arrivePos(targetMovePos);
}
bool Actor::animOver()
{
    return armature->getAnimation()->isComplete();
}
bool Actor::isOutWindow()
{
    return YNCamera::getInstance()->isOutWindow(getPosition());
}
void Actor::idleTimeEnough()
{
    idleTimeOut = true;
//    event = stateMachine->findEventVal("EVENT_IDLETIMEOUT");
}

void Actor::readyRush()
{
    setRushReady(true);
    auto func1 = CallFunc::create(CC_CALLBACK_0(Actor::setRushReady, this, false));
    auto rushAction = Sequence::create(DelayTime::create(RUSH_LINK_TIME),func1, NULL);
    rushAction->setTag(READY_RUSH_TAG);
    runAction(rushAction);
}

void Actor::readyVictory()
{
    slowRate = 0.5;
    armature->getAnimation()->setSpeedScale(slowRate);
}
void Actor::stageVictory()
{
    slowRate = 1.0;
    armature->getAnimation()->setSpeedScale(slowRate);
}

void Actor::setSlowRate(int val)
{
    slowRate = val;
    armature->getAnimation()->setSpeedScale(slowRate);
}

void Actor::increaceId()
{
    Actor::totalId++;
    createId = totalId;
}
