//
//  DialogLayer.cpp
//  life
//
//  Created by ff on 17/10/17.
//
//

#include "DialogLayer.h"
#include "GameDefine.h"
#include "ELProtocol.h"
#include "EventDefine.h"
#include "logic/ClientLogic.h"
#include "utils/GameUtils.h"
#include "utils/StringData.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;


DialogLayer::DialogLayer()
{
}

void DialogLayer::onEnter()
{
    Layer::onEnter();
    updateDialogListener = EventListenerCustom::create(UPDATE_DIALOG, CC_CALLBACK_1(DialogLayer::update, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateDialogListener, -1);
    completeDialogListener = EventListenerCustom::create(COMPLETE_DIALOG, CC_CALLBACK_0(DialogLayer::removeOff, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(completeDialogListener, -1);
    
}

DialogLayer::~DialogLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateDialogListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(completeDialogListener);
}

bool DialogLayer::init()
{
    if (!Layer::init()) {
        return false;
    }

    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(DialogLayer::touchLayer, this);
    listener->setSwallowTouches(true);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto bgImg = ImageView::create(GameUtils::format(STORY_DIR, "duihuakuang.png"));
    bgImg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    bgImg->setPosition(Vec2(GameUtils::winSize.width/2, 0));
    bgImg->setScaleX(GameUtils::winSize.width/800);
    addChild(bgImg);
    
    auto leftRoot = CSLoader::createNode(DIALOG_LEFT_UI);
    leftIcon = (ImageView*)leftRoot->getChildByName("dialog_bg");
    leftNameBg = (ImageView*)leftRoot->getChildByName("name_bg");
    leftNameText = (Text*)leftRoot->getChildByName("name_text");
    contentText = (Text*)leftRoot->getChildByName("content_text");
    contentText->setTextAreaSize(Size(GameUtils::winSize.width-250, 100));
    leftRoot->setPosition(Vec2::ZERO);
    addChild(leftRoot);
    
    auto rightRoot = CSLoader::createNode(DIALOG_RIGHT_UI);
    auto rightAction = CSLoader::createTimeline(DIALOG_RIGHT_UI);
    rightIcon = (ImageView*)rightRoot->getChildByName("dialog_icon");
    rightNameBg = (ImageView*)rightRoot->getChildByName("name_bg");
    rightNameText = (Text*)rightRoot->getChildByName("name_text");
    rightRoot->setPosition(Vec2(GameUtils::winSize.width, 0));
    rightRoot->runAction(rightAction);
    addChild(rightRoot);
    rightAction->play("play", true);
    
    return true;
}

void DialogLayer::setupView(cocos2d::EventCustom *event)
{
    L2E_SHOW_DIALOG info = *static_cast<L2E_SHOW_DIALOG*>(event->getUserData());
    if (info.direct == 1) {
        rightIcon->loadTexture(GameUtils::format(ARENA_DIR, info.icon.c_str()));
        rightNameText->setString(info.name);
    }else {
        leftIcon->loadTexture(GameUtils::format(ARENA_DIR, info.icon.c_str()));
        leftNameText->setString(info.name);
    }
    
    contentText->setString(info.content);
    
    rightIcon->setVisible(info.direct == 1);
    rightNameBg->setVisible(info.direct == 1);
    rightNameText->setVisible(info.direct == 1);
    leftIcon->setVisible(info.direct != 1);
    leftNameBg->setVisible(info.direct != 1);
    leftNameText->setVisible(info.direct != 1);
}

void DialogLayer::update(cocos2d::EventCustom *event)
{
    L2E_SHOW_DIALOG info = *static_cast<L2E_SHOW_DIALOG*>(event->getUserData());
    if (info.direct == 1) {
        rightIcon->loadTexture(GameUtils::format(ARENA_DIR, info.icon.c_str()));
        rightNameText->setString(info.name);
    }else {
        leftIcon->loadTexture(GameUtils::format(ARENA_DIR, info.icon.c_str()));
        leftNameText->setString(info.name);
    }
    
    contentText->setString(info.content);
    
    rightIcon->setVisible(info.direct == 1);
    rightNameBg->setVisible(info.direct == 1);
    rightNameText->setVisible(info.direct == 1);
    leftIcon->setVisible(info.direct != 1);
    leftNameBg->setVisible(info.direct != 1);
    leftNameText->setVisible(info.direct != 1);
}

bool DialogLayer::touchLayer(cocos2d::Touch *, cocos2d::Event *)
{
    E2L_COMMON info;
    info.eProtocol = e2l_next_dialog;
    ClientLogic::instance()->ProcessUIRequest(&info);
    return true;
}

void DialogLayer::removeOff()
{
    removeFromParent();
}
