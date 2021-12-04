/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-02     jing       the first version
 */
#include "gps.h"
#include "manager.h"

#include<string.h>

static rt_thread_t t_gps = RT_NULL;
rt_device_t gps_dev;
struct rt_semaphore gps_sem;
rt_size_t rx_len = 0;  // 本次接收数据长度
rt_int32_t last_rx_len = 0;   // 上次接收数据长度

struct serial_configure u3_configs = RT_SERIAL_CONFIG_DEFAULT;

rt_err_t rx_gps_callback(rt_device_t dev_, rt_size_t size_)
{
    rx_len = size_;
    rt_sem_release(&gps_sem);
    return RT_EOK;
}
//字符串查找
int find_str(char *buff,char *c)
{
    int i =0,j=0,num=0;
    int flag=0,buff_len;
    buff_len=(int)rt_strlen(buff);
    for(i=0;i<buff_len;i++)
    {
        if(c[0] == buff[i])
        {
            //printf("找到一个%d\n",i);
            for(j=0;c[j]!='\0';j++)
            {

                if(c[j] == buff[i+j])
                {
                    flag=i;
                }
                else
                    flag =-1;
            }
        }
        if(flag >0)
            return flag;
    }

}
//在字符串中查找字符并返回位置
char *find(char *buff,char *c,int ret)
{
    int i =0;

    for(i=ret;i<ret+50;i++)
    {
        if(buff[i]==c)
            return i;//&buff[i];
    }
    return 0;
}


//void check_gps(void *parameter)
//{
//    rt_size_t current_rx_len = 0;
//    char buffer[128] = {0};
//    char last_buffer[128] = {0};
//    int lng;
//    int lat;
//    while(1){
//        rt_sem_take(&gps_sem, RT_WAITING_FOREVER);
//        current_rx_len = rt_device_read(gps_dev, 0, buffer, rx_len);
//        // 判断是否有遗留数据需要拼接字符串
////        if (last_rx_len==0)
////        {
////            if (current_rx_len==64)
////            {
////                buffer[current_rx_len] = '\0';
////                rt_kprintf("gps rec len:%d buf %s\n",current_rx_len,buffer);
////                last_rx_len = 0;
////            }
////            else
////            {
////                last_rx_len = current_rx_len;
////                rt_strncpy(last_buffer, buffer, current_rx_len);
////                continue;
////            }
////        }
////        else {
////            rt_kprintf("gps current_rx_len:%d last_rx_len %d\n",current_rx_len,last_rx_len);
////            if (current_rx_len==64)
////                {
////                    buffer[current_rx_len] = '\0';
////                    rt_kprintf("gps rec current_rx_len:%d buf %s\n",current_rx_len,buffer);
////                    last_rx_len = 0;
////                }
////            else if (last_rx_len+current_rx_len==64)
////            {
////                rt_kprintf("last_buffer %s buffer %s\n",last_buffer,buffer);
////                  for(int i=0;i<current_rx_len;i++)
////                  {
////                      last_buffer[last_rx_len+i] =buffer[i];
////                  }
////
////                  last_rx_len = 0;
////            }
////            else {
////                last_rx_len = current_rx_len;
////                                rt_strncpy(last_buffer, buffer, current_rx_len);
////                                continue;
////            }
////        }
//
//        rt_kprintf("gps rec current_rx_len:%d\n",current_rx_len);
//        if (current_rx_len<64)
//            continue;
//
//        char* index1 = rt_strstr(buffer,"$GPRMC");
//        char* index2 = rt_strstr(buffer,"A");
//        char* index3 = rt_strstr(buffer,"N");
//        char* index4 = rt_strstr(buffer,"E");
//        if (index1!=RT_NULL && index2!=RT_NULL && index3!=RT_NULL && index4!=RT_NULL)
//        {
//            int buff_len;
//            buff_len=(int)rt_strlen(buffer);
//            int place,i,ret;
//            char b[100] = "$GPRMC";
//            ret=find_str(buffer,b);//返回字符串位置
//            int p1,p2,p3,p4;
//            p1=find(buffer,'A',6);//+6是跳过"$GPRMC"
//            p2=find(buffer,'N',6);
//            p3=find(buffer,'E',6);
//            rt_kprintf("buffer:%s\n",buffer);
//            rt_kprintf("len: %d, start: %d, p1: %d p2: %d p3: %d\n",buff_len, index1,p1,p2,p3);
//            char lng_buf[15]={0};
//            char lat_buf[15]={0};
//            int index_lng=0;
//            int index_lat=0;
//            for(i=p1+2;i<p2-1;i++)
//            {
//                lng_buf[index_lng] = buffer[i];
//                index_lng ++;
//            }
//            index_lng +=1;
//            lng_buf[index_lng] = '\0';
//            lng = (int)atoi(lng_buf);
//            rt_kprintf("\n lng_buf :%s",lng_buf);
//            rt_kprintf("\n lng: %d ",lng);
//            for(i=p2+2;i<p3-1;i++)
//            {
//                lat_buf[index_lat] = buffer[i];
//                index_lat ++;
//            }
//            index_lat +=1;
//            lat_buf[index_lat] = '\0';
//            lat = (int)atoi(lat_buf);
//            rt_kprintf("\n lat_buf :%s",lat_buf);
//            rt_kprintf("\n lat: %d ",lat);
//
//        }
//        memset(buffer, 0, sizeof buffer);          //清空数组
//    }
//}
void check_gps(void *parameter)
    {
        char g_char;
        int gps_index = 0;  // GPS数据
        char gps_buffer[128] = {0};  // GPS数据
        char lng_s[15]={0};
        char lat_s[15]={0};
        char temp_s[15]={0};
        while (1)
        {
            while(rt_device_read(gps_dev, 0, &g_char, 1) != 1)
            {
               rt_sem_take(&gps_sem, RT_WAITING_FOREVER);
            }
            if (g_char=='$')
            {
                gps_buffer[gps_index]=g_char;
                gps_index+=1;
            }
            else if (gps_index!=0)
            {
                gps_buffer[gps_index]=g_char;
                gps_index+=1;
            }
            if (g_char == 'E' && gps_index!=0)
            {
                gps_buffer[gps_index] = '\0';
//                rt_kprintf("gps_buffer rec %s\n",gps_buffer);
                sscanf(gps_buffer,"$GPRMC,%*[^,],A,%[^,],N,%[^,],E",lat_s,lng_s);
//                rt_kprintf("lng_s:%s,lat_s:%s\n",lng_s,lat_s);
//               lng = atoff(__nptr)(row_s);
//               lat = atoff(col_s);
//                rt_kprintf("lng %d lat %d\n",atoi(lng_s),atoi(lat_s));
                gps_index =0;
             }
//            rt_kprintf("gps rec %c\n",buffer); //调试输出到串口
        }
    }

