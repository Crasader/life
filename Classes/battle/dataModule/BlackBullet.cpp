//
//  BlackBullet.cpp
//  cocosGame
//
//  Created by ff on 14-12-15.
//
//

#include "BlackBullet.h"
#include "utils/GameReader.h"

using namespace cocostudio;

std::map<int, int> BlackBullet::idTable;


void BlackBullet::readFile(int idx, std::string filename)
{
    auto doc = GameReader::getDocInstance(filename);
    if(BlackBullet::idTable.empty())
    {
        BlackBullet::initIdTable(*doc);
    }
    initData(idx, *doc);
}

void BlackBullet::initIdTable(rapidjson::Document &_doc){
    if (_doc.IsArray()){
        int size = _doc.Size();
        int objId;
        for (int i = 0; i<size; i++) {
            objId =DICTOOL->getIntValue_json(_doc[i], "ID");
            idTable.insert(std::make_pair(objId, i));
        }
    }
}

bool BlackBullet::initData(int idx, rapidjson::Document& _doc) {
    int index = idTable.find(idx)->second;
    
    const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, index);
    if (dic.IsNull()) {
        return false;
    }
    
    
    bulletId = DICTOOL->getIntValue_json(dic, "ID");
    speed = DICTOOL->getFloatValue_json(dic, "speed");
    resName = DICTOOL->getStringValue_json(dic, "res");
    scaleRate = DICTOOL->getFloatValue_json(dic, "scaleFactor");
//    bufferId = DICTOOL->getIntValue_json(dic, "buffId");
//    attack = DICTOOL->getFloatValue_json(dic, "attack");
//    isSplash = ((DICTOOL->getIntValue_json(dic, "splash") == 0)?false:true);
    width = DICTOOL->getFloatValue_json(dic, "width");
    height = DICTOOL->getFloatValue_json(dic, "height");
    bulletType = DICTOOL->getIntValue_json(dic, "type");
    count = DICTOOL->getIntValue_json(dic, "count");
    interval = DICTOOL->getFloatValue_json(dic, "interval");
    skillId = DICTOOL->getIntValue_json(dic, "skill_id");
    copyReverse = (DICTOOL->getIntValue_json(dic, "copy_reverse")==1?true:false);
    through = (DICTOOL->getIntValue_json(dic, "through")==1?true:false);
    distDis = DICTOOL->getIntValue_json(dic, "distance");
    distRange = DICTOOL->getFloatValue_json(dic, "range");
    calAngel = (DICTOOL->getIntValue_json(dic, "cal_angel") == 1?true:false);
    return true;
}
