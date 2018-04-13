//
//  Account.cpp
//  life
//
//  Created by ff on 17/7/18.
//
//

#include "Account.h"
#include "utils/GameReader.h"
#include "GameDefine.h"
#include "utils/libs.h"

USING_NS_CC;
using namespace cocostudio;
using namespace rapidjson;

std::map<int, int> Account::jobConfigIdTable;
std::map<int, int> Account::uplevelConfigIdTable;
std::map<int, int> Account::unlockConfigIdTable;

Account::Account():
coreStr(""),
accountVIT(100),
accountMAXVIT(100)
{
    unlockConfigIdTable.clear();
    jobConfigIdTable.clear();
    jobConfigMap.clear();
    uplevelConfigIdTable.clear();
    uplevelConfigMap.clear();
    unlockConfigMap.clear();
}

Account::~Account()
{
    
}

void Account::readJobConfigFile()
{
    auto doc = GameReader::getDocInstance(JOB_CONFIG_FILE);
    if (jobConfigIdTable.empty()) {
        GameReader::initIdTable(*doc, "id", jobConfigIdTable);
    }
    
    for (int i=0; i<jobConfigIdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        
        JOB_CONFIG config;
        config.jobId = DICTOOL->getIntValue_json(dic, "id");
        config.name = DICTOOL->getStringValue_json(dic, "name");
        config.headIcon = DICTOOL->getStringValue_json(dic, "head_icon");
        config.armature = DICTOOL->getStringValue_json(dic, "armature");
        config.lock = DICTOOL->getIntValue_json(dic, "lock");
        config.pvpImg = DICTOOL->getStringValue_json(dic, "pvp_img");
        config.active = DICTOOL->getIntValue_json(dic, "active");
        config.fighterId = DICTOOL->getIntValue_json(dic, "fighter_id");
        config.opponentId = DICTOOL->getIntValue_json(dic, "opponent_id");
        config.descript = DICTOOL->getStringValue_json(dic, "descript");
        config.price = DICTOOL->getIntValue_json(dic, "price");
        jobConfigMap[config.jobId] = config;
    }
    
}

void Account::readUplevelConfigFile()
{
    auto doc = GameReader::getDocInstance(UPLEVEL_CONFIG_FILE);
    if (uplevelConfigIdTable.empty()) {
        GameReader::initIdTable(*doc, "id", uplevelConfigIdTable);
    }
    
    for (int i=0; i<uplevelConfigIdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        
        UPLEVEL_CONFIG config;
        config.level = DICTOOL->getIntValue_json(dic, "id");
        config.expThres = DICTOOL->getIntValue_json(dic, "exp_thres");
        config.deltaAttack = DICTOOL->getIntValue_json(dic, "attack");
        config.deltaDefence = DICTOOL->getIntValue_json(dic, "defence");
        config.deltaCrit = DICTOOL->getIntValue_json(dic, "crit");
        config.deltaDeCrit = DICTOOL->getIntValue_json(dic, "de_crit");
        config.deltaHp = DICTOOL->getIntValue_json(dic, "hp");
        
        uplevelConfigMap[config.level] = config;
    }

}

void Account::deleteJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "account.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return;
    }
    
    FileUtils::getInstance()->removeFile(path);
}

bool Account::readJson()
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += "account.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return false;
    }
    
    coreStr = FileUtils::getInstance()->getStringFromFile(path);
    if (!parseJson()) {
        return false;
    }
    
    return true;

}
void Account::save2Json()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "account.json";
    
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

bool Account::parseJson()
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

    data.currJob = DICTOOL->getIntValue_json(_doc, "curr_job");
    data.name = DICTOOL->getStringValue_json(_doc, "name");
    data.exp = DICTOOL->getIntValue_json(_doc, "exp");
    data.level = DICTOOL->getIntValue_json(_doc, "level");
    data.hp = DICTOOL->getIntValue_json(_doc, "hp");
    data.attack = DICTOOL->getIntValue_json(_doc, "attack");
    data.defence = DICTOOL->getIntValue_json(_doc, "defence");
    data.crit = DICTOOL->getIntValue_json(_doc, "crit");
    data.deCrit = DICTOOL->getIntValue_json(_doc, "de_crit");
    data.vipExp = DICTOOL->getIntValue_json(_doc, "vip_exp");
    data.vipLevel = DICTOOL->getIntValue_json(_doc, "vip_level");
    data.gold = DICTOOL->getIntValue_json(_doc, "gold");
    data.diamond = DICTOOL->getIntValue_json(_doc, "diamond");
    data.fame = DICTOOL->getIntValue_json(_doc, "fame");
    return true;
}

