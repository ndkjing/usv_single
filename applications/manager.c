/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-02     jing       the first version
 */
#include "manager.h"

static const int  draw_time = 10 ; //抽水时间
u_int row = 49;// 横向摇杆接受值0-99
u_int col = 49;// 纵向摇杆接受值0-99
rt_uint32_t left_pwm = 1500;
rt_uint32_t right_pwm = 1500;
u_int remote_forward_pwm = 1500;
u_int remote_steer_pwm = 1500;
u_int b_draw = 0;// 水泵控制接受值0初始值 1  开启 2关闭
float_t lng=0.0;  // 经度
float_t lat=0.0;   // 纬度
float_t dump_energy = 0;  //  当前剩余电量
float_t back_home_erergy = 20;  //  返航电量阈值
u_int led_status = 0;    //   当前状态灯编码  0 绿色  1 黄色  2 红色  3 蜂鸣
u_int side_light=1; //  舷灯状态  0  关  1  开
static rt_thread_t t_manager = RT_NULL;
void manager(void)
{
    while(1)
    {
        // 控制抽水泵
        if (b_draw==1)
        {
            set_relay(true);
        }
        else if (b_draw==2)
        {
            set_relay(false);
        }
       // 控制电机输出 将摇杆值转换到1000-2000
        if ((row>45) && (row<55))
            row=49;
        if ((col>45) && (col<55))
            col=49;
        remote_forward_pwm = (99 - row) * 10 + 1000;
        remote_steer_pwm = col * 10 + 1000;
        left_pwm = 1500 + (remote_forward_pwm - 1500) + (remote_steer_pwm - 1500);
        right_pwm = 1500 + (remote_forward_pwm - 1500) - (remote_steer_pwm - 1500);
        set_pwm(left_pwm,right_pwm);
//        rt_kprintf("left_pwm:%d ,right_pwm:%d",left_pwm,right_pwm);
        rt_thread_mdelay(100);
    }
}

/* 线程 */
int thread_manager(void)
{
    /* 创建线程 */
    t_manager = rt_thread_create("thread_manager",
                                  manager,
                                  RT_NULL,
                                  1024,
                                  20,
                                  5);

    /* 如果获得线程控制块，启动这个线程 */
    if (t_manager != RT_NULL)
    {
        rt_kprintf("t_manager start");
        rt_thread_startup(t_manager);
    }
    else
    {
        rt_kprintf("t_manager start error");
    }

    return 0;
}
