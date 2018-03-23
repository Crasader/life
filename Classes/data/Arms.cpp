//
//  Arms.cpp
//  life
//
//  Created by ff on 17/8/8.
//
//

#include "Arms.h"
#include "utils/GameReader.h"
#include "GameDefine.h"
#include "utils/libs.h"

USING_NS_CC;
using namespace cocostudio;
using namespace rapidjson;

std::map<int, int> Arms::armsConfigIdTable;
std::map<int, int> Arms::levelConfigIdTable;
std::map<int, int> Arms::gemConfigIdTable;

Arms::Arms():
coreStr(""),
totalHp(0),
totalAttack(0),
totalDefence(0),
totalCrit(0),
totalDecrit(0)
{
    armsMap.clear();
    gemCountMap.clear();
}

Arms::~Arms()
{
    
}

void Arms::readArmsConfigFile()
{
    auto doc = GameReader::getDocInstance(ARMS_CONFIG_FILE);
    if (armsConfigIdTable.empty()) {
        GameReader::initIdTable(*doc, "id", armsConfigIdTable);
    }
    
    armsConfigMap.clear();
    for (int i = 0; i < armsConfigIdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        
        ARMS_CONFIG config;
        config.armsId = DICTOOL->getIntValue_json(dic, "id");
        config.name = DICTOOL->getStringValue_json(dic, "name");
        config.icon = DICTOOL->getStringValue_json(dic, "icon");
        config.type = DICTOOL->getIntValue_json(dic, "type");
        config.unlockLevel[0] = 0;
        for (int i = 0; i < 5; i++) {
            config.unlockLevel[i+1] =
            DICTOOL->getIntValue_json(dic, GameUtils::format("unlock%d_level", i+1).c_str());
        }
        
        armsConfigMap[config.armsId] = config;
    }
}

void Arms::readLevelConfigFile()
{
    auto doc = GameReader::getDocInstance(ARMS_LEVEL_CONFIG_FILE);
    if (levelConfigIdTable.empty()) {
        GameReader::initIdTable(*doc, "id", levelConfigIdTable);
    }
    
    levelConfigMap.clear();
    maxLevel.clear();
    for (int i = 0; i < levelConfigIdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        
        ARMS_LEVEL_CONFIG config;
        config.levelId = DICTOOL->getIntValue_json(dic, "id");
        config.armsId = DICTOOL->getIntValue_json(dic, "type");
        config.level = DICTOOL->getIntValue_json(dic, "level");
        config.costFame = DICTOOL->getIntValue_json(dic, "cost_fame");
        config.deltaHp = DICTOOL->getIntValue_json(dic, "hp");
        config.deltaAttack = DICTOOL->getIntValue_json(dic, "attack");
        config.deltaDefence = DICTOOL->getIntValue_json(dic, "defence");
        config.deltaCrit = DICTOOL->getIntValue_json(dic, "crit");
        config.deltaDecrit = DICTOOL->getIntValue_json(dic, "decrit");
        
        levelConfigMap[config.levelId] = config;

        if (maxLevel.find(config.armsId) != maxLevel.end()) {
            if (maxLevel[config.armsId] < config.level) {
                maxLevel[config.armsId] = config.level;
            }
        }else{
            maxLevel[config.armsId] = config.level;
        }
    }
}

void Arms::readGemConfigFile()
{
    auto doc = GameReader::getDocInstance(GEM_CONFIG_FILE);
    if (gemConfigIdTable.empty()) {
        GameReader::initIdTable(*doc, "id", gemConfigIdTable);
    }
    
    gemsConfigMap.clear();
    for (int i = 0; i < gemConfigIdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        
        GEM_CONFIG config;
        config.gemId = DICTOOL->getIntValue_json(dic, "id");
        config.icon = DICTOOL->getStringValue_json(dic, "icon");
        config.type = DICTOOL->getIntValue_json(dic, "type");
        config.level = DICTOOL->getIntValue_json(dic, "level");
        config.value = DICTOOL->getIntValue_json(dic, "value");
        
        gemsConfigMap[config.gemId] = config;
    }
}

void Arms::deleteCoreJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "arms_info.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return;
    }
    
    FileUtils::getInstance()->removeFile(path);
}

void Arms::save2CoreJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "arms_info.json";
    
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

bool Arms::readCoreJson()
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += "arms_info.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return false;
    }
    
    coreStr = FileUtils::getInstance()->getStringFromFile(path);
    if (!parseCoreJson()) {
        return false;
    }
    
    return true;
}

