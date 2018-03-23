//
//  HallArenaLayer.h
//  life
//
//  Created by ff on 17/9/21.
//
//

#ifndef HallArenaLayer_h
#define HallArenaLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class HallArenaLayer : public cocos2d::Layer {
public:
    HallArenaLayer();
    ~HallArenaLayer();
    
    bool init();
    void onEnter();
    void update(float dt);
    
    void setupView(cocos2d::EventCustom *event);
    void showDivision(cocos2d::EventCustom *event);
    void startMatchOpponent(cocos2d::EventCustom *event);
    void buyTicket(cocos2d::EventCustom *event);
    void updateTicket(cocos2d::EventCustom *event);
    void updateTicketCD(cocos2d::EventCustom *event);
    void showReward(cocos2d::EventCustom *event);
    
    void clickClose();
    void clickStart();
    void clickDivistion();
    void clickCloseDivision();
    void clickAddTicket();
    void clickCloseBuy();
    void clickBuyTicket();
    
    bool onTouchReward(cocos2d::Touch* touch, cocos2d::Event* event);
    
    void removeOff();
    void removeOffDivision();
    void removeOffBuy();
    void removeOffReward();
    
    void endIn();
    
    CREATE_FUNC(HallArenaLayer);
private:
    cocos2d::ui::Text *ticketText;
    cocos2d::ui::Text *cdText;
    cocos2d::ui::Button *divisionButton;
    cocos2d::ui::Button *addButton;
    cocos2d::ui::ImageView *divisionIcon;
    cocos2d::ui::ImageView *divisionIcon2;
    cocos2d::ui::Text *placeText;
    cocos2d::ui::Text *scoreText;
    cocos2d::ui::Text *forceText;
    cocos2d::ui::ImageView* petIcon[3];
    cocos2d::ui::ImageView* petBg[3];
    cocos2d::ui::ImageView* swordIcon;
    cocos2d::ui::ListView *rankList;
    
    cocos2d::Layer* divisionLayer;
    cocos2d::LayerColor *buyLayer;
    cocos2d::LayerColor *rewardLayer;
    
    std::map<int, cocos2d::ui::ImageView*> rankBgMap;
    cocostudio::timeline::ActionTimeline *rootAction;
    cocostudio::timeline::ActionTimeline *divisionAction;
    cocostudio::timeline::ActionTimeline *buyAction;
    cocostudio::timeline::ActionTimeline *rewardAction;
    
    cocos2d::EventListenerCustom *showDivisionListener;
    cocos2d::EventListenerCustom *startMatchListener;
    cocos2d::EventListenerCustom *buyTicketListener;
    cocos2d::EventListenerCustom *updateTicketListener;
    cocos2d::EventListenerCustom *updateTicketCDListener;
    cocos2d::EventListenerCustom *showRewardListener;
    
    float cd;
    
};

#endif /* HallArenaLayer_h */
