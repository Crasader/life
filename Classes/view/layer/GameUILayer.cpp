//
//  GameUILayer.cpp
//  Dabao
//
//  Created by ff on 17/3/4.
//
//
#include "ELProtocol.h"
#include "EventDefine.h"
#include "GameDefine.h"
#include "GameUILayer.h"
#include "utils/GameUtils.h"
#include "utils/TimeUtil.h"
#include "utils/StringData.h"
#include "TipsNode.h"
#include "logic/ClientLogic.h"
//#include "../node/EffectNode.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace ui;
using namespace CocosDenshion;

cocos2d::Color3B hpColor[6] = {
    cocos2d::Color3B(122, 4, 0),
    cocos2d::Color3B(219, 5, 3),
    cocos2d::Color3B(244, 129, 12),
    cocos2d::Color3B(140, 173, 9),
    cocos2d::Color3B(1, 123, 201),
    cocos2d::Color3B(112, 101, 155)
};
GameUILayer::GameUILayer():
plotButtonId(0),
plotMissionId(0),
plotHidePause(false),
plotHideTime(false)
{
    memset(cooling, 0, sizeof(int)*SKEY_MAX);
    memset(enable, 1, sizeof(int)*SKEY_MAX);
    ghost = 0;
    hpBarCount = hpDeepCount = 0;
    deepPercent = barPercent = 0;
    stageTime = 0;
    for (int i = 0; i < 4; i++) {
        opponentApNode[i] = nullptr;
        opponentApAction[i] = nullptr;
    }
}

