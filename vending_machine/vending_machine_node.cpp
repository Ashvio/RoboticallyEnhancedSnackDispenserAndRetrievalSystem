// vending machine node
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





int main(int argc, char **argv) 
{
    //initialize the node
    ros::init(argc, argv, "vending_machine");

    //instantiate the node handle which is used for creating publishers and subscribers
    ros::NodeHandle n;

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














