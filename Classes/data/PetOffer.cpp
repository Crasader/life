//
//  PetOffer.cpp
//  life
//
//  Created by ff on 17/8/2.
//
//

#include "PetOffer.h"
#include "utils/GameReader.h"
#include "GameDefine.h"
#include "utils/libs.h"
#include "utils/GameUtils.h"

USING_NS_CC;
using namespace cocostudio;
using namespace rapidjson;

std::map<int, int> PetOffer::configIdTable;

PetOffer::PetOffer():
boardStr(""),
dataStr(""),
tip(false)
{
    configMap.clear();
    configIdTable.clear();
    workOffers.clear();
    boardOffer.clear();
    totalOfferVec.clear();
    optOfferId.clear();
    optOfferVec.clear();
}

PetOffer::~PetOffer()
{
    
}

void PetOffer::readConfig()
{
    auto doc = GameReader::getDocInstance(PET_OFFER_CONFIG_FILE);
    if (configIdTable.empty()) {
        GameReader::initIdTable(*doc, "id", configIdTable);
    }
    
    configMap.clear();
    optOfferId.clear();
    for (int i = 0; i < configIdTable.size(); i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, i);
        
        PET_OFFER_CONFIG config;
        config.offerId = DICTOOL->getIntValue_json(dic, "id");
        config.type = DICTOOL->getIntValue_json(dic, "type");
        config.petCount = DICTOOL->getIntValue_json(dic, "pet_count");
        config.costHours = DICTOOL->getIntValue_json(dic, "cost_time");
        config.price = DICTOOL->getIntValue_json(dic, "price");
        config.boundLevel = DICTOOL->getIntValue_json(dic, "bound_level");
        for (int i = 0; i < 3; i++) {
            config.boundType[i] = DICTOOL->getIntValue_json(dic, GameUtils::format("bound%d_type", i+1).c_str());
            config.boundId[i] = DICTOOL->getIntValue_json(dic, GameUtils::format("bound%d_id", i+1).c_str());
            config.boundCount[i] = DICTOOL->getIntValue_json(dic, GameUtils::format("bound%d_count", i+1).c_str());
        }
        if(config.boundLevel>5)
        {
            optOfferId.push_back(config.offerId);
        }
        configMap[config.offerId] = config;
    }
    
    randOffers();
    randOptOffers();
}

void PetOffer::deleteDataJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "pet_board_data.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return;
    }
    
    FileUtils::getInstance()->removeFile(path);
}

void PetOffer::deleteBoardJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "pet_board_offer.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return;
    }
    
    FileUtils::getInstance()->removeFile(path);
}

void PetOffer::save2DataJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "pet_board_data.json";
    
    FILE* pFile = fopen(path.c_str(),"wb");
    if (pFile)
    {
        fputs(dataStr.c_str(),pFile);
        fclose(pFile);
    }else
    {
        CCLOG("UserManage::SaveUserToFile error! can not open %s",path.c_str());
    }
}
void PetOffer::save2BoardJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "pet_board_offer.json";
    
    FILE* pFile = fopen(path.c_str(),"wb");
    if (pFile)
    {
        fputs(boardStr.c_str(),pFile);
        fclose(pFile);
    }else
    {
        CCLOG("UserManage::SaveUserToFile error! can not open %s",path.c_str());
    }
}

bool PetOffer::readDataJson()
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += "pet_board_data.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return false;
    }
    
    dataStr = FileUtils::getInstance()->getStringFromFile(path);
    if (!parseDataJson()) {
        return false;
    }
    
    return true;
}

bool PetOffer::readBoardJson()
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += "pet_curr_offer.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return false;
    }
    
    boardStr = FileUtils::getInstance()->getStringFromFile(path);
    if (!parseBoardJson()) {
        return false;
    }
    
    return true;
}

