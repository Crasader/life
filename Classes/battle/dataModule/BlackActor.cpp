//
//  BlackActor.cpp
//  ssoul
//
//  Created by ff on 14-10-14.
//
//

#include "BlackActor.h"
#include "cocostudio/CocoStudio.h"
#include "utils/GameReader.h"
#include "GameDefine.h"
#include "BlackColossus.h"

USING_NS_CC;
using namespace cocostudio;

std::map<int, int> BlackActor::idTable;

BlackActor::BlackActor():
plotProtect(false)
{
    colossusId = guardId = 0;
    av = 0;
    ap = 0;
    safeDis = 0;
    buffAttack = 0;
    buffDefence = 0;
    holySwordSkillId = 0;
    memset(petSkillId, 0, sizeof(int)*3);
    isOpponent = false;
    selSkills.clear();
    selSkillTags.clear();
}

BlackActor::~BlackActor()
{
    selSkills.clear();
    selSkillTags.clear();
}

void BlackActor::initIdTable(rapidjson::Document &_doc){
    if (_doc.IsArray()){
        int size = _doc.Size();
        int objId;
        for (int i = 0; i<size; i++) {
            objId =DICTOOL->getIntValue_json(_doc[i], "ID");
            idTable.insert(std::make_pair(objId, i));
        }
    }
}

bool BlackActor::initData(int idx, rapidjson::Document& _doc) {
    int index = idTable.find(idx)->second;
    
    const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(_doc, index);
    if (dic.IsNull()) {
        return false;
    }
    
    
    actorId =DICTOOL->getIntValue_json(dic, "ID");
    name = DICTOOL->getStringValue_json(dic, "name");
    resName = DICTOOL->getStringValue_json(dic, "csb_file");
    scaleRate = DICTOOL->getFloatValue_json(dic, "scale_factor");
    moveSpeed = DICTOOL->getIntValue_json(dic, "move_speed");
    rushSpeed = DICTOOL->getFloatValue_json(dic, "rush_speed");
    maxHp = hp = DICTOOL->getFloatValue_json(dic, "hp");
    barCount = DICTOOL->getIntValue_json(dic, "bar_count");
    maxMp = mp = DICTOOL->getFloatValue_json(dic, "mp");
    attack = DICTOOL->getFloatValue_json(dic, "attack");
    defence = DICTOOL->getFloatValue_json(dic, "defence");
    hitRatio = DICTOOL->getFloatValue_json(dic, "hitRatio");
    dodgeRatio = DICTOOL->getFloatValue_json(dic, "dodgeRatio");
    critRatio = DICTOOL->getFloatValue_json(dic, "critRatio");
    tenacity = DICTOOL->getFloatValue_json(dic, "tenacity");
    iceResist = DICTOOL->getFloatValue_json(dic, "iceResist");
    fireResist = DICTOOL->getFloatValue_json(dic, "fireResist");
    thunderResist = DICTOOL->getFloatValue_json(dic, "thunderResist");
    windResist = DICTOOL->getFloatValue_json(dic, "windResist");
    lightResist = DICTOOL->getFloatValue_json(dic, "lightResist");
    darkResist = DICTOOL->getFloatValue_json(dic, "darkResist");
    
    level = DICTOOL->getFloatValue_json(dic, "level");
    job = DICTOOL->getIntValue_json(dic, "job");
    stateMachineFile = DICTOOL->getStringValue_json(dic, "state_machine");
    waitTime = DICTOOL->getFloatValue_json(dic, "wait_time");
    deHitDownVal = DICTOOL->getFloatValue_json(dic, "deHitDown");
    
    atk1skillId = DICTOOL->getFloatValue_json(dic, "atk1");
    atkskillId = atk1skillId;
    
    atk2skillId = DICTOOL->getIntValue_json(dic, "atk2");
    atk3skillId = DICTOOL->getFloatValue_json(dic, "atk3");
    atk4skillId = DICTOOL->getFloatValue_json(dic, "atk4");
    
    skillAId = DICTOOL->getIntValue_json(dic, "curr_skill_1");
    skillBId = DICTOOL->getIntValue_json(dic, "curr_skill_2");
    skillCId = DICTOOL->getIntValue_json(dic, "curr_skill_3");
    skillDId = DICTOOL->getIntValue_json(dic, "curr_skill_4");
    
    skillSPId = DICTOOL->getIntValue_json(dic, "SPSkill");
    runAttack1ID = DICTOOL->getIntValue_json(dic, "RunAtk1");
    runAttack2ID = DICTOOL->getIntValue_json(dic, "RunAtk2");
    runAttackID = runAttack1ID;

    de2AirRate = DICTOOL->getIntValue_json(dic, "body");
    deDownRate = DICTOOL->getIntValue_json(dic, "balance");
    immuBuffRate = DICTOOL->getIntValue_json(dic, "immuBuff");
    
    atkAThreshold = DICTOOL->getIntValue_json(dic, "atkAThreshold");
    hurtAThreshold = DICTOOL->getIntValue_json(dic, "hurtAThreshold");
    maxAV = DICTOOL->getIntValue_json(dic, "MaxAV");
    maxAp = DICTOOL->getIntValue_json(dic, "MaxAP");
    mpRecover = DICTOOL->getFloatValue_json(dic, "MpRecover");
    ghost = maxGhost = 3;
    return true;
}

