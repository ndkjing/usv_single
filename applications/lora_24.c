/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-02     jing       the first version
 */
#include <lora_24.h>
#include <stdio.h>
#define THREAD_PRIORITY         10
#define THREAD_STACK_SIZE       512
#define THREAD_TIMESLICE        5

rt_thread_t thread_lora_rec = RT_NULL;
rt_thread_t thread_lora_send = RT_NULL;

#define LORA_UART_NAME       "uart2"    /* 串口设备名称 */
rt_device_t lora_serial;                /* 串口设备句柄 */
struct serial_configure lora_config = RT_SERIAL_CONFIG_DEFAULT;  /* 初始化配置参数 */
struct rt_semaphore lora_sem;  // 信号量
rt_size_t lora_rx_len = 0;   //数据接收长度
#include "manager.h"

rt_err_t lora_rx_callback(rt_device_t dev, rt_size_t size)
{
//    lora_rx_len = size;
    rt_sem_release(&lora_sem);
    return RT_EOK;
}

void init_lora()
{
    /* step1：查找串口设备 */
    lora_serial = rt_device_find(LORA_UART_NAME);
    /* step2：修改串口配置参数 */
    lora_config.baud_rate = BAUD_RATE_115200;        //修改波特率为 9600
    lora_config.data_bits = DATA_BITS_8;           //数据位 8
    lora_config.stop_bits = STOP_BITS_1;           //停止位 1
    lora_config.bufsz     = 128;                   //修改缓冲区 buff size 为 128
    lora_config.parity    = PARITY_NONE;           //无奇偶校验位

    /* step3：控制串口设备。通过控制接口传入命令控制字，与控制参数 */
    rt_device_control(lora_serial, RT_DEVICE_CTRL_CONFIG, &lora_config);

    /* step4：打开串口设备。以中断接收及轮询发送模式打开串口设备 */
    rt_device_open(lora_serial, RT_DEVICE_OFLAG_RDWR|RT_DEVICE_FLAG_INT_RX);
//    rt_device_open(lora_serial, RT_DEVICE_OFLAG_RDWR|RT_DEVICE_FLAG_DMA_RX);
    rt_device_set_rx_indicate(lora_serial, lora_rx_callback);
}



/* 线程 的入口函数 */
static void lora_send(void *parameter)
{
    char str[] = "RT!\r\n";
    while (1)
    {
        rt_device_write(lora_serial, 0, str, (sizeof(str) - 1));  //向指定串口发送数据
        rt_thread_mdelay(100);
    }
}

static void lora_rec(void *parameter)
{
    int lora_buffer_index = 0;  // 摇杆数据
    char lora_buffer[128] = {0};  // 摇杆数据
    int lora_draw_buffer_index = 0; // 水泵数据
    char lora_draw_buffer[32] = {0};// 水泵数据
    char lora_char;
    char row_s[5]={0};
    char col_s[5]={0};
    char draw_s[2]={0};
    while(1)
    {
//        rt_sem_take(&lora_sem, RT_WAITING_FOREVER);
//        lora_buffer_len = rt_device_read(lora_serial, 0, lora_buffer, lora_rx_len);
//        lora_buffer[lora_buffer_len] = '\0';
//        rt_kprintf("lora buffer:%s lora_rx_len %d\n ",lora_buffer,lora_buffer_len);
//        memset(lora_buffer,0,sizeof lora_buffer);

        while(rt_device_read(lora_serial, 0, &lora_char, 1) != 1){
                    rt_sem_take(&lora_sem, RT_WAITING_FOREVER);
                }
        if (lora_char == 'A')
        {
            lora_buffer[lora_buffer_index] = lora_char;
            lora_buffer_index+=1;
        }
        else if (lora_buffer_index != 0) {
            lora_buffer[lora_buffer_index] = lora_char;
            lora_buffer_index+=1;
        }
        // 接收到完整数据 后分隔字符串处理
        if (lora_char == 'Z' && lora_buffer_index!=0) {
            lora_buffer[lora_buffer_index] = '\0';
//            rt_kprintf("lora rec %s\n",lora_buffer);
            sscanf(lora_buffer,"A%[0-9],%[0-9]Z,",row_s,col_s);
//            rt_kprintf("row_s:%s,col_s:%d\n",row,col);
           row = atoi(row_s);
           col = atoi(col_s);

            rt_kprintf("row %d col %d\n",row,col);
            lora_buffer_index =0;
        }
        if (lora_char == 'B')
        {
            lora_draw_buffer[lora_draw_buffer_index] = lora_char;
            lora_draw_buffer_index+=1;
        }
        else if (lora_draw_buffer_index != 0) {
            lora_draw_buffer[lora_draw_buffer_index] = lora_char;
            lora_draw_buffer_index+=1;
        }
        // 接收到完整数据 后分隔字符串处理
        if (lora_char == 'Z' && lora_draw_buffer_index!=0) {
            lora_draw_buffer[lora_draw_buffer_index] = '\0';
//            rt_kprintf("lora rec %s\n",lora_buffer);
            sscanf(lora_draw_buffer,"B%[0-9]Z,",draw_s);
//            rt_kprintf("row_s:%s,col_s:%d\n",row,col);
            b_draw=atoi(draw_s);
            rt_kprintf("b_draw %d \n",b_draw);
            lora_draw_buffer_index =0;
        }
    }
}

/* 线程 */
int thread_lora_24(void)
{
    init_lora();
    rt_sem_init(&lora_sem,"lora_rx_sem", 0, RT_IPC_FLAG_FIFO);
    /* 创建线程 */
    thread_lora_rec = rt_thread_create("lora_rec",
            lora_rec, RT_NULL,
                            2048,
                            THREAD_PRIORITY, THREAD_TIMESLICE);

    /* 如果获得线程控制块，启动这个线程 */
    if (thread_lora_rec != RT_NULL)
        rt_thread_startup(thread_lora_rec);
    thread_lora_send = rt_thread_create("lora_send",
            lora_send, RT_NULL,
                                THREAD_STACK_SIZE,
                                5, THREAD_TIMESLICE);

        /* 如果获得线程控制块，启动这个线程 */
        if (thread_lora_send != RT_NULL)
            rt_thread_startup(thread_lora_send);

    return 0;
}

/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(thread_lora_24, thread_lora_24);

