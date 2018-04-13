//
//  GameScene.h
//  Dabao
//
//  Created by ff on 17/3/3.
//
//

#ifndef GameScene_h
#define GameScene_h

#include "cocos2d.h"
#include "ui/cocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "battle/actor/YNCamera.h"
#include "../layer/GameUILayer.h"
#include "battle/actor/Hero.h"
#include "battle/actor/ProtectNPC.h"
#include "ActorLayer.h"
#include "battle/actor/BulletManager.h"
#include "../node/PlotTip.h"


enum VIEW_GAME_STATES
{
    VIEW_GAME_NONE = 0,
    VIEW_GAME_INIT = 1,
    VIEW_GAME_NORMAL = 2,
    VIEW_GAME_LOAD = 3,
    VIEW_GAME_VICTORY = 4,
    VIEW_GAME_FAIL = 5,
    VIEW_GAME_PAUSE = 6,
    VIEW_GAME_PLOT = 7
};

class GameScene : public cocos2d::Scene {
public:
    GameScene();
    ~GameScene();
    
    bool init();
    void onEnter();
    
    void update(float dt);
    
    void showLoad();
    void replaceLoadImg(std::string imgFile);
    void updateLoad(cocos2d::EventCustom *event);
    void showFight(cocos2d::EventCustom *event);
    void setupTmxMap(cocos2d::EventCustom *event);
    void setupTmxEffect(cocos2d::EventCustom *event);
    void setupHero(cocos2d::EventCustom *event);
    void setupOpponent(cocos2d::EventCustom *event);
    void preloadMonsterRes(cocos2d::EventCustom *event);
    void setupWitness(cocos2d::EventCustom *event);
    void stageVictory(cocos2d::EventCustom *event);
    void stageFail();
    void showRevive();
    void revive();
    void stageTimeOver(cocos2d::EventCustom *event);
    void arenaVictory(cocos2d::EventCustom *event);
    void arenaFail(cocos2d::EventCustom *event);
    void showDialog(cocos2d::EventCustom *event);
    void updateExp(cocos2d::EventCustom *event);
    void showWarning(cocos2d::EventCustom *event);
    
    void setViewWindow();
    void unlockViewWindow();
    
//    bool touchLeftBegan(cocos2d::Ref *pSender, cocos2d::Event *unusedEvent);
//    void touchLeftEnd(cocos2d::Ref *pSender, cocos2d::Event *unusedEvent);
//    void touchLeftCancel(cocos2d::Ref *pSender, cocos2d::Event *unusedEvent);
//    
//    bool touchRightBegan(cocos2d::Ref *pSender, cocos2d::Event *unusedEvent);
//    void touchRightEnd(cocos2d::Ref *pSender, cocos2d::Event *unusedEvent);
//    void touchRightCancel(cocos2d::Ref *pSender, cocos2d::Event *unusedEvent);
    void clickLeft();
    void clickRight();
    
    void pauseFight();
    void resumtFight();
    
    void startPlot(cocos2d::EventCustom *event);
    void showCustomPackage(cocos2d::EventCustom *event);
    void completePlot();
    CREATE_FUNC(GameScene);
private:
    VIEW_GAME_STATES gameState;
    VIEW_GAME_STATES originState;
    
    cocos2d::Layer *backLayer;
    cocos2d::Layer *midLayer;
    cocos2d::Layer *frontLayer;
    cocos2d::Layer *weatherLayer;
    ActorLayer *actorLayer;
    cocos2d::LayerColor *flashLayer;
    GameUILayer *uiLayer;
    PlotTip *plotTip;
    
    cocos2d::Sprite *bgSprite;
    cocos2d::Sprite *midSprite;
    cocos2d::Sprite *frontSprite;
    Hero *hero;
    ProtectNPC *witness;
    cocostudio::Armature *ghost1;
    cocostudio::Armature *ghost2;
    cocos2d::Vec2 ghostPos[6];
//    cocos2d::Vec2 ghost2Pos;
    bool ghostFilpX[6];
    int ghostDir[6];
    std::string ghostAnim[6];
    bool ghostPause[6];
    
//    bool ghost1FlipX;
//    bool ghost2FlipX;
    YNCamera *camera;
    BulletManager *bulletManager;
    
    float speed;
    
    bool plotCamera;
    float plotCameraPosX;
    float plotCameraSpeed;
    int plotCameraMission;
    
    cocos2d::ui::LoadingBar *loadBar;
    cocos2d::LayerColor *loadLayer;
    
    cocos2d::EventListenerCustom *unlockViewListener;
    cocos2d::EventListenerCustom *updateLoadListener;
    cocos2d::EventListenerCustom *setupTmxMapListener;
    cocos2d::EventListenerCustom *setupTmxEffectListener;
    cocos2d::EventListenerCustom *setupHeroListener;
    cocos2d::EventListenerCustom *setupWitnessListener;
    cocos2d::EventListenerCustom *setupOpponentListener;
    cocos2d::EventListenerCustom *preloadMonsterResListener;
    cocos2d::EventListenerCustom *showFightListener;
    cocos2d::EventListenerCustom *stageVictoryListener;
    cocos2d::EventListenerCustom *stageFailListener;
    cocos2d::EventListenerCustom *showReviveListener;
    cocos2d::EventListenerCustom *reviveListener;
    cocos2d::EventListenerCustom *stageTimeOverListener;
    cocos2d::EventListenerCustom *pauseFightListener;
    cocos2d::EventListenerCustom *resumeFightListener;
    cocos2d::EventListenerCustom *arenaVictoryListener;
    cocos2d::EventListenerCustom *arenaFailListener;
    cocos2d::EventListenerCustom *startPlotListener;
    cocos2d::EventListenerCustom *showDialogListener;
    cocos2d::EventListenerCustom *completePlotListener;
    cocos2d::EventListenerCustom *updateExpListener;
    cocos2d::EventListenerCustom *showWarningListener;
    cocos2d::EventListenerCustom *showCustomPackageListener;
    
    CC_SYNTHESIZE_READONLY(cocos2d::Layer *, scrFrtLayer, scrFrtLayer);
    CC_SYNTHESIZE_READONLY(int, battleType, BattleType);
    CC_SYNTHESIZE_READONLY(int, bossTeamId, BossTeamId);
};

#endif /* GameScene_h */
