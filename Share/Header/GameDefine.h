//
//  GameDefine.h
//  owen
//
//  Created by ff on 16/8/9.
//
//

#ifndef GameDefine_h
#define GameDefine_h
#include "cocos2d.h"
const std::string xxteaKeyStr = "WBCKR";
const std::string xxteaSignStr = "FGXKQ";
#ifdef FOR_HUA
const int GroundY = 210;
#else
const int GroundY = 120;
#endif
const int bottlePrice[3] = {10,20,50};
const int comboThres[7] = {10,20,30,50,60,80,100};
const int comboFactor[7] = {2,5,10,25,45,70,100};
const int vipExpThres[2] = {7,15};
const int maxVipLevel = 3;
const int vipPriceThres[3] = {10,50,100};
const int originVIT = 100;
const int vitRestoreSec = 120;
const int arenaTicketRestoreSec = 3600;
const int offerBoardRefreshSec = 1200;
const int offerBoardRefreshPrice = 20;
const int normalDrawRestoreSec = 3600*12;
const int specialDrawRestoreSec = 3600*48;
const int assembleGemCount = 3;
const std::string propIcon[5] =
{"iconshengming.png","icongongji.png","iconfangyu.png","iconbaoji.png","iconkangbao.png"};
const int provTicketPrice = 10;
const int battleCostVIT = 5;
const int vitPrice = 50;
const int enableStarBoxCount[3] = {9, 18, 30};
const int abyssMaxFightCount = 3;
const int abyssResetPrice = 10;

const int sweepTowerPrice = 5;
const int sweepTowerTime = 20;
const int arenaTicketPrice = 50;
const int luckyTicketPrice = 100;
const int activeBoxVal[4] = {10, 40, 80, 100};
const int freeDiamondDuration[6] = {180, 300, 600, 900, 1200, 1800};
//const int freeDiamondDuration[6] = {18, 30, 60, 9, 12, 18};
const int freeDiamondCount[6] = {3, 5, 10, 15, 20, 30};
const int reviveOrderId = 100;

const int discountStart[2] = {11, 17};
const int discountEnd[2] = {14, 21};

const int maxRefreshOfferCount = 6;
const int refreshOfferPrice[maxRefreshOfferCount] = {10,20,20,30,30,50};

enum LAYER_INDEX {
    NONE_LAYER = 0,
    HALL_SECEN = 1,
    BATTLE_CHAPTER_LAYER = 2,
    CHAPTER_STAGE_LAYER = 3,
    STAGE_LAYER = 4,
    FIGHT_SCENE = 5,
    HERO_LAYER = 6,
    ITEM_WAY_LAYER = 7,
    PET_LAYER = 8,
    ARMS_LAYER = 9,
    FILL_GEM_LAYER = 10,
    HOLY_SWORD_LAYER = 11,
    PET_OFFER_LAYER = 12,
    ABYSS_LAYER = 13,
    TOWER_LAYER = 14,
    PROV_LAYER = 15,
    PVP_LAYER = 16,
    LUCKY_LAYER = 17,
    CASTLE_LAYER = 18,
    CAMPAIGN_LAYER = 19,
    VIP_LAYER = 20
};

static cocos2d::Size designResolutionSize = cocos2d::Size(960, 640);
#define UMENG_APPKEY "5aadc630a40fa32f88000d97"
#define SERVER_TIME_URL "http://miao.quleinfo.com:3700/forwardapi/time"

