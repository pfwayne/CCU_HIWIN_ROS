#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>

#include <moveit/robot_trajectory/robot_trajectory.h>
#include <moveit/trajectory_processing/iterative_time_parameterization.h>

#include <moveit_msgs/ExecuteKnownTrajectory.h>

int main(int argc, char **argv)
{
  // local variables
  float pi = 3.14159;
  static const std::string PLANNING_GROUP = "hiwin_arm";

  ros::init(argc, argv, "move_group_interface_tutorial");
  ros::NodeHandle node_handle;
  ros::AsyncSpinner spinner(1);
  spinner.start();

  // Setup
  moveit::planning_interface::MoveGroupInterface group(PLANNING_GROUP);
  // note the name of the planning group. This is for UR# robots
  moveit::planning_interface::PlanningSceneInterface planning_scene_interface;
  
  const robot_state::JointModelGroup* joint_model_group =
      group.getCurrentState()->getJointModelGroup(PLANNING_GROUP);


  // Getting Basic Information
  // ^^^^^^^^^^^^^^^^^^^^^^^^^
  // We can print the name of the reference frame for this robot.
  ROS_INFO("Reference frame: %s", group.getPlanningFrame().c_str());

  // We can also print the name of the end-effector link for this group.
  ROS_INFO("End effector frame: %s", group.getEndEffectorLink().c_str());

  //group.setPoseReferenceFrame("elbow_joint"); // all coordinates are relative to the position of the end effector
  moveit::planning_interface::MoveGroupInterface::Plan my_plan;

 // Planning a Cartesian Path
  // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  group.startStateMonitor();
  group.setStartStateToCurrentState();
  group.setPlanningTime(12);

  ROS_INFO_STREAM("current EE pose: " << group.getCurrentPose());
  
  moveit::core::RobotStatePtr current_state = group.getCurrentState();
  //
  // Next get the current set of joint values for the group.
  std::vector<double> joint_group_positions;
  current_state->copyJointGroupPositions(joint_model_group, joint_group_positions);

  // Now, let's modify one of the joints, plan to the new joint space goal and visualize the plan.
  joint_group_positions[0] = 1.0;  // radians
  group.setJointValueTarget(joint_group_positions);
  bool success = (group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);


  ROS_INFO_NAMED("tutorial", "Visualizing plan 2 (joint space goal) %s", success ? "" : "FAILED");
  group.move();


  ros::shutdown();
  return 0;


}