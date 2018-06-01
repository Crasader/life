//
//  ELProtocol.h
//  owen
//
//  Created by ff on 16/8/9.
//
//

#ifndef ELProtocol_h
#define ELProtocol_h

#include "cocos2d.h"
#include "GameDefine.h"
//#include "BlackDrop.h"
//#include "data/BlackArtifact.h"
enum E2LProtocol
{
    e2l_login_click,                                    //  点击登录界面
    e2l_check_net,                                      //  检查网络
    e2l_click_unlock,                                   //  点击解锁屏幕按钮（测试）
    e2l_monster_hp_empty,                               //  某对手hp为0
    e2l_monster_die,                                    //  某对手死亡
    e2l_witness_die,                                    //  保护对象死亡
    e2l_hero_die,                                       //  英雄死亡
    e2l_time_over,                                      //  时间耗尽
    e2l_time_over_2,                                    //  时间耗尽显示完成
    e2l_fight_star,                                     //  获得星级评价
    e2l_witness_arrive,                                 //  护送目标到达目标地
    e2l_start_fight,                                    //  战场准备结束，战斗开始
    e2l_use_hp_bottle,                                  //  用血瓶
    e2l_add_combo,                                      //  加combo
    e2l_clear_combo,                                    //  combo清零
    e2l_pause_fight,                                    //  暂停战斗
    e2l_resume_fight,                                   //  继续战斗
    
    e2l_click_chapter_name,                             //  点战场中的某章
    e2l_show_chapter,                                   //  点开章节界面
    e2l_show_stage,                                     //  点开关卡战备界面
    e2l_start_stage,                                    //  点战备界面开始按钮
    e2l_back_chapter,                                   //  点战备界面关闭按钮
    e2l_back_battle_chapters,                           //  点章节界面关闭按钮
    e2l_back_stage,                                     //  返回战备界面
    e2l_click_win,                                      //  胜利界面点击屏幕返回到章节界面
    e2l_click_revive,                                   //  复活界面点ok
    e2l_click_not_revive,                               //  复活界面点关闭
    e2l_click_fail,                                     //  失败界面点击屏幕返回到战备界面
    e2l_click_star_box,                                 //  点星级评价宝箱
    
    e2l_click_main_battle,                              //  点主界面战场
    e2l_click_main_hero,                                //  点主界面英雄
    e2l_click_main_pet,                                 //  点主界面使魔
    e2l_click_main_pet_offer,                           //  点主界面悬赏
    e2l_click_main_holy_sword,                          //  点主界面神兵
    e2l_click_main_arms,                                //  点主界面纹章
    e2l_click_main_prov,                                //  点主界面试炼之地
    e2l_click_main_abyss,                               //  点主界面深渊
    e2l_click_main_tower,                               //  点主界面爬塔
    e2l_click_main_arena,                               //  点主界面竞技场
    e2l_click_main_castle,                              //  点主界面城堡
    e2l_click_main_campaign,                            //  点主界面活动
    e2l_click_main_lucky,                               //  点主界面抽卡
    e2l_click_main_shop,                                //  点主界面商城
    e2l_click_main_novice,                              //  点主界面新手礼包
    e2l_click_main_vip,                                 //  点主界面VIP
    e2l_click_main_discount,                            //  点主界面折扣礼包
    
    e2l_sel_equip_pos,                                  //  选择装备位
    e2l_click_equip_uplevel,                            //  点击装备位升级
    e2l_click_equip_upgrade,                            //  点击装备位进阶
    e2l_click_equip_item,                               //  点击装备升阶物品
    e2l_buy_equip_item,                                 //  点击购买升阶物品
    e2l_click_sweep_item,                               //  点击扫荡升阶物品
    e2l_start_sweep_item,                               //  点击开始扫荡
    
    e2l_sel_pet_pos,                                    //  选择使魔位
    e2l_click_pet_uplevel,                              //  点击使魔升级
    e2l_add_fight_pet,                                  //  选择使魔进入出战序列
    e2l_remove_fight_pet,                               //  把使魔移出出战序列
    
    e2l_update_all_offer_cd,                            //  更新所有悬赏的CD时间
    e2l_click_refresh_offer,                            //  花钻石刷新悬赏
    e2l_open_board_offer,                               //  打开悬赏
    e2l_add_offer_pet,                                  //  选择使魔进入准备
    e2l_remove_offer_pet,                               //  撤销准备中的使魔
    e2l_take_board_offer,                               //  点出发领取悬赏
    e2l_take_offer_reward,                              //  点进行中悬赏，领取奖励
    e2l_buy_offer,                                      //  用钻石加速进行中悬赏
    
    e2l_sel_sword,                                      //  选择神兵
    e2l_set_fight_sword,                                //  装备神兵
    e2l_click_sword_uplevel,                            //  升级神兵
    
    e2l_sel_arms,                                       //  选择纹章
    e2l_uplevel_arms,                                   //  升级纹章
    e2l_assemble_gem,                                   //  合成宝石
    e2l_fill_gem,                                       //  装备宝石
    e2l_remove_gem,                                     //  卸下宝石
    e2l_show_fill_board,                                //  显示装备宝石界面
    
    e2l_click_prov_start,                               //  点击开始试炼
    e2l_confirm_prov_fighter,                           //  确认挑战者
    e2l_prov_fight,                                     //  试炼开战
    e2l_prov_end_fight,                                 //  交战动画放完
    e2l_prov_end_win,                                   //  胜利动画放完
    e2l_prov_end_lose,                                  //  失败动画放完
    e2l_prov_reset,                                     //  点试炼的重置
    e2l_prov_buy_ticket,                                //  点购买挑战劵
    
    e2l_click_abyss_start,                              //  点击开始深渊
    e2l_confirm_start_abyss,                            //  动画完毕进深渊关卡
    e2l_show_abyss_reset,                               //  显示深渊重置界面
    e2l_click_abyss_reset,                              //  点深渊关卡重置按钮
    
    e2l_click_tower_sweep,                              //  点爬塔扫荡
    e2l_confirm_tower_sweep,                            //  开始爬塔扫荡
    e2l_click_tower_start,                              //  点爬塔挑战
    e2l_click_tower_reset,                              //  点爬塔重置
    e2l_tower_take_reward,                              //  点爬塔扫荡领取奖励
    e2l_click_tower_speedup,                            //  点爬塔加速
    e2l_get_tower_sweep_sec,                            //  获得爬塔扫荡时间
    
    e2l_update_arena_cd,                                //  更新挑战次数回复CD时间
    e2l_click_arena_division,                           //  点击竞技场评级按钮
    e2l_click_arena_start,                              //  点竞技场开始
    e2l_arena_start,                                    //  竞技场开始请求匹配对手
    e2l_arena_match_robot,                              //  竞技场匹配机器人
    e2l_arena_start_fight,                              //  竞技场进战斗
    e2l_click_add_arena_ticket,                         //  点击竞技场加挑战次数按钮
    e2l_arena_take_reward,                              //  竞技场领取奖励
    
    e2l_show_buy_vit,                                   //  显示购买体力界面
    e2l_buy_vit,                                        //  购买体力
    e2l_show_hall_buy_vit,                              //  显示大厅购买体力界面
    e2l_show_shop_gold,                                 //  显示商城金币
    
    e2l_trigger_open_layer,                             //  剧情打开界面触发
    e2l_trigger_setup_hero,                             //  剧情初始化主角触发
    e2l_trigger_pass_npc,                               //  剧情经过NPC触发
    e2l_trigger_create_monster,                         //  剧情创建怪物
    e2l_trigger_kill_monster,                           //  剧情杀死怪物
    e2l_trigger_victory,                                //  剧情战斗胜利
    e2l_trigger_click_button,                           //  剧情点击按钮
    e2l_trigger_unlock,                                 //  剧情解锁触发
    e2l_complete_plot,                                  //  完成一个剧情
    e2l_next_dialog,                                    //  下一个对话
    
    e2l_hall_in,                                        //  大厅显示就绪
    e2l_click_job,                                      //  点击职业
    e2l_confirm_job,                                    //  切换职业
    e2l_active_job,                                     //  激活职业
    e2l_change_name,                                    //  改名
    
