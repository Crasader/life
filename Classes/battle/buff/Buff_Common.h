/* -------------------------------------------------------------------------
//	文件名		：	BuffCommon.h
//	功能描述	：	用于存放Buff的公共数据
// -----------------------------------------------------------------------*/

#ifndef _BUFF_COMMON_H
#define _BUFF_COMMON_H

//#include "../../Game_Interface/common.h"

#define	 MAX_BUFF_LOGIC_COUNT		10	
#define  MAX_BUFF_COUNT_ON_PALYER	10		// 武将身上最大BUFF数
#define  MAX_EFFECT_COUNT			4		// 一个BUFF最多拥有的效果个数
#define  MAX_BUFF_DESCRIBE_SIZE		126		// BUFF描述的长度

/************************************************************************/
// Buff单个效果的相关定义
// Buff效果属性的类型
enum BUFF_EFFECT_ATTRIBUTE_TYPE
{
    BUFF_EFFECT_ATTRIBUTE_TYPE_MAX_HP						= 1,								// 气血上限
    BUFF_EFFECT_ATTRIBUTE_TYPE_CUR_HP						= 2,								// 当前气血
    BUFF_EFFECT_ATTRIBUTE_TYPE_HP_RECOVER_SPEED				= 3,								// 气血回复速度
    BUFF_EFFECT_ATTRIBUTE_TYPE_MAX_MP						= 4,								// 魔法上限
    BUFF_EFFECT_ATTRIBUTE_TYPE_CUR_MP						= 5,								// 当前魔法
    BUFF_EFFECT_ATTRIBUTE_TYPE_MP_RECOVER_SPEED				= 6,								// 魔法回复速度
    BUFF_EFFECT_ATTRIBUTE_TYPE_MAX_AP						= 7,								// 怒气上限
    BUFF_EFFECT_ATTRIBUTE_TYPE_CUR_AP						= 8,								// 当前怒气
    BUFF_EFFECT_ATTRIBUTE_TYPE_AP_RECOVER_SPEED				= 9,								// 怒气恢复速度
    BUFF_EFFECT_ATTRIBUTE_TYPE_ATTACK						= 10,								// 攻击力
    BUFF_EFFECT_ATTRIBUTE_TYPE_DEFINE						= 11,								// 防御力
    BUFF_EFFECT_ATTRIBUTE_TYPE_HIT_RATIO					= 12,								// 命中值
    BUFF_EFFECT_ATTRIBUTE_TYPE_AVOIDANCE					= 13,								// 回避值
    BUFF_EFFECT_ATTRIBUTE_TYPE_CRIT_RATE					= 14,								// 暴击值
    BUFF_EFFECT_ATTRIBUTE_TYPE_TENACITY						= 15,								// 坚韧值
    BUFF_EFFECT_ATTRIBUTE_TYPE_MOVE_SPEED_RATE				= 16,								// 移动速率
    BUFF_EFFECT_ATTRIBUTE_TYPE_FLOAT						= 17,								// 抗浮空值
    BUFF_EFFECT_ATTRIBUTE_TYPE_HIT_DOWN                     = 18,								// 抗到地值
    BUFF_EFFECT_ATTRIBUTE_TYPE_FAINT                        = 19,								// 眩晕状态
    BUFF_EFFECT_ATTRIBUTE_TYPE_FIX							= 20,								// 定身状态
    BUFF_EFFECT_ATTRIBUTE_TYPE_FROZEN						= 21,								// 冰冻状态
    BUFF_EFFECT_ATTRIBUTE_TYPE_ICE_RESIST					= 22,								// 冰抗
    BUFF_EFFECT_ATTRIBUTE_TYPE_FIRE_RESIST					= 23,								// 火抗
    BUFF_EFFECT_ATTRIBUTE_TYPE_THUNDER_RESIST				= 24,								// 雷抗
    BUFF_EFFECT_ATTRIBUTE_TYPE_WIND_RESIST					= 25,								// 风抗
    BUFF_EFFECT_ATTRIBUTE_TYPE_LIGHT_RESIST					= 26,								// 光抗
    BUFF_EFFECT_ATTRIBUTE_TYPE_DARK_RESIST					= 27,								// 暗抗
    BUFF_EFFECT_ATTRIBUTE_TYPE_IMMU_ALL						= 28,								// 免疫所有减益状态
    BUFF_EFFECT_ATTRIBUTE_TYPE_IMMU_DEBUFF					= 29,								// 免疫指定减益状态
    BUFF_EFFECT_ATTRIBUTE_TYPE_IMMU_FIX						= 30,								// 免疫定身
    BUFF_EFFECT_ATTRIBUTE_TYPE_IMMU_FROZEN					= 31,								// 免疫冰冻
    BUFF_EFFECT_ATTRIBUTE_TYPE_1							= 32,								// 类型Buff_1
    BUFF_EFFECT_ATTRIBUTE_TYPE_2							= 33,								// 类型Buff_2
    BUFF_EFFECT_ATTRIBUTE_TYPE_3                            = 34,								// 类型Buff_3
    BUFF_EFFECT_ATTRIBUTE_TYPE_4                            = 35,								// 类型Buff_4
    BUFF_EFFECT_ATTRIBUTE_TYPE_5                            = 36,								// 类型Buff_5
    BUFF_EFFECT_ATTRIBUTE_TYPE_6                            = 37,								// 类型Buff_6
    BUFF_EFFECT_ATTRIBUTE_TYPE_7                            = 38,								// 类型Buff_7
    BUFF_EFFECT_ATTRIBUTE_TYPE_8                            = 39,								// 类型Buff_8
    BUFF_EFFECT_ATTRIBUTE_TYPE_MAX
};																							  
																							  
