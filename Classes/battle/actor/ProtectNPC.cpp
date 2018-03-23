//
//  ProtectNPC.cpp
//  life
//
//  Created by ff on 17/6/30.
//
//

#include "ProtectNPC.h"
#include "SimpleAudioEngine.h"
#include "EventDefine.h"
#include "ELProtocol.h"
#include "logic/ClientLogic.h"

USING_NS_CC;
using namespace cocostudio;
using namespace CocosDenshion;

ProtectNPC::ProtectNPC()
{
    
}

ProtectNPC::~ProtectNPC()
{
    removeHurtListener();
}

bool ProtectNPC::init()
{
    if (!Node::init()) {
        return false;
    }
    
    return true;
}

void ProtectNPC::onEnter()
{
    FightActor::onEnter();
    
    pauseListener = EventListenerCustom::create(PAUSE_FIGHT, CC_CALLBACK_0(ProtectNPC::pauseFight, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(pauseListener, -1);
    resumeListener = EventListenerCustom::create(RESUME_FIGHT, CC_CALLBACK_0(ProtectNPC::resumeFight, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(resumeListener, -1);
}

void ProtectNPC::setupData(int idx)
{
    initFightData(idx);
    
    initViewRes();
    
    
    initFightState();
    changeState(ACTORSTATE::INIT);
    changeActionWithName(HEROACTION::STAND);
    
    buffManager = BuffManager::create();
    addChild(buffManager);
    
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
    }
}

void ProtectNPC::initFightData(int idx)
{
    blackActor = new BlackActor();
    blackActor->readFile(idx, FIGHTER_CONFIG_FILE);
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
    
    walkSpeedX = blackActor->getMoveSpeed();
    rushSpeedX = blackActor->getRushSpeed();
    
    killer = nullptr;
    killers.clear();
    bHero = false;
    dying = false;
    dieLie = false;
}

void ProtectNPC::addHurtListener()
{
    hurtListener = getEventDispatcher()->addCustomEventListener(WITNESS_HURT_TAG, CC_CALLBACK_1(FightActor::parseHurtEvent, this));
    punchListener = getEventDispatcher()->addCustomEventListener(MONSTER_RESET_PUNCH, CC_CALLBACK_1(FightActor::resetPunchTag, this));
}

void ProtectNPC::dispatchResetPunchTag()
{
    EventCustom event(MONSTER_RESET_PUNCH);
    event.setUserData(this);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void ProtectNPC::removeHurtListener()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(pauseListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(resumeListener);
    getEventDispatcher()->removeEventListener(hurtListener);
    getEventDispatcher()->removeEventListener(punchListener);
}

void ProtectNPC::exitState(int currState)
{
    switch (currState) {
        case ACTORSTATE::IDLE:
        {
            idleTimeOut = true;
            stopActionByTag(IDLE_TIMEOUT_TAG);
        }
            break;
        case ACTORSTATE::ANY:
            frontEffectLayer->removeChildByTag(ATTACK_EFFECT_TAG);
            break;
            
        default:
            break;
    }
}

void ProtectNPC::enterState(int nextState)
{
    switch (nextState)
    {
        case ACTORSTATE::IDLE:
        {
            idleTimeOut = false;
            stopMove();
            stopActionByTag(MOVE_ACTION_TAG);
        }
            break;
        case ACTORSTATE::CHASE:
        {
            Move2Target(targetMovePos);
        }
            break;
        case ACTORSTATE::WAIT:
        {
            stopMove();
            stopActionByTag(MOVE_ACTION_TAG);
        }
            break;
        case ACTORSTATE::BACK:
        {
            E2L_COMMON info;
            info.eProtocol = e2l_witness_arrive;
            ClientLogic::instance()->ProcessUIRequest(&info);
        }
            break;
        case ACTORSTATE::INIT:
        {
            auto fade = Blink::create(0.5, 3);
            auto func = CallFunc::create(CC_CALLBACK_0(ProtectNPC::hasFadeIn, this));
            auto seq = Sequence::create(fade, func, NULL);
            armature->setOpacity(122);
            armature->runAction(seq);
        }
            break;
        case ACTORSTATE::DIE:
        {
            stopMove();
            stopActionByTag(MOVE_ACTION_TAG);
            if (blackActor->getBarCount() > 1) {
                EventCustom event(HIDE_MONSTER_HP);
                Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
            }
            if (blackActor->getHp() <= 0) {
                E2L_COMMON info;
                info.eProtocol = e2l_witness_die;
                ClientLogic::instance()->ProcessUIRequest(&info);
            }
        }
            break;
        case ACTORSTATE::HURT:
        {
            stopMove();
            stopActionByTag(MOVE_ACTION_TAG);
            log("hurt");
        }
            break;
        default:
            break;
    
    }
}


void ProtectNPC::hasFadeIn()
{
    event = stateMachine->findEventVal("EVENT_FADEIN");
    armature->setOpacity(255);
}

void ProtectNPC::setHp(float hp)
{
//    if (blackActor->getHp() > 0 && hp <= 0) {
//        E2L_COMMON info;
//        info.eProtocol = e2l_witness_die;
//        ClientLogic::instance()->ProcessUIRequest(&info);
//    }
    
    blackActor->setHp(hp);
    if (blackActor->getBarCount() > 1) {
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
void ProtectNPC::setMaxHp(float val)
{
    blackActor->setMaxHp(val);
}

void ProtectNPC::doFlag(int nextFlag)
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

        default:
            break;
    }
}

void ProtectNPC::updateAI()
{
    posX = getPositionX();
    posY = getPositionY();
    auto boneMoveVec2 = calBoneMove();
    
    updateFrontEffect();
    updateBackEffect();
    
    setActorShowPos(boneMoveVec2);
    if (blackActor->getBarCount() == 1) {
        auto info = armature->getBone("HeadHurtPoint")->getWorldInfo();
        hpBarBg->setPositionY(getArmature()->getPositionY()+info->y+50);
        hpBar->setPercent((float)blackActor->getHp()/(float)blackActor->getMaxHp()*100);
    }
    adjustHurtEffectPos();
}

void ProtectNPC::updateState()
{
    bool needChange = false;
    switch (state) {
        case ACTORSTATE::INIT:
            if (event != stateMachine->action_undefined) {
                needChange= true;
                break;
            }
            break;
        case ACTORSTATE::CHASE:
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
            if (event != stateMachine->action_undefined) {
                needChange= true;
                break;
            }
            if (armature->getAnimation()->isComplete()) {
                event = stateMachine->findEventVal("EVENT_ACTIONOVER");
                needChange = true;
            }
            break;
        case ACTORSTATE::HURT:
            
            if (event == stateMachine->findEventVal("EVENT_HEADHURT")) {
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
                    if (blackActor->getHp() <= 0) {
                        event = stateMachine->findEventVal("EVENT_DIE");
                        dieLie = true;
                    }
                    needChange = true;
                    break;
                }
                if (delayEvent == stateMachine->findEventVal("EVENT_WAISTFELLGROUND")) {
                    event = delayEvent;
                    delayEvent = StateMachine::action_undefined;
                    if (blackActor->getHp() <= 0) {
                        event = stateMachine->findEventVal("EVENT_DIE");
                        dieLie = true;
                    }
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
                    if (blackActor->getHp() <= 0) {
                        event = stateMachine->findEventVal("EVENT_DIE");
                        dieLie = true;
                    }
                }
                if (action == HEROACTION::WAISTHIT) {
                    event = stateMachine->findEventVal("WAISTHIT");
                    if (blackActor->getHp() <= 0) {
                        event = stateMachine->findEventVal("EVENT_DIE");
                        dieLie = true;
                    }
                }
                
                needChange = true;
            }
            
            break;
        case ACTORSTATE::WAIT:
        {
            if (armature->getAnimation()->isComplete()) {
                event = stateMachine->findEventVal("EVENT_ACTIONOVER");
                needChange = true;
            }
        }
            break;
        case ACTORSTATE::DIE:
            if (armature->getAnimation()->isComplete()) {
                dying = true;
                needChange = false;
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
    if (needChange) {
        switch2NextState(state, event);
        
        needChange = false;
        event = StateMachine::action_undefined;
    }

}

void ProtectNPC::resumeFight()
{
    armature->getAnimation()->resume();
    resume();
}

void ProtectNPC::pauseFight()
{
    armature->getAnimation()->pause();
    pause();

}