    e2l_take_everyday_bound,                            //  领取7天奖励
    e2l_show_everyday,                                  //  显示7天子界面
    e2l_normal_draw,                                    //  普通抽卡
    e2l_special_draw_one,                               //  特殊抽卡
    e2l_special_draw_ten,                               //  特殊十次抽卡
    e2l_update_lucky_cd,                                //  更新抽卡cd
    e2l_buy_special_ticket,                             //  买特殊抽卡次数
    e2l_add_normal_ticket,                              //  增加普通抽卡次数
    e2l_show_first_recharge,                            //  显示首充子界面
    e2l_take_first_recharge,                            //  领取首充奖励
    e2l_show_campaign_active,                           //  显示每日活跃度任务
    e2l_click_active_jump,                              //  点每日活跃度任务前往
    e2l_click_active_bound,                             //  点每日活跃度任务领取奖励
    e2l_click_active_box,                               //  点每日活跃度宝箱
    e2l_show_bound_mission,                             //  点奖励中心
    e2l_click_free_diamond,                             //  点免费钻石领取
    e2l_take_mission_reward,                            //  点奖励任务领取
    e2l_click_mission_jump,                             //  点奖励任务前往
    e2l_show_month_assign,                              //  点月签
    e2l_take_month_assign_reward,                       //  点月签领取
    
    e2l_click_shop_button,                              //  点商城类型按钮
    e2l_shop_buy_package,                               //  购买商城礼包
    e2l_shop_package_detail,                            //  点商城礼包细节
    e2l_click_first_recharge,                           //  点首充领取按钮
    
    e2l_show_vip_detail,                                //  切换VIP特权
    e2l_take_vip_bound,                                 //  领取VIP每日奖励
    e2l_vip_jump_recharge,                              //  点前往充值
    
    e2l_update_discount_state,                          //  更新折扣店开关和倒计时
    e2l_get_vit_cd,                                     //  获得体力恢复时间
    
    e2l_exit,                                           //  android返回键
    e2l_max
};

enum L2EProtocol
{
    l2e_show_login,                                     //  显示登录界面
    l2e_show_connected,                                 //  显示网络连接状况提示
    l2e_show_system_load,                               //  显示系统加载界面
    l2e_update_system_load,                             //  更新系统加载条
    l2e_show_load,                                      //  显示加载界面
    l2e_update_load_stage,                              //  更新加载进度
    l2e_setup_map,                                      //  更新地图三层图片
    l2e_setup_map_effect,                               //  更新地图动画元素
    l2e_setup_hero,                                     //  创建初始英雄
    l2e_setup_witness,                                  //  创建保护对象
    l2e_setup_opponent,                                 //  创建对手
    l2e_setup_monster_team,                             //  创建对手小队
    l2e_show_fight,                                     //  显示战斗界面
    l2e_ready_victory,                                  //  胜利前的慢动作
    l2e_victory,                                        //  胜利
    l2e_show_revive,                                    //  显示复活界面
    l2e_revive,                                         //  复活
    l2e_fail,                                           //  失败
    l2e_arena_victory,                                  //  竞技场胜利
    l2e_arena_fail,                                     //  竞技场失败
    l2e_fight_time_over,                                //  显示时间耗尽
    l2e_fight_star,                                     //  显示关卡评星
    l2e_update_wave_count,                              //  更新波次
    l2e_unlock_view,                                    //  解锁镜头（测试）
    l2e_full_hero_hp,                                   //  补满血
    l2e_update_hp_bottle_price,                         //  更新血瓶价格
    l2e_update_combo,                                   //  更新combo
    l2e_clear_combo,                                    //  combo清零
    l2e_pause_fight,                                    //  暂停战斗
    l2e_resume_fight,                                   //  继续战斗
    l2e_preload_monster_res,                            //  预加载关卡怪物动画
    
//    l2e_show_chapter_scene,                             //  切换到战场总图场景
    l2e_show_battle_chapters,                           //  显示战场总图
    l2e_back_battle_chapters,                           //  返回战场总图
    l2e_show_chapter_name,                              //  战场总图中显示章的名字
    l2e_show_chapter_stages,                            //  显示章节总图
    l2e_show_stage,                                     //  显示关卡战备
    l2e_start_stage,                                    //  确定是否能开启关卡
    l2e_open_star_box,                                  //  打开星级评价宝箱
    l2e_show_star_bounds,                               //  显示星级评价宝箱的奖励
    
    l2e_show_hall_scene,                                //  显示系统主界面
    l2e_show_hall_hero,                                 //  显示系统英雄界面
    l2e_show_hall_pet,                                  //  显示系统使魔界面
    l2e_show_hall_pet_offer,                            //  显示系统悬赏界面
    l2e_show_hall_sword,                                //  显示系统神兵界面
    l2e_show_hall_arms,                                 //  显示系统纹章界面
    l2e_show_hall_prov,                                 //  显示试炼之地界面
    l2e_show_hall_abyss,                                //  显示深渊界面
    l2e_show_hall_tower,                                //  显示爬塔界面
    l2e_show_hall_arena,                                //  显示竞技场界面
    l2e_show_hall_castle,                               //  显示城堡界面
    l2e_show_hall_everyday,                             //  显示7天界面
    l2e_show_hall_lucky,                                //  显示抽卡界面
    l2e_show_hall_shop,                                 //  显示商城界面
    l2e_show_hall_novice,                               //  显示新手礼包
    l2e_show_hall_vip,                                  //  显示VIP界面
    l2e_show_hall_discount,                             //  显示折扣店界面
    
    l2e_sel_equip_pos,                                  //  更新装备位详情
    l2e_update_equip_level,                             //  更新装备位等级
    l2e_update_equip_star,                              //  更新装备位星级
    l2e_update_item_count,                              //  更新装备升阶材料数量
    l2e_show_item_way,                                  //  显示装备升阶材料获取方式
    l2e_show_item_sweep,                                //  显示装备升阶材料扫荡界面
    l2e_item_sweep,                                     //  显示装备升阶材料扫荡结果
    l2e_update_equip_tip,                               //  更新装备提示
    
    l2e_sel_pet_pos,                                    //  更新使魔详情
    l2e_update_pet_level,                               //  升级后更新使魔详情
    l2e_update_fight_pet,                               //  更新出战使魔序列
    l2e_update_pet_tip,                                 //  更新使魔提示
    
    l2e_update_offer_all_cd,                            //  更新悬赏的所有CD时间
    l2e_update_board_offer,                             //  更新悬赏
    l2e_open_board_offer,                               //  打开任务，选择使魔
    l2e_add_offer_pet,                                  //  添加悬赏使魔
    l2e_remove_offer_pet,                               //  撤销准备中的使魔
    l2e_take_board_offer,                               //  领取悬赏，开始计时
    l2e_work_offer_over,                                //  完成悬赏，可领取奖励
    l2e_take_offer_reward,                              //  领取悬赏奖励，删除进行中悬赏
    l2e_show_buy_offer,                                 //  显示进行悬赏加速界面
    l2e_buy_offer,                                      //  加速完成悬赏
    l2e_reset_offer_refresh_count,
    
    l2e_update_sword,                                   //  更新神兵的详细信息
    l2e_update_fight_sword,                             //  更新神兵的装备标志
    l2e_update_sword_level,                             //  更新神兵等级
    
    l2e_update_arms,                                    //  更新纹章的详细信息
    l2e_update_arms_level,                              //  更新纹章的等级
    l2e_update_gems_count,                              //  更新宝石合成后的数量
    l2e_update_arms_gem,                                //  更新装备和卸下宝石的详细信息
    l2e_show_fill_board,                                //  显示装备宝石界面
    l2e_update_arms_tip,                                //  更新纹章
    
    l2e_prov_confirm_stage,                             //  确认试炼关卡
    l2e_prov_confirm_fighter,                           //  确认试炼挑战者
    l2e_prov_fight,                                     //  试炼战斗
    l2e_prov_win,                                       //  战斗胜利
    l2e_prov_lose,                                      //  战斗失败
    l2e_prov_reset,                                     //  重置试炼
    l2e_prov_add_ticket,                                //  增加挑战劵
    l2e_prov_refresh_reset,                             //  刷新重置次数
    
    l2e_ready_start_abyss,                              //  进深渊关卡，放动画，或错误提示
    l2e_show_abyss_reset,                               //  显示重置深渊界面
    l2e_reset_abyss,                                    //  重置深渊关卡
    