// Buff效果属性值的类型																	  
enum BUFF_EFFECT_ATTRIBUTE_VALUE_TYPE														  
{																							  
	BUFF_EFFECT_ATTRIBUTE_VALUE_TYPE_PERCENT			= 0,									// 百分数
	BUFF_EFFECT_ATTRIBUTE_VALUE_TYPE_INTEGER			= 1,									// 整数
	BUFF_EFFECT_ATTRIBUTE_VALUE_TYPE_ERROR
};

// Buff单个效果的数据
typedef struct __BUFF_EFFECT_DATA
{
	int													nEffectValue;							// Buff效果值

	BUFF_EFFECT_ATTRIBUTE_TYPE							eEffectAttributeType;					// Buff效果的属性类型
	BUFF_EFFECT_ATTRIBUTE_VALUE_TYPE					eEffectAttributeValueType;				// Buff效果属性值的类型
}BUFF_EFFECT_DATA;

/************************************************************************/
// 整个Buff的相关定义
// Buff总效果类型
enum BUFF_TYPE
{
	BUFF_TYPE_MAX_HP									= 1,									// 气血上限
	BUFF_TYPE_CUR_HP									= 2,									// 当前气血
	BUFF_TYPE_HP_RECOVER_SPEED							= 3,									// 气血回复速度
	BUFF_TYPE_MAX_MP									= 4,									// 魔法上限
	BUFF_TYPE_CUR_MP									= 5,									// 当前魔法
	BUFF_TYPE_MP_RECOVER_SPEED							= 6,									// 魔法回复速度
	BUFF_TYPE_MAX_AP									= 7,									// 怒气上限
	BUFF_TYPE_CUR_AP									= 8,									// 当前怒气
	BUFF_TYPE_AP_RECOVER_SPEED							= 9,									// 怒气恢复速度
	BUFF_TYPE_ATTACK									= 10,									// 攻击力
	BUFF_TYPE_DEFINE									= 11,									// 防御力
	BUFF_TYPE_HIT_RATIO									= 12,									// 命中值
	BUFF_TYPE_AVOIDANCE									= 13,									// 回避值
	BUFF_TYPE_CRIT_RATE									= 14,									// 暴击值
	BUFF_TYPE_TENACITY									= 15,									// 坚韧值
	BUFF_TYPE_MOVE_SPEED_RATE							= 16,									// 移动速率
	BUFF_TYPE_FLOAT										= 17,									// 抗浮空值
	BUFF_TYPE_HIT_DOWN                                  = 18,									// 抗到地值
	BUFF_TYPE_FAINT                                     = 19,									// 眩晕状态
	BUFF_TYPE_FIX										= 20,									// 定身状态
	BUFF_TYPE_FROZEN									= 21,									// 冰冻状态
	BUFF_TYPE_ICE_RESIST								= 22,									// 冰抗
	BUFF_TYPE_FIRE_RESIST								= 23,									// 火抗
	BUFF_TYPE_THUNDER_RESIST							= 24,									// 雷抗
	BUFF_TYPE_WIND_RESIST								= 25,									// 风抗
	BUFF_TYPE_LIGHT_RESIST								= 26,									// 光抗
	BUFF_TYPE_DARK_RESIST								= 27,									// 暗抗
	BUFF_TYPE_IMMU_ALL									= 28,									// 免疫所有减益状态
	BUFF_TYPE_IMMU_DEBUFF								= 29,									// 免疫指定减益状态
	BUFF_TYPE_IMMU_FIX									= 30,									// 免疫定身
	BUFF_TYPE_IMMU_FROZEN								= 31,									// 免疫冰冻
	BUFF_TYPE_INVINCIBLE								= 32,									// 无敌状态
	BUFF_TYPE_PA										= 33,									// 霸体状态
	BUFF_TYPE_DISABLE_SKILL                             = 34,									// 封技能
	BUFF_TYPE_REVERSE_MOVE                              = 35,									// 反向移动
	BUFF_TYPE_MAD                                       = 36,									// 狂暴
	BUFF_TYPE_6                                         = 37,									// 类型Buff_6
	BUFF_TYPE_7                                         = 38,									// 类型Buff_7
	BUFF_TYPE_8                                         = 39,									// 类型Buff_8
	BUFF_TYPE_MAX															  
};

