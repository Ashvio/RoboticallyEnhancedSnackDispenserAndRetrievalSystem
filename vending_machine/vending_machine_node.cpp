// vending machine node
// Created by ashvin on 3/3/16.
// (code base that doesn't do much yet)
#include "ros/ros.h"
#include "std_msgs/String.h"

//taken from a homework that used turtlesim
#include "geometry_msgs/Twist.h"
#include <stdexcept>
#include <cmath>
#include "turtlesim/Pose.h"
#include "turtlesim/Spawn.h"
#include "turtlesim/TeleportAbsolute.h"
#include "turtlesim/SetPen.h"

#define PI 3.14159265359


int main(int argc, char **argv) 
{
    //initialize the node
    ros::init(argc, argv, "vending_machine");

    //instantiate the node handle which is used for creating publishers and subscribers
    ros::NodeHandle n;

	//human orders food
	bool order = true;
	string choiceEntry;

	//get a valid order
	while (order){

		cout << "Please enter if you would like a drink or a snack: "
		cin >> choiceEntry;
		if (choiceEntry.equals("drink") || choiceEntry.equals("snack"){
			order = false;
		}
	}

	//move the motors to release the item
	orderItem(choiceEntry);

    return 0;
}

//call the method to move the correct motor to get the given item
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
		//shouldn't get here because of the while loop in main
		System.out.println("How did this even happen");
	}
}

//send the signal to move the motors
void moveMotor()
{
	
}