    l2e_show_tower_sweep,                               //  显示爬塔扫荡界面
    l2e_reset_tower,                                    //  重置爬塔关卡
    l2e_start_tower_sweep,                              //  开始爬塔扫荡
    l2e_complet_tower_sweep,                            //  完成爬塔扫荡计时
    l2e_tower_take_reward,                              //  领取爬塔扫荡奖励
    l2e_speedup_tower_sweep,                            //  加速完成爬塔扫荡
    l2e_update_tower_sweep_sec,                         //  更新爬塔扫荡倒计时
    
    l2e_show_arena_division,                            //  显示竞技场评级界面
    l2e_arena_start_match,                              //  显示匹配对手
    l2e_arena_show_robot,                               //  显示竞技场双方
    l2e_update_arena_ticket,                            //  更新竞技场挑战次数
    l2e_buy_arena_ticket,                               //  购买竞技场挑战次数
    l2e_update_arena_ticket_cd,                         //  更新竞技场挑战次数回复时间
    l2e_arena_reward,                                   //  显示竞技场段位领奖界面
    
    l2e_update_gold,                                    //  更新金币数
    l2e_update_diamond,                                 //  更新钻石数
    l2e_update_vit,                                     //  更新体力值
    l2e_update_exp,                                     //  更新经验，以及等级相关
    l2e_update_force,                                   //  更新战斗力
    l2e_update_hall_tip,                                //  更新提示
    l2e_update_vit_cd,                                  //  更新体力恢复时间
    
    l2e_show_hall_buy_vit,                              //  大厅显示购买体力界面
    l2e_show_buy_vit,                                   //  显示购买体力界面
    l2e_buy_vit,                                        //  购买体力
    l2e_update_vit_buy_count,                           //  刷新体力购买次数
    l2e_show_get,                                       //  显示或得提示
    
    l2e_start_plot,                                     //  发送剧情数据
    l2e_show_dialog,                                    //  显示对话数据
    l2e_update_dialog,                                  //  更新对话数据
    l2e_complete_dialog,                                //  结束对话
    l2e_complete_plot,                                  //  结束剧情
    l2e_add_npc,                                        //  添加临时NPC
    l2e_remove_npc,                                     //  删除临时NPC
    
    l2e_unlock_func,                                    //  解锁功能
    l2e_show_job,                                       //  显示职业说明
    l2e_change_job,                                     //  切换职业
    l2e_active_job,                                     //  激活职业
    l2e_active_all_job,                                 //  激活所有职业
    l2e_update_name,                                    //  更新名字
    
    l2e_show_everyday,                                  //  显示7天界面
    l2e_draw_normal_one,                                //  显示普通抽卡结果
    l2e_draw_special_one,                               //  显示特殊抽卡结果
    l2e_draw_special_ten,                               //  显示特殊十次抽卡结果
    l2e_update_draw_normal,                             //  更新免费普通抽卡
    l2e_update_draw_special,                            //  更新免费特殊抽卡
    l2e_update_lucky_cd,                                //  更新抽卡cd
    l2e_update_special_ticket,                          //  更新特殊抽卡次数
    l2e_update_normal_ticket,                           //  更新普通抽卡次数
    l2e_update_first_recharge,                          //  更新首充界面
    l2e_hide_campaign,                                  //  隐藏活动界面
    l2e_show_campaign_active,                           //  显示每日活跃度任务
    l2e_update_mission_bound,                           //  更新每日活跃度任务领奖
    l2e_update_active_box,                              //  更新每日活跃度宝箱
    l2e_refresh_campaign_active,                        //  更新每日活跃度任务
    l2e_update_free_diamond,                            //  更新免费钻石
    l2e_show_campaign_bound_mission,                    //  显示奖励中心界面
    l2e_update_campaign_tip,                            //  更新活动提示
    l2e_jump_locked,                                    //  显示跳转的功能未解锁
    l2e_show_month_assign,                              //  显示月签
    
    l2e_show_shop,                                      //  显示商城分类
    l2e_show_package_detail,                            //  显示礼包详情界面
    l2e_show_package_result,                            //  显示礼包结果
    l2e_show_custom_package,                            //  显示特殊礼包
    
    l2e_take_vip_bound,                                 //  领取vip奖励
    l2e_show_vip_detail,                                //  显示VIP特权
    l2e_hide_vip,                                       //  隐藏VIP界面
    
    l2e_update_discount_state,                          //  更新折扣店开关和倒计时
    l2e_show_unlock_job,                                //  显示解锁所有角色窗口
    
    l2e_max
};

typedef struct __L2E_SHOW_LOGIN
{
    L2EProtocol                 eProtocol;
}L2E_SHOW_LOGIN;//  显示登录或无网络界面

typedef struct __L2E_SHOW_LOAD
{
    L2EProtocol                 eProtocol;
    std::string                 loadTipImg;
}L2E_SHOW_LOAD;//  显示加载界面

typedef struct __E2L_CLICK_LOGIN
{
    E2LProtocol                 eProtocol;
}E2L_CLICK_LOGIN;//  点击登录或无网络界面

typedef struct __L2E_UPDATE_LOAD
{
    L2EProtocol                 eProtocol;
    bool                        isOver;
    int                         step;
    int                         count;
}L2E_UPDATE_LOAD;

typedef struct __L2E_SHOW_FIGHT
{
    L2EProtocol                 eProtocol;
    std::string                 tmxPath;
    std::string                 weatherPath;
    std::string                 musicFile;
    int                         heroId;
    float                       totalTime;
    int                         currWave;
    int                         totalWave;
    int                         battleType;
    int                         bossTeamId;
    std::string                 goalStr;
    bool                        stageFight;
}L2E_SHOW_FIGHT;

typedef struct __L2E_SETUP_MAP
{
    L2EProtocol                 eProtocol;
    std::string                 tmxPath;
}L2E_SETUP_MAP;// 更新地图三层图片

typedef struct __L2E_SETUP_HERO
{
    L2EProtocol                 eProtocol;
    int                         configId;
    int                         idInStage;
    std::string                 tmxPath;
    std::string                 armatureFile;
    float                       moveSpeed;
    float                       rushSpeed;
    float                       scaleFactor;
    int                         hp;
    int                         attack;
    int                         defence;
    int                         crit;
    int                         deCrit;
    int                         buffId[5];
    int                         petId[3];
    int                         holySwordId;
}L2E_SETUP_HERO;// 创建初始英雄

typedef struct __L2E_SETUP_WITNESS
{
    L2EProtocol                 eProtocol;
    int                         configId;
    std::string                 tmxPath;
}L2E_SETUP_WITNESS;

typedef struct __L2E_PRELOAD_MONSTER_RES
{
    L2EProtocol                 eProtocol;
    std::string                 tmxPath;
}L2E_PRELOAD_MONSTER_RES;

//typedef struct __L2E_SETUP_MONSTER
//{
//    L2EProtocol                 eProtocol;
//    std::string                 armatureFile;
//    std::string                 machineFile;
//    float                       moveSpeed;
//    float                       rushSpeed;
//    float                       scaleFactor;
//    int                         atkSkillId;
//}L2E_SETUP_MONSTER;// 创建初始对手
struct TeamMate{
    int                         monId;
    int                         teamId;
    int                         posType;
    bool                        fromTmx;
    bool                        opponentFlag;
    float                       x;
    int                         buffId[5];
};
typedef struct __L2E_SETUP_MONSTER_TEAM
{
    L2EProtocol                 eProtocol;
    std::vector<TeamMate>       team;
}L2E_SETUP_MONSTER_TEAM;

typedef struct __E2L_COMMON {
    E2LProtocol                 eProtocol;
}E2L_COMMON;// 无参数通用显示层到逻辑层数据
typedef struct __L2E_COMMON
{
    L2EProtocol                 eProtocol;
}L2E_COMMON;// 无参数通用逻辑层到显示层数据
typedef struct __E2L_UPDATE_ONE_VALUE
{
    E2LProtocol                 eProtocol;
    int                         value;
}E2L_UPDATE_ONE_VALUE;// 通用显示层到逻辑层提交一个数据

typedef struct __L2E_UPDATE_ONE_VALUE
{
    L2EProtocol                 eProtocol;
    int                         value;
}L2E_UPDATE_ONE_VALUE;// 通用逻辑层到显示层更新一个数据
typedef struct __L2E_UPDATE_VIT
{
    L2EProtocol                 eProtocol;
    int                         currVIT;
    int                         maxVIT;
}L2E_UPDATE_VIT;

