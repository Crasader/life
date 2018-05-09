//
//  ChapterLogic.cpp
//  life
//
//  Created by ff on 17/7/12.
//
//

#include "ChapterLogic.h"
#include "logic/ClientLogic.h"
#include "data/Account.h"
#include "data/Proving.h"
#include "data/Abyss.h"
#include "data/Account.h"
#include "data/Equip.h"
#include "utils/StringData.h"

USING_NS_CC;
using namespace cocostudio;

ChapterLogic::ChapterLogic():
currStage(0),
currAbyss(0),
currTower(0),
currArena(0)
{
    data = Chapter::instance();
    vit = VIT::instance();
    tower = Tower::instance();
    arena = Arena::instance();
    
    data->readConfigFile();

//    data->deleteJson();
    if(!data->readJson())
    {
        data->save2EmptyCoreStr();
        data->parseJson();
    }
}

ChapterLogic::~ChapterLogic()
{
    
}

void ChapterLogic::showBattle()
{
    int maxOpenChapter = 0;
    maxOpenChapter = data->getMaxChapterId();
//    maxOpenChapter = 8;
    if (data->getCurrChapter() == 0) {
        data->setCurrChapter(maxOpenChapter);
    }
    
    L2E_SHOW_BATTLE_CHAPTERS info;
    info.eProtocol = l2e_show_battle_chapters;
    info.maxOpenId = maxOpenChapter;
    info.focusId = data->getCurrChapter();
    info.imgFile = data->getChapterImg(info.focusId);
    ClientLogic::instance()->pass2Engine(&info);
}
void ChapterLogic::back2Battle()
{
    int maxOpenChapter = 0;
    maxOpenChapter = data->getMaxChapterId();
    
    L2E_SHOW_BATTLE_CHAPTERS info;
    info.eProtocol = l2e_back_battle_chapters;
    info.maxOpenId = maxOpenChapter;
    info.focusId = data->getCurrChapter();
    info.imgFile = data->getChapterImg(info.focusId);
    ClientLogic::instance()->pass2Engine(&info);

}

void ChapterLogic::clickChapterName(E2L_CLICK_CHAPTER_NAME info)
{
    L2E_SHOW_CHAPTER_NAME infoOut;
    infoOut.eProtocol = l2e_show_chapter_name;
    infoOut.chapterId = info.tag;
    infoOut.imgFile = data->getChapterImg(info.tag);
    ClientLogic::instance()->pass2Engine(&infoOut);
}

void ChapterLogic::showChapter(E2L_SHOW_CHAPTER info)
{
    if (!data->enterChapter(info.tag)) {
        return;
    }
    data->setCurrChapter(info.tag);
    
    L2E_SHOW_CHAPTER_STAGES infoOut;
    infoOut.eProtocol = l2e_show_chapter_stages;
    infoOut.chapterImg = data->getChapterImg(data->getCurrChapter());
    memcpy(infoOut.boxState, data->saveMap[data->getCurrChapter()].boxState, sizeof(int)*3);
    int starCount = 0;
    for(auto stage : data->stageSaveMap)
    {
        if (!stage.second.open) {
            continue;
        }
        STAGE_SUBINFO subInfo;
        subInfo.stageId = stage.second.stageId;
        subInfo.iconImg = data->stageConfigMap[subInfo.stageId].iconFile;
        subInfo.nameImg = data->stageConfigMap[subInfo.stageId].imgFile;
        for (int i = 0; i < 3; i++) {
            subInfo.star[i] = stage.second.star[i];
            if (subInfo.star[i] == 1) {
                starCount++;
            }
        }
        infoOut.stages.push_back(subInfo);
    }
    infoOut.totalStarCount = starCount;
    ClientLogic::instance()->pass2Engine(&infoOut);
}
void ChapterLogic::back2Chapter()
{
    L2E_SHOW_CHAPTER_STAGES infoOut;
    infoOut.eProtocol = l2e_show_chapter_stages;
    infoOut.chapterImg = data->getChapterImg(data->getCurrChapter());
    memcpy(infoOut.boxState, data->saveMap[data->getCurrChapter()].boxState, sizeof(int)*3);

    int starCount = 0;
    for(auto stage : data->stageSaveMap)
    {
        if (!stage.second.open) {
            continue;
        }
        STAGE_SUBINFO subInfo;
        subInfo.stageId = stage.second.stageId;
        subInfo.iconImg = data->stageConfigMap[subInfo.stageId].iconFile;
        subInfo.nameImg = data->stageConfigMap[subInfo.stageId].imgFile;
        for (int i = 0; i < 3; i++) {
            subInfo.star[i] = stage.second.star[i];
            if (subInfo.star[i] == 1) {
                starCount++;
            }
        }
        infoOut.stages.push_back(subInfo);
    }
    infoOut.totalStarCount = starCount;
    ClientLogic::instance()->pass2Engine(&infoOut);
}

