#include <ros\ros.h>
#include "C:\hiwin_ws\src\hiwin_driver\include\hiwin_driver\HRSDK.h"
//#include "HRSDK.h"
#include <string.h>
#include <windows.h>


#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include "control_msgs/FollowJointTrajectoryAction.h"

#pragma comment(lib, "C:\\hiwin_ws\\src\\hiwin_driver\\include\\hiwin_driver\\HRSDK.lib")
//#pragma comment(lib,"C:\\hiwin_ws\\src\\hiwin_driver\\src\\HRSDK.dll")

using namespace std;

void __stdcall callBack(uint16_t, uint16_t, uint16_t *, int)
{
}

int main(int argc, char **argv)
{

   

    ros::init(argc, argv, "test_hiwin_action");

    // create the action client
    // true causes the client to spin its own thread
    actionlib::SimpleActionClient<control_msgs::FollowJointTrajectoryAction> ac("hiwin_6dot/follow_joint_trajectory", true);

    ROS_INFO("Waiting for action server to start.");
    // wait for the action server to start
    ac.waitForServer(); //will wait for infinite time

    ROS_INFO("Action server started, sending goal.");
    // send a goal to the action
    control_msgs::FollowJointTrajectoryGoal goal;

    goal.trajectory.header.stamp = ros::Time::now() + ros::Duration(1.0);
    ac.sendGoal(goal);

    //wait for the action to return
    bool finished_before_timeout = ac.waitForResult(ros::Duration(30.0));

    if (finished_before_timeout)
    {
        actionlib::SimpleClientGoalState state = ac.getState();
        ROS_INFO("Action finished: %s", state.toString().c_str());
    }
    else
        ROS_INFO("Action did not finish before the time out.");

    //exit
    return 0;
}