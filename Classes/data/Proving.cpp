//
//  Proving.cpp
//  life
//
//  Created by ff on 17/8/11.
//
//

#include "Proving.h"
#include "utils/GameReader.h"
#include "GameDefine.h"
#include "utils/libs.h"

USING_NS_CC;
using namespace cocostudio;
using namespace rapidjson;

std::map<int, int> Proving::provConfigIdTable;

Proving::Proving():
coreStr(""),
fighterStr(""),
fighterIndex(-1)
{
    provConfigIdTable.clear();
    provConfigMap.clear();
    fighterData.clear();
}

Proving::~Proving()
{
    
}

void Proving::readProvConfigFile()
{
    auto doc = GameReader::getDocInstance(PORVING_CONFIG_FILE);
    if (provConfigIdTable.empty()) {
        GameReader::initIdTable(*doc, "id", provConfigIdTable);
    }
    
    provConfigMap.clear();
    for (int i = 0; i < provConfigIdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        
        PROVING_CONFIG config;
        config.provingId = DICTOOL->getIntValue_json(dic, "id");
        config.stageIcon = DICTOOL->getStringValue_json(dic, "stage_icon");
        config.petIcon = DICTOOL->getStringValue_json(dic, "pet_icon");
        config.hp = DICTOOL->getIntValue_json(dic, "hp");
        config.level = DICTOOL->getIntValue_json(dic, "level");
        config.dropGold = DICTOOL->getIntValue_json(dic, "drop_gold");
        config.dropStone = DICTOOL->getIntValue_json(dic, "drop_stone");
        provConfigMap[config.provingId] = config;
    }
    return;
}

void Proving::deleteCoreJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "prov_info.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return;
    }
    
    FileUtils::getInstance()->removeFile(path);
}

void Proving::save2CoreJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "prov_info.json";
    
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

bool Proving::readCoreJson()
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += "prov_info.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return false;
    }
    
    coreStr = FileUtils::getInstance()->getStringFromFile(path);
    if (!parseCoreJson()) {
        return false;
    }
    
    return true;
}

bool Proving::parseCoreJson()
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

    
    provData.provingStage = DICTOOL->getIntValue_json(_doc, "proving_stage");
    provData.maxResetCount = DICTOOL->getIntValue_json(_doc, "max_reset_count");
    provData.resetCount = DICTOOL->getIntValue_json(_doc, "reset_count");
    provData.sweepTicketCount = DICTOOL->getIntValue_json(_doc, "sweep_ticket_count");
    provData.petAttack = DICTOOL->getIntValue_json(_doc, "pet_attack");
//    provData.petId = DICTOOL->getIntValue_json(_doc, "pet_id");
    provData.petHp = DICTOOL->getIntValue_json(_doc, "pet_hp");
    provData.petMaxHp = DICTOOL->getIntValue_json(_doc, "pet_max_hp");
    provData.attackDirect = DICTOOL->getIntValue_json(_doc, "attack_direct");
    
    return true;
}

void Proving::save2EmptyCoreStr()
{
    rapidjson::Document _doc;
    if (coreStr != "") {
        return;
    }else{
        _doc.SetObject();
    }
    
    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    _doc.AddMember("proving_stage", 1, allocator);
    _doc.AddMember("max_reset_count", 1, allocator);
    _doc.AddMember("reset_count", 0, allocator);
    _doc.AddMember("sweep_ticket_count", 1, allocator);
    _doc.AddMember("pet_hp", provConfigMap[1].hp, allocator);
    _doc.AddMember("pet_max_hp", provConfigMap[1].hp, allocator);
    _doc.AddMember("pet_attack", provConfigMap[1].hp/12, allocator);
    //    _doc.AddMember("pet_id", 0, allocator);
    _doc.AddMember("attack_direct", 0, allocator);
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
}

void Proving::deleteFighterJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "prov_fighter.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return;
    }
    
    FileUtils::getInstance()->removeFile(path);
}

void Proving::save2FighterJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "prov_fighter.json";
    
    FILE* pFile = fopen(path.c_str(),"wb");
    if (pFile)
    {
        fputs(fighterStr.c_str(),pFile);
        fclose(pFile);
    }else
    {
        CCLOG("UserManage::SaveUserToFile error! can not open %s",path.c_str());
    }
}

bool Proving::readFighterJson()
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += "prov_fighter.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return false;
    }
    
    fighterStr = FileUtils::getInstance()->getStringFromFile(path);
    if (!parseFighterJson()) {
        return false;
    }
    
    return true;
}