bool Arms::parseCoreJson()
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
    
    if(!_doc.IsArray())
    {
        return false;
    }
    
    for (int i = 0; i < _doc.Size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, i);
        ARMS_INFO data;
        data.armsId = DICTOOL->getIntValue_json(dic, "arms_id");
        data.level = DICTOOL->getIntValue_json(dic, "level");
        data.hp = DICTOOL->getIntValue_json(dic, "hp");
        data.attack = DICTOOL->getIntValue_json(dic, "attack");
        data.defence = DICTOOL->getIntValue_json(dic, "defence");
        data.crit = DICTOOL->getIntValue_json(dic, "crit");
        data.decrit = DICTOOL->getIntValue_json(dic, "decrit");
        for (int i = 0 ; i < 6; i++) {
            data.gemsId[i] = DICTOOL->getIntValue_json(dic, GameUtils::format("gem_id%d", i+1).c_str());
        }
        for (int i = 0; i < 6; i++) {
            data.lockPos[i] = DICTOOL->getIntValue_json(dic, GameUtils::format("lock_pos%d", i+1).c_str());
        }
        armsMap[data.armsId] = data;
        
        totalHp += data.hp;
        totalAttack += data.attack;
        totalDefence += data.defence;
        totalCrit += data.crit;
        totalDecrit += data.decrit;
    }
    
    return true;
}

void Arms::save2EmptyCoreStr()
{
    rapidjson::Document _doc;
    if (coreStr != "") {
        return;
    }else{
        _doc.SetArray();
    }
    
    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    for (auto configId : armsConfigIdTable) {
        rapidjson::Value object(rapidjson::kObjectType);
        object.AddMember("arms_id", configId.first, allocator);
        object.AddMember("level", 1, allocator);
        object.AddMember("hp", 0, allocator);
        object.AddMember("attack", 0, allocator);
        object.AddMember("defence", 0, allocator);
        object.AddMember("crit", 0, allocator);
        object.AddMember("decrit", 0, allocator);
        object.AddMember("gem_id1", 0, allocator);
        object.AddMember("gem_id2", 0, allocator);
        object.AddMember("gem_id3", 0, allocator);
        object.AddMember("gem_id4", 0, allocator);
        object.AddMember("gem_id5", 0, allocator);
        object.AddMember("gem_id6", 0, allocator);
        object.AddMember("lock_pos1", 0, allocator);
        object.AddMember("lock_pos2", 1, allocator);
        object.AddMember("lock_pos3", 1, allocator);
        object.AddMember("lock_pos4", 1, allocator);
        object.AddMember("lock_pos5", 1, allocator);
        object.AddMember("lock_pos6", 1, allocator);
        _doc.PushBack(object, allocator);
    }
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
}

void Arms::deleteGemJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "gem_info.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return;
    }
    
    FileUtils::getInstance()->removeFile(path);
}

void Arms::save2GemJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "gem_info.json";
    
    FILE* pFile = fopen(path.c_str(),"wb");
    if (pFile)
    {
        fputs(gemStr.c_str(),pFile);
        fclose(pFile);
    }else
    {
        CCLOG("UserManage::SaveUserToFile error! can not open %s",path.c_str());
    }
}

bool Arms::readGemJson()
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += "gem_info.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return false;
    }
    
    gemStr = FileUtils::getInstance()->getStringFromFile(path);
    if (!parseGemJson()) {
        return false;
    }
    
    return true;
}

bool Arms::parseGemJson()
{
    if (gemStr == "") {
        return false;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(gemStr.c_str());
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
        GEM_INFO data;
        data.gemId = DICTOOL->getIntValue_json(dic, "gem_id");
        data.count = DICTOOL->getIntValue_json(dic, "count");
        gemCountMap[data.gemId] = data.count;
    }
    
    return true;
}

void Arms::save2EmptyGemStr()
{
    rapidjson::Document _doc;
    if (gemStr != "") {
        return;
    }else{
        _doc.SetArray();
    }
    
    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    for (auto configId : gemConfigIdTable) {
        rapidjson::Value object(rapidjson::kObjectType);
        object.AddMember("gem_id", configId.first, allocator);
        object.AddMember("count", ((configId.first==1)?1:0), allocator);
        _doc.PushBack(object, allocator);
    }
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    gemStr = encode(s);
}

