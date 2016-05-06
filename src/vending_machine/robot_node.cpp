//robot node
// Created by JG and JKC on 5.5
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

//from move_base_client
#include <tf/tf.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <tf/transform_listener.h>
#include <actionlib/server/simple_action_server.h>
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/PoseArray.h"

#include <vector>
#include <iostream>

#define PI 3.14159265359

ros::ServiceClient client;
ros::Publisher publish_snack;

bool heard;

bool create_new()
{
	//create a new service
	nodeName::newThing service;

	//set the new thing to the passed parameters
	service.request.variableName?

	if(client.call(service))
	{
		ROS_INFO("Response: %s", service.response.n);
		
		return true;
	}
	else
	{	
		ROS_WARN("New thing failed.");
		return false;
	}
	
}

void callback(cost nodeName::Pose::ConstPtr& msg)
{
	//set variables here
	heard = true;
}

void get_data()
{
	heard = false;
	float framerate = 40.0;
	ros::Rate loop_rate(framerate);

	while(!heard)
	{
		loop_rate.sleep();
		ros::spinOnce();
	}
}

int main(int argc, char **argv) 
{
    //initialize the node
    ros::init(argc, argv, "vending_machine");

    //instantiate the node handle which is used for creating publishers and subscribers
    ros::NodeHandle node_handle;

	client = node_handle.serviceClient<nodeName::::Spawn("/spawn")>;;
	
	ros::service::waitForService("/spawn");

	ros::Subscriber get_action = node_handle.subscribe("/object_name/pose", 1,  callback);

	get_data();
	
	while(ros::ok())
	{
		//do something...?
	}


	//human orders food
	bool order = true;
	string choiceEntry;

	while (order){

		cout << "Please enter if you would like a drink or a snack: "
		cin >> choiceEntry;
		if (choiceEntry.equals("drink") || choiceEntry.equals("snack"){
			order = false;
		}
	}

	orderItem(choiceEntry);


    return 0;

}


void orderItem(String choiceEntry)
{
	if(choiceEntry.equals("drink"))
	{
		moveMotor(1);
	}	
	else if(choiceEntry.equals("snack"))
	{
		moveMotor(2);
	}
	else
	{
		System.out.println("How did this even happen");
	}
}

void moveMotor()
{
	
}




//from move_base_client (in move_base_client)
//we need to always be able to go to the vending machine from current location
//~/catkin_ws/src/bwi_common/bwi_mapper/src/libbwi_mapper/path_finder.cpp ???
Actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> ac("move_base",true);
  
  move_base_msgs::MoveBaseGoal goal;
  
  double x = 0;
  double yaw = 0;
  
  while (ros::ok())
  {
    std::cout<<"enter forward distance:\n";
    std::cin>>x;
    std::cout<<"enter rotation angle:\n";
    std::cin>>yaw;

    //set the header
    goal.target_pose.header.stamp = ros::Time::now();
    goal.target_pose.header.frame_id = "/base_link";

    //set relative x, y, and angle
    goal.target_pose.pose.position.x = x;
    goal.target_pose.pose.position.y = 0.0;
    goal.target_pose.pose.position.z = 0.0;
    goal.target_pose.pose.orientation = tf::createQuaternionMsgFromYaw(yaw);

    //send the goal
    ac.sendGoal(goal);

    //block until the action is completed
    ac.waitForResult();
  }