typedef struct __L2E_UPDATE_FORCE
{
    L2EProtocol                 eProtocol;
    int                         forceVal;
    int                         upVal;
}L2E_UPDATE_FORCE;

typedef struct __L2E_UPDATE_ARENA_TICKET
{
    L2EProtocol                 eProtocol;
    int                         currCount;
    int                         maxCount;
}L2E_UPDATE_ARENA_TICKET;

typedef  struct __E2L_MONSTER_HP_EMPTY
{
    E2LProtocol                 eProtocol;
    int                         teamId;
    int                         monId;
}E2L_MONSTER_HP_EMPTY;

typedef struct __E2L_MONSTER_DIE
{
    E2LProtocol                 eProtocol;
    int                         teamId;
    int                         monId;
}E2L_MONSTER_DIE;

typedef struct __L2E_FIGHT_TIME_OVER
{
    L2EProtocol                 eProtocol;
    int                         teamId;
}L2E_FIGHT_TIME_OVER;

typedef struct __E2L_FIGHT_TIME_OVER2
{
    E2LProtocol                 eProtocol;
    int                         heroHp;
    int                         heroMaxHp;
    int                         opponentHp;
    int                         opponentMaxHp;
}E2L_FIGHT_TIME_OVER2;

typedef struct __E2L_FIGHT_STAR
{
    E2LProtocol                 eProtocol;
    int                         heroHp;
    int                         heroMaxHp;
    int                         leftSecond;
}E2L_FIGHT_STAR;

typedef struct __L2E_FIGHT_STAR
{
    L2EProtocol                 eProtocol;
    int                         normal;
    int                         star[3];
    int                         starType[3];
    int                         thresVal[3];
    int                         boundGold;
    int                         boundExp;
    int                         firstGold;
    int                         firstExp;
    std::string                 boundIcon;
    int                         boundIconCount;
}L2E_FIGHT_STAR;
typedef struct __L2E_UPDATE_WAVE_COUNT
{
    L2EProtocol                 eProtocol;
    int                         curr;
    int                         total;
}L2E_UPDATE_WAVE_COUNT;
typedef struct __E2L_CLICK_WIN
{
    E2LProtocol                 eProtocol;
    bool                        toHall;
}E2L_CLICK_WIN;

typedef struct __L2E_UPDATE_HP_BOTTLE_PRICE
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    int                         price;
}L2E_UPDATE_HP_BOTTLE_PRICE;

typedef struct __E2L_ADD_COMBO
{
    E2LProtocol                 eProtocol;
    int                         count;
}E2L_ADD_COMBO;

typedef struct __L2E_ADD_COMBO
{
    L2EProtocol                 eProtocol;
    int                         count;
    int                         curr;
    int                         max;
    int                         factor;
}L2E_ADD_COMBO;

typedef struct __L2E_SHOW_BATTLE_CHAPTERS
{
    L2EProtocol                 eProtocol;
    int                         maxOpenId;
    int                         focusId;
    std::string                 imgFile;
}L2E_SHOW_BATTLE_CHAPTERS;

typedef struct __E2L_CLICK_CHAPTER_NAME
{
    E2LProtocol                 eProtocol;
    int                         tag;
}E2L_CLICK_CHAPTER_NAME;

typedef struct __L2E_SHOW_CHAPTER_NAME
{
    L2EProtocol                 eProtocol;
    int                         chapterId;
    std::string                 imgFile;
}L2E_SHOW_CHAPTER_NAME;

typedef struct __E2L_SHOW_CHAPTER
{
    E2LProtocol                 eProtocol;
    int                         tag;
}E2L_SHOW_CHAPTER;

typedef struct __STAGE_SUBINFO
{
    int                         stageId;
    std::string                 nameImg;
    std::string                 iconImg;
    int                         star[3];
}STAGE_SUBINFO;
typedef struct __L2E_SHOW_CHAPTER_STAGES
{
    L2EProtocol                 eProtocol;
    std::string                 chapterImg;
    int                         totalStarCount;
    int                         boxState[3];
    std::vector<STAGE_SUBINFO>  stages;
}L2E_SHOW_CHAPTER_STAGES;

typedef struct __L2E_SHOW_STAR_BOUNDS
{
    L2EProtocol                 eProtocol;
    std::string                 icon[3];
    int                         count[3];
}L2E_SHOW_STAR_BOUNDS;

typedef struct __E2L_SHOW_STAGE
{
    E2LProtocol                 eProtocol;
    int                         stageId;
}E2L_SHOW_STAGE;

typedef struct __L2E_SHOW_STAGE{
    L2EProtocol                 eProtocol;
    int                         stageId;
    std::string                 nameImg;
    std::string                 heroImg;
    int                         star[3];
    int                         starType[3];
    int                         thresVal[3];
    int                         currVIT;
    int                         maxVIT;
    int                         boundGold;
    int                         boundExp;
    int                         firstGold;
    int                         firstExp;
}L2E_SHOW_STAGE;

typedef struct __L2E_SHOW_HALL
{
    L2EProtocol                 eProtocol;
    int                         vipLevel;
    bool                        vipTip;
    int                         totalForce;
    int                         level;
    std::string                 headIcon;
    std::string                 name;
    bool                        jobTip;
    int                         gold;
    int                         diamond;
    int                         VIT;
    int                         maxVIT;
    bool                        abyssLock;
    bool                        towerLock;
    bool                        pvpLock;
    bool                        provLock;
//    int                         onlineCountDown;
    bool                        rewardTip;
    bool                        soulTip;
    bool                        monsterTip;
    bool                        skillTip;
    bool                        heroTip;
    bool                        armsTip;
    bool                        noviceTip;
    bool                        luckyTip;
    bool                        firstTip;
    int                         lock[11];
    int                         lockType[11];
    int                         tip[8];
    float                       onlineDuration;
    int                         freeDiamondState;
    bool                        discountTip;
    int                         autoPopLayerId;
}L2E_SHOW_HALL;

typedef struct __L2E_SHOW_HALL_HERO
{
    L2EProtocol                 eProtocol;
    int                         level;
    int                         exp;
    int                         maxExp;
    int                         force;
    std::string                 armature;
    int                         totalHp;
    int                         totalAttack;
    int                         totalDefence;
    int                         totalCrit;
    int                         totalDeCrit;
    int                         totalGold;
    int                         equipId[6];
    std::string                 equipIcon[6];
    int                         equipLevel[6];
    int                         equipStar[6];
    int                         currPos;
    int                         currEquipLevel;
    std::string                 currEquipIcon;
    std::string                 currEquipName;
    int                         currEquipStar;
    int                         currEquipHp;
    int                         plusEquipHp;
    int                         currEquipAttack;
    int                         plusEquipAttack;
    int                         currEquipDefence;
    int                         plusEquipDefence;
    int                         costGold;
    bool                        maxLevel;
//    int                         starItemId[6];
    std::string                 starItemIcon[6];
    int                         starItemRequire[6];
    int                         starItemCount[6];
    bool                        maxGrade;
    
    std::vector<bool>           posTip;
    std::vector<bool>           posUplevelTip;
    std::vector<bool>           posUpgradeTip;
}L2E_SHOW_HALL_HERO;

typedef struct __E2L_SEL_EQUIP_POS
{
    E2LProtocol                 eProtocol;
    int                         pos;
}E2L_SEL_EQUIP_POS;

typedef struct __L2E_SEL_EQUIP_POS
{
    L2EProtocol                 eProtocol;
    int                         currPos;
    int                         currEquipLevel;
    std::string                 currEquipIcon;
    std::string                 currEquipName;
    int                         currEquipStar;
    int                         currEquipHp;
    int                         plusEquipHp;
    int                         currEquipAttack;
    int                         plusEquipAttack;
    int                         currEquipDefence;
    int                         plusEquipDefence;
    int                         costGold;
    bool                        maxLevel;
    //    int                         starItemId[6];
    std::string                 starItemIcon[6];
    int                         starItemRequire[6];
    int                         starItemCount[6];
    bool                        maxGrade;
    
    std::vector<bool>           posUplevelTip;
    std::vector<bool>           posUpgradeTip;

}L2E_SEL_EQUIP_POS;

