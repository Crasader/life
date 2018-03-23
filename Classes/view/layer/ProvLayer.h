//
//  ProvLayer.h
//  life
//
//  Created by ff on 17/8/24.
//
//

#ifndef ProvLayer_h
#define ProvLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class ProvLayer : public cocos2d::Layer {
public:
    ProvLayer();
    ~ProvLayer();
    
    bool init();
    void onEnter();
    void setupView(cocos2d::EventCustom *event);
    
    void clickClose();
    void clickStart();
    void clickAddTicket();
    void clickReset();
    
    void clickBuyTicket();
    void clickCloseBuy();
    void clickReduceTicket();
    void clickPlusTicket();
    
    void confirmStage(cocos2d::EventCustom *event);
    void confirmFighter(cocos2d::EventCustom *event);
    void provFight(cocos2d::EventCustom *event);
    void provWin(cocos2d::EventCustom *event);
    void provLose(cocos2d::EventCustom *event);
    void provReset(cocos2d::EventCustom *event);
    void addTicket(cocos2d::EventCustom *event);
    void refreshReset(cocos2d::EventCustom *event);
    
    void removeOff();
    void endConfirmStage();
    void endConfirmFighter();
    void endFight();
    void endWin();
    void endLose();
    void endKillDefence();
    void endKillAttack();
    void endReward();
    void removeBuyTicket();
    
    void endIn();
    CREATE_FUNC(ProvLayer);
private:
    cocos2d::EventListenerCustom *confirmStageListener;
    cocos2d::EventListenerCustom *confirmFighterListener;
    cocos2d::EventListenerCustom *provFightListener;
    cocos2d::EventListenerCustom *provWinListener;
    cocos2d::EventListenerCustom *provLoseListener;
    cocos2d::EventListenerCustom *provResetListener;
    cocos2d::EventListenerCustom *addTicketListener;
    cocos2d::EventListenerCustom *refreshResetListener;
    
    cocostudio::timeline::ActionTimeline *rootAction;
    cocos2d::ui::ScrollView *stageScroll;
    cocos2d::ui::Text *resetCountText;
    cocos2d::ui::Text *ticketCountText;
    cocos2d::ui::ListView *fighterList;
    
    cocos2d::Node *stageNode[50];
    cocostudio::timeline::ActionTimeline *stageAction[50];
    cocos2d::ui::Button *startButton;
    
    cocos2d::Layer *rewardLayer;
    cocostudio::timeline::ActionTimeline *rewardAction;
    cocos2d::ui::Text *rewardGoldText;
    cocos2d::ui::Text *rewardStoneText;
    
    cocos2d::ui::Text *buyTicketCountText;
    cocos2d::ui::Text *buyTicketPriceText;
    cocos2d::Layer *ticketLayer;
    cocostudio::timeline::ActionTimeline *ticketAction;
    bool playing;
    bool stopFight;
    int currStage;
    int buyTicketCount;
};

#endif /* ProvLayer_h */