// Buff所有效果的类型
enum BUFF_ALL_EFFECT_TYPE
{
	BUFF_ALL_EFFECT_TYPE_BUFF							= 0,									// 增益Buff
	BUFF_ALL_EFFECT_TYPE_DEBUFF							= 1,									// 非增益Buff
	BUFF_ALL_EFFECT_TYPE_ERROR
};


// Buff是否可以被替换
enum BUFF_REPLACE_TYPE
{
	BUFF_REPLACE_TYPE_YES								= 0,									// Buff可以被替换
	BUFF_REPLACE_TYPE_NO								= 1,									// Buff不可以被替换
	BUFF_REPLACE_TYPE_ERROR
};

// Buff在玩家死亡后是否会消失
enum BUFF_PLAYER_DIE_DISAPPEA_TYPE
{
	BUFF_PLAYER_DIE_DISAPPEA_TYPE_YES					= 0,									// Buff在玩家死亡后会消失
	BUFF_PLAYER_DIE_DISAPPEA_TYPE_NO					= 1,									// Buff在玩家死亡后不会消失
	BUFF_PLAYER_DIE_DISAPPEA_TYPE_ERROR
};

// Buff当前的时间类型
enum BUFF_CURRENT_TIME_TYPE
{
	BUFF_CURRENT_TIME_TYPE_EFFECTIVE_TIME				= 0,									// BUFF当前的处于有效时间内
	BUFF_CURRENT_TIME_INTERVAL_TIME						= 1,									// BUFF当前的处于间隔时间内
	BUFF_CURRENT_TIME_TYPE_ERROR
};

enum BUFF_TARGET
{
    BUFF_AT_ME                                          = 1,                                    //对自己的buff
    BUFF_AT_ENIMY                                       = 2,                                    //对敌方的buff
    BUFF_AT_FRIENDS                                     = 3,                                    //对友方包括自己
    BUFF_AT_ERROR
};

// Buff属性数据
typedef struct __BUFF_DATA
{
	int													nBuffIconID;							// Buff的图标ID
	int													nBuffEffectiveTime;						// Buff有效时间
	int													nBuffIntervalTime;						// Buff间隔时间
	int													nBuffLastTotalTime;						// Buff总持续时间
	
	char												szBuffDescribe[MAX_BUFF_DESCRIBE_SIZE];	// Buff的描述

	short												byIndexID;								// 索引ID

	BUFF_TYPE											eBuffType;								// Buff的类型
	BUFF_REPLACE_TYPE									eBuffRepalceType;						// Buff是否可以被替换
	BUFF_ALL_EFFECT_TYPE								eBuffAllEffectType;						// Buff所有效果的类型

    int                                                 nEffectCount;                           // 效果数量
	BUFF_EFFECT_DATA									strBuffEffectData[MAX_EFFECT_COUNT];	// Buff所有效果的数据
}BUFF_DATA;
#endif // _BUFF_COMMON_H
