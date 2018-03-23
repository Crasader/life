//
//  MonsterWave.cpp
//  cocosGame
//
//  Created by ff on 14-11-30.
//
//

#include "MonsterWave.h"
#include "ELProtocol.h"
#include "logic/ClientLogic.h"
#include "cocostudio/CocoStudio.h"
#include "utils/GameReader.h"
#include "GameDefine.h"
//#include "../Dialogue_System/Dialogue_Common.h"
//#include "../Dialogue_System/DialogueManager.h"

USING_NS_CC;
using namespace cocostudio;

std::map<int, int> MonsterWave::idTable;

MonsterWave::MonsterWave():
waveId(0),
surviveCount(0),
nextWaveId(0),
startTeamId(0),
teamCount(0),
tiledMap(nullptr),
sendTeamOver(true),
plotStop(false)
{
//	m_pDialogueIDList.clear();
//    waitSendTeams.clear();
    std::queue<int>().swap(waitTeamId);
    delaySendTeams.clear();
}

MonsterWave::~MonsterWave()
{
    waveId = 0;
    surviveCount = 0;
    nextWaveId = 0;
    startTeamId = 0;
    teamCount = 0;
    tiledMap = nullptr;
    teams.clear();
    std::queue<int>().swap(waitTeamId);
    delaySendTeams.clear();
//	m_pDialogueIDList.clear();
}

MonsterTeam *MonsterWave::getTeam(int teamId)
{
    if(teams.find(teamId) == teams.end())
    {
        return nullptr;
    }
    
    return teams[teamId];
}
void MonsterWave::clearWaitTeamAndMonster()
{
    waitSendMons.clear();
    std::queue<int>().swap(waitTeamId);
    delaySendTeams.clear();
}
void MonsterWave::update(float dt)
{
    if (plotStop) {
        return;
    }
    if (!waitSendMons.empty()) {
        sendWaitMonster();
    }
    
    for (auto itr = delaySendTeams.begin(); itr != delaySendTeams.end(); itr++) {
        (*itr).second -= dt;
        (*itr).second = MAX(0, (*itr).second);
        if ((*itr).second == 0) {
//            waitSendTeams.push_back(delayTeam.first);
            waitTeamId.push((*itr).first);
        }
    }
    
    for (auto itr = delaySendTeams.begin(); itr != delaySendTeams.end();) {
        if ((*itr).second <= 0) {
            itr = delaySendTeams.erase(itr);
        }else{
            itr++;
        }
    }
    if (!waitTeamId.empty()) {
        sendTeam(waitTeamId.front());
        waitTeamId.pop();
    }
    
}
bool MonsterWave::sendTeam(int teamId)
{
    if (!sendTeamOver) {
        return false;
    }
    
    auto monTeam = new MonsterTeam();
    if(!monTeam->readFile(teamId, MONSTER_TEAM_CONFIG_FILE))
    {
        return false;
    }
    teams[monTeam->getTeamId()] = monTeam;

//	int nDialogueID = monTeam->getBeginDialogueID();
//	if (DialogueManager::instance()->JudgeTargetDialogueIDIslegal(nDialogueID))
//	{
//		m_pDialogueIDList.push_back(monTeam->getBeginDialogueID());
//	}
	
    monTeam->sendMonster();
    auto waitSend = monTeam->getMons();
    for (auto waitMon : waitSend)
    {
        waitSendMons.push_back(waitMon);
    }
    

    if (monTeam->getNextTeamId() != 0) {
        if (monTeam->getTeamInterval() == 0) {
//            sendTeam(monTeam->getNextTeamId());
//            waitSendTeams.push_back(monTeam->getNextTeamId());
            waitTeamId.push(monTeam->getNextTeamId());
        }else{
            delaySendTeams[monTeam->getNextTeamId()] = monTeam->getTeamInterval();
//            EventCustom event("delayNextTeam");
//            event.setUserData(monTeam);
//            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
//            sendTeamOver = false;
        }
    }
    return true;
}
void MonsterWave::sendWaitOver()
{
    sendTeamOver = true;
    waitSendMons.clear();
}

