//
//  Arena.cpp
//  life
//
//  Created by ff on 17/9/18.
//
//

#include "Arena.h"
#include "utils/GameReader.h"
#include "GameDefine.h"
#include "utils/libs.h"
#include "utils/RandomName.h"

USING_NS_CC;
using namespace cocostudio;
using namespace rapidjson;

std::map<int, int> Arena::robotConfigIdTable;
std::map<int, int> Arena::divisionConfigIdTable;

Arena::Arena():
coreStr(""),
robotStr("")
{
    robotConfigIdTable.clear();
    robotConfigMap.clear();
    robotMap.clear();
    divisionConfigIdTable.clear();
    divistionConfigMap.clear();
}

Arena::~Arena()
{
    
}

void Arena::readRobotConfig()
{
    auto doc = GameReader::getDocInstance(ARENA_ROBOT_CONFIG_FILE);
    if (robotConfigIdTable.empty()) {
        GameReader::initIdTable(*doc, "id", robotConfigIdTable);
    }
    
    robotConfigMap.clear();
    for (int i = 0; i < robotConfigIdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        
        ARENA_ROBOT_CONFIG config;
        config.robotId = DICTOOL->getIntValue_json(dic, "id");
        config.level = DICTOOL->getIntValue_json(dic, "level");
        config.holySwordLevel = DICTOOL->getIntValue_json(dic, "holy_sword_level");
        for (int i = 0; i < 5; i++) {
            config.petCount[i] = DICTOOL->getIntValue_json(dic, GameUtils::format("pet%d_count", i+1).c_str());
            for (int j = 0; j < 2; j++) {
                config.petLevel[i][j] = DICTOOL->getIntValue_json(dic, GameUtils::format("pet%d_star%d", i+1, j+1).c_str());
            }
        }
        
        config.otherHp = DICTOOL->getIntValue_json(dic, "other_hp");
        config.otherAttack = DICTOOL->getIntValue_json(dic, "other_attack");
        config.otherDefence = DICTOOL->getIntValue_json(dic, "other_defence");
        config.otherCrit = DICTOOL->getIntValue_json(dic, "other_crit");
        config.otherDecrit = DICTOOL->getIntValue_json(dic, "other_decrit");
        config.score = DICTOOL->getIntValue_json(dic, "score");
        robotConfigMap[config.robotId] = config;
    }
    return;
}

void Arena::readDivisionConfig()
{
    auto doc = GameReader::getDocInstance(ARENA_DIVISION_CONFIG_FILE);
    if (divisionConfigIdTable.empty()) {
        GameReader::initIdTable(*doc, "id", divisionConfigIdTable);
    }
    
    divistionConfigMap.clear();
    int totalRobot = 0;
    for (int i = 0; i < divisionConfigIdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        
        ARENA_DIVISION_CONFIG config;
        config.divisionId = DICTOOL->getIntValue_json(dic, "id");
        config.icon1 = DICTOOL->getStringValue_json(dic, "icon");
        config.icon2 = DICTOOL->getStringValue_json(dic, "icon2");
        
        config.minScore = DICTOOL->getIntValue_json(dic, "min_score");
        config.maxScore = DICTOOL->getIntValue_json(dic, "max_score");
        config.adviceHp = DICTOOL->getIntValue_json(dic, "advice_hp");
        config.adviceAttack = DICTOOL->getIntValue_json(dic, "advice_attack");
        config.adviceDefence = DICTOOL->getIntValue_json(dic, "advice_defence");
        config.adviceCrit = DICTOOL->getIntValue_json(dic, "advice_crit");
        config.adviceDecrit = DICTOOL->getIntValue_json(dic, "advice_decrit");
        config.rewardFame = DICTOOL->getIntValue_json(dic, "reward_fame");
        config.rewardGold = DICTOOL->getIntValue_json(dic, "reward_gold");
        config.winGold = DICTOOL->getIntValue_json(dic, "win_gold");
        config.loseGold = DICTOOL->getIntValue_json(dic, "lose_gold");
        config.robotCount = DICTOOL->getIntValue_json(dic, "robot_count");
        config.maxSwordId = DICTOOL->getIntValue_json(dic, "max_holy_sword");
        for (int i = 0; i < 5; i++) {
            config.petCount[i] = DICTOOL->getIntValue_json(dic, GameUtils::format("pet%d_count", i+1).c_str());
        }
        totalRobot += config.robotCount;
        divistionConfigMap[config.divisionId] = config;
    }
    
    int sum = 0;
    for (auto config : divistionConfigMap) {
        divistionConfigMap[config.first].minPlace = totalRobot-sum;
        sum += divistionConfigMap[config.first].robotCount;
    }
    return;
}

