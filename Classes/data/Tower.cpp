//
//  Tower.cpp
//  life
//
//  Created by ff on 17/9/11.
//
//

#include "Tower.h"
#include "utils/GameReader.h"
#include "GameDefine.h"
#include "utils/libs.h"

USING_NS_CC;
using namespace cocostudio;
using namespace rapidjson;

std::map<int, int> Tower::towerConfigIdTable;

Tower::Tower():
coreStr("")
{
    towerConfigIdTable.clear();
    towerConfigMap.clear();
}

Tower::~Tower()
{
    
}

void Tower::readTowerConfigFile()
{
    auto doc = GameReader::getDocInstance(TOWER_CONFIG_FILE);
    if (towerConfigIdTable.empty()) {
        GameReader::initIdTable(*doc, "id", towerConfigIdTable);
    }
    
    towerConfigMap.clear();
    for (int i = 0; i < towerConfigIdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        
        TOWER_CONFIG config;
        config.towerId = DICTOOL->getIntValue_json(dic, "id");
        config.name = DICTOOL->getStringValue_json(dic, "name");
        config.descirpt = DICTOOL->getStringValue_json(dic, "descript");
        config.animationFile = DICTOOL->getStringValue_json(dic, "animation");
        config.gemId = DICTOOL->getIntValue_json(dic, "gem_id");
        config.gemCount = DICTOOL->getIntValue_json(dic, "gem_count");
        config.stageId = DICTOOL->getIntValue_json(dic, "stage_id");
        towerConfigMap[config.towerId] = config;
    }
    return;
}

void Tower::deleteJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "tower_info.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return;
    }
    
    FileUtils::getInstance()->removeFile(path);
}

void Tower::save2Json()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "tower_info.json";
    
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

bool Tower::readJson()
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += "tower_info.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return false;
    }
    
    coreStr = FileUtils::getInstance()->getStringFromFile(path);
    if (!parseJson()) {
        return false;
    }
    
    return true;
}

bool Tower::parseJson()
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
    
    
    towerData.openCount = DICTOOL->getIntValue_json(_doc, "open_count");
    towerData.currTower = DICTOOL->getIntValue_json(_doc, "curr_tower");
    towerData.resetCount = DICTOOL->getIntValue_json(_doc, "reset_count");
    towerData.maxResetCount = DICTOOL->getIntValue_json(_doc, "max_reset_count");
    towerData.sweepState = DICTOOL->getIntValue_json(_doc, "sweep_state");
    return true;
}

void Tower::save2EmptyCoreStr()
{
    rapidjson::Document _doc;
    if (coreStr != "") {
        return;
    }else{
        _doc.SetObject();
    }
    
    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    _doc.AddMember("open_count", 1, allocator);
    _doc.AddMember("curr_tower", 1, allocator);
    _doc.AddMember("reset_count", 1, allocator);
    _doc.AddMember("max_reset_count", 1, allocator);
    _doc.AddMember("sweep_state", 0, allocator);
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
}

int Tower::refreshTower()
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

    towerData.resetCount = towerData.maxResetCount;
    _doc["reset_count"] = towerData.resetCount;
    
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

int Tower::resetTower()
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
    towerData.resetCount = DICTOOL->getIntValue_json(_doc, "reset_count");
    if (towerData.resetCount <= 0) {
        return 2;
    }
    towerData.resetCount--;
    towerData.resetCount = MAX(0, towerData.resetCount);
    towerData.currTower = 1;
    _doc["reset_count"] = towerData.resetCount;
    _doc["curr_tower"] = towerData.currTower;
    
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
int Tower::checkSweepTower()
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
    
    towerData.currTower = DICTOOL->getIntValue_json(_doc, "curr_tower");
    towerData.openCount = DICTOOL->getIntValue_json(_doc, "open_count");
    if (towerData.currTower >= towerData.openCount) {
        return 2;
    }
    towerData.sweepState = DICTOOL->getIntValue_json(_doc, "sweep_state");
    if (towerData.sweepState != 0) {
        return 3;
    }
    
    return 0;
}
int Tower::startSweepTower()
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
    
    towerData.currTower = DICTOOL->getIntValue_json(_doc, "curr_tower");
    towerData.openCount = DICTOOL->getIntValue_json(_doc, "open_count");
    if (towerData.currTower >= towerData.openCount) {
        return 2;
    }
    towerData.sweepState = DICTOOL->getIntValue_json(_doc, "sweep_state");
    if (towerData.sweepState != 0) {
        return 3;
    }
    
    towerData.sweepState = 1;
    _doc["sweep_state"] = towerData.sweepState;
    
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

int Tower::endSweepTower()
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
    
    towerData.sweepState = DICTOOL->getIntValue_json(_doc, "sweep_state");
    if (towerData.sweepState != 1) {
        return 2;
    }
    
    towerData.sweepState = 2;
    _doc["sweep_state"] = towerData.sweepState;
    
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

int Tower::takeSweepReward()
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
    
    towerData.sweepState = DICTOOL->getIntValue_json(_doc, "sweep_state");
    if (towerData.sweepState != 2) {
        return 2;
    }
    
    towerData.currTower = DICTOOL->getIntValue_json(_doc, "curr_tower");
    towerData.openCount = DICTOOL->getIntValue_json(_doc, "open_count");
    towerData.currTower = towerData.openCount;

    towerData.sweepState = 0;
    _doc["sweep_state"] = towerData.sweepState;
    _doc["curr_tower"] = towerData.currTower;

    
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

int Tower::start(int towerId)
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
    
    towerData.sweepState = DICTOOL->getIntValue_json(_doc, "sweep_state");
    if (towerData.sweepState != 0) {
        return 2;
    }
    
    return 0;
}

int Tower::win(int towerId)
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
    
    towerData.currTower = DICTOOL->getIntValue_json(_doc, "curr_tower");
    towerData.openCount = DICTOOL->getIntValue_json(_doc, "open_count");
    if (towerId > towerConfigMap.size()) {
        return 2;
    }
    
    towerData.currTower++;
    if (towerId >= towerData.openCount) {
        towerData.openCount++;
        towerData.openCount = MIN(towerData.openCount, (int)towerConfigMap.size());
        towerData.currTower = towerData.openCount;
        _doc["open_count"] = towerData.openCount;
    }
    _doc["curr_tower"] = towerData.currTower;
    
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

void Tower::getSweepGems(std::map<int, int> &gems)
{
    if (coreStr == "") {
        return;
    }

    rapidjson::Document _doc;
    std::string tempStr = decode(coreStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    if(!_doc.IsObject())
    {
        return;
    }

    towerData.currTower = DICTOOL->getIntValue_json(_doc, "curr_tower");
    towerData.openCount = DICTOOL->getIntValue_json(_doc, "open_count");
    for (int i = towerData.currTower; i < towerData.openCount; i++) {
        int gemId = towerConfigMap[i].gemId;
        int gemCount = towerConfigMap[i].gemCount;
        gems[gemId] = gemCount;
    }
}

void Tower::addMaxResetCount(int val)
{
    if (coreStr == "") {
        return;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(coreStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    if(!_doc.IsObject())
    {
        return;
    }
    towerData.resetCount = DICTOOL->getIntValue_json(_doc, "reset_count");
    towerData.resetCount += val;
    towerData.maxResetCount = DICTOOL->getIntValue_json(_doc, "max_reset_count");
    towerData.maxResetCount += val;
    towerData.maxResetCount = MAX(0, towerData.maxResetCount);
    _doc["max_reset_count"] = towerData.maxResetCount;
    _doc["reset_count"] = towerData.resetCount;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2Json();
}
