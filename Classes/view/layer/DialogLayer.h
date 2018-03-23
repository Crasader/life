//
//  DialogLayer.h
//  life
//
//  Created by ff on 17/10/17.
//
//

#ifndef DialogLayer_h
#define DialogLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class DialogLayer : public cocos2d::Layer {
public:
    DialogLayer();
    ~DialogLayer();
    
    bool init();
    void onEnter();
    void setupView(cocos2d::EventCustom *event);
    void update(cocos2d::EventCustom *event);
    void removeOff();
    
    bool touchLayer(cocos2d::Touch*, cocos2d::Event*);
    
    CREATE_FUNC(DialogLayer);
private:
    cocos2d::ui::ImageView *leftIcon;
    cocos2d::ui::ImageView *leftNameBg;
    cocos2d::ui::Text *leftNameText;
    cocos2d::ui::ImageView *rightIcon;
    cocos2d::ui::ImageView *rightNameBg;
    cocos2d::ui::Text *rightNameText;
    
    cocos2d::ui::Text *contentText;
    
    cocos2d::EventListenerCustom *updateDialogListener;
    cocos2d::EventListenerCustom *completeDialogListener;
};

#endif /* DialogLayer_h */