void ChapterLogic::showStage(E2L_SHOW_STAGE info)
{
    currStage = info.stageId;
    
    L2E_SHOW_STAGE infoOut;
    infoOut.eProtocol = l2e_show_stage;
    infoOut.stageId = currStage;
    infoOut.nameImg = data->stageConfigMap[currStage].imgFile;
    int job = Account::instance()->getData().currJob;
    if (currStage == 10004) {
        //哲信要求第4关统一用剑姬
        job = 2;
    }
    infoOut.heroImg = GameUtils::format("choose0%d.png",job);
    infoOut.currVIT = vit->getTotalVIT();
    infoOut.maxVIT = vit->getMaxBaseVIT();
    infoOut.boundGold = data->stageConfigMap[currStage].boundsGold;
    infoOut.boundExp = data->stageConfigMap[currStage].boundsExp;
    infoOut.firstGold = 0;
    infoOut.firstExp = 0;
    if(data->stageSaveMap[currStage].star[0] == 0) {
        infoOut.firstGold = data->stageConfigMap[currStage].firstGold;
        infoOut.firstExp = data->stageConfigMap[currStage].firstExp;
    }
    int starThres[3];
    starThres[0] = data->stageConfigMap[currStage].starSecond;
    starThres[1] = data->stageConfigMap[currStage].starHpPer;
    starThres[2] = data->stageConfigMap[currStage].starUseBottle;
    
    memcpy(infoOut.star, data->stageSaveMap[currStage].star, sizeof(int)*3);
    memset(infoOut.starType, 0, sizeof(int)*3);
    memset(infoOut.thresVal, 0, sizeof(int)*3);
    int starCount = 1;
    for (int i= 0; i < 3; i++) {

        if (starThres[i] == 0) {
            continue;
        }
        if (i == 2) {
            infoOut.thresVal[starCount] = 0;
        }else{
            infoOut.thresVal[starCount] = starThres[i];
        }
        infoOut.starType[starCount] = i+1;
        starCount++;
        if (starCount == 3) {
            break;
        }
    }
    ClientLogic::instance()->pass2Engine(&infoOut);
}

void ChapterLogic::back2Stage()
{
    L2E_SHOW_STAGE infoOut;
    infoOut.eProtocol = l2e_show_stage;
    infoOut.stageId = currStage;
    infoOut.nameImg = data->stageConfigMap[currStage].imgFile;
    int job = Account::instance()->getData().currJob;
    if (currStage == 10004) {
        //哲信要求第4关统一用剑姬
        job = 2;
    }
    infoOut.heroImg = GameUtils::format("choose0%d.png",job);
    infoOut.currVIT = vit->getTotalVIT();
    infoOut.maxVIT = vit->getMaxBaseVIT();
    infoOut.boundGold = data->stageConfigMap[currStage].boundsGold;
    infoOut.boundExp = data->stageConfigMap[currStage].boundsExp;
    infoOut.firstGold = 0;
    infoOut.firstExp = 0;
    if(data->stageSaveMap[currStage].star[0] == 0) {
        infoOut.firstGold = data->stageConfigMap[currStage].firstGold;
        infoOut.firstExp = data->stageConfigMap[currStage].firstExp;
    }

    int starThres[3];
    starThres[0] = data->stageConfigMap[currStage].starSecond;
    starThres[1] = data->stageConfigMap[currStage].starHpPer;
    starThres[2] = data->stageConfigMap[currStage].starUseBottle;
    
    memcpy(infoOut.star, data->stageSaveMap[currStage].star, sizeof(int)*3);
    memset(infoOut.starType, 0, sizeof(int)*3);
    memset(infoOut.thresVal, 0, sizeof(int)*3);
    int starCount = 1;
    for (int i= 0; i < 3; i++) {
        
        if (starThres[i] == 0) {
            continue;
        }
        if (i == 2) {
            infoOut.thresVal[starCount] = 0;
        }else{
            infoOut.thresVal[starCount] = starThres[i];
        }
        infoOut.starType[starCount] = i+1;
        starCount++;
        if (starCount == 3) {
            break;
        }
    }
    ClientLogic::instance()->pass2Engine(&infoOut);
}