bool Proving::parseFighterJson()
{
    if (fighterStr == "") {
        return false;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(fighterStr.c_str());
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
        PROVING_FIGHTER_DATA data;
        data.petId = DICTOOL->getIntValue_json(dic, "pet_id");
        data.petHp = DICTOOL->getIntValue_json(dic, "pet_hp");
        data.petLevel = DICTOOL->getIntValue_json(dic, "pet_level");
        data.maxHp = DICTOOL->getIntValue_json(dic, "max_hp");
        data.petAttack = DICTOOL->getIntValue_json(dic, "pet_attack");
        data.critRate = DICTOOL->getIntValue_json(dic, "crit_rate");
        data.critPercent = DICTOOL->getIntValue_json(dic, "crit_percent");
        fighterData[data.petId] = data;
    }
    
    return true;
}

void Proving::save2EmptyFighterStr()
{
    rapidjson::Document _doc;
    if (fighterStr != "") {
        return;
    }else{
        _doc.SetArray();
    }
    
    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    rapidjson::Value object(rapidjson::kObjectType);
    object.AddMember("pet_id", 1, allocator);
    object.AddMember("pet_hp", 0, allocator);
    object.AddMember("pet_level", 0, allocator);
    object.AddMember("pet_attack", 1, allocator);
    object.AddMember("max_hp", 1, allocator);
    object.AddMember("crit_rate", 0, allocator);
    object.AddMember("crit_percent", 0, allocator);
    _doc.PushBack(object, allocator);
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    fighterStr = encode(s);
}

int Proving::refreshProv()
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

    provData.maxResetCount = DICTOOL->getIntValue_json(_doc, "max_reset_count");
    provData.resetCount = provData.maxResetCount;
    
    _doc["reset_count"] = provData.resetCount;
    
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

int Proving::resetProv()
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
    if (fighterStr == "") {
        return 1;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(fighterStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return 1;
    }
    
    if(!_doc.IsArray())
    {
        return 1;
    }
    
    provData.resetCount = DICTOOL->getIntValue_json(_coreDoc, "reset_count");
    if (provData.resetCount <= 0) {
        return 2;
    }
    provData.resetCount--;
    
    provData.provingStage = provConfigMap[1].provingId;
    provData.petHp = provConfigMap[provData.provingStage].hp;
    provData.petMaxHp = provConfigMap[provData.provingStage].hp;
    provData.petAttack = provData.petHp/3;
    provData.attackDirect = 0;
    
    _coreDoc["reset_count"] = provData.resetCount;
    _coreDoc["proving_stage"] = provData.provingStage;
    _coreDoc["pet_hp"] = provData.petHp;
    _coreDoc["pet_max_hp"] = provData.petMaxHp;
    _coreDoc["pet_attack"] = provData.petAttack;
    _coreDoc["attack_direct"] = provData.attackDirect;
    
    StringBuffer coreBuff;
    rapidjson::Writer<StringBuffer> coreWriter(coreBuff);
    _coreDoc.Accept(coreWriter);
    std::string cores = StringUtils::format("%s", coreBuff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(cores);
    
    fighterIndex = -1;
    for (int i = 0; i < _doc.Size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, i);
        int petId = DICTOOL->getIntValue_json(dic, "pet_id");
        int level = DICTOOL->getIntValue_json(dic, "pet_level");
        if (level < 1) {
            continue;
        }
        
        fighterData[petId].petHp = fighterData[petId].maxHp;
        _doc[i]["pet_hp"] = fighterData[petId].petHp;
        if (fighterIndex < 0) {
            fighterIndex = i;
        }
    }
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    fighterStr = encode(s);
    save2FighterJson();
    return 0;
}

void Proving::addMaxResetCount(int val)
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
    
    provData.maxResetCount = DICTOOL->getIntValue_json(_doc, "max_reset_count");
    provData.maxResetCount += val;
    provData.maxResetCount = MAX(1, provData.maxResetCount);
    provData.resetCount = DICTOOL->getIntValue_json(_doc, "reset_count");
    provData.resetCount += val;
    provData.maxResetCount = MAX(0, provData.resetCount);
    
    _doc["reset_count"] = provData.resetCount;
    _doc["max_reset_count"] = provData.maxResetCount;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2CoreJson();
}

void Proving::addTicket(int val)
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
    
    provData.sweepTicketCount = DICTOOL->getIntValue_json(_doc, "sweep_ticket_count");
    provData.sweepTicketCount += val;
    provData.sweepTicketCount = MAX(0, provData.sweepTicketCount);
    
    _doc["sweep_ticket_count"] = provData.sweepTicketCount;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2CoreJson();
}

