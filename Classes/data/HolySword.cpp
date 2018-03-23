//
//  HolySword.cpp
//  life
//
//  Created by ff on 17/8/7.
//
//

#include "HolySword.h"

#include "utils/GameReader.h"
#include "GameDefine.h"
#include "utils/libs.h"

USING_NS_CC;
using namespace cocostudio;
using namespace rapidjson;

std::map<int, int> HolySword::swordConfigIdTable;
std::map<int, int> HolySword::levelConfigIdTable;

HolySword::HolySword():
coreStr(""),
swordsStr("")
{
    swordsMap.clear();
    levelConfigMap.clear();
    maxLevel.clear();
}

HolySword::~HolySword()
{
    
}

void HolySword::readSwordConfigFile()
{
    auto doc = GameReader::getDocInstance(HOLY_SWORD_CONFIG_FILE);
    if (swordConfigIdTable.empty()) {
        GameReader::initIdTable(*doc, "id", swordConfigIdTable);
    }
    
    swordConfigMap.clear();
    for (int i = 0; i < swordConfigIdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        
        HOLY_SWORD_CONFIG config;
        config.swordId = DICTOOL->getIntValue_json(dic, "id");
        config.name = DICTOOL->getStringValue_json(dic, "name");
        config.image = DICTOOL->getStringValue_json(dic, "image");
        config.icon = DICTOOL->getStringValue_json(dic, "icon");
        config.pvpImg = DICTOOL->getStringValue_json(dic, "pvp_img");
        config.next = DICTOOL->getIntValue_json(dic, "next");
        config.fighterId = DICTOOL->getIntValue_json(dic, "fighter_id");
        config.lockLevel = DICTOOL->getIntValue_json(dic, "lock_level");

        swordConfigMap[config.swordId] = config;
    }
}

void HolySword::readLevelConfigFile()
{
    auto doc = GameReader::getDocInstance(SWORD_LEVEL_CONFIG_FILE);
    if (levelConfigIdTable.empty()) {
        GameReader::initIdTable(*doc, "id", levelConfigIdTable);
    }
    
    levelConfigMap.clear();
    maxLevel.clear();
    for (int i = 0; i < levelConfigIdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        
        HOLY_SWORD_LEVEL_CONFIG config;
        config.levelId = DICTOOL->getIntValue_json(dic, "id");
        config.swordId = DICTOOL->getIntValue_json(dic, "holy_sword_id");
        config.level = DICTOOL->getIntValue_json(dic, "level");
        config.costGold = DICTOOL->getIntValue_json(dic, "cost_gold");
        config.costStone = DICTOOL->getIntValue_json(dic, "cost_stone");
        config.deltaHp = DICTOOL->getIntValue_json(dic, "hp");
        config.deltaAttack = DICTOOL->getIntValue_json(dic, "attack");
        config.deltaDefence = DICTOOL->getIntValue_json(dic, "defence");
        levelConfigMap[config.levelId] = config;
        if (maxLevel.find(config.swordId) != maxLevel.end()) {
            if (maxLevel[config.swordId] < config.level) {
                maxLevel[config.swordId] = config.level;
            }
        }else{
            maxLevel[config.swordId] = config.level;
        }
    }
}

void HolySword::deleteInfoJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "holy_sword_info.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return;
    }
    
    FileUtils::getInstance()->removeFile(path);
}

void HolySword::save2InfoJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "holy_sword_info.json";
    
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

bool HolySword::readInfoJson()
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += "holy_sword_info.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return false;
    }
    
    coreStr = FileUtils::getInstance()->getStringFromFile(path);
    if (!parseInfoJson()) {
        return false;
    }
    
    return true;
}

bool HolySword::parseInfoJson()
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
    
    info.hp = DICTOOL->getIntValue_json(_doc, "hp");
    info.attack = DICTOOL->getIntValue_json(_doc, "attack");
    info.defence = DICTOOL->getIntValue_json(_doc, "defence");
    info.stoneCount = DICTOOL->getIntValue_json(_doc, "stone_count");
    info.fightSwordId = DICTOOL->getIntValue_json(_doc, "fight_sword");

    return true;
}