void ChapterLogic::startStage()
{
    currAbyss = 0;
    currTower = 0;
    currArena = 0;
    L2E_UPDATE_ONE_VALUE info;
    info.eProtocol = l2e_start_stage;
    info.value = 0;
    if (vit->getTotalVIT() < data->stageConfigMap[currStage].costVIT) {
        info.value = 1;
    }
    ClientLogic::instance()->pass2Engine(&info);
    
    if (info.value == 0) {
        // 哲信提出进战斗就扣体力，故增加
        vit->useVIT(data->stageConfigMap[currStage].costVIT);
        ClientLogic::instance()->ChangeState(GAME_STATE_FIGHT);
    }
    
    C2S_UMENG_TRACK umengInfo;
    umengInfo.eProtocol = c2s_umeng_track;
    umengInfo.eventName = "click_battle";
    umengInfo.count = 0;
    umengInfo.attrMap.clear();
    ClientLogic::instance()->pass2Service(&umengInfo);
    
    C2S_UMENG_STAGE_LEVEL umengStageInfo;
    umengStageInfo.eProtocol = c2s_umeng_enter_stage_level;
    umengStageInfo.stageName = Convert2String(currStage);
    ClientLogic::instance()->pass2Service(&umengStageInfo);
}

void ChapterLogic::openStarBox(E2L_UPDATE_ONE_VALUE info)
{
    int boxId = info.value;
    int errNo = data->openStarBox(data->getCurrChapter(), boxId);
    
    if (errNo == 0) {
        L2E_SHOW_GET infoGet;
        infoGet.eProtocol = l2e_show_get;
        memset(infoGet.count, 0, sizeof(int)*20);
        for (int i = 0; i < 3; i++) {
            int type = data->configMap[data->getCurrChapter()].boxBoundType[(boxId-1)*3+i];
            int count = data->configMap[data->getCurrChapter()].boxBoundCount[(boxId-1)*3+i];
            infoGet.frag[i] = false;
            switch (type) {
                case 1:
                {
                    infoGet.icon[i] = GameUtils::format(COMMON_DIR, "drop-16.png");
                    infoGet.name[i] = StringData::shared()->stringFromKey("gold");
                    Account::instance()->changeGold(count);
                    L2E_UPDATE_ONE_VALUE infoGold;
                    infoGold.eProtocol = l2e_update_gold;
                    infoGold.value = Account::instance()->getData().gold;
                    ClientLogic::instance()->pass2Engine(&infoGold);
                    
                    C2S_UMENG_TRACK infoUmeng;
                    infoUmeng.eProtocol = c2s_umeng_track;
                    infoUmeng.eventName = "get_gold";
                    infoUmeng.count = count;
                    infoUmeng.attrMap.clear();
                    ClientLogic::instance()->pass2Service(&infoUmeng);
                }
                    break;
                case 2:
                {
                    infoGet.icon[i] = GameUtils::format(COMMON_DIR, "tilibiao.png");
                    infoGet.name[i] = StringData::shared()->stringFromKey("VIT");
                    vit->addPlusVIT(count);
                }
                    break;
                case 3:
                {
                    infoGet.icon[i] = GameUtils::format(COMMON_DIR, "saodangquan.png");
                    infoGet.name[i] = StringData::shared()->stringFromKey("prov_ticket");
                    Proving::instance()->addTicket(count);
                }
                    break;
                case 4:
                {
                    infoGet.icon[i] = GameUtils::format(COMMON_DIR, "drop-17.png");
                    infoGet.name[i] = StringData::shared()->stringFromKey("diamond");
                    Account::instance()->changeDiamond(count);
                    L2E_UPDATE_ONE_VALUE infoDiamond;
                    infoDiamond.eProtocol = l2e_update_diamond;
                    infoDiamond.value = Account::instance()->getData().diamond;
                    ClientLogic::instance()->pass2Engine(&infoDiamond);
                }
                    break;
                default:
                    break;
            }
            
            infoGet.count[i] = count;
        }
        ClientLogic::instance()->pass2Engine(&infoGet);
        
        L2E_UPDATE_ONE_VALUE infoOut;
        infoOut.eProtocol = l2e_open_star_box;
        infoOut.value = boxId;
        ClientLogic::instance()->pass2Engine(&infoOut);
    }else if(errNo == 2) {
        L2E_SHOW_STAR_BOUNDS infoOut;
        infoOut.eProtocol = l2e_show_star_bounds;
        memset(infoOut.count, 0, sizeof(int)*3);
        for (int i = 0; i < 3; i++) {
            int type = data->configMap[data->getCurrChapter()].boxBoundType[(boxId-1)*3+i];
            int count = data->configMap[data->getCurrChapter()].boxBoundCount[(boxId-1)*3+i];
            switch (type) {
                case 1:
                {
                    infoOut.icon[i] = GameUtils::format(COMMON_DIR, "drop-16.png");
                }
                    break;
                case 2:
                {
                    infoOut.icon[i] = GameUtils::format(COMMON_DIR, "tilibiao.png");
                }
                    break;
                case 3:
                {
                    infoOut.icon[i] = GameUtils::format(COMMON_DIR, "saodangquan.png");
                }
                    break;
                case 4:
                {
                    infoOut.icon[i] = GameUtils::format(COMMON_DIR, "drop-17.png");
                }
                    break;
                default:
                    break;
            }
            infoOut.count[i] = count;
        }
        ClientLogic::instance()->pass2Engine(&infoOut);

    }
}

