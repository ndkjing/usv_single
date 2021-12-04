/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-02     jing       the first version
 */
#include "energy_adc.h"
static rt_thread_t t_energy = RT_NULL;
void check_dump_energy()
{
 while(1)
 {
     rt_thread_mdelay(200);
 }
}


void thread_energy()
{
    /* 创建线程 */
        t_energy = rt_thread_create("thread_energy",
                check_dump_energy, RT_NULL,
                                512,
                                16, 5);

        /* 如果获得线程控制块，启动这个线程 */
        if (t_energy != RT_NULL)
            rt_thread_startup(t_energy);
}


