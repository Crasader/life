//
//  TLProtocol.h
//  life
//
//  Created by ff on 17/7/27.
//
//

#ifndef TLProtocol_h
#define TLProtocol_h

#include "cocos2d.h"

enum T2LProtocol
{
    t2l_restore_vit,                                    //  回复体力        0
    t2l_refresh_offer_board,                            //  刷新悬赏任务面板
    t2l_work_offer1_over,                               //  完成执行的悬赏1
    t2l_work_offer2_over,                               //  完成执行的悬赏2
    t2l_work_offer3_over,                               //  完成执行的悬赏3
    t2l_offer_board_cd,                                 //  悬赏任务面板刷新剩余时间    5
    t2l_update_offer_all_cd,                            //  更新悬赏任务面板所有剩余时间
    t2l_refresh_prov_reset,                             //  刷新试炼重置次数
    t2l_reset_vit_buy_count,                            //  刷新体力购买次数
    t2l_complete_tower_sweep,                           //  完成爬塔扫荡
    t2l_refresh_tower_reset,                            //  刷新爬塔重置次数    10
    t2l_update_tower_sweep_cd,                          //  刷新爬塔扫荡计时
    t2l_refresh_abyss,                                  //  刷新深渊的挑战次数
    t2l_restore_arena_ticket,                           //  回复竞技场挑战次数
    t2l_update_arena_ticket_cd,                         //  更新竞技场挑战次数回复剩余时间
    t2l_refresh_arena_reward,                           //  更新竞技场的奖励    15
    t2l_refresh_everyday,                               //  更新7天领取
    t2l_restore_draw_normal,                            //  免费普通抽卡
    t2l_restore_draw_special,                           //  免费特殊抽卡
    t2l_update_lucky_cd,                                //  更新免费抽卡计时
    t2l_add_vip_login_day,                              //  增加VIP连续登陆天数  20
    t2l_refresh_campaign_active,                        //  刷新每日活跃度
    t2l_reset_free_diamond,                             //  重置免费钻石
    t2l_open_discount,                                  //  预定折扣店开张
    t2L_close_discount,                                 //  预定折扣店打烊
    t2l_update_discount_close_sec,                      //  获得折扣店打烊倒计时  25
    t2l_update_vit_restore_cd,                          //  更新体力恢复时间
    t2l_reset_petoffer_count,                           //  重置悬赏钻石更新次数
    t2l_max
};

enum L2TProtocol
{
    l2t_start_app,                                      //  开始应用通知
    l2t_start_restore_vit,                              //  开始回复体力
    l2t_reset_offer_board,                              //  重置悬赏任务面板刷新时间
    l2t_get_offer_board_cd,                             //  获得悬赏任务面板刷新剩余时间
    l2t_get_offer_all_cd,                               //  获得悬赏任务面板所有剩余时间
    l2t_start_work_offer,                               //  开始悬赏计时
    l2t_stop_work_offer,                                //  强制停止悬赏计时
    l2t_refresh_prov_reset,                             //  刷新试炼重置次数后，预订明天的刷新
    l2t_reset_vit_buy_count,                            //  刷新体力购买次数后，预订明天的刷新
    l2t_start_tower_sweep,                              //  开始爬塔扫荡计时
    l2t_stop_tower_sweep,                               //  强制停止爬塔扫荡计时
    l2t_refresh_tower_reset,                            //  刷新爬塔重置次数后，预定明天的刷新
    l2t_get_tower_sweep_cd,                             //  获得爬塔扫荡倒计时
    l2t_refresh_abyss,                                  //  刷新深渊的挑战次数后，预定明天的刷新
    l2t_restore_arena_ticket,                           //  竞技场挑战次数不满，开始恢复计时
    l2t_stop_arena_ticket_restore,                      //  竞技场挑战次数已满，停止恢复计时
    l2t_get_arena_ticket_cd,                            //  获得竞技场挑战次数回复剩余时间
    l2t_refresh_arena_reward,                           //  刷新竞技场奖励，预定明天的刷新
    l2t_refresh_everyday,                               //  刷新7天领取
    l2t_restore_draw_normal,                            //  开始免费普通抽卡计时
    l2t_restore_draw_special,                           //  开始免费特殊抽卡计时
    l2t_get_lucky_cd,                                   //  获得免费抽卡计时
    l2t_refresh_campaign_active,                        //  刷新每日活跃度
    l2t_reset_free_diamond,                             //  重置免费钻石
    l2t_wait_discount_open,                             //  等下一次折扣开始
    l2t_wait_discount_close,                            //  等折扣店打烊
    l2t_get_discount_close_cd,                          //  获得折扣店打烊倒计时
    l2t_get_vit_restore_sec,                            //  获得体力恢复时间
    l2t_reset_petoffer_count,                           //  定明天的悬赏刷新次数重置
    l2t_max
};

typedef struct __L2T_COMMON
{
    L2TProtocol                         eProtocol;
}L2T_COMMON;

typedef struct __T2L_COMMON
{
    T2LProtocol                         eProtocol;
    long                                passSec;
}T2L_COMMON;

typedef struct __L2T_RESTORE_VIT
{
    L2TProtocol                         eProtocol;
    int                                 restoreSec;
}L2T_RESTORE_VIT;

typedef struct __L2T_RESET_OFFER_BOARD
{
    L2TProtocol                         eProtocol;
    int                                 refreshSec;
}L2T_RESET_OFFER_BOARD;

typedef struct __T2L_UPDATE_OFFER_ALL_CD
{
    T2LProtocol                         eProtocol;
    long                                refreshLeftSec;
    long                                work1LeftSec;
    long                                work2LeftSec;
    long                                work3LeftSec;
}T2L_UPDATE_OFFER_ALL_CD;
typedef struct __T2L_UPDATE_LUCKY_CD
{
    T2LProtocol                         eProtocol;
    long                                normalLeftCD;
    long                                specialLeftCD;
}T2L_UPDATE_LUCKY_CD;

typedef struct __L2T_START_WORK_OFFER
{
    L2TProtocol                         eProtocol;
    long                                cd;
    int                                 pos;
}L2T_START_WORK_OFFER;
typedef struct __L2T_STOP_WORK_OFFER
{
    L2TProtocol                         eProtocol;
    int                                 pos;
}L2T_STOP_WORK_OFFER;
typedef struct __L2T_START_TOWER_SWEEP
{
    L2TProtocol                         eProtocol;
    int                                 sweepSec;
}L2T_START_TOWER_SWEEP;

typedef struct __L2T_RESTORE_ARENA_TICKET
{
    L2TProtocol                         eProtocol;
    int                                 restoreSec;
}L2T_RESTORE_ARENA_TICKET;

typedef struct __L2T_RESTORE_DRAW_TICKET
{
    L2TProtocol                         eProtocol;
    int                                 restoreSec;
}L2T_RESTORE_DRAW_TICKET;

typedef struct __L2T_WAIT_DISCOUNT_OPEN
{
    L2TProtocol                         eProtocol;
    long                                openSec;
}L2T_WAIT_DISCOUNT_OPEN;

typedef struct __L2T_WAIT_DISCOUNT_CLOSE
{
    L2TProtocol                         eProtocol;
    long                                closeSec;
}L2T_WAIT_DISCOUNT_CLOSE;

#endif /* TLProtocol_h */
