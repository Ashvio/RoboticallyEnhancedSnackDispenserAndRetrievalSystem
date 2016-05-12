//robot node
// Created by JCG and JKC on 5.5
// (a code base that doesn't currently do much)
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

//initialize the client and publisher
ros::ServiceClient client;
ros::Publisher publishSnack;

//for the callback function
bool heard;

//constants
final double RUN_FOR = 5.0, WAIT_TIME = 60.0;


//initialize something... from homework 4
bool createNew()
{
	//create a new service
	nodeName::newThing service;

	//set the new thing to the passed parameters
	service.request.variableName?

	//make sure it worked
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

//a callback function
void callback(cost nodeName::Pose::ConstPtr& msg)
{
	//set variables here
	heard = true;
}

//get the data
void getData()
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
    ros::init(argc, argv, "robot_node");

    //instantiate the node handle which is used for creating publishers and subscribers
    ros::NodeHandle nodeHandle;

	client = nodeHandle.serviceClient<nodeName::::Spawn("/spawn")>;;
	
	ros::service::waitForService("/spawn");

	ros::Subscriber getAction = nodeHandle.subscribe("/object_name/pose", 1,  callback);

	getData();
	
	while(ros::ok())
	{
		//do something...?
	}


	//human orders food
	bool order = true;
	string choiceEntry;

	//make sure the order is valid
	while (order){

		cout << "Please enter if you would like a drink or a snack: "
		cin >> choiceEntry;
		if (choiceEntry.equals("drink") || choiceEntry.equals("snack"){
			order = false;
		}
	}

	//save the current postion so the robot can get back here with the snack
	Pose returnHere = currentPose;
	
	orderItem(choiceEntry);

	moveTo(VENDING_MACHINE_POS);

	moveTo(currentPose);

    return 0;
}

//send the raspberry pi the number of the slot associated with the requested snack item
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
		//should not have been able to get past the loop in main
		System.out.println("How did this even happen");
	}
}

//raspberry pi uses GPIO or something else to run the given motor RUN_FOR times  
//returns true if the motor was able to move, false otherwise
bool moveMotor(int slotNumber)
{
	if(slotNumber == 1)
	{
		//send a signal to the first motor
		//run for either RUN_FOR time or RUN_FOR rotations
		//does the raspberry pi thing have a way to check if it actually ran? 
	}
	else if(slotNumber == 2)
	{
		//send a signal to the second motor 
		//run for either RUN_FOR time or RUN_FOR rotations
	}
	else
	{
		//should not have gotten to this point... 
		return false;
	}
	
	return true;
}



void moveTo()
{
//from move_base_client (in move_base_client)
//we need to always be able to go to the vending machine from current location
//~/catkin_ws/src/bwi_common/bwi_mapper/src/libbwi_mapper/path_finder.cpp ???
//potentially do something with a waypoint here? 
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
}

//the robot gets the message from the vending machine and asks a human for help
bool getSnack()
{
	double start = ros::Time::now().toSec();
	double end = start + WAIT_TIME;

	//loop for WAIT_TIME seconds or until the human says the snack has been placed
	while(start >= end)
	{
		//make a noise
		//somehow

		//display message asking human for help
		ROS_INFO("Could you place the snack safely on me?");
		
		cout << "If the snack has been placed, enter true: "
                cin >> snackPlaced;

		if(snackPlaced == "true" || "True" || "T" || "t")
		{
			return true;
		}

		start = ros::Time::now().toSec();
	}

	return false;
}