bool MonsterWave::isAllTeamOut()
{
    return false;
}

bool MonsterWave::isWaveOver()
{
    return true;
}

void MonsterWave::sendWaitMonster()
{
    sendTeamOver = false;
    auto mons = getWaitSendMons();
    L2E_SETUP_MONSTER_TEAM info;
    info.eProtocol = l2e_setup_monster_team;
    info.team.clear();
    for (auto mon : mons) {
        TeamMate mate;
        mate.monId = mon.monId;
        mate.teamId = mon.teamId;
        mate.fromTmx = mon.fromTmx;
        mate.posType = mon.posType;
        mate.x = mon.x;
        memcpy(mate.buffId, mon.buffId, sizeof(int)*5);
        mate.opponentFlag = false;
        if (battleType == 6 && bossTeamId == mon.teamId) {
            mate.opponentFlag = true;
        }
        info.team.push_back(mate);
    }
    ClientLogic::instance()->pass2Engine(&info);
    sendWaitOver();
}

bool MonsterWave::someOneDead(int teamIdx)
{
    if (teams.find(teamIdx) == teams.end()) {
        return false;
    }
    auto team = teams[teamIdx];
    if(team->someOneDead())
    {
        teamCount--;
		if (teamCount == 0)
		{
//			int nDialogueID = team->getEndDialogueID();
//			if (DialogueManager::instance()->JudgeTargetDialogueIDIslegal(nDialogueID))
//			{
//				m_pDialogueIDList.push_back(team->getEndDialogueID());
//			}
			return true;
		}
        if (team->getDieTeamId() == 0) 
		{
            return false;
        }
        
//        EventCustom event("arrangeDieTeam");
//        event.setUserData(team);
//        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
//        waitSendTeams.push_back(team->getDieTeamId());
        waitTeamId.push(team->getDieTeamId());
//        sendTeam(team->getDieTeamId());
//        auto mons = getWaitSendMons();
//        L2E_SETUP_MONSTER_TEAM info;
//        info.eProtocol = l2e_setup_monster_team;
//        info.team.clear();
//        for (auto mon : mons) {
//            TeamMate mate;
//            mate.monId = mon.monId;
//            mate.teamId = mon.teamId;
//            mate.fromTmx = mon.fromTmx;
//            mate.posType = mon.posType;
//            mate.x = mon.x;
//            memcpy(mate.buffId, mon.buffId, sizeof(int)*5);
//            info.team.push_back(mate);
//        }
//        ClientLogic::instance()->pass2Engine(&info);
//        sendWaitOver();
    }
    
    return false;
}

bool MonsterWave::readWave(int idx, rapidjson::Document& _doc)
{
    int index = idTable.find(idx)->second;
    
    const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, index);
    if (dic.IsNull()) {
        return false;
    }
    
    waveId = DICTOOL->getIntValue_json(dic, "id");
    nextWaveId = DICTOOL->getIntValue_json(dic, "nextWaveId");
    startTeamId = DICTOOL->getIntValue_json(dic, "startTeamId");
    teamCount = DICTOOL->getIntValue_json(dic, "teamCount");
    
    return true;
}

void MonsterWave::initIdTable(rapidjson::Document &_doc)
{
    if (_doc.IsArray()){
        int size = _doc.Size();
        int objId;
        for (int i = 0; i<size; i++) {
            objId =DICTOOL->getIntValue_json(_doc[i], "id");
            idTable.insert(std::make_pair(objId, i));
        }
    }
}

bool MonsterWave::readFile(int idx, std::string filename)
{
    auto doc = GameReader::getDocInstance(filename);
    if(MonsterWave::idTable.empty())
    {
        MonsterWave::initIdTable(*doc);
    }
    return readWave(idx, *doc);
}

bool MonsterWave::startWave()
{
    teams.clear();
    std::queue<int>().swap(waitTeamId);
    waitTeamId.push(startTeamId);
    return true;
}

void MonsterWave::setPlotStop(int val)
{
    plotStop = (val==1?false:true);
}
