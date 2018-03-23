//
//  NPC.cpp
//  life
//
//  Created by ff on 17/10/18.
//
//

#include "NPC.h"
#include "GameDefine.h"
#include "ELProtocol.h"
#include "utils/GameUtils.h"
#include "logic/ClientLogic.h"

USING_NS_CC;
using namespace cocostudio;

bool NPC::init()
{
    if (!Node::init()) {
        return false;
    }
    
    colActive = false;
    plotActionIdx = -1;
    plotActionMission = 0;
    plotMoveMission = 0;
    return true;
}

void NPC::setupArmature(int npcId, std::string armaFile)
{
    this->npcId = npcId;
    
    shadow = Sprite::create(SHADOW_FILE);
    shadow->setAnchorPoint(Vec2(0.5, 0.5));
    shadow->setPosition(Vec2(0, 0));
    addChild(shadow);
    
    //初始化和资源载入
    //  std::string resPath = ARM_DIR + blackActor->getResName() + ".ExportJson";
//    std::string resPath = GameUtils::format(ACTOR_DIR,
//                                            armaFile.c_str(),
//                                            armaFile.c_str());
    std::string resPath = GameUtils::format(ACTOR_DIR,
                                            armaFile.c_str());
    ArmatureDataManager::getInstance()->addArmatureFileInfo(resPath);
    armature = Armature::create(armaFile);
    
    
    addChild(armature,10);
    armature->getAnimation()->playWithIndex(0);
    
}

void NPC::initPos(float x, float y)
{
    setPosition(Vec2(x, y));
}

void NPC::setDir(int dircet)
{
    if (dircet == 1) {
        armature->setScaleX(-1);
    }else{
        armature->setScaleX(1);
    }
}

void NPC::changeAction(int action, bool flipX, int missionId)
{
    plotActionIdx = action;
    plotActionMission = missionId;
    armature->setScaleX(flipX?-1:1);
    armature->getAnimation()->playWithIndex(action);
    
    armature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(NPC::animationEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}
void NPC::animationEvent(Armature *armature,
                         MovementEventType movementType,
                         const std::string& movementID)
{
    
    if (movementType == MovementEventType::COMPLETE)
    {
        if (plotActionIdx != -1) {
            armature->getAnimation()->playWithIndex(0);
            
            E2L_COMPLETE_PLOT infoPlot;
            infoPlot.eProtocol = e2l_complete_plot;
            infoPlot.missionId = plotActionMission;
            infoPlot.value = 0;
            ClientLogic::instance()->ProcessUIRequest(&infoPlot);
            
            plotActionIdx = -1;
            plotActionMission = 0;
        }
    }else if (movementType == MovementEventType::LOOP_COMPLETE) {
        if (plotActionIdx != -1) {
            E2L_COMPLETE_PLOT infoPlot;
            infoPlot.eProtocol = e2l_complete_plot;
            infoPlot.missionId = plotActionMission;
            infoPlot.value = 0;
            ClientLogic::instance()->ProcessUIRequest(&infoPlot);
            
            plotActionIdx = -1;
            plotActionMission = 0;
        }
    }
}
void NPC::plotMove(float duration, float targetX, int missionId)
{
    plotMoveMission = missionId;
    auto move = MoveTo::create(duration, Vec2(targetX, GroundY));
    auto func = CallFunc::create(CC_CALLBACK_0(NPC::plotMoveEnd, this));
    auto seq = Sequence::create(move, func,  NULL);
    runAction(seq);
}

void NPC::plotMoveEnd()
{
    E2L_COMPLETE_PLOT infoPlot;
    infoPlot.eProtocol = e2l_complete_plot;
    infoPlot.missionId = plotMoveMission;
    infoPlot.value = 0;
    ClientLogic::instance()->ProcessUIRequest(&infoPlot);
    plotMoveMission = 0;
}

void NPC::passNpc(float x)
{
    if (colActive) {
        return;
    }
    
    if (x >= getPositionX()) {
        colActive = true;
        
        E2L_TRIGGER_PASS_NPC info;
        info.eProtocol = e2l_trigger_pass_npc;
        info.npcId = npcId;
        ClientLogic::instance()->ProcessUIRequest(&info);
    }
}
