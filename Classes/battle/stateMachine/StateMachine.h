//
//  StateMachine.h
//  Dabao
//
//  Created by ff on 17/3/9.
//
//

#ifndef StateMachine_h
#define StateMachine_h

#include "cocos2d.h"

#define ACTION_UNDEFINED -1;

struct StateFlow
{
    int state = -1;
    int event = -1;
    int flowIndex = -1;
};

struct NextStateData
{
    int nextTactics = -1;
    int nextAction = -1;
    int flag = 0;
};

enum StateAIFlag
{
    rate = 0x1000,
    skillRate = 0x2000,
    fallFlag = 0x4000,
    getFlag = 0x7000,
    
    getData = 0xfff,
    invincible = 0x1,
    startFall = 0x3,
    endFall = 0x2,
    selSkill = 0x4
    
};

struct StateRate
{
    int rate1;
    int flowIndex;
};

class StateMachine
{
public:
    CC_SYNTHESIZE(int, currTactics, CurrTactics);
    CC_SYNTHESIZE(int, currEvent, CurrEvent);
    CC_SYNTHESIZE(int, nextTactics, NextTactics);
    CC_SYNTHESIZE(int, nextAction, NextAction);
    CC_SYNTHESIZE_READONLY(int, nextFlag, NextFlag);
    CC_SYNTHESIZE_READONLY(int, totalRate, TotalRate);
    
    //int currTactics;
    //int currEvent;
    //int nextTactics;
    //int nextAction = ACTION_UNDEFINED;
    //int nextFlag;
    
    /**
     *	@brief	通过当前状态和事件，找出应该改变为的下一个状态和动作
     *
     */
    void nextState();
    
    /**
     *	@brief	设定当前状态和事件
     *
     *	@param 	state 	int 当前状态
     *	@param 	action 	int 触发事件
     */
    void changeState(int state, int action);
    
    /**
     *	@brief	由当前状态和触发事件，寻找下一状态或动作
     *
     *	@param 	event 	int 触发事件或结束动作
     *	@param 	state 	int 当前状态
     */
    void getTacticsAction(int event, int state);
    
    /**
     *	@brief	根据数据做跳转处理，或跳转到随机率条件的选择
     *
     *	@param 	flowData 	int 跳转数据条目索引
     */
    void parseData(int data);
    
    /**
     *	@brief	根据随机率实现随机的状态跳转。
     *          随机值的分配规则是涨水型。
     *
     *	@param 	randIndex 	int 随机集合索引
     */
    void getRateAction(int randIndex);
    
    /**
     *	@brief	加载所有状态机相关文件
     *
     *	@param 	filename std::string 状态机文件组的路径和前缀
     */
    void loadFromFile(std::string fileName);
    
    /**
     *	@brief	加载事件名称与code对应表。
     *          动作必须都包含在里面，并与实际美术动作顺序对应。
     *          不可有重复事件和code。
     
     *	@param 	filename 	std::string 状态机事件列表文件的路径
     */
    void loadEventList(std::string filename);
    
    /**
     *	@brief	加载状态名称与code对应表，不可有重复状态名和code。
     *
     *	@param 	filename 	std::string 状态机状态列表文件的路径
     */
    void loadStateList(std::string filename);
    
    /**
     *	@brief	加载状态跳转文件。
     *          状态和事件的值必须是在状态表和事件表中定义过的。
     *          如果程序中写了事件传递，而跳转文件没有定义过，会出现错误
     *
     *	@param 	filename 	std::string 状态机状态跳转文件的路径
     */
    void loadStateFlowList(std::string filename);
    
    /**
     *	@brief	加载跳转数据表。
     *          代表将要跳转到的状态和动作。
     *          flag用来在跳转前后，做一定的处理。
     *
     *	@param 	filename"] 	std::string 数据表路径
     */
    void loadFlowData(std::string filename);
    
    
    /**
     *	@brief	加载随机跳转数据表。
     *          当数据表中某数据有Rate标签时，走此表。
     *          由此表决定跳转路径和跳转几率。
     *
     *	@param 	filename 	std::string 数据表路径
     */
    void loadRateData(std::string filename);
    
    /**
     *	@brief	通过事件名称找到事件code
     *
     *	@param 	eventName 	std::string 事件名称
     *
     *	@return	int 事件code
     */
    int findEventVal(std::string eventName);
    
    std::vector<std::string> findActionNames(int action);
    
    std::map<std::string, int> getEventMap();
    std::map<std::string, int> getStateMap();
    
public:
    static const int action_undefined = -1;
    static const unsigned int max_Rate = 100;
    std::map<std::string, int> eventMap;
    std::map<std::string, int> stateMap;
    std::map<int, std::map<int, int>> stateFlowMap;
    std::vector<NextStateData> stateData;
    //    std::map<int, std::string> actionNames;
    std::map<int, std::vector<std::string>> actionNames;
    std::map<int, std::string> stateName;
    std::map<int, std::string> eventName;
private:
    
    std::map<int, std::vector<StateRate>> rateData;
};

#endif /* StateMachine_h */