void Arena::deleteJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "arena_info.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return;
    }
    
    FileUtils::getInstance()->removeFile(path);
}

void Arena::save2Json()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "arena_info.json";
    
    FILE* pFile = fopen(path.c_str(),"wb");
    if (pFile)
    {
        fputs(coreStr.c_str(),pFile);
        fclose(pFile);
    }else
    {
        CCLOG("UserManage::SaveUserToFile error! can not open %s",path.c_str());
    }
}

bool Arena::readJson()
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += "arena_info.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return false;
    }
    
    coreStr = FileUtils::getInstance()->getStringFromFile(path);
    if (!parseJson()) {
        return false;
    }
    
    return true;
}

bool Arena::parseJson()
{
    if (coreStr == "") {
        return false;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(coreStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return false;
    }
    
    if(!_doc.IsObject())
    {
        return false;
    }
    
    
    coreData.score = DICTOOL->getIntValue_json(_doc, "score");
    coreData.place = DICTOOL->getIntValue_json(_doc, "place");
    coreData.divisionId = DICTOOL->getIntValue_json(_doc, "division");
    coreData.ticketCount = DICTOOL->getIntValue_json(_doc, "ticket_count");
    coreData.maxTicketCount = DICTOOL->getIntValue_json(_doc, "max_ticket_count");
    coreData.rewardState = DICTOOL->getIntValue_json(_doc, "reward_state");
    
//    coreData.place = 4;
//    _doc["place"] = coreData.place;
//    
//    StringBuffer buff;
//    rapidjson::Writer<StringBuffer> writer(buff);
//    _doc.Accept(writer);
//    std::string s = StringUtils::format("%s", buff.GetString());
//    //    log("========================\n");
//    //    log("%s\n", s.c_str());
//    coreStr = encode(s);
    return true;
}

void Arena::save2EmptyCoreStr()
{
    rapidjson::Document _doc;
    if (coreStr != "") {
        return;
    }else{
        _doc.SetObject();
    }
    
    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    _doc.AddMember("score", 0, allocator);
    _doc.AddMember("place", 2000, allocator);
    _doc.AddMember("division", 1, allocator);
    _doc.AddMember("ticket_count", 6, allocator);
    _doc.AddMember("max_ticket_count", 6, allocator);
    _doc.AddMember("reward_state", 1, allocator);
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
}

void Arena::deleteRobotJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "arena_robot_info.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return;
    }
    
    FileUtils::getInstance()->removeFile(path);
}

void Arena::save2RobotJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "arena_robot_info.json";
    
    FILE* pFile = fopen(path.c_str(),"wb");
    if (pFile)
    {
        fputs(robotStr.c_str(),pFile);
        fclose(pFile);
    }else
    {
        CCLOG("UserManage::SaveUserToFile error! can not open %s",path.c_str());
    }
}

bool Arena::readRobotJson()
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += "arena_robot_info.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return false;
    }
    
    robotStr = FileUtils::getInstance()->getStringFromFile(path);
    if (!parseRobotJson()) {
        return false;
    }
    
    return true;
}

bool Arena::parseRobotJson()
{
    if (robotStr == "") {
        return false;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(robotStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return false;
    }
    
    if(!_doc.IsArray())
    {
        return false;
    }
    
    robotMap.clear();
    for (int i = 0; i < _doc.Size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, i);
        ROBOT_DATA data;
        data.place = DICTOOL->getIntValue_json(dic, "place");
        data.score = DICTOOL->getIntValue_json(dic, "score");
        data.level = DICTOOL->getIntValue_json(dic, "level");
        data.name = DICTOOL->getStringValue_json(dic, "name");
        data.jobId = DICTOOL->getIntValue_json(dic, "job");
        data.holySwordId = DICTOOL->getIntValue_json(dic, "holy_sword_id");
        for (int i = 0; i < 3; i++) {
            data.petId[i] = DICTOOL->getIntValue_json(dic, GameUtils::format("pet%d_id", i+1).c_str());
            data.petLevel[i] = DICTOOL->getIntValue_json(dic, GameUtils::format("pet%d_level", i+1).c_str());
        }
        data.hp = DICTOOL->getIntValue_json(dic, "hp");
        data.attack = DICTOOL->getIntValue_json(dic, "attack");
        data.defence = DICTOOL->getIntValue_json(dic, "defence");
        data.crit = DICTOOL->getIntValue_json(dic, "crit");
        data.decrit = DICTOOL->getIntValue_json(dic, "decrit");
        data.robot = DICTOOL->getIntValue_json(dic, "robot");
        robotMap.push_back(data);
    }
    return true;
}

