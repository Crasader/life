//
//  Campaign.cpp
//  life
//
//  Created by ff on 17/11/2.
//
//

#include "Campaign.h"
#include "Arena.h"
#include "utils/GameReader.h"
#include "GameDefine.h"
#include "utils/libs.h"
#include "utils/RandomName.h"

USING_NS_CC;
using namespace cocostudio;
using namespace rapidjson;

std::map<int, int> Campaign::monthAssignConifgIdTable;
std::map<int, int> Campaign::everydayConfigIdTable;
std::map<int, int> Campaign::activeMissionIdTable;
std::map<int, int> Campaign::activeBoxIdTable;
std::map<int, int> Campaign::boundMissionIdTable;
std::map<int, int> Campaign::boundPackageIdTable;
std::map<int, int> Campaign::boundDocIdTable;

Campaign::Campaign():
tip(false)
{
    monthAssignConifgIdTable.clear();
    monthAssignConfigMap.clear();
    everydayConfigIdTable.clear();
    everydayConfigMap.clear();
    activeMissionIdTable.clear();
    activeMissionMap.clear();
    activeBoxIdTable.clear();
    activeBoxMap.clear();
}

Campaign::~Campaign()
{
    
}

void Campaign::readMonthAssignConfig()
{
    auto doc = GameReader::getDocInstance(MONTH_ASSIGN_CONFIG_FILE);
    if (monthAssignConifgIdTable.empty()) {
        GameReader::initIdTable(*doc, "id", monthAssignConifgIdTable);
    }
    
    monthAssignConfigMap.clear();
    for (int i = 0; i < monthAssignConifgIdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        
        MONTH_ASSIGN_CONFIG config;
        config.configId = DICTOOL->getIntValue_json(dic, "id");
        config.dayId = DICTOOL->getIntValue_json(dic, "day_id");
        config.boundType = DICTOOL->getIntValue_json(dic, "bound_type");
        config.boundId = DICTOOL->getIntValue_json(dic, "bound_id");
        config.boundCount = DICTOOL->getIntValue_json(dic, "bound_count");
        config.vipLimit = DICTOOL->getIntValue_json(dic, "vip_limit");
        monthAssignConfigMap[config.configId] = config;
    }
}

void Campaign::readEverydayConfig()
{
    auto doc = GameReader::getDocInstance(EVERYDAY_CONFIG_FILE);
    if (everydayConfigIdTable.empty()) {
        GameReader::initIdTable(*doc, "id", everydayConfigIdTable);
    }
    
    everydayConfigMap.clear();
    for (int i = 0; i < everydayConfigIdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        
        EVERYDAY_CONFIG config;
        config.everydayId = DICTOOL->getIntValue_json(dic, "id");
        config.boundType = DICTOOL->getIntValue_json(dic, "bound_type");
        config.boundId = DICTOOL->getIntValue_json(dic, "bound_id");
        config.boundCount = DICTOOL->getIntValue_json(dic, "bound_count");
        everydayConfigMap[config.everydayId] = config;
    }
}

void Campaign::readActiveMissionConfig()
{
    auto doc = GameReader::getDocInstance(ACTIVE_MISSION_CONFIG_FILE);
    if (activeMissionIdTable.empty()) {
        GameReader::initIdTable(*doc, "id", activeMissionIdTable);
    }
    
    activeMissionMap.clear();
    for (int i = 0; i < activeMissionIdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        
        ACTIVE_MISSION_CONFIG config;
        config.missionId = DICTOOL->getIntValue_json(dic, "id");
        config.boundType = DICTOOL->getIntValue_json(dic, "bound_type");
        config.boundId = DICTOOL->getIntValue_json(dic, "bound_id");
        config.boundCount = DICTOOL->getIntValue_json(dic, "bound_count");
        config.count = DICTOOL->getIntValue_json(dic, "count");
        config.descript = DICTOOL->getStringValue_json(dic, "descript");
        config.activeVal = DICTOOL->getIntValue_json(dic, "active_val");
        config.activeType = DICTOOL->getIntValue_json(dic, "active_type");
        config.activeThres = DICTOOL->getIntValue_json(dic, "active_thres");
        activeMissionMap[config.missionId] = config;
    }
}

