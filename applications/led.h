/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-02     jing       the first version
 */
#ifndef APPLICATIONS_LED_H_
#define APPLICATIONS_LED_H_
#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"
#define LED_PIN_RED         GET_PIN(E, 5)  //红灯管脚
//#define LED_PIN_BLUE        GET_PIN(E, 6)  //蓝灯管脚

void control_status_led(u_int status_code);
int thread_led(void);


#endif /* APPLICATIONS_LED_H_ */