void BlackActor::readFile(int idx, std::string filename)
{
    auto doc = GameReader::getDocInstance(filename);
    if(BlackActor::idTable.empty())
    {
        BlackActor::initIdTable(*doc);
    }
    if (isOpponent) {
        idx = robot.fighterId;
    }
    initData(idx, *doc);
    if (isOpponent) {
        setupDataWithRobot();
    }
}

void BlackActor::getActorRes(int idx, std::string filename, std::string &res)
{
    res = "";
    auto doc = GameReader::getDocInstance(filename);
    if(BlackActor::idTable.empty())
    {
        BlackActor::initIdTable(*doc);
    }
    
    int index = idTable.find(idx)->second;
    
    const rapidjson::Value &dic = DICTOOL->getSubDictionary_json(*doc, index);
    if (dic.IsNull()) {
        return;
    }
    
    res = DICTOOL->getStringValue_json(dic, "csb_file");
}

int atkIdx;
void BlackActor::initSelSkill()
{
    atkIdx = 1;
    petCount = 0;
    maxPetCount = 0;
//    readRelativeSkill(atkskillId);
    equipSkills.clear();
    equipSkills[ATK_KEY] = atk1skillId;
    equipSkills[SKILLA_KEY] = skillAId;
    equipSkills[SKILLB_KEY] = skillBId;
    equipSkills[SKILLC_KEY] = skillCId;
    equipSkills[SKILLD_KEY] = skillDId;
    equipSkills[SKILLS_KEY] = skillSPId;
    equipSkills[RUN_ATK_KEY] = runAttack1ID;
    equipSkills[HOLY_SKILL_KEY] = holySwordSkillId;
    equipSkills[PET_SKILL1_KEY] = petSkillId[0];
    equipSkills[PET_SKILL2_KEY] = petSkillId[1];
    equipSkills[PET_SKILL3_KEY] = petSkillId[2];
    
    readRelativeSkill(atk1skillId);
    readRelativeSkill(runAttack1ID);
    readRelativeSkill(runAttack2ID);
    readRelativeSkill(skillAId);
    readRelativeSkill(skillBId);
    readRelativeSkill(skillCId);
    readRelativeSkill(skillDId);
    readRelativeSkill(skillSPId);
    readRelativeSkill(holySwordSkillId);
    readRelativeSkill(petSkillId[0]);
    readRelativeSkill(petSkillId[1]);
    readRelativeSkill(petSkillId[2]);
}

