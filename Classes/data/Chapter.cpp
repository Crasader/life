//
//  Chapter.cpp
//  life
//
//  Created by ff on 17/7/11.
//
//

#include "Chapter.h"
#include "utils/GameReader.h"
#include "GameDefine.h"
#include "utils/libs.h"

USING_NS_CC;
using namespace cocostudio;
using namespace rapidjson;

std::map<int, int> Chapter::configIdTable;
std::map<int, CHAPTER_CONFIG> Chapter::configMap;
std::map<int, CHAPTER_SAVE_DATA> Chapter::saveMap;

std::map<int, int> Chapter::stageConfigIdTable;

Chapter::Chapter():
coreStr(""),
currChapter(0)
{
    configIdTable.clear();
    stageConfigIdTable.clear();
    configMap.clear();
    saveMap.clear();
    stageStrMap.clear();
    stageConfigMap.clear();
    stageSaveMap.clear();
//    readConfigFile();
//    deleteJson();
}

Chapter::~Chapter()
{
    configIdTable.clear();
    configMap.clear();
    saveMap.clear();
    stageStrMap.clear();
    stageConfigMap.clear();
    stageSaveMap.clear();
}

void Chapter::readConfigFile()
{
    configIdTable.clear();
    configMap.clear();
    
    auto doc = GameReader::getDocInstance(CHAPTER_CONFIG_FILE);
    GameReader::initIdTable(*doc, "id", configIdTable);
    
    for (int i = 0; i < configIdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        
        CHAPTER_CONFIG config;
        config.chapterId = DICTOOL->getIntValue_json(dic, "id");
        config.name = DICTOOL->getStringValue_json(dic, "name");
        config.img = DICTOOL->getStringValue_json(dic, "img");
        config.nextId = DICTOOL->getIntValue_json(dic, "next");
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                config.boxBoundType[j*3+k] =
                    DICTOOL->getIntValue_json(dic, GameUtils::format("box%d_bound%d_type",j+1,k+1).c_str());
                config.boxBoundCount[j*3+k] =
                    DICTOOL->getIntValue_json(dic, GameUtils::format("box%d_bound%d_count",j+1,k+1).c_str());
            }
        }
        configMap[config.chapterId] = config;
    }
}
bool Chapter::readJson()
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += "chapter.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return false;
    }
    
    coreStr = FileUtils::getInstance()->getStringFromFile(path);
    if (!parseJson()) {
        return false;
    }
    
    return true;
}

void Chapter::deleteJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "chapter.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return;
    }
    
    FileUtils::getInstance()->removeFile(path);

    if (configMap.empty()) {
        return;
    }
    
    for (auto config : configMap) {
        int chapterId = config.first;
        std::string path = FileUtils::getInstance()->getWritablePath();
        path += GameUtils::format("chapter%d.json", chapterId);
        if(!FileUtils::getInstance()->isFileExist(path)) {
            continue;
        }
        
        FileUtils::getInstance()->removeFile(path);

    }
}

bool Chapter::parseJson()
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
    saveMap.clear();
    int count = _doc.Size();
    for (int i = 0; i < count; i++) {
        CHAPTER_SAVE_DATA data;
        data.chapterId = DICTOOL->getIntValue_json(_doc[i], "chapterId");
        for (int j = 0; j < 3; j++) {
            data.boxState[j] = DICTOOL->getIntValue_json(_doc[i], GameUtils::format("box%d", (j+1)).c_str());
        }
        data.open = (DICTOOL->getIntValue_json(_doc[i], "open")==1);
        saveMap[data.chapterId] = data;
    }

    
    return true;
}

void Chapter::save2Json()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "chapter.json";
    
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