void Account::save2EmptyCoreStr()
{
    rapidjson::Document _doc;
    if (coreStr != "") {
        return;
    }else{
        _doc.SetObject();
    }
    
    std::string name("骑士艾瑟");
    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    _doc.AddMember("curr_job", 1, allocator);
    _doc.AddMember("name", rapidjson::StringRef(name.c_str()), allocator);
    _doc.AddMember("exp", 0, allocator);
    _doc.AddMember("level", 1, allocator);
//    _doc.AddMember("hp", 99999999, allocator);
    _doc.AddMember("hp", 720, allocator);
    _doc.AddMember("attack", 24, allocator);
    _doc.AddMember("defence", 16, allocator);
    _doc.AddMember("crit", 0, allocator);
    _doc.AddMember("de_crit", 0, allocator);
    _doc.AddMember("vip_exp", 0, allocator);
    _doc.AddMember("vip_level", 0, allocator);
    _doc.AddMember("gold", 0, allocator);
    _doc.AddMember("diamond", 0, allocator);
    _doc.AddMember("fame", 0, allocator);
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2Json();
}

int Account::getForce()
{
    int result = 0;
    result = (float)data.hp*0.15+(float)data.attack*6.35+(float)data.crit*2.25+(float)data.defence*2.7+(float)data.deCrit*2.25;
    return result;
}

int Account::calForce(int hp, int attack, int defence, int crit, int decrit)
{
    int result = 0;
    result = (float)hp*0.15+(float)attack*6.35+(float)crit*2.25+(float)defence*2.7+(float)decrit*2.25;
    return result;
}

void Account::changeName(std::string name)
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

    data.name = name;
    _doc["name"] = rapidjson::StringRef(data.name.c_str());
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    
    save2Json();
}
void Account::changeHp(int delta)
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
    
    data.hp = DICTOOL->getIntValue_json(_doc, "hp");
    data.hp += delta;
    data.hp = MAX(0, data.hp);
    _doc["hp"] = data.hp;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    
    save2Json();
}

void Account::changeAttack(int delta)
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
    
    data.attack = DICTOOL->getIntValue_json(_doc, "attack");
    data.attack += delta;
    data.attack = MAX(1, data.attack);
    _doc["attack"] = data.attack;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);

    save2Json();
}

void Account::changeDefence(int delta)
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
    
    data.defence = DICTOOL->getIntValue_json(_doc, "defence");
    data.defence += delta;
    data.defence = MAX(0, data.defence);
    _doc["defence"] = data.defence;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);

    save2Json();
}

void Account::changeCrit(int delta)
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
    
    data.crit = DICTOOL->getIntValue_json(_doc, "crit");
    data.crit += delta;
    data.crit = MAX(0, data.crit);
    _doc["crit"] = data.crit;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);

    save2Json();
}

void Account::changeDeCrit(int delta)
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
    
    data.deCrit = DICTOOL->getIntValue_json(_doc, "de_crit");
    data.deCrit += delta;
    data.deCrit = MAX(0, data.deCrit);
    _doc["de_crit"] = data.deCrit;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);

    save2Json();
}

void Account::addExp(int val)
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
    data.exp = DICTOOL->getIntValue_json(_doc, "exp");
    data.exp += val;
    data.exp = MAX(0, data.exp);
    _doc["exp"] = data.exp;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    
    save2Json();
    
    uplevel();
}

void Account::uplevel()
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

    data.level = DICTOOL->getIntValue_json(_doc, "level");
    if (data.level >= uplevelConfigIdTable.size()) {
        return;
    }
    if (data.exp < uplevelConfigMap[data.level].expThres) {
        return;
    }
    data.attack = DICTOOL->getIntValue_json(_doc, "attack");
    data.defence = DICTOOL->getIntValue_json(_doc, "defence");
    data.crit = DICTOOL->getIntValue_json(_doc, "crit");
    data.deCrit = DICTOOL->getIntValue_json(_doc, "de_crit");
    data.hp = DICTOOL->getIntValue_json(_doc, "hp");
    while (data.exp >= uplevelConfigMap[data.level].expThres) {
        data.level++;
        data.attack += uplevelConfigMap[data.level].deltaAttack;
        data.defence += uplevelConfigMap[data.level].deltaDefence;
        data.crit += uplevelConfigMap[data.level].deltaCrit;
        data.deCrit += uplevelConfigMap[data.level].deltaDeCrit;
        data.hp += uplevelConfigMap[data.level].deltaHp;
        if (data.level >= uplevelConfigIdTable.size()) {
            break;
        }
    }
    
    _doc["level"] = data.level;
    _doc["attack"] = data.attack;
    _doc["defence"] = data.defence;
    _doc["crit"] = data.crit;
    _doc["de_crit"] = data.deCrit;
    _doc["hp"] = data.hp;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    
    save2Json();
}

