cmake_minimum_required(VERSION 3.0.2)
project(iusc_laser)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED
  roscpp
)

## Generate added messages and services with any dependencies listed here
# generate_messages(
#   DEPENDENCIES
#   std_msgs  # Or other packages containing msgs
# )

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES signal_sub_pub
#  CATKIN_DEPENDS other_catkin_pkg
#  DEPENDS system_lib
)

###########
## Build ##
###########

include_directories(
 include
 ${catkin_INCLUDE_DIRS}
 ${Eigen3_INCLUDE_DIRS}
)

add_executable(iusc_laser
  src/iusc_laser.cpp
  src/gpio.cpp)
target_link_libraries(iusc_laser
  ${catkin_LIBRARIES}
  )
