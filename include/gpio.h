#ifndef GPIO_H
#define GPIO_H

#include <cstdlib>  
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <fcntl.h> 

#define GPIO_ON "1"           //开启
#define GPIO_OFF "0"          //关闭
// GPIO port refer to https://wiki.amovlab.com/public/allspark-nx/开发帮助/接口功能测试/GPIO功能测试/GPIO.html
// This demo uses PIN 5
#define LASER_IO "419"     //控制引脚
#define DIRECTION "out"  //方向为输出

class gpio
{
private:
    /* data */
public:
    gpio(/* args */);
    ~gpio();
    int openGpio(const char *port);  //导入gpio
    int closeGpio(const char *port); //导出gpio
    int readGPIOValue(const char *port,int &level);  //读取gpio值
    int setGpioDirection(const char *port,const char *direction);  //设置gpio方向
    int setGpioValue(const char *port,const char *level);  //设置gpio值
    int setGpioEdge(const char *port,const char *edge);  //设置gpio输入模式
};

#endif
