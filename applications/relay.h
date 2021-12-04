/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 * 控制继电器  单次触发和延时翻转（水泵）
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-02     jing       the first version
 */
#ifndef APPLICATIONS_RELAY_H_
#define APPLICATIONS_RELAY_H_
#include <rtthread.h>
#include <rtdevice.h>
#include  <stdbool.h>
#include "drv_common.h"
#define DRAW_PIN        GET_PIN(E, 5)  //红灯管脚


void set_relay(bool a);
void set_relay_timing(bool a);

#endif /* APPLICATIONS_RELAY_H_ */
