//
//  Pet.cpp
//  life
//
//  Created by ff on 17/7/28.
//
//

#include "Pet.h"
#include "utils/GameReader.h"
#include "GameDefine.h"
#include "utils/libs.h"

USING_NS_CC;
using namespace cocostudio;
using namespace rapidjson;

std::map<int, int> Pet::petConfigIdTable;
std::map<int, int> Pet::levelConfigIdTable;

Pet::Pet():
coreStr(""),
petsStr(""),
tip(false)
{
    petsMap.clear();
    levelConfigMap.clear();
    maxLevel.clear();
}

Pet::~Pet()
{
    
}

void Pet::readPetConfigFile()
{
    auto doc = GameReader::getDocInstance(PET_CONFIG_FILE);
    if (petConfigIdTable.empty()) {
        GameReader::initIdTable(*doc, "id", petConfigIdTable);
    }
    
    petConfigMap.clear();
    for (int i = 0; i < petConfigIdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        
        PET_CONFIG config;
        config.petId = DICTOOL->getIntValue_json(dic, "id");
        config.name = DICTOOL->getStringValue_json(dic, "name");
        config.icon = DICTOOL->getStringValue_json(dic, "icon");
        config.image = DICTOOL->getStringValue_json(dic, "image");
        config.pvpImg = DICTOOL->getStringValue_json(dic, "pvp_img");
        config.skillName = DICTOOL->getStringValue_json(dic, "skill_name");
        config.fighterId = DICTOOL->getIntValue_json(dic, "fighter_id");
        config.skillId = DICTOOL->getIntValue_json(dic, "skill_id");
        for (int i = 0; i < 2; i++) {
            config.relation[i] = DICTOOL->getIntValue_json(dic, GameUtils::format("relation%d", i+1).c_str());
        }
        config.quality = DICTOOL->getIntValue_json(dic, "quality");
        petConfigMap[config.petId] = config;
    }
}

void Pet::readLevelConfigFile()
{
    auto doc = GameReader::getDocInstance(PET_LEVEL_CONFIG_FILE);
    if (levelConfigIdTable.empty()) {
        GameReader::initIdTable(*doc, "id", levelConfigIdTable);
    }
    
    levelConfigMap.clear();
    maxLevel.clear();
    for (int i = 0; i < levelConfigIdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        
        PET_LEVEL_CONFIG config;
        config.levelId = DICTOOL->getIntValue_json(dic, "id");
        config.qualityId = DICTOOL->getIntValue_json(dic, "quality_id");
        config.level = DICTOOL->getIntValue_json(dic, "level");
        config.costFragment = DICTOOL->getIntValue_json(dic, "cost_fragment");
        config.deltaHp = DICTOOL->getIntValue_json(dic, "hp");
        config.deltaAttack = DICTOOL->getIntValue_json(dic, "attack");
        config.deltaDefence = DICTOOL->getIntValue_json(dic, "defence");
        config.provHp = DICTOOL->getIntValue_json(dic, "prov_hp");
        config.provCritRate = DICTOOL->getIntValue_json(dic, "prov_crit_rate");
        config.provCirtPercent = DICTOOL->getIntValue_json(dic, "prov_crit_percent");
        levelConfigMap[config.levelId] = config;
        if (maxLevel.find(config.qualityId) != maxLevel.end()) {
            if (maxLevel[config.qualityId] < config.level) {
                maxLevel[config.qualityId] = config.level;
            }
        }else{
            maxLevel[config.qualityId] = config.level;
        }
    }
}

void Pet::deleteJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "pet_info.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return;
    }
    
    FileUtils::getInstance()->removeFile(path);
}

void Pet::save2Json()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "pet_info.json";
    
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

bool Pet::readJson()
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += "pet_info.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return false;
    }
    
    coreStr = FileUtils::getInstance()->getStringFromFile(path);
    if (!parseJson()) {
        return false;
    }
    
    return true;
}

bool Pet::parseJson()
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
    info.fightPet[0] = DICTOOL->getIntValue_json(_doc, "fight_pet_1");
    info.fightPet[1] = DICTOOL->getIntValue_json(_doc, "fight_pet_2");
    info.fightPet[2] = DICTOOL->getIntValue_json(_doc, "fight_pet_3");
    return true;
}

