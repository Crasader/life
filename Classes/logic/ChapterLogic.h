//
//  ChapterLogic.h
//  life
//
//  Created by ff on 17/7/12.
//
//

#ifndef ChapterLogic_h
#define ChapterLogic_h

#include "cocos2d.h"
#include "utils/GameUtils.h"
#include "cocostudio/CocoStudio.h"
#include "ELProtocol.h"
#include "TLProtocol.h"
#include "data/Chapter.h"
#include "data/VIT.h"
#include "data/Tower.h"
#include "data/Arms.h"
#include "data/Arena.h"


class ChapterLogic : public Singleton<ChapterLogic> {
public:
    ChapterLogic();
    ~ChapterLogic();
    
    void logic(float dt);
    void clickChapterName(E2L_CLICK_CHAPTER_NAME info);
    void showBattle();
    void back2Battle();
    void showChapter(E2L_SHOW_CHAPTER info);
    void back2Chapter();
    void showStage(E2L_SHOW_STAGE info);
    void back2Stage();
    void startStage();
    void openStarBox(E2L_UPDATE_ONE_VALUE info);
    void checkCheckAbyss(E2L_UPDATE_ONE_VALUE info);
    void startAbyss();
    void resetAbyss(E2L_UPDATE_ONE_VALUE info);
    void showAyssReset(E2L_UPDATE_ONE_VALUE info);
    void refreshAbyss();
    void showTower();
    void showTowerSweep();
    void startTower();
    void resetTower();
    void startTowerSweep();
    void completeTowerSweep();
    void towerTakeReward();
    void speedupTowerSweep();
    void refreshTowerReset();
    void getTowerSweepCD();
    void updateTowerSweepCD(T2L_COMMON infoTime);

    void startArenaFight();
private:
    Chapter *data;
    VIT *vit;
    Tower *tower;
    Arena *arena;
    CC_SYNTHESIZE_READONLY(int, currStage, CurrStage);
    CC_SYNTHESIZE_READONLY(int, currAbyss, CurrAbyss);
    CC_SYNTHESIZE_READONLY(int, currTower, CurrTower);
    CC_SYNTHESIZE_READONLY(int, currArena, CurrArena);
};


#endif /* ChapterLogic_h */