#define STAGE_CONFIG_FILE                                   "data/dabao_stage.json"
#define BATTLE_CONFIG_FILE                                  "data/dabao_battleField.json"
#define MONSTER_WAVE_CONFIG_FILE                            "data/dabao_monsterWave.json"
#define MONSTER_TEAM_CONFIG_FILE                            "data/dabao_monsterTeam.json"
#define FIGHTER_CONFIG_FILE                              "data/dabao_fighter.json"
#define SKILL_CONFIG_FILE                                "data/dabao_skill.json"
#define SKILL_TAG_CONFIG_FILE                            "data/dabao_skillTag.json"
#define BUFF_CONFIG_FILE                            "data/dabao_buff.json"
#define BULLET_CONFIG_FILE                          "data/dabao_bullet.json"
#define CLS_SKILL_CONFIG_FILE                      "data/dabao_ClsSkill.json"
#define CHAPTER_CONFIG_FILE                         "data/dabao_chapter.json"
#define STATE_MACHINE_DIR                           "data/statemachine/%s"
#define JOB_CONFIG_FILE                             "data/system_job.json"
#define UNLOCK_CONFIG_FILE                          "data/system_unlock.json"
#define UPLEVEL_CONFIG_FILE                         "data/system_level.json"
#define EQUIP_CONFIG_FILE                           "data/system_equip.json"
#define EQUIP_LEVEL_CONFIG_FILE                     "data/system_equip_level.json"
#define EQUIP_ITEM_CONFIG_FILE                      "data/system_equip_item.json"
#define PET_CONFIG_FILE                             "data/system_pet.json"
#define PET_LEVEL_CONFIG_FILE                       "data/system_pet_level.json"
#define PET_OFFER_CONFIG_FILE                       "data/system_pet_offer.json"
#define HOLY_SWORD_CONFIG_FILE                      "data/system_holy_sword.json"
#define SWORD_LEVEL_CONFIG_FILE                     "data/system_holy_sword_level.json"
#define ARMS_CONFIG_FILE                            "data/system_arms.json"
#define ARMS_LEVEL_CONFIG_FILE                      "data/system_arms_level.json"
#define GEM_CONFIG_FILE                             "data/system_gem.json"
#define PORVING_CONFIG_FILE                         "data/system_prov.json"
#define ABYSS_CONFIG_FILE                           "data/system_abyss.json"
#define TOWER_CONFIG_FILE                           "data/system_tower.json"
#define ARENA_ROBOT_CONFIG_FILE                     "data/system_pvp_robot.json"
#define ARENA_DIVISION_CONFIG_FILE                  "data/system_pvp_division.json"
#define NAME_PART1_FILE                             "data/system_xing.json"
#define NAME_PART2_FILE                             "data/system_ming.json"
#define PLOT_MISSION_CONFIG_FILE                    "data/system_plot_mission.json"
#define PLOT_CONFIG_FILE                            "data/plot_plot%d.json"
#define DIALOG_GROUP_CONFIG_FILE                    "data/dialog/dialog%d_group.json"
#define DIALOG_SEG_CONFIG_FILE                      "data/dialog/dialog%d_seg.json"
#define DIALOG_LINE_CONFIG_FILE                     "data/dialog/dialog%d_line.json"
#define NPC_CONFIG_FILE                             "data/dabao_npc.json"
#define MONTH_ASSIGN_CONFIG_FILE                    "data/system_month_assign.json"
#define EVERYDAY_CONFIG_FILE                        "data/system_everyday.json"
#define LUCKY_CONFIG_FILE                           "data/system_lucky.json"
#define SHOP_CONFIG_FILE                            "data/system_shop.json"
#define SHOP_PACKAGE_CONFIG_FILE                    "data/system_shop_package.json"
#define VIP_CONFIG_FILE                             "data/system_vip.json"
#define VIP_PACKAGE_CONFIG_FILE                     "data/system_vip_package.json"
#define ACTIVE_MISSION_CONFIG_FILE                  "data/system_active_mission.json"
#define ACTIVE_BOX_CONFIG_FILE                      "data/system_active_box.json"
#define BOUND_MISSION_CONFIG_FILE                   "data/system_bound_mission.json"
#define BOUND_PACKAGE_CONFIG_FILE                   "data/system_bound_mission_package.json"


#define TMX_DIR                                     "scene/%s.tmx"
#define WEATHER_DIR                                 "Particle/%s.plist"
#define SOUND_DIR                                   "sound/%s"