void BlackActor::initClsSelSkill(BlackColossus *cls)
{
    auto skillMap = cls->clsSkills;
    int skillId = skillMap.begin()->second.getSkillId();
    equipSkills[SKILLA_KEY] = skillId;
    atk1skillId = 0;
    for (auto skill : skillMap) {
        int skillId = skill.second.getSkillId();
        readRelativeSkill(skillId);
    }
}

void BlackActor::readRelativeSkill(int skillId)
{
    if (skillId <= 0) {
        return;
    }
    auto tSkill = new Skill;
    tSkill->readFile(skillId, SKILL_CONFIG_FILE);
    if (tSkill->getSkillType() == 4) {
        petCount++;
        maxPetCount++;
    }
    selSkills[skillId] = tSkill;
    
    
    int linkId = tSkill->getLinkID();
    if (linkId > 0) {
        if(tSkill->getBaseSkillID() == atk1skillId)
        {
            switch (atkIdx) {
                case 1:
                    atk2skillId = linkId;
                    break;
                case 2:
                    atk3skillId = linkId;
                    break;
                case 3:
                    atk4skillId = linkId;
                    break;
                default:
                    break;
            }
            atkIdx++;
        }
        readRelativeSkill(linkId);
    }
    
    int xuliId = tSkill->getXuliId();
    if (xuliId > 0) {
        readRelativeSkill(xuliId);
    }
    
}

void BlackActor::initSelSkillTag()
{
    auto doc = GameReader::getDocInstance(SKILL_TAG_CONFIG_FILE);
    if(SkillTag::idTable.empty())
    {
        SkillTag::initIdTable(*doc);
    }
    
    for (auto skill : selSkills) {
        int skillId = skill.first;
        readRelativeSkillTag(skillId);
    }
}

void BlackActor::calSkillDis()
{
    safeDis = 0;//先取最小的技能距离作为安全距离
    for (auto skill : selSkills) {
        int skillId = skill.first;
        float skillDis = 0;
        float sumMove = 0;
        selSkills[skillId]->setHasBullet(false);
        for (auto tag : selSkillTags) {
            if (skillId != tag.second->getSkillTagId()/100) {
                continue;
            }
            
            float maxX = abs(tag.second->getAttackRect().getMaxX());
            tag.second->getAtkMove();
            sumMove += tag.second->getAtkMove();
            if (sumMove+maxX > skillDis) {
                skillDis = sumMove+maxX;
            }
            if (tag.second->getBulletID() > 0) {
                selSkills[skillId]->setHasBullet(true);
            }
        }
        if (safeDis == 0) {
            safeDis = skillDis;
        }else{
            safeDis = MIN(safeDis, skillDis);
        }
        selSkills[skillId]->setValidDis(skillDis);
    }
//    //再取第二小的距离作为，备选安全距离
//    int secondSafeDis = 0;
//    for (auto skill : selSkills) {
//        if (skill.second->getValidDis() == 0) {
//            continue;
//        }
//        if (skill.second->getValidDis() == safeDis) {
//            continue;
//        }
//        if (secondSafeDis == 0) {
//            secondSafeDis = skill.second->getValidDis();
//        }else{
//            secondSafeDis = MIN(secondSafeDis, skill.second->getValidDis());
//        }
//    }
//    //取第二小的安全距离作为正式安全距离
//    safeDis = MAX(safeDis, secondSafeDis);
}

void BlackActor::readRelativeSkillTag(int skillId)
{
    int skillTagId = skillId * 100 + 1;
    while (SkillTag::idTable.find(skillTagId) != SkillTag::idTable.end() && skillTagId < skillId*100 +100) {
        auto skillTag = new SkillTag;
        skillTag->readFile(skillTagId, SKILL_TAG_CONFIG_FILE);
        selSkillTags[skillTagId] = skillTag;
        skillTagId++;
    }
}