int Arms::getLevelId(int armsId, int level)
{
    for (auto config : levelConfigMap) {
        if (config.second.armsId != armsId ) {
            continue;
        }
        if (config.second.level != level) {
            continue;
        }
        return config.first;
    }
    return 0;
}
void Arms::unlockPos(int armsId, int level, rapidjson::Document &doc)
{
    int index = armsConfigIdTable[armsId];

    for (int i = 0; i < 6; i++) {
        int unlockLevel = armsConfigMap[armsId].unlockLevel[i];
        if (unlockLevel > level) {
            continue;
        }
        if (armsMap[armsId].lockPos[i] == 0) {
            continue;
        }
        armsMap[armsId].lockPos[i] = 0;
        doc[index][GameUtils::format("lock_pos%d", i+1).c_str()] = 0;
    }
}

bool Arms::isMaxLevel(int armsId)
{
    if (armsMap[armsId].level >= maxLevel[armsId]) {
        return true;
    }
    
    return false;
}

int Arms::uplevelArms(int armsId)
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
    
    if(!_doc.IsArray())
    {
        return 1;
    }

    int index = armsConfigIdTable[armsId];
    const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, index);

    armsMap[armsId].level = DICTOOL->getIntValue_json(dic, "level");
    if (isMaxLevel(armsId)) {
        return 2;
    }
    armsMap[armsId].level++;
    _doc[index]["level"] = armsMap[armsId].level;
    
    for (int i = 0; i < 6; i++) {
        armsMap[armsId].lockPos[i] = DICTOOL->getIntValue_json(dic, GameUtils::format("lock_pos%d", i+1).c_str());
    }
    unlockPos(armsId, armsMap[armsId].level, _doc);
    
    int levelId = getLevelId(armsId, armsMap[armsId].level);
    armsMap[armsId].hp += levelConfigMap[levelId].deltaHp;
    armsMap[armsId].attack += levelConfigMap[levelId].deltaAttack;
    armsMap[armsId].defence += levelConfigMap[levelId].deltaDefence;
    armsMap[armsId].crit += levelConfigMap[levelId].deltaCrit;
    armsMap[armsId].decrit += levelConfigMap[levelId].deltaDecrit;
    _doc[index]["hp"] = armsMap[armsId].hp;
    _doc[index]["attack"] = armsMap[armsId].attack;
    _doc[index]["defence"] = armsMap[armsId].defence;
    _doc[index]["crit"] = armsMap[armsId].crit;
    _doc[index]["decrit"] = armsMap[armsId].decrit;
    
    totalHp += levelConfigMap[levelId].deltaHp;
    totalAttack += levelConfigMap[levelId].deltaAttack;
    totalDefence += levelConfigMap[levelId].deltaDefence;
    totalCrit += levelConfigMap[levelId].deltaCrit;
    totalDecrit += levelConfigMap[levelId].deltaDecrit;

    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2CoreJson();
    return 0;
}

void Arms::filterGems(int type, std::map<int, int> &gemIds)
{
    gemIds.clear();
    for (auto config : gemsConfigMap) {
        if (config.second.type != type) {
            continue;
        }
        
        gemIds[config.second.level] = config.first;
    }
}

int Arms::assembleGem(int gemId)
{
    if(gemsConfigMap.find(gemId) == gemsConfigMap.end()) {
        return 2;
    }
    
    int gemType = gemsConfigMap[gemId].type;
    std::map<int, int> gemIds;
    filterGems(gemType, gemIds);
    int gemLevel = gemsConfigMap[gemId].level;
    
    if (gemIds.find(gemLevel+1) == gemIds.end()) {
        return 3;
    }
    int nextGemId = gemIds[gemLevel+1];
    
    if (gemStr == "") {
        return 1;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(gemStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return 1;
    }
    
    if(!_doc.IsArray())
    {
        return 1;
    }

    int index = gemConfigIdTable[gemId];
    const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, index);

    gemCountMap[gemId] = DICTOOL->getIntValue_json(dic, "count");
    if (gemCountMap[gemId] < assembleGemCount) {
        return 4;
    }
    
    gemCountMap[gemId] -= assembleGemCount;
    
    int nextIndex = gemConfigIdTable[nextGemId];
    const rapidjson::Value &nextDic = DICTOOL->getSubDictionary_json(_doc, nextIndex);
    
    gemCountMap[nextGemId] = DICTOOL->getIntValue_json(nextDic, "count");
    gemCountMap[nextGemId]++;
    
    _doc[index]["count"] = gemCountMap[gemId];
    _doc[nextIndex]["count"] = gemCountMap[nextGemId];
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    gemStr = encode(s);
    save2GemJson();
    
    return 0;
}