void Arena::save2EmptyRobotStr()
{
    rapidjson::Document _doc;
    if (robotStr != "") {
        return;
    }else{
        _doc.SetArray();
    }
    
    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    for (int i = 0; i < 30; i++) {
        rapidjson::Value object(rapidjson::kObjectType);
        object.AddMember("place", robotMap[i].place, allocator);
        object.AddMember("score", robotMap[i].score, allocator);
        object.AddMember("level", robotMap[i].level, allocator);
        object.AddMember("name", rapidjson::StringRef(robotMap[i].name.c_str()), allocator);
        object.AddMember("job", robotMap[i].jobId, allocator);
        object.AddMember("holy_sword_id", robotMap[i].holySwordId, allocator);
        object.AddMember("pet1_id", robotMap[i].petId[0], allocator);
        object.AddMember("pet2_id", robotMap[i].petId[1], allocator);
        object.AddMember("pet3_id", robotMap[i].petId[2], allocator);
        object.AddMember("pet1_level", robotMap[i].petLevel[0], allocator);
        object.AddMember("pet2_level", robotMap[i].petLevel[1], allocator);
        object.AddMember("pet3_level", robotMap[i].petLevel[2], allocator);
        object.AddMember("hp", robotMap[i].hp, allocator);
        object.AddMember("attack", robotMap[i].attack, allocator);
        object.AddMember("defence", robotMap[i].defence, allocator);
        object.AddMember("crit", robotMap[i].crit, allocator);
        object.AddMember("decrit", robotMap[i].decrit, allocator);
        object.AddMember("robot", 1, allocator);
        
        _doc.PushBack(object, allocator);
    }
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    robotStr = encode(s);
}

void Arena::addRobot(int place, int swordCount, std::map<int, int> totalPets, std::string name, int job, int hp, int attack, int defence, int crit, int decrit)
{
    ROBOT_DATA robot;
    robot.place = place;
    robot.score = robotConfigMap[place].score;
    robot.level = robotConfigMap[place].level;
    robot.jobId = job;
    robot.name = name;
    robot.hp = hp;
    robot.attack = attack;
    robot.defence = defence;
    robot.crit = crit;
    robot.decrit = decrit;
    robot.holySwordId = abs(rand())%swordCount+1;
    robot.robot = 1;

    memset(robot.petId, 0, sizeof(int)*3);
    memset(robot.petLevel, 0, sizeof(int)*3);
    
    std::vector<int> petIndex;
    GameUtils::getRandArray(petIndex, (int)totalPets.size());
    for (int i = 0; i < 3; i++) {
        int index = 1;
        for (auto pet : totalPets) {
            if (index == petIndex[petIndex.size()-i-1]) {
                robot.petId[i] = pet.first;
                robot.petLevel[i] = pet.second;
            }
            index++;
        }

    }

    robotMap.push_back(robot);
}

void Arena::refreshTop30()
{
    addRobot2Top30();
    std::vector<int> robotIndex;
    GameUtils::getRandArray(robotIndex, 30);
    std::vector<int> robotPlace;
    robotPlace.clear();
    int activeCount = abs(rand())%10;
    for (int i = 0; i < activeCount; i++) {
        int refreshIndex = robotIndex[robotIndex.size()-i-1]-1;
        int deltaScore = (abs(rand())%20-10)*10;
        robotMap[refreshIndex].score += deltaScore;
    }
    sortTop30();
    
    for (int i = 0; i < 10; i++) {
        robotMap.pop_back();
    }
    log("end refresh");
}

