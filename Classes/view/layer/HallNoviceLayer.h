//
//  HallNoviceLayer.h
//  life
//
//  Created by ff on 17/11/10.
//
//

#ifndef HallNoviceLayer_h
#define HallNoviceLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class HallNoviceLayer : public cocos2d::Layer {
public:
    HallNoviceLayer();
    ~HallNoviceLayer();
    
    bool init();
    void onEnter();
    void setupView(cocos2d::EventCustom *event);
    void updateBuyResult(cocos2d::EventCustom *event);
    
    void clickClose();
    void clickOk();
    
    void removeOff();
    void endIn();
    
    CREATE_FUNC(HallNoviceLayer);
private:
    cocos2d::EventListenerCustom *updateResultListener;

    cocostudio::timeline::ActionTimeline *rootAction;
    cocos2d::ui::Button *okButton;
    cocos2d::ui::Text *priceText;
    cocos2d::ui::ImageView *priceImg;
    int packageId;
    
};

#endif /* HallNoviceLayer_h */
