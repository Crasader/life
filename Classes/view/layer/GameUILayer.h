//
//  GameUILayer.h
//  Dabao
//
//  Created by ff on 17/3/4.
//
//

#ifndef GameUILayer_h
#define GameUILayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "GameDefine.h"
#include "../SneakyJoy/SneakyJoystick.h"
#include "../SneakyJoy/SneakyJoystickSkinnedBase.h"

#define WAVE_TEXT "第%d/%d波"

class GameUILayer : public cocos2d::Layer {
public:
    GameUILayer();
    ~GameUILayer();
    
    bool init();
    void onEnter();
    
    int decodeJoyStick(float delta);
    bool countDownTime(float dt);
    
    void clickUnlock();
    bool touchBegan(cocos2d::Touch*, cocos2d::Event*);
    void touchEnded(cocos2d::Touch*, cocos2d::Event*);
    
    void loopButtoneEffect();
    
    /**
     *	@brief	由UI按键转成逻辑按键值
     *
     *	@param 	key 	int 当前按键，不包含方向键
     */
    bool setCurrKey(int key);
    
    void clearKey();
    
    void startCD(cocos2d::EventCustom *event);
    void updateCD(cocos2d::EventCustom *event);
    void updateAP(cocos2d::EventCustom *event);
    void updateOpponentAP(cocos2d::EventCustom *event);
    void updateHeroHP(cocos2d::EventCustom *event);
    void updateOpponentHP(cocos2d::EventCustom *event);
    void updateGhost(cocos2d::EventCustom *event);
    void updatePetCount(cocos2d::EventCustom *event);
    void showMonsterHp(cocos2d::EventCustom *event);
    void hideMonsterHp();
    void updateMonsterHp(cocos2d::EventCustom *event);
    void updateWaveCount(cocos2d::EventCustom *event);
//    void showWarning(cocos2d::EventCustom *event);
    void endCD(int key);
    void linkEffect2();
    
    float calHpPercent(int currHp);
    void setBarCount(int count);
    void setDeepCount(int count);
    void startHp();
    void endHp();
    void setTime(float total);
    void readyVictory();
    void showWaveCount(int curr, int total);
    void setHeroJob(int job);
    void setGoalText(std::string goalStr);
    
    void clickHpBottole();
    void updateHpBottlePrice(cocos2d::EventCustom *event);
    void clickPause();
    void clickBack();
    void clickExit();
    void resumeFight();
    
    void setupByType(int battleType, bool normal);
    void setOpponentJob(int job);
    void showOpponentInfo(cocos2d::EventCustom *event);
    
    void updateCombo(cocos2d::EventCustom *event);
    void clearCombo();
    
    void enableSkill(cocos2d::EventCustom *event);
    
    void startPlot(cocos2d::EventCustom *event);
    CREATE_FUNC(GameUILayer);
private:
    cocos2d::Rect areaAtk;
    cocos2d::Rect areaSkill1;
    cocos2d::Rect areaSkill2;
    cocos2d::Rect areaSkill3;
    cocos2d::Rect areaSP;
    cocos2d::Rect areaJump;
    cocos2d::Rect areaSword;
    cocos2d::Rect areaPet;
    float pressDur[2];
    float releaseDur;
    int rushPart;
    int joyDir[8];
    int touchButtonId;
    
    int cooling[SKEY_MAX];
    int enable[SKEY_MAX];
    int ghost;
    int hpBarCount;
    int hpDeepCount;
    float deepPercent;
    int hpPerBar;
    int lastBarHp;
    float barPercent;
    int comboIdx;
    
    cocos2d::Vec2 hpBgPos;
    cocos2d::Vec3 RedHSV= cocos2d::Vec3(0, 96, 88);
    
    cocos2d::Node *buttonEffect;
    cocostudio::timeline::ActionTimeline *buttonEffectAction;
    
    cocos2d::Node *apFullEffect;
    cocostudio::timeline::ActionTimeline *apFullAction;
    cocos2d::ProgressTimer* apProgress;
    
    cocos2d::ui::Button* pauseButton;
    cocos2d::Node* opponentApNode[4];
    cocostudio::timeline::ActionTimeline* opponentApAction[4];
    
    cocos2d::LayerColor *pauseLayer;
    cocostudio::timeline::ActionTimeline *pauseAction;
    
    cocos2d::EventListenerCustom *startCDListener;
    cocos2d::EventListenerCustom *updateCDListener;
    cocos2d::EventListenerCustom *updateSPCDListener;
    cocos2d::EventListenerCustom *updateHeroHPListener;
    cocos2d::EventListenerCustom *updateGhostListener;
    cocos2d::EventListenerCustom *showMonsterHpListener;
    cocos2d::EventListenerCustom *hideMonsterHpListener;
    cocos2d::EventListenerCustom *updateMonsterHpListener;
    cocos2d::EventListenerCustom *readyVictoryListener;
    cocos2d::EventListenerCustom *updateWaveCountListener;
    cocos2d::EventListenerCustom *updateHpBottlePriceListener;
    cocos2d::EventListenerCustom *showOpponentInfoListener;
    cocos2d::EventListenerCustom *updateOpponentHpListener;
    cocos2d::EventListenerCustom *updateOpponentApListener;
    cocos2d::EventListenerCustom *updateComboListener;
    cocos2d::EventListenerCustom *clearComboListener;
    cocos2d::EventListenerCustom *switchEnableListener;
    cocos2d::EventListenerCustom *updatePetCountListener;
    cocos2d::EventListenerCustom *startPlotListener;
//    cocos2d::EventListenerCustom *showWarningListener;
    