void Arena::addRobot2Top30()
{
    std::vector<int> robotIndex;
    GameUtils::getRandArray(robotIndex, 10);
    for (int i = 0; i < 10; i++) {
        ROBOT_DATA man;
        man.place = 40-i;
        man.level = robotMap[29-i].level+(abs(rand())%6-3);
        man.level = MAX(1, man.level);
        man.level = MIN(30, man.level);
        man.score = robotMap[29-i].score+(abs(rand())%20-10)*10;
        man.jobId = robotMap[29-i].jobId;
        man.name = RandomName::instance()->getRandomName();
        man.holySwordId = robotMap[29-i].holySwordId;
        man.robot = 1;
        memcpy(man.petId, robotMap[29-i].petId, sizeof(int)*3);
        memcpy(man.petLevel, robotMap[29-i].petLevel, sizeof(int)*3);
        for (int i = 0; i < 3; i++) {
            man.petLevel[i] += (abs(rand())%4-2);
            man.petLevel[i] = MIN(5, man.petLevel[i]);
            man.petLevel[i] = MAX(1, man.petLevel[i]);
        }
        man.hp = robotMap[29-i].hp;
        man.attack = robotMap[29-i].attack;
        man.defence = robotMap[29-i].defence;
        man.crit = robotMap[29-i].crit;
        man.decrit = robotMap[29-i].decrit;
        int randType = abs(rand())%5;
        switch (randType) {
            case 0:
            {
                man.hp += robotMap[29-i].hp*(abs(rand())%20-10)/1000;
            }
                break;
            case 1:
            {
                man.attack += robotMap[29-i].attack*(abs(rand())%20-10)/1000;
            }
                break;
            case 2:
            {
                man.defence += robotMap[29-i].defence*(abs(rand())%20-10)/1000;
            }
                break;
            case 3:
            {
                man.crit += robotMap[29-i].crit*(abs(rand())%20-10)/1000;
            }
                break;
            case 4:
            {
                man.decrit += robotMap[29-i].decrit*(abs(rand())%20-10)/1000;
            }
                break;
                
            default:
                break;
        }
        robotMap.push_back(man);
    }
}

void Arena::sortTop30()
{
    std::stable_sort(robotMap.begin(), robotMap.end(), CC_CALLBACK_2(Arena::compRobotScore, this));
    
    int index = 0;
    for (auto robot : robotMap) {
        robotMap[index].place = index+1;
        index++;
    }
}

bool Arena::compRobotScore(ROBOT_DATA first, ROBOT_DATA second)
{
    return first.score >= second.score;
}

void Arena::matchRobot(ROBOT_DATA robot)
{
    oppRobot = robot;
}

void Arena::matchRobotInTop30(ROBOT_DATA &robot)
{
    int top = MIN(coreData.place-1, 5);
    int distance = abs(rand())%16-top;
//    if (distance == 0) {
//        distance = -1;
//    }
    
    int robotPlace = coreData.place+distance-1;
    if (robotPlace == coreData.place-1) {
        robotPlace = coreData.place + 1;
    }
    robot.place = robotMap[robotPlace].place;
    robot.score = robotMap[robotPlace].score;
    robot.name = robotMap[robotPlace].name;
    robot.holySwordId = robotMap[robotPlace].holySwordId;
    robot.level = robotMap[robotPlace].level;
    robot.hp = robotMap[robotPlace].hp;
    robot.attack = robotMap[robotPlace].attack;
    robot.defence = robotMap[robotPlace].defence;
    robot.crit = robotMap[robotPlace].crit;
    robot.decrit = robotMap[robotPlace].decrit;
    robot.robot = robotMap[robotPlace].robot;
    robot.jobId = robotMap[robotPlace].jobId;
    
    memcpy(robot.petId, robotMap[robotPlace].petId, sizeof(int)*3);
    memcpy(robot.petLevel, robotMap[robotPlace].petLevel, sizeof(int)*3);
}

