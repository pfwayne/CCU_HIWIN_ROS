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
        ros::Rate r(1);
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
        goal_tolerance.assign(10,0);
        sensor_msgs::JointState hiwin_ref;
        hiwin_ref.name = goal->trajectory.joint_names;
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
                
                // Publish feedback
                feedback.header.stamp = ros::Time::now();
                //feedback.actual.positions = torso_meas_.position;
                feedback.desired = goal->trajectory.points[goal_index];
                // ToDo: error?
                as_.publishFeedback(feedback);

                // Count number of converged joints
                unsigned int n_converged = 0;
                double pos[6]; //set radian -> degree
                trajectory_msgs::JointTrajectoryPoint pos_point;
                printf("Goal index: %d\n",goal_index);
                for (unsigned int i = 0; i < n_joints; i++)
                {
                    pos [i] = hiwin_ref.position[i] * 57.29577951;
                    //cout << hiwin_ref.position[i] <<endl;
                    
                    printf("pos[%d]: %f\n",i,pos[i]);
                    ++n_converged;
                }
                if (!ptp_axis(device_id, 1, pos))
                { //絕對關節角度點對點運動
                    ROS_INFO("Motion send Successful");
                }
                if (n_converged == n_joints)
                {
                    ++goal_index;
                }
            }
            r.sleep();
        }
        /*
        int num = goal->trajectory.points.size();
        cout<< num <<endl;*/
        // check that preempt has not been requested by the client
        if (as_.isPreemptRequested() || !ros::ok())
        {
            ROS_INFO("%s: Preempted", action_name_.c_str());
            // set the action state to preempted
            as_.setPreempted();
            success = false;
        }
        //feedback_.sequence.push_back(feedback_.sequence[i] + feedback_.sequence[i - 1]);
        // publish the feedback
        //as_.publishFeedback(feedback);
        // this sleep is not necessary, the sequence is computed at 1 Hz for demonstration purposes
        //r.sleep();
        /*
        if (success)
        {
            //result_.sequence = feedback_.sequence;
            ROS_INFO("%s: Succeeded", action_name_.c_str());
            // set the action state to succeeded
            as_.setSucceeded(result_);
        }
        */
    }
    
    
};

void __stdcall callBack(uint16_t, uint16_t, uint16_t *, int)
{
}

int main(int argc, char **argv)
{
    string host;


    ros::init(argc, argv, "hiwin_driver");
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

		//HiwinMoveitAction HiwinMoveitAction("hiwin_6dot/follow_joint_trajectory");

        //test PTP
        /*set_override_ratio(device_id, 100);

        if (get_motor_state(device_id) == 0)
        {
            set_motor_state(device_id, 1); // Servo on
            
        }*/
        
    }
	
	

    //ros::AsyncSpinner spinner(6); //ROS跑多個thread
    //spinner.start();
    ros::spin();
    return (0);
}