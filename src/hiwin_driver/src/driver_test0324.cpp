#include <ros\ros.h>
#include "C:\hiwin_ws\src\hiwin_driver\include\hiwin_driver\HRSDK.h"
//#include "HRSDK.h"
#include <string.h>
#include <windows.h>

#include "actionlib/server/action_server.h"
#include "actionlib/server/server_goal_handle.h"
#include "moveit_msgs/DisplayTrajectory.h"
#include "control_msgs/FollowJointTrajectoryAction.h"
#include "control_msgs/FollowJointTrajectoryActionGoal.h"
#include "control_msgs/FollowJointTrajectoryActionResult.h"



#pragma comment(lib, "C:\\hiwin_ws\\src\\hiwin_driver\\include\\hiwin_driver\\HRSDK.lib")
//#pragma comment(lib,"C:\\hiwin_ws\\src\\hiwin_driver\\src\\HRSDK.dll")

using namespace std;
HROBOT device_id;
/*
class HiwinMoveitAction
{
  protected:
    
    ros::NodeHandle nh_;
    // !!! NodeHandle instance must be created before this line. Otherwise strange error occurs.
    //定義action服務端
    actionlib::ActionServer<control_msgs::FollowJointTrajectoryAction> as_;
    //定義action服務端目標控制
    actionlib::ServerGoalHandle<control_msgs::FollowJointTrajectoryAction> goal_handle_;

    //用來反饋action目標的執行狀況，客戶端可以由此得知服務端是否成功
    control_msgs::FollowJointTrajectoryResult result_;
    control_msgs::FollowJointTrajectoryFeedback feedback_;
    control_msgs::FollowJointTrajectoryGoal goal_;

    
    std::string action_name_;

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
    void executeCB(actionlib::ServerGoalHandle<control_msgs::FollowJointTrajectoryAction> gh)
    //void executeCB(const control_msgs::FollowJointTrajectoryGoalConstPtr msg)
    {
        cout << "in execution func\n";
        ROS_WARN("in execution CB func\n");
        actionlib::ActionServer<control_msgs::FollowJointTrajectoryAction>::Goal goal = *gh.getGoal(); //make a copy that we can modify

        goal_handle_ = gh;

        int Trajectory_num = 0; //How many Trajectory
        trajectory_msgs::JointTrajectoryPoint points;
        cout << "size: "<< goal.trajectory.points.size() <<endl;
        int point_size = goal.trajectory.points.size();
        ROS_WARN("goal.trajectory.points size:"+point_size);
        /*
        for (Trajectory_num = 0; Trajectory_num < goal.trajectory.points.size(); Trajectory_num++)
        {
            points = goal.trajectory.points[Trajectory_num];
            if (TrajectoryMove(points)) //假設軌跡成功後返回值為1，失敗為0
            {
                //tell client "success"
                goal_handle_.setAccepted();
                result_.error_code = result_.SUCCESSFUL;

                goal_handle_.setSucceeded(result_);
            }
        }
    }

    int TrajectoryMove(const trajectory_msgs::JointTrajectoryPoint points)
    {
        ROS_INFO("I heard an action msg~");

        int dot = 0;   //Robot arm dot number
        double pos[6]; //set radian -> degree

        for (dot = 0; dot < 6; dot++)
        {
            pos[dot] = points.positions[dot] * 57.29577951; //1 rad = （約57.29577951°）
                                                           //printf("%lf\t",pos[dot]);
        }
        //printf("round = %d\t\n",Trajectory_num+1);
        if (!ptp_axis(device_id, 1, pos))
        { //絕對關節角度點對點運動
            ROS_INFO("Motion send Successful");
        }
        double robot_cur_pos[6];
        int pos_eql = 0;
        //看有沒有到指定的點
        /*while (equal == 0)
            {
                if (get_current_joint(device_id, robot_cur_pos) == -1) //抓取位置失敗
                {
                    ROS_ERROR("Can't Get the Position of Robot");
                    pos_eql = -1;
                    //發送錯誤消息
                    return 0;
                }
                else
                {
                    for (dot = 0; dot < 6; dot++)
                    {
                        if (robot_cur_pos[dot] != pos[dot])
                        {
                            break;
                        }
                    }
                    if (dot == 6) //六個點都一樣
                    {
                        pos_eql = 1;
                        ROS_INFO("Motion receive Successful");
                        return 1;
                    }
                }
            }
        return 1;
    }
};
*/
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

    //HROBOT device_id = Connect("169.254.128.198", 1, callBack);
    HROBOT device_id =1 ;

    //此處變數設定有點不確定
    //HiwinMoveitAction HiwinMoveitAction("hiwin_6dot");

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
        /*set_override_ratio(device_id, 100);

        if (get_motor_state(device_id) == 0)
        {
            set_motor_state(device_id, 1); // Servo on
        }
        */
               
    }

    //ros::AsyncSpinner spinner(6); //ROS跑多個thread
    //spinner.start();
    ros::waitForShutdown();
    return (0);
}