int init_gps()
{
    rt_err_t ret = 0;
    gps_dev = rt_device_find("uart3");
    if(gps_dev == RT_NULL){
        LOG_E("rt_device_find[uart3] failed...\n");
        return -EINVAL;
    }

//    ret = rt_device_open(gps_dev, RT_DEVICE_FLAG_DMA_RX); //DMA
    ret = rt_device_open(gps_dev, RT_DEVICE_OFLAG_RDWR|RT_DEVICE_FLAG_INT_RX);  //中断
    if(ret < 0){
        LOG_E("rt_device_open[uart3] failed...\n");
        rt_kprintf("ret : %d\n",ret);
        return ret;
    }
    /* step2：修改串口配置参数 */
    u3_configs.baud_rate = BAUD_RATE_115200;        //修改波特率为 115200
    u3_configs.data_bits = DATA_BITS_8;           //数据位 8
    u3_configs.stop_bits = STOP_BITS_1;           //停止位 1
    u3_configs.bufsz     = 64;                   //修改缓冲区 buff size 为512
    u3_configs.parity    = PARITY_NONE;           //无奇偶校验位
    rt_device_control(gps_dev, RT_DEVICE_CTRL_CONFIG, (void *)&u3_configs);

    rt_device_set_rx_indicate(gps_dev, rx_gps_callback);

    ret = rt_sem_init(&gps_sem,"rx_sem", 0, RT_IPC_FLAG_FIFO);
    if(ret < 0){
        LOG_E("rt_sem_init failed[%d]...\n",ret);
        return ret;
    }
}

void thread_gps()
{
    init_gps();
    /* 创建线程 */
        t_gps = rt_thread_create("thread_gps",
                                check_gps,
                                RT_NULL,
                                2048,
                                10,
                                5);

        /* 如果获得线程控制块，启动这个线程 */
        if (t_gps != RT_NULL)
        {
            rt_thread_startup(t_gps);
        }
}