void Pet::save2EmptyCoreStr()
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
    _doc.AddMember("fight_pet_1", 0, allocator);
    _doc.AddMember("fight_pet_2", 0, allocator);
    _doc.AddMember("fight_pet_3", 0, allocator);
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);

}

int Pet::getFightPetCount()
{
    int count = 0;
    
    for (int i = 0; i < 3; i++) {
        if (info.fightPet[i] > 0) {
            count++;
        }
    }
    return count;
}

int Pet::addFightPet(int petId)
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
    
    info.fightPet[0] = DICTOOL->getIntValue_json(_doc, "fight_pet_1");
    info.fightPet[1] = DICTOOL->getIntValue_json(_doc, "fight_pet_2");
    info.fightPet[2] = DICTOOL->getIntValue_json(_doc, "fight_pet_3");
    int validCount = getFightPetCount();
    if (validCount >= 3) {
        return 2;
    }
    if (petsMap[petId].level == 0) {
        return 3;
    }
    info.fightPet[validCount] = petId;
    
    _doc["fight_pet_1"] = info.fightPet[0];
    _doc["fight_pet_2"] = info.fightPet[1];
    _doc["fight_pet_3"] = info.fightPet[2];
    
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

bool Pet::removeFightPet(int pos)
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
    
    info.fightPet[0] = DICTOOL->getIntValue_json(_doc, "fight_pet_1");
    info.fightPet[1] = DICTOOL->getIntValue_json(_doc, "fight_pet_2");
    info.fightPet[2] = DICTOOL->getIntValue_json(_doc, "fight_pet_3");
    for (int i = pos; i < 3; i++) {
        info.fightPet[i-1] = info.fightPet[i];
    }
    info.fightPet[2] = 0;
    
    _doc["fight_pet_1"] = info.fightPet[0];
    _doc["fight_pet_2"] = info.fightPet[1];
    _doc["fight_pet_3"] = info.fightPet[2];

    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    
    
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s);
    save2Json();
    return true;
}

void Pet::deletePetJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "pets.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return;
    }
    
    FileUtils::getInstance()->removeFile(path);
}

void Pet::save2PetJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "pets.json";
    
    FILE* pFile = fopen(path.c_str(),"wb");
    if (pFile)
    {
        fputs(petsStr.c_str(),pFile);
        fclose(pFile);
    }else
    {
        CCLOG("UserManage::SaveUserToFile error! can not open %s",path.c_str());
    }
}

bool Pet::readPetJson()
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += "pets.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return false;
    }
    
    petsStr = FileUtils::getInstance()->getStringFromFile(path);
    if (!parsePetJson()) {
        return false;
    }
    
    return true;
}

bool Pet::parsePetJson()
{
    if (petsStr == "") {
        return false;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(petsStr.c_str());
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
        PET_DATA data;
        data.petId = DICTOOL->getIntValue_json(dic, "pet_id");
        data.level = DICTOOL->getIntValue_json(dic, "level");
        data.fragmentCount = DICTOOL->getIntValue_json(dic, "fragment_count");
        petsMap[data.petId] = data;
    }

    return true;
}

void Pet::save2EmptyPetStr()
{
    rapidjson::Document _doc;
    if (petsStr != "") {
        return;
    }else{
        _doc.SetArray();
    }
    
    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    for (auto configId : petConfigIdTable) {
        rapidjson::Value object(rapidjson::kObjectType);
        object.AddMember("pet_id", configId.first, allocator);
        object.AddMember("level", 0, allocator);
        object.AddMember("fragment_count", ((configId.first==1)?10:0), allocator);
        _doc.PushBack(object, allocator);
    }
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    petsStr = encode(s);
}

bool Pet::isMaxLevel(int petId)
{
    if (petsMap[petId].level >= maxLevel[petConfigMap[petId].quality]) {
        return true;
    }
    
    return false;
}

