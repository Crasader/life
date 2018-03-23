//
//  SystemLoadScene.h
//  life
//
//  Created by ff on 17/7/27.
//
//

#ifndef SystemLoadScene_h
#define SystemLoadScene_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class SystemLoadScene : public cocos2d::Scene {
public:
    SystemLoadScene();
    ~SystemLoadScene();
    
    bool init();
    void onEnter();
    void update(float dt);
    void updateLoadBar(cocos2d::EventCustom *event);
    
    CREATE_FUNC(SystemLoadScene);
private:
    cocos2d::ui::LoadingBar *loadBar;
    
    cocos2d::EventListenerCustom *updateLoadListener;
};

#endif /* SystemLoadScene_h */
