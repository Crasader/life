//
//  NPC.h
//  life
//
//  Created by ff on 17/10/18.
//
//

#ifndef NPC_h
#define NPC_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

class NPC : public cocos2d::Node {
public:
    bool init();
    
    void setupArmature(int npcId, std::string armaFile);
    
    void setDir(int dircet);
    
    void initPos(float x, float y);
    
    void changeAction(int action, bool flipX, int missionId);
    
    void plotMove(float duration, float targetX, int missionId);
    
    void passNpc(float x);
    
    void animationEvent(cocostudio::Armature *armature,
                        cocostudio::MovementEventType movementType,
                        const std::string& movementID);
    
    void plotMoveEnd();
    CREATE_FUNC(NPC);
private:
    cocostudio::Armature *armature;
    cocos2d::Sprite *shadow;
    CC_SYNTHESIZE_READONLY(int, npcId, NpcId);
    CC_SYNTHESIZE_READONLY(bool, colActive, ColActive);
    CC_SYNTHESIZE_READONLY(int, plotActionIdx, PlotActionIdx);
    CC_SYNTHESIZE_READONLY(int, plotActionMission, PlotActionMission);
    CC_SYNTHESIZE_READONLY(int, plotMoveMission, PlotMoveMission);
};

#endif /* NPC_h */
