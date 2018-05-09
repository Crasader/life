//
//  GameCore.cpp
//  Dabao
//
//  Created by ff on 17/3/3.
//
//

#include "GameCore.h"
#include "ClientLogic.h"
#include "SystemLogic.h"
#include "ELProtocol.h"
#include "GameDefine.h"
#include "data/MonsterWave.h"
#include "data/Chapter.h"
#include "data/VIT.h"
#include "data/HolySword.h"
#include "data/Pet.h"
#include "data/Abyss.h"
#include "data/Tower.h"
#include "data/Arms.h"
#include "data/Arena.h"
#include "data/Campaign.h"

USING_NS_CC;

GameCore::GameCore() :
currState(FIGHT_GAME_NONE),
lastState(FIGHT_GAME_NONE),
originState(FIGHT_GAME_NONE)
{
    account = Account::instance();
    currState = FIGHT_GAME_NONE;
    lastState = FIGHT_GAME_NONE;
    originState = FIGHT_GAME_NONE;
    battleField = BattleField::instance();
//    fighterManager = FighterManager::instance();
}

GameCore::~GameCore()
{
    currState = FIGHT_GAME_NONE;
    lastState = FIGHT_GAME_NONE;
    originState = FIGHT_GAME_NONE;
}

void GameCore::changeGameState(FIGHT_GAME_STATES nextState)
{
    if(currState == nextState)
        return;
    log("change state from %d to %d", currState, nextState);
    exitGameState(currState);
    lastState	= currState;
    currState		= nextState;
    enterGameState(nextState);
}

void GameCore::enterGameState(FIGHT_GAME_STATES nextState)
{
    switch (nextState) {
        case FIGHT_GAME_LOAD:
        {
            addLoadFunc(Load::LoadFunc(&GameCore::loadBattleField));
            addLoadFunc(Load::LoadFunc(&GameCore::loadTmxMap));
            addLoadFunc(Load::LoadFunc(&GameCore::loadTmxEffect));
            addLoadFunc(Load::LoadFunc(&GameCore::loadHero));
            addLoadFunc(Load::LoadFunc(&GameCore::loadWitness));
            if (arenaId > 0) {
                addLoadFunc(Load::LoadFunc(&GameCore::loadOpponent));
            }
            addLoadFunc(Load::LoadFunc(&GameCore::preLoadMonsterRes));
            addLoadFunc(Load::LoadFunc(&GameCore::startMonsterTeam));
            stageResult = 0;
            useBottleCount = 1;
            L2E_SHOW_LOAD info;
            info.eProtocol = l2e_show_load;
            if(Campaign::instance()->getCoreData().everydayId == 1)
            {
                info.loadTipImg = "loading1.png";
            }else{
                info.loadTipImg = "loading1.png";
            }
            ClientLogic::instance()->pass2Engine(&info);
        }
            break;
        case FIGHT_GAME_INIT:
        {
            std::string tmxPath = GameUtils::format(TMX_DIR, battleField->getConfig().tmx.c_str());
            
            std::string weatherPath = GameUtils::format(WEATHER_DIR, battleField->getConfig().weather.c_str());
            if (battleField->getConfig().weather == "null") {
                weatherPath = "null";
            }
            L2E_SHOW_FIGHT info;
            info.eProtocol = l2e_show_fight;
            info.tmxPath = tmxPath;
            info.weatherPath = weatherPath;
            info.goalStr = Chapter::instance()->stageConfigMap[stageId].name;
            info.musicFile = Chapter::instance()->stageConfigMap[stageId].musicFile;
            info.totalTime = battleField->getConfig().secondLimit;
            if (battleField->getConfig().condition <= 3) {
                info.currWave = battleField->getCurrWaveCount();
                info.totalWave = battleField->getConfig().waveCount;
            } else {
                info.currWave = 0;
                info.totalWave = 0;
            }
            info.battleType = battleField->getConfig().condition;
            info.bossTeamId = battleField->getConfig().bossTeamId;
            if (abyssId != 0 || towerId != 0 || arenaId != 0) {
                info.stageFight = false;
            }else{
                info.stageFight = true;
            }
            
            ClientLogic::instance()->pass2Engine(&info);
        }
            break;
        default:
            break;
    }
}

void GameCore::exitGameState(FIGHT_GAME_STATES state)
{
    switch (state) {
        default:
            break;
    }
}

void GameCore::backGameState()
{
    exitGameState(currState);
    currState = lastState;
    log("back to currState: %d", currState);
}

void GameCore::onLoadFinish()
{
    Load::onLoadFinish();
    //加载完成，转入登陆状态
    changeGameState(FIGHT_GAME_INIT);
}

void GameCore::load()
{
    L2E_UPDATE_LOAD info;
    info.eProtocol = l2e_update_load_stage;
    info.isOver = false;
    info.step = 0;
    info.count = (int)m_vecLoadFunc.size();
    int result = Load::load();
    if (result != -1) {
        info.step = result;
        //更新加载界面
        ClientLogic::instance()->pass2Engine(&info);
    }else{
        onLoadFinish();
    }
}