void Account::clearVipExp()
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

    data.vipExp = 0;
    _doc["vip_exp"] = 0;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    
    save2Json();
}

void Account::addVipExp(int val)
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

    data.vipLevel = DICTOOL->getIntValue_json(_doc, "vip_level");
    if (data.vipLevel <= 0) {
        return;
    }
    if (data.vipLevel >= maxVipLevel) {
        return;
    }
    data.vipExp = DICTOOL->getIntValue_json(_doc, "vip_exp");
    data.vipExp += val;
    data.vipExp = MAX(0, data.vipExp);
    if (data.vipExp >= vipExpThres[data.vipLevel-1]) {
        data.vipLevel++;
        data.vipExp = 0;
    }
    _doc["vip_level"] = data.vipLevel;
    _doc["vip_exp"] = data.vipExp;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    
    save2Json();
}

void Account::upVipLevel(int val)
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
    
    data.vipLevel = DICTOOL->getIntValue_json(_doc, "vip_level");
    data.vipExp = DICTOOL->getIntValue_json(_doc, "vip_exp");
    
    if (data.vipLevel >= maxVipLevel) {
        return;
    }
    
    data.vipLevel += val;
    data.vipExp = 0;
    _doc["vip_level"] = data.vipLevel;
    _doc["vip_exp"] = data.vipExp;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);

    save2Json();
}

void Account::changeGold(int delta)
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
    
    data.gold = DICTOOL->getIntValue_json(_doc, "gold");
    data.gold += delta;
    data.gold = MAX(0, data.gold);
    _doc["gold"] = data.gold;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    
    save2Json();
}

void Account::changeDiamond(int delta)
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
    
    data.diamond = DICTOOL->getIntValue_json(_doc, "diamond");
    data.diamond += delta;
    data.diamond = MAX(0, data.diamond);
    _doc["diamond"] = data.diamond;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    
    save2Json();
}

void Account::changeFame(int delta)
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
    
    data.fame = DICTOOL->getIntValue_json(_doc, "fame");
    data.fame += delta;
    data.fame = MAX(0, data.fame);
    _doc["fame"] = data.fame;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    
    save2Json();
}

void Account::readUnlockConfigFile()
{
    auto doc = GameReader::getDocInstance(UNLOCK_CONFIG_FILE);
    if (unlockConfigIdTable.empty()) {
        GameReader::initIdTable(*doc, "id", unlockConfigIdTable);
    }
    
    for (int i=0; i<unlockConfigIdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        
        UNLOCK_CONFIG config;
        config.unlockId = DICTOOL->getIntValue_json(dic, "id");
        config.stageId = DICTOOL->getIntValue_json(dic, "stage_id");
        config.lockType = DICTOOL->getIntValue_json(dic, "lock_type");
        config.icon = DICTOOL->getStringValue_json(dic, "icon");
        unlockConfigMap[config.unlockId] = config;
    }
}

void Account::deleteUnlockJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "unlock.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return;
    }
    
    FileUtils::getInstance()->removeFile(path);
}

bool Account::readUnlockJson()
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += "unlock.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return false;
    }
    
    unlockStr = FileUtils::getInstance()->getStringFromFile(path);
    if (!parseUnlockJson()) {
        return false;
    }
    
    return true;
}

bool Account::parseUnlockJson()
{
    if (unlockStr == "") {
        return false;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(unlockStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return false;
    }
    
    if(!_doc.IsArray())
    {
        return false;
    }
    
    unlockMap.clear();
    int count = _doc.Size();
    for (int i = 0; i < count; i++) {
        UNLOCK_DATA data;
        data.unlockId = DICTOOL->getIntValue_json(_doc[i], "unlock_id");
        data.lock = DICTOOL->getIntValue_json(_doc[i], "lock");
        unlockMap[data.unlockId] = data;
    }
    
    return true;
}

void Account::save2EmptyUnlockStr()
{
    rapidjson::Document _doc;
    if (unlockStr != "") {
        return;
    }else{
        _doc.SetArray();
    }
    
    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    int unlockCount = (int)unlockConfigMap.size();
    for (int i = 0; i < unlockCount; i++) {
        rapidjson::Value object(rapidjson::kObjectType);
        object.AddMember("unlock_id", i+1, allocator);
        object.AddMember("lock", 1, allocator);
        _doc.PushBack(object, allocator);
    }
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    unlockStr = encode(s);
}

void Account::save2UnlockJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "unlock.json";
    
    FILE* pFile = fopen(path.c_str(),"wb");
    if (pFile)
    {
        fputs(unlockStr.c_str(),pFile);
        fclose(pFile);
    }else
    {
        CCLOG("UserManage::SaveUserToFile error! can not open %s",path.c_str());
    }
}