void Arena::matchRobotByDivision(int division, int jobId, std::string name, int swordId, std::vector<int> totalPets, ROBOT_DATA &robot)
{
    robot.place = 2000;
    robot.score = 0;
    robot.level = 1;
    robot.jobId = jobId;
    robot.name = name;
    robot.holySwordId = swordId;
    robot.robot = 1;
    
    memset(robot.petId, 0, sizeof(int)*3);
    memset(robot.petLevel, 0, sizeof(int)*3);
    
    std::vector<int> petIndex;
    GameUtils::getRandArray(petIndex, (int)totalPets.size());
    int fightPetCount = MIN(3, (int)totalPets.size());
    for (int i = 0; i < fightPetCount; i++) {
        robot.petId[i] = totalPets[petIndex.size()-i-1];
    }
    
    robot.hp = divistionConfigMap[division].adviceHp;
    robot.attack = divistionConfigMap[division].adviceAttack;
    robot.defence = divistionConfigMap[division].adviceDefence;
    robot.crit = divistionConfigMap[division].adviceCrit;
    robot.decrit = divistionConfigMap[division].adviceDecrit;
    int randType = abs(rand())%3;
    switch (randType) {
        case 0:
        {
            robot.hp += divistionConfigMap[division].adviceHp*(abs(rand())%20-10)/1000;
        }
            break;
        case 1:
        {
            robot.attack += divistionConfigMap[division].adviceAttack*(abs(rand())%20-10)/1000;
        }
            break;
        case 2:
        {
            robot.defence += divistionConfigMap[division].adviceDefence*(abs(rand())%20-10)/1000;
        }
            break;
            
        default:
            break;
    }
}