void Campaign::readActiveBoxConfig()
{
    auto doc = GameReader::getDocInstance(ACTIVE_BOX_CONFIG_FILE);
    if (activeBoxIdTable.empty()) {
        GameReader::initIdTable(*doc, "id", activeBoxIdTable);
    }
    
    activeBoxMap.clear();
    for (int i = 0; i < activeBoxIdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        
        ACTIVE_BOX_CONFIG config;
        config.itemId = DICTOOL->getIntValue_json(dic, "id");
        config.boundType = DICTOOL->getIntValue_json(dic, "bound_type");
        config.boundId = DICTOOL->getIntValue_json(dic, "bound_id");
        config.boundCount = DICTOOL->getIntValue_json(dic, "bound_count");
        config.packageId = DICTOOL->getIntValue_json(dic, "package_id");
        activeBoxMap[config.itemId] = config;
    }
}

void Campaign::readBoundMissionConfig()
{
    auto doc = GameReader::getDocInstance(BOUND_MISSION_CONFIG_FILE);
    if (boundMissionIdTable.empty()) {
        GameReader::initIdTable(*doc, "id", boundMissionIdTable);
    }
    
    boundMissionMap.clear();
    for (int i = 0; i < boundMissionIdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        
        BOUND_MISSION_CONFIG config;
        config.missionId = DICTOOL->getIntValue_json(dic, "id");
        config.open = (DICTOOL->getIntValue_json(dic, "open")==1);
        config.activeType = DICTOOL->getIntValue_json(dic, "active_type");
        config.activeThres = DICTOOL->getIntValue_json(dic, "active_thres");
        config.activeId = DICTOOL->getIntValue_json(dic, "active_id");
        config.descript = DICTOOL->getStringValue_json(dic, "descript");
        config.count = DICTOOL->getIntValue_json(dic, "count");
        config.packageId = DICTOOL->getIntValue_json(dic, "package_id");
        config.nextId = DICTOOL->getIntValue_json(dic, "next_id");
        config.icon = DICTOOL->getStringValue_json(dic, "icon");
        boundMissionMap[config.missionId] = config;
    }
}

void Campaign::readBoundPackageConfig()
{
    auto doc = GameReader::getDocInstance(BOUND_PACKAGE_CONFIG_FILE);
    if (boundPackageIdTable.empty()) {
        GameReader::initIdTable(*doc, "id", boundPackageIdTable);
    }
    
    boundPackageMap.clear();
    for (int i = 0; i < boundPackageIdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        
        ACTIVE_BOX_CONFIG config;
        config.itemId = DICTOOL->getIntValue_json(dic, "id");
        config.packageId = DICTOOL->getIntValue_json(dic, "package_id");
        config.boundType = DICTOOL->getIntValue_json(dic, "bound_type");
        config.boundId = DICTOOL->getIntValue_json(dic, "bound_id");
        config.boundCount = DICTOOL->getIntValue_json(dic, "bound_count");
        boundPackageMap[config.itemId]  = config;
    }
}

void Campaign::deleteCampaignJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "campaign.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return;
    }
    
    FileUtils::getInstance()->removeFile(path);

}

void Campaign::save2CampaignJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "campaign.json";
    
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

bool Campaign::readCampaignJson()
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += "campaign.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return false;
    }
    
    coreStr = FileUtils::getInstance()->getStringFromFile(path);
    if (!parseCampaignJson()) {
        return false;
    }
    
    return true;
}

bool Campaign::parseCampaignJson()
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
    
    coreData.monthAssignId = DICTOOL->getIntValue_json(_doc, "month_assign_id");
    coreData.monthAssignState = DICTOOL->getIntValue_json(_doc, "month_assign_state");
    coreData.everydayId = DICTOOL->getIntValue_json(_doc, "everyday_id");
    coreData.everydayState = DICTOOL->getIntValue_json(_doc, "everyday_state");
    coreData.totalActiveVal = DICTOOL->getIntValue_json(_doc, "total_active");
    coreData.boxState[0] = DICTOOL->getIntValue_json(_doc, "box_state1");
    coreData.boxState[1] = DICTOOL->getIntValue_json(_doc, "box_state2");
    coreData.boxState[2] = DICTOOL->getIntValue_json(_doc, "box_state3");
    coreData.boxState[3] = DICTOOL->getIntValue_json(_doc, "box_state4");
    coreData.freeDiamondId = DICTOOL->getIntValue_json(_doc, "free_diamond_id");
    coreData.freeDiamondState = DICTOOL->getIntValue_json(_doc, "free_diamond_state");
    coreData.onlineDuration = DICTOOL->getFloatValue_json(_doc, "online_duration");
    coreData.killMonsterCount = DICTOOL->getIntValue_json(_doc, "kill_monster_count");
    return true;
}