GameUILayer::~GameUILayer()
{
    memset(cooling, 0, sizeof(int)*SKEY_MAX);
    ghost = 0;
    Director::getInstance()->getEventDispatcher()->removeEventListener(startCDListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateCDListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateSPCDListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateHeroHPListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateGhostListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showMonsterHpListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(hideMonsterHpListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateMonsterHpListener);
//    Director::getInstance()->getEventDispatcher()->removeEventListener(showWarningListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(readyVictoryListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateWaveCountListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateHpBottlePriceListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showOpponentInfoListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateComboListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(clearComboListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(switchEnableListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updatePetCountListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(startPlotListener);
    
    for (int i = 0; i < 4; i++) {
        opponentApNode[i] = nullptr;
        opponentApAction[i] = nullptr;
    }
}

bool GameUILayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto root = (Layer*)CSLoader::createNode(BATTLE_UI);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    addChild(root);
    
    hpBg = (ImageView*)root->getChildByName("boss_hp_bg");
    hpBar = (LoadingBar*)hpBg->getChildByName("boss_hp_bar");
    nameText = (Text*)hpBg->getChildByName("name_text");
    countText = (Text*)hpBg->getChildByName("count_text_0");
    hp2Img = (ImageView*)hpBg->getChildByName("boss_hp2_bg");
//    hpDeepImg = (ImageView*)hpBg->getChildByName("boss_hp_deep");
//    hpWhiteImg = (ImageView*)hpBg->getChildByName("boss_hp2_white");
    whiteBar = (LoadingBar*)hpBg->getChildByName("white_bar");
    deepBar = (LoadingBar*)hpBg->getChildByName("deep_bar");
    timeText = (TextAtlas*)root->getChildByName("time_text");
    hpBg->setVisible(false);
    hpBgPos = hpBg->getPosition();
    typeImg = (ImageView*)root->getChildByName("type_img");
    typeBg = (ImageView*)root->getChildByName("guanqia_1");
    
    buttonEffect = CSLoader::createNode(BUTTON_EFFECT);
    buttonEffectAction = CSLoader::createTimeline(BUTTON_EFFECT);
    buttonEffect->runAction(buttonEffectAction);
    buttonEffect->setVisible(false);
    addChild(buttonEffect);
    
    auto leftRoot = (Layer*)CSLoader::createNode(BATTLE_UI_LEFT);
    leftRoot->setPosition(Vec2(0, GameUtils::winSize.height/2));
    addChild(leftRoot);
    
    auto rightRoot = (Layer*)CSLoader::createNode(BATTLE_UI_RIGHT);
    rightRoot->setPosition(Vec2(GameUtils::winSize.width, GameUtils::winSize.height/2));
    addChild(rightRoot);
    
    waveCountText = (Text*)rightRoot->getChildByName("wave_count_text");
    waveCountText->setVisible(false);
    
    touchButtonId = -1;
    atkBg = (ImageView*)rightRoot->getChildByName("atk_bg");
    skill1Bg = (ImageView*)rightRoot->getChildByName("skill1_bg");
    skill2Bg = (ImageView*)rightRoot->getChildByName("skill2_bg");
    skill3Bg = (ImageView*)rightRoot->getChildByName("skill3_bg");
    holySwordBg = (ImageView*)rightRoot->getChildByName("soul_skill_bg");
    spBg = (ImageView*)rightRoot->getChildByName("spskill_bg");
    jumpBg = (ImageView*)rightRoot->getChildByName("jump_bg");
    petBg = (ImageView*)rightRoot->getChildByName("guard_skill_bg");
    atkSprite = (ImageView*)atkBg->getChildByName("atk_icon_image");
    skill1Sprite = (ImageView*)skill1Bg->getChildByName("skill_icon_image");
    skill2Sprite = (ImageView*)skill2Bg->getChildByName("skill_icon_image");
    skill3Sprite = (ImageView*)skill3Bg->getChildByName("skill_icon_image");
    spSprite = (ImageView*)spBg->getChildByName("spskill_icon_image");
    jumpSprite = (ImageView*)jumpBg->getChildByName("jump_icon_image");
    petSprite = (ImageView*)petBg->getChildByName("guard_icon_image");
    holySwordSprite = (ImageView*)holySwordBg->getChildByName("soul_icon_image");
    
    areaAtk = Rect(atkSprite->getPositionX()-atkSprite->getContentSize().width/2, atkSprite->getPositionY()-atkSprite->getContentSize().height/2, atkSprite->getContentSize().width, atkSprite->getContentSize().height);
    areaSkill1 = Rect(skill1Sprite->getPositionX()-skill1Sprite->getContentSize().width/2, skill1Sprite->getPositionY()-skill1Sprite->getContentSize().height/2, skill1Sprite->getContentSize().width, skill1Sprite->getContentSize().height);
    areaSkill2 = Rect(skill2Sprite->getPositionX()-skill2Sprite->getContentSize().width/2, skill2Sprite->getPositionY()-skill2Sprite->getContentSize().height/2, skill2Sprite->getContentSize().width, skill2Sprite->getContentSize().height);
    areaSkill3 = Rect(skill3Sprite->getPositionX()-skill3Sprite->getContentSize().width/2, skill3Sprite->getPositionY()-skill3Sprite->getContentSize().height/2, skill3Sprite->getContentSize().width, skill3Sprite->getContentSize().height);
    areaSP = Rect(spSprite->getPositionX()-spSprite->getContentSize().width/2, spSprite->getPositionY()-spSprite->getContentSize().height/2, spSprite->getContentSize().width, spSprite->getContentSize().height);
    areaJump = Rect(jumpSprite->getPositionX()-jumpSprite->getContentSize().width/2, jumpSprite->getPositionY()-jumpSprite->getContentSize().height/2, jumpSprite->getContentSize().width, jumpSprite->getContentSize().height);
    areaPet = Rect(petSprite->getPositionX()-petSprite->getContentSize().width/2, petSprite->getPositionY()-petSprite->getContentSize().height/2, petSprite->getContentSize().width, petSprite->getContentSize().height);
    areaSword = Rect(holySwordSprite->getPositionX()-holySwordSprite->getContentSize().width/2, holySwordSprite->getPositionY()-holySwordSprite->getContentSize().height/2, holySwordSprite->getContentSize().width, holySwordSprite->getContentSize().height);
    
    skill1CDBg = (ImageView*)skill1Bg->getChildByName("skill_cd");
    skill2CDBg = (ImageView*)skill2Bg->getChildByName("skill_cd");
    skill3CDBg = (ImageView*)skill3Bg->getChildByName("skill_cd");
    spCDBg = (ImageView*)spBg->getChildByName("sp_cd");
    holySwordCDBg = (ImageView*)holySwordBg->getChildByName("skill_cd");
    petCDBg = (ImageView*)petBg->getChildByName("skill_cd");
    
    skill1CDText = (TextAtlas*)skill1Bg->getChildByName("skill_text");
    skill2CDText = (TextAtlas*)skill2Bg->getChildByName("skill_text");
    skill3CDText = (TextAtlas*)skill3Bg->getChildByName("skill_text");
    holySwordCDText = (TextAtlas*)holySwordBg->getChildByName("skill_text");
    petCDText = (TextAtlas*)petBg->getChildByName("skill_text");
    
    ghost1Sprite = (ImageView*)skill3Bg->getChildByTag(1)->getChildByName("point");
    ghost2Sprite = (ImageView*)skill3Bg->getChildByTag(2)->getChildByName("point");
    ghost3Sprite = (ImageView*)skill3Bg->getChildByTag(3)->getChildByName("point");
    petCount1Sprite = (ImageView*)petBg->getChildByName(GameUtils::format("point_bg_%d",1))->getChildByName("point");
    petCount2Sprite = (ImageView*)petBg->getChildByName(GameUtils::format("point_bg_%d",2))->getChildByName("point");
    petCount3Sprite = (ImageView*)petBg->getChildByName(GameUtils::format("point_bg_%d",3))->getChildByName("point");
    
    skill1Disable = (ImageView*)skill1Bg->getChildByName("feng");
    skill2Disable = (ImageView*)skill2Bg->getChildByName("feng");
    spDisable = (ImageView*)spBg->getChildByName("feng");
    holySwordDisable = (ImageView*)holySwordBg->getChildByName("feng");
    petDisable = (ImageView*)petBg->getChildByName("feng");
    
    skill1Disable->setVisible(false);
    skill2Disable->setVisible(false);
    spDisable->setLocalZOrder(20);
    spDisable->setVisible(false);
    holySwordDisable->setVisible(false);
    petDisable->setVisible(false);
    
    skill1CDBg->setVisible(false);
    skill2CDBg->setVisible(false);
    skill3CDBg->setVisible(false);
    holySwordCDBg->setVisible(false);
    petCDBg->setVisible(false);
    spCDBg->setVisible(false);
    spSprite->setColor(Color3B::GRAY);
    
    ghost1Sprite->setVisible(false);
    ghost2Sprite->setVisible(false);
    ghost3Sprite->setVisible(false);
//    petCount1Sprite->setVisible(false);
//    petCount2Sprite->setVisible(false);
//    petCount3Sprite->setVisible(false);
    
    skill1CDText->setVisible(false);
    skill2CDText->setVisible(false);
    skill3CDText->setVisible(false);
    holySwordCDText->setVisible(false);
    petCDText->setVisible(false);
    
    skill1CDText->setLocalZOrder(10);
    skill2CDText->setLocalZOrder(10);
    skill3CDText->setLocalZOrder(10);
    holySwordCDText->setLocalZOrder(10);
    petCDText->setLocalZOrder(10);
    
//    opponentNode = rightRoot->getChildByName("Node_3");
//    opponentNode->setVisible(false);
    
    opponentIcon = (Sprite*)rightRoot->getChildByName("ricon_sprite");
    opponentIcon->setVisible(false);
    opponentHpBar = (LoadingBar*)opponentIcon->getChildByName("hero_bg")->getChildByName("hp_bar");
    for (int i = 0; i < 4; i++) {
        opponentApNode[i] = opponentIcon->getChildByName("hero_bg")->getChildByName("ap_bg")->getChildByTag(i+1);
        opponentApAction[i] = CSLoader::createTimeline("csb/battle/nuqi.csb");
        opponentApNode[i]->runAction(opponentApAction[i]);
        
        opponentApNode[i]->setVisible(false);
    }
    hpBottleButton = (Button*)leftRoot->getChildByName("hp_button");
    hpBottleButton->addClickEventListener(CC_CALLBACK_0(GameUILayer::clickHpBottole, this));
    pauseButton = (Button*)rightRoot->getChildByName("pause_button");
    pauseButton->addClickEventListener(CC_CALLBACK_0(GameUILayer::clickPause, this));
    
    apProgress = ProgressTimer::create(Sprite::create("ui/battle/spload.png"));
    spBg->addChild(apProgress);
    apProgress->setPosition(spCDBg->getPosition());
    apProgress->setPercentage(0);
    apProgress->setType(cocos2d::ProgressTimer::Type::BAR);
    //    Setup for a bar starting from the bottom since the midpoint is 0 for the y
    apProgress->setMidpoint(Vec2(0,0));
    //    Setup for a vertical bar since the bar change rate is 0 for x meaning no horizontal change
    apProgress->setBarChangeRate(Vec2(0, 1));
    apFullEffect = CSLoader::createNode(AP_FULL_EFFECT);
    apFullAction = CSLoader::createTimeline(AP_FULL_EFFECT);
    apFullEffect->runAction(apFullAction);
    spBg->addChild(apFullEffect);
    apFullEffect->setPosition(spCDBg->getPosition());
    apFullEffect->setLocalZOrder(10);
    apFullEffect->setVisible(false);
    apFullAction->setAnimationEndCallFunc("play1", CC_CALLBACK_0(GameUILayer::linkEffect2, this));

    heroIcon = (Sprite*)leftRoot->getChildByName("icon_sprite");
    heroHpBar = (LoadingBar*)heroIcon->getChildByName("hero_bg")->getChildByName("hp_bar");
    for (int i = 0; i < 4; i++) {
        heroApNode[i] = heroIcon->getChildByName("hero_bg")->getChildByName("ap_bg")->getChildByTag(i+1);
        heroApAction[i] = CSLoader::createTimeline("csb/battle/nuqi.csb");
        heroApNode[i]->runAction(heroApAction[i]);
        
        heroApNode[i]->setVisible(false);
    }
    hpBottleButton = (Button*)leftRoot->getChildByName("hp_button");
    hpBottleButton->addClickEventListener(CC_CALLBACK_0(GameUILayer::clickHpBottole, this));
    goalText = (Text*)leftRoot->getChildByName("goal_text");
    
    auto touchListener = EventListenerTouchOneByOne::create();
//    EventListenerTouchAllAtOnce::create();
    touchListener->setSwallowTouches(false);
    touchListener->onTouchBegan = CC_CALLBACK_2(GameUILayer::touchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(GameUILayer::touchEnded, this);
    touchListener->onTouchCancelled = CC_CALLBACK_2(GameUILayer::touchEnded, this);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    keyCmd = GAMEKEY::BUTTON_NONE;
    
    auto joySprite = (Sprite*)leftRoot->getChildByName("joystick_sprite");
    joySprite->setVisible(false);
    auto joyPos = joySprite->getPosition();
    
    joyStick = SneakyJoystick::create();
    joyStick->initWithRect(Rect(0, 0, 427,GameUtils::winSize.height), true);
    joyStick->setAutoCenter(true);
    joyStick->setHasDeadzone(true);
    joyStick->setDeadRadius(10);
    auto joySkin = SneakyJoystickSkinnedBase::create();
    joySkin->setBackgroundSprite(Sprite::create("ui/battle/joystick_0.png"));
    joySkin->setThumbSprite(Sprite::create("ui/battle/joystick_1.png"));
    joySkin->setPosition(Vec2(joyPos.x, joyPos.y+GameUtils::winSize.height/2));
    joySkin->setJoystick(joyStick);
    addChild(joySkin);
    
//    auto unlockBtn = Button::create("ui/zanting.png");
//    unlockBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    unlockBtn->setPosition(Vec2(GameUtils::winSize.width-100, GameUtils::winSize.height-200));
//    unlockBtn->addClickEventListener(CC_CALLBACK_0(GameUILayer::clickUnlock, this));
//    addChild(unlockBtn);
    
    comboRoot = CSLoader::createNode(COMBO_UI);
    comboAction = CSLoader::createTimeline(COMBO_UI);
    comboRoot->runAction(comboAction);
    comboRoot->setPosition(Vec2(95, GameUtils::winSize.height/2+94));
    addChild(comboRoot);
    comboRoot->setVisible(false);
    comboCountText = (TextAtlas*)comboRoot->getChildByName("Image_2")->getChildByName("AtlasLabel_5");
    comboFactorText = (TextAtlas*)comboRoot->getChildByName("Image_2")->getChildByName("AtlasLabel_4");
    comboBar = (LoadingBar*)comboRoot->getChildByName("Image_2")->getChildByName("Image_1")->getChildByName("LoadingBar_1");
    comboIdx = 0;
    
    pressJoy1 = false;
    pressDur[0] = 0;
    pressDur[1] = 0;
    releaseDur = 0;
    lastRushCmd = rushCmd = KEYCOMMAND::CMD_STOPMOVE;
    lastDir = DIRECT::NONE;
    clearKey();
    
    joyDir[0] = DIRECT::RIGHT;
    joyDir[1] = DIRECT::RIGHT | DIRECT::UP;
    joyDir[2] = DIRECT::UP;
    joyDir[3] = DIRECT::LEFT | DIRECT::UP;
    joyDir[4] = DIRECT::LEFT;
    joyDir[5] = DIRECT::LEFT | DIRECT::DOWN;
    joyDir[6] = DIRECT::DOWN;
    joyDir[7] = DIRECT::RIGHT | DIRECT::DOWN;
    waitResult = false;
    return true;
}

void GameUILayer::onEnter()
{
    Layer::onEnter();
    startCDListener = EventListenerCustom::create(START_SKILL_CD, CC_CALLBACK_1(GameUILayer::startCD, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(startCDListener, -1);
    updateCDListener = EventListenerCustom::create(UPDATE_SKILL_CD, CC_CALLBACK_1(GameUILayer::updateCD, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateCDListener, -1);
    updateSPCDListener = EventListenerCustom::create(UPDATE_AP, CC_CALLBACK_1(GameUILayer::updateAP, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateSPCDListener, -1);
    updateHeroHPListener = EventListenerCustom::create(UPDATE_HERO_HP, CC_CALLBACK_1(GameUILayer::updateHeroHP, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateHeroHPListener, -1);
    updateGhostListener = EventListenerCustom::create(UPDATE_GHOST, CC_CALLBACK_1(GameUILayer::updateGhost, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateGhostListener, -1);
    showMonsterHpListener = EventListenerCustom::create(SHOW_MONSTER_HP, CC_CALLBACK_1(GameUILayer::showMonsterHp, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showMonsterHpListener, -1);
    hideMonsterHpListener = EventListenerCustom::create(HIDE_MONSTER_HP, CC_CALLBACK_0(GameUILayer::hideMonsterHp, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(hideMonsterHpListener, -1);
    updateMonsterHpListener = EventListenerCustom::create(UPDATE_MONSTER_HP, CC_CALLBACK_1(GameUILayer::updateMonsterHp, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateMonsterHpListener, -1);
//    showWarningListener = EventListenerCustom::create(SHOW_WARNING, CC_CALLBACK_1(GameUILayer::showWarning, this));
//    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showWarningListener, -1);
    readyVictoryListener = EventListenerCustom::create(READY_VICTORY, CC_CALLBACK_0(GameUILayer::readyVictory, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(readyVictoryListener, -1);
    updateWaveCountListener = EventListenerCustom::create(STAGE_UPDATE_WAVE_COUNT, CC_CALLBACK_1(GameUILayer::updateWaveCount, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateWaveCountListener, -1);
    updateHpBottlePriceListener = EventListenerCustom::create(UPDATE_HP_BOTTLE_PRICE, CC_CALLBACK_1(GameUILayer::updateHpBottlePrice, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateHpBottlePriceListener, -1);
    showOpponentInfoListener = EventListenerCustom::create(SHOW_OPPONENT_HP, CC_CALLBACK_1(GameUILayer::showOpponentInfo, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showOpponentInfoListener, -1);
    updateOpponentHpListener = EventListenerCustom::create(UPDATE_OPPONENT_HP, CC_CALLBACK_1(GameUILayer::updateOpponentHP, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateOpponentHpListener, -1);
    updateOpponentApListener = EventListenerCustom::create(UPDATE_OPPONENT_AP, CC_CALLBACK_1(GameUILayer::updateOpponentAP, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateOpponentApListener, -1);
    updateComboListener = EventListenerCustom::create(UPDATE_COMBO, CC_CALLBACK_1(GameUILayer::updateCombo, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateComboListener, -1);
    clearComboListener = EventListenerCustom::create(CLEAR_COMBO, CC_CALLBACK_0(GameUILayer::clearCombo, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(clearComboListener, -1);
    switchEnableListener = EventListenerCustom::create(SWITCH_SKILL_ENABLE, CC_CALLBACK_1(GameUILayer::enableSkill, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(switchEnableListener, -1);
    updatePetCountListener = EventListenerCustom::create(UPDATE_PET_COUNT, CC_CALLBACK_1(GameUILayer::updatePetCount, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updatePetCountListener, -1);
    startPlotListener = EventListenerCustom::create(START_PLOT, CC_CALLBACK_1(GameUILayer::startPlot, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(startPlotListener, -1);
}

int GameUILayer::decodeJoyStick(float delta)
{
    if (waitResult) {
        return -1;
    }
    int dir = -1;
    if (joyStick->getPress()) {
        if (plotButtonId == 1) {
            plotButtonId = 0;
            joyStick->removeChildByTag(100);
            plotMissionId = 0;
        }
        
        //8方向
        float dirDegree = joyStick->getDegrees();
        dir = ((dirDegree - 22) / 45 + 1);
        
        if (pressJoy1) {
            //hold press
            switch (rushPart) {
                case 1://第一次持续按左右方向，则计时
                    pressDur[0] += delta;
                    if (pressDur[0] > 0.2) {
                        rushPart = 0;
                    }
                    break;
                case 3://左右方向下双击后的持续计时
                    pressDur[1] += delta;
                    if (pressDur[1] > 0.5) {
                        rushCmd = KEYCOMMAND::CMD_WALK;
                        rushPart = 0;
                        lastDir = DIRECT::NONE;
                    }
                    break;
                default:
                    break;
            }
            
        } else {
            //press down
            pressJoy1 = true;
            
            int currDir_X = joyDir[dir % 8];
            currDir_X &= ~DIRECT::UP;
            currDir_X &= ~DIRECT::DOWN;
            
            
            if (currDir_X == DIRECT::NONE) {//本次没有左右分量
                rushCmd = KEYCOMMAND::CMD_WALK;
                rushPart = 0;
            }else if (lastDir == currDir_X) {//同方向点两下,且必有左右分量
                switch (rushPart) {
                    case 0:
                        rushCmd = KEYCOMMAND::CMD_WALK;
                        rushPart = 1;
                        pressDur[0] = 0;
                        break;
                    case 2:
                        if (releaseDur <= 0.2) {
                            if (plotButtonId != 1) {
                                rushCmd = KEYCOMMAND::CMD_RUSH;
                                rushPart = 3;
                                pressDur[1] = 0;
                            }
                        }else{
                            rushCmd = KEYCOMMAND::CMD_WALK;
                            rushPart = 0;
                            lastDir = DIRECT::NONE;
                        }
                        break;
                        
                    default:
                        break;
                }
                
            }else{//首次确定左右
                if (plotButtonId == 1) {
                    joyStick->removeChildByTag(100);
                }

                rushCmd = KEYCOMMAND::CMD_WALK;
                rushPart = 1;
                pressDur[0] = 0;
            }
            
            lastDir = currDir_X;
        }
    } else {
        pressJoy1 = false;
        //松开左右
        if (lastDir != DIRECT::NONE) {
            switch (rushPart) {
                case 1://第一次点击后松开
                    releaseDur = 0;
                    rushPart = 2;
                    break;
                case 2://松开计时
                    releaseDur += delta;
                    if (releaseDur > 0.2) {
                        rushPart = 0;
                        lastDir = DIRECT::NONE;
                    }
                    break;
                case 3:
                {//已经开始冲刺时，松开按键应对其无影响
                    pressDur[1] += delta;
                    if (pressDur[1] > 0.5) {
                        rushPart = 0;
                        lastDir = DIRECT::NONE;
                    }
                }
                    return rushCmd;
                default:
                    break;
            }
        }
        rushCmd = KEYCOMMAND::CMD_STOPMOVE;
    }
    if (dir == -1) {
        return -1;
    }
    return joyDir[dir % 8];
}

void GameUILayer::clickUnlock()
{
    if (waitResult) {
        return;
    }

    E2L_COMMON info;
    info.eProtocol = e2l_click_unlock;
    
    ClientLogic::instance()->ProcessUIRequest(&info);
}

bool GameUILayer::touchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (plotButtonId == 1) {
        return true;
    }
    
    if(areaAtk.containsPoint(atkSprite->convertToNodeSpace(touch->getLocation())))
    {
        if (plotButtonId == 2) {
            atkBg->removeChildByTag(100);
            plotButtonId = 0;
        }
        touchButtonId = touch->getID();
        atkSprite->setScale(0.75);
        buttonEffect->removeFromParent();
        
        buttonEffect = CSLoader::createNode(BUTTON_EFFECT);
        buttonEffectAction = CSLoader::createTimeline(BUTTON_EFFECT);
        buttonEffect->runAction(buttonEffectAction);
        atkBg->addChild(buttonEffect);
        buttonEffect->setPosition(atkBg->getContentSize()/2);
        buttonEffect->setVisible(true);
        buttonEffectAction->play("play", false);
        buttonEffectAction->setAnimationEndCallFunc("play", CC_CALLBACK_0(GameUILayer::loopButtoneEffect, this));
//        keyCmd = CMD_JUMP;
        setCurrKey(GAMEKEY::BUTTON_A);
    }else if(areaSkill1.containsPoint(skill1Sprite->convertToNodeSpace(touch->getLocation()))) {
        if (plotButtonId == 3) {
            skill1Bg->removeChildByTag(100);
            plotButtonId = 0;
        }
        if (cooling[SKILLA_KEY] == 1) {
            return true;
        }
        if (enable[SKILLA_KEY] == 0) {
            return true;
        }


        touchButtonId = touch->getID();
        skill1Sprite->setScale(0.75);
        buttonEffect->removeFromParent();
        
        buttonEffect = CSLoader::createNode(BUTTON_EFFECT);
        buttonEffectAction = CSLoader::createTimeline(BUTTON_EFFECT);
        buttonEffect->runAction(buttonEffectAction);
        skill1Bg->addChild(buttonEffect);
        buttonEffect->setPosition(skill1Bg->getContentSize()/2);
        buttonEffect->setVisible(true);
        buttonEffectAction->play("play", false);
        buttonEffectAction->setAnimationEndCallFunc("play", CC_CALLBACK_0(GameUILayer::loopButtoneEffect, this));
        setCurrKey(GAMEKEY::BUTTON_S1);
    }else if(areaSkill2.containsPoint(skill2Sprite->convertToNodeSpace(touch->getLocation()))) {
        if (plotButtonId == 4) {
            skill2Bg->removeChildByTag(100);
            plotButtonId = 0;
        }
        if (cooling[SKILLB_KEY] == 1) {
            return true;
        }
        if (enable[SKILLB_KEY] == 0) {
            return true;
        }


        touchButtonId = touch->getID();
        skill2Sprite->setScale(0.75);
        buttonEffect->removeFromParent();
        
        buttonEffect = CSLoader::createNode(BUTTON_EFFECT);
        buttonEffectAction = CSLoader::createTimeline(BUTTON_EFFECT);
        buttonEffect->runAction(buttonEffectAction);
        skill2Bg->addChild(buttonEffect);
        buttonEffect->setPosition(skill2Bg->getContentSize()/2);
        buttonEffect->setVisible(true);
        buttonEffectAction->play("play", false);
        buttonEffectAction->setAnimationEndCallFunc("play", CC_CALLBACK_0(GameUILayer::loopButtoneEffect, this));
        setCurrKey(GAMEKEY::BUTTON_S2);
    }else if(areaSkill3.containsPoint(skill3Sprite->convertToNodeSpace(touch->getLocation()))) {
        if (cooling[SKILLC_KEY] == 1) {
            return true;
        }
        if (ghost == 0) {
            return true;
        }
        
        touchButtonId = touch->getID();
        skill3Sprite->setScale(0.75);
        buttonEffect->removeFromParent();
        
        buttonEffect = CSLoader::createNode(BUTTON_EFFECT);
        buttonEffectAction = CSLoader::createTimeline(BUTTON_EFFECT);
        buttonEffect->runAction(buttonEffectAction);
        skill3Bg->addChild(buttonEffect);
        buttonEffect->setPosition(skill3Bg->getContentSize()/2);
        buttonEffect->setVisible(true);
        buttonEffectAction->play("play", false);
        buttonEffectAction->setAnimationEndCallFunc("play", CC_CALLBACK_0(GameUILayer::loopButtoneEffect, this));
        setCurrKey(GAMEKEY::BUTTON_S3);
    }else if(areaSP.containsPoint(spSprite->convertToNodeSpace(touch->getLocation()))) {
        if (plotButtonId == 5) {
            spBg->removeChildByTag(100);
            plotButtonId = 0;
        }
        if (cooling[SKILLS_KEY] == 0) {
            return true;
        }
        if (enable[SKILLS_KEY] == 0) {
            return true;
        }

        touchButtonId = touch->getID();
        spSprite->setScale(0.75);
        buttonEffect->removeFromParent();
        
        buttonEffect = CSLoader::createNode(BUTTON_EFFECT);
        buttonEffectAction = CSLoader::createTimeline(BUTTON_EFFECT);
        buttonEffect->runAction(buttonEffectAction);
        spBg->addChild(buttonEffect);
        buttonEffect->setPosition(spBg->getContentSize()/2);
        buttonEffect->setVisible(true);
        buttonEffectAction->play("play", false);
        buttonEffectAction->setAnimationEndCallFunc("play", CC_CALLBACK_0(GameUILayer::loopButtoneEffect, this));
        setCurrKey(GAMEKEY::BUTTON_SP);
    }else if(areaSword.containsPoint(holySwordSprite->convertToNodeSpace(touch->getLocation()))) {
        if (cooling[HOLY_SKILL_KEY] == 1) {
            return true;
        }
        if (enable[HOLY_SKILL_KEY] == 0) {
            return true;
        }
        
        touchButtonId = touch->getID();
        holySwordSprite->setScale(0.75);
        buttonEffect->removeFromParent();
        
        buttonEffect = CSLoader::createNode(BUTTON_EFFECT);
        buttonEffectAction = CSLoader::createTimeline(BUTTON_EFFECT);
        buttonEffect->runAction(buttonEffectAction);
        holySwordBg->addChild(buttonEffect);
        buttonEffect->setPosition(holySwordBg->getContentSize()/2);
        buttonEffect->setVisible(true);
        buttonEffectAction->play("play", false);
        buttonEffectAction->setAnimationEndCallFunc("play", CC_CALLBACK_0(GameUILayer::loopButtoneEffect, this));
        setCurrKey(GAMEKEY::BUTTON_HOLY_SWORD);
    }else if(areaPet.containsPoint(petSprite->convertToNodeSpace(touch->getLocation()))) {
        if (cooling[PET_SKILL1_KEY] == 1) {
            return true;
        }
        if (enable[PET_SKILL1_KEY] == 0) {
            return true;
        }
        touchButtonId = touch->getID();
        petSprite->setScale(0.75);
        buttonEffect->removeFromParent();
        
        buttonEffect = CSLoader::createNode(BUTTON_EFFECT);
        buttonEffectAction = CSLoader::createTimeline(BUTTON_EFFECT);
        buttonEffect->runAction(buttonEffectAction);
        petBg->addChild(buttonEffect);
        buttonEffect->setPosition(petBg->getContentSize()/2);
        buttonEffect->setVisible(true);
        buttonEffectAction->play("play", false);
        buttonEffectAction->setAnimationEndCallFunc("play", CC_CALLBACK_0(GameUILayer::loopButtoneEffect, this));
        setCurrKey(GAMEKEY::BUTTON_Y);

    }else if(areaJump.containsPoint(jumpSprite->convertToNodeSpace(touch->getLocation()))) {
        touchButtonId = touch->getID();
        jumpSprite->setScale(0.75);
        buttonEffect->removeFromParent();
        
        buttonEffect = CSLoader::createNode(BUTTON_EFFECT);
        buttonEffectAction = CSLoader::createTimeline(BUTTON_EFFECT);
        buttonEffect->runAction(buttonEffectAction);
        jumpBg->addChild(buttonEffect);
        buttonEffect->setPosition(jumpBg->getContentSize()/2);
        buttonEffect->setVisible(true);
        buttonEffectAction->play("play", false);
        buttonEffectAction->setAnimationEndCallFunc("play", CC_CALLBACK_0(GameUILayer::loopButtoneEffect, this));
        keyCmd = BUTTON_JUMP;
        //        setCurrKey(GAMEKEY::BUTTON_A);
    }
    return true;
}

void GameUILayer::touchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (touch->getID() != touchButtonId) {
        return;
    }
    const Rect area(GameUtils::winSize.width/2, 0, GameUtils::winSize.width/2, GameUtils::winSize.height);
    if(area.containsPoint(touch->getLocation()))
    {
        atkSprite->setScale(1);
        skill1Sprite->setScale(1);
        skill2Sprite->setScale(1);
        skill3Sprite->setScale(1);
        jumpSprite->setScale(1);
        spSprite->setScale(1);
        petSprite->setScale(1);
        holySwordSprite->setScale(1);
//        buttonEffect->setVisible(false);
        buttonEffectAction->clearFrameEndCallFuncs();
        keyCmd = BUTTON_NONE;
        clearKey();
    }
}

void GameUILayer::loopButtoneEffect()
{
    buttonEffectAction->play("play", false);
}

bool GameUILayer::setCurrKey(int key)
{
    if (waitResult) {
        return false;
    }

    if (currKey != 0)
    {
        //没松开任何一个键时，不允许再处理另外一个按键
        return false;
    }
    currKey = key;
//    if (Guard != nullptr) {
//        (dynamic_cast<Guardian *>(Guard))->setCurrKey(currKey);
//    }
    return true;
}

void GameUILayer::clearKey()
{
    currKey = 0;
    
//    atkSprite->setScale(1);
//    skill1Sprite->setScale(1);
//    skill2Sprite->setScale(1);
//    skill3Sprite->setScale(1);
//    jumpSprite->setScale(1);
//    spSprite->setScale(1);
//    petSprite->setScale(1);
//    holySwordSprite->setScale(1);
//    //        buttonEffect->setVisible(false);
//    buttonEffectAction->clearFrameEndCallFuncs();
//    keyCmd = BUTTON_NONE;

}

void GameUILayer::startCD(cocos2d::EventCustom *event)
{
    SKILL_CD_DATA info = *static_cast<SKILL_CD_DATA*>(event->getUserData());
    switch(info.skillBtnIdx)
    {
        case SKILLA_KEY:
        {
            skill1Sprite->setColor(Color3B::GRAY);
            skill1CDBg->setVisible(false);
            skill1CDText->setVisible(true);
            skill1CDText->setString(Convert2String((int)info.duration));
            stopActionByTag(info.skillBtnIdx);
//            auto func = CallFunc::create(CC_CALLBACK_0(GameUILayer::endCD, this, info->skillBtnIdx));
            auto to1 = Sequence::createWithTwoActions(ProgressTo::create(0, 100), ProgressTo::create(info.duration, 0));
            to1->setTag(info.skillBtnIdx);
            auto timeLeft = ProgressTimer::create(Sprite::create("ui/battle/skillload.png"));
            timeLeft->setType( ProgressTimer::Type::RADIAL);
            timeLeft->setTag(info.skillBtnIdx+1000);
            timeLeft->setReverseDirection(true);
            skill1Sprite->addChild(timeLeft);
            timeLeft->setPosition(skill1Sprite->getContentSize()/2);
            timeLeft->runAction(to1);
        }
            break;
        case SKILLB_KEY:
        {
            skill2Sprite->setColor(Color3B::GRAY);
            skill2CDBg->setVisible(false);
            skill2CDText->setVisible(true);
            skill2CDText->setString(Convert2String((int)info.duration));
//            auto func = CallFunc::create(CC_CALLBACK_0(GameUILayer::endCD, this, info->skillBtnIdx));
            auto to1 = Sequence::createWithTwoActions(ProgressTo::create(0, 100), ProgressTo::create(info.duration, 0));
            to1->setTag(info.skillBtnIdx);
            auto timeLeft = ProgressTimer::create(Sprite::create("ui/battle/skillload.png"));
            timeLeft->setType( ProgressTimer::Type::RADIAL);
            timeLeft->setTag(info.skillBtnIdx+1000);
            timeLeft->setReverseDirection(true);
            skill2Sprite->addChild(timeLeft);
            timeLeft->setPosition(skill2Sprite->getContentSize()/2);
            timeLeft->runAction(to1);
            
        }
            break;
        case SKILLC_KEY:
        {
            if (ghost <= 0) {
                break;
            }
            skill3Sprite->setColor(Color3B::GRAY);
            skill3CDBg->setVisible(false);
            skill3CDText->setVisible(true);
            skill3CDText->setString(Convert2String((int)info.duration));
            //            auto func = CallFunc::create(CC_CALLBACK_0(GameUILayer::endCD, this, info->skillBtnIdx));
            auto to1 = Sequence::createWithTwoActions(ProgressTo::create(0, 100), ProgressTo::create(info.duration, 0));
            to1->setTag(info.skillBtnIdx);
            auto timeLeft = ProgressTimer::create(Sprite::create("ui/battle/skillload.png"));
            timeLeft->setType( ProgressTimer::Type::RADIAL);
            timeLeft->setTag(info.skillBtnIdx+1000);
            timeLeft->setReverseDirection(true);
            skill3Sprite->addChild(timeLeft);
            timeLeft->setPosition(skill3Sprite->getContentSize()/2);
            timeLeft->runAction(to1);
            
        }
            break;
        case HOLY_SKILL_KEY:
        {
            holySwordSprite->setColor(Color3B::GRAY);
            holySwordCDBg->setVisible(false);
            holySwordCDText->setVisible(true);
            holySwordCDText->setString(Convert2String((int)info.duration));
            //            auto func = CallFunc::create(CC_CALLBACK_0(GameUILayer::endCD, this, info->skillBtnIdx));
            auto to1 = Sequence::createWithTwoActions(ProgressTo::create(0, 100), ProgressTo::create(info.duration, 0));
            to1->setTag(info.skillBtnIdx);
            auto timeLeft = ProgressTimer::create(Sprite::create("ui/battle/skillload.png"));
            timeLeft->setType( ProgressTimer::Type::RADIAL);
            timeLeft->setTag(info.skillBtnIdx+1000);
            timeLeft->setReverseDirection(true);
            holySwordSprite->addChild(timeLeft);
            timeLeft->setPosition(holySwordSprite->getContentSize()/2);
            timeLeft->runAction(to1);

        }
            break;
        case PET_SKILL1_KEY:
        case PET_SKILL2_KEY:
        case PET_SKILL3_KEY:
        {
            petSprite->setColor(Color3B::GRAY);
            petCDBg->setVisible(false);
            petCDText->setVisible(true);
            petCDText->setString(Convert2String((int)info.duration));
            //            auto func = CallFunc::create(CC_CALLBACK_0(GameUILayer::endCD, this, info->skillBtnIdx));
            auto to1 = Sequence::createWithTwoActions(ProgressTo::create(0, 100), ProgressTo::create(info.duration, 0));
            to1->setTag(info.skillBtnIdx);
            auto timeLeft = ProgressTimer::create(Sprite::create("ui/battle/skillload.png"));
            timeLeft->setType(ProgressTimer::Type::RADIAL);
            timeLeft->setTag(PET_SKILL1_KEY+1000);
            timeLeft->setReverseDirection(true);
            petSprite->addChild(timeLeft);
            timeLeft->setPosition(petSprite->getContentSize()/2);
            timeLeft->runAction(to1);
            cooling[PET_SKILL1_KEY] = 1;
        }
            break;

    }
    cooling[info.skillBtnIdx] = 1;
}

void GameUILayer::endCD(int key)
{
    switch(key)
    {
        case SKILLA_KEY:
        {
            skill1Sprite->setColor(Color3B::WHITE);
            skill1CDBg->setVisible(false);
            skill1CDText->setVisible(false);
            skill1CDText->setString("");
            skill1Sprite->removeChildByTag(key+1000);
            
        }
            break;
        case SKILLB_KEY:
        {
            skill2Sprite->setColor(Color3B::WHITE);
            skill2CDBg->setVisible(false);
            skill2CDText->setVisible(false);
            skill2CDText->setString("");
            skill2Sprite->removeChildByTag(key+1000);
        }
            break;
        case SKILLC_KEY:
        {
            skill3Sprite->setColor(Color3B::WHITE);
            skill3CDBg->setVisible(false);
            skill3CDText->setVisible(false);
            skill3CDText->setString("");
            skill3Sprite->removeChildByTag(key+1000);
        }
            break;
        case HOLY_SKILL_KEY:
        {
            holySwordSprite->setColor(Color3B::WHITE);
            holySwordCDBg->setVisible(false);
            holySwordCDText->setVisible(false);
            holySwordCDText->setString("");
            holySwordSprite->removeChildByTag(key+1000);
        }
            break;
        case PET_SKILL1_KEY:
        case PET_SKILL2_KEY:
        case PET_SKILL3_KEY:
        {
            petSprite->setColor(Color3B::WHITE);
            petCDBg->setVisible(false);
            petCDText->setVisible(false);
            petCDText->setString("");
            petSprite->removeChildByTag(PET_SKILL1_KEY+1000);
            cooling[PET_SKILL1_KEY] = 0;
        }
            return;
    }
    cooling[key] = 0;
}

void GameUILayer::updateCD(cocos2d::EventCustom *event)
{
    SKILL_CD_UPDATE info = *static_cast<SKILL_CD_UPDATE*>(event->getUserData());
    if (info.duration == 0) {
        endCD(info.skillBtnIdx);
        return;
    }
    
    switch(info.skillBtnIdx)
    {
        case SKILLA_KEY:
        {
            skill1CDText->setString(Convert2String(info.duration));
        }
            break;
        case SKILLB_KEY:
        {
            skill2CDText->setString(Convert2String(info.duration));
        }
            break;
        case SKILLC_KEY:
        {
            skill3CDText->setString(Convert2String(info.duration));
        }
            break;
        case HOLY_SKILL_KEY:
        {
            holySwordCDText->setString(Convert2String(info.duration));
        }
            break;
        case PET_SKILL1_KEY:
        case PET_SKILL2_KEY:
        case PET_SKILL3_KEY:
        {
            petCDText->setString(Convert2String(info.duration));
        }
            break;
    }
}
void GameUILayer::updateGhost(cocos2d::EventCustom *event)
{
    if (waitResult) {
        return;
    }

    GHOST_UPDATE info = *static_cast<GHOST_UPDATE*>(event->getUserData());
    ghost = info.curr;
    for (int i = 1; i<=3; i++) {
        bool useful = ghost>=i;
        ((ImageView*)skill3Bg->getChildByTag(i)->getChildByName("point"))->setVisible(useful);
    }
    if (info.curr < 1) {
        skill3Sprite->setColor(Color3B::GRAY);
        skill3CDBg->setVisible(false);
        skill3CDText->setVisible(false);
    }else{
        skill3Sprite->setColor(Color3B::WHITE);
//        skill3CDBg->setVisible(true);
//        skill3CDText->setVisible(true);
    }
}
void GameUILayer::updateAP(cocos2d::EventCustom *event)
{
    if (waitResult) {
        return;
    }

    AP_UPDATE info = *static_cast<AP_UPDATE*>(event->getUserData());
//    apProgress->stopAllActions();
    auto to1 = ProgressTo::create(0.5, info.curr*100/info.max);
    apProgress->runAction(to1);
    spSprite->setColor(Color3B::GRAY);
    if (info.curr != info.max){
        apFullEffect->setVisible(false);
        apFullAction->stop();
    }
    if (info.curr <= 0) {
//        spCDBg->setVisible(false);
        for (int i = 0; i < 3; i++) {
            heroApNode[i]->setVisible(false);
            heroApAction[i]->stop();
        }
        
        cooling[SKILLS_KEY] = 0;
        return;
    }
    
    if (info.curr == info.max) {
//        spCDBg->setVisible(false);
//        apProgress->setVisible(false);
//        if (enable[SKILLS_KEY] == 1) {
            spSprite->setColor(Color3B::WHITE);
            apFullEffect->setVisible(true);
            apFullAction->play("play1", false);
            cooling[SKILLS_KEY] = 1;
            for (int i = 0; i < 3; i++) {
                heroApNode[i]->setVisible(true);
                heroApAction[i]->play("play2", true);
            }
//        }
        return;
    }
    for (int i = 1; i <= 3; i++) {
        if (info.curr-i <= 0) {
            break;
        }
        heroApNode[info.curr-i]->setVisible(true);
        heroApAction[info.curr-i]->stop();
        heroApAction[info.curr-i]->play("play", false);
    }
    
}

void GameUILayer::updateOpponentAP(cocos2d::EventCustom *event)
{
    if (waitResult) {
        return;
    }
    
    AP_UPDATE info = *static_cast<AP_UPDATE*>(event->getUserData());
    if (info.curr <= 0) {
        //        spCDBg->setVisible(false);
        for (int i = 0; i < 3; i++) {
            opponentApNode[i]->setVisible(false);
            opponentApAction[i]->stop();
        }
        return;
    }
    if (info.curr == info.max) {
        for (int i = 0; i < 3; i++) {
            opponentApNode[i]->setVisible(true);
            opponentApAction[i]->play("play2", true);
        }
        return;
    }
    
    opponentApNode[info.curr-1]->setVisible(true);
    opponentApAction[info.curr-1]->stop();
    opponentApAction[info.curr-1]->play("play", false);
}

void GameUILayer::showMonsterHp(cocos2d::EventCustom *event)
{
    SHOW_MONSTER_HP_DATA data = *static_cast<SHOW_MONSTER_HP_DATA*>(event->getUserData());
    hpBg->setVisible(true);
    nameText->setString(data.name);
    hpPerBar = data.maxHp/data.count;
    lastBarHp = hpPerBar + data.maxHp%hpPerBar;
    barPercent = calHpPercent(data.currHp);
    
    setDeepCount(hpBarCount);
    deepPercent = barPercent;
    deepBar->setPercent(barPercent);
    whiteBar->setVisible(false);
    whiteBar->setPercent(deepPercent);
//    Color3B color(165,0,0);
//    auto hsv = GameUtils::RGB2HSV(color);
//    hsv.z -= 30;
//    hsv.z = MAX(0, hsv.z);
//    auto color2 = GameUtils::HSV2RGB(hsv);
//    auto hsv = RedHSV+Vec3((hpBarCount-1)/2*30+(hpBarCount+1)%2*180, 0, 0);
//    hsv.x = (int)hsv.x % 360;
//    auto color = GameUtils::HSV2RGB(hsv);
//    hpBar->setColor(color);
//    countText->setString(Convert2String(hpBarCount));
    
    hpBar->setPercent(barPercent);
    
}

float GameUILayer::calHpPercent(int currHp)
{
    float barPercent = 0;
    
    if (currHp <= lastBarHp) {
        setBarCount(1);
        barPercent = currHp*100/(float)lastBarHp;
    }else{
        float hp = (currHp-lastBarHp)%hpPerBar;
        barPercent = (hp==0)?100:(hp*100/hpPerBar);
        setBarCount((currHp-lastBarHp)/hpPerBar+2-((hp==0)?1:0));
    }
    return barPercent;
}
void GameUILayer::setDeepCount(int count)
{
//    if (hpDeepCount == count) {
//        return;
//    }
    hpDeepCount = count;
    
    int barCount = count -1;
    barCount %= 6;
    deepBar->setColor(hpColor[barCount]);
}
void GameUILayer::setBarCount(int count)
{
    if (hpBarCount == count) {
        return;
    }
    
//    hpDeepImg->setVisible(false);
//    hpWhiteImg->setVisible(false);
//    deepBar->setVisible(false);
//    whiteBar->setVisible(false);
    int bgCount = count-2;
    int barCount = count -1;
    bgCount %= 6;
    barCount %= 6;
    if (count <= 1) {
        hp2Img->setVisible(false);
    }else{
        hp2Img->setVisible(true);
//        auto hsv = RedHSV+Vec3((count-2)/2*80+(count)%2*180, 0, 0);
//        hsv.x = (int)hsv.x % 360;
//        auto color = GameUtils::HSV2RGB(hsv);
        hp2Img->setColor(hpColor[bgCount]);
    }
//    auto hsv = RedHSV+Vec3((count-1)/2*80+(count+1)%2*180, 0, 0);
//    hsv.x = (int)hsv.x % 360;
//    auto color = GameUtils::HSV2RGB(hsv);
    
    hpBarCount = count;
    hpBar->setColor(hpColor[barCount]);
    countText->setString(Convert2String(hpBarCount));
    

}
void GameUILayer::hideMonsterHp()
{
    hpBg->setVisible(false);
}
void GameUILayer::updateMonsterHp(cocos2d::EventCustom *event)
{
    UPDATE_MONSTER_HP_DATA data = *static_cast<UPDATE_MONSTER_HP_DATA*>(event->getUserData());
    barPercent = calHpPercent(data.currHp);
//    auto change = ProgressTo::create(0.5, barPercent);
//    hpBar->runAction(change);
    hpBar->setPercent(barPercent);
    hpBg->stopActionByTag(99);
    
    auto startFunc = CallFunc::create(CC_CALLBACK_0(GameUILayer::startHp, this));
    auto move1 = MoveBy::create(0.01, Vec2(2, -10));
    auto move2 = MoveBy::create(0.01, Vec2(-10, 10));
    auto move3 = MoveBy::create(0.01, Vec2(10,20));
    auto move4 = MoveTo::create(0.01, hpBgPos);
    auto endFunc = CallFunc::create(CC_CALLBACK_0(GameUILayer::endHp, this));
    auto seq = Sequence::create(startFunc, move1, move2, move3, move4, endFunc, NULL);
    seq->setTag(99);
    hpBg->runAction(seq);
}

void GameUILayer::startHp()
{
    if (hpDeepCount == hpBarCount) {
        deepBar->setPercent(barPercent);
    }else{
        deepBar->setPercent(0);
    }
    
    whiteBar->setVisible(true);
}

void GameUILayer::endHp()
{
    setDeepCount(hpBarCount);
    deepPercent = barPercent;
    deepBar->setPercent(deepPercent);
    whiteBar->setPercent(deepPercent);
    whiteBar->setVisible(false);
}

void GameUILayer::linkEffect2()
{
    apFullAction->play("play2", true);
}

void GameUILayer::setTime(float total)
{
    stageTime = total;
    timeText->setString(TimeUtil::timeFormatToMS(stageTime));
}

void GameUILayer::updateWaveCount(cocos2d::EventCustom *event)
{
    L2E_UPDATE_WAVE_COUNT info = *static_cast<L2E_UPDATE_WAVE_COUNT*>(event->getUserData());
    showWaveCount(info.curr, info.total);
}
void GameUILayer::showWaveCount(int curr, int total)
{
    waveCountText->setVisible(true);
    waveCountText->setString(StringUtils::format(WAVE_TEXT, curr, total));
}

bool GameUILayer::countDownTime(float dt)
{
    if (waitResult) {
        return false;
    }
    if (plotHideTime) {
        return false;
    }

    if (stageTime <= 0) {
        return true;
    }
    stageTime -= dt;
    stageTime = MAX(stageTime, 0);
    std::string timeStr = TimeUtil::timeFormatToMS(stageTime);
    timeStr.replace(2, 1, "/");
    timeText->setString(timeStr);
    if (stageTime <= 0) {
        E2L_COMMON info;
        info.eProtocol = e2l_time_over;
        ClientLogic::instance()->ProcessUIRequest(&info);
    }
    return false;
}

void GameUILayer::readyVictory()
{
    waitResult = true;
    keyCmd = BUTTON_NONE;
    lastRushCmd = rushCmd = KEYCOMMAND::CMD_STOPMOVE;
    lastDir = DIRECT::NONE;
    clearKey();
}

void GameUILayer::setHeroJob(int job)
{
    heroIcon->setTexture(StringUtils::format(HEAD_ICON_FILE, job));
}

void GameUILayer::setGoalText(std::string goalStr)
{
    goalText->setString(goalStr);
}

void GameUILayer::setOpponentJob(int job)
{
    opponentIcon->setTexture(StringUtils::format(HEAD_ICON_FILE, job));
}

void GameUILayer::updateHeroHP(cocos2d::EventCustom *event)
{
    HP_UPDATE data = *static_cast<HP_UPDATE*>(event->getUserData());
    heroHpBar->setPercent(data.curr*100.0/data.max);
}

void GameUILayer::updateOpponentHP(cocos2d::EventCustom *event)
{
    HP_UPDATE data = *static_cast<HP_UPDATE*>(event->getUserData());
    opponentHpBar->setPercent(data.curr*100.0/data.max);
}

void GameUILayer::clickHpBottole()
{
    if (heroHpBar->getPercent() >= 100 || heroHpBar->getPercent() <= 0) {
        return;
    }
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_use_hp_bottle;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void GameUILayer::updateHpBottlePrice(cocos2d::EventCustom *event)
{
    L2E_UPDATE_HP_BOTTLE_PRICE info = *static_cast<L2E_UPDATE_HP_BOTTLE_PRICE*>(event->getUserData());
    if (info.errNo > 0) {
        std::string errStr = StringData::shared()->stringFromKey("diamond_not_enough");
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        
        return;
    }
    auto priceText = (Text*)hpBottleButton->getChildByName("Text_2");
    priceText->setString(Convert2String(info.price));
}

void GameUILayer::setupByType(int battleType, bool normal)
{
    std::string icon = GameUtils::format("type%d.png", battleType);
    switch (battleType) {
        case 1:
        case 2:
        case 3:
        {
            opponentIcon->setVisible(false);
            hpBottleButton->setVisible(true);
            waveCountText->setVisible(true);
            timeText->setVisible(true);
            pauseButton->setVisible(true);
            if (normal) {
                typeImg->loadTexture(GameUtils::format(BATTLE_DIR, icon.c_str()));
                typeImg->setVisible(true);
                typeBg->setVisible(true);
            }else{
                typeImg->setVisible(false);
                typeBg->setVisible(false);
            }
            
        }
            break;
        case 4:
        {
            opponentIcon->setVisible(false);
            hpBottleButton->setVisible(true);
            waveCountText->setVisible(false);
            timeText->setVisible(true);
            pauseButton->setVisible(true);
            if (normal) {
                typeImg->loadTexture(GameUtils::format(BATTLE_DIR, icon.c_str()));
                typeImg->setVisible(true);
                typeBg->setVisible(true);
            }else{
                typeImg->setVisible(false);
                typeBg->setVisible(false);
            }
        }
            break;
        case 5:
        {
            opponentIcon->setVisible(false);
            hpBottleButton->setVisible(true);
            waveCountText->setVisible(false);
            timeText->setVisible(false);
            pauseButton->setVisible(true);
            if (normal) {
                typeImg->loadTexture(GameUtils::format(BATTLE_DIR, icon.c_str()));
                typeImg->setVisible(true);
                typeBg->setVisible(true);
            }else{
                typeImg->setVisible(false);
                typeBg->setVisible(false);
            }
        }
            break;
        case 6:
        {
            opponentIcon->setVisible(false);
            hpBottleButton->setVisible(false);
            waveCountText->setVisible(false);
            timeText->setVisible(true);
            pauseButton->setVisible(false);
            typeImg->setVisible(false);
            typeBg->setVisible(false);
        }
            break;
            
        default:
            break;
    }
    if (plotHidePause) {
        pauseButton->setVisible(false);
    }
    if (plotHideTime) {
        timeText->setVisible(false);
    }
}

void GameUILayer::showOpponentInfo(cocos2d::EventCustom *event)
{
    SHOW_OPPONENT_HP_DATA data = *static_cast<SHOW_OPPONENT_HP_DATA*>(event->getUserData());
    opponentIcon->setVisible(true);
    setOpponentJob(data.job);
    opponentHpBar->setPercent(data.currHp*100.0/data.maxHp);
    if (data.currAp <= 0) {
        //        spCDBg->setVisible(false);
        for (int i = 0; i < 3; i++) {
            opponentApNode[i]->setVisible(false);
            opponentApAction[i]->stop();
        }
        return;
    }
    if (data.currAp == 4) {
        for (int i = 0; i < 3; i++) {
            opponentApNode[i]->setVisible(true);
            opponentApAction[i]->play("play2", true);
        }
        return;
    }
    
    opponentApNode[data.currAp-1]->setVisible(true);
    opponentApAction[data.currAp-1]->stop();
    opponentApAction[data.currAp-1]->play("play", false);
}

void GameUILayer::updateCombo(cocos2d::EventCustom *event)
{
    comboRoot->setVisible(true);
    comboAction->play(GameUtils::format("play%d",comboIdx+1), false);
    comboIdx++;
    comboIdx %= 4;
    log("comboIdx=%d",comboIdx);
    L2E_ADD_COMBO info = *static_cast<L2E_ADD_COMBO*>(event->getUserData());
    comboCountText->setString(Convert2String(info.count));
//    comboFactorText->setString(Convert2String(info.factor)+"%");
    comboFactorText->setString(Convert2String(info.factor));
    float percent = info.curr*100.0/info.max;
    comboBar->setPercent(percent);
}

void GameUILayer::clearCombo()
{
    comboRoot->setVisible(false);
    comboIdx = 0;
}

void GameUILayer::enableSkill(cocos2d::EventCustom *event)
{
    SWITCH_SKILL_ENABLE_DATA data = *static_cast<SWITCH_SKILL_ENABLE_DATA*>(event->getUserData());
    enable[data.skillKey] = (data.enable?1:0);
    switch (data.skillKey) {
        case SKILLA_KEY:
        {
            skill1Disable->setVisible(!data.enable);
            skill1CDBg->setVisible(!data.enable);
        }
            break;
        case SKILLB_KEY:
        {
            skill2Disable->setVisible(!data.enable);
            skill2CDBg->setVisible(!data.enable);
        }
            break;
        case SKILLS_KEY:
        {
            spDisable->setVisible(!data.enable);
            spCDBg->setVisible(!data.enable);
        }
            break;
        case HOLY_SKILL_KEY:
        {
            holySwordDisable->setVisible(!data.enable);
            holySwordCDBg->setVisible(!data.enable);
        }
            break;
        case PET_SKILL1_KEY:
        case PET_SKILL2_KEY:
        case PET_SKILL3_KEY:
        {
            petDisable->setVisible(!data.enable);
            petCDBg->setVisible(!data.enable);
            enable[PET_SKILL1_KEY] = (data.enable?1:0);
        }
        default:
            break;
    }
}

void GameUILayer::clickPause()
{
    auto skill1Action = (ProgressTimer*)skill1Bg->getChildByTag(SKILLA_KEY+1000);
    if (skill1Action) {
        skill1Action->pause();
    }
    auto skill2Action = (ProgressTimer*)skill2Bg->getChildByTag(SKILLB_KEY+1000);
    if (skill2Action) {
        skill2Action->pause();
    }
    auto skill3Action = (ProgressTimer*)skill3Bg->getChildByTag(SKILLC_KEY+1000);
    if (skill3Action) {
        skill3Action->pause();
    }
    auto holySwordAction = (ProgressTimer*)holySwordBg->getChildByTag(HOLY_SKILL_KEY+1000);
    if (holySwordAction) {
        holySwordAction->pause();
    }
    auto petAction = (ProgressTimer*)petBg->getChildByTag(PET_SKILL1_KEY+1000);
    if (petAction) {
        petAction->pause();
    }

    pauseLayer = LayerColor::create(Color4B(10, 10, 10, 127));
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,pauseLayer);
    addChild(pauseLayer);
    
    auto root = CSLoader::createNode(BATTLE_PAUSE_UI);
    pauseAction= CSLoader::createTimeline(BATTLE_PAUSE_UI);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    root->runAction(pauseAction);
    pauseLayer->addChild(root);
    
    auto backButton = (Button*)root->getChildByName("bg_img")->getChildByName("continue");
    backButton->addClickEventListener(CC_CALLBACK_0(GameUILayer::clickBack, this));
    auto closeButton = (Button*)root->getChildByName("bg_img")->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(GameUILayer::clickBack, this));
    auto exitButton = (Button*)root->getChildByName("bg_img")->getChildByName("exit");
    exitButton->addClickEventListener(CC_CALLBACK_0(GameUILayer::clickExit, this));
    
    pauseAction->play("in", false);
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_COMMON info;
    info.eProtocol = e2l_pause_fight;
    ClientLogic::instance()->ProcessUIRequest(&info);
    
    
}

void GameUILayer::clickBack()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
    pauseAction->play("out", false);
    pauseAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(GameUILayer::resumeFight, this));
}

void GameUILayer::resumeFight()
{
    auto skill1Action = (ProgressTimer*)skill1Bg->getChildByTag(SKILLA_KEY+1000);
    if (skill1Action) {
        skill1Action->resume();
    }
    auto skill2Action = (ProgressTimer*)skill2Bg->getChildByTag(SKILLB_KEY+1000);
    if (skill2Action) {
        skill2Action->resume();
    }
    auto skill3Action = (ProgressTimer*)skill3Bg->getChildByTag(SKILLC_KEY+1000);
    if (skill3Action) {
        skill3Action->resume();
    }
    auto holySwordAction = (ProgressTimer*)holySwordBg->getChildByTag(HOLY_SKILL_KEY+1000);
    if (holySwordAction) {
        holySwordAction->resume();
    }
    auto petAction = (ProgressTimer*)petBg->getChildByTag(PET_SKILL1_KEY+1000);
    if (petAction) {
        petAction->resume();
    }
   removeChild(pauseLayer);
    E2L_COMMON info;
    info.eProtocol = e2l_resume_fight;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void GameUILayer::clickExit()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    removeChild(pauseLayer);
    E2L_COMMON info;
    info.eProtocol = e2l_click_fail;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void GameUILayer::updatePetCount(cocos2d::EventCustom *event)
{
    PET_COUNT_DATA data = *static_cast<PET_COUNT_DATA*>(event->getUserData());
    if (data.count <= 0) {
        enable[PET_SKILL1_KEY] = 0;
        enable[PET_SKILL2_KEY] = 0;
        enable[PET_SKILL3_KEY] = 0;
        petSprite->setColor(Color3B(80,80,80));
        petCDBg->setVisible(false);
        petCDText->setVisible(false);
        petCDText->setString("");
        petBg->removeChildByTag(PET_SKILL1_KEY+1000);
    }
    petCount1Sprite->setVisible(data.count>=1);
    petCount2Sprite->setVisible(data.count>=2);
    petCount3Sprite->setVisible(data.count>=3);
}

void GameUILayer::startPlot(cocos2d::EventCustom *event)
{
    L2E_START_PLOT info = *static_cast<L2E_START_PLOT*>(event->getUserData());
    switch (info.type) {
        case 17:
        {
            plotHidePause = true;
            
            E2L_COMPLETE_PLOT infoPlot;
            infoPlot.eProtocol = e2l_complete_plot;
            infoPlot.missionId = info.missionId;
            infoPlot.value = 0;
            ClientLogic::instance()->ProcessUIRequest(&infoPlot);
        }
            break;
        case 14:
        {
            plotHideTime = true;
            
            E2L_COMPLETE_PLOT infoPlot;
            infoPlot.eProtocol = e2l_complete_plot;
            infoPlot.missionId = info.missionId;
            infoPlot.value = 0;
            ClientLogic::instance()->ProcessUIRequest(&infoPlot);
        }
            break;
        case 9:
        {
            switch (info.param[0]) {
                case SKILLA_KEY:
                {
                    enable[SKILLA_KEY] = !info.param[1];
                    skill1Disable->setVisible(!enable[SKILLA_KEY]);
                    skill1CDBg->setVisible(!enable[SKILLA_KEY]);
                }
                    break;
                case SKILLB_KEY:
                {
                    enable[SKILLB_KEY] = !info.param[1];
                    skill2Disable->setVisible(!enable[SKILLB_KEY]);
                    skill2CDBg->setVisible(!enable[SKILLB_KEY]);
                }
                    break;
                case SKILLS_KEY:
                {
                    enable[SKILLS_KEY] = !info.param[1];
                    spDisable->setVisible(!enable[SKILLS_KEY]);
                    spCDBg->setVisible(!enable[SKILLS_KEY]);
                }
                    break;
                case HOLY_SKILL_KEY:
                {
                    enable[HOLY_SKILL_KEY] = !info.param[1];
                    holySwordDisable->setVisible(!enable[HOLY_SKILL_KEY]);
                    holySwordCDBg->setVisible(!enable[HOLY_SKILL_KEY]);
                }
                    break;
                case PET_SKILL1_KEY:
                case PET_SKILL2_KEY:
                case PET_SKILL3_KEY:
                {
                    enable[PET_SKILL1_KEY] = !info.param[1];
                    enable[PET_SKILL2_KEY] = !info.param[1];
                    enable[PET_SKILL3_KEY] = !info.param[1];
                    petDisable->setVisible(info.param[1]);
                    petCDBg->setVisible(info.param[1]);
//                    enable[PET_SKILL1_KEY] = (data.enable?1:0);
                }
                    break;
                default:
                    break;
            }
            
            E2L_COMPLETE_PLOT infoPlot;
            infoPlot.eProtocol = e2l_complete_plot;
            infoPlot.missionId = info.missionId;
            infoPlot.value = 0;
            ClientLogic::instance()->ProcessUIRequest(&infoPlot);
        }
            break;
        case 3:
        {
            plotButtonId = info.param[3];
            plotMissionId = info.missionId;
            
            auto point = CSLoader::createNode(GameUtils::format(GUIDE_POINT_UI, info.param[0]));
            auto action = CSLoader::createTimeline(GameUtils::format(GUIDE_POINT_UI, info.param[0]));
            point->runAction(action);
            action->play("play", true);
            auto pointTip = CSLoader::createNode(GUIDE_POINT_TIP_UI);
            
            switch (info.param[3]) {
                case 1:
                {
                    pointTip->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                    pointTip->setPosition(Vec2(30, 50));
                    
                    point->setPosition(joyStick->getContentSize()/2);
                    point->setTag(100);
                    atkBg->removeChildByTag(100);
                    joyStick->removeChildByTag(100);
                    skill1Bg->removeChildByTag(100);
                    skill2Bg->removeChildByTag(100);
                    spBg->removeChildByTag(100);
                    joyStick->addChild(point);
                }
                    break;
                case 2:
                {
                    pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
                    pointTip->setPosition(Vec2(-150, 50));
                    
                    point->setPosition(atkBg->getContentSize()/2);
                    point->setTag(100);
                    joyStick->removeChildByTag(100);
                    skill1Bg->removeChildByTag(100);
                    skill2Bg->removeChildByTag(100);
                    atkBg->removeChildByTag(100);
                    spBg->removeChildByTag(100);
                    atkBg->addChild(point);
                }
                    break;
                case 3:
                {
                    pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
                    pointTip->setPosition(Vec2(-150, 50));
                    
                    point->setPosition(skill1Bg->getContentSize()/2);
                    point->setTag(100);
                    joyStick->removeChildByTag(100);
                    skill1Bg->removeChildByTag(100);
                    skill2Bg->removeChildByTag(100);
                    spBg->removeChildByTag(100);
                    atkBg->removeChildByTag(100);
                    skill1Bg->addChild(point);
                }
                    break;
                case 4:
                {
                    pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
                    pointTip->setPosition(Vec2(-150, 50));
                    
                    point->setPosition(skill1Bg->getContentSize()/2);
                    point->setTag(100);
                    joyStick->removeChildByTag(100);
                    skill1Bg->removeChildByTag(100);
                    skill2Bg->removeChildByTag(100);
                    spBg->removeChildByTag(100);
                    atkBg->removeChildByTag(100);
                    skill2Bg->addChild(point);
                    
                }
                    break;
                case 5:
                {
                    pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
                    pointTip->setPosition(Vec2(-150, 50));
                    
                    point->setPosition(skill1Bg->getContentSize()/2);
                    point->setTag(100);
                    joyStick->removeChildByTag(100);
                    skill1Bg->removeChildByTag(100);
                    skill2Bg->removeChildByTag(100);
                    spBg->removeChildByTag(100);
                    atkBg->removeChildByTag(100);
                    spBg->addChild(point);
                }
                default:
                    break;
            }
            
            if (info.param[4] != 0) {
                auto text = (Text*)pointTip->getChildByName("bg_img")->getChildByName("count_text");
                text->setString(StringData::shared()->stringFromKey(GameUtils::format("guide_tip%d", info.param[4])));
                point->addChild(pointTip);
            }
            
            E2L_COMPLETE_PLOT infoPlot;
            infoPlot.eProtocol = e2l_complete_plot;
            infoPlot.missionId = info.missionId;
            infoPlot.value = 0;
            ClientLogic::instance()->ProcessUIRequest(&infoPlot);

        }
            break;
        default:
            break;
    }
}

//void GameUILayer::showWarning(cocos2d::EventCustom *event)
//{
//    auto effect = EffectNode::create();
//    effect->initWithRes(WARNING_UI);
//    effect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    effect->setPosition(Vec2::ZERO);
//    addChild(effect);
//}