int Arms::fillGem(int armsId, int gemId)
{
    if (gemStr == "") {
        return 1;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(gemStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return 1;
    }
    
    if(!_doc.IsArray())
    {
        return 1;
    }
    
    int index = gemConfigIdTable[gemId];
    const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, index);
    
    gemCountMap[gemId] = DICTOOL->getIntValue_json(dic, "count");
    if (gemCountMap[gemId] <= 0) {
        return 2;
    }
    
    if (coreStr == "") {
        return 1;
    }
    
    rapidjson::Document _coreDoc;
    std::string tempCoreStr = decode(coreStr.c_str());
    _coreDoc.Parse < 0 > (tempCoreStr.c_str());
    if(_coreDoc.HasParseError())
    {
        return 1;
    }
    
    if(!_coreDoc.IsArray())
    {
        return 1;
    }

    int validPos = 0;
    for (int i = 0; i < 6; i++) {
        if(armsMap[armsId].lockPos[i] == 1) {
            continue;
        }
        if (armsMap[armsId].gemsId[i] != 0) {
            continue;
        }
        
        validPos = i+1;
        break;
    }
    if (validPos == 0) {
        return 3;
    }
    int armsIndex = armsConfigIdTable[armsId];
    armsMap[armsId].gemsId[validPos-1] = gemId;
    _coreDoc[armsIndex][GameUtils::format("gem_id%d", validPos).c_str()] = gemId;
    switch (gemsConfigMap[gemId].type) {
        case 1:
        {
            armsMap[armsId].hp += gemsConfigMap[gemId].value;
            _coreDoc[armsIndex]["hp"] = armsMap[armsId].hp;
            totalHp += gemsConfigMap[gemId].value;
        }
            break;
        case 2:
        {
            armsMap[armsId].attack += gemsConfigMap[gemId].value;
            _coreDoc[armsIndex]["attack"] = armsMap[armsId].attack;
            totalAttack += gemsConfigMap[gemId].value;
        }
            break;
        case 3:
        {
            armsMap[armsId].defence += gemsConfigMap[gemId].value;
            _coreDoc[armsIndex]["defence"] = armsMap[armsId].defence;
            totalDefence += gemsConfigMap[gemId].value;
        }
            break;
        case 4:
        {
            armsMap[armsId].crit += gemsConfigMap[gemId].value;
            _coreDoc[armsIndex]["crit"] = armsMap[armsId].crit;
            totalCrit += gemsConfigMap[gemId].value;
        }
            break;
        case 5:
        {
            armsMap[armsId].decrit += gemsConfigMap[gemId].value;
            _coreDoc[armsIndex]["decrit"] = armsMap[armsId].decrit;
            totalDecrit += gemsConfigMap[gemId].value;
        }
            break;
            
        default:
            break;
    }
    
    
    gemCountMap[gemId]--;
    _doc[index]["count"] = gemCountMap[gemId];
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    gemStr = encode(s);
    save2GemJson();
    
    StringBuffer buffCore;
    rapidjson::Writer<StringBuffer> writer1(buffCore);
    _coreDoc.Accept(writer1);
    std::string s1 = StringUtils::format("%s", buffCore.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s1);
    save2CoreJson();
    
    return 0;

}