typedef struct __L2E_UPDATE_EQUIP_LEVEL
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    int                         currPos;
    int                         currEquipLevel;
    int                         currEquipHp;
    int                         plusEquipHp;
    int                         currEquipAttack;
    int                         plusEquipAttack;
    int                         currEquipDefence;
    int                         plusEquipDefence;
    int                         costGold;
    bool                        maxLevel;
    
    int                         force;
    int                         totalHp;
    int                         totalAttack;
    int                         totalDefence;
}L2E_UPDATE_EQUIP_LEVEL;

typedef struct __L2E_UPDATE_EQUIP_STAR
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    int                         force;
    int                         totalHp;
    int                         totalAttack;
    int                         totalDefence;
    int                         currPos;
    std::string                 currEquipIcon;
    std::string                 currEquipName;
    int                         currEquipStar;
    int                         currEquipHp;
    int                         currEquipAttack;
    int                         currEquipDefence;
    
    std::string                 starItemIcon[6];
    int                         starItemRequire[6];
    int                         starItemCount[6];
    bool                        maxGrade;

}L2E_UPDATE_EQUIP_STAR;

typedef struct __L2E_SHOW_ITEM_WAY
{
    L2EProtocol                 eProtocol;
    int                         itemTag;
    int                         count;
    int                         price;
    int                         totalDiamond;
}L2E_SHOW_ITEM_WAY;

typedef struct __E2L_BUY_EQUIP_ITEM
{
    E2LProtocol                 eProtocol;
    int                         count;
    int                         itemTag;
}E2L_BUY_EQUIP_ITEM;

typedef struct __L2E_UPDATE_ITEM_COUNT
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    int                         itemTag;
    int                         count;
    int                         requireCount;
    int                         totalDiamond;
}L2E_UPDATE_ITEM_COUNT;

typedef struct __L2E_SHOW_ITEM_SWEEP
{
    L2EProtocol                 eProtocol;
    int                         itemTag;
    int                         dropGold;
    int                         dropExp;
    int                         dropItemCount;
    std::string                 icon;
    int                         requireCount;
    int                         currVIT;
    int                         maxVIT;
}L2E_SHOW_ITEM_SWEEP;

typedef struct __E2L_START_ITEM_SWEEP
{
    E2LProtocol                 eProtocol;
    int                         count;
    int                         itemTag;
}E2L_START_ITEM_SWEEP;
typedef struct __L2E_ITEM_SWEEP
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    int                         count;
    bool                        getItem[10];
}L2E_ITEM_SWEEP;

typedef struct __L2E_UPDATE_EXP
{
    L2EProtocol                 eProtocol;
    int                         currExp;
    int                         maxExp;
    int                         level;
    bool                        upLevel;
    int                         hp;
    int                         attack;
    int                         defence;
    int                         crit;
    int                         deCrit;
    int                         force;
}L2E_UPDATE_EXP;

typedef struct __L2E_SHOW_HALL_PET
{
    L2EProtocol                 eProtocol;
    std::string                 icon[10];
    std::string                 name[10];
    int                         petId[10];
    int                         petLevel[10];
    int                         fightPetId[3];
    
    int                         currPetId;
    std::string                 currPetImg;
    std::string                 currPetSkillName;
    int                         currPetFragCount;
    int                         currPetMaxFragCount;
    int                         currPetLevel;
    int                         currPetLevelhp;
    int                         currPetLevelAttack;
    int                         currPetLevelDefence;
    bool                        currPetMaxLevel;
    
    int                         totalHp;
    int                         totalAttack;
    int                         totalDefence;

    std::vector<int>            uplevelTip;
}L2E_SHOW_HALL_PET;

typedef struct __L2E_SEL_PET_POS
{
    L2EProtocol                 eProtocol;
    int                         currPetId;
    std::string                 currPetImg;
    std::string                 currPetSkillName;
    int                         currPetFragCount;
    int                         currPetMaxFragCount;
    int                         currPetLevel;
    int                         currPetLevelhp;
    int                         currPetLevelAttack;
    int                         currPetLevelDefence;
    bool                        currPetMaxLevel;
    std::vector<int>            uplevelTip;
}L2E_SEL_PET_POS;

typedef struct __L2E_UPDATE_PET_LEVEL
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    int                         currPetId;
    std::string                 currPetImg;
    std::string                 currPetSkillName;
    int                         currPetFragCount;
    int                         currPetMaxFragCount;
    int                         currPetLevel;
    int                         currPetLevelhp;
    int                         currPetLevelAttack;
    int                         currPetLevelDefence;
    bool                        currPetMaxLevel;
    
    int                         totalHp;
    int                         totalAttack;
    int                         totalDefence;

}L2E_UPDATE_PET_LEVEL;

typedef struct __L2E_UPDATE_FIGHT_PET
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    std::string                 icon[3];
    int                         fightPetId[3];

}L2E_UPDATE_FIGHT_PET;

typedef struct __L2E_SHOW_HALL_PET_OFFER
{
    L2EProtocol                 eProtocol;
    int                         workOfferType[3];
    int                         workOfferState[3];
//    int                         workOfferCD[3];
    int                         boardOfferType[3];
    std::string                 boundIcons[3][3];
    int                         boundCount[3][3];
    int                         boundType[3][3];
    int                         boardOfferCostTime[3];
    int                         boardOfferState[3];
    int                         refreshBoardPrice;
//    int                         refreshCD;
}L2E_SHOW_HALL_PET_OFFER;

typedef struct __L2E_UPDATE_ALL_OFFER_CD
{
    L2EProtocol                 eProtocol;
    long                         refreshCD;
    long                        workOfferCD[3];
}L2E_UPDATE_ALL_OFFER_CD;

typedef struct __L2E_UPDATE_LUCKY_CD
{
    L2EProtocol                 eProtocol;
    long                        normalCD;
    long                        specialCD;
}L2E_UPDATE_LUCKY_CD;

typedef struct __L2E_UPDATE_BOARD_OFFER
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    int                         boardOfferType[3];
    std::string                 boundIcons[3][3];
    int                         boundCount[3][3];
    int                         boundType[3][3];
    int                         boardOfferCostTime[3];
    int                         boardOfferState[3];
    int                         refreshCD;
    int                         refreshPrice;
}L2E_UPDATE_BOARD_OFFER;

typedef struct __L2E_OPEN_BOARD_OFFER
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    int                         offerPos;
    int                         offerId;
    std::vector<int>            workPet;
    int                         offerType;
    int                         needCount;
    std::string                 petsIcon[10];
    bool                        unlock[10];
}L2E_OPEN_BOARD_OFFER;

typedef struct __E2L_ADD_OFFER_PET
{
    E2LProtocol                 eProtocol;
    int                         readyPets[3];
    int                         petId;
    int                         offerId;
}E2L_ADD_OFFER_PET;

typedef struct __L2E_ADD_OFFER_PET
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    int                         petId;
    std::string                 petIcon;
    std::vector<int>            relationPets;
}L2E_ADD_OFFER_PET;

typedef struct __E2L_REMOVE_OFFER_PET
{
    E2LProtocol                 eProtocol;
    int                         readyPets[3];
    int                         offerId;
}E2L_REMOVE_OFFER_PET;

typedef struct __L2E_REMOVE_OFFER_PET
{
    L2EProtocol                 eProtocol;
    std::vector<int>            relationPets;
}L2E_REMOVE_OFFER_PET;

typedef struct __E2L_TAKE_BOARD_OFFER
{
    E2LProtocol                 eProtocol;
    int                         offerPos;
    int                         petsId[3];
}E2L_TAKE_BOARD_OFFER;

typedef struct __L2E_TAKE_BOARD_OFFER
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    int                         boardOfferPos;
    int                         workOfferPos;
    int                         workOfferType;
    long                        workOfferCD;
}L2E_TAKE_BOARD_OFFER;

typedef struct __L2E_TAKE_OFFER_REWARD
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    int                         workOfferPos;
    std::vector<int>            workPet;
}L2E_TAKE_OFFER_REWARD;

typedef struct __L2E_SHOW_BUY_OFFER
{
    L2EProtocol                 eProtocol;
    int                         workOfferPos;
    int                         workOfferType;
    std::string                 petIcons[3];
    int                         boundType[3];
    int                         boundCount[3];
    std::string                 boundIcons[3];
    int                         price;
}L2E_SHOW_BUY_OFFER;