void ChapterLogic::checkCheckAbyss(E2L_UPDATE_ONE_VALUE info)
{
    currAbyss = 0;
    int abyssId = info.value;
    L2E_UPDATE_ONE_VALUE infoOut;
    infoOut.eProtocol = l2e_ready_start_abyss;
    infoOut.value = Abyss::instance()->checkStart(abyssId);
    int stageId = Abyss::instance()->abyssConfigMap[abyssId].stageId;
    if (vit->getTotalVIT() < Chapter::instance()->stageConfigMap[stageId].costVIT) {
        infoOut.value = 4;
    }
    if (infoOut.value == 0) {
        currStage = stageId;
        currAbyss = abyssId;
        Abyss::instance()->start(abyssId);
    }
    
    ClientLogic::instance()->pass2Engine(&infoOut);
}

void ChapterLogic::startAbyss()
{
    currArena = 0;
    currTower = 0;
    // 哲信提出进战斗就扣体力，故增加
    vit->useVIT(data->stageConfigMap[currStage].costVIT);
    ClientLogic::instance()->ChangeState(GAME_STATE_FIGHT);
    
    C2S_UMENG_TRACK umengInfo;
    umengInfo.eProtocol = c2s_umeng_track;
    umengInfo.eventName = "click_abyss";
    umengInfo.count = 0;
    umengInfo.attrMap.clear();
    ClientLogic::instance()->pass2Service(&umengInfo);
    
    C2S_UMENG_STAGE_LEVEL umengStageInfo;
    umengStageInfo.eProtocol = c2s_umeng_enter_stage_level;
    umengStageInfo.stageName = Convert2String(currStage);
    ClientLogic::instance()->pass2Service(&umengStageInfo);
}

void ChapterLogic::resetAbyss(E2L_UPDATE_ONE_VALUE info)
{
    int abyssId = info.value;
    int errNo = Abyss::instance()->resetAbyss(abyssId);
    if (errNo ==0){
        if(Account::instance()->getData().diamond < abyssResetPrice) {
            errNo = 3;
        }
    }
    L2E_RESET_ABYSS infoOut;
    infoOut.eProtocol = l2e_reset_abyss;
    infoOut.errNo = errNo;
    if (infoOut.errNo != 0) {
        ClientLogic::instance()->pass2Engine(&infoOut);
        return;
    }
    
    infoOut.abyssId = abyssId;
    infoOut.fightCount = Abyss::instance()->getAbyssData().fightCount[abyssId-1];
    ClientLogic::instance()->pass2Engine(&infoOut);
    
    Account::instance()->changeDiamond(abyssResetPrice);
    L2E_UPDATE_ONE_VALUE infoDiamond;
    infoDiamond.eProtocol = l2e_update_diamond;
    infoDiamond.value = Account::instance()->getData().diamond;
    ClientLogic::instance()->pass2Engine(&infoDiamond);
}

void ChapterLogic::showAyssReset(E2L_UPDATE_ONE_VALUE info)
{
    int abyssId = info.value;
    L2E_SHOW_ABYSS_RESET infoOut;
    infoOut.eProtocol = l2e_show_abyss_reset;
    infoOut.abyssId = abyssId;
    infoOut.price = abyssResetPrice;
    infoOut.resetCount = Abyss::instance()->getAbyssData().resetCount[abyssId-1];
    ClientLogic::instance()->pass2Engine(&infoOut);
}

