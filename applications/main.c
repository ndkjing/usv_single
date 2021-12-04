/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-02     RT-Thread    first version
 */

#include <rtthread.h>
#include <stdio.h>
#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>


#include "led.h"
#include "lora_24.h"
//#include "manager.h"
//#include "gps.h"
//#include "motor_pwm.h"

int main(void)
{
    thread_led();
    thread_lora_24();
    thread_pwm();
//    thread_gps();
    thread_manager();
//    char lng_s[15]={0};
//    char lat_s[15]={0};
//    char temp_s[15]={0};
//    char gps_buffer[]="$GPRMC,045347.00,A,3030.31038,N,11425.44782,E";
//    sscanf(gps_buffer,"$GPRMC,%*[^,],A,%[^,],N,%[^,],E",lat_s,lng_s);
//    rt_kprintf("temp_s:%s, lng_s:%s,lat_s:%s\n",temp_s,lng_s,lat_s);
//    char gps_buffer[]=",A,3030.31038,N,11425.44782,E";
//    sscanf(gps_buffer,",A,%[^,],N,%[^,],E",lat_s,lng_s);
//    rt_kprintf("lng_s:%s,lat_s:%s\n",lng_s,lat_s);
//    rt_kprintf("lng %d lat %d\n",atoi(lng_s),atoi(lat_s));
//    thread_manager();
    return RT_EOK;
}