#define BUTTON_EFFECT                               "csb/battle/guangxiao.csb"
#define BATTLE_UI                                   "csb/battle/battle_mid.csb"
#define BATTLE_UI_LEFT                              "csb/battle/battle_left.csb"
#define BATTLE_UI_RIGHT                             "csb/battle/battle_right.csb"
#define COMBO_UI                                    "csb/battle/combo.csb"
#define AP_FULL_EFFECT                              "csb/battle/spshan.csb"
#define BATTLE_PAUSE_UI                             "csb/battle/pause.csb"
#define BATTLE_WIN_UI                               "csb/za/win.csb"
#define BATTLE_WIN_STAR_UI                          "csb/za/winstar.csb"
#define ABYSS_WIN_UI                                "csb/za/win2.csb"
#define TOWER_WIN_UI                                "csb/za/win3.csb"
#define BATTLE_FAIL_UI                              "csb/za/lose.csb"
#define BATTLE_REVIVE_UI                            "csb/za/revive.csb"
#define BATTLE_TIMEOVER_UI                          "csb/za/timeover.csb"
#define FORCE_UPLEVEL_UI                            "csb/za/tisheng.csb"
#define BATTLE_CHAPTERS_UI                          "csb/dungeons/dungeons.csb"
#define CHAPTER_GATE_UI                             "csb/dungeons/dungeons1.csb"
#define CHAPTER_UI                                  "csb/dungeons/dungeons2.csb"
#define STAGE_UI                                    "csb/dungeons/dungeons3.csb"
#define STAGE_NODE_UI                               "csb/dungeons/dungeons4.csb"
#define STAR_BOUNDS_UI                              "csb/dungeons/bound.csb"
#define HOME_UI                                     "csb/home/home_mid.csb"
#define HOME_LEFT_UI                                "csb/home/home_left.csb"
#define HOME_RIGHT_UI                               "csb/home/home_right.csb"
#define HALL_HERO_UI                                "csb/hero/hero.csb"
#define EQUIP_FOCUS_UI                              "csb/hero/herochoose.csb"
#define ITEM_WAY_UI                                 "csb/hero/way.csb"
#define ITEM_SWEEP_UI                               "csb/hero/sweep.csb"
#define ITEM_SWEEP_NODE_UI                          "csb/hero/sweep_node.csb"
#define TIPS_UI                                     "csb/za/tips.csb"
#define LEVELUP_TIP_UI                              "csb/za/levelup.csb"
#define FORCEUP_TIP_UI                              "csb/za/tisheng.csb"
#define HALL_PET_UI                                 "csb/monster/monster.csb"
#define HALL_PET_MASK_UI                            "csb/monster/unmonster.csb"
#define HALL_PET_OFFER_UI                           "csb/reward/reward.csb"
#define HALL_BUY_OFFER_UI                           "csb/reward/reward2.csb"
#define HALL_PET_RELATION_UI                        "csb/reward/choose.csb"
#define HALL_HOLY_SWORD_UI                          "csb/soul/soul.csb"
#define HALL_ARMS_UI                                "csb/badge/badge.csb"
#define HALL_ARMS_FILL_UI                           "csb/badge/badge2.csb"
#define HALL_ARMS_REMOVE_UI                         "csb/badge/badgeoff.csb"
#define ASSEMBLE_GEM_EFFECT                         "csb/badge/stoneup.csb"
#define ARMS_GEM_EFFECT                             "csb/badge/badgeequip.csb"
#define HALL_PROV_UI                                "csb/proving/proving.csb"
#define PROV_POINT_UI                               "csb/proving/provingpoint.csb"
#define PROV_REWARD_UI                              "csb/proving/provingget.csb"
#define PROV_TICKET_UI                              "csb/proving/roll.csb"
#define HALL_ABYSS_UI                               "csb/abyss/abyss.csb"
#define ABYSS_NODE_UI                               "csb/abyss/abysschoose.csb"
#define ABYSS_RESET_UI                              "csb/abyss/buy.csb"
#define HALL_TOWER_UI                               "csb/tower/tower.csb"
#define TOWER_BASE_UI                               "csb/tower/tower_base.csb"
#define TOWER_BODY_UI                               "csb/tower/tower_body.csb"
#define TOWER_SWEEP_UI                              "csb/tower/tower_get.csb"
#define TOWER_SWEEP_NODE_UI                         "csb/tower/tower_get2.csb"
#define HALL_ARENA_UI                               "csb/pvp/pvp.csb"
#define ARENA_RANK_NODE_UI                          "csb/pvp/pvplist.csb"
#define DIVISION_UI                                 "csb/pvp/division.csb"
#define DIVISION_NODE_UI                            "csb/pvp/divisionlist.csb"
#define ARENA_MATCH_UI                              "csb/pvp/pvpstart.csb"
#define ARENA_WIN_UI                                "csb/za/pvpwin.csb"
#define ARENA_LOSE_UI                               "csb/za/pvplose.csb"
#define ARENA_REWARD_UI                             "csb/za/pvpget.csb"
#define DIALOG_LEFT_UI                              "csb/story/duihuazuo.csb"
#define DIALOG_RIGHT_UI                             "csb/story/duihuayou.csb"
#define PLOT_TIPS_UI                                "csb/story/tishikuang2.csb"
#define UNLOCK_EFFECT_UI                            "csb/za/newfunction.csb"
#define HALL_CASTLE_UI                              "csb/castle/castle.csb"
#define CASTLE_BUY_JOB_UI                           "csb/castle/castlebuy.csb"
#define HALL_CAMPAIGN_UI                            "csb/everyday/everyday.csb"
#define HALL_LUCKY_UI                               "csb/lucky/lucky.csb"
#define LUCKY_BUY_TICKET_UI                         "csb/lucky/luckybuy.csb"
#define LUCKY_REWARD1_UI                            "csb/lucky/luckyreward1.csb"
#define LUCKY_REWARD2_UI                            "csb/lucky/luckyreward2.csb"
#define LUCKY_GET1_UI                               "csb/lucky/luckyget1.csb"
#define LUCKY_GET2_UI                               "csb/lucky/luckyget2.csb"
#define HALL_SHOP_UI                                "csb/shop/shop.csb"
#define SHOP_PACKAGE1_UI                            "csb/shop/shop_package1.csb"
#define SHOP_PACKAGE2_UI                            "csb/shop/shop_package2.csb"
#define SHOP_PACKAGE3_UI                            "csb/shop/shop_package3.csb"
#define SHOP_PACKAGE_ITEM_UI                        "csb/shop/package_item.csb"
#define SHOP_PACKAGE_DETAIL_UI                      "csb/shop/package_detail.csb"
#define HALL_NOVICE_UI                              "csb/novice/novice.csb"
#define HALL_VIP_UI                                 "csb/noble/noble.csb"
#define HALL_VIP_UI                                 "csb/noble/noble.csb"
#define ACTIVE_MISSION_UI                           "csb/everyday/mission_reward.csb"
#define ACTIVE_BOX_REWARD_UI                        "csb/everyday/everyday_mission_get.csb"
#define BOUND_MISSION_UI                            "csb/everyday/mission_reward2.csb"
#define DISCOUNT_UI                                 "csb/home/zhekoulibao.csb"
//#define DISCOUNT_UI                                 "csb/home/discount.csb"
#define LOGIN_UI                                    "csb/load/login.csb"
#define LOAD_UI                                     "csb/load/loading.csb"
#define UNLOCK_JOB_UI                               "csb/za/get3.csb"