void Campaign::save2EmptyCampaignJson()
{
    rapidjson::Document _doc;
    if (coreStr != "") {
        return;
    }else{
        _doc.SetObject();
    }
    
    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    _doc.AddMember("month_assign_id", 1, allocator);
    _doc.AddMember("month_assign_state", 1, allocator);
    _doc.AddMember("everyday_id", 1, allocator);
    _doc.AddMember("everyday_state", 1, allocator);
    _doc.AddMember("total_active", 0, allocator);
    _doc.AddMember("box_state1", 0, allocator);
    _doc.AddMember("box_state2", 0, allocator);
    _doc.AddMember("box_state3", 0, allocator);
    _doc.AddMember("box_state4", 0, allocator);
    _doc.AddMember("free_diamond_id", 1, allocator);
    _doc.AddMember("free_diamond_state", 0, allocator);
    _doc.AddMember("online_duration", freeDiamondDuration[0], allocator);
    _doc.AddMember("kill_monster_count", 0, allocator);
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
}

void Campaign::enableEveryday()
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
    
    coreData.everydayState = 1;
    _doc["everyday_state"] = 1;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2CampaignJson();
}

void Campaign::takeEverydayBound()
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
    coreData.everydayId = DICTOOL->getIntValue_json(_doc, "everyday_id");
    int dayId = coreData.everydayId-1;
    dayId++;
    dayId %= everydayConfigIdTable.size();
    
    coreData.everydayId = dayId+1;
    coreData.everydayState = 2;
    
    _doc["everyday_id"] = coreData.everydayId;
    _doc["everyday_state"] = coreData.everydayState;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2CampaignJson();
}

void Campaign::enableMonthAssign()
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
    coreData.monthAssignId = DICTOOL->getIntValue_json(_doc, "month_assign_id");
    int dayId = coreData.monthAssignId-1;
    dayId++;
    dayId %= monthAssignConifgIdTable.size();
    
    coreData.monthAssignId = dayId+1;
    coreData.monthAssignState = 1;
    _doc["month_assign_state"] = 1;
    _doc["month_assign_id"] = coreData.monthAssignId;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2CampaignJson();
}

void Campaign::takeMonthAssignBound()
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
    
    coreData.monthAssignState = 2;
    
    _doc["month_assign_state"] = coreData.monthAssignState;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2CampaignJson();
}

void Campaign::deleteActiveJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "active_mission.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return;
    }
    
    FileUtils::getInstance()->removeFile(path);
}

void Campaign::save2ActiveJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "active_mission.json";
    
    FILE* pFile = fopen(path.c_str(),"wb");
    if (pFile)
    {
        fputs(activeStr.c_str(),pFile);
        fclose(pFile);
    }else
    {
        CCLOG("UserManage::SaveUserToFile error! can not open %s",path.c_str());
    }
}

void Campaign::save2EmptyActiveJson()
{
    rapidjson::Document _doc;
    if (activeStr != "") {
        return;
    }else{
        _doc.SetArray();
    }
    
    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    for (auto config : activeMissionMap) {
        rapidjson::Value object(rapidjson::kObjectType);
        object.AddMember("mission_id", config.second.missionId, allocator);
        object.AddMember("count", 0, allocator);
        object.AddMember("bound_state", 0, allocator);
        _doc.PushBack(object, allocator);
    }

    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    activeStr = encode(s);
    save2ActiveJson();
}

bool Campaign::readActiveJson()
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += "active_mission.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return false;
    }
    
    activeStr = FileUtils::getInstance()->getStringFromFile(path);
    if (!parseActiveJson()) {
        return false;
    }
    
    return true;
}