void ChapterLogic::refreshAbyss()
{
    Abyss::instance()->refreshAbyss();
    
    L2T_COMMON info;
    info.eProtocol = l2t_refresh_abyss;
    ClientLogic::instance()->ProcessTimeResponse(&info);
}

void ChapterLogic::showTower()
{
    Chapter::instance()->readStageConfig(200);
    L2E_SHOW_HALL_TOWER info;
    info.eProtocol = l2e_show_hall_tower;
    info.totalStage = (int)tower->towerConfigMap.size();
    info.openCount = tower->getTowerData().openCount;
    info.currStage = tower->getTowerData().currTower;
    info.name = tower->towerConfigMap[info.currStage].name;
    info.animationFile = tower->towerConfigMap[info.currStage].animationFile;
    info.descript = tower->towerConfigMap[info.currStage].descirpt;
    int stageId = tower->towerConfigMap[info.currStage].stageId;
    int gemId = tower->towerConfigMap[info.currStage].gemId;
    info.rewardGemCount = tower->towerConfigMap[info.currStage].gemCount;
    info.rewardGold = data->stageConfigMap[stageId].boundsGold;
    info.rewardExp = data->stageConfigMap[stageId].boundsExp;
    info.rewardGemIcon = Arms::instance()->gemsConfigMap[gemId].icon;
    info.stageName = data->stageConfigMap[info.currStage].name;
    
    info.sweepState = tower->getTowerData().sweepState;
    info.speedupPrice = 50;
    info.sweepRewards.clear();
    if (info.sweepState != 0) {
        int rewardGold = 0;
        int rewardExp = 0;
        for (int i = tower->getTowerData().currTower; i < tower->getTowerData().openCount; i++) {
            int stageId = tower->towerConfigMap[i].stageId;
            rewardGold += data->stageConfigMap[stageId].boundsGold;
            rewardExp += data->stageConfigMap[stageId].boundsExp;
        }
        if (rewardGold > 0) {
            info.sweepRewards["drop-16.png"] = rewardGold;
        }
        if (rewardExp > 0) {
            info.sweepRewards["drop-18.png"] = rewardExp;
        }
        
        std::map<int, int> gems;
        tower->getSweepGems(gems);
        for (auto rewardGem : gems) {
            int gemId = rewardGem.first;
            std::string icon = Arms::instance()->gemsConfigMap[gemId].icon;
            info.sweepRewards[icon] = rewardGem.second;
        }
    }
    ClientLogic::instance()->pass2Engine(&info);
}