#define BUY_VIT_UI                                  "csb/za/VIT.csb"
#define GET_TIP_UI                                  "csb/za/get.csb"
#define WARNING_UI                                  "csb/za/warning.csb"
#define GUIDE_POINT_UI                              "csb/story/shouzhi%d.csb"
#define GUIDE_POINT_TIP_UI                          "csb/story/tishikuang.csb"
#define LOCK_UI                                     "csb/za/unlock%d.csb"
#define FUNC_ICON_DIR                               "ui/za/%s"
#define HERO_HEAD_DIR                               "ui/head/%s"
#define HOME_DIR                                    "ui/home/%s"
#define LOAD_DIR                                    "ui/load/%s"

#define ACTOR_DIR                                   "actor/%s.ExportJson"

#define SHADOW_FILE                                 "ui/battle/shadow.png"
#define BATTLE_DIR                                  "ui/battle/%s"
#define HEAD_ICON_FILE                              "ui/head/Head_icon_%d.png"
#define COMMON_DIR                                  "ui/common/%s"
#define OFFER_ICON                                  "ui/reward/reward%d.png"
#define CHAPTER_NAME_ICON                           "ui/dungeons/%s"
#define HOLY_SWORD_DIR                              "ui/soul/%s"
#define ARMS_DIR                                    "ui/badge/%s"
#define ABYSS_DIR                                   "ui/abyss/%s"
#define ARENA_DIR                                   "ui/pvp/%s"
#define VIDEO_DIR                                   "ui/video/plot_%d.mp4"
#define STORY_DIR                                   "ui/story/%s"
#define CASTLE_DIR                                  "ui/castle/%s"
#define SHOP_DIR                                    "ui/shop/%s"
#define LUCKY_DIR                                   "ui/lucky/%s"
#define PROV_DIR                                    "ui/proving/%s"
#define CAMPAIGN_DIR                                "ui/everyday/%s"

