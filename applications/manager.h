/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-02     jing       the first version
 */
#ifndef APPLICATIONS_MANAGER_H_
#define APPLICATIONS_MANAGER_H_
#include <rtthread.h>
#include "rtconfig.h"
#include <rtdevice.h>
#include "drv_common.h"
#include "math.h"

#include "relay.h"
#include "motor_pwm.h"

extern u_int row;// 横向摇杆接受值0-99
extern u_int col;// 纵向摇杆接受值0-99
extern u_int b_draw;// 水泵控制接受值0初始值 1  开启 2关闭
extern float_t lng;  // 经度
extern float_t lat;   // 纬度
extern float_t dump_energy;  //  当前剩余电量
extern float_t back_home_erergy;  //  返航电量阈值
extern u_int led_status;    //   当前状态灯编码  0 绿色  1 黄色  2 红色  3 蜂鸣
extern u_int side_light; //  舷灯状态  0  关  1  开

int thread_manager(void);
#endif /* APPLICATIONS_MANAGER_H_ */