void Chapter::save2EmptyCoreStr()
{
    rapidjson::Document _doc;
    if (coreStr != "") {
        return;
    }else{
        _doc.SetArray();
    }
    
    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    int chatperCount = configMap.size();
    for (int i = 0; i < chatperCount; i++) {
        rapidjson::Value object(rapidjson::kObjectType);
        object.AddMember("chapterId", i+1, allocator);
        if (i == 0 || i == 1) {
            object.AddMember("box1", -1, allocator);
            object.AddMember("box2", -1, allocator);
            object.AddMember("box3", -1, allocator);
            object.AddMember("open", i==0?1:0, allocator);
//            object.AddMember("open", 1, allocator);
        }else{
            object.AddMember("box1", 0, allocator);
            object.AddMember("box2", 0, allocator);
            object.AddMember("box3", 0, allocator);
            object.AddMember("open", 0, allocator);
//            object.AddMember("open", 1, allocator);
        }
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

void Chapter::openChapter(int chapterId)
{
    if (coreStr == "") {
        return;
    }
    
    if (saveMap.find(chapterId) == saveMap.end()) {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(coreStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    if(!_doc.IsArray())
    {
        return;
    }

    _doc[chapterId-1]["open"] = 1;
    saveMap[chapterId].open = true;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2Json();
}

int Chapter::openStarBox(int chapterId, int boxId)
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
    
    const rapidjson::Value& dic = DICTOOL->getSubDictionary_json(_doc, chapterId-1);
    saveMap[chapterId].boxState[boxId-1] = DICTOOL->getIntValue_json(dic, GameUtils::format("box%d", boxId).c_str());
    if (saveMap[chapterId].boxState[boxId-1] != 1) {
        return 2;
    }

    _doc[chapterId-1][GameUtils::format("box%d", boxId).c_str()] = 2;
    saveMap[chapterId].boxState[boxId-1] = 2;
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

void Chapter::enableStarBox(int chapterId, int boxId)
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
    
    if(!_doc.IsArray())
    {
        return;
    }
    
    const rapidjson::Value& dic = DICTOOL->getSubDictionary_json(_doc, chapterId-1);
    saveMap[chapterId].boxState[boxId-1] = DICTOOL->getIntValue_json(dic, GameUtils::format("box%d", boxId).c_str());
    if (saveMap[chapterId].boxState[boxId-1] != 0) {
        return;
    }
    
    _doc[chapterId-1][GameUtils::format("box%d", boxId).c_str()] = 1;
    saveMap[chapterId].boxState[boxId-1] = 1;
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2Json();

}

int Chapter::getChapterStarCount(int chapterId)
{
    int starCount = 0;
    for(auto stage : stageSaveMap)
    {
        if (!stage.second.open) {
            continue;
        }
        for (int i = 0; i < 3; i++) {
            if (stage.second.star[i] == 1) {
                starCount++;
            }
        }
    }

    return starCount;
}

void Chapter::readStageConfig(int chapterId)
{
    stageConfigMap.clear();
    auto doc = GameReader::getDocInstance(STAGE_CONFIG_FILE);
    if (stageConfigIdTable.empty()) {
        GameReader::initIdTable(*doc, "id", stageConfigIdTable);
    }
    
    for (int i = 0; i < stageConfigIdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        if (DICTOOL->getIntValue_json(dic, "chapter_id") != chapterId) {
            continue;
        }
        
        STAGE_CONFIG data;
        data.stageId = DICTOOL->getIntValue_json(dic, "id");
        data.name = DICTOOL->getStringValue_json(dic, "name");
        data.imgFile = DICTOOL->getStringValue_json(dic, "img");
        data.iconFile = DICTOOL->getStringValue_json(dic, "icon");
        data.musicFile = DICTOOL->getStringValue_json(dic, "music");
        data.chapterId = DICTOOL->getIntValue_json(dic, "chapter_id");
        data.boundsGold = DICTOOL->getIntValue_json(dic, "bounds_gold");
        data.boundsExp = DICTOOL->getIntValue_json(dic, "bounds_exp");
        data.firstGold = DICTOOL->getIntValue_json(dic, "first_gold");
        data.firstExp = DICTOOL->getIntValue_json(dic, "first_exp");
        data.starSecond = DICTOOL->getIntValue_json(dic, "star_second");
        data.starHpPer = DICTOOL->getIntValue_json(dic, "star_hp_per");
        data.starUseBottle = DICTOOL->getIntValue_json(dic, "star_use_bottle");
        data.costVIT = DICTOOL->getIntValue_json(dic, "cost_VIT");
        stageConfigMap[data.stageId] = data;
    }
}

bool Chapter::readStageJson(int chapterId)
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += GameUtils::format("chapter%d.json", chapterId);
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return false;
    }
    
    stageStrMap[chapterId] = FileUtils::getInstance()->getStringFromFile(path);
    if (!parseStageJson(chapterId)) {
        return false;
    }
    
    return true;
}

bool Chapter::parseStageJson(int chapterId)
{
    if (stageStrMap.empty()) {
        return false;
    }
    if (stageStrMap.find(chapterId) == stageStrMap.end()) {
        return false;
    }
    if (stageStrMap[chapterId] == "") {
        return false;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(stageStrMap[chapterId].c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return false;
    }
    
    if(!_doc.IsArray())
    {
        return false;
    }
    stageSaveMap.clear();
    stageStrIdTable.clear();
    int count = _doc.Size();
    for (int i = 0; i < count; i++) {
        STAGE_SAVE_DATA data;
        data.stageId = DICTOOL->getIntValue_json(_doc[i], "stageId");
        for (int j = 0; j < 3; j++) {
            data.star[j] = DICTOOL->getIntValue_json(_doc[i], GameUtils::format("star%d", (j+1)).c_str());
        }
        data.open = (DICTOOL->getIntValue_json(_doc[i], "open")==1);
        stageSaveMap[data.stageId] = data;
        stageStrIdTable[data.stageId] = i;
    }
    
    return true;
}

void Chapter::saveStageJson(int chapterId)
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += GameUtils::format("chapter%d.json",chapterId);
    
    FILE* pFile = fopen(path.c_str(),"wb");
    if (pFile)
    {
        fputs(stageStrMap[chapterId].c_str(),pFile);
        fclose(pFile);
    }else
    {
        CCLOG("UserManage::SaveUserToFile error! can not open %s",path.c_str());
    }
}


void Chapter::save2EmptyStageStr(int chapterId)
{
    rapidjson::Document _doc;
    _doc.SetArray();
    
    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    int stageCount = stageConfigMap.size();
    int index = 0;
    for (auto config : stageConfigMap) {
        rapidjson::Value object(rapidjson::kObjectType);
        object.AddMember("stageId", config.first, allocator);
        object.AddMember("star1", 0, allocator);
//        object.AddMember("star1", 1, allocator);
        object.AddMember("star2", 0, allocator);
        object.AddMember("star3", 0, allocator);
        object.AddMember("open", index==0?1:0, allocator);
//        object.AddMember("open", 1, allocator);
        _doc.PushBack(object, allocator);
        index++;
    }
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    stageStrMap[chapterId] = encode(s);
    saveStageJson(chapterId);
}

bool Chapter::enterChapter(int chapterId)
{
    if (configMap.empty()) {
        return false;
    }
    
    if (configMap.find(chapterId) == configMap.end()) {
        return false;
    }
    
    readStageConfig(chapterId);
    
    if (!readStageJson(chapterId)) {
        save2EmptyStageStr(chapterId);
        parseStageJson(chapterId);
    }
    return true;
}

void Chapter::boundsStar(int chapterId, int stageId, int star[3])
{
    if (stageStrMap.empty()) {
        return;
    }
    if (stageStrMap.find(chapterId) == stageStrMap.end()) {
        return;
    }
    if (stageStrMap[chapterId] == "") {
        return;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(stageStrMap[chapterId].c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    if(!_doc.IsArray())
    {
        return;
    }
    
    int index = stageStrIdTable[stageId];
    const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, index);
    for (int i = 0; i < 3; i++) {
        stageSaveMap[stageId].star[i] = DICTOOL->getIntValue_json(dic, GameUtils::format("star%d", i+1).c_str());
        stageSaveMap[stageId].star[i] |= star[i];
        _doc[index][GameUtils::format("star%d", i+1).c_str()] = stageSaveMap[stageId].star[i];
    }

    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    stageStrMap[chapterId] = encode(s);
    saveStageJson(chapterId);
}

void Chapter::updateStageInfo(int chapterId, int stageId)
{
//    int starCount = 0;
//    for (auto stage : stageSaveMap) {
//        for (int i = 0; i < 3; i++) {
//            if(stage.second.star[i] == 1)
//            {
//                starCount++;
//            }
//        }
//    }
    
    if(stageSaveMap.find(stageId+1) == stageSaveMap.end())
    {
        openChapter(chapterId+1);
        return;
    }else if(stageSaveMap[stageId+1].open) {
        return;
    }
    
    if (stageStrMap.empty()) {
        return;
    }
    if (stageStrMap.find(chapterId) == stageStrMap.end()) {
        return;
    }
    if (stageStrMap[chapterId] == "") {
        return;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(stageStrMap[chapterId].c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    if(!_doc.IsArray())
    {
        return;
    }
    
    int index = stageStrIdTable[stageId+1];
    _doc[index]["open"] = 1;
    stageSaveMap[stageId+1].open = true;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    stageStrMap[chapterId] = encode(s);
    saveStageJson(chapterId);

}

int Chapter::getMaxChapterId()
{
    int maxOpenChapter = 0;
    for (auto chapter : saveMap) {
        if (chapter.second.open) {
            maxOpenChapter = MAX(maxOpenChapter, chapter.first);
        }
    }

    return maxOpenChapter;
}

std::string Chapter::getChapterImg(int chapterId)
{
    if (configMap.find(chapterId) == configMap.end()) {
        return "";
    }
    
    return configMap[chapterId].img;
}