void GameCore::loadBattleField()
{
    battleField->setStageId(stageId);
//    battleField->readFile(BATTLE_FIELD_CONFIG);
    battleField->readBattle(stageId);
}

void GameCore::loadTmxMap()
{
    L2E_SETUP_MAP info;
    info.eProtocol = l2e_setup_map;
    std::string tmxPath = GameUtils::format(TMX_DIR, battleField->getConfig().tmx.c_str());
    info.tmxPath = tmxPath;
    ClientLogic::instance()->pass2Engine(&info);
}
void GameCore::loadTmxEffect()
{
    L2E_SETUP_MAP info;
    info.eProtocol = l2e_setup_map_effect;
    std::string tmxPath = GameUtils::format(TMX_DIR, battleField->getConfig().tmx.c_str());
    info.tmxPath = tmxPath;
    ClientLogic::instance()->pass2Engine(&info);
}

void GameCore::loadHero()
{
    L2E_SETUP_HERO info;
    info.eProtocol = l2e_setup_hero;
    std::string tmxPath = GameUtils::format(TMX_DIR, battleField->getConfig().tmx.c_str());
    info.tmxPath = tmxPath;
    int job = account->getData().currJob;
    if (stageId == 10004) {
        //哲信要求第4关统一用剑姬
        job = 2;
    }
    info.configId = account->jobConfigMap[job].fighterId;
    info.hp = account->getData().hp;
    info.attack = account->getData().attack;
    info.defence = account->getData().defence;
    info.crit = account->getData().crit;
    info.deCrit = account->getData().deCrit;
    memcpy(info.buffId, battleField->getConfig().buffId, sizeof(int)*5);
    if (account->unlockMap[7].lock == 1) {
        info.holySwordId = 0;
    }else{
        int holySwordIdx = HolySword::instance()->getInfo().fightSwordId;
        if (holySwordIdx > 0) {
            info.holySwordId = HolySword::instance()->swordConfigMap[holySwordIdx].fighterId;
        }else{
            info.holySwordId = 0;
        }
    }
    for (int i = 0; i < 3; i++) {
        int pet = Pet::instance()->getInfo().fightPet[i];
        if (pet <= 0) {
            info.petId[i] = 0;
        }else{
            info.petId[i] = Pet::instance()->petConfigMap[pet].fighterId;
        }
    }
    
//    FIGHTER_DATA fighterData;
//    fighterManager->loadHero(1, fighterData);
//    info.idInStage = fighterData.idInStage;
//    info.armatureFile = fighterData.armatureFile;
//    info.scaleFactor = fighterData.scaleFactor;
//    info.moveSpeed = fighterData.moveSpeed;
//    info.rushSpeed = fighterData.rushSpeed;
    ClientLogic::instance()->pass2Engine(&info);
    
}

void GameCore::loadWitness()
{
    L2E_SETUP_WITNESS info;
    info.eProtocol = l2e_setup_witness;
    std::string tmxPath = GameUtils::format(TMX_DIR, battleField->getConfig().tmx.c_str());
    info.tmxPath = tmxPath;
    info.configId = battleField->getConfig().protectorId;
    ClientLogic::instance()->pass2Engine(&info);
}

void GameCore::loadOpponent()
{
    auto arena = Arena::instance();
    
    L2E_SETUP_OPPONENT info;
    info.eProtocol = l2e_setup_opponent;
    info.jobId = arena->getOppRobot().jobId;
    info.fighterId = account->jobConfigMap[info.jobId].opponentId;
    info.name = arena->getOppRobot().name;
    info.hp = arena->getOppRobot().hp;
    info.attack = arena->getOppRobot().attack;
    info.defence = arena->getOppRobot().defence;
    info.crit = arena->getOppRobot().crit;
    info.decrit = arena->getOppRobot().decrit;
    int holyIdx = arena->getOppRobot().holySwordId;
    info.holySwordId = HolySword::instance()->swordConfigMap[holyIdx].fighterId;
    for (int i = 0; i < 3; i++) {
        if (arena->getOppRobot().petId[i] == 0) {
            info.petId[i] = 0;
        }else{
            info.petId[i] = Pet::instance()->petConfigMap[arena->getOppRobot().petId[i]].fighterId;
        }
    }
//    memcpy(info.petId, arena->getOppRobot().petId, sizeof(int)*3);
    ClientLogic::instance()->pass2Engine(&info);
}

void GameCore::preLoadMonsterRes()
{
    L2E_PRELOAD_MONSTER_RES info;
    info.eProtocol = l2e_preload_monster_res;
    std::string tmxPath = GameUtils::format(TMX_DIR, battleField->getConfig().tmx.c_str());
    info.tmxPath = tmxPath;
    ClientLogic::instance()->pass2Engine(&info);
}

void GameCore::startMonsterTeam()
{
    battleField->load1stMonsterWave();
}