bool Campaign::parseActiveJson()
{
    if (activeStr == "") {
        return false;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(activeStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return false;
    }
    
    if(!_doc.IsArray())
    {
        return false;
    }

    activeDataMap.clear();
    for (int i = 0; i < _doc.Size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, i);
        
        ACTIVE_MISSION_DATA data;
        data.missionId = DICTOOL->getIntValue_json(dic, "mission_id");
        data.count = DICTOOL->getIntValue_json(dic, "count");
        data.boundState = DICTOOL->getIntValue_json(dic, "bound_state");
        activeDataMap[data.missionId] = data;
    }
    
    return true;
}

void Campaign::resetActive()
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
    coreData.totalActiveVal = 0;
    _coreDoc["total_active"] = coreData.totalActiveVal;
    for (int i = 0; i < 4; i++) {
        coreData.boxState[i] = 0;
        _coreDoc[GameUtils::format("box_state%d", i+1).c_str()] = 0;
    }
    
    StringBuffer coreBuff;
    rapidjson::Writer<StringBuffer> corewriter(coreBuff);
    _coreDoc.Accept(corewriter);
    std::string cores = StringUtils::format("%s", coreBuff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(cores);
    save2CampaignJson();
    
    if (activeStr == "") {
        return;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(activeStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    if(!_doc.IsArray())
    {
        return;
    }
    for (int i = 0; i < _doc.Size(); i++) {
        activeDataMap[i+1].count = 0;
        activeDataMap[i+1].boundState = 0;
        _doc[i]["count"] = 0;
        _doc[i]["bound_state"] = 0;
    }
    
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    activeStr = encode(s);
    save2ActiveJson();
}

void Campaign::activeMission(int activeType, int activeThres)
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
    if (activeStr == "") {
        return;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(activeStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    if(!_doc.IsArray())
    {
        return;
    }

    for (auto data : activeDataMap) {
        if (activeMissionMap[data.first].activeType != activeType) {
            continue;
        }
        
        if (activeMissionMap[data.first].activeThres > activeThres) {
            continue;
        }
        
        if (activeDataMap[data.first].boundState != 0) {
            continue;
        }
        activeDataMap[data.first].count++;
        _doc[activeMissionIdTable[data.first]]["count"] = activeDataMap[data.first].count;
        if (activeDataMap[data.first].count >= activeMissionMap[data.first].count) {
            activeDataMap[data.first].boundState = 1;
            _doc[activeMissionIdTable[data.first]]["bound_state"] = activeDataMap[data.first].boundState;
            coreData.totalActiveVal += activeMissionMap[data.first].activeVal;
            _coreDoc["total_active"] = coreData.totalActiveVal;
            for (int i = 0; i < 4; i++) {
                if (coreData.totalActiveVal >= activeBoxVal[i]) {
                    if (coreData.boxState[i] == 0) {
                        coreData.boxState[i] = 1;
                        _coreDoc[GameUtils::format("box_state%d", i+1).c_str()] = coreData.boxState[i];
                    }
                }else{
                    break;
                }
            }
        }
    }
    
    
    StringBuffer coreBuff;
    rapidjson::Writer<StringBuffer> corewriter(coreBuff);
    _coreDoc.Accept(corewriter);
    std::string cores = StringUtils::format("%s", coreBuff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(cores);
    save2CampaignJson();
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    activeStr = encode(s);
    save2ActiveJson();
}

int Campaign::takeActiveMissionBound(int missionId)
{
    if (activeStr == "") {
        return 1;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(activeStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return 1;
    }
    
    if(!_doc.IsArray())
    {
        return 1;
    }
    
    if (activeDataMap[missionId].boundState != 1) {
        return 2;
    }
    activeDataMap[missionId].boundState = 2;
    _doc[activeMissionIdTable[missionId]]["bound_state"] = activeDataMap[missionId].boundState;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    activeStr = encode(s);
    save2ActiveJson();
    return 0;
}

int Campaign::takeBoxBound(int boxId)
{
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
    
    if(!_coreDoc.IsObject())
    {
        return 1;
    }
    if (coreData.boxState[boxId-1] != 1) {
        return 2;
    }
    coreData.boxState[boxId-1] = 2;
    _coreDoc[GameUtils::format("box_state%d", boxId).c_str()] = coreData.boxState[boxId-1];
    
    StringBuffer coreBuff;
    rapidjson::Writer<StringBuffer> corewriter(coreBuff);
    _coreDoc.Accept(corewriter);
    std::string cores = StringUtils::format("%s", coreBuff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(cores);
    save2CampaignJson();
    return 0;
}

void Campaign::getPackageById(int packageId, std::vector<int> &packageIdVec)
{
    packageIdVec.clear();
    for (auto config : activeBoxMap) {
        if (config.second.packageId == packageId) {
            packageIdVec.push_back(config.first);
        }
    }
}

int Campaign::takeFreeDiamond()
{
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
    
    if(!_coreDoc.IsObject())
    {
        return 1;
    }
    
    coreData.freeDiamondId = DICTOOL->getIntValue_json(_coreDoc, "free_diamond_id");
    coreData.freeDiamondState = DICTOOL->getIntValue_json(_coreDoc, "free_diamond_state");
    if (coreData.freeDiamondState != 1) {
        return 2;
    }
    if (coreData.freeDiamondId >= 6) {
        coreData.freeDiamondState = 2;
        _coreDoc["free_diamond_state"] = coreData.freeDiamondState;
    } else {
        coreData.freeDiamondId++;
        coreData.freeDiamondState = 0;
        coreData.onlineDuration = freeDiamondDuration[coreData.freeDiamondId-1];
        _coreDoc["free_diamond_id"] = coreData.freeDiamondId;
        _coreDoc["free_diamond_state"] = coreData.freeDiamondState;
        _coreDoc["online_duration"] = coreData.onlineDuration;
    }
    
    StringBuffer coreBuff;
    rapidjson::Writer<StringBuffer> corewriter(coreBuff);
    _coreDoc.Accept(corewriter);
    std::string cores = StringUtils::format("%s", coreBuff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(cores);
    save2CampaignJson();
    return 0;
}

void Campaign::enableFreeDiamond()
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
    
    coreData.freeDiamondState = 1;
    _coreDoc["free_diamond_state"] = coreData.freeDiamondState;
    coreData.onlineDuration = 0;
    _coreDoc["online_duration"] = coreData.onlineDuration;
    
    StringBuffer coreBuff;
    rapidjson::Writer<StringBuffer> corewriter(coreBuff);
    _coreDoc.Accept(corewriter);
    std::string cores = StringUtils::format("%s", coreBuff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(cores);
    save2CampaignJson();
}

void Campaign::resetFreeDiamond()
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
    coreData.freeDiamondId = 1;
    _coreDoc["free_diamond_id"] = coreData.freeDiamondId;
    coreData.freeDiamondState = 0;
    _coreDoc["free_diamond_state"] = coreData.freeDiamondState;
    coreData.onlineDuration = freeDiamondDuration[0];
    _coreDoc["online_duration"] = coreData.onlineDuration;
    
    StringBuffer coreBuff;
    rapidjson::Writer<StringBuffer> corewriter(coreBuff);
    _coreDoc.Accept(corewriter);
    std::string cores = StringUtils::format("%s", coreBuff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(cores);
    save2CampaignJson();
}

bool Campaign::addDiamond(float dt)
{
    if (coreData.freeDiamondState == 0) {
        coreData.onlineDuration -= dt;
        if (coreData.onlineDuration <= 0) {
//            enableFreeDiamond();
            return true;
        }
    }
    
    return false;
}

void Campaign::saveDiamond()
{
    if (coreData.freeDiamondState != 0) {
        return;
    }
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
    
    _coreDoc["online_duration"] = coreData.onlineDuration;
    
    StringBuffer coreBuff;
    rapidjson::Writer<StringBuffer> corewriter(coreBuff);
    _coreDoc.Accept(corewriter);
    std::string cores = StringUtils::format("%s", coreBuff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(cores);
    save2CampaignJson();
}

void Campaign::deleteMissionJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "bound_mission.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return;
    }
    
    FileUtils::getInstance()->removeFile(path);
}

void Campaign::save2MissionJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "bound_mission.json";
    
    FILE* pFile = fopen(path.c_str(),"wb");
    if (pFile)
    {
        fputs(boundStr.c_str(),pFile);
        fclose(pFile);
    }else
    {
        CCLOG("UserManage::SaveUserToFile error! can not open %s",path.c_str());
    }
}

void Campaign::save2EmptyMissionJson()
{
    rapidjson::Document _doc;
    if (boundStr!= "") {
        return;
    }else{
        _doc.SetArray();
    }
    
    int index = 0;
    boundDocIdTable.clear();
    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    for (auto config : boundMissionMap) {
        if (config.second.open) {
            rapidjson::Value object(rapidjson::kObjectType);
            object.AddMember("mission_id", config.second.missionId, allocator);
            object.AddMember("count", 0, allocator);
            object.AddMember("bound_state", 0, allocator);
            object.AddMember("max_thres", 0, allocator);
            _doc.PushBack(object, allocator);
            boundDocIdTable[config.second.missionId] = index;
            index++;
        }
    }
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    boundStr = encode(s);
    save2MissionJson();
}

bool Campaign::readMissionJson()
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += "bound_mission.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return false;
    }
    
    boundStr = FileUtils::getInstance()->getStringFromFile(path);
    if (!parseMissionJson()) {
        return false;
    }
    
    return true;
}

bool Campaign::parseMissionJson()
{
    if (boundStr == "") {
        return false;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(boundStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return false;
    }
    
    if(!_doc.IsArray())
    {
        return false;
    }
    
    boundDataMap.clear();
    boundDocIdTable.clear();
    for (int i = 0; i < _doc.Size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, i);
        
        ACTIVE_MISSION_DATA data;
        data.missionId = DICTOOL->getIntValue_json(dic, "mission_id");
        data.count = DICTOOL->getIntValue_json(dic, "count");
        data.boundState = DICTOOL->getIntValue_json(dic, "bound_state");
        data.maxThres = DICTOOL->getIntValue_json(dic, "max_thres");
        boundDataMap[data.missionId] = data;
        boundDocIdTable[data.missionId] = i;
    }
    
    return true;
}

void Campaign::cleanBoundStr()
{
    boundStr = "";
    rapidjson::Document _doc;
    _doc.SetArray();
    
    int index = 0;
    boundDocIdTable.clear();
    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    for (auto data : boundDataMap) {
        if (data.second.boundState != 2) {
            rapidjson::Value object(rapidjson::kObjectType);
            object.AddMember("mission_id", data.second.missionId, allocator);
            object.AddMember("count", data.second.count, allocator);
            object.AddMember("bound_state", data.second.boundState, allocator);
            object.AddMember("max_thres", data.second.maxThres, allocator);
            _doc.PushBack(object, allocator);
            boundDocIdTable[data.second.missionId] = index;
            index++;
        }
    }
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    boundStr = encode(s);
    save2MissionJson();
}

void Campaign::getBoundPackageById(int packageId, std::vector<int> &packageIdVec)
{
    packageIdVec.clear();
    for (auto config : boundPackageMap) {
        if (config.second.packageId == packageId) {
            packageIdVec.push_back(config.first);
        }
    }
}

void Campaign::activeBoundMission(int activeType, int activeId, int activeThres)
{
    if (boundStr == "") {
        return;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(boundStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    if(!_doc.IsArray())
    {
        return;
    }
    
    if (activeType == 11) {
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
        
        coreData.killMonsterCount++;
        _coreDoc["kill_monster_count"] = coreData.killMonsterCount;
        
        StringBuffer coreBuff;
        rapidjson::Writer<StringBuffer> corewriter(coreBuff);
        _coreDoc.Accept(corewriter);
        std::string cores = StringUtils::format("%s", coreBuff.GetString());
        //    log("========================\n");
        //    log("%s\n", s.c_str());
        coreStr = encode(cores);
        save2CampaignJson();
    }
    
    for (auto data : boundDataMap) {
        if (boundMissionMap[data.first].activeType != activeType) {
            continue;
        }
        if (boundMissionMap[data.first].activeId != activeId) {
            continue;
        }
//        if (activeType == 8) {
//            if (boundDataMap[data.first].count < activeThres) {
//                boundDataMap[data.first].count = activeThres;
//                _doc[boundDocIdTable[data.first]]["count"] = boundDataMap[data.first].count;
//            }
//            if (boundDataMap[data.first].boundState != 0) {
//                continue;
//            }
//            if (boundMissionMap[data.first].activeThres > activeThres) {
//                continue;
//            }
//        }else{
        if (activeThres > boundDataMap[data.first].maxThres) {
            boundDataMap[data.first].maxThres = activeThres;
            _doc[boundDocIdTable[data.first]]["max_thres"] = boundDataMap[data.first].maxThres;
        }
        if (boundDataMap[data.first].boundState != 0) {
            continue;
        }
        if (boundMissionMap[data.first].activeThres > activeThres) {
            continue;
        }
        boundDataMap[data.first].count++;
        _doc[boundDocIdTable[data.first]]["count"] = boundDataMap[data.first].count;
//        }
        
        
        
        if (boundMissionMap[data.first].activeType == 11) {
            if(coreData.killMonsterCount >= boundMissionMap[data.first].count) {
                boundDataMap[data.first].boundState = 1;
                _doc[boundDocIdTable[data.first]]["bound_state"] = boundDataMap[data.first].boundState;
            }
        }else{
            if (boundDataMap[data.first].count >= boundMissionMap[data.first].count) {
                boundDataMap[data.first].boundState = 1;
                _doc[boundDocIdTable[data.first]]["bound_state"] = boundDataMap[data.first].boundState;
            }
        }
        
    }
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    boundStr = encode(s);
    save2MissionJson();
}

int Campaign::takeBoundMissionReward(int missionId)
{
    if (boundStr == "") {
        return 1;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(boundStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return 1;
    }
    
    if(!_doc.IsArray())
    {
        return 1;
    }
    
    if (boundDataMap[missionId].boundState != 1) {
        return 2;
    }
    boundDataMap[missionId].boundState = 2;
    _doc[boundDocIdTable[missionId]]["bound_state"] = boundDataMap[missionId].boundState;
    refreshBoundMission(missionId, _doc);
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    boundStr = encode(s);
    save2MissionJson();
    return 0;
}

void Campaign::refreshBoundMission(int missionId, rapidjson::Document &_doc)
{
    if (boundDataMap[missionId].boundState != 2) {
        return;
    }
    

    int nextMissionId = boundMissionMap[missionId].nextId;
    if (nextMissionId == 0) {
        return;
    }
    
    ACTIVE_MISSION_DATA data;
    data.missionId = nextMissionId;
    data.count = 0;
    data.boundState = 0;
    data.maxThres = 0;
//    if (boundMissionMap[nextMissionId].activeType == 8) {
//        data.count = boundDataMap[missionId].count;
//    }else{
    if ((boundMissionMap[nextMissionId].activeType == boundMissionMap[missionId].activeType) &&
        (boundMissionMap[nextMissionId].activeId == boundMissionMap[missionId].activeId)){
        data.maxThres = boundDataMap[missionId].maxThres;
        if (data.maxThres >= boundMissionMap[nextMissionId].activeThres) {
            data.count = 1;
            if (boundMissionMap[nextMissionId].count <= 1) {
                data.boundState = 1;
            }
        }
    }
//    }
    boundDataMap[nextMissionId] = data;
    
    boundDocIdTable[nextMissionId] = boundDocIdTable[missionId];
    _doc[boundDocIdTable[nextMissionId]]["mission_id"] = data.missionId;
    _doc[boundDocIdTable[nextMissionId]]["count"] = data.count;
    _doc[boundDocIdTable[nextMissionId]]["bound_state"] = data.boundState;
    _doc[boundDocIdTable[nextMissionId]]["max_thres"] = data.maxThres;
//    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
//    rapidjson::Value object(rapidjson::kObjectType);
//    object.AddMember("mission_id", data.missionId, allocator);
//    object.AddMember("count", 0, allocator);
//    object.AddMember("bound_state", 0, allocator);
//    _doc.PushBack(object, allocator);
}

void Campaign::currBoundMission(std::vector<int> &missionIdVec)
{
    missionIdVec.clear();
    if (boundStr == "") {
        return;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(boundStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    if(!_doc.IsArray())
    {
        return;
    }
    
    for (int i = 0; i < _doc.Size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, i);
        int missionId = DICTOOL->getIntValue_json(dic, "mission_id");
        missionIdVec.push_back(missionId);
    }
}

void Campaign::checkTip()
{
    tip = false;
    activeTip = false;
    boundTip = false;
    monthTip = false;
    for(auto data : activeDataMap) {
        if (data.second.boundState == 1) {
            tip = true;
            activeTip = true;
            break;
//            return;
        }
    }
    for (auto data : boundDataMap) {
        if (data.second.boundState == 1) {
            tip = true;
            boundTip = true;
            break;
//            return;
        }
    }
    
    if (coreData.monthAssignState == 1) {
        tip = true;
        monthTip = true;
    }
    
}