void ChapterLogic::showTowerSweep()
{
    L2E_SHOW_TOWER_SWEEP info;
    info.eProtocol = l2e_show_tower_sweep;
    info.errNo = tower->checkSweepTower();
    if (info.errNo != 0) {
        ClientLogic::instance()->pass2Engine(&info);
        return;
    }
    
    info.sweepState = tower->getTowerData().sweepState;
    info.speedupPrice = 50;
    info.sweepRewards.clear();
//    if (info.sweepState != 0) {
        int rewardGold = 0;
        int rewardExp = 0;
        for (int i = tower->getTowerData().currTower; i < tower->getTowerData().openCount; i++) {
            int stageId = tower->towerConfigMap[i].stageId;
            rewardGold += data->stageConfigMap[stageId].boundsGold;
            rewardExp += data->stageConfigMap[stageId].boundsExp;
        }
        if (rewardGold > 0) {
            info.sweepRewards["drop-16.png"] = rewardGold;
        }
        if (rewardExp > 0) {
            info.sweepRewards["drop-18.png"] = rewardExp;
        }
        
        std::map<int, int> gems;
        tower->getSweepGems(gems);
        for (auto rewardGem : gems) {
            int gemId = rewardGem.first;
            std::string icon = Arms::instance()->gemsConfigMap[gemId].icon;
            info.sweepRewards[icon] = rewardGem.second;
        }
//    }
    ClientLogic::instance()->pass2Engine(&info);
}
void ChapterLogic::resetTower()
{
    L2E_RESET_TOWER info;
    info.eProtocol = l2e_reset_tower;
    info.errNo = tower->resetTower();
    if (info.errNo != 0) {
        ClientLogic::instance()->pass2Engine(&info);
        return;
    }
    
    info.currStage = tower->getTowerData().currTower;
    info.name = tower->towerConfigMap[info.currStage].name;
    info.animationFile = tower->towerConfigMap[info.currStage].animationFile;
    info.descript = tower->towerConfigMap[info.currStage].descirpt;
    int stageId = tower->towerConfigMap[info.currStage].stageId;
    int gemId = tower->towerConfigMap[info.currStage].gemId;
    info.rewardGemCount = tower->towerConfigMap[info.currStage].gemCount;
    info.rewardGold = data->stageConfigMap[stageId].boundsGold;
    info.rewardExp = data->stageConfigMap[stageId].boundsExp;
    info.rewardGemIcon = Arms::instance()->gemsConfigMap[gemId].icon;
    info.stageName = data->stageConfigMap[info.currStage].name;
    ClientLogic::instance()->pass2Engine(&info);
}
void ChapterLogic::startTower()
{
    currAbyss = 0;
    currArena = 0;
    currTower = tower->getTowerData().currTower;
    currStage = tower->towerConfigMap[currTower].stageId;
    ClientLogic::instance()->ChangeState(GAME_STATE_FIGHT);
    
    C2S_UMENG_TRACK umengInfo;
    umengInfo.eProtocol = c2s_umeng_track;
    umengInfo.eventName = "click_tower";
    umengInfo.count = 0;
    umengInfo.attrMap.clear();
    ClientLogic::instance()->pass2Service(&umengInfo);
    
    C2S_UMENG_STAGE_LEVEL umengStageInfo;
    umengStageInfo.eProtocol = c2s_umeng_enter_stage_level;
    umengStageInfo.stageName = Convert2String(currStage);
    ClientLogic::instance()->pass2Service(&umengStageInfo);
}
void ChapterLogic::startTowerSweep()
{
    L2E_START_TOWER_SWEEP info;
    info.eProtocol = l2e_start_tower_sweep;
    
    info.errNo = tower->startSweepTower();
    if (info.errNo != 0) {
        ClientLogic::instance()->pass2Engine(&info);
        return;
    }
    
    info.sweepState = tower->getTowerData().sweepState;
    int currStage = tower->getTowerData().currTower;
    int openStage = tower->getTowerData().openCount;
    info.speedupPrice = (openStage-currStage)*sweepTowerPrice;
    info.sweepSec = (openStage-currStage)*sweepTowerTime;
    
    L2T_START_TOWER_SWEEP infoTime;
    infoTime.eProtocol = l2t_start_tower_sweep;
    infoTime.sweepSec = info.sweepSec;
    ClientLogic::instance()->pass2Time(&infoTime);
    
    ClientLogic::instance()->pass2Engine(&info);
}

void ChapterLogic::completeTowerSweep()
{
    L2E_COMPLETE_TOWER_SWEEP info;
    info.eProtocol = l2e_complet_tower_sweep;
    info.errNo = tower->endSweepTower();
    if (info.errNo != 0) {
        ClientLogic::instance()->pass2Engine(&info);
        return;
    }

    
    info.sweepState = tower->getTowerData().sweepState;
    ClientLogic::instance()->pass2Engine(&info);
}