void GameCore::plotOpenSendMonster(int val, int missionId)
{
    MonsterWave::instance()->setPlotStop(val);
    
    E2L_COMPLETE_PLOT info;
    info.eProtocol = e2l_complete_plot;
    info.missionId = missionId;
    info.value = 0;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void GameCore::logic(float dt)
{
    switch (currState) {
        case FIGHT_GAME_LOAD:
        {
            load();
        }
            break;
        case FIGHT_GAME_NORMAL:
        {
            battleField->update(dt);
        }
            break;
        default:
            break;
    }
    
    if (Campaign::instance()->addDiamond(dt)) {
        Campaign::instance()->enableFreeDiamond();
    }
}

void GameCore::unlockView()
{
    L2E_COMMON info;
    info.eProtocol = l2e_unlock_view;
    ClientLogic::instance()->pass2Engine(&info);
}

void GameCore::monsterHpEmpty(E2L_MONSTER_HP_EMPTY info)
{
    if (stageResult != 0) {
        return;
    }
    if (arenaId == 0) {
        Campaign::instance()->activeBoundMission(11, 0, 0);
        Campaign::instance()->checkTip();
        L2E_UPDATE_HALL_TIP infoTip;
        infoTip.eProtocol = l2e_update_hall_tip;
        infoTip.tipIndex = 7;
        infoTip.tipValue = Campaign::instance()->getTip();
        ClientLogic::instance()->pass2Engine(&infoTip);
    }
    E2L_TRIGGER_KILL_MONSTER infoTrigger;
    infoTrigger.eProtocol = e2l_trigger_kill_monster;
    infoTrigger.monsterId = info.monId;
    SystemLogic::instance()->triggerKillMonster(infoTrigger);
//    if (SystemLogic::instance()->triggerKillMonster(infoTrigger)) {
//        return;
//    }

    if (battleField->getConfig().condition == 3 || battleField->getConfig().condition == 6) {
        if (info.teamId == battleField->getConfig().bossTeamId) {
            L2E_COMMON infoOut;
            infoOut.eProtocol = l2e_ready_victory;
            ClientLogic::instance()->pass2Engine(&infoOut);
            MonsterWave::instance()->clearWaitTeamAndMonster();
            return;
        }
    }
    bool waveOver = MonsterWave::instance()->someOneDead(info.teamId);
    if (waveOver) {
        int nextWaveId = battleField->getNextWaveId();
        if (nextWaveId == 0) {
            L2E_COMMON infoOut;
            infoOut.eProtocol = l2e_ready_victory;
            ClientLogic::instance()->pass2Engine(&infoOut);
            MonsterWave::instance()->clearWaitTeamAndMonster();
            return;
        }
        battleField->change2Wave(nextWaveId);
        
        if (battleField->getConfig().condition <= 3) {
            L2E_UPDATE_WAVE_COUNT infoOut;
            infoOut.eProtocol = l2e_update_wave_count;
            infoOut.curr = battleField->getCurrWaveCount();
            infoOut.total = battleField->getConfig().waveCount;
            ClientLogic::instance()->pass2Engine(&infoOut);
        }
        
    }
}

void GameCore::monsterDie(E2L_MONSTER_DIE info)
{
    if (stageResult != 0) {
        return;
    }
    
    if (battleField->getConfig().condition == 3 || battleField->getConfig().condition == 6) {
        if (info.teamId == battleField->getConfig().bossTeamId) {
            stageResult = 1;
            if (arenaId != 0) {
                int originDiv = Arena::instance()->getCoreData().divisionId;
                account->changeGold(Arena::instance()->divistionConfigMap[originDiv].winGold);
                
                C2S_UMENG_TRACK infoUmeng;
                infoUmeng.eProtocol = c2s_umeng_track;
                infoUmeng.eventName = "get_gold";
                infoUmeng.count = Arena::instance()->divistionConfigMap[originDiv].winGold;
                infoUmeng.attrMap.clear();
                ClientLogic::instance()->pass2Service(&infoUmeng);
                
                int result = Arena::instance()->win();
                Campaign::instance()->activeMission(6, 1);
                Campaign::instance()->activeBoundMission(8, 0, Arena::instance()->getCoreData().score);
                
                
                L2E_ARENA_RESULT infoOut;
                infoOut.eProtocol = l2e_arena_victory;
                infoOut.deltaScore = 30;
                infoOut.totalScore = Arena::instance()->getCoreData().score;
                infoOut.changeDivision = ((originDiv==Arena::instance()->getCoreData().divisionId)?0:1);
                infoOut.gold = Arena::instance()->divistionConfigMap[originDiv].winGold;
                infoOut.divisionIcon1 = Arena::instance()->divistionConfigMap[Arena::instance()->getCoreData().divisionId].icon1;
                infoOut.divisionIcon2 = Arena::instance()->divistionConfigMap[Arena::instance()->getCoreData().divisionId].icon2;
                ClientLogic::instance()->pass2Engine(&infoOut);
                
                Campaign::instance()->checkTip();
                L2E_UPDATE_HALL_TIP infoTip;
                infoTip.eProtocol = l2e_update_hall_tip;
                infoTip.tipIndex = 7;
                infoTip.tipValue = Campaign::instance()->getTip();
                ClientLogic::instance()->pass2Engine(&infoTip);
                
                if (result == 2) {
                    SystemLogic::instance()->updateArenaRank();
                }
            }else{
                L2E_UPDATE_ONE_VALUE infoOut;
                infoOut.eProtocol = l2e_victory;
                if (abyssId != 0 || arenaId != 0) {
                    infoOut.value = 0;
                }else if (towerId != 0) {
                    infoOut.value = 2;
                }else{
                    infoOut.value = 1;
                }
    //            infoOut.value = ((abyssId == 0)?1:0);
                C2S_UMENG_STAGE_LEVEL umengStageInfo;
                umengStageInfo.eProtocol = c2s_umeng_finish_stage_level;
                umengStageInfo.stageName = Convert2String(battleField->getConfig().stageId);
                ClientLogic::instance()->pass2Service(&umengStageInfo);
                
                ClientLogic::instance()->pass2Engine(&infoOut);
            }
            MonsterWave::instance()->clearWaitTeamAndMonster();
            return;
        }
    }
    if (battleField->getWaveOver()) {
        stageResult = 1;
        if (arenaId != 0) {
            int originDiv = Arena::instance()->getCoreData().divisionId;
            account->changeGold(Arena::instance()->divistionConfigMap[originDiv].winGold);
            
            C2S_UMENG_TRACK infoUmeng;
            infoUmeng.eProtocol = c2s_umeng_track;
            infoUmeng.eventName = "get_gold";
            infoUmeng.count = Arena::instance()->divistionConfigMap[originDiv].winGold;
            infoUmeng.attrMap.clear();
            ClientLogic::instance()->pass2Service(&infoUmeng);

            int result = Arena::instance()->win();
            Campaign::instance()->activeMission(6, 1);
            Campaign::instance()->activeBoundMission(8, 0, Arena::instance()->getCoreData().score);
            L2E_ARENA_RESULT infoOut;
            infoOut.eProtocol = l2e_arena_victory;
            infoOut.deltaScore = 30;
            infoOut.totalScore = Arena::instance()->getCoreData().score;
            infoOut.changeDivision = ((originDiv==Arena::instance()->getCoreData().divisionId)?0:1);
            infoOut.gold = Arena::instance()->divistionConfigMap[originDiv].winGold;
            infoOut.divisionIcon1 = Arena::instance()->divistionConfigMap[Arena::instance()->getCoreData().divisionId].icon1;
            infoOut.divisionIcon2 = Arena::instance()->divistionConfigMap[Arena::instance()->getCoreData().divisionId].icon2;
            ClientLogic::instance()->pass2Engine(&infoOut);
            
            Campaign::instance()->checkTip();
            L2E_UPDATE_HALL_TIP infoTip;
            infoTip.eProtocol = l2e_update_hall_tip;
            infoTip.tipIndex = 7;
            infoTip.tipValue = Campaign::instance()->getTip();
            ClientLogic::instance()->pass2Engine(&infoTip);
            
            if (result == 2) {
                SystemLogic::instance()->updateArenaRank();
            }
        }else{
            L2E_UPDATE_ONE_VALUE infoOut;
            infoOut.eProtocol = l2e_victory;
            if (abyssId != 0 || arenaId != 0) {
                infoOut.value = 0;
            }else if (towerId != 0) {
                infoOut.value = 2;
            }else{
                infoOut.value = 1;
            }
            //            infoOut.value = ((abyssId == 0)?1:0);
            
            C2S_UMENG_STAGE_LEVEL umengStageInfo;
            umengStageInfo.eProtocol = c2s_umeng_finish_stage_level;
            umengStageInfo.stageName = Convert2String(battleField->getConfig().stageId);
            ClientLogic::instance()->pass2Service(&umengStageInfo);
            
            ClientLogic::instance()->pass2Engine(&infoOut);
        }
        MonsterWave::instance()->clearWaitTeamAndMonster();
    }
}

void GameCore::witnessDie()
{
    if (stageResult != 0) {
        return;
    }
    if (battleField->getConfig().condition == 4 || battleField->getConfig().condition == 5) {
        stageResult = 2;
        L2E_COMMON infoOut;
        infoOut.eProtocol = l2e_fail;
        ClientLogic::instance()->pass2Engine(&infoOut);
        
        C2S_UMENG_STAGE_LEVEL umengStageInfo;
        umengStageInfo.eProtocol = c2s_umeng_fail_stage_level;
        umengStageInfo.stageName = Convert2String(battleField->getConfig().stageId);
        ClientLogic::instance()->pass2Service(&umengStageInfo);
        
        MonsterWave::instance()->clearWaitTeamAndMonster();
    }
}
void GameCore::heroDie()
{
    if (stageResult != 0) {
        return;
    }
//    MonsterWave::instance()->clearWaitTeamAndMonster();
    
    stageResult = 2;
    if (arenaId != 0)
    {
        int originDiv = Arena::instance()->getCoreData().divisionId;
        account->changeGold(Arena::instance()->divistionConfigMap[originDiv].loseGold);
        
        C2S_UMENG_TRACK infoUmeng;
        infoUmeng.eProtocol = c2s_umeng_track;
        infoUmeng.eventName = "get_gold";
        infoUmeng.count = Arena::instance()->divistionConfigMap[originDiv].loseGold;
        infoUmeng.attrMap.clear();
        ClientLogic::instance()->pass2Service(&infoUmeng);

        int result = Arena::instance()->lose();
        L2E_ARENA_RESULT infoOut;
        infoOut.eProtocol = l2e_arena_fail;
        infoOut.deltaScore = 10;
        infoOut.totalScore = Arena::instance()->getCoreData().score;
        infoOut.changeDivision = ((originDiv==Arena::instance()->getCoreData().divisionId)?0:1);
        infoOut.gold = Arena::instance()->divistionConfigMap[originDiv].loseGold;
        infoOut.divisionIcon1 = Arena::instance()->divistionConfigMap[Arena::instance()->getCoreData().divisionId].icon1;
        infoOut.divisionIcon2 = Arena::instance()->divistionConfigMap[Arena::instance()->getCoreData().divisionId].icon2;
        ClientLogic::instance()->pass2Engine(&infoOut);
        
        if (result == 2) {
            SystemLogic::instance()->updateArenaRank();
        }
        MonsterWave::instance()->clearWaitTeamAndMonster();
    }else{
//        L2E_COMMON infoOut;
//        infoOut.eProtocol = l2e_fail;
//        ClientLogic::instance()->pass2Engine(&infoOut);
        pauseFight();
        L2E_COMMON infoOut;
        infoOut.eProtocol = l2e_show_revive;
        ClientLogic::instance()->pass2Engine(&infoOut);
    }
//    MonsterWave::instance()->clearWaitTeamAndMonster();
}

void GameCore::notRevive()
{
    resumeFight();
    L2E_COMMON infoOut;
    infoOut.eProtocol = l2e_fail;
    ClientLogic::instance()->pass2Engine(&infoOut);
    
    C2S_UMENG_STAGE_LEVEL umengStageInfo;
    umengStageInfo.eProtocol = c2s_umeng_fail_stage_level;
    umengStageInfo.stageName = Convert2String(battleField->getConfig().stageId);
    ClientLogic::instance()->pass2Service(&umengStageInfo);
    
    MonsterWave::instance()->clearWaitTeamAndMonster();
}

void GameCore::revive()
{
    resumeFight();
    stageResult = 0;
    L2E_COMMON info;
    info.eProtocol = l2e_revive;
    ClientLogic::instance()->pass2Engine(&info);
}

void GameCore::fightTimeOver()
{
    if (stageResult != 0) {
        return;
    }
    MonsterWave::instance()->clearWaitTeamAndMonster();
    
    if (battleField->getConfig().condition == 4 || battleField->getConfig().condition == 5) {
        stageResult = 1;
        L2E_UPDATE_ONE_VALUE infoOut;
        infoOut.eProtocol = l2e_victory;
        if (abyssId != 0 || arenaId != 0) {
            infoOut.value = 0;
        }else if (towerId != 0) {
            infoOut.value = 2;
        }else{
            infoOut.value = 1;
        }
        //            infoOut.value = ((abyssId == 0)?1:0);
        
        C2S_UMENG_STAGE_LEVEL umengStageInfo;
        umengStageInfo.eProtocol = c2s_umeng_finish_stage_level;
        umengStageInfo.stageName = Convert2String(battleField->getConfig().stageId);
        ClientLogic::instance()->pass2Service(&umengStageInfo);
        
        ClientLogic::instance()->pass2Engine(&infoOut);
        return;
    }
    
    stageResult = 3;
    L2E_FIGHT_TIME_OVER infoOut;
    infoOut.eProtocol = l2e_fight_time_over;
    if (battleField->getConfig().condition == 6) {
        infoOut.teamId = battleField->getConfig().bossTeamId;
    }else{
        infoOut.teamId = 0;
    }
    ClientLogic::instance()->pass2Engine(&infoOut);
}
void GameCore::fightTimeOver2(E2L_FIGHT_TIME_OVER2 info)
{
    if (stageResult != 3) {
        return;
    }

    if (arenaId != 0)
    {
        float heroHpPercent = (float)info.heroHp*100.0/info.heroMaxHp;
        float opponentHpPercent = (float)info.opponentHp*100.0/info.opponentMaxHp;
        if (heroHpPercent >= opponentHpPercent) {
            stageResult = 1;
            int originDiv = Arena::instance()->getCoreData().divisionId;
            account->changeGold(Arena::instance()->divistionConfigMap[originDiv].winGold);
            
            C2S_UMENG_TRACK infoUmeng;
            infoUmeng.eProtocol = c2s_umeng_track;
            infoUmeng.eventName = "get_gold";
            infoUmeng.count = Arena::instance()->divistionConfigMap[originDiv].winGold;
            infoUmeng.attrMap.clear();
            ClientLogic::instance()->pass2Service(&infoUmeng);
            
            int result = Arena::instance()->win();
            Campaign::instance()->activeMission(6, 1);
            Campaign::instance()->activeBoundMission(8, 0, Arena::instance()->getCoreData().score);
            L2E_ARENA_RESULT infoOut;
            infoOut.eProtocol = l2e_arena_victory;
            infoOut.deltaScore = 30;
            infoOut.totalScore = Arena::instance()->getCoreData().score;
            infoOut.changeDivision = ((originDiv==Arena::instance()->getCoreData().divisionId)?0:1);
            infoOut.gold = Arena::instance()->divistionConfigMap[originDiv].winGold;
            infoOut.divisionIcon1 = Arena::instance()->divistionConfigMap[Arena::instance()->getCoreData().divisionId].icon1;
            infoOut.divisionIcon2 = Arena::instance()->divistionConfigMap[Arena::instance()->getCoreData().divisionId].icon2;
            ClientLogic::instance()->pass2Engine(&infoOut);
            MonsterWave::instance()->clearWaitTeamAndMonster();
//            L2E_UPDATE_ONE_VALUE infoOut;
//            infoOut.eProtocol = l2e_victory;
//            if (abyssId != 0 || towerId != 0 || arenaId != 0) {
//                infoOut.value = 0;
//            }else{
//                infoOut.value = 1;
//            }
//            //            infoOut.value = ((abyssId == 0)?1:0);
//
//            ClientLogic::instance()->pass2Engine(&infoOut);
            Campaign::instance()->checkTip();
            L2E_UPDATE_HALL_TIP infoTip;
            infoTip.eProtocol = l2e_update_hall_tip;
            infoTip.tipIndex = 7;
            infoTip.tipValue = Campaign::instance()->getTip();
            ClientLogic::instance()->pass2Engine(&infoTip);
            
            if (result == 2) {
                SystemLogic::instance()->updateArenaRank();
            }
        }else{
            stageResult = 2;
            int originDiv = Arena::instance()->getCoreData().divisionId;
            account->changeGold(Arena::instance()->divistionConfigMap[originDiv].loseGold);
            
            C2S_UMENG_TRACK infoUmeng;
            infoUmeng.eProtocol = c2s_umeng_track;
            infoUmeng.eventName = "get_gold";
            infoUmeng.count = Arena::instance()->divistionConfigMap[originDiv].loseGold;
            infoUmeng.attrMap.clear();
            ClientLogic::instance()->pass2Service(&infoUmeng);
            
            int result = Arena::instance()->lose();
            L2E_ARENA_RESULT infoOut;
            infoOut.eProtocol = l2e_arena_fail;
            infoOut.deltaScore = 10;
            infoOut.totalScore = Arena::instance()->getCoreData().score;
            infoOut.changeDivision = ((originDiv==Arena::instance()->getCoreData().divisionId)?0:1);
            infoOut.gold = Arena::instance()->divistionConfigMap[originDiv].loseGold;
            infoOut.divisionIcon1 = Arena::instance()->divistionConfigMap[Arena::instance()->getCoreData().divisionId].icon1;
            infoOut.divisionIcon2 = Arena::instance()->divistionConfigMap[Arena::instance()->getCoreData().divisionId].icon2;
            ClientLogic::instance()->pass2Engine(&infoOut);
            
            if (result == 2) {
                SystemLogic::instance()->updateArenaRank();
            }
        }
    } else {
        stageResult = 2;
        L2E_COMMON infoOut;
        infoOut.eProtocol = l2e_fail;
        
        C2S_UMENG_STAGE_LEVEL umengStageInfo;
        umengStageInfo.eProtocol = c2s_umeng_fail_stage_level;
        umengStageInfo.stageName = Convert2String(battleField->getConfig().stageId);
        ClientLogic::instance()->pass2Service(&umengStageInfo);
        
        ClientLogic::instance()->pass2Engine(&infoOut);
    }
}

void GameCore::fightStar(E2L_FIGHT_STAR info)
{
    int starThres[3];
    
//    starThres[0] = battleField->battleMap[stageId].starSecond;
//    starThres[1] = battleField->battleMap[stageId].starHpPer;
//    starThres[2] = battleField->battleMap[stageId].starUseBottle;
    starThres[0] = Chapter::instance()->stageConfigMap[stageId].starSecond;
    starThres[1] = Chapter::instance()->stageConfigMap[stageId].starHpPer;
    starThres[2] = Chapter::instance()->stageConfigMap[stageId].starUseBottle;
    
    int starInfo[3];
    starInfo[0] = info.leftSecond;
    starInfo[1] = info.heroHp*100.0/info.heroMaxHp;
    starInfo[2] = useBottleCount-1;
    
    // 哲信提出进战斗就扣体力，故删掉这里
//    VIT::instance()->useVIT(Chapter::instance()->stageConfigMap[stageId].costVIT);
    int gold = Chapter::instance()->stageConfigMap[stageId].boundsGold;
    int exp = Chapter::instance()->stageConfigMap[stageId].boundsExp;
    bool firstPass = (Chapter::instance()->stageSaveMap[stageId].star[0] == 0);
    if (firstPass) {
        gold += Chapter::instance()->stageConfigMap[stageId].firstGold;
        exp += Chapter::instance()->stageConfigMap[stageId].firstExp;
    }
    
    account->changeGold(gold);
    
    C2S_UMENG_TRACK infoUmeng;
    infoUmeng.eProtocol = c2s_umeng_track;
    infoUmeng.eventName = "get_gold";
    infoUmeng.count = gold;
    infoUmeng.attrMap.clear();
    ClientLogic::instance()->pass2Service(&infoUmeng);
    
    int originLevel = account->getData().level;
    account->addExp(exp);
    //todo 评星
    L2E_FIGHT_STAR infoOut;
    infoOut.eProtocol = l2e_fight_star;
    infoOut.boundGold = Chapter::instance()->stageConfigMap[stageId].boundsGold;
    infoOut.boundExp = Chapter::instance()->stageConfigMap[stageId].boundsExp;
    infoOut.firstGold = 0;
    infoOut.firstExp = 0;
    if (abyssId != 0 || towerId != 0 || arenaId != 0) {
        infoOut.normal = 0;
    }else{
        infoOut.normal = 1;
    }
    //            infoOut.value = ((abyssId == 0)?1:0);

    infoOut.boundIcon = "";
    infoOut.boundIconCount = 0;
    memset(infoOut.star, 0, sizeof(int)*3);
    memset(infoOut.starType, 0, sizeof(int)*3);
    memset(infoOut.thresVal, 0, sizeof(int)*3);
    if (arenaId == 1) {
        int result = Arena::instance()->win();
        Campaign::instance()->activeMission(6, 1);
        Campaign::instance()->activeBoundMission(8, 0, Arena::instance()->getCoreData().score);
        Campaign::instance()->checkTip();
        L2E_UPDATE_HALL_TIP infoTip;
        infoTip.eProtocol = l2e_update_hall_tip;
        infoTip.tipIndex = 7;
        infoTip.tipValue = Campaign::instance()->getTip();
        ClientLogic::instance()->pass2Engine(&infoTip);
        
        if (result == 2) {
            SystemLogic::instance()->updateArenaRank();
        }
    }else if(abyssId != 0){
        int petId = Abyss::instance()->abyssConfigMap[abyssId].petFragId;
        infoOut.boundIcon = Pet::instance()->petConfigMap[petId].icon;
        infoOut.boundIconCount = Abyss::instance()->abyssConfigMap[abyssId].petFragCount;
        Pet::instance()->addFragment(petId, infoOut.boundIconCount);
        Abyss::instance()->win(abyssId);
        Campaign::instance()->activeMission(3, 1);
        Campaign::instance()->activeBoundMission(9, 0, abyssId);
        Campaign::instance()->checkTip();
        L2E_UPDATE_HALL_TIP infoTip;
        infoTip.eProtocol = l2e_update_hall_tip;
        infoTip.tipIndex = 7;
        infoTip.tipValue = Campaign::instance()->getTip();
        ClientLogic::instance()->pass2Engine(&infoTip);
    }else if (towerId != 0) {
        int gemId = Tower::instance()->towerConfigMap[towerId].gemId;
        int gemCount = Tower::instance()->towerConfigMap[towerId].gemCount;
        infoOut.boundIcon = Arms::instance()->gemsConfigMap[gemId].icon;
        infoOut.boundIconCount = gemCount;
        Arms::instance()->addGem(gemId, gemCount);
        Tower::instance()->win(towerId);
        Campaign::instance()->activeMission(4, towerId);
        
        Arms::instance()->checkTip(account->getData().fame);
        L2E_UPDATE_HALL_TIP infoTip;
        infoTip.eProtocol = l2e_update_hall_tip;
        infoTip.tipIndex = 4;
        infoTip.tipValue = Arms::instance()->getTip();
        ClientLogic::instance()->pass2Engine(&infoTip);
        Campaign::instance()->checkTip();
        L2E_UPDATE_HALL_TIP infoTip1;
        infoTip1.eProtocol = l2e_update_hall_tip;
        infoTip1.tipIndex = 7;
        infoTip1.tipValue = Campaign::instance()->getTip();
        ClientLogic::instance()->pass2Engine(&infoTip1);
    } else {
        Campaign::instance()->activeMission(2, 1);
        Campaign::instance()->activeBoundMission(7, 0, stageId);
        Campaign::instance()->checkTip();
        L2E_UPDATE_HALL_TIP infoTip;
        infoTip.eProtocol = l2e_update_hall_tip;
        infoTip.tipIndex = 7;
        infoTip.tipValue = Campaign::instance()->getTip();
        ClientLogic::instance()->pass2Engine(&infoTip);
        
        infoOut.star[0] = 1;
        infoOut.starType[0] = 0;
        infoOut.thresVal[0] = 0;
        int starCount = 1;
        for (int i= 0; i < 3; i++) {
            if (starThres[i] == 0) {
                continue;
            }
            if (i == 2) {
                if (starInfo[i]<starThres[i]) {
                    infoOut.star[starCount] = 1;
                }else{
                    infoOut.star[starCount] = 0;
                }
                infoOut.thresVal[starCount] = 0;
            }else{
                if (starInfo[i]>starThres[i]) {
                    infoOut.star[starCount] = 1;
                }else{
                    infoOut.star[starCount] = 0;
                }
                infoOut.thresVal[starCount] = starThres[i];
            }
            infoOut.starType[starCount] = i+1;
            starCount++;
            if (starCount == 3) {
                break;
            }
        }
        if (firstPass) {
            infoOut.firstGold = Chapter::instance()->stageConfigMap[stageId].firstGold;
            infoOut.firstExp = Chapter::instance()->stageConfigMap[stageId].firstExp;
        }
        int charpterId = Chapter::instance()->getCurrChapter();
        Chapter::instance()->boundsStar(charpterId, stageId, infoOut.star);
        int charpterStarCount = Chapter::instance()->getChapterStarCount(charpterId);
        if (Chapter::instance()->getCurrChapter() > 2) {
            for (int i = 0; i < 3; i++) {
                if (charpterStarCount < enableStarBoxCount[i]) {
                    break;
                }
                
                Chapter::instance()->enableStarBox(charpterId, i+1);
            }
        }
        for (auto unlock : account->unlockConfigMap) {
            if (unlock.second.stageId == stageId)
            {
                if (account->unlockMap[unlock.first].lock != 1) {
                    break;
                }
                
                account->openLock(unlock.first);
                break;
            }
        }
        Chapter::instance()->updateStageInfo(Chapter::instance()->getCurrChapter(), stageId);
        Chapter::instance()->saveStageJson(Chapter::instance()->getCurrChapter());
        Chapter::instance()->save2Json();
    }
    ClientLogic::instance()->pass2Engine(&infoOut);
    L2E_UPDATE_EXP infoExp;
    infoExp.eProtocol = l2e_update_exp;
    infoExp.currExp = account->getData().exp;
    infoExp.level = account->getData().level;
    infoExp.upLevel = (infoExp.level>originLevel);
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
}
void GameCore::witnessArrive()
{
    if (stageResult != 0) {
        return;
    }
    if (battleField->getConfig().condition == 5) {
        stageResult = 1;
        
        L2E_UPDATE_ONE_VALUE infoOut;
        infoOut.eProtocol = l2e_victory;
        infoOut.value = ((abyssId == 0)?1:0);
        ClientLogic::instance()->pass2Engine(&infoOut);
        
        C2S_UMENG_STAGE_LEVEL umengStageInfo;
        umengStageInfo.eProtocol = c2s_umeng_finish_stage_level;
        umengStageInfo.stageName = Convert2String(battleField->getConfig().stageId);
        ClientLogic::instance()->pass2Service(&umengStageInfo);
        
        MonsterWave::instance()->clearWaitTeamAndMonster();
    }
}
void GameCore::startFight()
{
    changeGameState(FIGHT_GAME_NORMAL);
}

void GameCore::useHpBottle()
{
    L2E_UPDATE_HP_BOTTLE_PRICE infoOut;
    infoOut.eProtocol = l2e_update_hp_bottle_price;
    infoOut.errNo = 0;
    if (account->getData().diamond < bottlePrice[useBottleCount]) {
        infoOut.errNo = 1;
        ClientLogic::instance()->pass2Engine(&infoOut);
        return;
    }
    account->changeDiamond(-bottlePrice[useBottleCount]);
    useBottleCount++;
    useBottleCount = MIN(3, useBottleCount);
    infoOut.price = bottlePrice[useBottleCount-1];
    ClientLogic::instance()->pass2Engine(&infoOut);
    
    L2E_COMMON info;
    info.eProtocol = l2e_full_hero_hp;
    ClientLogic::instance()->pass2Engine(&info);
    
}

void GameCore::addCombo(E2L_ADD_COMBO info)
{
    int count = info.count;
    int level = 7;
    for (int i = 0; i < 7; i++) {
        if (count < comboThres[i]) {
            level = i;
            break;
        }
    }
    
    L2E_ADD_COMBO infoOut;
    infoOut.eProtocol = l2e_update_combo;
    infoOut.count = count;
    if (level == 7) {
        infoOut.max = 100;
        infoOut.curr = 100;
        infoOut.factor = comboFactor[6];
    }else{
        if (level > 0) {
            infoOut.curr = count-comboThres[level-1];
            infoOut.factor = comboFactor[level-1];
            infoOut.max = comboThres[level]-comboThres[level-1];
        }else{
            infoOut.curr = count;
            infoOut.factor = 0;
            infoOut.max = comboThres[level];
        }        
    }
    ClientLogic::instance()->pass2Engine(&infoOut);
}

void GameCore::clearCombo()
{
    L2E_COMMON info;
    info.eProtocol = l2e_clear_combo;
    ClientLogic::instance()->pass2Engine(&info);
}

void GameCore::pauseFight()
{
    changeGameState(FIGHT_GAME_PAUSE);
    L2E_COMMON info;
    info.eProtocol = l2e_pause_fight;
    ClientLogic::instance()->pass2Engine(&info);
}

void GameCore::resumeFight()
{
    backGameState();
    L2E_COMMON info;
    info.eProtocol = l2e_resume_fight;
    ClientLogic::instance()->pass2Engine(&info);
}
