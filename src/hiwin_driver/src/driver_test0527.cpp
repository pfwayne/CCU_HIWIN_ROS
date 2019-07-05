#include <ros\ros.h>
#include "C:\hiwin_ws\src\hiwin_driver\include\hiwin_driver\HRSDK.h"
//#include "HRSDK.h"
#include <string.h>
#include <windows.h>

#include <actionlib/server/simple_action_server.h>
#include "actionlib/server/server_goal_handle.h"
#include "moveit_msgs/DisplayTrajectory.h"
#include "control_msgs/FollowJointTrajectoryAction.h"
#include "control_msgs/FollowJointTrajectoryActionGoal.h"
#include "control_msgs/FollowJointTrajectoryActionResult.h"

#pragma comment(lib, "C:\\hiwin_ws\\src\\hiwin_driver\\include\\hiwin_driver\\HRSDK.lib")
//#pragma comment(lib,"C:\\hiwin_ws\\src\\hiwin_driver\\src\\HRSDK.dll")

using namespace std;
HROBOT device_id;

class HiwinMoveitAction
{
  protected:
    ros::NodeHandle nh_;
    // !!! NodeHandle instance must be created before this line. Otherwise strange error occurs.
    //定義action服務端
    actionlib::SimpleActionServer<control_msgs::FollowJointTrajectoryAction> as_;
    //定義action服務端目標控制
    //actionlib::ServerGoalHandle<control_msgs::FollowJointTrajectoryAction> goal_handle_;

    std::string action_name_;

