/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-02     jing       the first version
 */
#ifndef APPLICATIONS_MOTOR_PWM_H_
#define APPLICATIONS_MOTOR_PWM_H_
#include <rtthread.h>
void set_pwm(rt_uint32_t left_pwm,rt_uint32_t right_pwm);
int thread_pwm(void);
#endif /* APPLICATIONS_MOTOR_PWM_H_ */
