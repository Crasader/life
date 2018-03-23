//
//  PackageDetailLayer.h
//  life
//
//  Created by ff on 17/11/9.
//
//

#ifndef PackageDetailLayer_h
#define PackageDetailLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class PackageDetailLayer : public cocos2d::Layer {
public:
    PackageDetailLayer();
    ~PackageDetailLayer();
    
    bool init();
    void setupView(cocos2d::EventCustom *event);
    
    void clickClose();
    void clickOk();
    
    void removeOff();
    
    void endIn();
    CREATE_FUNC(PackageDetailLayer);
private:
    int packageId;
    cocostudio::timeline::ActionTimeline *rootAction;
    cocos2d::ui::ListView *itemList;
    cocos2d::ui::Text *priceText;
    cocos2d::ui::ImageView *titleImg;
    cocos2d::ui::ImageView *packageImg;
    
};

#endif /* PackageDetailLayer_h */