#define VIT_STR                                     "%d/%d"
#define STAGE_VICTORY_STAR                          "副本通关"
#define STAGE_TIME_STAR                             "时间剩余%d秒"
#define STAGE_HP_STAR                               "生命剩余%d%%"
#define STAGE_BOTTLE_STAR                           "未使用血瓶"

#define XULI_EFFECT_TAG 100
#define ATTACK_EFFECT_TAG 101

#define MOVE_ACTION_TAG 1001
#define READY_RUSH_TAG 1002
#define READY_RUSHATK_TAG 1003

#define XULI_ENOUGH_TAG 10000
#define SKILLA_LINK_TIMEOUT_TAG 10001
#define SKILLB_LINK_TIMEOUT_TAG 10002
#define SKILLC_LINK_TIMEOUT_TAG 10003
#define SKILLD_LINK_TIMEOUT_TAG 10004
#define IDLE_TIMEOUT_TAG 10005
#define WAIT_TIMEOUT_TAG 10006
#define TIP_TIMEOUT_TAG 10007
#define GHOST_BAR_CHANGE_TAG 10008
#define HOLY_SWORD_LINK_TIMEOUT_TAG 10009

//原是走松开后，连接跑的极限时间定义
//现在城镇中，已取消冲刺跑
//现在战斗中，用于跑攻1和跑攻2的时间分界点
#define RUSH_LINK_TIME 0.3

//与伤害相关的自定义事件名称
//主角被击事件标签，由击中主角的对象发出。需要在userData中配置攻击方与防御方
#define HERO_HURT_TAG                               "hero_parse_hurt"
//怪物重置动作时，让主角将其从案犯记录（造成过伤害的怪物的列表）中删掉
#define MONSTER_RESET_PUNCH                         "monster_reset_punchtag"
//怪物被击事件标签，与主角被击事件标签作用类似
#define MONSTER_HURT_TAG                            "monster_parse_hurt"
//主角重置动作标签，与怪物重置动作标签作用类似
#define HERO_RESET_PUNCH                            "hero_reset_punchtag"
//保护对象被击事件标签，由击中保护对象的对象发出。需要在userData中配置攻击方与防御方
#define WITNESS_HURT_TAG                            "witness_parse_hurt"
//保护对象重置动作标签，与怪物重置动作标签作用类似
#define WITNESS_RESET_PUNCH                         "witness_reset_punchtag"
enum SKILL_FIGHT_TYPE
{
    TARGET_SKILL_TYPE_LINK				= 0,
    TARGET_SKILL_TYPE_SELECT			= 1,
    TARGET_SKILL_TYPE_SP				= 2,
    TARGET_SKILL_TYPE_GENERAL           = 3,
    TARGET_SKILL_TYPE_RUN_1             = 4,
    TARGET_SKILL_TYPE_RUN_2             = 5,
    TARGET_SKILL_TYPE_RUN_3             = 6,
    TARGET_SKILL_TYPE_ERROR
};

//按键解码结果列表，目前只用到前三个。
enum KEYCOMMAND
{
    CMD_NONE = 0,
    CMD_STOPMOVE = 1,
    CMD_WALK,
    CMD_RUSH,
    CMD_RUSHATK1,
    CMD_RUSHATK2,
    CMD_ATTACK,
    CMD_SKILLA,
    CMD_SKILLB,
    CMD_SKILLC,
    CMD_SKILLD,
    CMD_JUMP
};

