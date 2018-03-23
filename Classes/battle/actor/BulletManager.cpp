//
//  BulletManager.cpp
//  life
//
//  Created by ff on 17/6/8.
//
//

#include "BulletManager.h"
#include "GameDefine.h"
#include "EventDefine.h"
#include "utils/GameUtils.h"

USING_NS_CC;
using namespace cocostudio;

std::map<int, int> BulletManager::idTable;

BulletManager::BulletManager()
{
    idTable.clear();
    cmdQueue.clear();
    
    
}

BulletManager::~BulletManager()
{
    idTable.clear();
    cmdQueue.clear();
    
    Director::getInstance()->getEventDispatcher()->removeEventListener(createGunListner);
}

bool BulletManager::init()
{
    if (!Node::init()) {
        return false;
    }
    return true;
}
void BulletManager::onEnter()
{
    Node::onEnter();
    createGunListner = EventListenerCustom::create(CREATE_GUN, CC_CALLBACK_1(BulletManager::createBullets, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(createGunListner, -1);
}

void BulletManager::createBullets(EventCustom *event)
{
    CREATE_GUN_DATA data = *static_cast<CREATE_GUN_DATA*>(event->getUserData());
    CREATE_BULLET_CMD cmd;
    readFile(data.bulletId, BULLET_CONFIG_FILE, cmd);
    cmd.gunX = data.gunPosX;
    cmd.gunY = data.gunPosY;
    cmd.ownerType = data.ownerType;
    cmd.ownerX = data.actorPosX;
    cmd.ownerY = data.actorPosY;
    cmd.flipX = data.flipX;
    cmd.baseAttack = data.baseAttack;
    cmd.baseCritRatio = data.baseCritRatio;
    cmd.comboFactor = data.comboFactor;
    fire(cmd);
    if (cmd.count > 0) {
        cmdQueue.push_back(cmd);
    }
}

void BulletManager::update(float dt)
{
    for (auto iter = cmdQueue.begin(); iter != cmdQueue.end(); ) {
        (*iter).interval -= dt;
        if ((*iter).interval <= 0) {
            fire(*iter);
        }
        if ((*iter).count <= 0) {
            iter = cmdQueue.erase(iter);
        }else{
            iter++;
        }
    }
}

void BulletManager::readFile(int idx, std::string filename, CREATE_BULLET_CMD& cmd)
{
    auto doc = GameReader::getDocInstance(filename);
    if (idTable.empty()) {
        GameReader::initIdTable(*doc, "ID", idTable);
    }
    
    initData(idx, *doc, cmd);
}

bool BulletManager::initData(int idx, rapidjson::Document &_doc, CREATE_BULLET_CMD &cmd)
{
    int index = idTable.find(idx)->second;
    
    const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, index);
    if (dic.IsNull()) {
        return false;
    }
    
    cmd.bulletConfigId = DICTOOL->getIntValue_json(dic, "ID");
    cmd.speed = DICTOOL->getFloatValue_json(dic, "speed");
    cmd.res = DICTOOL->getStringValue_json(dic, "res");
    cmd.scale = DICTOOL->getFloatValue_json(dic, "scaleFactor");
    cmd.bulletWidth = DICTOOL->getFloatValue_json(dic, "width");
    cmd.bulletHeight = DICTOOL->getFloatValue_json(dic, "height");
    cmd.bulletType = DICTOOL->getIntValue_json(dic, "type");
    cmd.configCount = DICTOOL->getIntValue_json(dic, "count");
    cmd.configInterval = DICTOOL->getFloatValue_json(dic, "interval");
    cmd.bulletSkillId = DICTOOL->getIntValue_json(dic, "skill_id");
    cmd.copyReverse = (DICTOOL->getIntValue_json(dic, "copy_reverse")==1?true:false);
    cmd.through = (DICTOOL->getIntValue_json(dic, "through")==1?true:false);
    cmd.distDistance = DICTOOL->getIntValue_json(dic, "distance");
    cmd.distRange = DICTOOL->getFloatValue_json(dic, "range");
    cmd.calAngel = (DICTOOL->getIntValue_json(dic, "cal_angel") == 1?true:false);
    
    cmd.count = cmd.configCount;
    cmd.interval = cmd.configInterval;
    return true;
}

void BulletManager::fire(CREATE_BULLET_CMD& cmd)
{
    cmd.count--;
    cmd.interval = cmd.configInterval;
    loose(cmd, false);
    
    if (cmd.copyReverse) {
        loose(cmd, true);
    }
}

void BulletManager::loose(CREATE_BULLET_CMD &cmd, bool copyReverse)
{
    CREATE_BULLET_DATA data;
    data.res = cmd.res;
    data.bulletType = cmd.bulletType;
    data.bulletWidth = cmd.bulletWidth;
    data.bulletHeight = cmd.bulletHeight;
    data.bulletSkillId = cmd.bulletSkillId;
    data.scale = cmd.scale;
    data.calAngel = cmd.calAngel;
    data.comboFactor = cmd.comboFactor;
    data.flipX = (copyReverse?!cmd.flipX:cmd.flipX);
    if (cmd.bulletType == 3) {
        data.speed = cmd.speed;
    }else{
        data.speed = cmd.speed*(data.flipX?-1:1);
    }
    data.through = cmd.through;
    data.srcX = cmd.ownerX + cmd.gunX*(data.flipX?-1:1);
    data.srcY = cmd.ownerY + cmd.gunY;
    if (data.bulletType == 1) {
        if (cmd.distDistance > 0) {
            data.distPosX = cmd.ownerX + cmd.distDistance*(data.flipX?-1:1);
        }else{
            data.distPosX = 0;
        }
    }else if (data.bulletType == 2) {
        if (cmd.distDistance > 0) {
            data.srcX += cmd.distDistance*(data.flipX?-1:1)*(cmd.configCount - cmd.count);
            data.distPosX = 0;
        }
    }else if (data.bulletType == 3) {
        if (cmd.distRange == 0) {
            //全屏
            data.srcX = -10000;
            data.distPosX = rand()%(int)(GameUtils::winSize.width)-GameUtils::winSize.width/2;
        }else{
            data.srcX = cmd.ownerX + cmd.distDistance*(data.flipX?-1:1);
            data.distPosX = rand()%(int)(cmd.distRange)- cmd.distRange/2;
        }
        
        float tempY = rand()%50;
        data.scale += tempY / 200;
        data.srcY = cmd.ownerY + tempY;
    }else{
        data.distPosX = cmd.ownerX + cmd.distDistance*(data.flipX?-1:1);
        if (cmd.distRange > 0) {
            data.distPosX += rand()%cmd.distRange/2;
        }
    }

    data.ownerType = cmd.ownerType;
    data.baseAttack = cmd.baseAttack;
    data.baseCritRatio = cmd.baseCritRatio;
    EventCustom event(CREATE_BULLET);
    event.setUserData(&data);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}