void Account::unlock(int unlockId)
{
    if (unlockStr == "") {
        return;
    }
    
    if (unlockMap.find(unlockId) == unlockMap.end()) {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(unlockStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    if(!_doc.IsArray())
    {
        return;
    }

    _doc[unlockId-1]["lock"] = 0;
    unlockMap[unlockId].lock = 0;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    unlockStr = encode(s);
    
    save2UnlockJson();
}

void Account::openLock(int unlockId)
{
    if (unlockStr == "") {
        return;
    }
    
    if (unlockMap.find(unlockId) == unlockMap.end()) {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(unlockStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    if(!_doc.IsArray())
    {
        return;
    }
    
    _doc[unlockId-1]["lock"] = 2;
    unlockMap[unlockId].lock = 2;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    unlockStr = encode(s);
    
    save2UnlockJson();
}

void Account::deleteJobJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "job.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return;
    }
    
    FileUtils::getInstance()->removeFile(path);
}

void Account::save2JobJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "job.json";
    
    FILE* pFile = fopen(path.c_str(),"wb");
    if (pFile)
    {
        fputs(jobStr.c_str(),pFile);
        fclose(pFile);
    }else
    {
        CCLOG("UserManage::SaveUserToFile error! can not open %s",path.c_str());
    }
}

bool Account::readJobJson()
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += "job.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return false;
    }
    
    jobStr = FileUtils::getInstance()->getStringFromFile(path);
    if (!parseJobJson()) {
        return false;
    }
    
    return true;
}

bool Account::parseJobJson()
{
    if (jobStr == "") {
        return false;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(jobStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return false;
    }
    
    if(!_doc.IsArray())
    {
        return false;
    }
    
    jobMap.clear();
    int count = _doc.Size();
    for (int i = 0; i < count; i++) {
        JOB_DATA data;
        data.jobId = DICTOOL->getIntValue_json(_doc[i], "job_id");
        data.lock = DICTOOL->getIntValue_json(_doc[i], "lock");
        data.active = DICTOOL->getIntValue_json(_doc[i], "active");
        jobMap[data.jobId] = data;
    }
    
    return true;
}

void Account::save2EmptyJobStr()
{
    rapidjson::Document _doc;
    if (jobStr != "") {
        return;
    }else{
        _doc.SetArray();
    }
    
    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    int jobCount = (int)jobConfigMap.size();
    for (int i = 0; i < jobCount; i++) {
        rapidjson::Value object(rapidjson::kObjectType);
        object.AddMember("job_id", jobConfigMap[i+1].jobId, allocator);
        object.AddMember("lock", jobConfigMap[i+1].lock, allocator);
        object.AddMember("active", jobConfigMap[i+1].active, allocator);
        _doc.PushBack(object, allocator);
    }
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    jobStr = encode(s);
}

void Account::unlockJob(int jobId)
{
    if (jobStr == "") {
        return;
    }
    
    if (jobMap.find(jobId) == jobMap.end()) {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(jobStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    if(!_doc.IsArray())
    {
        return;
    }
    
    _doc[jobId-1]["lock"] = 0;
    jobMap[jobId].lock = 0;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    jobStr = encode(s);
    
    save2JobJson();
}

void Account::activeJob(int jobId)
{
    if (jobStr == "") {
        return;
    }
    
    if (jobMap.find(jobId) == jobMap.end()) {
        return;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(jobStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    if(!_doc.IsArray())
    {
        return;
    }
    
    _doc[jobId-1]["active"] = 1;
    jobMap[jobId].active = 1;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    jobStr = encode(s);
    
    save2JobJson();
}

void Account::activeAllJob()
{
    if (jobStr == "") {
        return;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(jobStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    if(!_doc.IsArray())
    {
        return;
    }
    
    for (auto job : jobMap) {
        int jobId = job.second.jobId;
        _doc[jobId-1]["active"] = 1;
        jobMap[jobId].active = 1;
    }
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    jobStr = encode(s);
    
    save2JobJson();
}

int Account::changeJob(int jobId)
{
    if (jobStr == "") {
        return 1;
    }
    
    if (jobMap.find(jobId) == jobMap.end()) {
        return 2;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(jobStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return 1;
    }
    
    if(!_doc.IsArray())
    {
        return 1;
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
    
    if (jobMap[jobId].lock == 1) {
        return 3;
    }
    
    if (jobMap[jobId].active == 0) {
        return 4;
    }
    
    data.currJob = jobId;
    _coreDoc["curr_job"] = data.currJob;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _coreDoc.Accept(writer);
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    
    save2Json();
    return 0;
}