int Arena::startFight()
{
    if (coreStr == "") {
        return 1;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(coreStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return 1;
    }
    
    if(!_doc.IsObject())
    {
        return 1;
    }

    coreData.ticketCount = DICTOOL->getIntValue_json(_doc, "ticket_count");
    if (coreData.ticketCount <= 0) {
        return 2;
    }
    
    coreData.ticketCount--;
    _doc["ticket_count"] = coreData.ticketCount;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2Json();

    return 0;
}

int Arena::win()
{
    if (coreStr == "") {
        return 1;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(coreStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return 1;
    }
    
    if(!_doc.IsObject())
    {
        return 1;
    }

    coreData.score = DICTOOL->getIntValue_json(_doc, "score");
    coreData.divisionId = DICTOOL->getIntValue_json(_doc, "division");
    coreData.place = DICTOOL->getIntValue_json(_doc, "place");
    coreData.score += 30;
    
    if(divistionConfigMap[coreData.divisionId].maxScore < coreData.score)
    {
        coreData.divisionId++;
        coreData.divisionId = MIN(30, coreData.divisionId);
    }
    int place = coreData.place;
    if (coreData.place > 30) {// 30名以外，未进入榜单
        if (coreData.score <= robotMap[29].score) {// 积分未能上榜
            int totalScore;
            int temp;
            int score = coreData.score-divistionConfigMap[coreData.divisionId].minScore;
            if (coreData.divisionId == 30) {// 如果在最高一档，减去前30
                totalScore = robotMap[29].score-10-divistionConfigMap[coreData.divisionId].minScore;
                temp = score * (divistionConfigMap[coreData.divisionId].robotCount-30) / totalScore;
                place = divistionConfigMap[coreData.divisionId].minPlace-temp+30;
            }else{
                totalScore = divistionConfigMap[coreData.divisionId].maxScore-
                                divistionConfigMap[coreData.divisionId].minScore;
                temp = score * divistionConfigMap[coreData.divisionId].robotCount / totalScore;
                place = divistionConfigMap[coreData.divisionId].minPlace-temp;
            }
        }else{// 积分可上榜
            ROBOT_DATA robot;
            robot.score = coreData.score;
            robot.level = 1;
            robot.jobId = 1;
            robot.name = "";
            robot.place = 31;
            robot.holySwordId = 1;
            memset(robot.petId, 0, sizeof(int)*3);
            memset(robot.petLevel, 0, sizeof(int)*3);
            robot.hp = 0;
            robot.attack = 0;
            robot.defence = 0;
            robot.crit = 0;
            robot.decrit = 0;
            robot.robot = 0;
            robotMap.push_back(robot);
            sortTop30();
            robotMap.pop_back();
            for (auto man : robotMap) {
                if (man.robot == 0) {
                    place = man.place;
                }
            }
            coreData.place = place;
            
            _doc["score"] = coreData.score;
            _doc["division"] = coreData.divisionId;
            _doc["place"] = coreData.place;
            
            StringBuffer buff;
            rapidjson::Writer<StringBuffer> writer(buff);
            _doc.Accept(writer);
            std::string s = StringUtils::format("%s", buff.GetString());
            //    log("========================\n");
            //    log("%s\n", s.c_str());
            coreStr = encode(s);
            save2Json();

            return 2;
        }
    }else{// 在榜单中
        robotMap[coreData.place-1].score = coreData.score;
        sortTop30();
        for (auto robot : robotMap) {
            if (robot.robot == 0) {
                place = robot.place;
                break;
            }
        }
    }
    place = MIN(2000, place);
    coreData.place = place;
    
    _doc["score"] = coreData.score;
    _doc["division"] = coreData.divisionId;
    _doc["place"] = coreData.place;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2Json();
    
    
    return 0;
}

int Arena::lose()
{
    if (coreStr == "") {
        return 1;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(coreStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return 1;
    }
    
    if(!_doc.IsObject())
    {
        return 1;
    }
    
    coreData.score = DICTOOL->getIntValue_json(_doc, "score");
    coreData.divisionId = DICTOOL->getIntValue_json(_doc, "division");
    coreData.place = DICTOOL->getIntValue_json(_doc, "place");
    coreData.score -= 10;
    coreData.score = MAX(0, coreData.score);
    
    if(divistionConfigMap[coreData.divisionId].minScore > coreData.score)
    {
        coreData.divisionId--;
    }
    coreData.divisionId = MAX(1, coreData.divisionId);
    
    int place = coreData.place;
    if (place == 30) {
        ROBOT_DATA robot;
        robotMap[29].score = robotMap[28].score-10;
        robotMap[29].level = robotMap[28].level;
        robotMap[29].jobId = robotMap[28].jobId;
        robotMap[29].name = RandomName::instance()->getRandomName();
        robotMap[29].place = 30;
        robotMap[29].holySwordId = robotMap[28].holySwordId;
        memcpy(robotMap[29].petId, robotMap[28].petId, sizeof(int)*3);
        memcpy(robotMap[29].petLevel, robotMap[28].petLevel, sizeof(int)*3);
        robotMap[29].hp = robotMap[28].hp;
        robotMap[29].attack = robotMap[28].attack;
        robotMap[29].defence = robotMap[28].defence;
        robotMap[29].crit = robotMap[28].crit;
        robotMap[29].decrit = robotMap[28].decrit;
        robotMap[29].robot = 1;
        place = 31;
    }else if (place < 30){
        robotMap[place-1].score = coreData.score;
        sortTop30();
        for (auto man : robotMap) {
            if (man.robot == 0) {
                place = man.place;
            }
        }
        place = MIN(2000, place);
        coreData.place = place;
        
        _doc["score"] = coreData.score;
        _doc["division"] = coreData.divisionId;
        _doc["place"] = coreData.place;
        
        StringBuffer buff;
        rapidjson::Writer<StringBuffer> writer(buff);
        _doc.Accept(writer);
        std::string s = StringUtils::format("%s", buff.GetString());
        //    log("========================\n");
        //    log("%s\n", s.c_str());
        coreStr = encode(s);
        save2Json();
        
        return 2;

    }else{
        int totalScore;
        int temp;
        int score = coreData.score-divistionConfigMap[coreData.divisionId].minScore;
        if (coreData.divisionId == 30) {// 如果在最高一档，减去前30
            totalScore = robotMap[29].score-10-divistionConfigMap[coreData.divisionId].minScore;
            temp = score * (divistionConfigMap[coreData.divisionId].robotCount-30) / totalScore;
            place = divistionConfigMap[coreData.divisionId].minPlace-temp+30;
        }else{
            totalScore = divistionConfigMap[coreData.divisionId].maxScore-
                            divistionConfigMap[coreData.divisionId].minScore;
            temp = score * divistionConfigMap[coreData.divisionId].robotCount / totalScore;
            place = divistionConfigMap[coreData.divisionId].minPlace-temp;
        }
//        int totalScore = divistionConfigMap[coreData.divisionId].maxScore-
//            divistionConfigMap[coreData.divisionId].minScore;
//        int score = coreData.score-divistionConfigMap[coreData.divisionId].minScore;
//        int temp = score * divistionConfigMap[coreData.divisionId].robotCount / totalScore;
//        place = divistionConfigMap[coreData.divisionId].minPlace-temp+30;
    }
    
    place = MIN(2000, place);
    coreData.place = place;        

    _doc["score"] = coreData.score;
    _doc["division"] = coreData.divisionId;
    _doc["place"] = coreData.place;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2Json();

    return 0;

}

int Arena::addTicket(int val)
{
    if (coreStr == "") {
        return 1;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(coreStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return 1;
    }
    
    if(!_doc.IsObject())
    {
        return 1;
    }
    
    coreData.ticketCount = DICTOOL->getIntValue_json(_doc, "ticket_count");
    coreData.maxTicketCount = DICTOOL->getIntValue_json(_doc, "max_ticket_count");
    coreData.ticketCount += val;
    coreData.ticketCount = MIN(coreData.maxTicketCount, coreData.ticketCount);
    
    _doc["ticket_count"] = coreData.ticketCount;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2Json();

    return 0;
}

int Arena::addMaxTicket(int val)
{
    if (coreStr == "") {
        return 1;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(coreStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return 1;
    }
    
    if(!_doc.IsObject())
    {
        return 1;
    }
    
    coreData.maxTicketCount = DICTOOL->getIntValue_json(_doc, "max_ticket_count");
    coreData.maxTicketCount+=val;
    
    _doc["max_ticket_count"] = coreData.maxTicketCount;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2Json();

    return 0;
}

int Arena::reward()
{
    if (coreStr == "") {
        return 1;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(coreStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return 1;
    }
    
    if(!_doc.IsObject())
    {
        return 1;
    }
    
    coreData.rewardState = 1;
    _doc["reward_state"] = coreData.rewardState;

    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2Json();

    return 0;
}

int Arena::takeReward()
{
    if (coreStr == "") {
        return 1;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(coreStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return 1;
    }
    
    if(!_doc.IsObject())
    {
        return 1;
    }
    
    coreData.rewardState = 0;
    _doc["reward_state"] = coreData.rewardState;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2Json();

    return 0;
}

void Arena::refreshSelfRobot(int job, std::string name, int level, int holySwordId, int *petId, int *petLevel, int hp, int attack, int defence, int crit, int decrit)
{
    if (coreStr == "") {
        return;
    }
    
    rapidjson::Document _coreDoc;
    std::string tempCoreStr = decode(coreStr.c_str());
    _coreDoc.Parse < 0 > (tempCoreStr.c_str());
    if(_coreDoc.HasParseError())
    {
        return;
    }
    
    if(!_coreDoc.IsObject())
    {
        return;
    }
    
    coreData.place = DICTOOL->getIntValue_json(_coreDoc, "place");
    if (coreData.place > 30) {
        return;
    }
    
    int placeIndex = coreData.place-1;
    robotMap[placeIndex].jobId = job;
    robotMap[placeIndex].name = name;
    robotMap[placeIndex].level = level;
    robotMap[placeIndex].holySwordId = holySwordId;
    memcpy(robotMap[placeIndex].petId, petId, sizeof(int)*3);
    memcpy(robotMap[placeIndex].petLevel, petLevel, sizeof(int)*3);
    robotMap[placeIndex].hp = hp;
    robotMap[placeIndex].attack = attack;
    robotMap[placeIndex].defence = defence;
    robotMap[placeIndex].crit = crit;
    robotMap[placeIndex].decrit = decrit;
    robotMap[placeIndex].robot = 0;
    
    if (robotStr == "") {
        return;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(robotStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    if(!_doc.IsArray())
    {
        return;
    }
    
    for (int i = 0; i < robotMap.size(); i++) {
        _doc[i]["job"] = robotMap[i].jobId;
        _doc[i]["name"] = rapidjson::StringRef(robotMap[i].name.c_str());
        _doc[i]["level"] = robotMap[i].level;
        _doc[i]["score"] = robotMap[i].score;
        _doc[i]["holy_sword_id"] = robotMap[i].holySwordId;
        for (int j = 0; j < 3; j++) {
            _doc[i][GameUtils::format("pet%d_id", j+1).c_str()] = robotMap[i].petId[j];
            _doc[i][GameUtils::format("pet%d_level", j+1).c_str()] = robotMap[i].petLevel[j];
        }
        _doc[i]["hp"] = robotMap[i].hp;
        _doc[i]["attack"] = robotMap[i].attack;
        _doc[i]["defence"] = robotMap[i].defence;
        _doc[i]["crit"] = robotMap[i].crit;
        _doc[i]["decrit"] = robotMap[i].decrit;
        _doc[i]["robot"] = robotMap[i].robot;
    }
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    log("%s",s.c_str());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    robotStr = encode(s);
    save2RobotJson();
}