    //用來反饋action目標的執行狀況，客戶端可以由此得知服務端是否成功
    control_msgs::FollowJointTrajectoryResult result_;
    //control_msgs::FollowJointTrajectoryFeedback feedback_;
    //control_msgs::FollowJointTrajectoryGoal goal_;
	//ros::Publisher joint_pub = nh_.advertise<sensor_msgs::JointState>("/move_group/hiwin_6dot_moveit_joint_states", 1);
  public:
    //以下為初始化列表
    HiwinMoveitAction(std::string name) : //nh_: action 所在ros節點的節點名稱
                                          //name: 為action的名字，server&client的依據 參考controllers.yaml參數來寫
                                          //boost為固定用法
                                          as_(nh_, name, boost::bind(&HiwinMoveitAction::executeCB, this, _1), false),
                                          action_name_(name)
    {
        cout << "start action\n";
        as_.start(); //啟動action server

        ROS_WARN("start action server by hiwin_driver");
    }
    ~HiwinMoveitAction(void)
    {
    }
    //寫callback函數
    void executeCB(const control_msgs::FollowJointTrajectoryGoalConstPtr &hiwin_goal)
    //void executeCB(const control_msgs::FollowJointTrajectoryGoalConstPtr msg)
    {

        // helper variables
        ros::Rate r(10);
        ros::NodeHandle n;
        bool success = true;

        // publish info to the console for the user
        ROS_INFO("%s: Executing", action_name_.c_str());
        control_msgs::FollowJointTrajectoryGoalConstPtr goal = hiwin_goal;
        control_msgs::FollowJointTrajectoryFeedback feedback;

        int goal_index = 0;
        int n_joints = goal->trajectory.joint_names.size(); //6
        feedback.joint_names = goal->trajectory.joint_names;
        std::vector<double> goal_tolerance;
        goal_tolerance.assign(10, 0);
        sensor_msgs::JointState hiwin_ref;
        hiwin_ref.name = goal->trajectory.joint_names;
        sensor_msgs::JointState hiwin_return;

        //setGoalTolerance(goal, goal_tolerance);

        // start executing the action

        while (n.ok() && as_.isActive())
        {

            if (as_.isNewGoalAvailable())
            {
                ROS_INFO("New goal received.");
                goal = as_.acceptNewGoal();
                goal_index = 0;
                n_joints = goal->trajectory.joint_names.size();
                hiwin_ref.name = goal->trajectory.joint_names;
                feedback.joint_names = goal->trajectory.joint_names;
                //setGoalTolerance(goal, goal_tolerance);
            }
            if (goal_index == goal->trajectory.points.size())
            {
                control_msgs::FollowJointTrajectoryResult result;
                result.error_code = 0;
                as_.setSucceeded(result, "Hiwin  goal succeeded!");
                ROS_INFO("Hiwin  goal succeeded!");
            }
            else
            {
                hiwin_ref.position = goal->trajectory.points[goal_index].positions;
                // Count number of converged joints
                unsigned int n_converged = 0;
                double pos[6]; //set radian -> degree
                trajectory_msgs::JointTrajectoryPoint pos_point;
                printf("Goal index: %d\n", goal_index);
                for (unsigned int i = 0; i < n_joints; i++)
                {
                    pos[i] = hiwin_ref.position[i] * 57.29577951;
                    //cout << hiwin_ref.position[i] <<endl;

                    printf("pos[%d]: %f\n", i, pos[i]);
                    ++n_converged;
                }
                if (!ptp_axis(device_id, 1, pos))
                { //絕對關節角度點對點運動
                    ROS_INFO("Motion send Successful");
                }
				ROS_INFO("conunt: %d", get_command_count(device_id));

                // Publish feedback
                double return_pos[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
                if (device_id >= 0)
                {
                    feedback.header.stamp = ros::Time::now();
                    int getcur = get_current_joint(device_id, return_pos);
                    if (getcur == -1) //抓取位置失敗
                    {
                        ROS_ERROR("Can't Get the Position of Robot");
                        //發送錯誤消息
                        //return 0;
                    }
                    else
                    {
                        hiwin_return.name = goal->trajectory.joint_names;
                        hiwin_return.position.clear();
                        cout << "----------------feedback current pos--------------------" << endl;
                        for (unsigned int i = 0; i < n_joints; i++)
                        {
                            printf("return_pos[%d]: %f\n", i, return_pos[i]);
                            if (return_pos[i] != 0.0)
                            {
                                return_pos[i] = return_pos[i] / 57.29577951;
                            }
                            hiwin_return.position.push_back(return_pos[i]);
                            //printf("hiwin_return[%d]: %f\n", i, hiwin_return.position[i]);
                        }
                        cout << "--------------------------------------------------------" << endl;
                    }
                    feedback.actual.positions = hiwin_return.position;
                    feedback.desired = goal->trajectory.points[goal_index];
					
					hiwin_return.header.stamp = ros::Time::now();
					
					//joint_pub.publish(hiwin_return);
					
                    // ToDo: error?
                    as_.publishFeedback(feedback);
                }

                if (n_converged == n_joints)
                {
                    ++goal_index;
                }
            }
            r.sleep();
        }

        // check that preempt has not been requested by the client
        if (as_.isPreemptRequested() || !ros::ok())
        {
            ROS_INFO("%s: Preempted", action_name_.c_str());
            // set the action state to preempted
            as_.setPreempted();
            success = false;
        }
    }
};

void __stdcall callBack(uint16_t, uint16_t, uint16_t *, int)
{
}

int main(int argc, char **argv)
{
    string host;

    ros::init(argc, argv, "hiwin_driver");
	ros::NodeHandle nh_;
    if (!(ros::param::get("~robot_ip", host)))
    {
        if (argc > 1)
        {
            host = argv[1];
        }
        else
        {
            ROS_WARN("Could not get robot ip. Please supply it as command line parameter or on the parameter server as robot_ip");
            exit(1);
        }
    }

    HROBOT device_id = Connect("169.254.128.198", 1, callBack);
    //HROBOT device_id = 1;

    //此處變數設定有點不確定
    //action server test
    //HiwinMoveitAction HiwinMoveitAction("hitrajectory");
    HiwinMoveitAction HiwinMoveitAction("hiwin_6dot/follow_joint_trajectory");

    if (device_id < 0)
    {
        cout << "Failed Connection.\n";
        //ROS_ERROR("Failed Connection.\n");
        ROS_WARN("Failed Connection.\n");
    }
    else
    {
        cout << "Successful Connection\n";
        ROS_INFO("Successful Connection.\n");

        //test PTP
        set_override_ratio(device_id, 100);

        if (get_motor_state(device_id) == 0)
        {
            set_motor_state(device_id, 1); // Servo on
                                           // HiwinMoveitAction HiwinMoveitAction("hiwin_6dot/follow_joint_trajectory");
        }
    }
	
	ros::Rate loop_rate(10);
	ros::Publisher joint_pub = nh_.advertise<sensor_msgs::JointState>("/move_group/hiwin_6dot_moveit_joint_states", 10);
	sensor_msgs::JointState hiwin_return;
	double return_pos[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	hiwin_return.name.push_back("j_0001");
	hiwin_return.name.push_back("j_0102");
	hiwin_return.name.push_back("j_0203");
	hiwin_return.name.push_back("j_0304");
	hiwin_return.name.push_back("j_0405");
	hiwin_return.name.push_back("j_0506");
    while (ros::ok())
    {
        /*...TODO...*/ 
		if (device_id >= 0)
                {
                    int getcur = get_current_joint(device_id, return_pos);
                    if (getcur == -1) //抓取位置失敗
                    {
                        ROS_ERROR("Can't Get the Position of Robot");
                        //發送錯誤消息
                        //return 0;
                    }
                    else
                    {
                       
                        hiwin_return.position.clear();
                       
                        for (unsigned int i = 0; i < 6; i++)
                        {
                            
                            if (return_pos[i] != 0.0)
                            {
                                return_pos[i] = return_pos[i] / 57.29577951;
                            }
                            hiwin_return.position.push_back(return_pos[i]);
                            //printf("hiwin_return[%d]: %f\n", i, hiwin_return.position[i]);
                        }
                       
                    }
			
					hiwin_return.header.stamp = ros::Time::now();
					
					joint_pub.publish(hiwin_return);
					
                }
        ros::spinOnce();
        loop_rate.sleep();
    }

    //ros::AsyncSpinner spinner(6); //ROS跑多個thread
    //spinner.start();
    //ros::spin();
    return (0);
}