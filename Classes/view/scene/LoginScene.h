//
//  LoginScene.h
//  life
//
//  Created by ff on 18/1/23.
//
//

#ifndef LoginScene_h
#define LoginScene_h

#include "cocos2d.h"
#include "ui/cocosGUI.h"
#include "cocostudio/CocoStudio.h"

class LoginScene : public cocos2d::Scene {
public:
    LoginScene();
    ~LoginScene();
    
    bool init();
    void onEnter();
    void updateNetState(cocos2d::EventCustom *event);
    bool onTouchesBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    
    void animationEvent(cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID);
    void endIn();
//    void endLogoIn();
    CREATE_FUNC(LoginScene);
private:
    bool connected;
    cocos2d::Sprite *bgSpt;
    cocostudio::Armature* logoArm;
    cocostudio::Armature* actorArm;
    
    cocostudio::timeline::ActionTimeline *rootAction;
    
    cocos2d::EventListenerCustom *netStateListener;
};

#endif /* LoginScene_h */