void ChapterLogic::towerTakeReward()
{
    int rewardGold = 0;
    int rewardExp = 0;
    for (int i = tower->getTowerData().currTower; i < tower->getTowerData().openCount; i++) {
        int stageId = tower->towerConfigMap[i].stageId;
        rewardGold += data->stageConfigMap[stageId].boundsGold;
        rewardExp += data->stageConfigMap[stageId].boundsExp;
    }
    auto account = Account::instance();
    Account::instance()->changeGold(rewardGold);
    L2E_UPDATE_ONE_VALUE infoGold;
    infoGold.eProtocol = l2e_update_gold;
    infoGold.value = Account::instance()->getData().gold;
    ClientLogic::instance()->pass2Engine(&infoGold);
    
    C2S_UMENG_TRACK infoUmeng;
    infoUmeng.eProtocol = c2s_umeng_track;
    infoUmeng.eventName = "get_gold";
    infoUmeng.count = rewardGold;
    infoUmeng.attrMap.clear();
    ClientLogic::instance()->pass2Service(&infoUmeng);
    
    int originLevel = account->getData().level;
    account->addExp(rewardExp);
    int originForce = account->getForce();
    L2E_UPDATE_EXP infoExp;
    infoExp.eProtocol = l2e_update_exp;
    infoExp.currExp = account->getData().exp;
    infoExp.level = account->getData().level;
    if (infoExp.upLevel) {
        Equip::instance()->checkTip(infoExp.level);
        
        if (infoExp.level>=15 && originLevel < 15) {
            C2S_UMENG_TRACK umengLevelInfo;
            umengLevelInfo.eProtocol = c2s_umeng_track;
            umengLevelInfo.eventName = "user_up_level";
            umengLevelInfo.count = 0;
            umengLevelInfo.attrMap.clear();
            umengLevelInfo.attrMap["level"] = Convert2String(15);
            ClientLogic::instance()->pass2Service(&umengLevelInfo);
        }
        
        if (infoExp.level>=10 && originLevel < 10) {
            C2S_UMENG_TRACK umengLevelInfo;
            umengLevelInfo.eProtocol = c2s_umeng_track;
            umengLevelInfo.eventName = "user_up_level";
            umengLevelInfo.count = 0;
            umengLevelInfo.attrMap.clear();
            umengLevelInfo.attrMap["level"] = Convert2String(10);
            ClientLogic::instance()->pass2Service(&umengLevelInfo);
        }
        if (infoExp.level>=5 && originLevel < 5) {
            C2S_UMENG_TRACK umengLevelInfo;
            umengLevelInfo.eProtocol = c2s_umeng_track;
            umengLevelInfo.eventName = "user_up_level";
            umengLevelInfo.count = 0;
            umengLevelInfo.attrMap.clear();
            umengLevelInfo.attrMap["level"] = Convert2String(5);
            ClientLogic::instance()->pass2Service(&umengLevelInfo);
        }

    }
    if (account->isMaxLevel()) {
        infoExp.currExp = 100;
        infoExp.maxExp = 100;
    }else{
        int baseExp = 0;
        if (infoExp.level > 1) {
            baseExp = account->uplevelConfigMap[infoExp.level-1].expThres;
        }
        int nextExp = account->uplevelConfigMap[infoExp.level].expThres;
        infoExp.currExp = account->getData().exp - baseExp;
        infoExp.maxExp = nextExp-baseExp;
        //        infoExp.currExp = account->getData().exp;
        //        infoExp.maxExp = account->uplevelConfigMap[infoExp.level].expThres;
    }
    
    infoExp.hp = account->getData().hp;
    infoExp.attack = account->getData().attack;
    infoExp.defence = account->getData().defence;
    infoExp.crit = account->getData().crit;
    infoExp.deCrit = account->getData().deCrit;
    infoExp.force = account->getForce();
    ClientLogic::instance()->pass2Engine(&infoExp);
    

    
    L2E_UPDATE_FORCE infoForce;
    infoForce.eProtocol = l2e_update_force;
    infoForce.forceVal = account->getForce();
    infoForce.upVal = infoForce.forceVal-originForce;
    ClientLogic::instance()->pass2Engine(&infoForce);

    
    std::map<int, int> gems;
    tower->getSweepGems(gems);
    for (auto gem :gems) {
        
        int count = gem.second;
        int gemId = gem.first;
        Arms::instance()->addGem(gemId, count);
    }
    
    L2E_SHOW_GET infoGet;
    infoGet.eProtocol = l2e_show_get;
    memset(infoGet.count, 0, sizeof(int)*20);
    infoGet.frag[0] = false;
    infoGet.icon[0] =  GameUtils::format(COMMON_DIR, "drop-16.png");
    infoGet.count[0] = rewardGold;

    int index = 1;
    for(std::map<int,int>::reverse_iterator gem=gems.rbegin();gem!=gems.rend();gem++)
    {
        int count = (*gem).second;
        int gemId = (*gem).first;
        infoGet.count[index] = count;
        infoGet.frag[index] = false;
        infoGet.icon[index] = GameUtils::format(COMMON_DIR, Arms::instance()->gemsConfigMap[gemId].icon.c_str());
        infoGet.name[index] = StringData::shared()->stringFromKey("gem");
        index++;
        if (index >= 20) {
            break;
        }
    }
//        cout<<(*rit).first<<","<<(*rit).second<<endl;
//    for (auto gem :gems) {
//        
//        int count = gem.second;
//        int gemId = gem.first;
//        infoGet.count[index] = count;
//        infoGet.frag[index] = false;
//        infoGet.icon[index] = GameUtils::format(COMMON_DIR, Arms::instance()->gemsConfigMap[gemId].icon.c_str());
//        index++;
//        if (index >= 20) {
//            break;
//        }
//    }
    ClientLogic::instance()->pass2Engine(&infoGet);
    
    Arms::instance()->checkTip(Account::instance()->getData().fame);
    L2E_UPDATE_HALL_TIP infoTip;
    infoTip.eProtocol = l2e_update_hall_tip;
    infoTip.tipIndex = 4;
    infoTip.tipValue = Arms::instance()->getTip();
    ClientLogic::instance()->pass2Engine(&infoTip);
    
    L2E_TOWER_TAKE_REWARD info;
    info.eProtocol = l2e_tower_take_reward;
    
    info.errNo = tower->takeSweepReward();
    if (info.errNo != 0) {
        ClientLogic::instance()->pass2Engine(&info);
        return;
    }
    
    
    info.sweepState = tower->getTowerData().sweepState;
    info.currStage = tower->getTowerData().currTower;
    info.name = tower->towerConfigMap[info.currStage].name;
    info.animationFile = tower->towerConfigMap[info.currStage].animationFile;
    info.descript = tower->towerConfigMap[info.currStage].descirpt;
    int stageId = tower->towerConfigMap[info.currStage].stageId;
    int gemId = tower->towerConfigMap[info.currStage].gemId;
    info.rewardGemCount = tower->towerConfigMap[info.currStage].gemCount;
    info.rewardGold = data->stageConfigMap[stageId].boundsGold;
    info.rewardExp = data->stageConfigMap[stageId].boundsExp;
    info.rewardGemIcon = Arms::instance()->gemsConfigMap[gemId].icon;
    info.stageName = data->stageConfigMap[info.currStage].name;
    ClientLogic::instance()->pass2Engine(&info);
    
}

