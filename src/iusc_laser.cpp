#include "ros/ros.h"
#include "std_msgs/Bool.h"
#include <iostream>
#include <fstream>
#include "gpio.h"
#include <time.h>

using namespace std;

#define ros_freq 100
typedef enum LASER_STATE{
  LASER_READY=0,
  LASER_ON,
  LASER_OVERHEAT,
} LASER_STATE;


bool laser_command = false;
LASER_STATE laser_state = LASER_READY;
struct timeval tvlaser_command;

void laser_command_callback(const std_msgs::Bool& message){
  laser_command = message.data;
  gettimeofday(&tvlaser_command, NULL);
}

int main(int argc, char **argv)
{
  float ros_dt = 1.0/ros_freq;

  ros::init(argc, argv, "iusc_laser");
  ros::NodeHandle n;
  ros::NodeHandle nh("~");	
  ros::Rate loop_rate(ros_freq);
  
  ros::Subscriber sub_joy       = n.subscribe("/laser_command", 1, laser_command_callback);

  struct timeval tvstart, tvend, laser_on_time, laser_off_time;
  gettimeofday(&tvstart,NULL);

  int count = 0;
  srand((int)time(0));

  gpio *gp = new gpio();
  if(gp->openGpio(LASER_IO) != 0)  //打开GPIO
    {
      exit(1);	
    }
  sleep(1);  //两次操作之间延时10ms
  if(gp->setGpioDirection(LASER_IO, DIRECTION) != 0)  //设置GPIO方向
    {
      gp->closeGpio(LASER_IO);
      exit(1);		
    }
  sleep(1);
  
  while (ros::ok())
  {
    gettimeofday(&tvend,NULL);
    double totaltime = tvend.tv_sec - tvstart.tv_sec + 1e-6 * (tvend.tv_usec - tvstart.tv_usec);
    switch(laser_state){
    case LASER_READY:{
      double laser_command_callback_timeout = tvend.tv_sec - tvlaser_command.tv_sec + 1e-6 * (tvend.tv_usec - tvlaser_command.tv_usec);
      if(laser_command_callback_timeout >= 5){
	ROS_INFO_THROTTLE(20, "Waiting for laser command...");
      }else{
	// turn on the laser when the command is triggered
	if(laser_command == true){
	  gp->setGpioValue(LASER_IO, GPIO_ON);
	  laser_on_time = tvend;
	  laser_state = LASER_ON;
	  ROS_INFO("IUSC LASER Triggered!");
	}
      }
      break;
    }
    case LASER_ON:{
      if(laser_command == false){
	gp->setGpioValue(LASER_IO, GPIO_OFF);
	laser_state = LASER_OVERHEAT;
	laser_off_time = tvend;
	ROS_INFO("IUSC LASER Turning Off");
      }else{
	double on_time = tvend.tv_sec - laser_on_time.tv_sec + 1e-6 * (tvend.tv_usec - laser_on_time.tv_usec);
	if(on_time >= 5){
	  gp->setGpioValue(LASER_IO, GPIO_OFF);
	  laser_state = LASER_OVERHEAT;
	  laser_off_time = tvend;
	  ROS_INFO("IUSC LASER Overheat!");
	}
      }
      break;
    }
    case LASER_OVERHEAT:{
      double off_time = tvend.tv_sec - laser_off_time.tv_sec + 1e-6 * (tvend.tv_usec - laser_off_time.tv_usec);
      if(off_time >= 5){
	laser_state = LASER_READY;
      }
      break;
    }
    default:
      break;
    }
    
    ros::spinOnce();
    loop_rate.sleep();
    ++count;
  }

  gp->closeGpio(LASER_IO);
  return 0;
}