void HolySword::save2EmptyCoreStr()
{
    rapidjson::Document _doc;
    if (coreStr != "") {
        return;
    }else{
        _doc.SetObject();
    }
    
    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    _doc.AddMember("hp", 0, allocator);
    _doc.AddMember("attack", 0, allocator);
    _doc.AddMember("defence", 0, allocator);
    _doc.AddMember("stone_count", 0, allocator);
    _doc.AddMember("fight_sword", 0, allocator);
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
}

int HolySword::setFightSword(int swordId)
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

    info.fightSwordId = swordId;
    _doc["fight_sword"] = info.fightSwordId;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2InfoJson();
    return 0;
}

int HolySword::addStone(int val)
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
    
    info.stoneCount = DICTOOL->getIntValue_json(_doc, "stone_count");
    info.stoneCount += val;
    info.stoneCount = MAX(0, info.stoneCount);
    _doc["stone_count"] = info.stoneCount;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2InfoJson();
    return 0;
}

void HolySword::deleteSwordsJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "holy_swords.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return;
    }
    
    FileUtils::getInstance()->removeFile(path);
}

void HolySword::save2SwordsJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "holy_swords.json";
    
    FILE* pFile = fopen(path.c_str(),"wb");
    if (pFile)
    {
        fputs(swordsStr.c_str(),pFile);
        fclose(pFile);
    }else
    {
        CCLOG("UserManage::SaveUserToFile error! can not open %s",path.c_str());
    }
}

bool HolySword::readSwordsJson()
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += "holy_swords.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return false;
    }
    
    swordsStr = FileUtils::getInstance()->getStringFromFile(path);
    if (!parseSwordsJson()) {
        return false;
    }
    
    return true;
}

bool HolySword::parseSwordsJson()
{
    if (swordsStr == "") {
        return false;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(swordsStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return false;
    }
    
    if(!_doc.IsArray())
    {
        return false;
    }
    
    for (int i = 0; i < _doc.Size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, i);
        HOLY_SWORD_DATA data;
        data.swordId = DICTOOL->getIntValue_json(dic, "sword_id");
        data.level = DICTOOL->getIntValue_json(dic, "level");
        data.lock = DICTOOL->getIntValue_json(dic, "lock");
        swordsMap[data.swordId] = data;
    }
    
    return true;
}

void HolySword::save2EmptySwordsStr()
{
    rapidjson::Document _doc;
    if (swordsStr != "") {
        return;
    }else{
        _doc.SetArray();
    }
    
    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    for (auto configId : swordConfigIdTable) {
        rapidjson::Value object(rapidjson::kObjectType);
        object.AddMember("sword_id", configId.first, allocator);
        object.AddMember("level", 1, allocator);
        object.AddMember("lock", 1, allocator);
        _doc.PushBack(object, allocator);
    }
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    swordsStr = encode(s);
}

bool HolySword::isMaxLevel(int swordId)
{
    if (swordsMap[swordId].level >= maxLevel[swordId]) {
        return true;
    }
    
    return false;
}

int HolySword::uplevel(int swordId)
{
    if (coreStr == "") {
        return 1;
    }
    
    rapidjson::Document _infoDoc;
    std::string tempInfoStr = decode(coreStr.c_str());
    _infoDoc.Parse < 0 > (tempInfoStr.c_str());
    if(_infoDoc.HasParseError())
    {
        return 1;
    }
    
    if(!_infoDoc.IsObject())
    {
        return 1;
    }
    
    if (swordsStr == "") {
        return 1;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(swordsStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return 1;
    }
    
    if(!_doc.IsArray())
    {
        return 1;
    }
    
    int index = swordConfigIdTable[swordId];
    const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, index);
    
    swordsMap[swordId].lock = DICTOOL->getIntValue_json(dic, "lock");
    if (swordsMap[swordId].lock == 1) {
        return 4;
    }
    swordsMap[swordId].level = DICTOOL->getIntValue_json(dic, "level");
    if (isMaxLevel(swordId)) {
        return 2;
    }
    
    info.stoneCount = DICTOOL->getIntValue_json(_infoDoc, "stone_count");
    int nextLevelId = checkStone(swordId);
    if (nextLevelId == 0) {
        return 3;
    }
    
    swordsMap[swordId].level++;
    _doc[swordConfigIdTable[swordId]]["level"] = swordsMap[swordId].level;
    if (swordsMap[swordId].level >= swordConfigMap[swordId].lockLevel) {
        int nextSword = swordConfigMap[swordId].next;
        if(swordsMap[nextSword].lock == 1) {
            unlockSword(nextSword, _infoDoc, _doc);
        }
    }
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    swordsStr = encode(s);
    save2SwordsJson();
    
    info.hp = DICTOOL->getIntValue_json(_infoDoc, "hp");
    info.attack = DICTOOL->getIntValue_json(_infoDoc, "attack");
    info.defence = DICTOOL->getIntValue_json(_infoDoc, "defence");
    
    info.hp += levelConfigMap[nextLevelId].deltaHp;
    info.attack += levelConfigMap[nextLevelId].deltaAttack;
    info.defence += levelConfigMap[nextLevelId].deltaDefence;
    info.stoneCount -= levelConfigMap[nextLevelId].costStone;
    _infoDoc["hp"] = info.hp;
    _infoDoc["attack"] = info.attack;
    _infoDoc["defence"] = info.defence;
    _infoDoc["stone_count"] = info.stoneCount;
    
    StringBuffer buffInfo;
    rapidjson::Writer<StringBuffer> writerInfo(buffInfo);
    _infoDoc.Accept(writerInfo);
    std::string s1 = StringUtils::format("%s", buffInfo.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s1);
    save2InfoJson();
    
    return 0;
}