void ChapterLogic::speedupTowerSweep()
{
    int currStage = tower->getTowerData().currTower;
    int openStage = tower->getTowerData().openCount;
    int speedupPrice = (openStage-currStage)*sweepTowerPrice;
    
    L2E_SPEEDUP_TOWER_SWEEP info;
    info.eProtocol = l2e_speedup_tower_sweep;
    if (Account::instance()->getData().diamond < speedupPrice) {
        info.errNo = 3;
        ClientLogic::instance()->pass2Engine(&info);
        return;
    }
    
    info.errNo = tower->endSweepTower();
    if (info.errNo != 0) {
        ClientLogic::instance()->pass2Engine(&info);
        return;
    }
    
    info.sweepState = tower->getTowerData().sweepState;
    ClientLogic::instance()->pass2Engine(&info);
    
    L2T_COMMON infoTime;
    infoTime.eProtocol = l2t_stop_tower_sweep;
    ClientLogic::instance()->pass2Time(&infoTime);

}

void ChapterLogic::refreshTowerReset()
{
    tower->refreshTower();
    L2T_COMMON infoTime;
    infoTime.eProtocol = l2t_refresh_tower_reset;
    ClientLogic::instance()->pass2Time(&infoTime);
}

void ChapterLogic::getTowerSweepCD()
{
    L2T_COMMON infoTime;
    infoTime.eProtocol = l2t_get_tower_sweep_cd;
    ClientLogic::instance()->pass2Time(&infoTime);
}

void ChapterLogic::updateTowerSweepCD(T2L_COMMON infoTime)
{
    L2E_UPDATE_ONE_VALUE info;
    info.eProtocol = l2e_update_tower_sweep_sec;
    info.value = infoTime.passSec;
    ClientLogic::instance()->pass2Engine(&info);
}

void ChapterLogic::startArenaFight()
{
    Chapter::instance()->readStageConfig(300);
    currAbyss = 0;
    currTower = 0;
    currArena = 1;
    currStage = 50000;
    ClientLogic::instance()->ChangeState(GAME_STATE_FIGHT);
    
    C2S_UMENG_TRACK umengInfo;
    umengInfo.eProtocol = c2s_umeng_track;
    umengInfo.eventName = "click_arena";
    umengInfo.count = 0;
    umengInfo.attrMap.clear();
    ClientLogic::instance()->pass2Service(&umengInfo);
    
    C2S_UMENG_STAGE_LEVEL umengStageInfo;
    umengStageInfo.eProtocol = c2s_umeng_enter_stage_level;
    umengStageInfo.stageName = Convert2String(currStage);
    ClientLogic::instance()->pass2Service(&umengStageInfo);
}