int Arms::removeGem(int armsId, int pos)
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
    
    if(!_doc.IsArray())
    {
        return 1;
    }
    

    if (gemStr == "") {
        return 1;
    }
    
    rapidjson::Document _gemDoc;
    std::string tempGemStr = decode(gemStr.c_str());
    _gemDoc.Parse < 0 > (tempGemStr.c_str());
    if(_gemDoc.HasParseError())
    {
        return 1;
    }
    
    if(!_gemDoc.IsArray())
    {
        return 1;
    }
    
    int index = armsConfigIdTable[armsId];
    const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, index);
    armsMap[armsId].gemsId[pos-1] =
        DICTOOL->getIntValue_json(dic, GameUtils::format("gem_id%d", pos).c_str());
    int gemId = armsMap[armsId].gemsId[pos-1];
    armsMap[armsId].gemsId[pos-1] = 0;
    _doc[index][GameUtils::format("gem_id%d", pos).c_str()] = 0;
    switch (gemsConfigMap[gemId].type) {
        case 1:
        {
            armsMap[armsId].hp -= gemsConfigMap[gemId].value;
            _doc[index]["hp"] = armsMap[armsId].hp;
            totalHp -= gemsConfigMap[gemId].value;
        }
            break;
        case 2:
        {
            armsMap[armsId].attack -= gemsConfigMap[gemId].value;
            _doc[index]["attack"] = armsMap[armsId].attack;
            totalAttack -= gemsConfigMap[gemId].value;
        }
            break;
        case 3:
        {
            armsMap[armsId].defence -= gemsConfigMap[gemId].value;
            _doc[index]["defence"] = armsMap[armsId].defence;
            totalDefence -= gemsConfigMap[gemId].value;
        }
            break;
        case 4:
        {
            armsMap[armsId].crit -= gemsConfigMap[gemId].value;
            _doc[index]["crit"] = armsMap[armsId].crit;
            totalCrit -= gemsConfigMap[gemId].value;
        }
            break;
        case 5:
        {
            armsMap[armsId].decrit -= gemsConfigMap[gemId].value;
            _doc[index]["decrit"] = armsMap[armsId].decrit;
            totalDecrit -= gemsConfigMap[gemId].value;
        }
            break;
            
        default:
            break;
    }
    
    int gemIndex = gemConfigIdTable[gemId];
    const rapidjson::Value &gemDic = DICTOOL->getSubDictionary_json(_gemDoc, gemIndex);
    
    gemCountMap[gemId] = DICTOOL->getIntValue_json(gemDic, "count");
    gemCountMap[gemId]++;
    _gemDoc[gemIndex]["count"] = gemCountMap[gemId];

    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2CoreJson();
    
    StringBuffer buffGem;
    rapidjson::Writer<StringBuffer> writer1(buffGem);
    _gemDoc.Accept(writer1);
    std::string s1 = StringUtils::format("%s", buffGem.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    gemStr = encode(s1);
    save2GemJson();
    return 0;
}

void Arms::addGem(int gemId, int val)
{
    if (gemStr == "") {
        return;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(gemStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    if(!_doc.IsArray())
    {
        return;
    }
    
    int index = gemConfigIdTable[gemId];
    const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, index);
    
    gemCountMap[gemId] = DICTOOL->getIntValue_json(dic, "count");
    gemCountMap[gemId] += val;
    gemCountMap[gemId] = MAX(0, gemCountMap[gemId]);
    _doc[index]["count"] = gemCountMap[gemId];

    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    gemStr = encode(s);
    save2GemJson();
}

void Arms::checkTip(int fame)
{
    tip = false;
    armsTip.clear();
    armsFameTip.clear();
    armsGemIdTip.clear();
    int index = 0;
    for (auto arm : armsMap) {
        armsTip.push_back(false);
        armsFameTip.push_back(false);
        armsGemIdTip.push_back(0);
        int currArmsId = arm.second.armsId;
        if (!isMaxLevel(currArmsId)) {
            // 可升级纹章
            int levelId = getLevelId(currArmsId, armsMap[currArmsId].level);
            int costFame = levelConfigMap[levelId].costFame;
            if (fame >= costFame) {
                tip = true;
                armsTip[index] = true;
                armsFameTip[index] = true;
//                return;
            }
        }
        
        // 纹章镶嵌的宝石最低等级
        int armMinLevel = 6;
        for (int i = 0; i < 6; i++) {
            if (arm.second.lockPos[i] == 1) {
                break;
            }
            int gemId = arm.second.gemsId[i];
            if (gemId == 0) {
                armMinLevel = 0;
                break;
            }
            armMinLevel = MIN(armMinLevel, gemsConfigMap[gemId].level);
        }
        
        std::map<int, int> gemIds;
        filterGems(armsConfigMap[currArmsId].type, gemIds);
        //  拥有的宝石的最大等级
        int gemMaxLevel = 0;
        for (auto gemId : gemIds) {
            int count = gemCountMap[gemId.second];
            if (count <= 0) {
                continue;
            }
            
            gemMaxLevel = MAX(gemMaxLevel, gemsConfigMap[gemId.second].level);
        }
        
        //  可镶嵌更高等级宝石
        if (armMinLevel < gemMaxLevel) {
            tip = true;
            armsTip[index] = true;
            armsGemIdTip[index] = gemMaxLevel;
//            return;
        }
        index++;
    }
}