int HolySword::checkStone(int swordId)
{
    int nextLevl = swordsMap[swordId].level + 1;
    for (auto config : levelConfigMap) {
        if (config.second.swordId != swordId ) {
            continue;
        }
        if (config.second.level != nextLevl) {
            continue;
        }
        
        if (info.stoneCount >= config.second.costStone) {
            return config.first;
        }else{
            return 0;
        }
    }
    return 0;
}

void HolySword::unlockSword(int swordId, rapidjson::Document &coreDoc, rapidjson::Document &swordsDoc)
{

    swordsMap[swordId].lock = 0;
    swordsDoc[swordConfigIdTable[swordId]]["lock"] = 0;

    int originLevelId = getLevelId(swordId, 1);
    if (originLevelId != 0) {
        info.hp += levelConfigMap[originLevelId].deltaHp;
        info.attack += levelConfigMap[originLevelId].deltaAttack;
        info.defence += levelConfigMap[originLevelId].deltaDefence;
        coreDoc["hp"] = info.hp;
        coreDoc["attack"] = info.attack;
        coreDoc["defence"] = info.defence;
    }

}

void HolySword::unlockAndSave(int swordId)
{
    if (coreStr == "") {
        return;
    }
    
    rapidjson::Document _infoDoc;
    std::string tempInfoStr = decode(coreStr.c_str());
    _infoDoc.Parse < 0 > (tempInfoStr.c_str());
    if(_infoDoc.HasParseError())
    {
        return;
    }
    
    if(!_infoDoc.IsObject())
    {
        return;
    }
    
    if (swordsStr == "") {
        return;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(swordsStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    if(!_doc.IsArray())
    {
        return;
    }
    unlockSword(swordId, _infoDoc, _doc);
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    swordsStr = encode(s);
    save2SwordsJson();
    
    StringBuffer buffInfo;
    rapidjson::Writer<StringBuffer> writerInfo(buffInfo);
    _infoDoc.Accept(writerInfo);
    std::string s1 = StringUtils::format("%s", buffInfo.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s1);
    save2InfoJson();
}

int HolySword::getLevelId(int swordId, int level)
{
//    int nextLevl = swordsMap[swordId].level + 1;
    for (auto config : levelConfigMap) {
        if (config.second.swordId != swordId ) {
            continue;
        }
        if (config.second.level != level) {
            continue;
        }
        return config.first;
    }
    return 0;
}

void HolySword::checkTip(int gold)
{
    tip = false;
    uplevelTip = 0;
    for (auto data : swordsMap) {
        uplevelTip++;
        if (data.second.lock == 1) {
            return;
        }
        int swordId = data.second.swordId;
        if (isMaxLevel(swordId)) {
            continue;
        }
        int swordLevel = data.second.level;
        int currLevel = getLevelId(swordId, swordLevel);
        int costGold = levelConfigMap[currLevel].costGold;
        if (costGold > gold) {
            return;
        }
        if (checkStone(swordId) == 0) {
            return;
        }
        
        tip = true;
        return;
    }
}
