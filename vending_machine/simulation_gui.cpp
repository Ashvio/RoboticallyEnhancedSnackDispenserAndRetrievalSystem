//a modified version of visit_door_list_gui, which can be found in ~/catkin_ws/src/bwi_common/bwi_tasks (on my machine, anyway...)
//to be used with roslaunch bwi_launch simulation.launch
/*
 * Example task intended to introduce new students
 * to the GUI and basic logical navigation
 * 
 */ 

#include "bwi_kr_execution/ExecutePlanAction.h"

#include <actionlib/client/simple_action_client.h>

#include <ros/ros.h>

#include <bwi_msgs/QuestionDialog.h>

//from move_base_client
#include <tf/tf.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <tf/transform_listener.h>
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/PoseArray.h"

#include <vector>
#include <iostream>
#include <tf/tf.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <tf/transform_listener.h>
#include <actionlib/server/simple_action_server.h>
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/PoseArray.h"

#include <vector>
#include <iostream>


typedef actionlib::SimpleActionClient<bwi_kr_execution::ExecutePlanAction> Client;

//using namespace std;

//position of the vending machine found using rviz and rostopic echo amcl_pose
double VENDING_MACHINE_POSE[] = {16.98, 3.75, 0.0, 0.0, 0.0, -0.096, 0.99};

//client needs to be initialized here so we can use it in multiple methods
Client client("/action_executor/execute_plan", true);

//uses move_base_client code to go to the preset vending machine location
void goToMachine(bwi_msgs::QuestionDialog question, ros::ServiceClient client_gui, 
				 int& argc, char** argv)
{
  ros::init(argc, argv, "move_base_client");
  ros::NodeHandle n;

  actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> ac("move_base",true);  
  
  move_base_msgs::MoveBaseGoal goal;
  
  while (ros::ok())
  {
    //set the header
    goal.target_pose.header.stamp = ros::Time::now();
    goal.target_pose.header.frame_id = "/map";
    
    //set position and orientation
    goal.target_pose.pose.position.x = VENDING_MACHINE_POSE[0];
    goal.target_pose.pose.position.y = VENDING_MACHINE_POSE[1];
    goal.target_pose.pose.position.z = VENDING_MACHINE_POSE[2];
    goal.target_pose.pose.orientation.x = VENDING_MACHINE_POSE[3];
    goal.target_pose.pose.orientation.y = VENDING_MACHINE_POSE[4];
    goal.target_pose.pose.orientation.z = VENDING_MACHINE_POSE[5]; 
    goal.target_pose.pose.orientation.w = VENDING_MACHINE_POSE[6];

    //send the goal
    ac.sendGoal(goal);
    
    //block until the action is completed
    ac.waitForResult();
  }

  bool placed = false;

  //loop until the human says the item has been placed
  while(!placed)
  {
  	question.request.message = "Please place the snack on my shelf! Has the item been placed? ";
	
	//these will appear as buttons the human can press to give input
        question.request.options.push_back("Yes");
        question.request.options.push_back("No");

	//after 30 seconds, move on
        question.request.timeout = 30.0;
		int current_door = 0;

	//if no response, set some default response that won't mess things up
        if (client_gui.call(question))
        {
                if (question.response.index >= 0){
                        current_door = question.response.index;
                }
                else {
                        ROS_INFO("No response detected, defaulting to 0");
                        current_door = 0;
                }
        }
        else
        {
                ROS_ERROR("Failed to call service /question_dialog");
                return;
        }

	//if the human entered yes, exit the loop
        placed  = question.response.text == "Yes";
  }

}

//go to the given door
void goToDoor(std::string location) {
	
	ROS_INFO_STREAM("going to " << location);

	bwi_kr_execution::ExecutePlanGoal goal;

	bwi_kr_execution::AspRule rule;
	bwi_kr_execution::AspFluent fluent;
	fluent.name = "not facing";

	fluent.variables.push_back(location);

	rule.body.push_back(fluent);
	goal.aspGoal.push_back(rule);

	ROS_INFO("sending goal");
	client.sendGoalAndWait(goal);

	if (client.getState() == actionlib::SimpleClientGoalState::ABORTED) {
	  ROS_INFO("Aborted");
	} else if (client.getState() == actionlib::SimpleClientGoalState::PREEMPTED) {
	  ROS_INFO("Preempted");
	}

	else if (client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
	  ROS_INFO("Succeeded!");
	} else {
	  ROS_INFO("Terminated");
	}
}

int main(int argc, char**argv) {
  ros::init(argc, argv, "visit_door_list_gui");
  ros::NodeHandle n;

  ros::NodeHandle privateNode("~");
  
  //vector of door strings... these can be sent to goToDoor because they are validly stored somewhere 
  std::vector<std::string> doors;

  doors.push_back("d3_414b1");
  doors.push_back("d3_414b2");
  doors.push_back("d3_414a1");
  doors.push_back("d3_414a2");
  doors.push_back("d3_418");
  int current_door = 0, snack_dest = 0;

  //vector of snack choices
  std::vector<std::string> snacks;
  
  snacks.push_back("goldfish");
  snacks.push_back("izze");
  snacks.push_back("none");

  client.waitForServer();

  ros::ServiceClient client_gui = n.serviceClient<bwi_msgs::QuestionDialog>("/question_dialog");
  
  while (ros::ok()) {
	
	//set up the thing that allows questions to appear on the GUI and the responses to be recorded  
	bwi_msgs::QuestionDialog question;
	question.request.type = question.request.CHOICE_QUESTION;
	
	//loop through all the doors, go to each one and ask if someone wants a snack
	for (int i = 0; i < doors.size(); i++) {
		
		//go to this door
		std::string location = doors.at(i);
		goToDoor(location);

		//ask for snack selection	
		question.request.message = "Please select a snack: ";
		for (unsigned int k = 0; k < snacks.size(); k++){
			question.request.options.push_back(snacks.at(k));
		}

		//if no response after 30 seconds, send a default and go to the next door
		question.request.timeout = 30.0;
		
		if (client_gui.call(question))
			{
			if (question.response.index >= 0){
				current_door = question.response.index;
			}
			else {
				ROS_INFO("No response detected, defaulting to 0");
				current_door = 0;
			}
		}
		else
		{
			ROS_ERROR("Failed to call service /question_dialog");
			return 1;
		}


	//if a snack was requested, go to the vending machine to get it then return to the door so we can loop again
		if(question.response.index != question.request.NO_RESPONSE || question.response.index != question.request.TIMED_OUT)
		{
			goToMachine(question,  client_gui, argc, argv);
			goToDoor(location);
		}
  }

  return 0;
}
}