typedef struct __L2E_BUY_OFFER
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    int                         workOfferPos;
    std::vector<int>            workPet;
}L2E_BUY_OFFER;

typedef struct __L2E_SHOW_HOLY_SWORD
{
    L2EProtocol                 eProtocol;
    std::string                 swordName[5];
    bool                        swordLock[5];
    int                         fightSwordId;
    int                         currSwrodId;
    std::string                 currSwordImg;
    int                         currSwordLevel;
    int                         nextSwordLockLevel;
    int                         totalHp;
    int                         totalAttack;
    int                         totalDefence;
    int                         totalGold;
    int                         totalStone;
    bool                        maxLevel;
    int                         nextHp;
    int                         nextAttack;
    int                         nextDefence;
    int                         costGold;
    int                         costStone;
    int                         tipIndex;
}L2E_SHOW_HOLY_SWORD;

typedef struct __L2E_UPDATE_SWORD
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    int                         currSwordId;
    std::string                 currSwordImg;
    int                         currSwordLevel;
    int                         nextSwordLockLevel;
    bool                        maxLevel;
    int                         nextHp;
    int                         nextAttack;
    int                         nextDefence;
    int                         costGold;
    int                         costStone;
    int                         tipIndex;
}L2E_UPDATE_SWORD;

typedef struct __L2E_UPDATE_FIGHT_SWORD
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    int                         fightSwordId;

}L2E_UPDATE_FIGHT_SWORD;

typedef struct __L2E_UPDATE_SWORD_LEVEL
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    bool                        swordLock[5];
    int                         currSwordLevel;
    int                         nextSwordLockLevel;
    int                         totalHp;
    int                         totalAttack;
    int                         totalDefence;
    
    int                         totalStone;
    bool                        maxLevel;
    int                         nextHp;
    int                         nextAttack;
    int                         nextDefence;
    int                         costGold;
    int                         costStone;
    int                         tipIndex;
}L2E_UPDATE_SWORD_LEVEL;

typedef struct __L2E_SHOW_HALL_ARMS
{
    L2EProtocol                 eProtocol;
    std::string                 armsNames[5];
    int                         totalHp;
    int                         totalAttack;
    int                         totalDefence;
    int                         totalCrit;
    int                         totalDecrit;
    
    int                         currArmsId;
    std::string                 armsIcon;
    int                         armsType;
    int                         level;
    int                         armsValue;
    bool                        maxLevel;
    int                         currFame;
    int                         costFame;
    int                         nextLevelValue;
    
    std::string                 posGemIcon[6];
    int                         lockPos[6];
    int                         unlockLevel[6];
    std::string                 gemsIcon[6];
    int                         gemCount[6];
    
    std::vector<bool>           armsTip;
    std::vector<bool>           armsFameTip;
    std::vector<int>            gemTip;
}L2E_SHOW_HALL_ARMS;

typedef struct __L2E_UPDATE_ARMS
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    int                         currArmsId;
    std::string                 armsIcon;
    int                         armsType;
    int                         level;
    int                         armsValue;
    
    bool                        maxLevel;
    int                         currFame;
    int                         costFame;
    int                         nextLevelValue;
    
    std::string                 posGemIcon[6];
    int                         lockPos[6];
    int                         unlockLevel[6];
    
    std::string                 gemsIcon[6];
    int                         gemCount[6];
    std::vector<bool>           armsFameTip;
    std::vector<int>            gemTip;
}L2E_UPDATE_ARMS;

typedef struct __L2E_UPDATE_ARMS_LEVEL
{
    L2EProtocol                 eProtocol;
    int                         errNo;

    int                         totalHp;
    int                         totalAttack;
    int                         totalDefence;
    int                         totalCrit;
    int                         totalDecrit;

    int                         level;
    int                         armsValue;
    bool                        maxLevel;
    int                         currFame;
    int                         costFame;
    int                         nextLevelValue;
    int                         lockPos[6];
    std::string                 posGemIcon[6];
    int                         unlockLevel[6];
}L2E_UPDATE_ARMS_LEVEL;

typedef struct __L2E_UPDATE_GEM_COUNT
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    int                         gemCount[6];
    int                         nextGemPos;
}L2E_UPDATE_GEM_COUNT;

typedef struct __L2E_UPDATE_ARMS_GEM
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    
    int                         totalHp;
    int                         totalAttack;
    int                         totalDefence;
    int                         totalCrit;
    int                         totalDecrit;

    int                         armsValue;
    std::string                 posGemIcon[6];
    int                         gemCount[6];
    int                         lockPos[6];
    int                         unlockLevel[6];
}L2E_UPDATE_ARMS_GEM;

typedef struct __L2E_SHOW_FILL_GEM
{
    L2EProtocol                 eProtocol;
    std::string                 gemIcon;
    int                         gemCount;
    int                         gemType;
    int                         gemValue;
}L2E_SHOW_FILL_GEM;

typedef struct __L2E_SHOW_HALL_PROV
{
    L2EProtocol                 eProtocol;
    int                         ticketCount;
    int                         resetCount;
    
    int                         fighterId[10];
    std::string                 fighterIcon[10];
    int                         fighterLock[10];
    int                         fighterAlive[10];
    
    int                         stage;
    int                         petHp;
    int                         petMaxHp;
    std::string                 petIcon;
    int                         petLevel;
    int                         boundGold;
    int                         boundStone;
    
    std::vector<std::string>    stageIcon;
}L2E_SHOW_HALL_PROV;

typedef struct __L2E_PROV_RESET
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    int                         resetCount;
    
    int                         fighterLock[10];
    int                         fighterAlive[10];
    
    int                         stage;
    int                         petHp;
    int                         petMaxHp;
    std::string                 petIcon;
    int                         petLevel;
    int                         boundGold;
    int                         boundStone;
}L2E_PROV_RESET;

typedef struct __L2E_PROV_CONFIRM_STAGE
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    int                         stage;
    int                         petHp;
    int                         petMaxHp;
    std::string                 petIcon;
    int                         petLevel;
    int                         boundGold;
    int                         boundStone;
}L2E_PROV_CONFIRM_STAGE;

typedef struct __L2E_PROV_CONFIRM_FIGHTER
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    int                         fighterId;
    std::string                 fighterIcon;
    int                         fighterHp;
    int                         fighterMaxHp;
    int                         fighterLevel;
    int                         ticketCount;
}L2E_PROV_CONFIRM_FIGHTER;

typedef struct __L2E_PROV_FIGHT
{
    L2EProtocol                 eProtocol;
    int                         attackDirect;
    int                         defenceHp;
    int                         defenceMaxHp;
}L2E_PROV_FIGHT;

typedef struct __L2E_PROV_LOSE
{
    L2EProtocol                 eProtocol;
    int                         fighterId;
}L2E_PROV_LOSE;

typedef struct __L2E_PROV_WIN
{
    L2EProtocol                 eProtocol;
    int                         boundGold;
    int                         boundStone;
}L2E_PROV_WIN;

typedef struct __L2E_PROV_ADD_TICKET
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    int                         ticketCount;
}L2E_PROV_ADD_TICKET;

typedef struct __L2E_SHOW_BUY_VIT
{
    L2EProtocol                 eProtocol;
    int                         buyCount;
    int                         price;
}L2E_SHOW_BUY_VIT;

typedef struct __L2E_BUY_VIT
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    int                         buyCount;
}L2E_BUY_VIT;

typedef struct __L2E_SHOW_GET
{
    L2EProtocol                 eProtocol;
    std::string                 icon[20];
    std::string                 name[20];
    int                         count[20];
    bool                        frag[20];
}L2E_SHOW_GET;

typedef struct __L2E_SHOW_HALL_ABYSS
{
    L2EProtocol                 eProtocol;
    int                         abyssId[10];
    std::string                 abyssNameImg[10];
    std::string                 abyssAnimation[10];
    std::string                 petFragIcon[10];
    int                         petFragCount[10];
    int                         boundGold[10];
    int                         boundExp[10];
    int                         openAbyssCount;
    int                         fightCount[10];
    int                         currVIT;
    int                         maxVIT;
    int                         focus;
}L2E_SHOW_HALL_ABYSS;

typedef struct __L2E_RESET_ABYSS
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    int                         abyssId;
    int                         fightCount;
}L2E_RESET_ABYSS;

