//
//  HallArmsLayer.cpp
//  life
//
//  Created by ff on 17/8/9.
//
//

#include "HallArmsLayer.h"
#include "GameDefine.h"
#include "ELProtocol.h"
#include "EventDefine.h"
#include "logic/ClientLogic.h"
#include "utils/GameUtils.h"
#include "utils/StringData.h"
#include "../node/TipsNode.h"
#include "../node/EffectNode.h"
#include "SimpleAudioEngine.h"
#include "../scene/HallScene.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace CocosDenshion;

HallArmsLayer::HallArmsLayer():
currArmsId(0),
currGemTag(0),
plotButtonId(0),
plotMissionId(0),
plotFillButtonId(0),
plotFillMissionId(0)
{
}

void HallArmsLayer::onEnter()
{
    Layer::onEnter();
    updateArmsListener = EventListenerCustom::create(UPDATE_ARMS, CC_CALLBACK_1(HallArmsLayer::updateArms, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateArmsListener, -1);
    updateLevelListener = EventListenerCustom::create(UPDATE_ARMS_LEVEL, CC_CALLBACK_1(HallArmsLayer::updateLevel, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateLevelListener, -1);
    updateGemCountListener = EventListenerCustom::create(UPDATE_GEM_COUNT, CC_CALLBACK_1(HallArmsLayer::updateGemCount, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateGemCountListener, -1);
    updatePosGemListener = EventListenerCustom::create(UPDATE_ARMS_GEM, CC_CALLBACK_1(HallArmsLayer::updatePosGem, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updatePosGemListener, -1);
    showFillBoardListener = EventListenerCustom::create(SHOW_FILL_GEM, CC_CALLBACK_1(HallArmsLayer::showFillBoard, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(showFillBoardListener, -1);
    startPlotListener = EventListenerCustom::create(START_PLOT, CC_CALLBACK_1(HallArmsLayer::startPlot, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(startPlotListener, -1);
    updateTipListener = EventListenerCustom::create(UPDATE_ARMS_TIP, CC_CALLBACK_1(HallArmsLayer::updateTip, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateTipListener, -1);
    
}

HallArmsLayer::~HallArmsLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateArmsListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateLevelListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateGemCountListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updatePosGemListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(showFillBoardListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(startPlotListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateTipListener);
    ArmatureDataManager::destroyInstance();
    Director::getInstance()->purgeCachedData();
}

bool HallArmsLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
    
    auto root = CSLoader::createNode(HALL_ARMS_UI);
    rootAction = CSLoader::createTimeline(HALL_ARMS_UI);
    root->runAction(rootAction);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    addChild(root);
    
    auto rootBg = (ImageView*)root->getChildByName("bg_img");
    
    closeButton = (Button *)rootBg->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(HallArmsLayer::clickClose, this));
    
    auto leftBg = (ImageView*)rootBg->getChildByName("left_bg");
    for (int i = 0; i < 5; i++) {
        armsButton[i] = (Button*)leftBg->getChildByTag(i+1);
        armsButton[i]->addClickEventListener(CC_CALLBACK_1(HallArmsLayer::clickArms, this));
    }
    totalHpText = (Text*)leftBg->getChildByName("hp_text");
    totalAttackText = (Text*)leftBg->getChildByName("attack_text");
    totalDefenceText = (Text*)leftBg->getChildByName("defence_text");
    totalCritText = (Text*)leftBg->getChildByName("crit_text");
    totalDecritText = (Text*)leftBg->getChildByName("decrit_text");
    
    auto rightBg = (ImageView*)rootBg->getChildByName("right_bg");
    armsTypeImg = (ImageView*)rightBg->getChildByName("arms_type_img");
    armsValueText = (Text*)rightBg->getChildByName("arms_value_text");
    nextLevelText = (ImageView*)rightBg->getChildByName("next_level_text");
    plusValueText = (Text*)nextLevelText->getChildByName("plus_value_text");
    for (int i = 0; i < 6; i++) {
        gemsBg[i] = (ImageView*)rightBg->getChildByTag(i+1);
        auto gemButton = (Button*)gemsBg[i]->getChildByName("gem_button");
        gemButton->addClickEventListener(CC_CALLBACK_1(HallArmsLayer::clickGem, this));
        auto assembleButton = (Button*)gemsBg[i]->getChildByName("assemble_button");
        if (assembleButton != NULL) {
            assembleButton->addClickEventListener(CC_CALLBACK_1(HallArmsLayer::clickAssemble, this));
        }
    }
    
    auto centerBg = (ImageView*)rootBg->getChildByName("center_bg");
    for (int i = 0; i < 6; i++) {
        posButton[i] = (Button*)centerBg->getChildByTag(i+1);
        posButton[i]->addClickEventListener(CC_CALLBACK_1(HallArmsLayer::clickArmsGem, this));
    }
    for (int i = 0; i < 6; i++) {
        lockPosText[i] = (Text*)centerBg->getChildByName(GameUtils::format("lock%d", i+1));
    }
    armsIcon = (Sprite*)centerBg->getChildByName("icon");
    uplevelButton = (Button*)centerBg->getChildByName("uplevel_button");
    uplevelButton->addClickEventListener(CC_CALLBACK_0(HallArmsLayer::clickUplevel, this));
    fameCountText = (Text*)centerBg->getChildByName("fame_count_text");
    levelAtlas = (TextAtlas*)centerBg->getChildByName("level_atlas");
    
    fillGemLayer = nullptr;
    return true;
}

void HallArmsLayer::setupView(cocos2d::EventCustom *event)
{
    L2E_SHOW_HALL_ARMS info = *static_cast<L2E_SHOW_HALL_ARMS*>(event->getUserData());
    for (int i = 0; i < 5; i++) {
        armsButton[i]->setTitleText(info.armsNames[i]);
        armsButton[i]->loadTextureNormal(GameUtils::format(COMMON_DIR, "mubandi.png"));
    }
    totalHpText->setString(Convert2String(info.totalHp));
    totalAttackText->setString(Convert2String(info.totalAttack));
    totalDefenceText->setString(Convert2String(info.totalDefence));
    totalCritText->setString(Convert2String(info.totalCrit));
    totalDecritText->setString(Convert2String(info.totalDecrit));
    
    armsTypeImg->loadTexture(GameUtils::format(COMMON_DIR, propIcon[info.armsType-1].c_str()));
    armsValueText->setString(Convert2String(info.armsValue));
    plusValueText->setString(Convert2String(info.nextLevelValue));
    nextLevelText->setVisible(!info.maxLevel);
    for (int i = 0; i < 6; i++) {
        auto icon = (ImageView*)gemsBg[i]->getChildByName("icon");
        icon->loadTexture(GameUtils::format(COMMON_DIR, info.gemsIcon[i].c_str()));
        auto countText = (Text*)gemsBg[i]->getChildByName("count_text");
        countText->setString(GameUtils::format("%d/%d", info.gemCount[i], assembleGemCount));
    }
    
    for (int i = 0; i < 6; i++) {
        if (info.posGemIcon[i] == "") {
            posButton[i]->setVisible(false);
        }else{
            posButton[i]->setVisible(true);
            posButton[i]->loadTextureNormal(GameUtils::format(COMMON_DIR, info.posGemIcon[i].c_str()));
        }
//        posButton[i]->setVisible(!info.lockPos[i]);
        auto popNode =  posButton[i]->getChildByName("pop");
        auto effectNode = posButton[i]->getChildByName("effect");
        auto effectAction = CSLoader::createTimeline(ARMS_GEM_EFFECT);
        effectNode->runAction(effectAction);
        effectAction->play("play", true);
        auto removeButton = (Button*)popNode->getChildByName("remove_button");
        removeButton->addClickEventListener(CC_CALLBACK_1(HallArmsLayer::clickRemove, this));
        popNode->setVisible(false);
        
//        if (i>=1) {
        if (info.lockPos[i] == 1) {
            lockPosText[i]->
            setString(GameUtils::format(
                                        StringData::shared()->stringFromKey("arms_pos_lock").c_str(),
                                        info.unlockLevel[i]));
            lockPosText[i]->setTextColor(Color4B::WHITE);
            lockPosText[i]->setVisible(true);
        }else{
            if (info.posGemIcon[i] == "") {
                lockPosText[i]->
                setString(StringData::shared()->stringFromKey("arms_gem_empty").c_str());
                lockPosText[i]->setTextColor(Color4B::GREEN);
                lockPosText[i]->setVisible(true);
            }else{
                lockPosText[i]->setVisible(false);
            }
        }
//        }
    }
    armsIcon->setTexture(GameUtils::format(ARMS_DIR, info.armsIcon.c_str()));
    levelAtlas->setString(Convert2String(info.level));
    if (info.maxLevel) {
        fameCountText->setString(GameUtils::format("%d", info.currFame));
        uplevelButton->setVisible(false);
    }else{
        fameCountText->setString(GameUtils::format("%d/%d", info.currFame, info.costFame));
        uplevelButton->setVisible(true);
    }
    
    currArmsId = info.currArmsId;
    armsButton[currArmsId-1]->loadTextureNormal(GameUtils::format(COMMON_DIR, "mubandi2.png"));
    
    for (int i = 0; i < 5; i++) {
        auto tip = (ImageView*)armsButton[i]->getChildByName("tip");
        tip->setVisible(info.armsTip[i]);
    }
    
    auto uplevelTip = (ImageView*)uplevelButton->getChildByName("tip");
    uplevelTip->setVisible(info.armsFameTip[currArmsId-1]);
    
    for (int i = 0; i < 6; i++) {
        auto gemButton = (Button*)gemsBg[i]->getChildByName("gem_button");
        auto tip = (ImageView*)gemButton->getChildByName("tip");
        tip->setVisible(info.gemTip[currArmsId-1] == (i+1));
    }
    
    rootAction->play("in", false);
    rootAction->setAnimationEndCallFunc("in", CC_CALLBACK_0(HallArmsLayer::endIn, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "open.mp3").c_str(),false,1,0,0.5);
}

void HallArmsLayer::clickClose()
{
    if (plotButtonId == 4) {
        armsButton[0]->removeChildByTag(1000);
        
        E2L_COMPLETE_PLOT info;
        info.eProtocol = e2l_complete_plot;
        info.missionId = plotMissionId;
        info.value = 0;
        ClientLogic::instance()->ProcessUIRequest(&info);
        plotButtonId = 0;
        plotMissionId = 0;
        
        E2L_TRIGGER_CLICK_BUTTON infoTrigger;
        infoTrigger.eProtocol = e2l_trigger_click_button;
        infoTrigger.index = ARMS_LAYER;
        infoTrigger.buttonId = 4;
        ClientLogic::instance()->ProcessUIRequest(&infoTrigger);
        
    }else if (plotButtonId != 0) {
        return;
    }
    rootAction->play("out", false);
    rootAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(HallArmsLayer::removeOff, this));
}

void HallArmsLayer::removeOff()
{
    removeFromParent();
}

void HallArmsLayer::updateArms(cocos2d::EventCustom *event)
{
    L2E_UPDATE_ARMS info = *static_cast<L2E_UPDATE_ARMS*>(event->getUserData());
    if (info.errNo != 0) {
        std::string errStr = StringData::shared()->stringFromKey("arms_err0");
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        
        return;
    }

    armsButton[currArmsId-1]->loadTextureNormal(GameUtils::format(COMMON_DIR, "mubandi.png"));
    currArmsId = info.currArmsId;
    armsButton[currArmsId-1]->loadTextureNormal(GameUtils::format(COMMON_DIR, "mubandi2.png"));

    armsTypeImg->loadTexture(GameUtils::format(COMMON_DIR, propIcon[info.armsType-1].c_str()));
    armsValueText->setString(Convert2String(info.armsValue));
    plusValueText->setString(Convert2String(info.nextLevelValue));
    nextLevelText->setVisible(!info.maxLevel);
    
    for (int i = 0; i < 6; i++) {
        if (info.posGemIcon[i] == "") {
            posButton[i]->setVisible(false);
        }else{
            posButton[i]->setVisible(true);
            posButton[i]->loadTextureNormal(GameUtils::format(COMMON_DIR, info.posGemIcon[i].c_str()));
        }
//        posButton[i]->setVisible(!info.lockPos[i]);
        auto popNode =  posButton[i]->getChildByName("pop");
        auto effectNode = posButton[i]->getChildByName("effect");
        auto effectAction = CSLoader::createTimeline(ARMS_GEM_EFFECT);
        effectNode->stopAllActions();
        effectNode->runAction(effectAction);
        effectAction->stop();
        effectAction->play("play", true);

        auto removeButton = (Button*)popNode->getChildByName("remove_button");
        removeButton->addClickEventListener(CC_CALLBACK_1(HallArmsLayer::clickRemove, this));
        popNode->setVisible(false);
        
        //        if (i>=1) {
        if (info.lockPos[i] == 1) {
            lockPosText[i]->
            setString(GameUtils::format(
                                        StringData::shared()->stringFromKey("arms_pos_lock").c_str(),
                                        info.unlockLevel[i]));
            lockPosText[i]->setTextColor(Color4B::WHITE);
            lockPosText[i]->setVisible(true);
        }else{
            if (info.posGemIcon[i] == "") {
                lockPosText[i]->
                setString(StringData::shared()->stringFromKey("arms_gem_empty").c_str());
                lockPosText[i]->setTextColor(Color4B::GREEN);
                lockPosText[i]->setVisible(true);
            }else{
                lockPosText[i]->setVisible(false);
            }
        }
        //        }
    }
    armsIcon->setTexture(GameUtils::format(ARMS_DIR, info.armsIcon.c_str()));
    levelAtlas->setString(Convert2String(info.level));
    if (info.maxLevel) {
        fameCountText->setString(GameUtils::format("%d", info.currFame));
        uplevelButton->setVisible(false);
    }else{
        fameCountText->setString(GameUtils::format("%d/%d", info.currFame, info.costFame));
        uplevelButton->setVisible(true);
    }
    for (int i = 0; i < 6; i++) {
        auto icon = (ImageView*)gemsBg[i]->getChildByName("icon");
        icon->loadTexture(GameUtils::format(COMMON_DIR, info.gemsIcon[i].c_str()));
        auto countText = (Text*)gemsBg[i]->getChildByName("count_text");
        countText->setString(GameUtils::format("%d/%d", info.gemCount[i], assembleGemCount));
    }
    
    auto uplevelTip = (ImageView*)uplevelButton->getChildByName("tip");
    uplevelTip->setVisible(info.armsFameTip[currArmsId-1]);
    
    for (int i = 0; i < 6; i++) {
        auto gemButton = (Button*)gemsBg[i]->getChildByName("gem_button");
        auto tip = (ImageView*)gemButton->getChildByName("tip");
        tip->setVisible(info.gemTip[currArmsId-1] == (i+1));
    }
}

void HallArmsLayer::updateLevel(cocos2d::EventCustom *event)
{
    L2E_UPDATE_ARMS_LEVEL info = *static_cast<L2E_UPDATE_ARMS_LEVEL*>(event->getUserData());
    if (info.errNo != 0) {
        std::string errStr =
            StringData::shared()->stringFromKey(GameUtils::format("arms_uplevel_err%d", info.errNo).c_str());
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        
        return;
    }

    totalHpText->setString(Convert2String(info.totalHp));
    totalAttackText->setString(Convert2String(info.totalAttack));
    totalDefenceText->setString(Convert2String(info.totalDefence));
    totalCritText->setString(Convert2String(info.totalCrit));
    totalDecritText->setString(Convert2String(info.totalDecrit));

    armsValueText->setString(Convert2String(info.armsValue));
    plusValueText->setString(Convert2String(info.nextLevelValue));
    nextLevelText->setVisible(!info.maxLevel);

    for (int i = 0; i < 6; i++) {
        if (info.lockPos[i] == 1) {
            lockPosText[i]->
            setString(GameUtils::format(
                                        StringData::shared()->stringFromKey("arms_pos_lock").c_str(),
                                        info.unlockLevel[i]));
            lockPosText[i]->setTextColor(Color4B::WHITE);
            lockPosText[i]->setVisible(true);
        }else{
            if (info.posGemIcon[i] == "") {
                lockPosText[i]->
                setString(StringData::shared()->stringFromKey("arms_gem_empty").c_str());
                lockPosText[i]->setTextColor(Color4B::GREEN);
                lockPosText[i]->setVisible(true);
            }else{
                lockPosText[i]->setVisible(false);
            }
        }
    }
    levelAtlas->setString(Convert2String(info.level));
    if (info.maxLevel) {
        fameCountText->setString(GameUtils::format("%d", info.currFame));
        uplevelButton->setVisible(false);
    }else{
        fameCountText->setString(GameUtils::format("%d/%d", info.currFame, info.costFame));
        uplevelButton->setVisible(true);
    }
}

void HallArmsLayer::updateGemCount(cocos2d::EventCustom *event)
{
    L2E_UPDATE_GEM_COUNT info = *static_cast<L2E_UPDATE_GEM_COUNT*>(event->getUserData());
    if (info.errNo != 0) {
        std::string errStr =
        StringData::shared()->stringFromKey(GameUtils::format("assemble_gem_err%d", info.errNo).c_str());
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        
        return;
    }

    for (int i = 0; i < 6; i++) {
        auto countText = (Text*)gemsBg[i]->getChildByName("count_text");
        countText->setString(GameUtils::format("%d/%d", info.gemCount[i], assembleGemCount));
    }
    
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "stone.mp3").c_str(),false,1,0,0.5);
    if (info.nextGemPos == 0) {
        return;
    }
    auto effect = EffectNode::create();
    effect->initWithRes(ASSEMBLE_GEM_EFFECT);
    effect->setPosition(gemsBg[info.nextGemPos -1]->getContentSize()/2);
    gemsBg[info.nextGemPos -1]->addChild(effect);
    
}

void HallArmsLayer::updatePosGem(cocos2d::EventCustom *event)
{
    L2E_UPDATE_ARMS_GEM info = *static_cast<L2E_UPDATE_ARMS_GEM*>(event->getUserData());
    if (info.errNo != 0) {
        std::string errStr =
        StringData::shared()->stringFromKey(GameUtils::format("fill_gem_err%d", info.errNo).c_str());
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        
        return;
    }

    for (int i = 0; i < 6; i++) {
        auto countText = (Text*)gemsBg[i]->getChildByName("count_text");
        countText->setString(GameUtils::format("%d/%d", info.gemCount[i], assembleGemCount));
    }
    
    totalHpText->setString(Convert2String(info.totalHp));
    totalAttackText->setString(Convert2String(info.totalAttack));
    totalDefenceText->setString(Convert2String(info.totalDefence));
    totalCritText->setString(Convert2String(info.totalCrit));
    totalDecritText->setString(Convert2String(info.totalDecrit));
    
    armsValueText->setString(Convert2String(info.armsValue));
    for (int i = 0; i < 6; i++) {
        if (info.posGemIcon[i] == "") {
            posButton[i]->setVisible(false);
//            lockPosText[i]->setVisible(true);
        }else{
//            lockPosText[i]->setVisible(false);
            posButton[i]->setVisible(true);
            posButton[i]->loadTextureNormal(GameUtils::format(COMMON_DIR, info.posGemIcon[i].c_str()));
        }
        if (info.lockPos[i] == 1) {
            lockPosText[i]->
            setString(GameUtils::format(
                                        StringData::shared()->stringFromKey("arms_pos_lock").c_str(),
                                        info.unlockLevel[i]));
            lockPosText[i]->setTextColor(Color4B::WHITE);
            lockPosText[i]->setVisible(true);
        }else{
            if (info.posGemIcon[i] == "") {
                lockPosText[i]->
                setString(StringData::shared()->stringFromKey("arms_gem_empty").c_str());
                lockPosText[i]->setTextColor(Color4B::GREEN);
                lockPosText[i]->setVisible(true);
            }else{
                lockPosText[i]->setVisible(false);
            }
        }
    }

}

void HallArmsLayer::showFillBoard(cocos2d::EventCustom *event)
{
    L2E_SHOW_FILL_GEM info = *static_cast<L2E_SHOW_FILL_GEM*>(event->getUserData());
    fillGemLayer = (Layer*)CSLoader::createNode(HALL_ARMS_FILL_UI);
    fillAction = CSLoader::createTimeline(HALL_ARMS_FILL_UI);
    fillGemLayer->runAction(fillAction);
    addChild(fillGemLayer);
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, fillGemLayer);
    
    auto rootBg = (ImageView*)fillGemLayer->getChildByName("bg_img");
    
    auto closeButton = (Button*)rootBg->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(HallArmsLayer::clickFillBoardClose, this));
    fillButton = (Button*)rootBg->getChildByName("fill_button");
    fillButton->addClickEventListener(CC_CALLBACK_0(HallArmsLayer::clickFill, this));
    
    auto gemIcon = (ImageView*)rootBg->getChildByName("gem_bg")->getChildByName("icon");
    auto typeImg = (ImageView*)rootBg->getChildByName("type_img");
    auto valueText = (Text*)rootBg->getChildByName("value_text");
    auto countText = (Text*)rootBg->getChildByName("count_text");
    gemIcon->loadTexture(GameUtils::format(COMMON_DIR, info.gemIcon.c_str()));
    typeImg->loadTexture(GameUtils::format(COMMON_DIR, propIcon[info.gemType-1].c_str()));
    valueText->setString(Convert2String(info.gemValue));
    countText->setString(Convert2String(info.gemCount));
    fillAction->play("in", false);
    fillAction->setAnimationEndCallFunc("in", CC_CALLBACK_0(HallArmsLayer::endFillIn, this));
}
void HallArmsLayer::clickFillBoardClose()
{
    if (plotFillButtonId != 0) {
        return;
    }
    fillAction->play("out", false);
    fillAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(HallArmsLayer::removeOffFillBoard, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
}
void HallArmsLayer::removeOffFillBoard()
{
    removeChild(fillGemLayer);
    fillGemLayer = nullptr;
}
void HallArmsLayer::clickUplevel()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    if (plotButtonId != 0) {
        return;
    }
    E2L_COMMON info;
    info.eProtocol = e2l_uplevel_arms;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallArmsLayer::clickArms(cocos2d::Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    int tag = ((Button*)pSender)->getTag();
    if (plotButtonId == 1) {
        if (tag != 1) {
            return;
        }
        armsButton[0]->removeChildByTag(1000);
        
        E2L_COMPLETE_PLOT info;
        info.eProtocol = e2l_complete_plot;
        info.missionId = plotMissionId;
        info.value = 0;
        ClientLogic::instance()->ProcessUIRequest(&info);
        plotButtonId = 0;
        plotMissionId = 0;
        
        E2L_TRIGGER_CLICK_BUTTON infoTrigger;
        infoTrigger.eProtocol = e2l_trigger_click_button;
        infoTrigger.index = ARMS_LAYER;
        infoTrigger.buttonId = 1;
        ClientLogic::instance()->ProcessUIRequest(&infoTrigger);
        
    }else if (plotButtonId != 0) {
        return;
    }
    
    if (tag == currArmsId) {
        return;
    }
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_sel_arms;
    info.value = tag;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallArmsLayer::clickAssemble(cocos2d::Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    if (plotButtonId != 0) {
        return;
    }
    int tag = ((Button*)pSender)->getParent()->getTag();
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_assemble_gem;
    info.value = tag;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallArmsLayer::clickRemove(cocos2d::Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    if (plotButtonId != 0) {
        return;
    }
    for (int i = 0; i < 6; i++) {
        auto popNode =  posButton[i]->getChildByName("pop");
        popNode->setVisible(false);
    }
    int tag = ((Button*)pSender)->getParent()->getParent()->getTag();
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_remove_gem;
    info.value = tag;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallArmsLayer::clickArmsGem(cocos2d::Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    for (int i = 0; i < 6; i++) {
        auto popNode =  posButton[i]->getChildByName("pop");
        popNode->setVisible(false);
    }

    int tag = ((Button*)pSender)->getTag();
    if (plotButtonId == 3) {
        if (tag != 1) {
            return;
        }
        ((Button*)pSender)->removeChildByTag(1000);
        
        E2L_COMPLETE_PLOT info;
        info.eProtocol = e2l_complete_plot;
        info.missionId = plotMissionId;
        info.value = 0;
        ClientLogic::instance()->ProcessUIRequest(&info);
        plotButtonId = 0;
        plotMissionId = 0;
        
        E2L_TRIGGER_CLICK_BUTTON infoTrigger;
        infoTrigger.eProtocol = e2l_trigger_click_button;
        infoTrigger.index = ARMS_LAYER;
        infoTrigger.buttonId = 3;
        ClientLogic::instance()->ProcessUIRequest(&infoTrigger);
        
    }else if (plotButtonId != 0) {
        return;
    }
    auto popNode =  posButton[tag-1]->getChildByName("pop");
    auto effectAction = CSLoader::createTimeline(HALL_ARMS_REMOVE_UI);
    popNode->stopAllActions();
    popNode->runAction(effectAction);
    effectAction->stop();
    effectAction->play("in", false);
    popNode->setVisible(true);

}

void HallArmsLayer::clickGem(cocos2d::Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    int tag = ((Button*)pSender)->getParent()->getTag();
    if (plotButtonId == 2) {
        if (tag != 1) {
            return;
        }
        ((Button*)pSender)->removeChildByTag(1000);
        
        E2L_COMPLETE_PLOT info;
        info.eProtocol = e2l_complete_plot;
        info.missionId = plotMissionId;
        info.value = 0;
        ClientLogic::instance()->ProcessUIRequest(&info);
        plotButtonId = 0;
        plotMissionId = 0;
        
        E2L_TRIGGER_CLICK_BUTTON infoTrigger;
        infoTrigger.eProtocol = e2l_trigger_click_button;
        infoTrigger.index = ARMS_LAYER;
        infoTrigger.buttonId = 2;
        ClientLogic::instance()->ProcessUIRequest(&infoTrigger);
        
    }else if (plotButtonId != 0) {
        return;
    }
    currGemTag = tag;
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_show_fill_board;
    info.value = tag;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallArmsLayer::clickFill()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    if (plotFillButtonId == 1) {
        fillButton->removeChildByTag(1000);
        
        E2L_COMPLETE_PLOT info;
        info.eProtocol = e2l_complete_plot;
        info.missionId = plotFillMissionId;
        info.value = 0;
        ClientLogic::instance()->ProcessUIRequest(&info);
        plotFillButtonId = 0;
        plotFillMissionId = 0;
        
        E2L_TRIGGER_CLICK_BUTTON infoTrigger;
        infoTrigger.eProtocol = e2l_trigger_click_button;
        infoTrigger.index = FILL_GEM_LAYER;
        infoTrigger.buttonId = 1;
        ClientLogic::instance()->ProcessUIRequest(&infoTrigger);
        
    }else if (plotFillButtonId != 0) {
        return;
    }
    clickFillBoardClose();
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_fill_gem;
    info.value = currGemTag;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallArmsLayer::startPlot(cocos2d::EventCustom *event)
{
    L2E_START_PLOT info = *static_cast<L2E_START_PLOT*>(event->getUserData());
    switch (info.type) {
        case 3:
        {
            if(info.param[2] == ARMS_LAYER)
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
                        pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                        pointTip->setPosition(Vec2(150, 35));
                        
                        point->setPosition(armsButton[0]->getContentSize()/2);
                        point->setTag(1000);
                        auto gemButton = (Button*)gemsBg[0]->getChildByName("gem_button");
                        gemButton->removeChildByTag(1000);
                        armsButton[0]->removeChildByTag(1000);
                        posButton[0]->removeChildByTag(1000);
                        closeButton->removeChildByTag(1000);
                        armsButton[0]->addChild(point);
                    }
                        break;
                    case 2:
                    {
                        auto gemButton = (Button*)gemsBg[0]->getChildByName("gem_button");
                        
                        pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                        pointTip->setPosition(Vec2(150, 35));
                        
                        point->setPosition(gemButton->getContentSize()/2);
                        point->setTag(1000);
                        gemButton->removeChildByTag(1000);
                        armsButton[0]->removeChildByTag(1000);
                        posButton[0]->removeChildByTag(1000);
                        closeButton->removeChildByTag(1000);
                        gemButton->addChild(point);
                        
                    }
                        break;
                    case 3:
                    {
                        pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                        pointTip->setPosition(Vec2(150, 35));
                        
                        point->setPosition(posButton[0]->getContentSize()/2);
                        point->setTag(1000);
                        auto gemButton = (Button*)gemsBg[0]->getChildByName("gem_button");
                        gemButton->removeChildByTag(1000);
                        armsButton[0]->removeChildByTag(1000);
                        posButton[0]->removeChildByTag(1000);
                        closeButton->removeChildByTag(1000);
                        posButton[0]->addChild(point);
                    }
                        break;
                    case 4:
                    {
                        pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                        pointTip->setPosition(Vec2(150, 35));
                        
                        point->setPosition(closeButton->getContentSize()/2);
                        point->setTag(1000);
                        auto gemButton = (Button*)gemsBg[0]->getChildByName("gem_button");
                        gemButton->removeChildByTag(1000);
                        armsButton[0]->removeChildByTag(1000);
                        posButton[0]->removeChildByTag(1000);
                        closeButton->removeChildByTag(1000);
                        closeButton->addChild(point);
                    }
                        break;
                        
                    default:
                        break;
                }
                
                if (info.param[4] != 0) {
                    auto text = (Text*)pointTip->getChildByName("bg_img")->getChildByName("count_text");
                    text->setString(StringData::shared()->stringFromKey(GameUtils::format("guide_tip%d", info.param[4])));
                    point->addChild(pointTip);
                }
            }else if(info.param[2] == FILL_GEM_LAYER) {
                plotFillButtonId = info.param[3];
                plotFillMissionId = info.missionId;
                
                auto point = CSLoader::createNode(GameUtils::format(GUIDE_POINT_UI, info.param[0]));
                auto action = CSLoader::createTimeline(GameUtils::format(GUIDE_POINT_UI, info.param[0]));
                point->runAction(action);
                action->play("play", true);
                
                auto pointTip = CSLoader::createNode(GUIDE_POINT_TIP_UI);
                
                switch (info.param[3]) {
                    case 1:
                    {
                        pointTip->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                        pointTip->setPosition(Vec2(150, 35));
                        
                        point->setPosition(fillButton->getContentSize()/2);
                        point->setTag(1000);
                        fillButton->removeChildByTag(1000);
                        fillButton->addChild(point);
                    }
                        break;
                        
                    default:
                        break;
                }
                
                if (info.param[4] != 0) {
                    auto text = (Text*)pointTip->getChildByName("bg_img")->getChildByName("count_text");
                    text->setString(StringData::shared()->stringFromKey(GameUtils::format("guide_tip%d", info.param[4])));
                    point->addChild(pointTip);
                }
            }
            

        }
            break;
            
        default:
            break;
    }
}

void HallArmsLayer::endIn()
{
    HallScene::autoPopLayerId = NONE_LAYER;
    E2L_TRIGGER_OPEN_LAYER info;
    info.eProtocol = e2l_trigger_open_layer;
    info.index = ARMS_LAYER;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallArmsLayer::endFillIn()
{
    E2L_TRIGGER_OPEN_LAYER info;
    info.eProtocol = e2l_trigger_open_layer;
    info.index = FILL_GEM_LAYER;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void HallArmsLayer::updateTip(cocos2d::EventCustom *event)
{
    L2E_UPDATE_ARMS_TIP info = *static_cast<L2E_UPDATE_ARMS_TIP*>(event->getUserData());
    for (int i = 0; i < 5; i++) {
        auto tip = (ImageView*)armsButton[i]->getChildByName("tip");
        tip->setVisible(info.armsTip[i]);
    }
    
    auto uplevelTip = (ImageView*)uplevelButton->getChildByName("tip");
    uplevelTip->setVisible(info.armsFameTip[currArmsId-1]);
    
    for (int i = 0; i < 6; i++) {
        auto gemButton = (Button*)gemsBg[i]->getChildByName("gem_button");
        auto tip = (ImageView*)gemButton->getChildByName("tip");
        tip->setVisible(info.gemTip[currArmsId-1] == (i+1));
    }
}
