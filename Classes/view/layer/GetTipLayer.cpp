//
//  GetTipLayer.cpp
//  life
//
//  Created by ff on 17/8/31.
//
//

#include "GetTipLayer.h"
#include "EventDefine.h"
#include "ELProtocol.h"
#include "utils/GameUtils.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace CocosDenshion;

bool GetTipLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    waitTips.clear();
    bottomIndex = -1;
    currPack.validCount = 0;
    top = 0;
    bottom = 0;
    topIndex = 0;
    return true;
}

void GetTipLayer::onEnter()
{
    Layer::onEnter();
    
    addTipListener = EventListenerCustom::create(SHOW_GET_TIP, CC_CALLBACK_1(GetTipLayer::addTipPack, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(addTipListener, -1);
}

void GetTipLayer::onExit()
{
    Layer::onExit();
    Director::getInstance()->getEventDispatcher()->removeEventListener(addTipListener);
}

void GetTipLayer::addTipPack(cocos2d::EventCustom *event)
{
    L2E_SHOW_GET info = *static_cast<L2E_SHOW_GET*>(event->getUserData());
    TIP_DATA data;
    memcpy(data.count, info.count, sizeof(int)*20);
    for (int i = 0; i < 20; i++) {
        data.icon[i] = info.icon[i];
        data.frag[i] = info.frag[i];
        data.name[i] = info.name[i];
    }
    
    data.validCount = 0;
    for (int i = 0; i < 20; i++) {
        if (data.count[i] > 0) {
            data.validCount++;
        }
    }
    data.totalCount = data.validCount;
//    waitTips.reserve(waitTips.size());
    waitTips.push_back(data);
//    waitTips.reserve(waitTips.size());
    if (currPack.validCount == 0) {
        startWaitTipPack();
    }
}

void GetTipLayer::startWaitTipPack()
{
    //加第一个
    currPack = waitTips[0];
    waitTips.begin() = waitTips.erase(waitTips.begin());
    bottomIndex = 0;
    top = 1;
    bottom = 1;
    topIndex = 0;
    auto getRoot = CSLoader::createNode(GET_TIP_UI);
    getRoot->setTag(bottomIndex+1);
    getRoot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    getRoot->setPosition(GameUtils::winSize/2);
    addChild(getRoot);
    tipAction[bottomIndex] = CSLoader::createTimeline(GET_TIP_UI);
    getRoot->runAction(tipAction[bottomIndex]);
    
    auto iconImg = (ImageView*)getRoot->getChildByName("bg_img")->getChildByName("icon");
    auto countText = (Text*)getRoot->getChildByName("bg_img")->getChildByName("count_text");
    auto fragSprite = (Sprite*)iconImg->getChildByName("frag");
    auto nameText = (Text*)getRoot->getChildByName("bg_img")->getChildByName("name_text");
    nameText->setString(currPack.name[bottomIndex]);
    iconImg->loadTexture(currPack.icon[bottomIndex].c_str());
    fragSprite->setVisible(currPack.frag[bottomIndex]);
    countText->setString(Convert2String(currPack.count[bottomIndex]));
    
    tipAction[bottomIndex]->play("move0", false);
    tipAction[bottomIndex]->setAnimationEndCallFunc("move0", CC_CALLBACK_0(GetTipLayer::endTipIn, this));
    
    SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "get.mp3").c_str(),false,1,0,0.5);
}

void GetTipLayer::endTipIn()
{
    if (bottomIndex+1 < currPack.totalCount) {//还有新的
        // 所有已经显示的向上
        for (int i = 0; i < 3; i++) {
            // 防止不足三个的情况
            if (top-i < bottom) {
                break;
            }
            std::string animStr(GameUtils::format("move%d",(top-i)));
            tipAction[topIndex+i]->play(animStr, false);
        }
        // 移动顶部标签
        top++;
        
        // 第三个出去后，顶部索引指向下一个，顶部标签维持在3
        if (top > 3) {
            currPack.validCount--;
            topIndex++;
            top = 3;
        }
        // 出新的
        bottomIndex++;
        bottom = 1;
        auto getRoot = CSLoader::createNode(GET_TIP_UI);
        getRoot->setTag(bottomIndex+1);
        getRoot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        getRoot->setPosition(GameUtils::winSize/2);
        addChild(getRoot);
        tipAction[bottomIndex] = CSLoader::createTimeline(GET_TIP_UI);
        getRoot->runAction(tipAction[bottomIndex]);
        
        auto iconImg = (ImageView*)getRoot->getChildByName("bg_img")->getChildByName("icon");
        auto countText = (Text*)getRoot->getChildByName("bg_img")->getChildByName("count_text");
        auto fragSprite = (Sprite*)iconImg->getChildByName("frag");
        auto nameText = (Text*)getRoot->getChildByName("bg_img")->getChildByName("name_text");
        nameText->setString(currPack.name[bottomIndex]);
        iconImg->loadTexture(currPack.icon[bottomIndex].c_str());
        fragSprite->setVisible(currPack.frag[bottomIndex]);
        countText->setString(Convert2String(currPack.count[bottomIndex]));
        
        tipAction[bottomIndex]->play("move0", false);
        tipAction[bottomIndex]->setAnimationEndCallFunc("move0", CC_CALLBACK_0(GetTipLayer::endTipIn, this));
        
        SimpleAudioEngine::getInstance()->playEffect(GameUtils::format(SOUND_DIR, "get.mp3").c_str(),false,1,0,0.5);
    }else{// 没有新的
        if (top == 3) {//已到顶部
            tipAction[topIndex]->play(GameUtils::format("out%d",(top-1)), false);
            // 顶部消失后，进入下一个的消失
            tipAction[topIndex]->setAnimationEndCallFunc(GameUtils::format("out%d",(top-1)), CC_CALLBACK_0(GetTipLayer::endOneTip, this));
        }else{//未到顶部
            // 所有已经显示的向上
            for (int i = 0; i < 3; i++) {
                // 防止不足三个的情况
                if (top-i < bottom) {
                    break;
                }
                std::string animStr(GameUtils::format("move%d",(top-i)));
                tipAction[topIndex+i]->play(animStr, false);
            }
            //顶部移动后，回到这里
            tipAction[topIndex]->setAnimationEndCallFunc(GameUtils::format("move%d",top), CC_CALLBACK_0(GetTipLayer::endTipIn, this));
            // 移动顶部标签,并移动底部标签
            top++;
            bottom++;
        }
    }
}

void GetTipLayer::endOneTip()
{
    // 向后移动顶部标签和索引
    top--;
    currPack.validCount--;
    topIndex++;
    if (top < bottom) {// 所有的都消失了
        removeAllChildren();
        if (!waitTips.empty()) {// 还有下一队
            startWaitTipPack();
        }
        return;
    }
    tipAction[topIndex]->play(GameUtils::format("out%d",(top-1)), false);
    // 顶部消失后，进入下一个的消失
    tipAction[topIndex]->setAnimationEndCallFunc(GameUtils::format("out%d",(top-1)), CC_CALLBACK_0(GetTipLayer::endOneTip, this));
}
