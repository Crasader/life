//
//  Abyss.cpp
//  life
//
//  Created by ff on 17/9/7.
//
//

#include "Abyss.h"
#include "utils/GameReader.h"
#include "GameDefine.h"
#include "utils/libs.h"

USING_NS_CC;
using namespace cocostudio;
using namespace rapidjson;

std::map<int, int> Abyss::abyssConfigIdTable;

Abyss::Abyss():
coreStr("")
{
    abyssConfigIdTable.clear();
    abyssConfigMap.clear();
}

Abyss::~Abyss()
{
    
}

void Abyss::readAbyssConfigFile()
{
    auto doc = GameReader::getDocInstance(ABYSS_CONFIG_FILE);
    if (abyssConfigIdTable.empty()) {
        GameReader::initIdTable(*doc, "id", abyssConfigIdTable);
    }
    
    abyssConfigMap.clear();
    for (int i = 0; i < abyssConfigIdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        
        ABYSS_CONFIG config;
        config.abyssId = DICTOOL->getIntValue_json(dic, "id");
        config.animationFile = DICTOOL->getStringValue_json(dic, "animation");
        config.petFragId = DICTOOL->getIntValue_json(dic, "pet_fragment_id");
        config.petFragCount = DICTOOL->getIntValue_json(dic, "pet_fragment_count");
        config.stageId = DICTOOL->getIntValue_json(dic, "stage_id");
        abyssConfigMap[config.abyssId] = config;
    }
    return;
}

void Abyss::deleteJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "abyss_info.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return;
    }
    
    FileUtils::getInstance()->removeFile(path);
}

void Abyss::save2Json()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "abyss_info.json";
    
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

bool Abyss::readJson()
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += "abyss_info.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return false;
    }
    
    coreStr = FileUtils::getInstance()->getStringFromFile(path);
    if (!parseJson()) {
        return false;
    }
    
    return true;
}

bool Abyss::parseJson()
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
    
    
    abyssData.openStageCount = DICTOOL->getIntValue_json(_doc, "open_stage_count");
    abyssData.maxResetCount = DICTOOL->getIntValue_json(_doc, "max_reset_count");
    for (int i = 0; i < 10; i++) {
        abyssData.resetCount[i] =
            DICTOOL->getIntValue_json(_doc, GameUtils::format("reset_count_%d", i+1).c_str());
    }
    for (int i = 0; i < 10; i++) {
        abyssData.fightCount[i] =
            DICTOOL->getIntValue_json(_doc, GameUtils::format("fight_count_%d", i+1).c_str());
    }
    return true;
}

void Abyss::save2EmptyCoreStr()
{
    rapidjson::Document _doc;
    if (coreStr != "") {
        return;
    }else{
        _doc.SetObject();
    }
    
    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    _doc.AddMember("open_stage_count", 1, allocator);
    _doc.AddMember("max_reset_count", 0, allocator);
    _doc.AddMember("reset_count_1", 0, allocator);
    _doc.AddMember("reset_count_2", 0, allocator);
    _doc.AddMember("reset_count_3", 0, allocator);
    _doc.AddMember("reset_count_4", 0, allocator);
    _doc.AddMember("reset_count_5", 0, allocator);
    _doc.AddMember("reset_count_6", 0, allocator);
    _doc.AddMember("reset_count_7", 0, allocator);
    _doc.AddMember("reset_count_8", 0, allocator);
    _doc.AddMember("reset_count_9", 0, allocator);
    _doc.AddMember("reset_count_10", 0, allocator);
    _doc.AddMember("fight_count_1", abyssMaxFightCount, allocator);
    _doc.AddMember("fight_count_2", abyssMaxFightCount, allocator);
    _doc.AddMember("fight_count_3", abyssMaxFightCount, allocator);
    _doc.AddMember("fight_count_4", abyssMaxFightCount, allocator);
    _doc.AddMember("fight_count_5", abyssMaxFightCount, allocator);
    _doc.AddMember("fight_count_6", abyssMaxFightCount, allocator);
    _doc.AddMember("fight_count_7", abyssMaxFightCount, allocator);
    _doc.AddMember("fight_count_8", abyssMaxFightCount, allocator);
    _doc.AddMember("fight_count_9", abyssMaxFightCount, allocator);
    _doc.AddMember("fight_count_10", abyssMaxFightCount, allocator);

    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
}

