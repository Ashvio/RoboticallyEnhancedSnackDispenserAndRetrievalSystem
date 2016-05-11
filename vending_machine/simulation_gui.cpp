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

//found using rviz 
double VENDING_MACHINE_POSE[] = {16.98, 3.75, 0.0, 0.0, 0.0, -0.096, 0.99};
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

  while(!placed)
  {
  	question.request.message = "Please place the snack on my shelf! Has the item been placed? ";
	
        question.request.options.push_back("Yes");
        question.request.options.push_back("No");

        question.request.timeout = 30.0;
		int current_door = 0;
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
        placed  = question.response.text == "Yes";
  }

}

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
  
  std::vector<std::string> doors;

  doors.push_back("d3_414b1");
  doors.push_back("d3_414b2");
  doors.push_back("d3_414a1");
  doors.push_back("d3_414a2");
  doors.push_back("d3_418");
  int current_door = 0, snack_dest = 0;

  std::vector<std::string> snacks;
  
  snacks.push_back("goldfish");
  snacks.push_back("izze");
  snacks.push_back("none");

  client.waitForServer();

  ros::ServiceClient client_gui = n.serviceClient<bwi_msgs::QuestionDialog>("/question_dialog");
  
  while (ros::ok()) {
	  
	//use gui to decide which door to go to
	bwi_msgs::QuestionDialog question;
	question.request.type = question.request.CHOICE_QUESTION;
	for (int i = 0; i < doors.size(); i++) {
		std::string location = doors.at(i);
		goToDoor(location);
		//TODO: ask for snack instead, go to the next door in the end	
		question.request.message = "Please select a snack: ";
		for (unsigned int k = 0; k < snacks.size(); k++){
			question.request.options.push_back(snacks.at(k));
		}
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


	//if a snack was requested, location = vending machine
		if(question.response.index != question.request.NO_RESPONSE || question.response.index != question.request.TIMED_OUT)
		{
			goToMachine(question,  client_gui, argc, argv);
			goToDoor(location);
		}
		
		//do this
		
		

  }

  return 0;
}
}