int Pet::uplevel(int petId)
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
    
    if (petsStr == "") {
        return 1;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(petsStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return 1;
    }
    
    if(!_doc.IsArray())
    {
        return 1;
    }
    
    int index = petConfigIdTable[petId];
    const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, index);
    
    petsMap[petId].level = DICTOOL->getIntValue_json(dic, "level");
    petsMap[petId].fragmentCount = DICTOOL->getIntValue_json(dic, "fragment_count");
    
    if (isMaxLevel(petId)) {
        return 2;
    }
    
    int nextLevelId = checkFragment(petId);
    if (nextLevelId == 0) {
        return 3;
    }
    
    petsMap[petId].fragmentCount -= levelConfigMap[nextLevelId].costFragment;
    petsMap[petId].level++;
    _doc[petConfigIdTable[petId]]["level"] = petsMap[petId].level;
    _doc[petConfigIdTable[petId]]["fragment_count"] = petsMap[petId].fragmentCount;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    petsStr = encode(s);
    save2PetJson();
    
    info.hp = DICTOOL->getIntValue_json(_infoDoc, "hp");
    info.attack = DICTOOL->getIntValue_json(_infoDoc, "attack");
    info.defence = DICTOOL->getIntValue_json(_infoDoc, "defence");
    
    info.hp += levelConfigMap[nextLevelId].deltaHp;
    info.attack += levelConfigMap[nextLevelId].deltaAttack;
    info.defence += levelConfigMap[nextLevelId].deltaDefence;
    _infoDoc["hp"] = info.hp;
    _infoDoc["attack"] = info.attack;
    _infoDoc["defence"] = info.defence;
    
    StringBuffer buffInfo;
    rapidjson::Writer<StringBuffer> writerInfo(buffInfo);
    _infoDoc.Accept(writerInfo);
    std::string s1 = StringUtils::format("%s", buffInfo.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    coreStr = encode(s1);
    save2Json();

    return 0;
}

bool Pet::addFragment(int petId, int count)
{
    if (petsStr == "") {
        return false;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(petsStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return false;
    }
    
    if(!_doc.IsArray())
    {
        return false;
    }
    
    int index = petConfigIdTable[petId];
    const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, index);
    
    petsMap[petId].fragmentCount = DICTOOL->getIntValue_json(dic, "fragment_count");
    petsMap[petId].fragmentCount += count;
    petsMap[petId].fragmentCount = MAX(0, petsMap[petId].fragmentCount);
    _doc[petConfigIdTable[petId]]["fragment_count"] = petsMap[petId].fragmentCount;

    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    petsStr = encode(s);
    save2PetJson();

    return true;
}

int Pet::getLevelId(int quality, int petLevel)
{
    for (auto config : levelConfigMap) {
        
        if (config.second.qualityId != quality) {
            continue;
        }
        if (config.second.level != petLevel) {
            continue;
        }
        
        return config.first;
    }
    return 0;
}

int Pet::checkFragment(int petId)
{
    int nextLevl = petsMap[petId].level + 1;
    for (auto config : levelConfigMap) {
        if (config.second.qualityId != petConfigMap[petId].quality) {
            continue;
        }
        if (config.second.level != nextLevl) {
            continue;
        }
        
        if (petsMap[petId].fragmentCount >= config.second.costFragment) {
            return config.first;
        }else{
            return 0;
        }
    }
    return 0;
}

int Pet::getNextLevelId(int petId)
{
    int nextLevl = petsMap[petId].level + 1;
    for (auto config : levelConfigMap) {
        if (config.second.qualityId != petConfigMap[petId].quality) {
            continue;
        }
        if (config.second.level != nextLevl) {
            continue;
        }
        
        return config.first;
    }
    return 0;
    
}

void Pet::checkTip()
{
    tip = false;
    uplevelTip.clear();
    int index = 0;
    for (auto config : petConfigMap) {
        uplevelTip.push_back(false);
        int petId  = config.second.petId;
        if (checkFragment(petId) != 0) {
            tip = true;
            uplevelTip[index] = true;
//            break;
        }
       index++;
    }
}

void Pet::getRelations(int petId, std::vector<int> &relation)
{
    relation.clear();
    for (int i = 0; i < 2; i++) {
        if (petConfigMap[petId].relation[i] != 0) {
            relation.push_back(petConfigMap[petId].relation[i]);
        }
    }
}

void Pet::getPetsWithSameRelation(int relationId, std::vector<int> &petsId)
{
    petsId.clear();
    for (auto config : petConfigMap) {
        for (int i = 0; i < 2; i++) {
            if (config.second.relation[i] == relationId) {
                petsId.push_back(config.second.petId);
                break;
            }
        }
    }
}

void Pet::getPetsWithQuality(int quality, std::vector<int> &petsId)
{
    petsId.clear();
    for (auto config : petConfigMap) {
        if (config.second.quality == quality) {
            petsId.push_back(config.second.petId);
        }
    }
}
