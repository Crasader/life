//
//  HallNoviceLayer.cpp
//  life
//
//  Created by ff on 17/11/10.
//
//

#include "HallNoviceLayer.h"
#include "GameDefine.h"
#include "ELProtocol.h"
#include "EventDefine.h"
#include "logic/ClientLogic.h"
#include "utils/GameUtils.h"
#include "utils/StringData.h"
#include "utils/TimeUtil.h"
#include "../node/TipsNode.h"
#include "SimpleAudioEngine.h"
#include "../scene/HallScene.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace CocosDenshion;

HallNoviceLayer::HallNoviceLayer():
packageId(0)
{

}
void HallNoviceLayer::onEnter()
{
    Layer::onEnter();
    updateResultListener = EventListenerCustom::create(SHOW_PACKAGE_RESULT, CC_CALLBACK_1(HallNoviceLayer::updateBuyResult, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(updateResultListener, -1);
    
}

HallNoviceLayer::~HallNoviceLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(updateResultListener);
    ArmatureDataManager::destroyInstance();
    Director::getInstance()->purgeCachedData();
}

bool HallNoviceLayer::init()
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
    
    auto root = CSLoader::createNode(HALL_NOVICE_UI);
    rootAction = CSLoader::createTimeline(HALL_NOVICE_UI);
    root->runAction(rootAction);
    root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    root->setPosition(GameUtils::winSize/2);
    addChild(root);
    
    auto rootBg = (ImageView*)root->getChildByName("bg_img");
    
    auto closeButton = (Button *)rootBg->getChildByName("close_button");
    closeButton->addClickEventListener(CC_CALLBACK_0(HallNoviceLayer::clickClose, this));

    okButton = (Button*)rootBg->getChildByName("ok_button");
    okButton->addClickEventListener(CC_CALLBACK_0(HallNoviceLayer::clickOk, this));
    
    priceImg = (ImageView*)rootBg->getChildByName("price_img");
    priceText = (Text*)rootBg->getChildByName("price_text");
    return true;
}

void HallNoviceLayer::setupView(cocos2d::EventCustom *event)
{
    L2E_SHOW_PACKAGE_DETAIL info = *static_cast<L2E_SHOW_PACKAGE_DETAIL*>(event->getUserData());
    packageId = info.packageId;
    if (info.noviceState == 1) {
        okButton->setTitleText(StringData::shared()->stringFromKey("novice_complete"));
        okButton->setBright(false);
        okButton->setEnabled(false);
        priceText->setVisible(false);
        priceImg->setVisible(false);
    }else{
        priceText->setString(Convert2String(info.price));
    }
    rootAction->play("in", false);
    rootAction->setAnimationEndCallFunc("in", CC_CALLBACK_0(HallNoviceLayer::endIn, this));
}
void HallNoviceLayer::updateBuyResult(cocos2d::EventCustom *event)
{
    L2E_SHOW_PACKAGE_RESULT info = *static_cast<L2E_SHOW_PACKAGE_RESULT*>(event->getUserData());
    if (info.errNo != 0) {
        std::string errStr = StringData::shared()->stringFromKey("diamond_not_enough");
        if (info.errNo == 1) {
            errStr = StringData::shared()->stringFromKey("buy_err");
        }
        auto tip = TipsNode::create();
        tip->setupText(errStr);
        tip->setPosition(GameUtils::winSize/2);
        addChild(tip, 100);
        return;
    }
    if (info.noviceState == 1) {
        okButton->setTitleText(StringData::shared()->stringFromKey("novice_complete"));
        okButton->setBright(false);
        okButton->setEnabled(false);
        priceText->setVisible(false);
        priceImg->setVisible(false);
    }
}

void HallNoviceLayer::endIn()
{
    HallScene::autoPopLayerId = NONE_LAYER;
}

void HallNoviceLayer::clickClose()
{
    rootAction->play("out", false);
    rootAction->setAnimationEndCallFunc("out", CC_CALLBACK_0(HallNoviceLayer::removeOff, this));
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "close.mp3").c_str(),false,1,0,0.5);
}

void HallNoviceLayer::removeOff()
{
    removeFromParent();
}

void HallNoviceLayer::clickOk()
{
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "click.mp3").c_str(),false,1,0,0.5);
    E2L_UPDATE_ONE_VALUE info;
    info.eProtocol = e2l_shop_buy_package;
    info.value = packageId;
    ClientLogic::instance()->ProcessUIRequest(&info);
}