typedef struct __L2E_SHOW_ABYSS_RESET
{
    L2EProtocol                 eProtocol;
    int                         price;
    int                         resetCount;
    int                         abyssId;
}L2E_SHOW_ABYSS_RESET;

typedef struct __L2E_SHOW_HALL_TOWER
{
    L2EProtocol                 eProtocol;
    int                         totalStage;
    int                         openCount;
    int                         currStage;
    std::string                 stageName;
    std::string                 name;
    std::string                 animationFile;
    std::string                 descript;
    int                         rewardGold;
    int                         rewardExp;
    int                         rewardGemCount;
    std::string                 rewardGemIcon;
    
    int                         sweepState;
    int                         sweepSec;
    int                         speedupPrice;
    std::map<std::string, int>  sweepRewards;
}L2E_SHOW_HALL_TOWER;

typedef struct __L2E_SHOW_TOWER_SWEEP
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    int                         sweepState;
    int                         sweepSec;
    int                         speedupPrice;
    std::map<std::string, int>  sweepRewards;
}L2E_SHOW_TOWER_SWEEP;

typedef struct __L2E_RESET_TOWER
{
    L2EProtocol                 eProtocol;
    int                         errNo;

    int                         currStage;
    std::string                 stageName;
    std::string                 name;
    std::string                 animationFile;
    std::string                 descript;
    int                         rewardGold;
    int                         rewardExp;
    int                         rewardGemCount;
    std::string                 rewardGemIcon;

}L2E_RESET_TOWER;

typedef struct __L2E_START_TOWER_SWEEP
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    
    int                         sweepState;
    int                         sweepSec;
    int                         speedupPrice;
}L2E_START_TOWER_SWEEP;

typedef struct __L2E_COMPLETE_TOWER_SWEEP
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    
    int                         sweepState;

}L2E_COMPLETE_TOWER_SWEEP;

typedef struct __L2E_TOWER_TAKE_REWARD
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    
    int                         sweepState;
    int                         currStage;
    std::string                 stageName;
    std::string                 name;
    std::string                 animationFile;
    std::string                 descript;
    int                         rewardGold;
    int                         rewardExp;
    int                         rewardGemCount;
    std::string                 rewardGemIcon;
}L2E_TOWER_TAKE_REWARD;

typedef struct __L2E_SPEEDUP_TOWER_SWEEP
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    int                         sweepState;
}L2E_SPEEDUP_TOWER_SWEEP;

typedef struct __L2E_SHOW_HALL_ARENA
{
    L2EProtocol                 eProtocol;
    int                         place;
    int                         score;
    int                         force;
    std::string                 divisionIcon1;
    std::string                 divisionIcon2;
    std::string                 petIcon[3];
    int                         petLevel[3];
    std::string                 swordIcon;
    int                         ticketCount;
    int                         maxTicketCount;
    int                         cd;
    
    int                         robotPlace[20];
    int                         robotLevel[20];
    std::string                 robotJobIcon[20];
    std::string                 robotName[20];
    int                         robotScore[20];
    int                         robotForce[20];
    std::string                 robotPetIcon[20][3];
    int                         robotPetLevel[20][3];
    std::string                 robotSwordIcon[20];
    
    int                         rewardState;
    int                         rewardGold;
    int                         rewardFame;
}L2E_SHOW_HALL_ARENA;

typedef struct __L2E_SHOW_ARENA_DIVISION
{
    L2EProtocol                 eProtocol;
    std::string                 icon1[30];
    std::string                 icon2[30];
    int                         minScore[30];
    int                         maxScore[30];
    int                         goldCount[30];
    int                         fameCount[30];
}L2E_SHOW_ARENA_DIVISION;

typedef struct __L2E_ARENA_SHOW_ROBOT
{
    L2EProtocol                 eProtocol;
    std::string                 heroJobIcon;
    std::string                 heroName;
    int                         heroForce;
    std::string                 heroPetImg[3];
    std::string                 heroPetName[3];
    std::string                 heroSwordImg;
    std::string                 heroSwordName;
    
    std::string                 oppJobIcon;
    std::string                 oppName;
    int                         oppForce;
    std::string                 oppPetImg[3];
    std::string                 oppPetName[3];
    std::string                 oppSwordImg;
    std::string                 oppSwordName;

}L2E_ARENA_SHOW_ROBOT;

typedef struct __L2E_SETUP_OPPONENT
{
    L2EProtocol                 eProtocol;
    std::string                 name;
    int                         jobId;
    int                         fighterId;
    int                         holySwordId;
    int                         petId[3];
    int                         hp;
    int                         attack;
    int                         defence;
    int                         crit;
    int                         decrit;
}L2E_SETUP_OPPONENT;

typedef struct __L2E_ARENA_RESULT
{
    L2EProtocol                 eProtocol;
    int                         gold;
    int                         totalScore;
    int                         deltaScore;
    int                         changeDivision;
    std::string                 divisionIcon1;
    std::string                 divisionIcon2;
}L2E_ARENA_RESULT;

typedef struct __L2E_ARENA_REWARD
{
    L2EProtocol                 eProtocol;
    int                         gold;
    int                         fame;
    std::string                 divisionIcon1;
    std::string                 divisionIcon2;
}L2E_ARENA_REWARD;

typedef struct __E2L_TRIGGER_OPEN_LAYER
{
    E2LProtocol                 eProtocol;
    LAYER_INDEX                 index;
}E2L_TRIGGER_OPEN_LAYER;

typedef struct __E2L_TRIGGER_PASS_NPC
{
    E2LProtocol                 eProtocol;
    int                         npcId;
}E2L_TRIGGER_PASS_NPC;

typedef struct __E2L_TRIGGER_CREATE_MONSTER
{
    E2LProtocol                 eProtocol;
    int                         monsterId;
}E2L_TRIGGER_CREATE_MONSTER;

typedef struct __E2L_TRIGGER_KILL_MONSTER
{
    E2LProtocol                 eProtocol;
    int                         monsterId;
}E2L_TRIGGER_KILL_MONSTER;

typedef struct __E2L_TRIGGER_CLICK_BUTTON
{
    E2LProtocol                 eProtocol;
    LAYER_INDEX                 index;
    int                         buttonId;
}E2L_TRIGGER_CLICK_BUTTON;

typedef struct __L2E_START_PLOT
{
    L2EProtocol                 eProtocol;
    int                         missionId;
    int                         type;
    int                         param[5];
}L2E_START_PLOT;

typedef struct __E2L_COMPLETE_PLOT
{
    E2LProtocol                 eProtocol;
    int                         missionId;
    int                         value;
}E2L_COMPLETE_PLOT;

typedef struct __L2E_SHOW_DIALOG
{
    L2EProtocol                 eProtocol;
    int                         direct;
    std::string                 icon;
    std::string                 name;
    std::string                 content;
}L2E_SHOW_DIALOG;

typedef struct __L2E_ADD_NPC
{
    L2EProtocol                 eProtocol;
    int                         npcId;
    std::string                 armaFile;
    int                         posX;
    int                         direct;
    int                         missionId;
}L2E_ADD_NPC;

typedef struct __L2E_REMOVE_NPC
{
    L2EProtocol                 eProtocol;
    int                         npcId;
    int                         missionId;
}L2E_REMOVE_NPC;

typedef struct __L2E_SHOW_HALL_CASTLE
{
    L2EProtocol                 eProtocol;
    int                         currJob;
    int                         jobLock[4];
    int                         jobActive[4];
    
    std::string                 jobImg;
    std::string                 jobDescript;
    std::string                 name;
}L2E_SHOW_HALL_CASTLE;

typedef struct __L2E_SHOW_JOB
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    int                         currJob;
    std::string                 jobImg;
    std::string                 jobDescript;
    int                         price;
}L2E_SHOW_JOB;

typedef struct __L2E_CHANGE_JOB
{
    L2EProtocol                 eProtocol;
    std::string                 jobIcon;
}L2E_CHANGE_JOB;

typedef struct __L2E_SHOW_EVERYDAY
{
    L2EProtocol                 eProtocol;
    int                         everydayState;
    int                         everydayId;
    std::string                 icon[7];
    std::string                 name[7];
    int                         count[7];
    
    bool                        activeTip;
    bool                        boundTip;
    bool                        monthTip;
}L2E_SHOW_EVERYDAY;

