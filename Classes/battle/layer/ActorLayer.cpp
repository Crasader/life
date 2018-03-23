//
//  ActorLayer.cpp
//  soul
//
//  Created by ff on 14-9-15.
//
//

#include "ActorLayer.h"
#include "EventDefine.h"
#include "ELProtocol.h"
#include "../actor/Monster.h"
#include "cocostudio/CocoStudio.h"
#include "../actor/Bullet.h"
#include "../actor/YNCamera.h"
#include "../actor/Gear.h"
#include "logic/ClientLogic.h"


USING_NS_CC;
using namespace cocostudio;

ActorLayer::ActorLayer()
{
    
}
ActorLayer::~ActorLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(sendMonsterTeamListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(fireListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(addNpcListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(removeNpcListener);
}
bool ActorLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
//    auto monster = Monster::create();
//    monster->addHurtListener();
//	monster->setAnchorPoint(Vec2(0.5, 0));
//    monster->setPosX(1600);
//    monster->setPosY(150);
//	monster->setPosition(Vec2(1600, 150));
//    
//    
//    auto p1 = Vec2(1700, 150);
//    auto p2 = Vec2(1000, 150);
//    std::vector<Point> line;
//    line.push_back(p1);
//    line.push_back(p2);
//    monster->setPartolPosVec(line);
//	addChild(monster);
//
//    monsterVec.pushBack(monster);
    plotPause = false;
    showMonster = nullptr;
    witness = nullptr;
    witnessVec.clear();
    npcVec.clear();
    oppRobot.jobId = 0;
    return true;
}
void ActorLayer::onEnter()
{
    Layer::onEnter();
    sendMonsterTeamListener = EventListenerCustom::create(SETUP_MONSTER_TEAM, CC_CALLBACK_1(ActorLayer::sendMonsterTeam, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(sendMonsterTeamListener, -1);
    fireListener = EventListenerCustom::create(CREATE_BULLET, CC_CALLBACK_1(ActorLayer::createBullet, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(fireListener, -1);
    addNpcListener = EventListenerCustom::create(ADD_NPC, CC_CALLBACK_1(ActorLayer::addNPC, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(addNpcListener, -1);
    removeNpcListener = EventListenerCustom::create(REMOVE_NPC, CC_CALLBACK_1(ActorLayer::removeNPC, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(removeNpcListener, -1);
}

void ActorLayer::createBullet(cocos2d::EventCustom *event)
{
    CREATE_BULLET_DATA data = *static_cast<CREATE_BULLET_DATA*>(event->getUserData());
    auto bullet = Bullet::create();
    bullet->setupData(event);
    addChild(bullet);
    if (data.ownerType == 1) {
        heroBullets.pushBack(bullet);
    }else{
        monsterBullets.pushBack(bullet);
    }
}

void ActorLayer::addGear(int gearId, float x, float y, bool flipX)
{
    auto gear = Gear::create();
    addChild(gear);
    gear->setupData(gearId);
//    gear->initFightData(gearId);
//    gear->initViewRes();
//    gear->initFightState();
    gear->setPosX(x);
    gear->setPosY(y);
    gear->setPosition(Vec2(x, y));
    gear->setFlipX(flipX);
    gear->getArmature()->setScaleX(flipX?-1:1);
    monsterView2Hero(gear);
    
    gear->preloadSkillRes();
    
    gearVec.pushBack(gear);
}

void ActorLayer::addNPC(cocos2d::EventCustom *event)
{
    L2E_ADD_NPC info = *static_cast<L2E_ADD_NPC*>(event->getUserData());
    auto npc = NPC::create();
    npc->setupArmature(info.npcId, info.armaFile);
    npc->initPos(info.posX, GroundY);
    npc->setDir(info.direct);
    addChild(npc);
    
    npcVec.pushBack(npc);
    
    E2L_COMPLETE_PLOT infoPlot;
    infoPlot.eProtocol = e2l_complete_plot;
    infoPlot.missionId = info.missionId;
    infoPlot.value = 0;
    ClientLogic::instance()->ProcessUIRequest(&infoPlot);
}

void ActorLayer::removeNPC(cocos2d::EventCustom *event)
{
    L2E_REMOVE_NPC info = *static_cast<L2E_REMOVE_NPC*>(event->getUserData());
    for (auto iter = npcVec.begin(); iter != npcVec.end(); ) {
        if ((*iter)->getNpcId() == info.npcId) {
            (*iter)->removeFromParent();
            iter = npcVec.erase(iter);
            E2L_COMPLETE_PLOT infoPlot;
            infoPlot.eProtocol = e2l_complete_plot;
            infoPlot.missionId = info.missionId;
            infoPlot.value = 0;
            ClientLogic::instance()->ProcessUIRequest(&infoPlot);
            break;
        }else{
            iter++;
        }
    }
    
    
}
void ActorLayer::setupOpponent(cocos2d::EventCustom *event)
{
    L2E_SETUP_OPPONENT info = *static_cast<L2E_SETUP_OPPONENT*>(event->getUserData());
    oppRobot.jobId = info.jobId;
    oppRobot.fighterId = info.fighterId;
    oppRobot.name = info.name;
    oppRobot.hp = info.hp;
    oppRobot.attack = info.attack;
    oppRobot.defence = info.defence;
    oppRobot.crit = info.crit;
    oppRobot.decrit = info.decrit;
    oppRobot.holySwordId = info.holySwordId;
    memcpy(oppRobot.petId, info.petId, sizeof(int)*3);
}
void ActorLayer::addMonster(int monId, int teamId, float x, float y, int buffId[5], bool opponentFlag)
{
    auto monster = Monster::create();
    addChild(monster);
    monster->setOpponentFlag(opponentFlag);
    if (opponentFlag) {
        monster->initOppRobot(oppRobot);
    }
    monster->setTeamId(teamId);
    monster->setupData(monId);
    if (opponentFlag) {
        EventCustom event(SHOW_OPPONENT_HP);
        SHOW_OPPONENT_HP_DATA data;
        data.name = monster->getBlackActor()->getName();
        data.job = monster->getBlackActor()->getJob();
        data.currAp = monster->getBlackActor()->getAp();
        data.currHp = monster->getBlackActor()->getHp();
        data.maxHp = monster->getBlackActor()->getMaxHp();
        event.setUserData(&data);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    }
//    monster->initFightData(monId);
//    monster->initViewRes();
//    monster->initFightState();
    monster->addHurtListener();
//    auto scene = dynamic_cast<GameScene *>(Director::getInstance()->getRunningScene());
    
    
    monster->setAnchorPoint(Vec2(0.5, 0));
    monster->setPosX(x);
    monster->setPosY(y);
    monster->setPosition(Vec2(x, y));
    
    auto p1 = Vec2(1700, y);
    auto p2 = Vec2(1000, y);
//    monster->addPartolPoint(p1, p2);
    std::vector<Point> line;
    line.push_back(p1);
    line.push_back(p2);
    monster->setPartolPosVec(line);
//    addChild(monster);
    monster->changeState(ACTORSTATE::INIT);
    //    changeAction(HEROACTION::STAND);
    monster->changeActionWithName(HEROACTION::BORN);
    if (x > hero->getPosX()) {
//        monster->setDir(DIRECT::RIGHT);
        monster->getArmature()->setScaleX(-1);
//    }else{
//        monster->setDir(DIRECT::LEFT);
    }
    
    
    monsterVec.pushBack(monster);
    
    addActorHitEffect(monster);
    monsterView2Hero(monster);
    
    monster->preloadSkillRes();
    for (int i = 0; i < 5; i++) {
        if (buffId[i] <= 0) {
            continue;
        }
        
        monster->addSingleBUff(true, buffId[i], 100);
    }
    
    E2L_TRIGGER_CREATE_MONSTER info;
    info.eProtocol = e2l_trigger_create_monster;
    info.monsterId = monId;
    ClientLogic::instance()->ProcessUIRequest(&info);
}

void ActorLayer::removeBullet(Bullet *bullet)
{
    
    if (bullet->getOwnerType() == 1) {
        auto bulletItr = heroBullets.find(bullet);
        if (bulletItr != heroBullets.end()) {
            heroBullets.erase(bulletItr);
        }
    }else{
        auto bulletItr = monsterBullets.find(bullet);
        if (bulletItr != monsterBullets.end()) {
            monsterBullets.erase(bulletItr);
        }
    }
    removeChild(bullet);
    
}

void ActorLayer::removeMonster(Monster *actor)
{
    actor->addAllGhost2Guard();
//    actor->getEventDispatcher()->removeEventListenersForTarget(actor);
//    if (typeid(*actor)==typeid(Monster)) {
        monsterVec.erase(monsterVec.find(actor));
//        actor->getHurtEffectLayer()->setVisible(false);
//        actor->setVisible(false);
//        hitEffeLayer->removeChild(actor->getHurtEffectLayer());
//        actor->setVisible(false);
        actor->buffManager->removeAllBuff();
        removeChild(actor);
    
//    }
}

void ActorLayer::addActorHitEffect(FightActor *actor)
{
    auto effLayer = Layer::create();
    effLayer->setPosition(actor->getPosition());
    actor->setHurtEffectLayer(effLayer);
    
    hitEffeLayer->addChild(effLayer);
}

void ActorLayer::fillHitEffect()
{
    hitEffeLayer = HitEffectLayer::create();
    size_t size = getChildrenCount();
    auto actors = getChildren();
    for (int i = 0; i < size; i++) {
        FightActor *actor = dynamic_cast<FightActor *>(actors.at(i));
		if (actor == NULL)
			continue;

        auto effLayer = Layer::create();
        effLayer->setPosition(actor->getPosition());
        actor->setHurtEffectLayer(effLayer);
        
        hitEffeLayer->addChild(effLayer);
    }
    addChild(hitEffeLayer,100);
    
    ghostLayer = Layer::create();
    addChild(ghostLayer);
}
void ActorLayer::setPlotPause(bool val)
{
    plotPause = val;
    size_t size = monsterVec.size();
    for (int i = 0; i < size; i++) {
        auto monster = dynamic_cast<Monster *>(monsterVec.at(i));
        monster->setPlotPause(plotPause);
    }
    hero->setPlotPause(plotPause);
}
void ActorLayer::update(float dt)
{
    if (pause) {
        return;
    }
    if (plotPause) {
        
        return;
    }
    size_t size = monsterVec.size();
//    auto monsters = monsterVec;
    for(int i = size - 1; i >= 0;i--)
    {
        auto obj = monsterVec.at(i);
//        if (typeid(*obj) == typeid(HitEffectLayer)) {
//            continue;
//        }
        auto actor = dynamic_cast<Monster *>(obj);
        if(actor->getDieLie())
        {
            if (actor == showMonster) {
                showMonster = nullptr;
            }
        }
        if (actor->getDying()) {
            removeMonster(actor);
        }
    }
    size = npcVec.size();
    for (int i = 0; i < size; i++) {
        auto npc = npcVec.at(i);
        npc->passNpc(hero->getPositionX());
    }
    size = monsterBullets.size();
    for (int i = size - 1; i >= 0; i--) {
        auto bullet = monsterBullets.at(i);
        if (!bullet->getAlive()) {
            removeBullet(bullet);
            continue;
        }
//        if(bullet->getBlackBullet()->getBulletType() == 1){
//            float cameraPosX = YNCamera::getInstance()->getPositionX();
//            if (bullet->isOutsideCamera(cameraPosX))
//            {
//                removeBullet(bullet);
//            }
//            //            if (bullet->getPositionX()<-50 || bullet->getPositionX()>MAP_AREA_WIDTH+50) {
//            //                removeBullet(bullet);
//            //                continue;
//            //            }
//            //
//            //
//            //            if (abs(cameraPosX - bullet->getPositionX()) < 530) {
//            //                continue;
//            //            }
//            //
//            //            if (cameraPosX < 480) {
//            //                if (bullet->getPositionX() - cameraPosX > 960-cameraPosX+50 ) {
//            //                    removeBullet(bullet);
//            //                }
//            //            }else if(cameraPosX > MAP_AREA_WIDTH - 480) {
//            //                if (cameraPosX - bullet->getPositionX() > 960 + 50 - (MAP_AREA_WIDTH - cameraPosX)) {
//            //                    removeBullet(bullet);
//            //                }
//            //            }            
//        }else if(bullet->getBlackBullet()->getBulletType() == 2){
//            if(bullet->isAnimationOver())
//            {
//                removeBullet(bullet);
//            }
//        }
    }
    size = heroBullets.size();
    for (int i = size - 1; i >= 0; i--) {
        auto bullet = heroBullets.at(i);
        if (!bullet->getAlive()) {
            removeBullet(bullet);
            continue;
        }
//        if(bullet->getBlackBullet()->getBulletType() == 1){
//            float cameraPosX = YNCamera::getInstance()->getPositionX();
//            if (bullet->isOutsideCamera(cameraPosX))
//            {
//                removeBullet(bullet);
//            }
////            if (bullet->getPositionX()<-50 || bullet->getPositionX()>MAP_AREA_WIDTH+50) {
////                removeBullet(bullet);
////                continue;
////            }
////            
////            
////            if (abs(cameraPosX - bullet->getPositionX()) < 530) {
////                continue;
////            }
////            
////            if (cameraPosX < 480) {
////                if (bullet->getPositionX() - cameraPosX > 960-cameraPosX+50 ) {
////                    removeBullet(bullet);
////                }
////            }else if(cameraPosX > MAP_AREA_WIDTH - 480) {
////                if (cameraPosX - bullet->getPositionX() > 960 + 50 - (MAP_AREA_WIDTH - cameraPosX)) {
////                    removeBullet(bullet);
////                }
////            }            
//        }else if(bullet->getBlackBullet()->getBulletType() == 2){
//            
//            
//            if(bullet->isAnimationOver())
//            {
//                removeBullet(bullet);
//            }
//        }

        
    }
//    actors.clear();
//    size = getChildrenCount();
//    auto actors = getChildren();
//    for (int i = 0; i < size; i++) {
//        actors.at(i)->setLocalZOrder(240 - actors.at(i)->getPositionY());
//    }
    size = monsterVec.size();
    for (int i = 0; i < size; i++) {
        auto monster = dynamic_cast<Monster *>(monsterVec.at(i));
//        monster->setPlotPause(plotPause);
        monster->logic(0);
        monster->adjustHurtEffectPos();
        monster->skillCDClock(dt);
    }
    size = gearVec.size();
    for (int i = 0; i < size; i++) {
        auto gear = dynamic_cast<Gear *>(gearVec.at(i));
        gear->logic(0);
        gear->skillCDClock(dt);
    }
//    hero->setPlotPause(plotPause);
    if (hero->Guard != nullptr){
        hero->Guard->logic(0);
    }
    if (hero->colossus != nullptr) {
        hero->colossus->logic(0);
    }
    
    
    size = heroBullets.size();
    for (int i = 0; i < size; i++) {
        auto bullet = dynamic_cast<Bullet *>(heroBullets.at(i));

        bullet->Logic(0);
        bool hitSomeOne = false;
        FightActor *hurtMonster = nullptr;
        for (auto mon : monsterVec) {
            if (mon->getIsInvincible())
            {
                continue;
            }
            if (abs(bullet->getPositionX() - mon->getPositionX()) < 500){
                auto bRect = mon->getBodyRect();
                auto armature = mon->getArmature();
                auto armatureY = armature->getPositionY();
                bRect.origin += Vec2(mon->getPositionX(), armatureY + mon->getPositionY());
                if (bullet->isCollide(bRect)) {
                    hurtMonster = mon;
                    EventCustom event(MONSTER_HURT_TAG);
                    fightPair pair;
                    pair.attacker = bullet;
                    pair.defender = mon;
                    event.setUserData(&pair);
                    mon->getEventDispatcher()->dispatchEvent(&event);

                    hitSomeOne = true;
//                    break;
                }
            }
        }
    }
    
    size = monsterBullets.size();
    for (int i = 0; i < size; i++) {
        auto bullet = dynamic_cast<Bullet *>(monsterBullets.at(i));
        
        bullet->Logic(0);
        if (!hero->getIsInvincible())
        {
            if (abs(bullet->getPositionX() - hero->getPositionX()) < 500)
            {
                auto bRect = hero->getBodyRect();
                auto armature = hero->getArmature();
                auto armatureY = armature->getPositionY();
                bRect.origin += Vec2(hero->getPositionX(), armatureY + hero->getPositionY());
                if (bullet->isCollide(bRect))
                {
                    EventCustom event(HERO_HURT_TAG);
                    fightPair pair;
                    pair.attacker = bullet;
                    pair.defender = hero;
                    event.setUserData(&pair);
                    hero->getEventDispatcher()->dispatchEvent(&event);
                }
            }
        }
        if (witness == nullptr) {
            continue;
        }
        if (!witness->getIsInvincible())
        {
            if (abs(bullet->getPositionX() - witness->getPositionX()) < 500)
            {
                auto bRect = witness->getBodyRect();
                auto armature = witness->getArmature();
                auto armatureY = armature->getPositionY();
                bRect.origin += Vec2(witness->getPositionX(), armatureY + witness->getPositionY());
                if (bullet->isCollide(bRect))
                {
                    EventCustom event(WITNESS_HURT_TAG);
                    fightPair pair;
                    pair.attacker = bullet;
                    pair.defender = witness;
                    event.setUserData(&pair);
                    witness->getEventDispatcher()->dispatchEvent(&event);
                }
            }
        }
    }
    size = monsterVec.size();
    for (int i = 0; i<size; i++) {
        auto mon = dynamic_cast<Monster *>(monsterVec.at(i));
        if(mon->getAtking())
        {
            Rect rAttackRect = mon->getCurrSkillTag()->getAttackRect();
            float maxDis = rAttackRect.origin.x + rAttackRect.size.width;
            if (mon->getFlipX()) {
                rAttackRect.origin.x += rAttackRect.size.width;
                rAttackRect.origin.x *= -1;
            }
            float thick = mon->getCurrSkillTag()->getAtkAreaThick();
            bool  bMonsterFlipX = mon->getFlipX();
//                mon->DealAttackRectByFlip(bMonsterFlipX, &rAttackRect);
            float boneY = 0, armatureY = 0;
            auto armature = mon->getArmature();
            armatureY = armature->getPositionY();
            rAttackRect.origin += Vec2(mon->getPositionX(), armatureY + boneY + mon->getPositionY());
            if (!hero->getIsInvincible()) {
                auto bRect = hero->getBodyRect();

                if (mon->getCurrSkillTag()->getFullScreenRange() == 1)
                {
                    EventCustom event(HERO_HURT_TAG);
                    fightPair pair;
                    pair.attacker = mon;
                    pair.defender = hero;
                    event.setUserData(&pair);
                    hero->getEventDispatcher()->dispatchEvent(&event);
                    mon->setIsValidAtk(true);
                }else{
                    if (abs(mon->getPositionY() - hero->getPositionY()) < thick && abs(mon->getPositionX() - hero->getPositionX()) < abs(maxDis)+bRect.size.width/2)
                    {
                        
                        armature = hero->getArmature();
                        armatureY = armature->getPositionY();
                        bRect.origin += Vec2(hero->getPositionX(), armatureY+boneY+hero->getPositionY());
                        if (mon->getCurrSkillTag()->getFullScreenRange() == 0) {
                            if (bRect.intersectsRect(rAttackRect)) {
                                EventCustom event(HERO_HURT_TAG);
                                fightPair pair;
                                pair.attacker = mon;
                                pair.defender = hero;
                                event.setUserData(&pair);
                                hero->getEventDispatcher()->dispatchEvent(&event);
                                mon->setIsValidAtk(true);
                            }                        
                        } else if (mon->getCurrSkillTag()->getFullScreenRange() == 2) {
                            if ((mon->getFlipX() && hero->getPositionX() <= mon->getPositionX()) || (!mon->getFlipX() && hero->getPositionX() >= mon->getPositionX())) {
                                EventCustom event(HERO_HURT_TAG);
                                fightPair pair;
                                pair.attacker = mon;
                                pair.defender = hero;
                                event.setUserData(&pair);
                                hero->getEventDispatcher()->dispatchEvent(&event);
                                mon->setIsValidAtk(true);
                            }
                        }

                    }                    
                }

            }
            if (witness == nullptr) {
                continue;
            }
            if (!witness->getIsInvincible())
            {
                auto bRect = witness->getBodyRect();
                
                if (mon->getCurrSkillTag()->getFullScreenRange() == 1)
                {
                    EventCustom event(WITNESS_HURT_TAG);
                    fightPair pair;
                    pair.attacker = mon;
                    pair.defender = witness;
                    event.setUserData(&pair);
                    witness->getEventDispatcher()->dispatchEvent(&event);
                    mon->setIsValidAtk(true);
                }else{
                    if (abs(mon->getPositionY() - witness->getPositionY()) < thick && abs(mon->getPositionX() - witness->getPositionX()) < abs(maxDis)+bRect.size.width/2)
                    {
                        
                        armature = witness->getArmature();
                        armatureY = armature->getPositionY();
                        bRect.origin += Vec2(witness->getPositionX(), armatureY+boneY+witness->getPositionY());
                        if (mon->getCurrSkillTag()->getFullScreenRange() == 0) {
                            if (bRect.intersectsRect(rAttackRect)) {
                                EventCustom event(WITNESS_HURT_TAG);
                                fightPair pair;
                                pair.attacker = mon;
                                pair.defender = witness;
                                event.setUserData(&pair);
                                witness->getEventDispatcher()->dispatchEvent(&event);
                                mon->setIsValidAtk(true);
                            }
                        } else if (mon->getCurrSkillTag()->getFullScreenRange() == 2) {
                            if ((mon->getFlipX() && witness->getPositionX() <= mon->getPositionX()) || (!mon->getFlipX() && witness->getPositionX() >= mon->getPositionX())) {
                                EventCustom event(WITNESS_HURT_TAG);
                                fightPair pair;
                                pair.attacker = mon;
                                pair.defender = witness;
                                event.setUserData(&pair);
                                witness->getEventDispatcher()->dispatchEvent(&event);
                                mon->setIsValidAtk(true);
                            }
                        }
                        
                    }
                }
                
            }
        }
    }
    
    size = gearVec.size();
    if (!hero->getIsInvincible()) {
        for (int i = 0; i<size; i++) {
            auto gear = dynamic_cast<Gear *>(gearVec.at(i));
            if(gear->getAtking())
            {
                Rect rAttackRect = gear->getCurrSkillTag()->getAttackRect();
                if (gear->getFlipX()) {
                    rAttackRect.origin.x += rAttackRect.size.width;
                }
                float thick = gear->getCurrSkillTag()->getAtkAreaThick();
                bool  bGearsterFlipX = gear->getFlipX();
//                gear->DealAttackRectByFlip(bGearsterFlipX, &rAttackRect);
                float boneY = 0, armatureY = 0;
                auto armature = gear->getArmature();
                armatureY = armature->getPositionY();
                rAttackRect.origin += Vec2(gear->getPositionX(), armatureY + boneY + gear->getPositionY());

                if (abs(gear->getPositionY() - hero->getPositionY()) < thick && abs(gear->getPositionX() - hero->getPositionX()) < rAttackRect.size.width)
                {
                    auto bRect = hero->getBodyRect();
                    armature = hero->getArmature();
                    armatureY = armature->getPositionY();
                    bRect.origin += Vec2(hero->getPositionX(), armatureY+boneY+hero->getPositionY());
                    if (bRect.intersectsRect(rAttackRect)) {
                        EventCustom event(HERO_HURT_TAG);
                        fightPair pair;
                        pair.attacker = gear;
                        pair.defender = hero;
                        event.setUserData(&pair);
                        hero->getEventDispatcher()->dispatchEvent(&event);
                        gear->setIsValidAtk(true);
                    }
                }
            }
        }
    }
    if(hero->getAtking())
	{
		Rect rAttackRect = hero->getCurrSkillTag()->getAttackRect();
        if (hero->getFlipX()) {
            rAttackRect.origin.x = (-rAttackRect.origin.x-rAttackRect.size.width);
        }
        float thick = hero->getCurrSkillTag()->getAtkAreaThick();
		bool  bMonsterFlipX = hero->getFlipX();
//		hero->DealAttackRectByFlip(bMonsterFlipX, &rAttackRect);
        float boneY = 0, armatureY = 0;
        auto armature = hero->getArmature();
        armatureY = armature->getPositionY();
		rAttackRect.origin += Vec2(hero->getPositionX(), armatureY + boneY + hero->getPositionY());
        
        size = monsterVec.size();
        hurtMonsters.clear();
        FightActor *monster;
        FightActor *nearstMon = nullptr;
        float nearstDis = Director::getInstance()->getWinSize().width;
        for (int i = 0; i<size; i++) {
            monster = monsterVec.at(i);
            if (monster->getIsInvincible()) {
                continue;
            }
            if (hero->getCurrSkillTag()->getFullScreenRange() == 1) {
                hurtMonsters.pushBack(monster);
                EventCustom event(MONSTER_HURT_TAG);
                fightPair pair;
                pair.attacker = hero;
                pair.defender = monster;
                event.setUserData(&pair);
                monster->getEventDispatcher()->dispatchEvent(&event);
                hero->setIsValidAtk(true);
                continue;
            }
            if (hero->getCurrSkillTag()->getFullScreenRange() == 2) {
                if (fabs(hero->getPositionX() - monster->getPositionX()) < Director::getInstance()->getWinSize().width) {
                    if ((hero->getFlipX() && (hero->getPositionX() >= monster->getPositionX())) ||
                        (!hero->getFlipX() && (hero->getPositionX() <= monster->getPositionX()))) {
                        if (fabs(hero->getPositionX() - monster->getPositionX()) < nearstDis) {
                            nearstDis = fabs(hero->getPositionX() - monster->getPositionX());
                            nearstMon = monster;
//                            hurtMonsters.pushBack(monster);
//                            EventCustom event(MONSTER_HURT_TAG);
//                            fightPair pair;
//                            pair.attacker = hero;
//                            pair.defender = monster;
//                            event.setUserData(&pair);
//                            monster->getEventDispatcher()->dispatchEvent(&event);
//                            hero->setIsValidAtk(true);
                        }
                    }
                }
                continue;
            }
            if (fabs(hero->getPositionY() - monster->getPositionY()) < thick && fabs(hero->getPositionX() - monster->getPositionX()) < rAttackRect.size.width) {
                auto bRect = monster->getBodyRect();
                armature = monster->getArmature();
                armatureY = armature->getPositionY();
                bRect.origin += Vec2(monster->getPositionX(), armatureY+boneY+monster->getPositionY());
                if (hero->getCurrSkillTag()->getFullScreenRange() == 0) {
                    if (bRect.intersectsRect(rAttackRect)) {
                        hurtMonsters.pushBack(monster);
                        EventCustom event(MONSTER_HURT_TAG);
                        fightPair pair;
                        pair.attacker = hero;
                        pair.defender = monster;
                        event.setUserData(&pair);
                        monster->getEventDispatcher()->dispatchEvent(&event);
                        hero->setIsValidAtk(true);
                    }
                }
            }
        }
        if (hero->getCurrSkillTag()->getFullScreenRange() == 2 && nearstMon != nullptr) {
            hurtMonsters.pushBack(nearstMon);
            EventCustom event(MONSTER_HURT_TAG);
            fightPair pair;
            pair.attacker = hero;
            pair.defender = nearstMon;
            event.setUserData(&pair);
            nearstMon->getEventDispatcher()->dispatchEvent(&event);
            hero->setIsValidAtk(true);

            for (int i = 0; i<size; i++) {
                monster = monsterVec.at(i);
                if (monster == nearstMon) {
                    continue;
                }
                if (fabs(hero->getPositionX() - monster->getPositionX()) < Director::getInstance()->getWinSize().width) {
                    if ((hero->getFlipX() && (hero->getPositionX() >= monster->getPositionX())) ||
                        (!hero->getFlipX() && (hero->getPositionX() <= monster->getPositionX()))) {
                        float dis = fabs(nearstMon->getPositionX() - monster->getPositionX());
                        if (dis <= hero->getCurrSkillTag()->getAttackRect().size.width) {
                            hurtMonsters.pushBack(monster);
                            EventCustom event(MONSTER_HURT_TAG);
                            fightPair pair;
                            pair.attacker = hero;
                            pair.defender = monster;
                            event.setUserData(&pair);
                            monster->getEventDispatcher()->dispatchEvent(&event);
                            hero->setIsValidAtk(true);
                        }
                    }
                }
            }
        }

        
        if (hurtMonsters.empty()) {
            //消失血条
            showMonster = nullptr;
        } else {
            if (showMonster != nullptr) {
                if(hurtMonsters.find(showMonster) != hurtMonsters.end()){
                    showMonster = hurtMonsters.at(0);
                }
            }else{
                showMonster = hurtMonsters.at(0);
            }
        }
    }
    
    auto guard = hero->Guard;
    if (guard !=nullptr) {
        if(guard->getAtking())
        {
            Rect rAttackRect = guard->getCurrSkillTag()->getAttackRect();
            float thick = guard->getCurrSkillTag()->getAtkAreaThick();
            bool  bMonsterFlipX = guard->getFlipX();
//            guard->DealAttackRectByFlip(bMonsterFlipX, &rAttackRect);
            float boneY = 0, armatureY = 0;
            auto armature = guard->getArmature();
            armatureY = armature->getPositionY();
            rAttackRect.origin += Vec2(guard->getPositionX(), armatureY + boneY + guard->getPositionY());
            
            size = monsterVec.size();
            hurtMonsters.clear();
            FightActor *monster;
            FightActor *nearstMon = nullptr;
            float nearstDis = Director::getInstance()->getWinSize().width;
            for (int i = 0; i<size; i++) {
                monster = monsterVec.at(i);
                if (monster->getIsInvincible()) {
                    continue;
                }
                if (guard->getCurrSkillTag()->getFullScreenRange() == 1) {
                    hurtMonsters.pushBack(monster);
                    EventCustom event(MONSTER_HURT_TAG);
                    fightPair pair;
                    pair.attacker = hero->Guard;
                    pair.defender = monster;
                    event.setUserData(&pair);
                    monster->getEventDispatcher()->dispatchEvent(&event);
                    guard->setIsValidAtk(true);
                    continue;
                }
                if (guard->getCurrSkillTag()->getFullScreenRange() == 2) {
                    if (fabs(guard->getPositionX() - monster->getPositionX()) < Director::getInstance()->getWinSize().width) {
                        if ((guard->getFlipX() && (guard->getPositionX() >= monster->getPositionX())) ||
                            (!guard->getFlipX() && (guard->getPositionX() <= monster->getPositionX()))) {
                            if (fabs(guard->getPositionX() - monster->getPositionX()) < nearstDis) {
                                nearstDis = fabs(guard->getPositionX() - monster->getPositionX());
                                nearstMon = monster;
                                //                            hurtMonsters.pushBack(monster);
                                //                            EventCustom event(MONSTER_HURT_TAG);
                                //                            fightPair pair;
                                //                            pair.attacker = hero;
                                //                            pair.defender = monster;
                                //                            event.setUserData(&pair);
                                //                            monster->getEventDispatcher()->dispatchEvent(&event);
                                //                            hero->setIsValidAtk(true);
                            }
                        }
                    }
                    continue;
                }
                if (fabs(guard->getPositionY() - monster->getPositionY()) < thick && fabs(guard->getPositionX() - monster->getPositionX()) < rAttackRect.size.width) {
                    auto bRect = monster->getBodyRect();
                    armature = monster->getArmature();
                    armatureY = armature->getPositionY();
                    bRect.origin += Vec2(monster->getPositionX(), armatureY+boneY+monster->getPositionY());
                    if (guard->getCurrSkillTag()->getFullScreenRange() == 0) {
                        if (bRect.intersectsRect(rAttackRect)) {
                            hurtMonsters.pushBack(monster);
                            EventCustom event(MONSTER_HURT_TAG);
                            fightPair pair;
                            pair.attacker = hero->Guard;
                            pair.defender = monster;
                            event.setUserData(&pair);
                            monster->getEventDispatcher()->dispatchEvent(&event);
                            guard->setIsValidAtk(true);
                        }
                    }
                }
            }
            if (guard->getCurrSkillTag()->getFullScreenRange() == 2 && nearstMon != nullptr) {
                hurtMonsters.pushBack(nearstMon);
                EventCustom event(MONSTER_HURT_TAG);
                fightPair pair;
                pair.attacker = hero->Guard;
                pair.defender = nearstMon;
                event.setUserData(&pair);
                nearstMon->getEventDispatcher()->dispatchEvent(&event);
                guard->setIsValidAtk(true);
                
                for (int i = 0; i<size; i++) {
                    monster = monsterVec.at(i);
                    if (monster == nearstMon) {
                        continue;
                    }
                    if (fabs(guard->getPositionX() - monster->getPositionX()) < Director::getInstance()->getWinSize().width) {
                        if ((guard->getFlipX() && (guard->getPositionX() >= monster->getPositionX())) ||
                            (!guard->getFlipX() && (guard->getPositionX() <= monster->getPositionX()))) {
                            float dis = fabs(nearstMon->getPositionX() - monster->getPositionX());
                            if (dis <= guard->getCurrSkillTag()->getAttackRect().size.width) {
                                hurtMonsters.pushBack(monster);
                                EventCustom event(MONSTER_HURT_TAG);
                                fightPair pair;
                                pair.attacker = hero->Guard;
                                pair.defender = monster;
                                event.setUserData(&pair);
                                monster->getEventDispatcher()->dispatchEvent(&event);
                                guard->setIsValidAtk(true);
                            }
                        }
                    }
                }
            }
        }
    }
    
    auto col = hero->colossus;
    if (col != nullptr) {
        if(col->getAtking())
        {
            Rect rAttackRect = col->getCurrSkillTag()->getAttackRect();
            float thick = col->getCurrSkillTag()->getAtkAreaThick();
            bool  bMonsterFlipX = col->getFlipX();
//            col->DealAttackRectByFlip(bMonsterFlipX, &rAttackRect);
            float boneY = 0, armatureY = 0;
            auto armature = col->getArmature();
            armatureY = armature->getPositionY();
            rAttackRect.origin += Vec2(col->getPositionX(), armatureY + boneY + col->getPositionY());
            
            size = monsterVec.size();
            hurtMonsters.clear();
            FightActor *monster;
            FightActor *nearstMon = nullptr;
            float nearstDis = Director::getInstance()->getWinSize().width;
            for (int i = 0; i<size; i++) {
                monster = monsterVec.at(i);
                if (monster->getIsInvincible()) {
                    continue;
                }
                if (col->getCurrSkillTag()->getFullScreenRange() == 1) {
                    hurtMonsters.pushBack(monster);
                    EventCustom event(MONSTER_HURT_TAG);
                    fightPair pair;
                    pair.attacker = hero->colossus;
                    pair.defender = monster;
                    event.setUserData(&pair);
                    monster->getEventDispatcher()->dispatchEvent(&event);
                    col->setIsValidAtk(true);
                    continue;
                }
                if (col->getCurrSkillTag()->getFullScreenRange() == 2) {
                    if (fabs(col->getPositionX() - monster->getPositionX()) < Director::getInstance()->getWinSize().width) {
                        if ((col->getFlipX() && (col->getPositionX() >= monster->getPositionX())) ||
                            (!col->getFlipX() && (col->getPositionX() <= monster->getPositionX()))) {
                            if (fabs(col->getPositionX() - monster->getPositionX()) < nearstDis) {
                                nearstDis = fabs(col->getPositionX() - monster->getPositionX());
                                nearstMon = monster;
                                //                            hurtMonsters.pushBack(monster);
                                //                            EventCustom event(MONSTER_HURT_TAG);
                                //                            fightPair pair;
                                //                            pair.attacker = hero;
                                //                            pair.defender = monster;
                                //                            event.setUserData(&pair);
                                //                            monster->getEventDispatcher()->dispatchEvent(&event);
                                //                            hero->setIsValidAtk(true);
                            }
                        }
                    }
                    continue;
                }
                if (fabs(col->getPositionY() - monster->getPositionY()) < thick && fabs(col->getPositionX() - monster->getPositionX()) < rAttackRect.size.width) {
                    auto bRect = monster->getBodyRect();
                    armature = monster->getArmature();
                    armatureY = armature->getPositionY();
                    bRect.origin += Vec2(monster->getPositionX(), armatureY+boneY+monster->getPositionY());
                    if (col->getCurrSkillTag()->getFullScreenRange() == 0) {
                        if (bRect.intersectsRect(rAttackRect)) {
                            hurtMonsters.pushBack(monster);
                            EventCustom event(MONSTER_HURT_TAG);
                            fightPair pair;
                            pair.attacker = hero->colossus;
                            pair.defender = monster;
                            event.setUserData(&pair);
                            monster->getEventDispatcher()->dispatchEvent(&event);
                            col->setIsValidAtk(true);
                        }
                    }
                }
            }
            if (col->getCurrSkillTag()->getFullScreenRange() == 2 && nearstMon != nullptr) {
                hurtMonsters.pushBack(nearstMon);
                EventCustom event(MONSTER_HURT_TAG);
                fightPair pair;
                pair.attacker = hero->colossus;
                pair.defender = nearstMon;
                event.setUserData(&pair);
                nearstMon->getEventDispatcher()->dispatchEvent(&event);
                col->setIsValidAtk(true);
                
                for (int i = 0; i<size; i++) {
                    monster = monsterVec.at(i);
                    if (monster == nearstMon) {
                        continue;
                    }
                    if (fabs(col->getPositionX() - monster->getPositionX()) < Director::getInstance()->getWinSize().width) {
                        if ((col->getFlipX() && (col->getPositionX() >= monster->getPositionX())) ||
                            (!col->getFlipX() && (col->getPositionX() <= monster->getPositionX()))) {
                            float dis = fabs(nearstMon->getPositionX() - monster->getPositionX());
                            if (dis <= col->getCurrSkillTag()->getAttackRect().size.width) {
                                hurtMonsters.pushBack(monster);
                                EventCustom event(MONSTER_HURT_TAG);
                                fightPair pair;
                                pair.attacker = hero->colossus;
                                pair.defender = monster;
                                event.setUserData(&pair);
                                monster->getEventDispatcher()->dispatchEvent(&event);
                                col->setIsValidAtk(true);
                            }
                        }
                    }
                }
            }
        }
    }
    
}

void ActorLayer::setHero(Hero *hero)
{
    this->hero = hero;
    heroVec.pushBack(hero);
//    size_t size = monsterVec.size();
//    for (int i = 0; i < size; i++) {
//        dynamic_cast<Monster *>(monsterVec.at(i))->setEnemiesVec(heroVec);
//    }
}

void ActorLayer::setWitness(ProtectNPC *witness)
{
    this->witness = witness;
    witnessVec.pushBack(witness);
}
void ActorLayer::monsterView2Hero(FightActor *monster)
{
    if (!witnessVec.empty()) {
        monster->setEnemiesVec(witnessVec);
    }else{
        monster->setEnemiesVec(heroVec);
    }
    
}

void ActorLayer::onExit()
{
    FightActor::removeHurtListener();
    Layer::onExit();
}

std::vector<FightActor *> ActorLayer::findEnemies(FightActor *man)
{
    std::vector<FightActor *> result;
    
    auto actors = heroVec;
    if (!witnessVec.empty()) {
        actors = witnessVec;
    }
    if (typeid(*man) ==  typeid(Hero)) {
        actors = monsterVec;
    }
    auto size = actors.size();
    // 取消可视区域，整地图都可见
//    Size viewSize = Director::getInstance()->getWinSize();
//    viewSize.height = 10000;
    
//    auto viewRect = Rect(man->getPositionX()-viewSize.width/2, 0, viewSize.width, viewSize.height);
    for (size_t i=0; i<size; i++) {
        FightActor *actor = actors.at(i);
        if (actor->getDying() || actor->getDieLie()) {
            continue;
        }
//        if (viewRect.containsPoint(actor->getPosition())) {
            result.push_back(actor);
//        }
    }
    return result;
}

void ActorLayer::sendMonsterTeam(cocos2d::EventCustom *event)
{
    L2E_SETUP_MONSTER_TEAM info = *static_cast<L2E_SETUP_MONSTER_TEAM*>(event->getUserData());
    for(auto mon : info.team)
    {
        addMonster(mon.monId, mon.teamId, mon.x, GroundY, mon.buffId, mon.opponentFlag);
    }
}

FightActor* ActorLayer::getMonsterWithTeamId(int teamId)
{
    for(auto mon : monsterVec)
    {
        if (((Monster*)mon)->getTeamId() == teamId) {
            return mon;
        }
    }
    return nullptr;
}

void ActorLayer::changeNPCAction(int npcId, int actionId, bool flipX, int missionId)
{
    for (auto npc : npcVec)
    {
        if (npc->getNpcId() != npcId) {
            continue;
        }
        
        npc->changeAction(actionId, flipX, missionId);
        break;
    }
}

void ActorLayer::moveNPC(int npcId, float duration, int targetX, int missionId)
{
    for (auto npc : npcVec)
    {
        if (npc->getNpcId() != npcId) {
            continue;
        }
        
        npc->plotMove(duration, targetX, missionId);
        break;
    }
}

void ActorLayer::changeMonsterAction(int monsterId, int actionId, bool flipX, int missionId)
{
    for (auto actor : monsterVec)
    {
        if (actor->getBlackActor()->getActorID() != monsterId) {
            continue;
        }
        
        ((Monster*)actor)->plotChangeAction(actionId, flipX, missionId);
        break;
    }
}

void ActorLayer::moveMonster(int monsterId, float duration, int targetX, int missionId)
{
    for (auto actor : monsterVec)
    {
        if (actor->getBlackActor()->getActorID() != monsterId) {
            continue;
        }
        
        ((Monster*)actor)->plotMove(duration, targetX, missionId);
        break;
    }
}