int Abyss::refreshAbyss()
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

    abyssData.maxResetCount = DICTOOL->getIntValue_json(_doc, "max_reset_count");
    for (int i = 0; i < 10; i++) {
        abyssData.resetCount[i] = abyssData.maxResetCount;
        abyssData.fightCount[i] = abyssMaxFightCount;
    }
    for (int i = 0; i < 10; i++) {
        _doc[GameUtils::format("reset_count_%d", i+1).c_str()] = abyssData.resetCount[i];
        _doc[GameUtils::format("fight_count_%d", i+1).c_str()] = abyssData.fightCount[i];
    }
    
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

int Abyss::resetAbyss(int abyssId)
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

    abyssData.resetCount[abyssId-1] = DICTOOL->getIntValue_json(_doc, GameUtils::format("reset_count_%d", abyssId).c_str());
    if (abyssData.resetCount[abyssId-1] <= 0) {
        return 2;
    }
    abyssData.resetCount[abyssId-1]--;
    abyssData.resetCount[abyssId-1] = MAX(0, abyssData.resetCount[abyssId-1]);
//    abyssData.fightCount[abyssId-1] = abyssMaxFightCount;
    abyssData.fightCount[abyssId-1] = 1;
    
    _doc[GameUtils::format("reset_count_%d", abyssId).c_str()] = abyssData.resetCount[abyssId-1];
    _doc[GameUtils::format("fight_count_%d", abyssId).c_str()] = abyssData.fightCount[abyssId-1];

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

void Abyss::addMaxResetCount(int val)
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
    
    abyssData.maxResetCount = DICTOOL->getIntValue_json(_doc, "max_reset_count");
    abyssData.maxResetCount += val;
    abyssData.maxResetCount = MAX(0, abyssData.maxResetCount);
    for (int i = 0; i < 10; i++) {
        abyssData.resetCount[i] += val;
        abyssData.resetCount[i] = MAX(0, abyssData.resetCount[i]);
        abyssData.resetCount[i] = MIN(abyssData.resetCount[i], abyssData.maxResetCount);
    }

    _doc["max_reset_count"] = abyssData.maxResetCount;
    for (int i = 0; i < 10; i++) {
        _doc[GameUtils::format("reset_count_%d", i+1).c_str()] = abyssData.resetCount[i];
    }
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2Json();
}
int Abyss::checkStart(int abyssId)
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
    
    abyssData.openStageCount = DICTOOL->getIntValue_json(_doc, "open_stage_count");
    abyssData.fightCount[abyssId-1] =
    DICTOOL->getIntValue_json(_doc, GameUtils::format("fight_count_%d", abyssId).c_str());
    if (abyssId > (abyssData.openStageCount)) {
        return 2;
    }
    if (abyssData.fightCount[abyssId-1] <= 0) {
        return 3;
    }

    return 0;

}

int Abyss::start(int abyssId)
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

    abyssData.openStageCount = DICTOOL->getIntValue_json(_doc, "open_stage_count");
    abyssData.fightCount[abyssId-1] =
        DICTOOL->getIntValue_json(_doc, GameUtils::format("fight_count_%d", abyssId).c_str());
    if (abyssId > (abyssData.openStageCount)) {
        return 2;
    }
    if (abyssData.fightCount[abyssId-1] <= 0) {
        return 3;
    }
    abyssData.fightCount[abyssId-1]--;
    _doc[GameUtils::format("fight_count_%d", abyssId).c_str()] = abyssData.fightCount[abyssId-1];
    
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

void Abyss::win(int abyssId)
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
    abyssData.openStageCount = DICTOOL->getIntValue_json(_doc, "open_stage_count");
    if (abyssData.openStageCount >= 10) {
        return;
    }
    if (abyssId != abyssData.openStageCount) {
        return;
    }
    abyssData.openStageCount++;
    abyssData.openStageCount = MIN(10, abyssData.openStageCount);
    _doc["open_stage_count"] = abyssData.openStageCount;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2Json();
}
