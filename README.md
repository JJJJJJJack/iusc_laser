# iusc_laser
IUSC competition ROS node for throttled laser shooter control. The node is compiled and tested on the Allspark-NX board provided by AmovLab.

## Description
The laser shooter is controlled through topic `/laser_command` with type `std_msgs/Bool`. The laser shooter is triggered when the value is set to true. **It can only stay on for no more than 5 sec. It also requires a 5 sec rest time between every two triggers.**

## Topics
`/laser_command` with type `std_msgs/Bool`

## Startup configuration
To run the ros nodes needed for IUSC program, the [robot-upstart](http://wiki.ros.org/robot_upstart) ros package is used. Follow the command below to setup the startup configuration files.
```
sudo apt-get update && sudo apt-get install ros-melodic-robot-upstart
rosrun robot_upstart install p450_experiment/launch_basic/p450_optitrack.launch --job iusc_startup --symlink
```
* Naming is configured by `--job iusc_startup`.
* Link file instead of launch file created for startup by using `--symlink`.
