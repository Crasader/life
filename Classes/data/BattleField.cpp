//
//  BattleField.cpp
//  Dabao
//
//  Created by ff on 17/3/3.
//
//

#include "BattleField.h"
#include "utils/GameReader.h"
#include "GameDefine.h"
#include "ELProtocol.h"
#include "ClientLogic.h"

USING_NS_CC;
using namespace cocostudio;

std::map<int, int> BattleField::idTable;
std::map<int, int> BattleField::stageIdTable;

BattleField::BattleField() :
stageId(0),secondLimit(0),
waveCount(0),currSecondLeft(0),gameFail(false),
currWaveId(0),currWaveCount(0)
{
    currMonsterWave = MonsterWave::instance();
}

BattleField::~BattleField()
{
}

bool BattleField::readBattle(int idx)
{
    auto doc = GameReader::getDocInstance(BATTLE_CONFIG_FILE);
    if (stageIdTable.empty()) {
        GameReader::initIdTable(*doc, "id", stageIdTable);
    }
    int index = stageIdTable.find(idx)->second;
    const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, index);
    if (dic.IsNull()) {
        return false;
    }
    
    config.stageId = idx;
    config.tmx = DICTOOL->getStringValue_json(dic, "tiledMap");
    config.weather = DICTOOL->getStringValue_json(dic, "weather");
    config.startWaveId = DICTOOL->getIntValue_json(dic, "waveSheetID");
    config.waveCount = DICTOOL->getIntValue_json(dic, "waveCount");
    config.condition = DICTOOL->getIntValue_json(dic, "condition");
    config.bossTeamId = DICTOOL->getIntValue_json(dic, "boss_team");
    config.secondLimit = DICTOOL->getIntValue_json(dic, "time");
    config.protectorId = DICTOOL->getIntValue_json(dic, "witness_id");
    memset(config.buffId, 0, sizeof(int)*5);
    for (int i = 0; i < 5; i++) {
        std::string tempStr = GameUtils::format("buff%d", (i+1));
        config.buffId[i] = DICTOOL->getIntValue_json(dic, tempStr.c_str());
    }

    gameFail = false;
    waveOver = false;

    currSecondLeft = 0;
    currWaveId = 0;
    currWaveCount = 0;
    return true;
}

void BattleField::update(float dt)
{
    currMonsterWave->update(dt);
}

void BattleField::load1stMonsterWave()
{
    start1stWave();
//    if (!start1stWave()) {
//        return;
//    }
    
//    currMonsterWave->sendWaitMonster();
//    auto mons = currMonsterWave->getWaitSendMons();
//    L2E_SETUP_MONSTER_TEAM info;
//    info.eProtocol = l2e_setup_monster_team;
//    info.team.clear();
//    for (auto mon : mons) {
//        TeamMate mate;
//        mate.monId = mon.monId;
//        mate.teamId = mon.teamId;
//        mate.fromTmx = mon.fromTmx;
//        mate.posType = mon.posType;
//        mate.x = mon.x;
//        memcpy(mate.buffId, mon.buffId, sizeof(int)*5);
//        info.team.push_back(mate);
//    }
//    ClientLogic::instance()->pass2Engine(&info);
//    currMonsterWave->sendWaitOver();
}

bool BattleField::start1stWave()
{
    currWaveId = config.startWaveId;
    currWaveCount = 1;
    
    if (currWaveId <= 0) {
        return false;
    }
    
    if (!readCurrWave()) {
        return false;
    }
    
    currMonsterWave->startWave();
    return true;
}

bool BattleField::readCurrWave()
{
    if (currWaveId <= 0) {
        return false;
    }
    currMonsterWave = MonsterWave::instance();
    currMonsterWave->setBattleType(config.condition);
    currMonsterWave->setBossTeamId(config.bossTeamId);
    return currMonsterWave->readFile(currWaveId, MONSTER_WAVE_CONFIG_FILE);
    
}

int BattleField::getNextWaveId()
{
    if (currWaveCount == config.waveCount) {
        waveOver = true;
        return 0;
    }
    
    return currMonsterWave->getNextWaveId();
}

bool BattleField::change2Wave(int waveId)
{
    
    currWaveId = waveId;
    currWaveCount++;
    if(!readCurrWave())
    {
        return false;
    }

    currMonsterWave->startWave();
    return true;
}
