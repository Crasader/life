//
//  BuffManager.cpp
//  WuhunLua
//
//  Created by ff on 15-3-19.
//
//

#include "BuffManager.h"
#include "utils/GameReader.h"
#include "GameDefine.h"

USING_NS_CC;
using namespace cocostudio;

std::map<int, int> BuffManager::idTable;
std::map<int, int> BuffManager::targetTable;
std::map<int, int> BuffManager::typeTable;
std::map<int, bool> BuffManager::deBuffTable;

bool BuffManager::init()
{
    if (!Node::init()) {
        return false;
    }
    buffs.clear();
    
    auto doc = GameReader::getDocInstance(BUFF_CONFIG_FILE);
    if(BuffManager::idTable.empty())
    {
        BuffManager::initIdTable(*doc);
    }
    
//    scheduleUpdate();
    return true;
}


//void BuffManager::onExit()
//{
//    buffs.clear();
//}

void BuffManager::initIdTable(rapidjson::Document &_doc){
    if (_doc.IsArray()){
        int size = _doc.Size();
        int objId;
        for (int i = 0; i<size; i++) {
            objId =DICTOOL->getIntValue_json(_doc[i], "ID");
            idTable.insert(std::make_pair(objId, i));
            int target = DICTOOL->getIntValue_json(_doc[i], "target");
            targetTable[objId] = target;
            int type = DICTOOL->getIntValue_json(_doc[i], "type");
            typeTable[objId] = type;
            bool debuff = (DICTOOL->getIntValue_json(_doc[i], "debuff") == 1);
            deBuffTable[objId] = debuff;
        }
    }
}

int BuffManager::addBuff(int idx, std::function<void(Buff *)> handle, std::function<void(Buff *)> remove)
{
    if (idx < 0) {
        return 0;
    }
    if (!immuTable.empty()) {
        if (immuTable.find(typeTable[idx])!=immuTable.end()) {
            if ((immuTable[typeTable[idx]] > 0) && deBuffTable[idx]) {
                return 0;
            }
        }
    }

    auto doc = GameReader::getDocInstance(BUFF_CONFIG_FILE);
    if(BuffManager::idTable.empty())
    {
        BuffManager::initIdTable(*doc);
    }
    
    if (buffs.find(idx) != buffs.end()) {
        replaceBuff(idx, *doc, handle, remove);
        return 2;
    }else{
        addBuff(idx, *doc, handle, remove);
        return 1;
    }
    
    return 1;
}

void BuffManager::addBuff(int buffId, rapidjson::Document& _doc, std::function<void(Buff *)> handle, std::function<void(Buff *)> remove)
{
    
    Buff* buff = Buff::create();
    
    int index = idTable.find(buffId)->second;
    buff->initData(index, _doc);
    
    if (typeTable[buffId] == BUFF_TYPE_IMMU_DEBUFF) {
        for (auto obj : buffs) {
            if (obj.second->getType() == buff->getValue() && obj.second->getDeBuff()) {
                obj.second->onRemove(0);
            }
        }
        immuTable[buff->getValue()]++;
    }else if (typeTable[buffId] == BUFF_TYPE_IMMU_ALL) {
        for (auto obj : buffs) {
            if (obj.second->getDeBuff()) {
                obj.second->onRemove(0);
            }
        }
        
        for (int n = BUFF_TYPE_MAX_HP; n<BUFF_TYPE_MAX;n++)
        {
            switch (n) {
                case BUFF_TYPE_IMMU_ALL:
                case BUFF_TYPE_IMMU_DEBUFF:
                    break;
                default:
                    immuTable[n]++;
                    break;
            }
        }
    }
    
    buff->initHandle(handle, remove);
    buffs[buffId] = buff;
    buff->beginActive();
    addChild(buff);
    buff->setTag(buff->getBuffId());
    

}

void BuffManager::replaceBuff(int buffId, rapidjson::Document& _doc, std::function<void(Buff *)> handle, std::function<void(Buff *)> remove)
{
    if(buffs[buffId]->getValid()){
        buffs[buffId]->onRemove(0);
    }
    addBuff(buffId, _doc, handle, remove);
}

void BuffManager::removeBuff(int buffId)
{
//    if(buffs[buffId]->getValid()){
//        buffs[buffId]->onRemove(0);
//    }
    removeChild(buffs[buffId]);
    buffs.erase(buffId);
}

void BuffManager::removeAllBuff()
{
    removeAllChildren();
    unscheduleUpdate();
//    if (!buffs.empty()) {
//        for (auto buff : buffs) {
//            if(buff.second->getValid()){
//                buff.second->onRemove(0);
//            }
//        }
//    }

    buffs.clear();
}

void BuffManager::removeImmu(int buffId)
{
    if (typeTable[buffId] == BUFF_TYPE_IMMU_DEBUFF) {
        immuTable[buffs[buffId]->getValue()]--;
        immuTable[buffs[buffId]->getValue()] = MAX(0, immuTable[buffs[buffId]->getValue()]);
    }else if (typeTable[buffId] == BUFF_TYPE_IMMU_ALL) {
        for (auto immu : immuTable) {
            immu.second--;
            immu.second = MAX(0, immu.second);
        }
    }
}

void BuffManager::update(float dt)
{
//    for (auto buff : buffs) {
//        if (!buff.second->getValid()) {
//            if (buffs.size() == 1) {
//                buffs.clear();
//            }else{
//               buffs.erase(buff.first);
//            }
//            removeChild(buff.second);
//        }
//    }
}