void BlackActor::initPlayerData()
{
//    int player_id = Account_Data_Mgr::instance()->get_current_role_id();
//    Game_Data::Player* player = dynamic_cast<Game_Data::Player*>(CHARACTER_MGR::instance()->get_character(player_id));
//    Value tmpLevel(player->get_character_level());
//    level = tmpLevel.asInt();
//
//    
////    maxHp = player->get_character_max_hp();
//    maxMp = player->get_character_max_mp();
////    hp = player->get_character_cur_hp();
//    mp = player->get_character_cur_mp();
////    maxAp = player->get_character_max_gp();
////    ap = player->get_character_cur_gp();
//    attack = player->get_character_attack();
//    defence = player->get_character_define();
//    
//    hitRatio = player->get_character_hit_ratio();
//    dodgeRatio = player->get_character_avoidance();
//    critRatio = player->get_character_crit_rate();
//    tenacity = player->get_character_tenacity();
//    
//    iceResist = player->get_Ice_Resist();
//    fireResist = player->get_Fire_Resist();
//    thunderResist = player->get_Thunder_Resist();
//    windResist = player->get_Wind_Resist();
//    lightResist = player->get_Light_Resist();
//    darkResist = player->get_Dark_Resist();
//    playerName = player->get_character_name();
//    
//    colossusId = player->get_cur_god_id();
//    guardId = player->get_cur_pet_id();
    colossusId = 0;
    guardId = 0;
//
//    //ff
//    TARGET_CHARACTER_BATTLE_SKILL_DATA skillData;
//    if(!SkillManager::instance()->GetCharacterBattleSkillData(&skillData))
//    {
//        return;
//    }
//    
////    int job = player->get_job();
////    auto skillVec = player->get_select_job_skill_list();
////    for (auto skill : skillVec) {
////        Skill::readySkillFromData(job, skill);
////    }
////    runAttack1ID = skillData.nRunSkill[0];
////    runAttack2ID = skillData.nRunSkill[1];
//    if(runAttack2ID == 0)
//    {
//        runAttack2ID = runAttack1ID;
//    }
//    runAttackID = runAttack1ID;
//    skillAId = skillData.nSelectedSkill[0];
//    skillBId = skillData.nSelectedSkill[1];
//    skillCId = skillData.nSelectedSkill[2];
//    skillDId = skillData.nSelectedSkill[3];
//    
////    atk1skillId = skillData.nGeneralSkillID;
////    atkskillId = skillData.nGeneralSkillID;
//    skillSPId = skillData.nSpSkillID;
    initSelSkill();
    initSelSkillTag();
}

void BlackActor::initSkills(int holySwordSkillId, int *petSkillId)
{
    colossusId = 0;
    guardId = 0;
    this->holySwordSkillId = holySwordSkillId;
    memcpy(this->petSkillId, petSkillId, sizeof(int)*3);
    
    initSelSkill();
    initSelSkillTag();
}

void BlackActor::addAv()
{
    av++;
    if (av >= maxAV) {
        av = 0;
        ap++;
        ap = MIN(ap, maxAp);
    }
}

void BlackActor::initOppRobot(OPPONENT_ROBOT &opp)
{
    robot.name = opp.name;
    robot.jobId = opp.jobId;
    robot.fighterId = opp.fighterId;
    robot.hp = opp.hp;
    robot.attack = opp.attack;
    robot.defence = opp.defence;
    robot.crit = opp.crit;
    robot.decrit = opp.decrit;
    robot.holySwordId = opp.holySwordId;
    memcpy(robot.petId, opp.petId, sizeof(int)*3);
    
}

void BlackActor::setupDataWithRobot()
{
    colossusId = 0;
    guardId = 0;
    name = robot.name;
    job = robot.jobId;
    maxHp = hp = robot.hp;
    attack = robot.attack;
    defence = robot.defence;
    critRatio = robot.crit;
    tenacity = robot.decrit;
    holySwordSkillId = robot.holySwordId;
    memcpy(petSkillId, robot.petId, sizeof(int)*3);
}
