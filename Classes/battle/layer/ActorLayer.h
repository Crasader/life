//
//  ActorLayer.h
//  soul
//
//  Created by ff on 14-9-15.
//
//

#ifndef __soul__ActorLayer__
#define __soul__ActorLayer__

#include "cocos2d.h"
#include "../actor/FightActor.h"
#include "../actor/Hero.h"
#include "../actor/ProtectNPC.h"
#include "../actor/Monster.h"
#include "HitEffectLayer.h"
#include "GameDefine.h"
#include "../actor/Bullet.h"
#include "../actor/NPC.h"


class ActorLayer : public cocos2d::Layer
{
public:
    ActorLayer();
    ~ActorLayer();
    bool init();
    void onEnter();
    void onExit();
    CREATE_FUNC(ActorLayer);
    
    void setHero(Hero *hero);
    void setWitness(ProtectNPC *witness);
    void update(float dt);
    void fillHitEffect();
    void addMonster(int monId, int teamId, float x, float y, int buffId[5], bool opponentFlag);
    void addGear(int gearId, float x, float y, bool flipX);
    void createBullet(cocos2d::EventCustom *event);
    void addActorHitEffect(FightActor *actor);
    void monsterView2Hero(FightActor *monster);
    void removeMonster(Monster *monster);
    void removeBullet(Bullet *bullet);
    
    std::vector<FightActor *> findEnemies(FightActor *man);
    FightActor* getMonsterWithTeamId(int teamId);
    void sendMonsterTeam(cocos2d::EventCustom *event);
    void setupOpponent(cocos2d::EventCustom *event);
    
    void addNPC(cocos2d::EventCustom *event);
    void removeNPC(cocos2d::EventCustom *event);
    void changeNPCAction(int npcId, int actionId, bool flipX, int missionId);
    void moveNPC(int npcId, float duration, int targetX, int missionId);
    void changeMonsterAction(int monsterId, int actionId, bool flipX, int missionId);
    void moveMonster(int monsterId, float duration, int targetX, int missionId);
    
    void setPlotPause(bool val);
private:
    cocos2d::Vector<FightActor *> heroVec;
    cocos2d::Vector<FightActor *> witnessVec;
    cocos2d::Vector<FightActor *> monsterVec;
    cocos2d::Vector<FightActor *> hurtMonsters;
    cocos2d::Vector<Bullet *> monsterBullets;
    cocos2d::Vector<Bullet *> heroBullets;
    cocos2d::Vector<FightActor *> gearVec;
    cocos2d::Vector<NPC *> npcVec;
//    Hero *hero;
    
    cocos2d::EventListenerCustom *sendMonsterTeamListener;
    cocos2d::EventListenerCustom *fireListener;
    cocos2d::EventListenerCustom *addNpcListener;
    cocos2d::EventListenerCustom *removeNpcListener;
    
    CC_SYNTHESIZE_READONLY(Hero *, hero, Hero);
    CC_SYNTHESIZE_READONLY(ProtectNPC *, witness, Witness);
    CC_SYNTHESIZE_READONLY(OPPONENT_ROBOT, oppRobot, OppRobot);
    CC_SYNTHESIZE_READONLY(FightActor *, showMonster, ShowMonster);
    CC_SYNTHESIZE_READONLY(HitEffectLayer *, hitEffeLayer, HitEffeLayer);
    CC_SYNTHESIZE_READONLY(Layer *, ghostLayer, GhostLayer);
    CC_SYNTHESIZE(bool, pause, Pause);
    CC_SYNTHESIZE_READONLY(bool, plotPause, PlotPause);
};

#endif /* defined(__soul__ActorLayer__) */