    cocos2d::Node* heroApNode[4];
    cocostudio::timeline::ActionTimeline* heroApAction[4];
    
    cocos2d::Node* comboRoot;
    cocostudio::timeline::ActionTimeline* comboAction;
    
    CC_SYNTHESIZE_READONLY(cocos2d::ui::TextAtlas*, comboCountText, ComboCountText);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::TextAtlas*, comboFactorText, ComboFactorText);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::LoadingBar*, comboBar, ComboBar);
    
    CC_SYNTHESIZE_READONLY(cocos2d::ui::LoadingBar*, heroHpBar, HeroHpBar);
    CC_SYNTHESIZE_READONLY(cocos2d::Sprite*, heroIcon, HeroIcon);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::LoadingBar*, opponentHpBar, OpponentHpBar);
    CC_SYNTHESIZE_READONLY(cocos2d::Sprite*, opponentIcon, OpponentIcon);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::Button*, hpBottleButton, HpBottleButton);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::Text*, goalText, GoalText);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, typeBg, TypeBg);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, typeImg, TypeImg);
    
    CC_SYNTHESIZE_READONLY(cocos2d::ui::Text*, waveCountText, WaveCountText);
    
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, hpBg, HpBg);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::LoadingBar*, hpBar, HpBar);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::Text*, nameText, NameText);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::Text*, countText, CountText);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, hp2Img, Hp2Img);
//    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, hpDeepImg, HpDeepImg);
//    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, hpWhiteImg, HpWhiteImg);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::LoadingBar*, whiteBar, WhiteBar);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::LoadingBar*, deepBar, DeepBar);
    
    CC_SYNTHESIZE_READONLY(cocos2d::ui::TextAtlas*, timeText, TimeText);
    
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, atkSprite, AtkSprite);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, skill1Sprite, Skill1Sprite);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, skill2Sprite, Skill2Sprite);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, skill3Sprite, Skill3Sprite);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, spSprite, SPSprite);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, jumpSprite, JumpSprite);
//    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, guardSprite, GuardSprite);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, holySwordSprite, HolySwordSprite);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, petSprite, PetSprite);
    
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, atkBg, AtkBg);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, skill1Bg, Skill1Bg);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, skill2Bg, Skill2Bg);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, skill3Bg, Skill3Bg);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, spBg, SPBg);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, jumpBg, JumpBg);
//    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, guardBg, GuardBg);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, holySwordBg, HolySwordBg);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, petBg, PetBg);
    
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, skill1CDBg, Skill1CDBg);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, skill2CDBg, Skill2CDBg);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, skill3CDBg, Skill3CDBg);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, spCDBg, SPCDBg);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, holySwordCDBg, HolySwrodCDBg);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, petCDBg, PetCDBg);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::TextAtlas*, skill1CDText, Skill1CDText);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::TextAtlas*, skill2CDText, Skill2CDText);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::TextAtlas*, skill3CDText, Skill3CDText);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::TextAtlas*, holySwordCDText, HolySwordCDText);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::TextAtlas*, petCDText, PetCDText);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, skill1Disable, Skill1Disable);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, skill2Disable, Skill2Disable);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, spDisable, SPDisable);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, holySwordDisable, HolySwordDisable);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, petDisable, PetDisable);
    
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, ghost1Sprite, Ghost1Sprite);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, ghost2Sprite, Ghost2Sprite);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, ghost3Sprite, Ghost3Sprite);
    
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, petCount1Sprite, PetCount1Sprite);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, petCount2Sprite, PetCount2Sprite);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::ImageView*, petCount3Sprite, PetCount3Sprite);
    
    CC_SYNTHESIZE_READONLY(SneakyJoystick *, joyStick, JoyStick);
    CC_SYNTHESIZE_READONLY(bool, pressJoy1, PressJoy1);
    CC_SYNTHESIZE(KEYCOMMAND, lastRushCmd, LastRushCmd);
    CC_SYNTHESIZE_READONLY(KEYCOMMAND, rushCmd, RushCmd);
    CC_SYNTHESIZE_READONLY(GAMEKEY, keyCmd, KeyCmd);
    CC_SYNTHESIZE_READONLY(int, currKey, CurrKey);
    CC_SYNTHESIZE_READONLY(int, lastDir, LastDir);
    CC_SYNTHESIZE_READONLY(bool, waitResult, WaitResult);
    
    CC_SYNTHESIZE_READONLY(float, stageTime, StageTime);
    
    CC_SYNTHESIZE_READONLY(int, plotButtonId, PlotButtonId);
    CC_SYNTHESIZE_READONLY(int, plotMissionId, PlotMissionId);

    bool plotHidePause;
    bool plotHideTime;
};

#endif /* GameUILayer_h */
