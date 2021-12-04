/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-02     jing       the first version
 */
#include "relay.h"



#define THREAD_PRIORITY         20
#define THREAD_STACK_SIZE       512
#define THREAD_TIMESLICE        5
bool a = true;
/* 定时器的控制块 */
static rt_timer_t relay_timer;
void set_relay(bool a)
{
    if (a)
    {
        rt_pin_write(DRAW_PIN, PIN_LOW);
    }
    else {
        rt_pin_write(DRAW_PIN, PIN_HIGH);
    }
}

void start_draw()
{
    rt_pin_write(DRAW_PIN, PIN_LOW);
}

void set_relay_timing(bool a)
{
    if (a)
    {
        rt_pin_write(DRAW_PIN, PIN_LOW);
        /* 创建定时器 单次定时器 */
        relay_timer = rt_timer_create("relay_timer", start_draw,
                                     RT_NULL,  5,
                                     RT_TIMER_FLAG_ONE_SHOT);
            /* 启动定时器 */
            if (relay_timer != RT_NULL)
                rt_timer_start(relay_timer);
    }
    else {
        rt_pin_write(DRAW_PIN, PIN_HIGH);
    }
}

//初始化继电器
void init_relay()
{
    rt_pin_mode(DRAW_PIN, PIN_MODE_OUTPUT);

}



/* 线程 */
//int thread_led(void)
//{
//    //设置管脚的模式
//    rt_pin_mode(LED_PIN_RED, PIN_MODE_OUTPUT);
//
//    /* 创建线程 */
//    t_led = rt_thread_create("blink_led",
//                            blink_led, RT_NULL,
//                            THREAD_STACK_SIZE,
//                            THREAD_PRIORITY, THREAD_TIMESLICE);
//
//    /* 如果获得线程控制块，启动这个线程 */
//    if (t_led != RT_NULL)
//        rt_thread_startup(t_led);
//    return 0;
//}




