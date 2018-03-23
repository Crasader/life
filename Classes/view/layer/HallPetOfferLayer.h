//
//  HallPetOfferLayer.h
//  life
//
//  Created by ff on 17/8/3.
//
//

#ifndef HallPetOfferLayer_h
#define HallPetOfferLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class HallPetOfferLayer : public cocos2d::Layer {
public:
    HallPetOfferLayer();
    ~HallPetOfferLayer();
    
    bool init();
    void onEnter();
    void setupView(cocos2d::EventCustom *event);
    void getAllCD();
    
    void update(float dt);
    
    void clickClose();
    void clickWorkOffer(cocos2d::Ref *pSender);
    void clickRefresh();
    void clickOpenOffer(cocos2d::Ref *pSender);
    void clickTakeOffer(cocos2d::Ref *pSender);
    void clickSelPet(cocos2d::Ref *pSender);
    void clickRemovePet(cocos2d::Ref *pSender);
    void removeOff();
    
    void updateAllCD(cocos2d::EventCustom *event);
    void updateBoardOffer(cocos2d::EventCustom *event);
    void openBoardOffer(cocos2d::EventCustom *event);
    void addOfferPet(cocos2d::EventCustom *event);
    void removeOfferPet(cocos2d::EventCustom *event);
    void takeBoardOffer(cocos2d::EventCustom *event);
    void workOfferOver(cocos2d::EventCustom *event);
    void takeOfferReward(cocos2d::EventCustom *event);
    void showBuyOffer(cocos2d::EventCustom *event);
    void buyOffer(cocos2d::EventCustom *event);
    void resetOfferRefreshCount(cocos2d::EventCustom *event);
    
    void clickBuyClose();
    void clickBuyOk();
    void removeOffBuy();
    
    void removeReadyPet(int petId);
    
    CREATE_FUNC(HallPetOfferLayer);
private:
    cocos2d::ui::ImageView *petsBg;
    cocos2d::ui::ImageView *workPetsBg;
    cocos2d::ui::ImageView *workIcon[3];
    cocos2d::ui::Text *workCDText[3];
    cocos2d::ui::Button *workSelButton[3];
    cocos2d::ui::Text *boardRefreshCDText;
    cocos2d::ui::ImageView *boardOfferBg[3];
    cocos2d::ui::Text *refreshPriceText;
    
    cocos2d::Layer *buyOfferLayer;
    cocos2d::ui::Text *buyOfferCDText;
    cocostudio::timeline::ActionTimeline *buyAction;
    
    cocostudio::timeline::ActionTimeline *rootAction;
    
    cocos2d::EventListenerCustom *updateAllCDListener;
    cocos2d::EventListenerCustom *updateBoardOfferListener;
    cocos2d::EventListenerCustom *openBoardOfferListener;
    cocos2d::EventListenerCustom *addOfferPetListener;
    cocos2d::EventListenerCustom *removeOfferPetListener;
    cocos2d::EventListenerCustom *workOfferOverListener;
    cocos2d::EventListenerCustom *takeBoardOfferListener;
    cocos2d::EventListenerCustom *takeOfferRewardListener;
    cocos2d::EventListenerCustom *showBuyOfferListener;
    cocos2d::EventListenerCustom *buyOfferListener;
    cocos2d::EventListenerCustom *resetOfferRefreshCountListener;
    float refreshCD;
    float workOfferCD[3];
    int petsState[10];
    int readyPets[3];
    int readyCount;
    int needCount;
    int openOfferId;
    int buyOfferPos;
};

#endif /* HallPetOfferLayer_h */
