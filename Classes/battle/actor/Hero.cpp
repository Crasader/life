#include "Hero.h"
#include "utils/GameUtils.h"
#include "GameDefine.h"
#include "ELProtocol.h"
#include "EventDefine.h"
#include "logic/ClientLogic.h"
#include "YNCamera.h"
#include "Effect.h"
//#include "ClientLogic/GameScene/GameScene.h"
#include "Guardian.h"
#include "SimpleAudioEngine.h"
//#include "Player_Account/Account_Data_Mgr.h"
//#include "Character_System/Character_Mgr.h"
//#include "Character_System/Player.h"

USING_NS_CC;
using namespace cocostudio;
using namespace CocosDenshion;

bool Hero::init()
{
    if (!Node::init()) {
        return false;
    }
    

    return true;
}
void Hero::onEnter()
{
    FightActor::onEnter();
    
    pauseListener = EventListenerCustom::create(PAUSE_FIGHT, CC_CALLBACK_0(Hero::pauseFight, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(pauseListener, -1);
    resumeListener = EventListenerCustom::create(RESUME_FIGHT, CC_CALLBACK_0(Hero::resumeFight, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(resumeListener, -1);
//    startPlotListener = EventListenerCustom::create(START_PLOT, CC_CALLBACK_1(Hero::startPlot, this));
//    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(startPlotListener, -1);
    fullHpListener = EventListenerCustom::create(FULL_HERO_HP, CC_CALLBACK_0(Hero::fullHp, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(fullHpListener, -1);

    addHurtListener();
}

void Hero::setupData(int idx)
{
    //    int player_id = Account_Data_Mgr::instance()->get_current_role_id();
    //    Game_Data::Player* player = dynamic_cast<Game_Data::Player*>(CHARACTER_MGR::instance()->get_character(player_id));
    initFightData(idx);
    weaponId = 6;
    
    initViewRes();
//    armature->getBone("WQ1")->changeDisplayWithIndex(weaponId, true);
    initFightState();
    
//    for (int i = 0; i < 6; i++) {
//        ghostPos[i] = Vec2(0, 0);
//    }
//
//    ghost1 = Armature::create(blackActor->getResName());
//    ghost1->setScale(blackActor->getScaleRate());
//    ghost1->setOpacity(180);
//    ghost1->setColor(Color3B(106, 113, 209));
//    ghost1->setPosition(ghostPos[2]);
//    addChild(ghost1);
//    
//    ghost2 = Armature::create(blackActor->getResName());
//    ghost2->setScale(blackActor->getScaleRate());
//    ghost2->setOpacity(100);
//    ghost2->setColor(Color3B(106, 113, 209));
//    ghost2->setPosition(ghostPos[5]);
//    addChild(ghost2);

    buffManager = BuffManager::create();
    addChild(buffManager);
    
    setDir(DIRECT::NONE);
    armature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(Hero::animEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void Hero::initFightData(int idx)
{
    blackActor = new BlackActor();
    blackActor->readFile(idx, FIGHTER_CONFIG_FILE);
    currSkill = nullptr;
    currSkillTag = nullptr;
    killSkill = nullptr;
    killSkillTag = nullptr;
    
    flipX = false;
    currKey = frameCommand = 0;
    isValidAtk = true;
    rush = false;
    rushReady = false;
    rushAtkReady = false;
    
    deHitDownVal = currHitDownVal = blackActor->getDeHitDownVal();
    
    walkSpeedX = blackActor->getMoveSpeed();
    rushSpeedX = blackActor->getRushSpeed();
    slowRate = 1.0;
    
    atkStiff = hurtStiff = false;
    
    localBonePoint = Point::ZERO;
    hurtMoveSpeed = 0;
    backInAir = false;
    comboNum = maxCombo = 0;
    showCombo = false;
    killer = nullptr;
    killers.clear();
    bHero = true;
    dying = false;
    dieLie = false;
    
    keyCommand = -1;
    plotActionIdx = -1;
    plotActionMission = 0;
    plotMoveMission = 0;
}

void Hero::addHurtListener()
{
//    hurtListener = EventListenerCustom::create(HERO_HURT_TAG, CC_CALLBACK_1(FightActor::parseHurtEvent, this));
//    punchListener = EventListenerCustom::create(MONSTER_RESET_PUNCH, CC_CALLBACK_1(FightActor::resetPunchTag, this));
//    
//    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(hurtListener, -1);
//    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(punchListener, -1);
    
    hurtListener = getEventDispatcher()->addCustomEventListener(HERO_HURT_TAG, CC_CALLBACK_1(FightActor::parseHurtEvent, this));
    punchListener = getEventDispatcher()->addCustomEventListener(MONSTER_RESET_PUNCH, CC_CALLBACK_1(FightActor::resetPunchTag, this));
    updateComboListener = EventListenerCustom::create(UPDATE_COMBO, CC_CALLBACK_1(Hero::updateComboFactor, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateComboListener, -1);
    clearComboListener = EventListenerCustom::create(CLEAR_COMBO, CC_CALLBACK_0(Hero::clearCombo, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(clearComboListener, -1);

}
void Hero::removeHurtListener()
{
    getEventDispatcher()->removeEventListener(hurtListener);
    getEventDispatcher()->removeEventListener(punchListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateComboListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(clearComboListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(pauseListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(resumeListener);
//    Director::getInstance()->getEventDispatcher()->removeEventListener(startPlotListener);

    buffManager->removeAllBuff();
//	getEventDispatcher()->removeCustomEventListeners(HERO_HURT_TAG);
//	getEventDispatcher()->removeCustomEventListeners(MONSTER_RESET_PUNCH);

}
Hero::Hero()
{
}
Hero::~Hero()
{
	removeHurtListener();
    Director::getInstance()->getEventDispatcher()->removeEventListener(fullHpListener);
}
void Hero::setupValues(int hpVal, int atkVal, int defVal, int critVal, int deCritVal)
{
    blackActor->setHp(hpVal);
    blackActor->setMaxHp(hpVal);
    blackActor->setMaxHp(hpVal);
    blackActor->setAttack(atkVal);
    blackActor->setDefence(defVal);
    blackActor->setCritRatio(critVal);
    blackActor->setTenacity(deCritVal);
}

void Hero::pauseFight()
{
    armature->getAnimation()->pause();
    pause();
}
void Hero::resumeFight()
{
    armature->getAnimation()->resume();
    resume();
}

void Hero::exitState(int currState)
{
    switch (currState) {
//        case ACTORSTATE::IDLE:
//            
//            break;
        
        case ACTORSTATE::FELLDOWN:
            currHitDownVal = deHitDownVal;
            break;
        case ACTORSTATE::ANY:
            frontEffectLayer->removeChildByTag(ATTACK_EFFECT_TAG);
        case ACTORSTATE::XULI:
            frontEffectLayer->removeChildByTag(XULI_EFFECT_TAG);
            break;
        default:
            break;
    }
}

void Hero::enterState(int nextState)
{
    switch (nextState) {
        case ACTORSTATE::IDLE:
            rush = false;
            stopMove();
            break;
        case ACTORSTATE::ATTACK:
            stopMove();
            break;
        case ACTORSTATE::SKILL:
            rush = false;
            stopMove();
            break;
        case ACTORSTATE::INAIR:
//            if (stateMachine->getNextAction() == HEROACTION::AIRHITDOWN) {
//                if (action == HEROACTION::AIRHITUP) {
//                    fallAccY = 0.2f;
//                    fallSpeedY = 3.0f;
//                }else{
//                    fallAccY = 0.07f;
//                    fallSpeedY = 0.8f;
//                }
//            }
            break;
        case ACTORSTATE::XULI:
        {
            std::string resPath = "armature/charge.ExportJson";
            ArmatureDataManager::getInstance()->addArmatureFileInfo(resPath);
            
            auto eff1 = Effect::create();
            eff1->initWithArmature("charge");
            eff1->setTag(XULI_EFFECT_TAG);
            eff1->setFollowY(false);
            frontEffectLayer->addChild(eff1);
        }
            break;
        case ACTORSTATE::DIE:
            if (blackActor->getHp() <= 0) {
                E2L_COMMON info;
                info.eProtocol = e2l_hero_die;
                ClientLogic::instance()->ProcessUIRequest(&info);
            }
            break;
        default:
            break;
    }
}

void Hero::dispatchResetPunchTag()
{
    EventCustom event(HERO_RESET_PUNCH);
    event.setUserData(this);
    getEventDispatcher()->dispatchEvent(&event);
}

void Hero::updateAI()
{
//    log("curr combo : %d", comboNum);
//    log("max combo : %d", maxCombo);

//    std::string ghost1Anim = ghost1->getAnimation()->getCurrentMovementID();
//    std::string ghost2Anim = ghost2->getAnimation()->getCurrentMovementID();
//    if (ghost1Anim != ghostAnim[2]) {
//        ghost1Anim = ghostAnim[2];
//        ghost1->getAnimation()->play(ghost1Anim);
//    }
//    if (ghost2Anim != ghostAnim[5]) {
//        ghost2Anim = ghostAnim[5];
//        ghost2->getAnimation()->play(ghost2Anim);
//    }
//    
//    float sx = getScaleRate();
//    ghost2->setScaleX((ghostFilpX[2]) ? -sx : sx);
//    ghost1->setScaleX((ghostFilpX[5]) ? -sx : sx);

    auto boneMoveVec2 = calBoneMove();
    
    updateFrontEffect();
    updateBackEffect();

    
    //状态内执行
    switch (state) {
        case ACTORSTATE::MOVE:
        {
            if(currDir == DIRECT::NONE)
            {
                break;
            }
            if (reverseMove) {
                int mask = 10;
                currDir = currDir ^ mask;
                speedX *= -1;
            }
            if (((currDir & DIRECT::LEFT)!=0) != flipX)
            {
                flipX = !flipX;
                float sx = blackActor->getScaleRate();
                armature->setScaleX((flipX) ? -sx : sx);
            }
            
            //移动
            posX = getPositionX() + speedX*slowRate;
            posY = getPositionY() + speedY*slowRate;
        }
            break;
        case ACTORSTATE::RUSH:
        {
            if(currDir == DIRECT::NONE)
            {
                break;
            }
            if (reverseMove) {
                int mask = 10;
                currDir = currDir ^ mask;
                speedX *= -1;
            }
            if (((currDir & DIRECT::LEFT)!=0) != flipX)
            {
                flipX = !flipX;
                float sx = blackActor->getScaleRate();
                armature->setScaleX((flipX) ? -sx : sx);
            }
            
//            //移动
//            posX = getPositionX() + speedX;
//            posY = getPositionY() + speedY;
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
            if (reverseMove) {
                int mask = 10;
                currDir = currDir ^ mask;
                speedX *= -1;
            }
            if (((currDir & DIRECT::LEFT)!=0) != flipX)
            {
                flipX = !flipX;
                float sx = blackActor->getScaleRate();
                armature->setScaleX((flipX) ? -sx : sx);
            }
            
            //移动
            posX = getPositionX() + speedX*slowRate;
            posY = getPositionY() + speedY*slowRate;
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
                    }
                }
            }else{
                if (hurtStiff) {
                    posX = getPositionX() + hurtMoveSpeed;
                }else{
                    posX = getPositionX();
                }
            }

        }
            break;
          
        case ACTORSTATE::IDLE:
        {
//            currHitDownVal++;
//            currHitDownVal = MIN(deHitDownVal, currHitDownVal);
        }
            break;
        case ACTORSTATE::FELLDOWN:
        {
            if (action == HEROACTION::LIESHOCK) {
                posX = getPositionX() + lieShockSpeed;
            }
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
                            startHurtStiff(killSkillTag->getHitStiffDur());
                        }else{
                            startHurtStiff(0.1);
                        }
                    }
                }
            }else{
                if (action == HEROACTION::AIRHITDOWN) {
                    fallSpeedY += fallAccY;
                    fallAccY *= 1.15;
                    fallSpeedY = MIN(30, fallSpeedY);
                    boneMoveVec2.y = -fallSpeedY*slowRate;
                }else if (action == HEROACTION::SAFE_DOWN){
                    fallSpeedY += fallAccY;
                    fallAccY *= 1.15;
                    fallSpeedY = MIN(30, fallSpeedY);
                    boneMoveVec2.y = -fallSpeedY*slowRate;
                    break;
                }
                
                if (killSkill!=nullptr) {
                    if (action == HEROACTION::AIRHIT)
                    {
                        boneMoveVec2.y =
                        killSkillTag->getAirAtkUpVal();
                    }else if (action == HEROACTION::AIRHITUP && killSkillTag->getFloatRate() != 99) {
                        boneMoveVec2.y *= killSkillTag->getFloatRate();
                    }
                    float hitSpeed = killSkillTag->getAirHitBackSpeed();
                    if (killSkillTag->gethitFlyRate() != 99) {
                        hitSpeed *= killSkillTag->gethitFlyRate();
                        hitSpeed += fallAccX;
                        fallAccX *= 1.25;
                        hitSpeed = MIN(10, hitSpeed);
                    }
                    hurtMoveSpeed = hitSpeed * ((currDir==DIRECT::RIGHT) ? -1 : 1);
                    posX = getPositionX() + hurtMoveSpeed;
                    
                }else{
                    posX = getPositionX() - 0.5 *((flipX) ? -1 : 1);
                }
            }
            
        }
            break;
            
        case ACTORSTATE::DIE:
        {
            bool visual = armature->isVisible();
            armature->setVisible(!visual);
        }
            break;
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
}
void Hero::resetWalkOrRush(int moveCmd, int dir)
{
    switch (moveCmd) {
        case KEYCOMMAND::CMD_STOPMOVE:
            rush = false;
//            stopMove();
            break;
        case KEYCOMMAND::CMD_WALK:
            rush = false;
//            getActionByTag(READY_RUSH_TAG)->stop();
//            setDir(dir);
//            startMove();
            break;
        case KEYCOMMAND::CMD_RUSH:
            rush = true;
            readyRushAtk();
//            setDir(dir);
//            startMove();
            break;
        default:
            break;
    }
    joyCommand = moveCmd;
}
void Hero::updateState()
{
//    if (rushReady && (currDir & (DIRECT::RIGHT | DIRECT::LEFT))!=0)
//    {
//        if (((currDir & DIRECT::LEFT)!=0) == flipX)
//        {
////            log("rush dir %d", currDir);
//
//            rush = true;
//            readyRushAtk();
//        }else{
//            getActionByTag(READY_RUSH_TAG)->stop();
//            rushReady = false;
//        }
//        
//    }
    bool needChange = false;
    
//    if (autoControll) {
//        switch (state) {
//            case ACTORSTATE::IDLE:
//                if (event != stateMachine->action_undefined)
//                {
//                    needChange = true;
//                    break;
//                }
//                
////                if (对方阵营没有人) {
////                    needChange = false;
////                     break;
////                }
//                if (!canFoundOpponent()) {
//                    foundSomeOneOutView();
//                    event = stateMachine->findEventVal("EVENT_MOVE2NEWVIEW");
//                    needChange = true;
//                }else{
//                    std::string skillKey = canUseSomeSkill();
//                    if (skillKey != "NONE") {
//                        event = stateMachine->findEventVal(skillKey);
//                        needChange = true;
//                    } else if (canAttackTarget()) {
//                        event = stateMachine->findEventVal("EVENT_AUTOATTACK");
//                        needChange = true;
//                    } else if (canRushAttack()) {
//                        event = stateMachine->findEventVal("EVENT_RUSHATTACK");
//                        needChange = true;
//                    } else {
//                        event = stateMachine->findEventVal("EVENT_AUTOCHASE");
//                        needChange = true;
//                    }
//                }
//                break;
//            case ACTORSTATE::MOVE:
//                if (event != stateMachine->action_undefined) {
//                    needChange = true;
//                    break;
//                }
//                if (canAttackTarget()) {
//                    event = stateMachine->findEventVal("EVENT_AUTOATTACK");
//                    needChange = true;
//                } else if (canRushAttack()) {
//                    event = stateMachine->findEventVal("EVENT_RUSHATTACK");
//                    needChange = true;
//                } else if (arriveTargetPos()) {
//                    event = stateMachine->findEventVal("EVENT_ARRIVE_TARGET");
//                    needChange = true;
//                }
//                break;
//            case ACTORSTATE::CHASE:
//                if (event != stateMachine->action_undefined) {
//                    needChange = true;
//                    break;
//                }
//                if (canAttackTarget()) {
//                    event = stateMachine->findEventVal("EVENT_AUTOATTACK");
//                    needChange = true;
//                } else if (canRushAttack()) {
//                    event = stateMachine->findEventVal("EVENT_RUSHATTACK");
//                    needChange = true;
//                } else if (arriveTargetPos()) {
//                    event = stateMachine->findEventVal("EVENT_ARRIVE_TARGET");
//                    needChange = true;
//                }
//                break;
//            default:
//                break;
//        }
//    }
    
    
    //此处相关的判断，应移交lua处理
    switch (state)
	{
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
        {
            if (event == stateMachine->findEventVal("EVENT_XULI")) {
                needChange = true;
            }

            if (armature->getAnimation()->isComplete()) {
                if (action == HEROACTION::WIN) {
                    event = stateMachine->findEventVal("WIN");
                    needChange = true;
                    break;
                }
            }
            if (event != stateMachine->action_undefined) {
                needChange = true;
                break;
            }
            
            
            if (joyCommand == KEYCOMMAND::CMD_WALK)
            {
                event = stateMachine->findEventVal("EVENT_MOVE");
                needChange = true;
            }
            if (joyCommand == KEYCOMMAND::CMD_RUSH)
            {
                event = stateMachine->findEventVal("EVENT_RUSH");
                needChange = true;
            }
            
            if (keyCommand == GAMEKEY::BUTTON_JUMP)
            {
                event = stateMachine->findEventVal("EVENT_JUMP");
                needChange = true;
            }
            
            if (currKey == GAMEKEY::BUTTON_A)
            {
                event = stateMachine->findEventVal("EVENT_ATTACK1");
                needChange = true;
            }
            if (currKey == GAMEKEY::BUTTON_SP)
            {
                if (canUseSkill(blackActor->equipSkills[SKILLS_KEY]) == SKILL_NONE_ERROR) {
                    event = stateMachine->findEventVal("EVENT_SKILL4");
                    needChange = true;
                }
            }
            if (keyCommand == GAMEKEY::BUTTON_S1) {
                if (canUseSkill(blackActor->equipSkills[SKILLA_KEY]) == SKILL_NONE_ERROR) {
                    event = stateMachine->findEventVal("EVENT_SKILLA");
                    needChange = true;
                }
            }
            if (keyCommand == GAMEKEY::BUTTON_S2) {
                if (canUseSkill(blackActor->equipSkills[SKILLB_KEY]) == SKILL_NONE_ERROR) {
                    event = stateMachine->findEventVal("EVENT_SKILLB");
                    needChange = true;
                }
            }
            if (keyCommand == GAMEKEY::BUTTON_HOLY_SWORD) {
                if (canUseSkill(blackActor->equipSkills[HOLY_SKILL_KEY]) == SKILL_NONE_ERROR) {
                    event = stateMachine->findEventVal("EVENT_HOLY_SWORD");
                    needChange = true;
                }
            }
            if (keyCommand == GAMEKEY::BUTTON_Y) {
                if (canUseSkill(blackActor->equipSkills[PET_SKILL1_KEY+(blackActor->getMaxPetCount()-blackActor->getPetCount())]) == SKILL_NONE_ERROR) {
                    event = stateMachine->findEventVal("EVENT_PET");
                    needChange = true;
                }
            }
//            if (keyCommand == GAMEKEY::BUTTON_S3) {
//                if (canUseSkill(blackActor->equipSkills[SKILLC_KEY]) == SKILL_NONE_ERROR) {
//                    event = stateMachine->findEventVal("EVENT_SKILLC");
//                    needChange = true;
//                }
//            }

            if (keyCommand == GAMEKEY::BUTTON_J) {
                if (canUseSkill(blackActor->equipSkills[SKILLD_KEY]) == SKILL_NONE_ERROR) {
                    event = stateMachine->findEventVal("EVENT_SKILLD");
                    needChange = true;
                }
            }

        }
            break;
        case ACTORSTATE::MOVE:
            if (event != stateMachine->action_undefined) {
                needChange = true;
                break;
            }
            
            if (joyCommand == KEYCOMMAND::CMD_STOPMOVE)
            {
//                rush = false;
                event = stateMachine->findEventVal("EVENT_STOPMOVE");
                needChange = true;
            }

            if (joyCommand == KEYCOMMAND::CMD_RUSH) {
                event = stateMachine->findEventVal("EVENT_RUSH");
                needChange = true;
            }
        
            if (keyCommand == GAMEKEY::BUTTON_JUMP)
            {
                event = stateMachine->findEventVal("EVENT_JUMP");
                needChange = true;
            }
            if (currKey == GAMEKEY::BUTTON_A)
            {
                event = stateMachine->findEventVal("EVENT_ATTACK1");
                needChange = true;
            }
            if (currKey == GAMEKEY::BUTTON_SP)
            {
                if (canUseSkill(blackActor->getSkillSPID()) == SKILL_NONE_ERROR) {
                    event = stateMachine->findEventVal("EVENT_SKILL4");
                    needChange = true;
                }
            }
            
            if (keyCommand == GAMEKEY::BUTTON_J) {
                if (canUseSkill(blackActor->equipSkills[SKILLD_KEY]) == SKILL_NONE_ERROR) {
                    event = stateMachine->findEventVal("EVENT_SKILLD");
                    needChange = true;
                }
            }
            if (keyCommand == GAMEKEY::BUTTON_S1) {
                if (canUseSkill(blackActor->equipSkills[SKILLA_KEY]) == SKILL_NONE_ERROR) {
                    event = stateMachine->findEventVal("EVENT_SKILLA");
                    needChange = true;
                }
            }
            if (keyCommand == GAMEKEY::BUTTON_S2) {
                if (canUseSkill(blackActor->equipSkills[SKILLB_KEY]) == SKILL_NONE_ERROR) {
                    event = stateMachine->findEventVal("EVENT_SKILLB");
                    needChange = true;
                }
            }
//            if (keyCommand == GAMEKEY::BUTTON_S3) {
//                if (canUseSkill(blackActor->equipSkills[SKILLC_KEY]) == SKILL_NONE_ERROR) {
//                    event = stateMachine->findEventVal("EVENT_SKILLC");
//                    needChange = true;
//                }
//            }
            if (keyCommand == GAMEKEY::BUTTON_HOLY_SWORD) {
                if (canUseSkill(blackActor->equipSkills[HOLY_SKILL_KEY]) == SKILL_NONE_ERROR) {
                    event = stateMachine->findEventVal("EVENT_HOLY_SWORD");
                    needChange = true;
                }
            }
            if (keyCommand == GAMEKEY::BUTTON_Y) {
                if (canUseSkill(blackActor->equipSkills[PET_SKILL1_KEY+(blackActor->getMaxPetCount()-blackActor->getPetCount())]) == SKILL_NONE_ERROR) {
                    event = stateMachine->findEventVal("EVENT_PET");
                    needChange = true;
                }
            }


            break;
        case ACTORSTATE::RUSH:
        {
            if (event == stateMachine->findEventVal("EVENT_XULI")) {
                needChange = true;
            }

            if (event != stateMachine->action_undefined) {
                needChange = true;
                break;
            }
            
            if (joyCommand == KEYCOMMAND::CMD_STOPMOVE)
            {
                event = stateMachine->findEventVal("EVENT_STOPMOVE");
                needChange = true;
            }
            if (joyCommand == KEYCOMMAND::CMD_WALK) {
                event = stateMachine->findEventVal("EVENT_MOVE");
                needChange = true;
            }
            if (currKey == GAMEKEY::BUTTON_SP)
            {
                if (canUseSkill(blackActor->getSkillSPID()) == SKILL_NONE_ERROR) {
                    event = stateMachine->findEventVal("EVENT_SKILL4");
                    needChange = true;
                }
            }
            
            if (keyCommand == GAMEKEY::BUTTON_J) {
                if (canUseSkill(blackActor->equipSkills[SKILLD_KEY]) == SKILL_NONE_ERROR) {
                    event = stateMachine->findEventVal("EVENT_SKILLD");
                    needChange = true;
                }
            }
            if (keyCommand == GAMEKEY::BUTTON_S1) {
                if (canUseSkill(blackActor->equipSkills[SKILLA_KEY]) == SKILL_NONE_ERROR) {
                    event = stateMachine->findEventVal("EVENT_SKILLA");
                    needChange = true;
                }
            }
            if (keyCommand == GAMEKEY::BUTTON_S2) {
                if (canUseSkill(blackActor->equipSkills[SKILLB_KEY]) == SKILL_NONE_ERROR) {
                    event = stateMachine->findEventVal("EVENT_SKILLB");
                    needChange = true;
                }
            }
//            if (keyCommand == GAMEKEY::BUTTON_S3) {
//                if (canUseSkill(blackActor->equipSkills[SKILLC_KEY]) == SKILL_NONE_ERROR) {
//                    event = stateMachine->findEventVal("EVENT_SKILLC");
//                    needChange = true;
//                }
//            }
            if (keyCommand == GAMEKEY::BUTTON_HOLY_SWORD) {
                if (canUseSkill(blackActor->equipSkills[HOLY_SKILL_KEY]) == SKILL_NONE_ERROR) {
                    event = stateMachine->findEventVal("EVENT_HOLY_SWORD");
                    needChange = true;
                }
            }
            if (keyCommand == GAMEKEY::BUTTON_Y) {
                if (canUseSkill(blackActor->equipSkills[PET_SKILL1_KEY+(blackActor->getMaxPetCount()-blackActor->getPetCount())]) == SKILL_NONE_ERROR) {
                    event = stateMachine->findEventVal("EVENT_PET");
                    needChange = true;
                }
            }


            if (currKey == GAMEKEY::BUTTON_A)
            {
                if (rushAtkReady) {
                    rushAtkReady = false;
                    if (canUseSkill(blackActor->getRunAttack1ID()) == SKILL_NONE_ERROR) {
                        event = stateMachine->findEventVal("EVENT_RUNATTACK1");
                    }
                }else{
                    if (canUseSkill(blackActor->getRunAttack2ID()) == SKILL_NONE_ERROR) {
                        event = stateMachine->findEventVal("EVENT_RUNATTACK2");
                    }
                }
                
                needChange = true;
            }
       }
            break;
        case ACTORSTATE::JUMP:
        {
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
        case ACTORSTATE::ATTACK:
            if (event == stateMachine->findEventVal("EVENT_HEADHURT")|| event == stateMachine->findEventVal("EVENT_WAISTHURT") || event == stateMachine->findEventVal("EVENT_HEADFELLGROUND") || event == stateMachine->findEventVal("EVENT_WAISTFELLGROUND") || event == stateMachine->findEventVal("EVENT_HIT2AIR") || event == stateMachine->findEventVal("EVENT_AIRHIT")) {
                state = ACTORSTATE::ANY;
                needChange = true;
                break;
            }

            //            if (getIsValidAtk() || (blackActor->getJob() == 3)) {
            if (currKey == GAMEKEY::BUTTON_A && delayEvent == StateMachine::action_undefined)
            {
                //                    if ((frameCommand & HEROFRAMECOMMAND::COMMAND_PUNCHA2) != 0) {
                //                        event = stateMachine->findEventVal("EVENT_ATTACK");
                //                    }
                if ((frameCommand & HEROFRAMECOMMAND::COMMAND_PUNCHA2) != 0) {
                    //                        event = stateMachine->findEventVal("EVENT_ATTACK2");
                    delayEvent = stateMachine->findEventVal("EVENT_ATTACK2");
                }
                if ((frameCommand & HEROFRAMECOMMAND::COMMAND_PUNCHA3) != 0) {
                    //                        event = stateMachine->findEventVal("EVENT_ATTACK3");
                    delayEvent = stateMachine->findEventVal("EVENT_ATTACK3");
                }
                if ((frameCommand & HEROFRAMECOMMAND::COMMAND_PUNCHA4) != 0) {
                    //                        event = stateMachine->findEventVal("EVENT_ATTACK4");
                    delayEvent = stateMachine->findEventVal("EVENT_ATTACK4");
                }
            }
            //            }
            
            if (armature->getAnimation()->isComplete())
            {
                //                if (delayEvent == StateMachine::action_undefined) {
                //                    if (action == HEROACTION::ATTACKPART1) {
                //                        event = stateMachine->findEventVal("ATTACKPART1");
                //                    }else {
                //                        event = stateMachine->findEventVal("EVENT_ACTIONOVER");
                //                    }
                //                }
                if (delayEvent == stateMachine->findEventVal("EVENT_ATTACK2")) {
                    event = delayEvent;
                    delayEvent = StateMachine::action_undefined;
                }else if (delayEvent == stateMachine->findEventVal("EVENT_ATTACK3")) {
                    event = delayEvent;
                    delayEvent = StateMachine::action_undefined;
                }else if (delayEvent == stateMachine->findEventVal("EVENT_ATTACK4")) {
                    event = delayEvent;
                    delayEvent = StateMachine::action_undefined;
                }else{
                    if (action == HEROACTION::ATTACKPART1) {
                        event = stateMachine->findEventVal("ATTACKPART1");
                    }else {
                        event = stateMachine->findEventVal("EVENT_ACTIONOVER");
                    }
                }
                if(currDir != DIRECT::NONE)
                {
                    if (reverseMove) {
                        int mask = 10;
                        currDir = currDir ^ mask;
                        speedX *= -1;
                    }
                    if (((currDir & DIRECT::LEFT)!=0) != flipX)
                    {
                        flipX = !flipX;
                        float sx = blackActor->getScaleRate();
                        armature->setScaleX((flipX) ? -sx : sx);
                    }
                }
                
                needChange = true;
            }
            break;

        case ACTORSTATE::RUNATTACK:
            if (currKey == GAMEKEY::BUTTON_A)
            {
                if (currSkill->getLinkID() != -1) {
                    if (canUseSkill(currSkill->getLinkID()) == SKILL_NONE_ERROR) {
                        event = stateMachine->findEventVal("EVENT_RUNATTACK");
                    }
                }
                
            }
            if (armature->getAnimation()->isComplete()) {
                if (event == StateMachine::action_undefined) {
                    event = stateMachine->findEventVal("EVENT_ACTIONOVER");
                }
                needChange = true;
            }            
            break;
            
        case ACTORSTATE::XULI:
        {
            if (event != stateMachine->action_undefined) {
                needChange = true;
                break;
            }
            int baseSkillId = -1;
            if (keyCommand == GAMEKEY::BUTTON_S1) {
                baseSkillId = blackActor->equipSkills[SKILLA_KEY];
                event = stateMachine->findEventVal("EVENT_SKILLA");
                needChange = true;
            }else if (keyCommand == GAMEKEY::BUTTON_S2) {
                baseSkillId = blackActor->equipSkills[SKILLB_KEY];
                event = stateMachine->findEventVal("EVENT_SKILLB");
                needChange = true;
//            }else if (keyCommand == GAMEKEY::BUTTON_S3) {
//                baseSkillId = blackActor->equipSkills[SKILLC_KEY];
//                event = stateMachine->findEventVal("EVENT_SKILLC");
//                needChange = true;
            }else if (keyCommand == GAMEKEY::BUTTON_J) {
                baseSkillId = blackActor->equipSkills[SKILLD_KEY];
                event = stateMachine->findEventVal("EVENT_SKILLD");
                needChange = true;
            }
            if (keyCommand == GAMEKEY::BUTTON_HOLY_SWORD) {
                if (canUseSkill(blackActor->equipSkills[HOLY_SKILL_KEY]) == SKILL_NONE_ERROR) {
                    event = stateMachine->findEventVal("EVENT_HOLY_SWORD");
                    needChange = true;
                }
            }
            if (keyCommand == GAMEKEY::BUTTON_Y) {
                if (canUseSkill(blackActor->equipSkills[PET_SKILL1_KEY+(blackActor->getMaxPetCount()-blackActor->getPetCount())]) == SKILL_NONE_ERROR) {
                    event = stateMachine->findEventVal("EVENT_PET");
                    needChange = true;
                }
            }


            if (baseSkillId != -1) {
                auto baseSkill = blackActor->selSkills[baseSkillId];
                int linkSkillId = baseSkill->getXuliId();
                if (canUseSkill(linkSkillId) != SKILL_NONE_ERROR) {
                    event = stateMachine->findEventVal("EVENT_ACTIONOVER");
                }
            }
        }
            break;
        case ACTORSTATE::SKILL:
//            if (event != stateMachine->action_undefined) {
//                needChange = true;
//                break;
//            }
            if (event == stateMachine->findEventVal("EVENT_HEADHURT")|| event == stateMachine->findEventVal("EVENT_WAISTHURT") || event == stateMachine->findEventVal("EVENT_HEADFELLGROUND") || event == stateMachine->findEventVal("EVENT_WAISTFELLGROUND") || event == stateMachine->findEventVal("EVENT_HIT2AIR") || event == stateMachine->findEventVal("EVENT_AIRHIT")) {
                state = ACTORSTATE::ANY;
                needChange = true;
                break;
            }
            
            if (keyCommand == GAMEKEY::BUTTON_S1) {
                event = stateMachine->findEventVal("EVENT_SKILLA");
            }
            if (keyCommand == GAMEKEY::BUTTON_S2) {
                event = stateMachine->findEventVal("EVENT_SKILLB");
            }
//            if (keyCommand == GAMEKEY::BUTTON_S3) {
//                event = stateMachine->findEventVal("EVENT_SKILLC");
//            }
            if (keyCommand == GAMEKEY::BUTTON_J) {
                event = stateMachine->findEventVal("EVENT_SKILLD");
            }
            if (currKey == GAMEKEY::BUTTON_A)
            {
                keyCommand = StateMachine::action_undefined;
                event = stateMachine->findEventVal("EVENT_ATTACK1");
            }
            if (keyCommand == GAMEKEY::BUTTON_HOLY_SWORD) {
                event = stateMachine->findEventVal("EVENT_HOLY_SWORD");
            }
            if (keyCommand == GAMEKEY::BUTTON_Y) {
                event = stateMachine->findEventVal("EVENT_PET");
            }

//            log("skill event: %d", event);
            if (armature->getAnimation()->isComplete()) {
                log("event active: %d", event);
//                log("animation end: %s", armature->getAnimation()->getCurrentMovementID().c_str());
//                if(currDir != DIRECT::NONE)
//                {
//                    if (reverseMove) {
//                        int mask = 10;
//                        currDir = currDir ^ mask;
//                        speedX *= -1;
//                    }
//                    if (((currDir & DIRECT::LEFT)!=0) != flipX)
//                    {
//                        flipX = !flipX;
//                        float sx = blackActor->getScaleRate();
//                        armature->setScaleX((flipX) ? -sx : sx);
//                    }
//                }
                if (event == StateMachine::action_undefined) {
                    event = stateMachine->findEventVal("EVENT_ACTIONOVER");
                    if (action == HEROACTION::SKILLATK3 && !isLandGround()) {
                        event = stateMachine->findEventVal("EVENT_AIR_SKILL");
                        needChange = true;
                        break;
                    }
                }
                int skillId = -1;
                if (event == stateMachine->findEventVal("EVENT_SKILLA")) {
                    skillId = blackActor->equipSkills[SKILLA_KEY];
                }else if (event == stateMachine->findEventVal("EVENT_SKILLB")) {
                    skillId = blackActor->equipSkills[SKILLB_KEY];
                }else if (event == stateMachine->findEventVal("EVENT_SKILLC")) {
                    skillId = blackActor->equipSkills[SKILLC_KEY];
                }else if (event == stateMachine->findEventVal("EVENT_SKILLD")) {
                    skillId = blackActor->equipSkills[SKILLD_KEY];
                }else if (event == stateMachine->findEventVal("EVENT_HOLY_SWORD")) {
                    skillId = blackActor->equipSkills[HOLY_SKILL_KEY];
                }else if (event == stateMachine->findEventVal("EVENT_PET")) {
                    skillId = blackActor->equipSkills[PET_SKILL1_KEY+(blackActor->getMaxPetCount()-blackActor->getPetCount())];
                }
                if (skillId != -1) {
                    if (canUseSkill(skillId) != SKILL_NONE_ERROR) {
                        event = stateMachine->findEventVal("EVENT_ACTIONOVER");
                    }
                }
                
                
                
                needChange = true;
            }
            break;
        case ACTORSTATE::SPSKILL:
        {
            if (event != stateMachine->action_undefined) {
                needChange = true;
                break;
            }
            if (armature->getAnimation()->isComplete()) {
                event = stateMachine->findEventVal("EVENT_ACTIONOVER");
                needChange = true;
            }
        }
            break;
        case ACTORSTATE::FAINT:
            if (!isFaint) {
                event = stateMachine->findEventVal("EVENT_ACTIONOVER");
                needChange = true;
            }
            break;

        case ACTORSTATE::HURT:
            if (currKey == GAMEKEY::BUTTON_SP)
            {
                if (canUseSkill(blackActor->getSkillSPID()) == SKILL_NONE_ERROR) {
                    event = stateMachine->findEventVal("EVENT_SKILL4");
                    needChange = true;
//                    break;
                }
            }
            if (keyCommand == GAMEKEY::BUTTON_S3) {
                if (canUseSkill(blackActor->equipSkills[SKILLC_KEY]) == SKILL_NONE_ERROR) {
                    event = stateMachine->findEventVal("EVENT_SKILLC");
                    needChange = true;
                    break;
                }
            }

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
                
            }
            
            break;
        case ACTORSTATE::FELLDOWN:
            if (event == stateMachine->findEventVal("EVENT_KG")) {
                needChange = true;
                break;
            }
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
                    }

                    event = stateMachine->findEventVal("GETUP");
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
                dying = true;
                needChange = false;
            }
            break;
        case ACTORSTATE::INAIR:
            if (keyCommand == GAMEKEY::BUTTON_S3) {
                if (canUseSkill(blackActor->equipSkills[SKILLC_KEY]) == SKILL_NONE_ERROR) {
                    event = stateMachine->findEventVal("EVENT_SKILLC");
                    needChange = true;
                    break;
                }
            }

            if (event == stateMachine->findEventVal("EVENT_HIT2AIR") ||
                event == stateMachine->findEventVal("EVENT_HEADHURT") ||
                event == stateMachine->findEventVal("EVENT_WAISTHURT") ||
                event == stateMachine->findEventVal("EVENT_HEADFELLGROUND") ||
                event == stateMachine->findEventVal("EVENT_WAISTFELLGROUND")) {
                
                event = stateMachine->findEventVal("EVENT_AIRHIT");
                needChange = true;
                break;
            }
            if (isLandGround() && action == HEROACTION::AIRHITDOWN) {
                event = stateMachine->findEventVal("EVENT_LANDGROUND");
                needChange = true;
            }
            if (isLandGround() && action == HEROACTION::SAFE_DOWN) {
                event = stateMachine->findEventVal("EVENT_LAND_CACHE");
                needChange = true;
            }
            if (armature->getAnimation()->isComplete()) {
                if (action == HEROACTION::AIRHITDOWN) {
                    break;
                }
                if (action == HEROACTION::SAFE_DOWN) {
                    break;
                }
                event = action;
                needChange = true;
            }
            break;
        case ACTORSTATE::BOUNDGROUND:
        {
            if (armature->getAnimation()->isComplete()) {
                event = stateMachine->findEventVal("EVENT_KGE");
                needChange = true;
            }
        }
            break;
        default:
            break;
	}
    std::string eventStr = stateMachine->eventName[event];
    if (eventStr == "EVENT_HEADHURT" || eventStr == "EVENT_WAISTHURT" || eventStr == "EVENT_HEADFELLGROUND" || eventStr == "EVENT_WAISTFELLGROUND" || eventStr == "EVENT_HIT2AIR" || eventStr == "EVENT_AIRHIT") {
        state = ACTORSTATE::ANY;
        needChange = true;
    }
    if (eventStr == "EVENT_VICTORY") {
        state = ACTORSTATE::ANY;
        setDir(DIRECT::NONE);
        needChange = true;
    }

    
    clearKey();
    //确定要转换时，再进入状态机进行状态跳转
    if (needChange) {
        switch2NextState(state, event);
        
        needChange = false;
    }
    event = StateMachine::action_undefined;
    keyCommand = StateMachine::action_undefined;
//    std::string heroAnim = armature->getAnimation()->getCurrentMovementID();
//    ghost2->setPosition(ghostPos[5]);
//    ghost1->setPosition(ghostPos[2]);
//    ghostPos[0] = (armature->getPosition());
//    ghostFilpX[0] = flipX;
//    ghostAnim[0] = heroAnim;
//    for (int i = 5; i > 0; i--) {
//        ghostPos[i] = ghostPos[i-1];
//        ghostFilpX[i] = ghostFilpX[i-1];
//        ghostAnim[i] = ghostAnim[i-1];
//    }

}


bool Hero::setCurrKey(int key)
{
	if (currKey != 0)
	{
        //没松开任何一个键时，不允许再处理另外一个按键
		return false;
	}
	currKey = key;
    if (Guard != nullptr) {
        (dynamic_cast<Guardian *>(Guard))->setCurrKey(currKey);
    }
    return true;
}


void Hero::clearKey()
{
	currKey = 0;
    keyCommand = -1;
}

void Hero::readyRushAtk()
{
    setRushAtkReady(true);
    auto func2 = CallFunc::create(CC_CALLBACK_0(Hero::setRushAtkReady, this, false));
    auto rushAtkAction = Sequence::create(DelayTime::create(RUSH_LINK_TIME), func2, NULL);
    runAction(rushAtkAction);
    rushAtkAction->setTag(READY_RUSHATK_TAG);
}


void Hero::doFlag(int nextFlag)
{
    switch (nextFlag) {
        case StateAIFlag::invincible:
            addSingleBUff(true, 34, 100);
            break;
        case StateAIFlag::startFall:
        {
//            float originOffy = armature->getBone("OriginPoint")->getWorldInfo()->y;
//            fallStartY += armature->getBone(localBodyName)->getWorldInfo()->y - originOffy;
//            armature->setPositionY(fallStartY);
        }
            break;
        case StateAIFlag::endFall:
        {
            if (currDir != DIRECT::NONE) {
                stateMachine->setNextTactics(ACTORSTATE::MOVE);
                stateMachine->setNextAction(HEROACTION::WALK);
            }
            setPosY(GroundY);
            setPositionY(GroundY);
            armature->setPositionY(0);
        }
        break;
//            fallStartY = 0;
//            armature->setPositionY(0);
        default:
            break;
    }
    
    
}

void Hero::backSkillAtkBase()
{
    auto skl = blackActor->selSkills[blackActor->equipSkills[ATK_KEY]];
//    auto skl = blackActor->selSkills[blackActor->getAtkSkillID()];
    blackActor->setAtkSkillID(skl->getBaseSkillID());
//    dynamic_cast<GameScene *>(Director::getInstance()->getRunningScene())->hideLinkBar(SKILLD_BTN, blackActor->selSkills[blackActor->getSkillDID()]->geticonStr(), blackActor->selSkills[blackActor->getSkillDID()]->getCoolDownDur());
}

void Hero::startXuli(int key)
{
    int skillId=-1;
    switch (key) {
        case GAMEKEY::BUTTON_S1:
            skillId = blackActor->equipSkills[SKILLA_KEY];
            break;
        case GAMEKEY::BUTTON_S2:
            skillId = blackActor->equipSkills[SKILLB_KEY];
            break;
        case GAMEKEY::BUTTON_S3:
            skillId = blackActor->equipSkills[SKILLC_KEY];;
            break;
        case GAMEKEY::BUTTON_J:
            skillId = blackActor->equipSkills[SKILLD_KEY];;
            break;
        case GAMEKEY::BUTTON_HOLY_SWORD:
            skillId = blackActor->equipSkills[HOLY_SKILL_KEY];
            break;
        case GAMEKEY::BUTTON_Y:
            skillId = blackActor->equipSkills[PET_SKILL1_KEY+(blackActor->getMaxPetCount()-blackActor->getPetCount())];
            break;
        default:
            break;
    }
    
    if (skillId <= 0) {
        return;
    }
    
    auto skl = blackActor->selSkills[skillId];
    if (skl->getXuliId() == -1) {
        keyCommand = key;
        return;
    }
    
    isXuliEnough = false;
    stopActionByTag(XULI_ENOUGH_TAG);
    auto func = CallFunc::create(CC_CALLBACK_0(Hero::xuliEnough, this));
    auto s = Sequence::create(DelayTime::create(skl->getXuliDur()), func, nullptr);
    s->setTag(XULI_ENOUGH_TAG);
    runAction(s);
}

void Hero::endXuli(int key)
{
    stopActionByTag(XULI_ENOUGH_TAG);
    int skillId=-1;
    switch (key) {
        case GAMEKEY::BUTTON_S1:
            skillId = blackActor->equipSkills[SKILLA_KEY];
            break;
        case GAMEKEY::BUTTON_S2:
            skillId = blackActor->equipSkills[SKILLB_KEY];
            break;
        case GAMEKEY::BUTTON_S3:
            skillId = blackActor->equipSkills[SKILLC_KEY];
            break;
        case GAMEKEY::BUTTON_J:
            skillId = blackActor->equipSkills[SKILLD_KEY];
            break;
        case GAMEKEY::BUTTON_HOLY_SWORD:
            skillId = blackActor->equipSkills[HOLY_SKILL_KEY];
            break;
        case GAMEKEY::BUTTON_Y:
            skillId = blackActor->equipSkills[PET_SKILL1_KEY+(blackActor->getMaxPetCount()-blackActor->getPetCount())];
            break;

        default:
            break;
    }
    if (skillId == -1) {
        return;
    }
    
    auto skl = blackActor->selSkills[skillId];
    if (skl->getXuliId() == -1) {
        return;
    }

    keyCommand = key;
    isXuliEnough = false;
}

void Hero::xuliEnough()
{
    isXuliEnough = true;
    event = stateMachine->findEventVal("EVENT_XULI");
}


void Hero::setHp(float hp)
{
    blackActor->setHp(hp);
    //todo 更新界面
//    GameScene::updateHp(hp, blackActor->getMaxHp());
    EventCustom event(UPDATE_HERO_HP);
    HP_UPDATE data;
    data.curr = blackActor->getHp();
    data.max = blackActor->getMaxHp();
    event.setUserData(&data);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    
}

void Hero::setMaxHp(float val)
{
    blackActor->setMaxHp(val);
}

void Hero::setMp(float val)
{
    float mp = MAX(0, val);
    mp = MIN(blackActor->getMaxMp(), mp);
    blackActor->setMp(mp);
    //todo 更新界面
//    GameScene::updateMp(mp, blackActor->getMaxMp());
}
void Hero::addAv(float val)
{
    float av = blackActor->getAV() + val;
    if (av >= blackActor->getMaxAV()) {
        if (blackActor->getAp() >= blackActor->getMaxAp()) {
            blackActor->setAV(blackActor->getMaxAV()-1);
            //todo 更新界面
//            GameScene::updateAp(blackActor->getAp(), blackActor->getAV(), blackActor->getMaxAV());
        }else{
            blackActor->setAV(0);
            blackActor->setAp(blackActor->getAp()+1);
            //todo 更新界面
            EventCustom event(UPDATE_AP);
            AP_UPDATE data;
            data.curr = blackActor->getAp();
            data.max = blackActor->getMaxAp();
            event.setUserData(&data);
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
//            GameScene::updateAp(blackActor->getAp(), blackActor->getAV(), blackActor->getMaxAV());
//            GameScene::addAp(1, blackActor->getAp());
        }
    }else{
        blackActor->setAV(av);
        //todo 更新界面
//        GameScene::updateAp(blackActor->getAp(), blackActor->getAV(), blackActor->getMaxAV());
    }
}
void Hero::useAp(float val)
{
    if (val <= 0) {
        return;
    }
    float ap = blackActor->getAp() - val;
    ap = MAX(0, ap);
    blackActor->setAp(ap);
    //todo 更新界面
    EventCustom event(UPDATE_AP);
    AP_UPDATE data;
    data.curr = blackActor->getAp();
    data.max = blackActor->getMaxAp();
    event.setUserData(&data);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);

//    GameScene::updateAp(blackActor->getAp(), blackActor->getAV(), blackActor->getMaxAV());
}

void Hero::setGhost(float ghost)
{
//    if (Guard == nullptr) {
//        return;
//    }
//    float val = MAX(0, ghost);
//    val = MIN(Guard->getBlackActor()->getMaxGhost(), val);
//    Guard->getBlackActor()->setGhost(ghost);
//    GameScene::updateMp(ghost, Guard->getBlackActor()->getMaxGhost());

    float val = MAX(0, ghost);
    val = MIN(blackActor->getMaxGhost(), val);
    blackActor->setGhost(val);
    EventCustom event(UPDATE_GHOST);
    GHOST_UPDATE data;
    data.curr = blackActor->getGhost();
    event.setUserData(&data);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void Hero::fullHp()
{
    setHp(blackActor->getMaxHp());
}

void Hero::revive()
{
    fullHp();
    hpEmpty = false;
    armature->setVisible(true);
    dieLie = false;
    dying = false;
    changeState(ACTORSTATE::INIT);
    changeActionWithName(HEROACTION::BORN);
    stateMachine->changeState(ACTORSTATE::INIT, HEROACTION::BORN);
    addSingleBUff(true, 34, 100);
}

void Hero::updateComboFactor(cocos2d::EventCustom *event)
{
    L2E_ADD_COMBO info = *static_cast<L2E_ADD_COMBO*>(event->getUserData());
    comboFactor = info.factor;
}

void Hero::clearCombo()
{
    comboFactor = 0;
}

void Hero::startPlot(cocos2d::EventCustom *event)
{
    L2E_START_PLOT info = *static_cast<L2E_START_PLOT*>(event->getUserData());
    if(info.type == 15) {
        blackActor->setPlotProtect(true);
        
        E2L_COMPLETE_PLOT infoPlot;
        infoPlot.eProtocol = e2l_complete_plot;
        infoPlot.missionId = info.missionId;
        infoPlot.value = 0;
        ClientLogic::instance()->ProcessUIRequest(&infoPlot);
    }else if (info.type == 16){
        blackActor->setAp(3);
        
        E2L_COMPLETE_PLOT infoPlot;
        infoPlot.eProtocol = e2l_complete_plot;
        infoPlot.missionId = info.missionId;
        infoPlot.value = 0;
        ClientLogic::instance()->ProcessUIRequest(&infoPlot);
    }
}

void Hero::plotChangeAction(int action, bool flipX, int missionId)
{
    changeState(IDLE);
    setDir(DIRECT::NONE);
//    changeActionWithName(0);
    plotActionIdx = action;
    plotActionMission = missionId;
    armature->setScaleX(flipX?-1:1);
    armature->getAnimation()->playWithIndex(action);
    
    armature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(Hero::plotAnimationEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void Hero::plotAnimationEvent(cocostudio::Armature *armature,
                              cocostudio::MovementEventType movementType,
                              const std::string& movementID)
{
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
}

void Hero::plotMove(float duration, float targetX, int missionId)
{
    plotMoveMission = missionId;
    auto move = MoveTo::create(duration, Vec2(targetX, GroundY));
    auto func = CallFunc::create(CC_CALLBACK_0(Hero::plotMoveEnd, this));
    auto seq = Sequence::create(move, func,  NULL);
    runAction(seq);
}

void Hero::plotMoveEnd()
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

void Hero::animEvent(Armature *armature, MovementEventType movementType, const std::string& movementID)
{
    if (movementType == MovementEventType::COMPLETE)
    {
        if (state == ACTORSTATE::SKILL) {
            log("animation end:%s",movementID.c_str());
            if(currDir != DIRECT::NONE)
            {
                log("enter skill run");
                if (reverseMove) {
                    int mask = 10;
                    currDir = currDir ^ mask;
                    speedX *= -1;
                }
                if (((currDir & DIRECT::LEFT)!=0) != flipX)
                {
                    flipX = !flipX;
                    float sx = blackActor->getScaleRate();
                    armature->setScaleX((flipX) ? -sx : sx);
                }
            }
        }
        
    }
}