void Proving::pushPet(int petId, int level, int maxHp, int critRate, int critPercent)
{
    if (fighterStr == "") {
        return;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(fighterStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    if(!_doc.IsArray())
    {
        return;
    }

    
    if (fighterData.find(petId) == fighterData.end()) {
        //如果是新加入的，所有数据都是新的
        PROVING_FIGHTER_DATA data;
        data.petId = petId;
        data.petLevel = level;
        data.maxHp = maxHp;
        data.petAttack = maxHp/3;
        data.critRate = critRate;
        data.critPercent = critPercent;
        data.petHp = maxHp;
        fighterData[data.petId] = data;
        
        rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
        rapidjson::Value object(rapidjson::kObjectType);
        object.AddMember("pet_id", data.petId, allocator);
        object.AddMember("pet_hp", data.petHp, allocator);
        object.AddMember("pet_level", data.petLevel, allocator);
        object.AddMember("max_hp", data.maxHp, allocator);
        object.AddMember("pet_attack", data.petAttack, allocator);
        object.AddMember("crit_rate", data.critRate, allocator);
        object.AddMember("crit_percent", data.critPercent, allocator);
        _doc.PushBack(object, allocator);
    }else{
        if (fighterData[petId].petLevel != level) {
            if (fighterData[petId].petLevel == 0) {
                fighterData[petId].petHp = maxHp;
            }
            //如果等级变化，除了hp不增加，其他都随等级变化
            fighterData[petId].petId = petId;
            fighterData[petId].petLevel = level;
            fighterData[petId].maxHp = maxHp;
            fighterData[petId].petAttack = maxHp/3;
            fighterData[petId].critRate = critRate;
            fighterData[petId].critPercent = critPercent;
            
            for (int index = 0; index < _doc.Size(); index++) {
                const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, index);
                int docPetId = DICTOOL->getIntValue_json(dic, "pet_id");
                if (petId == docPetId) {
                    _doc[index]["pet_id"] = petId;
                    _doc[index]["pet_level"] = fighterData[petId].petLevel;
                    _doc[index]["pet_hp"] = fighterData[petId].petHp;
                    _doc[index]["max_hp"] = fighterData[petId].maxHp;
                    _doc[index]["pet_attack"] = fighterData[petId].petAttack;
                    _doc[index]["crit_rate"] = fighterData[petId].critRate;
                    _doc[index]["crit_percent"] = fighterData[petId].critPercent;
                }
            }
            
        }
    }
    
    //重新寻找第一个出战的挑战者
    fighterIndex = -1;
    for (int i = 0; i < _doc.Size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, i);
        int petId = DICTOOL->getIntValue_json(dic, "pet_id");
        int level = DICTOOL->getIntValue_json(dic, "pet_level");
        if (level < 1) {
            continue;
        }
        fighterData[petId].petHp = DICTOOL->getIntValue_json(dic, "pet_hp");
        if (fighterData[petId].petHp <= 0) {
            continue;
        }
        if (fighterIndex < 0) {
            fighterIndex = i;
            break;
        }
    }
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    fighterStr = encode(s);
    save2FighterJson();
}
int Proving::confirmStage()
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
    provData.provingStage = DICTOOL->getIntValue_json(_doc, "proving_stage");
    provData.petHp = DICTOOL->getIntValue_json(_doc, "pet_hp");
    //镇守方已死
    if (provData.petHp <= 0) {
        //通过所有试炼
        if (provData.provingStage >= provConfigIdTable.size()) {
            return 2;
        }
        //开启下一试炼，设定挑战方先攻击
        provData.provingStage++;
        provData.petHp = provConfigMap[provData.provingStage].hp;
        provData.petMaxHp = provConfigMap[provData.provingStage].hp;
        provData.petAttack = provData.petHp/3;
        provData.attackDirect = 0;
        
        _doc["proving_stage"] = provData.provingStage;
        _doc["pet_hp"] = provData.petHp;
        _doc["pet_max_hp"] = provData.petMaxHp;
        _doc["pet_attack"] = provData.petAttack;
        _doc["attack_direct"] = provData.attackDirect;
        
        StringBuffer buff;
        rapidjson::Writer<StringBuffer> writer(buff);
        _doc.Accept(writer);
        std::string s = StringUtils::format("%s", buff.GetString());
        //    log("========================\n");
        //    log("%s\n", s.c_str());
        coreStr = encode(s);
        save2CoreJson();
    }
    
    
    return 0;
}
int Proving::confirmFighter()
{
    if (fighterStr == "") {
        return 1;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(fighterStr.c_str());
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
    
    if(!_coreDoc.IsObject())
    {
        return 1;
    }

    if (fighterIndex == -1) {
        return 2;
    }
    provData.sweepTicketCount = DICTOOL->getIntValue_json(_coreDoc, "sweep_ticket_count");
    if (provData.sweepTicketCount <= 0) {
        return 3;
    }
    const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, fighterIndex);
    int fightIdx = DICTOOL->getIntValue_json(dic, "pet_id");
    
    //如果当前挑战方已败，换下一个队友
    if(fighterData[fightIdx].petHp <= 0)
    {
        //如果所有队友都败
        if (fighterIndex + 1 >= _doc.Size()) {
            fighterIndex = -1;
            return 2;
        }
        //找到下一个可完成挑战的队友
        int nextFighter = fighterIndex;
        for (int i = fighterIndex + 1; i < _doc.Size(); i++) {
            const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, i);
            int petId = DICTOOL->getIntValue_json(dic, "pet_id");
            int level = DICTOOL->getIntValue_json(dic, "pet_level");
            if (level < 1) {
                continue;
            }
            fighterData[petId].petHp = DICTOOL->getIntValue_json(dic, "pet_hp");
            if (fighterData[petId].petHp <= 0) {
                continue;
            }

            nextFighter = i;
            break;
        }
        //无队友可用
        if (nextFighter == fighterIndex) {
            fighterIndex = -1;
            return 2;
        }
        fighterIndex = nextFighter;
        
        provData.attackDirect = 0;
        _coreDoc["attack_direct"] = provData.attackDirect;
        


    }
    provData.sweepTicketCount = DICTOOL->getIntValue_json(_coreDoc, "sweep_ticket_count");
    provData.sweepTicketCount--;
    provData.sweepTicketCount = MAX(0, provData.sweepTicketCount);
    
    _coreDoc["sweep_ticket_count"] = provData.sweepTicketCount;
    
    StringBuffer coreBuff;
    rapidjson::Writer<StringBuffer> coreWriter(coreBuff);
    _coreDoc.Accept(coreWriter);
    std::string cores = StringUtils::format("%s", coreBuff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(cores);
    save2CoreJson();
    
    return 0;
}
int Proving::getFighterId()
{
    if (fighterStr == "") {
        return -1;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(fighterStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return -1;
    }
    
    if(!_doc.IsArray())
    {
        return -1;
    }
    
    if (fighterIndex < 0 || fighterIndex >= _doc.Size()) {
        return -1;
    }
    
    const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, fighterIndex);
    int fightIdx = DICTOOL->getIntValue_json(dic, "pet_id");
    
    return fightIdx;
}

