#include <ros\ros.h>
#include "C:\hiwin_ws\src\test\include\test\HRSDK.h"
//#include "HRSDK.h"
#include <string.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>

#pragma comment(lib,"C:\\hiwin_ws\\src\\test\\include\\test\\HRSDK.lib")
//#pragma comment(lib,"C:\\hiwin_ws\\src\\test\\src\\HRSDK.dll")

using namespace std;

void __stdcall callBack(uint16_t, uint16_t, uint16_t*, int) {

}


int main(int argc, char **argv)
{
    string host;
    
    ros::init(argc, argv, "test");
	/*if (!(ros::param::get("~robot_ip", host))) 
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
	}*/

    HROBOT device_id = Connect("169.254.128.198", 1, callBack);
    if (device_id < 0)
    {
        cout<<"Failed Connection.\n";
        ROS_WARN("Failed Connection.\n");
    }
    else
    {
        //cout << "Successful Connection\n";
        /*ROS_WARN("Successful Connection.\n");

        //test PTP
        	set_override_ratio(device_id, 100);

        if (get_motor_state(device_id) == 0)
        {
            set_motor_state(device_id, 1);   // Servo on
        }*/

/*       const int pointNum = 8; 
        int pointIdx = 0;
        double x[pointNum] = { 0 };
        double y[pointNum] = { 0 };
        double z[pointNum] = { 0 };
        int xoffset = 100;
        int zoffset = 50;
        double pos[6] = { 0, 450, 200, 180, 0, 90 };
		
        //make path
        x[pointIdx] = pos[0];
        y[pointIdx] = pos[1];
        z[pointIdx] = pos[2];
        pointIdx++;
                
        x[pointIdx] = pos[0] + xoffset;
        y[pointIdx] = pos[1];
        z[pointIdx] = pos[2];
        pointIdx++;
                
        x[pointIdx] = pos[0] + xoffset;
        y[pointIdx] = pos[1];
        z[pointIdx] = pos[2] - zoffset;
        pointIdx++;

        x[pointIdx] = pos[0] + xoffset;
        y[pointIdx] = pos[1];
        z[pointIdx] = pos[2];
        pointIdx++;

        x[pointIdx] = pos[0] - xoffset;
        y[pointIdx] = pos[1];
        z[pointIdx] = pos[2];
        pointIdx++;

        x[pointIdx] = pos[0] - xoffset;
        y[pointIdx] = pos[1];
        z[pointIdx] = pos[2] - zoffset;
        pointIdx++;

        x[pointIdx] = pos[0] - xoffset;
        y[pointIdx] = pos[1];
        z[pointIdx] = pos[2];
        pointIdx++;

        x[pointIdx] = pos[0];
        y[pointIdx] = pos[1];
        z[pointIdx] = pos[2];*/
		
        //ptp motion
            /*for (int a = 0; a < pointNum; a++) {
                pos[0] = x[a];
                pos[1] = y[a];
                pos[2] = z[a];
                while (get_command_count(device_id) > 100) {
                    Sleep(500);
                }
                ptp_pos(device_id, 1, pos);
            }*/
		
		
		/*double pick_home[6] = {0, 0, 0, 0, -90, 0};
		double place_home[6] = {-90, 0, 0, 0, -90, 0};
		double pick_angle[6] = { 0, -45, 0, 0, 45, 90 };
		double place_angle[6] = { -90, -45, 0, 0, 45, 90 };
		double pos1[6]={0, 50, 0, 0, 0, 0};
		double pos2[6]={0, -50, 0, 0, 0, 0};
		double pos3[6]={50, 0, 0, 0, 0, 0};
		double pos4[6]={-50, 0, 0, 0, 0, 0};
		
		speed_limit_off(device_id);
		set_acc_dec_ratio(device_id, 50);
		
		ptp_axis(device_id, 1, pick_home);
		ptp_axis(device_id, 1, pick_angle);
		motion_delay(device_id, 1000);
		ptp_rel_pos(device_id, 1, pos1);
		motion_delay(device_id, 1000);
		ptp_rel_pos(device_id, 1, pos2);
		ptp_axis(device_id, 1, pick_home);
		ptp_axis(device_id, 1, place_home);
		ptp_axis(device_id, 1, place_angle);
		motion_delay(device_id, 1000);
		ptp_rel_pos(device_id, 1, pos3);
		motion_delay(device_id, 1000);
		ptp_rel_pos(device_id, 1, pos4);
		ptp_axis(device_id, 1, place_home);
		ptp_axis(device_id, 1, pick_home);*/
		
		char motion[15];
		double input[6];
		int c = 1;
		
		do{
			
		cout<<"Please type in your motion"<<endl;
		cin>>motion;
		for(int i=0; i<6; i++){
			cin>>input[i];
		}
		
		if(strncmp(motion, "ptppos", 15) == 0){
			cout<<"do ptppos"<<endl;
			for(int i=0; i<6; i++){
				cout<<input[i]<<" ";
			}
			cout<<endl;
			ptp_pos(device_id, 1, input);
		}else if(strncmp(motion, "ptprelpos", 15) == 0){
			cout<<"do ptprelpos"<<endl;
			for(int i=0; i<6; i++){
				cout<<input[i]<<" ";
			}
			cout<<endl;
			ptp_rel_pos(device_id, 1, input);
		}else if(strncmp(motion, "ptpaxis", 15) == 0){
			cout<<"do ptpaxis"<<endl;
			for(int i=0; i<6; i++){
				cout<<input[i]<<" ";
			}
			cout<<endl;
			ptp_axis(device_id, 1, input);
		}else if(strncmp(motion, "ptprelaxis", 15) == 0){
			cout<<"do ptprelaxis"<<endl;
			for(int i=0; i<6; i++){
				cout<<input[i]<<" ";
			}
			cout<<endl;
			ptp_rel_axis(device_id, 1, input);
		}else{
			cout<<"undefine motion"<<endl;
		}
		}while(c==1);
    }

    //ros::AsyncSpinner spinner(6); //ROS跑多個thread 
	//spinner.start();
	ros::waitForShutdown();
	return(0);
}