//
// Created by ashvin on 3/3/16.
//
#include "ros/ros.h"
#include "std_msgs/String.h"

#include "geometry_msgs/Twist.h"
#include <stdexcept>
#include <cmath>
#include "turtlesim/Pose.h"
#include "turtlesim/Spawn.h"
#include "turtlesim/TeleportAbsolute.h"
#include "turtlesim/SetPen.h"

#define PI 3.14159265359


int main(int argc, char **argv) {

    //initialize the node
    ros::init(argc, argv, "vending_machine");

    //instantiate the node handle which is used for creating publishers and subscribers
    ros::NodeHandle n;

    return 0;
}