int Proving::fight()
{
    if (fighterStr == "") {
        return 1;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(fighterStr.c_str());
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
    
    if(!_coreDoc.IsObject())
    {
        return 1;
    }

    
    provData.attackDirect = DICTOOL->getIntValue_json(_coreDoc, "attack_direct");
    if (provData.attackDirect == 0) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, fighterIndex);
        int attackPetId = DICTOOL->getIntValue_json(dic, "pet_id");
        int attack = fighterData[attackPetId].petAttack;
        if(abs(rand())%100 < fighterData[attackPetId].critRate) {
            attack += attack*fighterData[attackPetId].critPercent/100;
        }
        provData.petHp = DICTOOL->getIntValue_json(_coreDoc, "pet_hp");
        provData.petHp -= attack;
        provData.petHp = MAX(0, provData.petHp);
        provData.attackDirect = 1;
        
        _coreDoc["pet_hp"] = provData.petHp;
    }else{
        int attack = provData.petAttack;
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, fighterIndex);
        int defencePetId = DICTOOL->getIntValue_json(dic, "pet_id");
        fighterData[defencePetId].petHp = DICTOOL->getIntValue_json(dic, "pet_hp");
        fighterData[defencePetId].petHp -= attack;
        fighterData[defencePetId].petHp = MAX(0, fighterData[defencePetId].petHp);
        provData.attackDirect = 0;
        
        _doc[fighterIndex]["pet_hp"] = fighterData[defencePetId].petHp;
    }
    _coreDoc["attack_direct"] = provData.attackDirect;
    
    
    StringBuffer coreBuff;
    rapidjson::Writer<StringBuffer> coreWriter(coreBuff);
    _coreDoc.Accept(coreWriter);
    std::string cores = StringUtils::format("%s", coreBuff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(cores);
    save2CoreJson();

    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    fighterStr = encode(s);
    save2FighterJson();
    return 0;
}