enum GAMEKEY
{
    BUTTON_NONE = 0,
    BUTTON_A = 1,
    BUTTON_S1 = 2,
    BUTTON_S2 = 3,
    BUTTON_S3 = 4,
    BUTTON_J = 5,
    BUTTON_S1_E = 6,
    BUTTON_S2_E = 7,
    BUTTON_S3_E = 8,
    BUTTON_SP = 9,
    BUTTON_Y = 10,
    BUTTON_HOLY_SWORD = 11,
    BUTTON_JUMP = 12
};

enum HEROFRAMECOMMAND
{
    COMMAND_PUNCH = 0x1,
    COMMAND_PUNCHA2 = 0x2,
    COMMAND_PUNCHA3 = 0x4,
    COMMAND_PUNCHA4 = 0x8,
    COMMAND_SKL_XFT = 0x10,
    COMMAND_SKL_SLT = 0x20,
    COMMAND_JUMP_ATK = 0x40
};

//方向定义，可位操作查看叠加方向信息
enum DIRECT
{
    NONE = 0,
    UP = 1,
    RIGHT = 2,
    DOWN = 4,
    LEFT = 8
};

//角色状态列表，所有角色涉及的所有状态。各状态机表格需要与它保持一致。
enum ACTORSTATE
{
    ANY = -2,
    UNDEFINED = -1,
    IDLE = 0,
    MOVE,
    RUSH,
    ATTACK,
    SKILL,
    SKILL4, //5
    RUNATTACK,
    JUMP,
    JUMPATTACK,
    HURT,
    FELLDOWN, //10
    XULI,
    INAIR,
    SPSKILL,
    DIE,
    BLINK, //15
    PATROL,
    CHASE,
    RETURN,
    WAIT,
    LINGERS, //20
    BACK,
    AUTOMOVE,
    INIT,
    BOUNDGROUND,
    BOUNDWALL, //25
    SNEAK,
    SNEAKRUSH,
    FAINT,
    RUSHBACK,
    FORZEN  //30
};

//原为所有角色所用的动作名表，但现在动作的顺序已不再需要遵循。
//再使用这个会是比较危险的。
//todo 需要将所有用这个的地方再检查一遍。能不使用就替换掉。
enum HEROACTION
{
    STAND = 0,
    GETUP,
    HEADHIT,
    WAISTHIT,
    WALK,
    RUN, //5
    JUMP_UP,
    JUMP_DOWN,
    LANDGROUND,
    JUMPATTACK1,
    JUMPATTACK2, //10
    HEADFELLDOWN,
    WAISTFELLDOWN,
    AIRHITUP,
    AIRHITDOWN,
    AIRHIT, //15
    ATTACKPART1,
    ATTACKPART2,
    ATTACKPART3,
    ATTACKPART4,
    ATTACKPART5, //20
    ATTACKPART6,
    ATTACKPART7,
    ATTACKPART8,
    ATTACKPART9,
    SKILLATK1, //25
    SKILLATK2,
    SKILLATK3,
    SKILLATK4,
    SKILLATK5,
    SKILLATK6, //30
    SKILLATK7,
    SKILLATK8,
    SKILLATK9,
    RUNATTACK1,
    RUNATTACK2, //35
    RUNATTACK3,
    RUNATTACK4,
    RUNATTACK5,
    RUNATTACK6,
    RUNATTACK7, //40
    RUNATTACK8,
    RUNATTACK9,
    ATTACKCACHE,
    LIE,
    LIESHOCK, //45
    BOUND2G,
    BOUNDUP,
    BOUND2W,
    BOUNDBACK,
    JUMP_STICK = 59,
    SAFE_DOWN = 60,
    BORN = 61,
    WIN = 64
};

enum KEY_SKILL {
    ATK_KEY = 0,
    SKILLA_KEY = 1,
    SKILLB_KEY = 2,
    SKILLC_KEY = 3,
    SKILLD_KEY = 4,
    HOLY_SKILL_KEY = 5,
    PET_SKILL1_KEY = 6,
    PET_SKILL2_KEY = 7,
    PET_SKILL3_KEY = 8,
    SKILLS_KEY = 9,
    RUN_ATK_KEY = 10,
    SKEY_MAX = 11
};

#endif /* GameDefine_h */