typedef struct __L2E_SHOW_MONTH_ASSIGN
{
    L2EProtocol                 eProtocol;
    int                         monthAssignState;
    int                         monthAssignId;
    std::string                 icon[40];
    std::string                 name[40];
    int                         count[40];
    int                         vipLimit[40];
    int                         configCount;
}L2E_SHOW_MONTH_ASSIGN;

typedef struct __L2E_SHOW_LUCKY
{
    L2EProtocol                 eProtocol;
    int                         normalTicket;
    int                         specialTicket;
//    int                         normalCD;
//    int                         specialCD;
    int                         normalState;
    int                         specialState;
}L2E_SHOW_LUCKY;

typedef struct __L2E_DRAW_ONE
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    std::string                 icon;
    std::string                 name;
    bool                        frag;
    int                         count;
    int                         normalTicket;
    int                         specialTicket;
    int                         normalCD;
    int                         specialCD;
    int                         normalState;
    int                         specialState;
}L2E_DRAW_ONE;

typedef struct __L2E_DRAW_TEN
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    std::string                 icon[10];
    std::string                 name[10];
    bool                        frag[10];
    int                         count[10];
    int                         normalTicket;
    int                         specialTicket;
//    int                         normalCD;
//    int                         specialCD;
    int                         normalState;
    int                         specialState;
}L2E_DRAW_TEN;

typedef struct __L2E_UPDATE_DRAW
{
    L2EProtocol                 eProtocol;
    int                         normalCD;
    int                         specialCD;
    int                         normalState;
    int                         specialState;
}L2E_UPDATE_DRAW;

typedef struct __L2E_UPDATE_SPECIAL_TICKET
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    int                         count;
}L2E_UPDATE_SPECIAL_TICKET;

typedef struct __L2E_SHOW_HALL_SHOP
{
    L2EProtocol                 eProtocol;
    int                         type;
    int                         count;
    int                         shopId[10];
    std::string                 name[10];
    std::string                 icon[10];
    float                       price[10];
    int                         boundCount[10];
}L2E_SHOW_HALL_SHOP;

typedef struct __L2E_SHOW_PACKAGE_DETAIL
{
    L2EProtocol                 eProtocol;
    int                         packageId;
    std::string                 packageImg;
    std::string                 packageTitle;
    int                         noviceState;
    float                       price;
    int                         itemCount;
    std::string                 icon[20];
    int                         count[20];
}L2E_SHOW_PACKAGE_DETAIL;

typedef struct __L2E_SHOW_CUSTOM_PACKAGE
{
    L2EProtocol                 eProtocol;
    int                         shopId;
    std::string                 showRes;
    int                         missionId;
}L2E_SHOW_CUSTOM_PACKAGE;

typedef struct __L2E_SHOW_PACKAGE_RESULT
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    int                         noviceState;
}L2E_SHOW_PACKAGE_RESULT;

typedef struct __L2E_SHOW_FIRST_RECHARGE
{
    L2EProtocol                 eProtocol;
    int                         firstRechargeState;
}L2E_SHOW_FIRST_RECHARGE;

typedef struct __L2E_TAKE_VIP_BOUND
{
    L2EProtocol                 eProtocol;
    int                         errNo;
}L2E_TAKE_VIP_BOUND;

typedef struct __L2E_SHOW_VIP
{
    L2EProtocol                 eProtocol;
    int                         currVipLevel;
    float                       nextRechargeCount;
    int                         totalLoginDay;
    int                         currLoginDay;
    
    std::string                 detailDescript[10];
    std::string                 boundIcon[6];
    int                         boundCount[6];
    int                         boundState;
}L2E_SHOW_VIP;

typedef struct __L2E_SHOW_VIP_DETAIL
{
    L2EProtocol                 eProtocol;
    int                         detailVipLevel;
    std::string                 detailDescript[10];
}L2E_SHOW_VIP_DETAIL;

typedef struct __L2E_SHOW_CAMPAIGN_ACTIVE
{
    L2EProtocol                 eProtocol;
    int                         totalActiveVal;
    int                         boxState[4];
    int                         missionCount;
    std::vector<int>            missionId;
    std::vector<std::string>    descript;
    std::vector<int>            count;
    std::vector<int>            totalCount;
    std::vector<int>            activeVal;
    std::vector<int>            boundCount;
    std::vector<int>            boundState;
}L2E_SHOW_CAMPAIGN_ACTIVE;

typedef struct __L2E_UPDATE_CAMPAIGN_ACTIVE_MISSION
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    int                         missionId;
    int                         totalActiveVal;
    int                         boxState[4];
    int                         boundState;
}L2E_UPDATE_CAMPAIGN_ACTIVE_MISSION;

typedef struct __L2E_UPDATE_CAMPAIGN_ACTIVE_BOX
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    int                         boxState[4];
    int                         boundCount;
    std::vector<std::string>    icon;
    std::vector<int>            count;
}L2E_UPDATE_CAMPAIGN_ACTIVE_BOX;

typedef struct __L2E_UPDATE_FREE_DIAMOND
{
    L2EProtocol                 eProtocol;
    int                         errNo;
    int                         freeDiamondState;
    float                       onlineDuration;
}L2E_UPDATE_FREE_DIAMOND;

typedef struct __L2E_UPDATE_CAMPAIGN_BOUND_MISSION
{
    L2EProtocol                 eProtocol;
    int                         missionCount;
    std::vector<int>            missionState;
    std::vector<int>            missionId;
    std::vector<std::string>    descript;
    std::vector<int>            count;
    std::vector<int>            totalCount;
    std::vector<int>            totalBound;
    std::vector<std::string>            missionIcon;
    int                         boundCount[50][4];
    std::string                 icon[50][4];
}L2E_UPDATE_CAMPAIGN_BOUND_MISSION;

typedef struct __E2L_CHANGE_NAME
{
    E2LProtocol                 eProtocol;
    std::string                 name;
}E2L_CHANGE_NAME;

typedef struct __L2E_UPDATE_NAME
{
    L2EProtocol                 eProtocol;
    std::string                 name;
}L2E_UPDATE_NAME;

typedef struct __L2E_UPDATE_HALL_TIP
{
    L2EProtocol                 eProtocol;
    int                         tipIndex;
    bool                        tipValue;
}L2E_UPDATE_HALL_TIP;

typedef struct __L2E_UPDATE_ARMS_TIP
{
    L2EProtocol                 eProtocol;
    std::vector<bool>           armsTip;
    std::vector<bool>           armsFameTip;
    std::vector<int>            gemTip;
}L2E_UPDATE_ARMS_TIP;

typedef struct __L2E_UPDATE_EQUIP_TIP
{
    L2EProtocol                 eProtocol;
    std::vector<bool>           posTip;
    std::vector<bool>           posUplevelTip;
    std::vector<bool>           posUpgradeTip;
}L2E_UPDATE_EQUIP_TIP;

typedef struct __L2E_UPDATE_PET_TIP
{
    L2EProtocol                 eProtocol;
    std::vector<bool>           uplevelTip;
}L2E_UPDATE_PET_TIP;

typedef struct __L2E_UPDATE_CAMPAIGN_TIP
{
    L2EProtocol                 eProtocol;
    bool                        activeTip;
    bool                        boundTip;
    bool                        monthTip;
}L2E_UPDATE_CAMPAIGN_TIP;

typedef struct __L2E_UPDATE_DISCOUNT_STATE
{
    L2EProtocol                 eProtocol;
    bool                        state;
    long                        leftSec;
    
    int                         packageId;
    std::string                 packageImg;
    std::string                 packageTitle;
    
    float                       price;
    int                         itemCount;
    std::string                 icon[100];
    int                         count[100];

}L2E_UPDATE_DISCOUNT_STATE;

typedef struct __L2E_SHOW_DISCOUNT
{
    L2EProtocol                 eProtocol;
    bool                        state;
    long                        leftSec;
    
    int                         packageId;
    std::string                 packageImg;
    std::string                 packageTitle;
    
    float                       price;
    int                         itemCount;
    std::string                 icon[100];
    int                         count[100];

}L2E_SHOW_DISCOUNT;

typedef struct __L2E_UPDATE_VIT_CD
{
    L2EProtocol                 eProtocol;
    bool                        state;
    long                        leftSec;
}L2E_UPDATE_VIT_CD;
#endif /* ELProtocol_h */
