/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-02     jing       the first version
 */

#include "led.h"

#define THREAD_PRIORITY         25
#define THREAD_STACK_SIZE       512
#define THREAD_TIMESLICE        5

static rt_thread_t t_led = RT_NULL;
// 控制板子上led闪烁表示程序正在运行
void blink_led()
{
    while (1)
    {
        rt_pin_write(LED_PIN_RED, PIN_LOW);
//        rt_pin_write(LED_PIN_BLUE, PIN_LOW);
        rt_thread_mdelay(100);
        rt_pin_write(LED_PIN_RED, PIN_HIGH);
//        rt_pin_write(LED_PIN_BLUE, PIN_HIGH);
        rt_thread_mdelay(100);
    }
}

// 控制状态灯
void control_status_led(u_int status_code)
{

}


/* 线程 */
int thread_led(void)
{
    //设置管脚的模式
    rt_pin_mode(LED_PIN_RED, PIN_MODE_OUTPUT);
//    rt_pin_mode(LED_PIN_BLUE, PIN_MODE_OUTPUT);

    /* 创建线程 */
    t_led = rt_thread_create("blink_led",
                            blink_led, RT_NULL,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY, THREAD_TIMESLICE);

    /* 如果获得线程控制块，启动这个线程 */
    if (t_led != RT_NULL)
        rt_thread_startup(t_led);
    return RT_EOK;
}
