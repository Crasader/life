//
//  Monster.cpp
//  soul
//
//  Created by ff on 14-9-15.
//
//

#include "Monster.h"
#include "cocostudio/CocoStudio.h"
#include "EventDefine.h"
#include "ELProtocol.h"
#include "logic/ClientLogic.h"
#include "utils/GameUtils.h"
#include "YNCamera.h"
#include "Effect.h"
#include "../layer/ActorLayer.h"
#include "Guardian.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace cocostudio;
using namespace CocosDenshion;

bool Monster::init()
{
    if (!Node::init()) {
        return false;
    }
    return true;
}

void Monster::onEnter()
{
    FightActor::onEnter();
    
    pauseListener = EventListenerCustom::create(PAUSE_FIGHT, CC_CALLBACK_0(Monster::pauseFight, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(pauseListener, -1);
    resumeListener = EventListenerCustom::create(RESUME_FIGHT, CC_CALLBACK_0(Monster::resumeFight, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(resumeListener, -1);
}

void Monster::setupData(int idx)
{
    initFightData(idx);

    initViewRes();
    

    initFightState();
    
    buffManager = BuffManager::create();
    addChild(buffManager);
    
    ghostList.clear();
    if (blackActor->getBarCount() == 1) {
        auto info = armature->getBone("HeadHurtPoint")->getWorldInfo();
        hpBarBg = Sprite::create("ui/battle/monster_hp.png");
        hpBarBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        hpBarBg->setPosition(getArmature()->getPosition()+Vec2(0,info->y+50));
        addChild(hpBarBg,11);
        hpBar = ui::LoadingBar::create("ui/battle/monster_hp2.png");
        hpBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        hpBar->setPosition(hpBarBg->getContentSize()/2);
        hpBar->setPercent((float)blackActor->getHp()/(float)blackActor->getMaxHp()*100);
        hpBarBg->addChild(hpBar);
    }else if(blackActor->getBarCount() > 1){
        EventCustom event(SHOW_MONSTER_HP);
        SHOW_MONSTER_HP_DATA data;
        data.name = blackActor->getName();
        data.count = blackActor->getBarCount();
        data.maxHp = blackActor->getMaxHp();
        data.currHp = blackActor->getHp();
//        data.currHp = 1001;
        event.setUserData(&data);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        
        EventCustom waringEvent(SHOW_WARNING);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&waringEvent);
    }

    armature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(Monster::plotAnimationEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}
void Monster::initFightData(int idx)
{
    blackActor = new BlackActor();
    if (opponentFlag) {
        getBlackActor()->setIsOpponent(true);
        getBlackActor()->initOppRobot(oppRobot);
//        getBlackActor()->initSkills(oppRobot.holySwordId, oppRobot.petId);
    }
    blackActor->readFile(idx, FIGHTER_CONFIG_FILE);
    blackActor->setHp(10);//测试用
    blackActor->setAp(0);
    blackActor->initSelSkill();
    blackActor->initSelSkillTag();
    blackActor->calSkillDis();
    
    currSkill = nullptr;
    currSkillTag = nullptr;
    killSkill = nullptr;

    killSkillTag = nullptr;

    targetActor = nullptr;
    flipX = false;
    flipX = false;
    deHitDownVal = currHitDownVal = blackActor->getDeHitDownVal();
    atkStiff = hurtStiff = false;
    hurtMoveSpeed = 0;
    slowRate = 1.0;
    backInAir = false;
    
    viewSize = Size(Director::getInstance()->getWinSize().width, 10000);
    
    walkSpeedX = blackActor->getMoveSpeed();
    rushSpeedX = blackActor->getRushSpeed();
    
    killer = nullptr;
    killers.clear();
    bHero = false;
    dying = false;
    dieLie = false;
    
    plotActionIdx = -1;
    plotActionMission = 0;
    plotMoveMission = 0;
    plotAction = "";

}
void Monster::initOppRobot(OPPONENT_ROBOT &opp)
{
    oppRobot.name = opp.name;
    oppRobot.jobId = opp.jobId;
    oppRobot.fighterId = opp.fighterId;
    oppRobot.hp = opp.hp;
    oppRobot.attack = opp.attack;
    oppRobot.defence = opp.defence;
    oppRobot.crit = opp.crit;
    oppRobot.decrit = opp.decrit;
    oppRobot.holySwordId = opp.holySwordId;
    memcpy(oppRobot.petId, opp.petId, sizeof(int)*3);
    
}


void Monster::addPartolPoint(Point p1, Point p2)
{
    partolPosVec.clear();
    partolPosVec.push_back(p1);
    partolPosVec.push_back(p2);
}
void Monster::addHurtListener()
{
//    hurtListener = EventListenerCustom::create(MONSTER_HURT_TAG, CC_CALLBACK_1(FightActor::parseHurtEvent, this));
//    punchListener = EventListenerCustom::create(HERO_RESET_PUNCH, CC_CALLBACK_1(FightActor::resetPunchTag, this));
//    getEventDispatcher()->addEventListenerWithFixedPriority(hurtListener, -1);
//    getEventDispatcher()->addEventListenerWithFixedPriority(punchListener, -1);

    hurtListener = getEventDispatcher()->addCustomEventListener(MONSTER_HURT_TAG, CC_CALLBACK_1(FightActor::parseHurtEvent, this));
    punchListener = getEventDispatcher()->addCustomEventListener(HERO_RESET_PUNCH, CC_CALLBACK_1(FightActor::resetPunchTag, this));
}

void Monster::removeHurtListener()
{
    getEventDispatcher()->removeEventListener(hurtListener);
    getEventDispatcher()->removeEventListener(punchListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(pauseListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(resumeListener);
//    getEventDispatcher()->removeCustomEventListeners(MONSTER_HURT_TAG);
//	getEventDispatcher()->removeCustomEventListeners(HERO_RESET_PUNCH);

}

Monster::~Monster()
{
	removeHurtListener();
    ghostList.clear();
}

void Monster::exitState(int currState)
{
    switch (currState) {
        case ACTORSTATE::IDLE:
            idleTimeOut = true;
            stopActionByTag(IDLE_TIMEOUT_TAG);
            break;
        case ACTORSTATE::PATROL:
            stopActionByTag(MOVE_ACTION_TAG);
            leavePartolPos = getPosition();
            break;
        case ACTORSTATE::RETURN:
            stopActionByTag(MOVE_ACTION_TAG);
            break;
        case ACTORSTATE::CHASE:
            stopActionByTag(MOVE_ACTION_TAG);
            break;
        case ACTORSTATE::FELLDOWN:
            currHitDownVal = deHitDownVal;
            break;
        case ACTORSTATE::WAIT:
            waitTimeOut = true;
            stopActionByTag(WAIT_TIMEOUT_TAG);
            break;
        case ACTORSTATE::ANY:
            frontEffectLayer->removeChildByTag(ATTACK_EFFECT_TAG);
        case ACTORSTATE::XULI:
            frontEffectLayer->removeChildByTag(XULI_EFFECT_TAG);
            break;
        case ACTORSTATE::BOUNDWALL:
        {
            hurtMoveSpeed *= -1;
            backInAir = true;
            asBomb = false;
        }
            break;
        case ACTORSTATE::INAIR:
            backInAir = false;
            break;
        default:
            break;
    }

}
void Monster::pauseFight()
{
    armature->getAnimation()->pause();
    pause();
}
void Monster::resumeFight()
{
    armature->getAnimation()->resume();
    resume();
}
void Monster::enterState(int nextState)
{
    if (currDir != DIRECT::NONE) {
        if (((currDir & DIRECT::LEFT)!=0) != flipX)
        {
            flipX = !flipX;
            float sx = blackActor->getScaleRate();
            armature->setScaleX((flipX) ? -sx : sx);
        }
    }
    switch (nextState) {
        case ACTORSTATE::IDLE:
        {
//            killer = nullptr;
            idleTimeOut = false;
            auto func = CallFunc::create(CC_CALLBACK_0(Actor::idleTimeEnough, this));

            stopActionByTag(IDLE_TIMEOUT_TAG);
            auto s = Sequence::create(DelayTime::create(idleDuration),func, NULL);
            s->setTag(IDLE_TIMEOUT_TAG);
            runAction(s);
            stopMove();
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            armature->changeEffect(0);
        #endif
            
        }
            break;
        case ACTORSTATE::CHASE:
        {
            setTargetToActor(targetActor);
            Move2Target(targetMovePos);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            armature->changeEffect(0);
#endif
        }
            break;
        case ACTORSTATE::BACK:
        {
            calBackTarget();
            Move2Target(targetMovePos);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            armature->changeEffect(0);
#endif
        }
            break;
        case ACTORSTATE::JUMP:
        {
            startMove();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            armature->changeEffect(0);
#endif
        }
            break;
        case ACTORSTATE::RETURN:
        {
            Move2Target(leavePartolPos);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            armature->changeEffect(0);
#endif
        }
            break;
        case ACTORSTATE::PATROL:
        {
            Move2Target(partolPosVec.at(partolIndex));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            armature->changeEffect(0);
#endif
        }
            break;
        case ACTORSTATE::WAIT:
        {
            isValidWait = false;
            waitTimeOut = false;
            auto func = CallFunc::create(CC_CALLBACK_0(Monster::waitTimeEnough, this));
            
            stopActionByTag(WAIT_TIMEOUT_TAG);
            auto s = Sequence::create(DelayTime::create(blackActor->getWaitTime()),func, NULL);
            s->setTag(WAIT_TIMEOUT_TAG);
            runAction(s);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            armature->changeEffect(0);
#endif
        }
            break;
        case ACTORSTATE::ATTACK:
        {
            isValidWait = true;
            stopMove();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            armature->changeEffect(0);
#endif
        }
            break;

        case ACTORSTATE::HURT:
            stopActionByTag(MOVE_ACTION_TAG);
            break;
        case ACTORSTATE::INAIR:
        {
            stopActionByTag(MOVE_ACTION_TAG);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            armature->changeEffect(0);
#endif
        }
            break;
        case ACTORSTATE::XULI:
        {
            std::string resPath = "armature/charge.ExportJson";
            ArmatureDataManager::getInstance()->addArmatureFileInfo(resPath);
            
            auto eff = Armature::create("charge");
//            armature->setScale(0.5f);
            eff->getAnimation()->playWithIndex(0);
            eff->setTag(XULI_EFFECT_TAG);
            frontEffectLayer->addChild(eff);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            armature->changeEffect(0);
#endif
        }
            break;
        case ACTORSTATE::INIT:
        {
//            auto fade = Blink::create(0.5, 3);
//            auto func = CallFunc::create(CC_CALLBACK_0(Monster::hasFadeIn, this));
//            auto seq = Sequence::create(fade, func, NULL);
//            armature->setOpacity(122);
//            armature->runAction(seq);
        }
            break;
        case ACTORSTATE::SNEAK:
        {
            bool trianglePath = false;
//            bool trianglePath = (abs(rand()%100)>50);
            
            auto tp = targetActor->getPosition();
            float randBackDis = abs(rand()%100);
            if (randBackDis<10) {
                randBackDis = 0;
            }

            int disxFlag = (tp.x < getPositionX())?1:-1;
            float startPointX = getPositionX() + disxFlag * randBackDis;
            startPointX = MIN(YNCamera::getInstance()->getMapAreaRight(), startPointX);
            startPointX = MAX(YNCamera::getInstance()->getMapAreaLeft(), startPointX);
            
            
            auto startPoint = Point(startPointX, getPositionY());
            
//            int disyFlag = (tp.y > 0)?-1:1;
            
            float endPointY = getPositionY();
            
            float dis = abs(tp.x-startPointX) + 100;
            float endPointX = startPointX- (disxFlag*dis*(trianglePath?0.5:1)+rand()%20);
            endPointX = MIN(YNCamera::getInstance()->getMapAreaRight(), endPointX);
            endPointX = MAX(YNCamera::getInstance()->getMapAreaLeft(), endPointX);
            
            auto endPoint = Point(endPointX, endPointY);
            
            sneakPath.clear();
            sneakPath.push_back(endPoint);
            if(!trianglePath)
            {
                auto point2 = Point(startPointX, endPointY);
                sneakPath.push_back(point2);
            }
            Move2Target(startPoint);
//            sneakPath.push_back(startPoint);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            armature->changeEffect(0);
#endif
        }
            break;
        case ACTORSTATE::SNEAKRUSH:
        {
            auto tp = targetActor->getPosition();
            float randBackDis = abs(rand()%40);
            int disxFlag = targetActor->getFlipX()?1:-1;
            float targetX = tp.x+disxFlag*(30+randBackDis);
            float targetY = tp.y+rand()%20;
            
            targetX = MIN(YNCamera::getInstance()->getMapAreaRight(), targetX);
            targetX = MAX(YNCamera::getInstance()->getMapAreaLeft(), targetX);
            
            Run2Target(Point(targetX, targetY));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            armature->changeEffect(0);
#endif
        }
            break;
        case ACTORSTATE::BOUNDWALL:
        {
            asBomb = true;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            armature->changeEffect(0);
#endif
        }
            break;
        case ACTORSTATE::FELLDOWN:
        {
            fallSpeedY = startFallSpeedY = 0;
            fallAccX = 0;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            armature->changeEffect(0);
#endif
        }
            break;
        case ACTORSTATE::DIE:
        {
//            auto scene = dynamic_cast<GameScene *>(Director::getInstance()->getRunningScene());
//            auto hero = dynamic_cast<ActorLayer *>(getParent())->getHero();
//            if (hero == nullptr) {
//                break;
//            }
//            
//            auto guard = hero->Guard;
//            if (guard == nullptr) {
//                break;
//            }
//            
//            popItem();
            if (blackActor->getBarCount() > 1) {                               
                EventCustom event(HIDE_MONSTER_HP);
                Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
            }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            armature->changeEffect(0);
#endif
        }
            break;
        default:
            break;
    }
}

void Monster::popItem()
{
    auto ghostPoint = ItemSpirit::create();
    ghostPoint->initPos(posX, posY+135);
    ghostList.push_back(ghostPoint);
    //todo 添加魂点
    dynamic_cast<ActorLayer *>(getParent())->getGhostLayer()->addChild(ghostPoint);
}

void Monster::updateGhostPoint()
{
    if (ghostList.empty()) {
        return;
    }
    
    for (auto it = ghostList.begin(); it != ghostList.end(); ) {
        auto ghost = *(it);
        ghost->updatePos();
        if (ghost->getAct() == RELEASE) {
            if (ghost->getCount() > 25) {
                if (addGhost2Guard(ghost)) {
                    ghostList.erase(it);
                    continue;
                }
            }
        }
        ++it;
    }
}
void Monster::addAllGhost2Guard()
{
    if (ghostList.empty()) {
        return;
    }
    
    for (auto it = ghostList.begin(); it != ghostList.end(); ) {
        auto ghost = *(it);
        ghost->updatePos();
        if (ghost->getAct() == RELEASE) {
//            if (ghost->getCount() > 15) {
                if (addGhost2Guard(ghost)) {
                    ghostList.erase(it);
                    continue;
                }
//            }
        }
        ghostList.erase(it);
        ghost->removeFromParent();
        ++it;
    }
}
bool Monster::addGhost2Guard(ItemSpirit *ghost)
{
    ghost->setAct(ABSORB);
    //todo 魂点飞向守护兽
    auto hero = dynamic_cast<ActorLayer *>(getParent())->getHero();
    if (hero == nullptr) {
        return false;
    }
    
    auto guard = dynamic_cast<Guardian *>(hero->Guard);
    if (guard == nullptr) {
        return false;
    }
    guard->add2GhostList(ghost);
    return true;
}

void Monster::updateState()
{
//    bool needChange = false;
//    std::string luaFile("test/hello4.lua");
//    std::map<std::string, Value> baseData;
//    baseData["state"] = stateMachine->stateName[state];
//    if (event==stateMachine->action_undefined) {
//        baseData["event"] = "action_undefined";
//    }else{
//        baseData["event"] = stateMachine->eventName[event];
//    }
//    baseData["sneak_empty"] = (sneakPath.empty()?1:0);
//    if (delayEvent==stateMachine->action_undefined) {
//        baseData["delayEvent"] = "action_undefined";
//    }else{
//        baseData["delayEvent"] = stateMachine->eventName[delayEvent];
//    }
//    if (action==stateMachine->action_undefined) {
//        baseData["action"] = "action_undefined";
//    }else{
//        baseData["action"] = stateMachine->eventName[action];
//    }
//    std::map<std::string, void*> objParam;
//    objParam["Monster"] = this;
//    
//    baseData["isFaint"] = (isFaint ? 1 : 0);
//    
//    std::string event_back = PublicSendLuaData::getInstance()->callLuaFuncParReturn(luaFile, "switch_state", baseData, objParam);
////    log("lua switch_state: %s", event_back.c_str());
////    log("lua switch_state: %s", PublicSendLuaData::getInstance()->callLuaFuncParReturn(luaFile, "switch_state", baseData));
//    
//    if (event_back == "NONE") {
//        needChange = false;
//    } else {
//        needChange = true;
//        if(event_back == "delayEvent") {
//            event = delayEvent;
//            delayEvent = StateMachine::action_undefined;
//        }else if(event_back == "action") {
//            event = action;
//        }else if(event_back != "event") {
//            event = stateMachine->findEventVal(event_back);
////        }else {
////            event = event;
//        }
//    }
//    
//    switch (state) {
//        case ACTORSTATE::PATROL:
//            if (event == stateMachine->findEventVal("EVENT_ARRIVE_TARGET")) {
//                if(partolForward){
//                    if (partolIndex>=partolPosVec.size()-1) {
//                        partolIndex--;
//                        partolForward = false;
//                    }else{
//                        partolIndex++;
//                    }
//                    
//                }else{
//                    if (partolIndex == 0) {
//                        partolIndex++;
//                        partolForward = true;
//                    }else{
//                        partolIndex--;
//                    }
//                }
//                
//                posX = getPositionX();
//                posY = getPositionY();
//            }
//            break;
//        case ACTORSTATE::SNEAK:
//            if (event == stateMachine->findEventVal("EVENT_ARRIVE_TARGET")) {
//                sneakPath.pop_back();
//            }
//            break;
//        case ACTORSTATE::FELLDOWN:
//            if (event == stateMachine->findEventVal("EVENT_DIE")) {
//                dieLie = true;
//            }
//            break;
//        case ACTORSTATE::BOUNDWALL:
//            if (event == stateMachine->findEventVal("EVENT_KWG")) {
//                asBomb = true;
//            }
//            break;
//        case ACTORSTATE::DIE:
//            if (armature->getAnimation()->isComplete()) {
//                //                armature->setOpacity(122);
//                //                armature->runAction(Blink::create(0.3, 3));
//                dying = true;
//                needChange = false;
//            }
//            break;
//        case ACTORSTATE::WAIT:
////            log("wait Enough: %d" , waitTimeOut);
////            log("event>>>>>==== %s", event_back.c_str());
//            break;
//        default:
//            break;
//    }
    
    bool needChange = false;
    switch (state) {
        case ACTORSTATE::INIT:
            if (armature->getAnimation()->isComplete()) {
                event = stateMachine->findEventVal("EVENT_ACTIONOVER");
                needChange= true;
            }
//            if (event != stateMachine->action_undefined) {
//                needChange= true;
//                break;
//            }
            break;
        case ACTORSTATE::IDLE:
//            if (event == stateMachine->findEventVal("EVENT_IDLETIMEOUT")) {
//                needChange= true;
//            }
            if (blackActor->getHp() <= 0) {
                event = stateMachine->findEventVal("EVENT_WAISTFELLGROUND");
                needChange = true;
                stopActionByTag(MOVE_ACTION_TAG);
                atking = false;
                break;
            }

            if (event != stateMachine->action_undefined) {
                needChange= true;
                break;
            }

            if (canFindOpponent()) {
                int backResult = needBack();
                if (backResult == 0) {
                    if (canAttackTarget()) {
                        if (isValidWait) {
                            event = stateMachine->findEventVal("EVENT_CANWAIT");
                        }else{
                            event = stateMachine->findEventVal("EVENT_CANATTACK");
                        }
                        
                        needChange = true;
                        
                    }else{
                        if (!isFix) {
                            int chaseMethod = selChaseMethod();
                            switch (chaseMethod) {
                                case 0:
                                    event = stateMachine->findEventVal("EVENT_CANCHASE");
                                    break;
                                case 1:
                                    event = stateMachine->findEventVal("EVENT_SAFECHASE");
                                    break;
    //                            case 2:
    //                                event = stateMachine->findEventVal("EVENT_RUSHCHASE");
    //                                break;
                                case 3:
                                    event = stateMachine->findEventVal("EVENT_WAIT");
                                    break;
                                default:
                                    break;
                            }

                            needChange = true;
                        }
                        
                    }
                }else if(backResult == 1){
                    if (!isFix) {
                        event = stateMachine->findEventVal("EVENT_NEEDRETRACE");
                        needChange = true;
                    }
                }else if(backResult == 2){
                    if (!isFix) {
                        event = stateMachine->findEventVal("EVENT_RUSHRETRACE");
                        needChange = true;
                    }
                }
            }
            
//            if (canFindOpponent()) {
//                event = stateMachine->findEventVal("EVENT_FIND_ENEMY");
//                needChange = true;
//            }
            break;
        case ACTORSTATE::PATROL:
            if (blackActor->getHp() <= 0) {
                event = stateMachine->findEventVal("EVENT_WAISTFELLGROUND");
                needChange = true;
                stopActionByTag(MOVE_ACTION_TAG);
                atking = false;
                break;
            }

            if (event != stateMachine->action_undefined) {
                needChange= true;
                break;
            }

            if (canFindOpponent()) {
                event = stateMachine->findEventVal("EVENT_FIND_ENEMY");
                needChange = true;
            }
            if (arrivePos(targetMovePos)) {
                if(partolForward){
                    if (partolIndex>=partolPosVec.size()-1) {
                        partolIndex--;
                        partolForward = false;
                    }else{
                        partolIndex++;
                    }
                    
                }else{
                    if (partolIndex == 0) {
                        partolIndex++;
                        partolForward = true;
                    }else{
                        partolIndex--;
                    }
                }
                
                posX = getPositionX();
                posY = getPositionY();
                event = stateMachine->findEventVal("EVENT_ARRIVE_TARGET");
                needChange = true;
            }
            break;
        case ACTORSTATE::CHASE:
            if (blackActor->getHp() <= 0) {
                event = stateMachine->findEventVal("EVENT_WAISTFELLGROUND");
                needChange = true;
                stopActionByTag(MOVE_ACTION_TAG);
                atking = false;
                break;
            }

            if (event != stateMachine->action_undefined) {
                needChange= true;
                break;
            }

            if (canAttackTarget()) {
                if (isValidWait) {
                    event = stateMachine->findEventVal("EVENT_CANWAIT");
                }else{
                    event = stateMachine->findEventVal("EVENT_CANATTACK");
                }
                
                needChange = true;
                break;
            }

            if (arrivePos(targetMovePos)) {
                event = stateMachine->findEventVal("EVENT_ARRIVE_TARGET");
                needChange = true;
            }
            break;
        case ACTORSTATE::BACK:
            if (blackActor->getHp() <= 0) {
                event = stateMachine->findEventVal("EVENT_WAISTFELLGROUND");
                needChange = true;
                stopActionByTag(MOVE_ACTION_TAG);
                atking = false;
                break;
            }

            if (event != stateMachine->action_undefined) {
                needChange= true;
                break;
            }

            if (arrivePos(targetMovePos)) {
                event = stateMachine->findEventVal("EVENT_ARRIVE_TARGET");
                needChange = true;
            }
            
            break;
        case ACTORSTATE::RUSHBACK:
        case ACTORSTATE::RUSH:
            if (blackActor->getHp() <= 0) {
                event = stateMachine->findEventVal("EVENT_WAISTFELLGROUND");
                needChange = true;
                stopActionByTag(MOVE_ACTION_TAG);
                atking = false;
                break;
            }

            if (event != stateMachine->action_undefined) {
                needChange= true;
                break;
            }
            if (armature->getAnimation()->isComplete()) {
                event = stateMachine->findEventVal("EVENT_ACTIONOVER");
                needChange= true;
            }
            break;
            
        case ACTORSTATE::RETURN:
            if (blackActor->getHp() <= 0) {
                event = stateMachine->findEventVal("EVENT_WAISTFELLGROUND");
                needChange = true;
                stopActionByTag(MOVE_ACTION_TAG);
                atking = false;
                break;
            }

            if (event != stateMachine->action_undefined) {
                needChange= true;
                break;
            }

            if (arrivePos(targetMovePos)) {
                event = stateMachine->findEventVal("EVENT_ARRIVE_TARGET");
                needChange = true;
            }
            if (canFindOpponent()) {
                event = stateMachine->findEventVal("EVENT_FIND_ENEMY");
                needChange = true;
            }
            
            break;
        case ACTORSTATE::SNEAK:
            if (blackActor->getHp() <= 0) {
                event = stateMachine->findEventVal("EVENT_WAISTFELLGROUND");
                needChange = true;
                stopActionByTag(MOVE_ACTION_TAG);
                atking = false;
                break;
            }

            if (event != stateMachine->action_undefined) {
                needChange= true;
                break;
            }
            if (arrivePos(targetMovePos)) {
                if (sneakPath.empty()) {
                    event = stateMachine->findEventVal("EVENT_ENDSNEAK");
                }else{
                    Move2Target(sneakPath[sneakPath.size()-1]);
                    sneakPath.pop_back();
                    event = stateMachine->findEventVal("EVENT_ARRIVE_TARGET");
                }
                
                needChange = true;
            }
            break;
        case ACTORSTATE::SNEAKRUSH:
            if (blackActor->getHp() <= 0) {
                event = stateMachine->findEventVal("EVENT_WAISTFELLGROUND");
                needChange = true;
                stopActionByTag(MOVE_ACTION_TAG);
                atking = false;
                break;
            }

            if (event != stateMachine->action_undefined) {
                needChange= true;
                break;
            }
            if (arrivePos(targetMovePos)) {
                event = stateMachine->findEventVal("EVENT_ARRIVE_TARGET");
                needChange = true;
            }
            break;
        case ACTORSTATE::ATTACK:
            if (blackActor->getHp() <= 0) {
                event = stateMachine->findEventVal("EVENT_WAISTFELLGROUND");
                needChange = true;
                atking = false;
                break;
            }

            if (event != stateMachine->action_undefined) {
                needChange= true;
                break;
            }
            if (getIsValidAtk()) {
                //                    if ((frameCommand & HEROFRAMECOMMAND::COMMAND_PUNCHA2) != 0) {
                //                        event = stateMachine->findEventVal("EVENT_ATTACK");
                //                    }
                if ((frameCommand & HEROFRAMECOMMAND::COMMAND_PUNCHA2) != 0) {
                    delayEvent = stateMachine->findEventVal("EVENT_ATTACK2");
                }
                if ((frameCommand & HEROFRAMECOMMAND::COMMAND_PUNCHA3) != 0) {
                    delayEvent = stateMachine->findEventVal("EVENT_ATTACK3");
                }
                if ((frameCommand & HEROFRAMECOMMAND::COMMAND_PUNCHA4) != 0) {
                    delayEvent = stateMachine->findEventVal("EVENT_ATTACK4");
                }
            }
            if (armature->getAnimation()->isComplete()) {
                int backResult = needBack();
                if (backResult == 0) {
                    if (delayEvent == StateMachine::action_undefined) {
                        event = stateMachine->findEventVal("EVENT_ACTIONOVER");
                    }else{
                        event = delayEvent;
                        delayEvent = StateMachine::action_undefined;
                    }
                }else if(backResult == 1){
                    event = stateMachine->findEventVal("EVENT_NEEDRETRACE");
                }else if(backResult == 2){
                    event = stateMachine->findEventVal("EVENT_RUSHRETRACE");
                }

                needChange = true;
            }
            break;
        case ACTORSTATE::WAIT:
            if (blackActor->getHp() <= 0) {
                event = stateMachine->findEventVal("EVENT_WAISTFELLGROUND");
                needChange = true;
                stopActionByTag(MOVE_ACTION_TAG);
                atking = false;
                break;
            }

            if (event != stateMachine->action_undefined) {
                needChange= true;
                break;
            }

            if (waitNotSafe()) {
                if(stateMachine->eventMap.find("EVENT_NEEDAVOID") != stateMachine->eventMap.end()){
                    setDir((abs(rand())%100>50) ? LEFT:RIGHT);
                    event = stateMachine->findEventVal("EVENT_NEEDAVOID");
                    needChange = true;
                }
                
            }
            if (waitTimeOut) {
//                if (lostTarget()) {
//                    event = stateMachine->findEventVal("EVENT_LOSTTARGET");
//                    needChange = true;
//                }else{
//                    if (canAttackTarget()) {
//                        event = stateMachine->findEventVal("EVENT_CANATTACK");
//                        needChange = true;
//                    }else if (canChase()){
//                        if (needSneak()) {
//                            event = stateMachine->findEventVal("EVENT_NEEDSNEAK");
//                            needChange = true;
//                        }else{
//                            event = stateMachine->findEventVal("EVENT_CANCHASE");
//                            needChange = true;
//                        }
//                    }
//                }
                event = stateMachine->findEventVal("EVENT_IDLETIMEOUT");
                needChange = true;
            }
            break;
        case ACTORSTATE::JUMP:
        {
            if (blackActor->getHp() <= 0) {
                event = stateMachine->findEventVal("EVENT_WAISTFELLGROUND");
                needChange = true;
                stopActionByTag(MOVE_ACTION_TAG);
                atking = false;
                break;
            }

            if (event == stateMachine->findEventVal("EVENT_LANDGROUND")) {
                needChange = true;
            }
            if (armature->getAnimation()->isComplete())
            {
                if (event == StateMachine::action_undefined) {
                    switch (action) {
                        case HEROACTION::JUMP_UP:
                        {
                            event = stateMachine->findEventVal("EVENT_JUMP_TOP");
                        }
                            break;
                        case HEROACTION::JUMP_STICK:
                        {
                            event = stateMachine->findEventVal("EVENT_JUMP_DOWN");
                        }
                            break;
                        case HEROACTION::JUMP_DOWN:
                        {
                            event = stateMachine->findEventVal("EVENT_LANDGROUND");
                        }
                            break;
                        default:
                            break;
                    }
                }
                needChange = true;
            }
        }
            break;

        case ACTORSTATE::FAINT:
            if (blackActor->getHp() <= 0) {
                event = stateMachine->findEventVal("EVENT_WAISTFELLGROUND");
                needChange = true;
                stopActionByTag(MOVE_ACTION_TAG);
                atking = false;
                break;
            }

            if (!isFaint) {
                
                event = stateMachine->findEventVal("EVENT_CANATTACK");
                needChange = true;
            }
            break;
        case ACTORSTATE::HURT:
            if (event == stateMachine->findEventVal("EVENT_HEADHURT")) {
                if (blackActor->equipSkills[SKILLC_KEY] != 0) {
                    if(!blackActor->selSkills[blackActor->equipSkills[SKILLC_KEY]]->getCooling())
                    {
                        if (canUseSkill(blackActor->equipSkills[SKILLC_KEY]) == SKILL_NONE_ERROR) {
                            event = stateMachine->findEventVal("EVENT_SKILLC");
                            delayEvent = StateMachine::action_undefined;
                        }
                    }
                    
                }
                needChange = true;
                break;
            }
            if (event == stateMachine->findEventVal("EVENT_WAISTHURT")) {
                needChange = true;
                break;
            }
            if (event == stateMachine->findEventVal("EVENT_HIT2AIR")) {
                needChange = true;
                break;
            }
            
            if (armature->getAnimation()->isComplete()) {
                if (delayEvent == stateMachine->findEventVal("EVENT_HEADFELLGROUND")) {
                    event = delayEvent;
                    delayEvent = StateMachine::action_undefined;
                    needChange = true;
                    break;
                }
                if (delayEvent == stateMachine->findEventVal("EVENT_WAISTFELLGROUND")) {
                    event = delayEvent;
                    delayEvent = StateMachine::action_undefined;
                    needChange = true;
                    break;
                }
                if (isFaint) {
                    event = stateMachine->findEventVal("EVENT_FAINT");
                    needChange = true;
                    break;
                }
                if (action == HEROACTION::HEADHIT) {
                    event = stateMachine->findEventVal("HEADHIT");
                }
                if (action == HEROACTION::WAISTHIT) {
                    event = stateMachine->findEventVal("WAISTHIT");
                }
                
                needChange = true;
//                if (blackActor->getHp() <= 0) {
//                    dying = true;
//                    needChange = false;
//                }
            }

            break;
        case ACTORSTATE::FELLDOWN:
//            if (event == stateMachine->findEventVal("EVENT_KG")) {
//                needChange = true;
//                break;
//            }
            if (armature->getAnimation()->isComplete()) {
                if (action == HEROACTION::HEADFELLDOWN) {
                    event = stateMachine->findEventVal("HEADFELLDOWN");
                    if (blackActor->getHp() <= 0) {
                        event = stateMachine->findEventVal("EVENT_DIE");
                        dieLie = true;
                    }
                }
                if (action == HEROACTION::WAISTFELLDOWN) {
                    event = stateMachine->findEventVal("WAISTFELLDOWN");
                    if (blackActor->getHp() <= 0) {
                        event = stateMachine->findEventVal("EVENT_DIE");
                        dieLie = true;
                    }
                }
                if (action == HEROACTION::GETUP) {
                    if (isFaint) {
                        event = stateMachine->findEventVal("EVENT_FAINT");
                        needChange = true;
                        break;
                    }else {
                        event = stateMachine->findEventVal("GETUP");
                    }
                    
                }
                if (action == HEROACTION::LIE) {
                    event = stateMachine->findEventVal("LIE");
                }
                if (action == HEROACTION::LIESHOCK) {
                    event = stateMachine->findEventVal("LIESHOCK");
                    if (blackActor->getHp() <= 0) {
                        event = stateMachine->findEventVal("EVENT_DIE");
                        dieLie = true;
                    }
                }
                needChange = true;
            }
            break;
            
        case ACTORSTATE::DIE:
            if (armature->getAnimation()->isComplete()) {
//                armature->setOpacity(122);
//                armature->runAction(Blink::create(0.3, 3));
                dying = true;
                needChange = false;
                E2L_MONSTER_DIE info;
                info.eProtocol = e2l_monster_die;
                info.teamId = teamId;
                info.monId = getBlackActor()->getActorID();
                ClientLogic::instance()->ProcessUIRequest(&info);
            }
            break;
            
        case ACTORSTATE::INAIR:
            if (blackActor->getHp() <= 0) {
                event = stateMachine->findEventVal("EVENT_WAISTFELLGROUND");
                needChange = true;
                stopActionByTag(MOVE_ACTION_TAG);
                atking = false;
                break;
            }

            if (event == stateMachine->findEventVal("EVENT_HIT2AIR") ||
                event == stateMachine->findEventVal("EVENT_HEADHURT") ||
                event == stateMachine->findEventVal("EVENT_WAISTHURT") ||
                event == stateMachine->findEventVal("EVENT_HEADFELLGROUND") ||
                event == stateMachine->findEventVal("EVENT_WAISTFELLGROUND") ||
                event == stateMachine->findEventVal("EVENT_FAINT")) {
                
                event = stateMachine->findEventVal("EVENT_AIRHIT");
                if (blackActor->equipSkills[SKILLC_KEY] != 0) {
                    if(!blackActor->selSkills[blackActor->equipSkills[SKILLC_KEY]]->getCooling()) {
                        if (canUseSkill(blackActor->equipSkills[SKILLC_KEY]) == SKILL_NONE_ERROR) {
                            event = stateMachine->findEventVal("EVENT_SKILLC");
                        }
                    }
                }
                needChange = true;
                break;
            }
            if (isLandGround() && action == HEROACTION::AIRHITDOWN) {
                event = stateMachine->findEventVal("EVENT_LANDGROUND");
                needChange = true;
            }
            if (armature->getAnimation()->isComplete()) {
                if (action == HEROACTION::AIRHITDOWN) {
                    break;
                }
                event = action;
                needChange = true;
            }
            break;
        case ACTORSTATE::BOUNDGROUND:
            if (armature->getAnimation()->isComplete()) {
                event = stateMachine->findEventVal("EVENT_KGE");
                needChange = true;
            }
            break;
        case ACTORSTATE::BOUNDWALL:
            if (action == HEROACTION::BOUND2W) {
                if (YNCamera::getInstance()->isOutWindow(getPosition())) {
                    event = stateMachine->findEventVal("EVENT_KWG");
                    needChange = true;
                    asBomb = false;
                } else if (armature->getAnimation()->isComplete()) {
                    event = stateMachine->findEventVal("EVENT_ACTIONOVER");
                    needChange = true;
                }
            } else {
                if (armature->getAnimation()->isComplete()) {
                    event = stateMachine->findEventVal("EVENT_KWE");
                    needChange = true;
                }
            }
            break;
        default:
            break;
    }
    if(event == stateMachine->action_undefined){
        needChange = false;
        return;
    }
    
    std::string eventStr = stateMachine->eventName[event];
    if (eventStr == "EVENT_HEADHURT" || eventStr == "EVENT_WAISTHURT" || eventStr == "EVENT_HEADFELLGROUND" || eventStr == "EVENT_WAISTFELLGROUND" || eventStr == "EVENT_HIT2AIR" || eventStr == "EVENT_AIRHIT" || eventStr == "EVENT_KW" || eventStr == "EVENT_KG") {
        state = ACTORSTATE::ANY;
        needChange = true;
    }
//    if (event == stateMachine->findEventVal("EVENT_HEADHURT")|| event == stateMachine->findEventVal("EVENT_WAISTHURT") || event == stateMachine->findEventVal("EVENT_HEADFELLGROUND") || event == stateMachine->findEventVal("EVENT_WAISTFELLGROUND") || event == stateMachine->findEventVal("EVENT_HIT2AIR") || event == stateMachine->findEventVal("EVENT_AIRHIT") || event == stateMachine->findEventVal("EVENT_KW") || event == stateMachine->findEventVal("EVENT_KG")) {
//        state = ACTORSTATE::ANY;
//        needChange = true;
//    }
    
    if (needChange) {
        log("entry state:%d", state);
        log("%s", eventStr.c_str());
        switch2NextState(state, event);
        log("state:%d", state);
        log("action:%d", action);
        needChange = false;
        event = StateMachine::action_undefined;
    }
}

void Monster::updateAI()
{
    posX = getPositionX();
    posY = getPositionY();
    auto boneMoveVec2 = calBoneMove();

    updateFrontEffect();
    updateBackEffect();
    
    //状态内执行
//    switch (state) {
//        case ACTORSTATE::WALK:
    if (currDir != DIRECT::NONE) {
        if (((currDir & DIRECT::LEFT)!=0) != flipX)
        {
            flipX = !flipX;
            float sx = blackActor->getScaleRate();
            armature->setScaleX((flipX) ? -sx : sx);
        }
    }
    switch (state) {
        case ACTORSTATE::IDLE:
        {
//            currHitDownVal++;
//            currHitDownVal = MIN(deHitDownVal, currHitDownVal);
            if (targetActor == nullptr) {
                break;
            }
            if (event != stateMachine->action_undefined) {
                break;
            }
            if(targetActor->getPositionX() > getPositionX())
            {
                setDir(DIRECT::RIGHT);
            }else{
                setDir(DIRECT::LEFT);
            }
        }
            break;
        case ACTORSTATE::MOVE:
        case ACTORSTATE::PATROL:
        case ACTORSTATE::RETURN:
        case ACTORSTATE::CHASE:
        case ACTORSTATE::BACK:
        
        {

            return;
        }
            break;

        case ACTORSTATE::JUMP:
        {
            float armY = armature->getPositionY()+boneMoveVec2.y;
            if (armY<0) {
                event = stateMachine->findEventVal("EVENT_LANDGROUND");
            }
            if(currDir == DIRECT::NONE)
            {
                break;
            }


            //移动
            posX = getPositionX() + speedX*slowRate;
//            posY = getPositionY() + speedY;
        }
            break;

        case ACTORSTATE::SNEAK:
        case ACTORSTATE::SNEAKRUSH:
        {
            flipX = (getPositionX()>targetActor->getPositionX());
            float sx = blackActor->getScaleRate();
            armature->setScaleX((flipX) ? -sx : sx);
        }
            break;
        case ACTORSTATE::WAIT:
//        case ACTORSTATE::ATTACK:
            if (targetActor == nullptr) {
                break;
            }
            if (event != stateMachine->action_undefined) {
                break;
            }
            if(targetActor->getPositionX() > getPositionX())
            {
                setDir(DIRECT::RIGHT);
            }else{
                setDir(DIRECT::LEFT);
            }
            break;
        case ACTORSTATE::HURT:
        {
            if (chatterLoop != 0) {
                armature->getAnimation()->pause();
                posX = getPositionX() + chatterOffX[chatterIndex]*((flipX) ? -1 : 1);
                chatterIndex++;
                if (chatterIndex >= 3) {
                    chatterLoop--;
                    chatterIndex = 1;
                    if (chatterLoop == 0) {
                        if (killSkill != nullptr) {
                            startHurtStiff(killSkillTag->getHitStiffDur());
                        }else{
                            startHurtStiff(0.1);
                        }
//                        startHurtStiff(killSkill->getHitStiffDur());
                    }
                }
            }else{
                if (hurtStiff) {
                    if (!isFix) {
                        posX = getPositionX() + hurtMoveSpeed;
                    }
                }else{
                    posX = getPositionX();
                }
            }
            
            
//            posX = MIN(1900, posX);
//            posX = MAX(20, posX);
//            setPositionX(posX);
        }
            break;
        case ACTORSTATE::FELLDOWN:
            if (action == HEROACTION::LIESHOCK) {
                posX = getPositionX() + lieShockSpeed;
            }
            break;
        case ACTORSTATE::BOUNDGROUND:
            if (killSkill != nullptr ) {
                boneMoveVec2.y *= killSkillTag->getBoundUpRate();
            }
            break;
        case ACTORSTATE::INAIR:
        {
            if (chatterLoop != 0) {
                armature->getAnimation()->pause();
                posX = getPositionX() + chatterOffX[chatterIndex]*((flipX) ? -1 : 1);
                chatterIndex++;
                if (chatterIndex >= 3) {
                    chatterLoop--;
                    chatterIndex = 1;
                    if (chatterLoop == 0) {
                        if (killSkill != nullptr) {
                            startHurtStiff(killSkillTag->getAirHitStiffDur());
                        }else{
                            startHurtStiff(0.1);
                        }
//                        startHurtStiff(killSkillTag->getAirHitStiffDur());
                    }
                }
            }else{
                if (action == HEROACTION::AIRHITDOWN) {
                    fallSpeedY += fallAccY;
                    fallAccY *= 1.15;
                    fallSpeedY = MIN(30, fallSpeedY);
                    boneMoveVec2.y = -fallSpeedY*slowRate;
                }
                
                if (killSkill != nullptr) {
                    if (action == HEROACTION::AIRHIT)
                    {
                        boneMoveVec2.y = killSkillTag->getAirAtkUpVal();
                    }else if (action == HEROACTION::AIRHITUP) {
                        if(killSkillTag->getFloatRate() != 99){
                            boneMoveVec2.y *= killSkillTag->getFloatRate();
                        }
                    }
                    float hitSpeed = killSkillTag->getAirHitBackSpeed();
                    if (killSkillTag->gethitFlyRate() != 99) {
                        hitSpeed *= killSkillTag->gethitFlyRate();
                    }
                    
                    if (killSkillTag->getHitDownVal() > getDeHitDownVal()) {
                        if (hurtMoveSpeed == 0) {
                            hurtMoveSpeed = hitSpeed * ((currDir==DIRECT::RIGHT) ? -1 : 1) * (backInAir?-1:1);
                        }
                        hurtMoveSpeed += fallAccX* ((currDir==DIRECT::RIGHT) ? -1 : 1) * (backInAir?-1:1);
                        fallAccX *= 1.25;
                        if (hurtMoveSpeed > 0) {
                            hurtMoveSpeed = MIN(10, hurtMoveSpeed);
                        }else{
                            hurtMoveSpeed = MAX(-10, hurtMoveSpeed);
                        }
                    }else{
                        hurtMoveSpeed = hitSpeed * ((currDir==DIRECT::RIGHT) ? -1 : 1) * (backInAir?-1:1);
                    }
                    if (!isFix) {
                        posX = getPositionX() + hurtMoveSpeed*slowRate;
                    }
                    
                    
                }else{
                    posX = getPositionX() - 0.5 *((flipX) ? -1 : 1) * (backInAir?-1:1);
                }
//                if (hurtStiff) {
//                    posX = getPositionX() + hurtMoveSpeed;
//                }else{
//                    posX = getPositionX();
//                }
            }
            
                
//            }
        }
            break;
            
//        case ACTORSTATE::DIE:
//        {
//            bool visual = armature->isVisible();
//            armature->setVisible(!visual);
//        }
//            break;
        default:
            break;
    }
    
    if (getIsInvincible()){
//        bool visual = armature->isVisible();
//        armature->setVisible(!visual);
        armature->setOpacity(128);
    }
    
    if (killSkillTag != nullptr) {
        if (killSkillTag->gethitFlyRate() != 99) {
            boneMoveVec2.x *= killSkillTag->gethitFlyRate();
        }
    }
    setActorShowPos(boneMoveVec2);
    if (blackActor->getBarCount() == 1) {
        auto info = armature->getBone("HeadHurtPoint")->getWorldInfo();
        hpBarBg->setPositionY(getArmature()->getPositionY()+info->y+50);
        hpBar->setPercent((float)blackActor->getHp()/(float)blackActor->getMaxHp()*100);
    }
    
    updateGhostPoint();
}

void Monster::doFlag(int nextFlag)
{
    switch (nextFlag) {
        case StateAIFlag::invincible:
            addSingleBUff(true, 34, 100);
            break;
        case StateAIFlag::startFall:
        {
//            armature->setPositionY(localBonePoint.y);
//            localBonePoint.y = 0;
            //            float originOffy = armature->getBone("OriginPoint")->getWorldInfo()->y;
            //            fallStartY += armature->getBone(localBodyName)->getWorldInfo()->y - originOffy;
            //            armature->setPositionY(fallStartY);
        }
            break;
        case StateAIFlag::endFall:
            //            fallStartY = 0;
            armature->setPositionY(0);
            break;
        case StateAIFlag::selSkill:
        {
            log("-------flag selSkill-----");
            int randSel = selValidSkill();
            log("-------end selSkill-----");
            int nextAction = -1;
            switch (randSel) {
                case ATK_KEY:
                {
                    nextAction = HEROACTION::ATTACKPART1;
                }
                    break;
                case SKILLA_KEY:
                {
                    nextAction = stateMachine->eventMap["EVENT_SKILLA"];
                }
                    break;
                case SKILLB_KEY:
                {
                    nextAction = stateMachine->eventMap["EVENT_SKILLB"];
                }
                    break;
                case SKILLC_KEY:
                {
                    nextAction = stateMachine->eventMap["EVENT_SKILLC"];
                }
                    break;
                case SKILLD_KEY:
                {
                    nextAction = stateMachine->eventMap["EVENT_SKILLD"];
                }
                    break;
                case HOLY_SKILL_KEY:
                {
                    nextAction = stateMachine->eventMap["EVENT_HOLY_SWORD"];
                }
                    break;
                case PET_SKILL1_KEY:
                case PET_SKILL2_KEY:
                case PET_SKILL3_KEY:
                {
                    nextAction = stateMachine->eventMap["EVENT_PET"];
                }
                    break;
                case SKILLS_KEY:
                {
                    nextAction = stateMachine->eventMap["SKILLATK4"];
                }
                    break;
                default:
                    return;
            }
            stateMachine->setNextTactics(ACTORSTATE::ATTACK);
            stateMachine->setNextAction(nextAction);
        }
            break;
        default:
            break;
    }

}


void Monster::Hurt(int hurtVal)
{

}

//void Monster::changeSkill(int event)
//{
//    currSkill = nullptr;
//    currSkillTag = nullptr;
//    if(event == stateMachine->findEventVal("ATTACKPART1")){
//        blackActor->equipSkills[ATK_KEY] = blackActor->getAtk1SkillID();
//        currSkill = blackActor->selSkills[blackActor->equipSkills[ATK_KEY]];
////        currSkill = blackActor->selSkills[blackActor->getAtk1SkillID()];
//        currSkillTag = blackActor->selSkillTags[currSkill->getSkillID()*100+1];
//    }
//    if(event == stateMachine->findEventVal("ATTACKPART2")){
//        blackActor->equipSkills[ATK_KEY] = blackActor->getAtk2SkillID();
//        currSkill = blackActor->selSkills[blackActor->equipSkills[ATK_KEY]];
////        currSkill = blackActor->selSkills[blackActor->getAtk2SkillID()];
//        currSkillTag = blackActor->selSkillTags[currSkill->getSkillID()*100+1];
//    }
//    if(event == stateMachine->findEventVal("ATTACKPART3")){
//        blackActor->equipSkills[ATK_KEY] = blackActor->getAtk3SkillID();
//        currSkill = blackActor->selSkills[blackActor->equipSkills[ATK_KEY]];
////        currSkill = blackActor->selSkills[blackActor->getAtk3SkillID()];
//        currSkillTag = blackActor->selSkillTags[currSkill->getSkillID()*100+1];
//    }
//    if(event == stateMachine->findEventVal("ATTACKPART4")){
//        blackActor->equipSkills[ATK_KEY] = blackActor->getAtk4SkillID();
//        currSkill = blackActor->selSkills[blackActor->equipSkills[ATK_KEY]];
////        currSkill = blackActor->selSkills[blackActor->getAtk4SkillID()];
//        currSkillTag = blackActor->selSkillTags[currSkill->getSkillID()*100+1];
//    }
//    
//    if (currSkill != nullptr) {
////        addBuff(true);
//        SimpleAudioEngine::getInstance()->setEffectsVolume(0.8);
//        SimpleAudioEngine::getInstance()->playEffect(currSkillTag->getSkillSoundRes().c_str(),false,1,0,0.5);
//    }
//}




void Monster::dispatchResetPunchTag()
{
    EventCustom event(MONSTER_RESET_PUNCH);
    event.setUserData(this);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

//void Monster::waitTimeEnough()
//{
//    waitTimeOut = true;
//    event = stateMachine->findEventVal("EVENT_WAITTIMEOUT");
//}

void Monster::hasFadeIn()
{
    event = stateMachine->findEventVal("EVENT_FADEIN");
    armature->setOpacity(255);
}
//bool Monster::canFindOpponent()
//{
////    return false;
//    
//    auto enemiesInView = dynamic_cast<ActorLayer *>(getParent())->findEnemies(this);
//    if (enemiesInView.empty()) {
//        targetActor = nullptr;
//        return false;
//    }
//    
//    auto opponent = selectOpponent(enemiesInView);
//    if (opponent == nullptr) {
//        targetActor = nullptr;
//        return false;
//    }
//    
//    targetActor = opponent;
//    return true;
//}

//std::vector<FightActor *> Monster::findEnemies(Vector<FightActor *> actors)
//{
//    std::vector<FightActor *> result;
//    auto size = actors.size();
//    
//    auto viewRect = Rect(getPositionX()-viewSize.width/2, MAP_AREA_BOTTOM, viewSize.width, viewSize.height);
//    for (size_t i=0; i<size; i++) {
//        FightActor *actor = actors.at(i);
//        if (actor->getDying() || actor->getDieLie()) {
//            continue;
//        }
//        if (viewRect.containsPoint(actor->getPosition())) {
//            result.push_back(actor);
//        }
//    }
//    return result;
//}


//FightActor* Monster::selectOpponent(std::vector<FightActor *> enemies)
//{
//    FightActor *result = nullptr;
//    auto size = enemies.size();
//    int minDis = viewSize.width;
//    for (size_t i=0; i<size; i++) {
//        FightActor *actor = enemies.at(i);
//        if (actor->getDying() || actor->getDieLie()) {
//            continue;
//        }
//        if (abs(actor->getPosition().getDistance(getPosition())) < minDis) {
//            minDis = abs(actor->getPosition().getDistance(getPosition()));
//            result = actor;
//        }
//    }
//    
//    return result;
//}

//void Monster::setTargetToActor(FightActor *actor)
//{
//    auto originPos = actor->getPosition();
//
//    
//    float maxAtkDis = 0;
//    float minAtkDis = 0;
//    if (actor->getPositionX() >= getPositionX()) {
//        auto atkRect = getAtkRect(false);
//        maxAtkDis = atkRect.getMaxX();
//        minAtkDis = MAX(0, atkRect.getMinX());
//    }else{
//        auto atkRect = getAtkRect(true);
//        maxAtkDis = atkRect.getMinX();
//        minAtkDis = MIN(0,atkRect.getMaxX());
//    }
//    float tx = originPos.x + rand() % abs((int)(maxAtkDis - minAtkDis)) + minAtkDis;
//    float ty = originPos.y + rand() % 20;
//    
//    tx = MIN(MAP_AREA_RIGHT, tx);
//    tx = MAX(MAP_AREA_LEFT, tx);
//    ty = MIN(ty, MAP_AREA_TOP);
//    ty = MAX(ty, MAP_AREA_BOTTOM);
//    targetMovePos = Point(tx, ty);
//}

//bool Monster::lostTarget()
//{
////    return true;
//    if (targetActor == nullptr) {
//        return true;
//    }
//    if(targetActor->getDying() || targetActor->getDieLie())
//    {
//        targetActor = nullptr;
//        return true;
//    }
//    auto viewRect = Rect(getPositionX()-viewSize.width/2, MAP_AREA_BOTTOM, viewSize.width, viewSize.height);
//    if (!viewRect.containsPoint(targetActor->getPosition())) {
//        targetActor = nullptr;
//        return true;
//    }
//    return false;
//}

//bool Monster::canAttackTarget()
//{
////    return false;
//    if (targetActor->getDying() || targetActor->getDieLie()) {
//        targetActor = nullptr;
//        return false;
//    }
//    Rect atkRect;
//    if (targetActor->getPositionX() >= getPositionX()) {
//        atkRect = getAtkRect(false);
//    }else{
//        atkRect = getAtkRect(true);
//    }
//
//    atkRect.origin += getPosition();
//    atkRect.origin.y -= 40;
//    atkRect.size.height = 80;
//    if (atkRect.containsPoint(targetActor->getPosition())) {
//        return true;
//    }
//    return false;
//}

//bool Monster::canChase(){
////    return false;
//    if (targetActor->getDying() || targetActor->getDieLie()) {
//        targetActor = nullptr;
//        return false;
//    }
//    auto viewRect = Rect(getPositionX()-viewSize.width*2/3, MAP_AREA_BOTTOM, viewSize.width*4/3, viewSize.height);
//    return viewRect.containsPoint(targetActor->getPosition());
//}
//
//bool Monster::needSneak()
//{
////    return false;
//    if (targetActor == nullptr) {
//        return false;
//    }
//
//    if(targetActor->getAtking())
//    {
//        if (targetActor->getAtkRect(targetActor->getFlipX()).size.width > targetActor->getPosition().getDistance(getPosition())) {
//            return true;
//        }
//    }
//    return false;
//}

void Monster::setMp(float val)
{
    float mp = MAX(0, val);
    mp = MIN(blackActor->getMaxMp(), mp);
    blackActor->setMp(mp);
}

void Monster::addAv(float val)
{
    float av = blackActor->getAV() + val;
    if (av >= blackActor->getMaxAV()) {
        if (blackActor->getAp() >= blackActor->getMaxAp()) {
            blackActor->setAV(blackActor->getMaxAV()-1);
        }else{
            blackActor->setAV(0);
            blackActor->setAp(blackActor->getAp()+1);
            if (opponentFlag) {
                EventCustom event(UPDATE_OPPONENT_AP);
                AP_UPDATE data;
                data.curr = blackActor->getAp();
                data.max = blackActor->getMaxAp();
                event.setUserData(&data);
                Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
            }
        }
    }else{
        blackActor->setAV(av);
    }
}

void Monster::useAp(float val)
{
    if (val == 0) {
        return;
    }
    float ap = blackActor->getAp() - val;
    ap = MAX(0, ap);
    blackActor->setAp(ap);
    if (opponentFlag) {
        EventCustom event(UPDATE_OPPONENT_AP);
        AP_UPDATE data;
        data.curr = blackActor->getAp();
        data.max = blackActor->getMaxAp();
        event.setUserData(&data);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    }
}

void Monster::setHp(float hp)
{
    if (blackActor->getHp() > 0 && hp <= 0) {
        E2L_MONSTER_HP_EMPTY info;
        info.eProtocol = e2l_monster_hp_empty;
        info.teamId = teamId;
        info.monId = getBlackActor()->getActorID();
        ClientLogic::instance()->ProcessUIRequest(&info);
    }

    blackActor->setHp(hp);
    if (opponentFlag) {
        EventCustom event(UPDATE_OPPONENT_HP);
        HP_UPDATE data;
        data.curr = blackActor->getHp();
        data.max = blackActor->getMaxHp();
        event.setUserData(&data);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    }else if (blackActor->getBarCount() > 1) {
        //todo update hp
        EventCustom event(UPDATE_MONSTER_HP);
        UPDATE_MONSTER_HP_DATA data;
        data.currHp = blackActor->getHp();
        data.maxHp = blackActor->getMaxHp();
        event.setUserData(&data);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    }

//    GameScene::updateHp(hp, blackActor->getMaxHp());
}
void Monster::setMaxHp(float val)
{
    blackActor->setMaxHp(val);
}

void Monster::setGhost(float ghost)
{
    float val = MAX(0, ghost);
    val = MIN(blackActor->getMaxGhost(), val);
    blackActor->setGhost(val);
}

void Monster::plotChangeAction(int action, bool flipX, int missionId)
{
    changeState(IDLE);
    setDir(DIRECT::NONE);
    //    changeActionWithName(0);
//    plotAction = armature->getAnimation()->getAnimationData()->movementNames[action];
    plotActionIdx = action;
    plotActionMission = missionId;
    armature->setScaleX(flipX?-1:1);
    armature->getAnimation()->stop();
    armature->getAnimation()->playWithIndex(action);
}

void Monster::plotAnimationEvent(cocostudio::Armature *armature,
                              cocostudio::MovementEventType movementType,
                              const std::string& movementID)
{
//    if (plotAction == movementID) {
    if (movementType == MovementEventType::COMPLETE)
    {
        if (plotActionIdx != -1) {
            armature->getAnimation()->playWithIndex(0);
            
            E2L_COMPLETE_PLOT infoPlot;
            infoPlot.eProtocol = e2l_complete_plot;
            infoPlot.missionId = plotActionMission;
            infoPlot.value = 0;
            ClientLogic::instance()->ProcessUIRequest(&infoPlot);
            
            plotActionIdx = -1;
            plotActionMission = 0;
        }
    }else if (movementType == MovementEventType::LOOP_COMPLETE) {
        if (plotActionIdx != -1) {
            E2L_COMPLETE_PLOT infoPlot;
            infoPlot.eProtocol = e2l_complete_plot;
            infoPlot.missionId = plotActionMission;
            infoPlot.value = 0;
            ClientLogic::instance()->ProcessUIRequest(&infoPlot);
            
            plotActionIdx = -1;
            plotActionMission = 0;
        }
    }
//    }
}

void Monster::plotMove(float duration, float targetX, int missionId)
{
    plotMoveMission = missionId;
    auto move = MoveTo::create(duration, Vec2(targetX, GroundY));
    auto func = CallFunc::create(CC_CALLBACK_0(Monster::plotMoveEnd, this));
    auto seq = Sequence::create(move, func,  NULL);
    runAction(seq);
}

void Monster::plotMoveEnd()
{
    setPosX(getPositionX());
    setPosY(GroundY);
    E2L_COMPLETE_PLOT infoPlot;
    infoPlot.eProtocol = e2l_complete_plot;
    infoPlot.missionId = plotMoveMission;
    infoPlot.value = 0;
    ClientLogic::instance()->ProcessUIRequest(&infoPlot);
    plotMoveMission = 0;
}