bool PetOffer::parseDataJson()
{
    if (dataStr == "") {
        return false;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(dataStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return false;
    }
    
    if(!_doc.IsObject())
    {
        return false;
    }
    
    offerData.refreshCount = DICTOOL->getIntValue_json(_doc, "refresh_count");

    
    return true;
}
bool PetOffer::parseBoardJson()
{
    if (boardStr == "") {
        return false;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(boardStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return false;
    }
    
    if(!_doc.IsArray())
    {
        return false;
    }

    for (int i = 0; i < 3; i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, i);
        PET_OFFER_INFO info;
        info.offerId = DICTOOL->getIntValue_json(dic, "offer_id");
        info.state = DICTOOL->getIntValue_json(dic, "state");
        memset(info.petId, 0, sizeof(int)*3);
        boardOffer[i+1] = info;
    }
    
    return true;
}

void PetOffer::save2EmptyDataStr()
{
    rapidjson::Document _doc;
    if (dataStr != "") {
        return;
    }else{
        _doc.SetObject();
    }
    
    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    _doc.AddMember("refresh_count", 0, allocator);
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    dataStr = encode(s);
    save2DataJson();
}
void PetOffer::save2EmptyBoardStr()
{
    rapidjson::Document _doc;
    if (boardStr != "") {
        return;
    }else{
        _doc.SetArray();
    }
    
    for (int i = 0; i < 3; i++) {
        PET_OFFER_INFO info;
        info.offerId = totalOfferVec[totalOfferVec.size()-1];
        info.state = 0;
        memset(info.petId, 0, sizeof(int)*3);
        boardOffer[i+1] = info;
        totalOfferVec.pop_back();
    }

    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    for (int i = 0; i < 3; i++) {
        rapidjson::Value object(rapidjson::kObjectType);
        object.AddMember("offer_id", boardOffer[i+1].offerId, allocator);
        object.AddMember("state", boardOffer[i+1].state, allocator);
        _doc.PushBack(object, allocator);
    }

    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    boardStr = encode(s);
    save2BoardJson();
}

void PetOffer::randOffers()
{
    totalOfferVec.clear();

    GameUtils::getRandArray(totalOfferVec, (int)configIdTable.size());
    
}
void PetOffer::randOptOffers()
{
    optOfferVec.clear();
    std::vector<int> tempVec;
    GameUtils::getRandArray(tempVec, (int)optOfferId.size());
    for (auto optId : tempVec) {
        optOfferVec.push_back(optOfferId[optId-1]);
    }
}
void PetOffer::refreshBoardOffers()
{
    if (boardStr == "") {
        return;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(boardStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    if(!_doc.IsArray())
    {
        return;
    }

    if (totalOfferVec.size() < configIdTable.size()/2) {
        randOffers();
    }
    boardOffer.clear();
    for (int i = 0; i < 3; i++) {
        PET_OFFER_INFO info;
        info.offerId = totalOfferVec[totalOfferVec.size()-1];
        info.state = 0;
        memset(info.petId, 0, sizeof(int)*3);
        boardOffer[i+1] = info;
        totalOfferVec.pop_back();
        
        _doc[i]["offer_id"] = info.offerId;
        _doc[i]["state"] = 0;
    }
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    boardStr = encode(s);
    save2BoardJson();
    

}

void PetOffer::refreshOptBoardOffers()
{
    if (boardStr == "") {
        return;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(boardStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return;
    }
    
    if(!_doc.IsArray())
    {
        return;
    }
    
    if (optOfferVec.size() < optOfferId.size()/2) {
        randOptOffers();
    }
    boardOffer.clear();
    for (int i = 0; i < 3; i++) {
        PET_OFFER_INFO info;
        info.offerId = optOfferVec[optOfferVec.size()-1];
        info.state = 0;
        memset(info.petId, 0, sizeof(int)*3);
        boardOffer[i+1] = info;
        optOfferVec.pop_back();
        
        _doc[i]["offer_id"] = info.offerId;
        _doc[i]["state"] = 0;
    }
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    boardStr = encode(s);
    save2BoardJson();
    
    if (dataStr == "") {
        return;
    }
    
    rapidjson::Document _dataDoc;
    std::string tempDataStr = decode(dataStr.c_str());
    _dataDoc.Parse < 0 > (tempDataStr.c_str());
    if(_dataDoc.HasParseError())
    {
        return;
    }
    
    if(!_dataDoc.IsObject())
    {
        return;
    }
    
    offerData.refreshCount = DICTOOL->getIntValue_json(_dataDoc, "refresh_count");
    offerData.refreshCount++;
    offerData.refreshCount = MIN(offerData.refreshCount, maxRefreshOfferCount-1);
    _dataDoc["refresh_count"] = offerData.refreshCount;
    
    StringBuffer buffData;
    rapidjson::Writer<StringBuffer> writerData(buffData);
    _dataDoc.Accept(writerData);
    std::string sData = StringUtils::format("%s", buffData.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    dataStr = encode(sData);
    save2DataJson();
}

void PetOffer::deleteWorkJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "pet_work_offer.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return;
    }
    
    FileUtils::getInstance()->removeFile(path);
}


void PetOffer::save2WorkJson()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path += "pet_work_offer.json";
    
    FILE* pFile = fopen(path.c_str(),"wb");
    if (pFile)
    {
        fputs(workStr.c_str(),pFile);
        fclose(pFile);
    }else
    {
        CCLOG("UserManage::SaveUserToFile error! can not open %s",path.c_str());
    }
}


bool PetOffer::readWorkJson()
{
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += "pet_work_offer.json";
    
    if(!FileUtils::getInstance()->isFileExist(path)) {
        return false;
    }
    
    workStr = FileUtils::getInstance()->getStringFromFile(path);
    if (!parseWorkJson()) {
        return false;
    }
    
    return true;
}


bool PetOffer::parseWorkJson()
{
    if (workStr == "") {
        return false;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(workStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return false;
    }
    
    if(!_doc.IsArray())
    {
        return false;
    }
    
    for (int i = 0; i < 3; i++) {
        const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, i);
        PET_OFFER_INFO info;
        info.offerId = DICTOOL->getIntValue_json(dic, "offer_id");
        info.state = DICTOOL->getIntValue_json(dic, "state");
        memset(info.petId, 0, sizeof(int)*3);
        for (int j = 0; j < 3; j++) {
            info.petId[j] = DICTOOL->getIntValue_json(dic, GameUtils::format("pet_id%d", j+1).c_str());
        }
        workOffers[i+1] = info;
    }
    
    return true;
}

void PetOffer::save2EmptyWorkStr()
{
    rapidjson::Document _doc;
    if (workStr != "") {
        return;
    }else{
        _doc.SetArray();
    }
    
    rapidjson::Document::AllocatorType& allocator= _doc.GetAllocator();
    for (int i = 0; i < 3; i++) {
        rapidjson::Value object(rapidjson::kObjectType);
        object.AddMember("offer_id", 0, allocator);
        object.AddMember("state", 2, allocator);
        object.AddMember("pet_id1", 0, allocator);
        object.AddMember("pet_id2", 0, allocator);
        object.AddMember("pet_id3", 0, allocator);
        _doc.PushBack(object, allocator);
    }
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    workStr = encode(s);
    save2WorkJson();
}

int PetOffer::takeOffer(int pos, int petId[3])
{
    if (pos < 1 || pos >3) {
        return 2;
    }
    if (workOffers.empty()) {
        return 1;
    }
    workPos = 0;
    for (auto offer : workOffers) {
        if (offer.second.offerId == 0) {
            workPos = offer.first;
            break;
        }
    }
    if (workPos == 0) {
        return 3;
    }
    if (boardStr == "") {
        return 1;
    }
    
    rapidjson::Document _doc;
    std::string tempStr = decode(boardStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return 1;
    }
    
    if(!_doc.IsArray())
    {
        return 1;
    }
    if (workStr == "") {
        return 1;
    }
    rapidjson::Document _doc1;
    std::string tempStr1 = decode(workStr.c_str());
    _doc1.Parse < 0 > (tempStr1.c_str());
    if(_doc1.HasParseError())
    {
        return 1;
    }
    
    if(!_doc1.IsArray())
    {
        return 1;
    }

    boardOffer[pos].state = 1;
    _doc[pos-1]["state"] = 1;

    workOffers[workPos].offerId = boardOffer[pos].offerId;
    workOffers[workPos].state = 1;
    memcpy(workOffers[workPos].petId, petId, sizeof(int)*3);
    _doc1[workPos-1]["offer_id"] = workOffers[workPos].offerId;
    _doc1[workPos-1]["state"] = workOffers[workPos].state;
    for (int i = 0; i < 3; i++) {
        _doc1[workPos-1][GameUtils::format("pet_id%d", i+1).c_str()] = workOffers[workPos].petId[i];
    }
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    boardStr = encode(s);
    save2BoardJson();
    
    StringBuffer buff1;
    rapidjson::Writer<StringBuffer> writer1(buff1);
    _doc1.Accept(writer1);
    std::string s1 = StringUtils::format("%s", buff1.GetString());
    workStr = encode(s1);
    save2WorkJson();
    
    return 0;
}

int PetOffer::overWork(int pos)
{
    if (pos < 1 || pos >3) {
        return 2;
    }
    if (workOffers.empty()) {
        return 1;
    }
    if (workStr == "") {
        return 1;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(workStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return 1;
    }
    
    if(!_doc.IsArray())
    {
        return 1;
    }

    workOffers[pos].state = 2;
    _doc[pos-1]["state"] = workOffers[pos].state;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    workStr = encode(s);
    save2WorkJson();

    return 0;
}

int PetOffer::takeReward(int pos)
{
    if (pos < 1 || pos >3) {
        return 2;
    }
    if (workOffers.empty()) {
        return 1;
    }
    if (workStr == "") {
        return 1;
    }
    rapidjson::Document _doc;
    std::string tempStr = decode(workStr.c_str());
    _doc.Parse < 0 > (tempStr.c_str());
    if(_doc.HasParseError())
    {
        return 1;
    }
    
    if(!_doc.IsArray())
    {
        return 1;
    }
    
    workOffers[pos].offerId = 0;
    workOffers[pos].state = 3;
    _doc[pos-1]["offer_id"] = workOffers[pos].offerId;
    _doc[pos-1]["state"] = workOffers[pos].state;
    
    StringBuffer buff;
    rapidjson::Writer<StringBuffer> writer(buff);
    _doc.Accept(writer);
    std::string s = StringUtils::format("%s", buff.GetString());
    workStr = encode(s);
    save2WorkJson();
    return 0;
}

int PetOffer::getWorkOfferCount()
{
    int count = 0;
    for (auto offer : workOffers) {
        if (offer.second.offerId != 0) {
            count++;
        }
    }
    
    return count;
}

void PetOffer::getWorkPetsId(std::vector<int> &petsId)
{
    petsId.clear();
    for (auto offer : workOffers) {
        if (offer.second.offerId == 0) {
            continue;
        }
        for (int i = 0; i < 3; i++) {
            if (offer.second.petId[i] != 0) {
                petsId.push_back(offer.second.petId[i]);
            }
        }
    }
}

void PetOffer::checkTip()
{
    tip = false;
    for (auto offer :workOffers) {
        if (offer.second.offerId <= 0) {
            continue;
        }
        if (offer.second.state == 2) {
            tip = true;
            return;
        }
    }
}

void PetOffer::resetRefreshCount()
{
    if (dataStr == "") {
        return;
    }
    
    rapidjson::Document _dataDoc;
    std::string tempDataStr = decode(dataStr.c_str());
    _dataDoc.Parse < 0 > (tempDataStr.c_str());
    if(_dataDoc.HasParseError())
    {
        return;
    }
    
    if(!_dataDoc.IsObject())
    {
        return;
    }
    
    offerData.refreshCount = 0;
    _dataDoc["refresh_count"] = offerData.refreshCount;
    
    StringBuffer buffData;
    rapidjson::Writer<StringBuffer> writerData(buffData);
    _dataDoc.Accept(writerData);
    std::string sData = StringUtils::format("%s", buffData.GetString());
    //    log("========================\n");
    //    log("%s\n", s.c_str());
    dataStr = encode(sData);
    save2DataJson();
}
