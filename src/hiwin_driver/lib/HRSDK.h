#ifndef HRSDK_HRSDK_H_
#define HRSDK_HRSDK_H_

#ifdef HRSDK_HRSDK_H_
#define HRSDK_API __declspec(dllexport)
#else
#define HRSDK_API __declspec(dllimport)
#endif

#include <cstdint>	// std::uint8_t
#include <vector>

typedef int HROBOT;
#ifdef __cplusplus
extern "C" {
#endif

enum Connection_Level {
	No_connection = -1,
	Monitor = 0,
	Controller,
};


enum operation_mode {
	Safety,
	Running,
};

enum space_system_type {
	Cart,
	Joint,
	Tool
};

enum space_direction {
	positive = 1,
	negative = -1,
};

enum robot_axis {
	joint_1,
	joint_2,
	joint_3,
	joint_4,
	joint_5,
	joint_6
};

enum robot_coor {
	Cart_x,
	Cart_y,
	Cart_z,
	Cart_a,
	Cart_b,
	Cart_c,
};

enum robot_motion_status {
	MS_IDLE = 1,
	MS_RUNNING = 2,
	MS_HOLD = 3,
	MS_DELAY = 4,
	MS_WAIT_CMD = 5
};

typedef void(__stdcall *callback_function)(uint16_t, uint16_t, uint16_t*, int);
HRSDK_API HROBOT __stdcall Connect(const char* address, int level, callback_function f);
HRSDK_API void __stdcall Close(HROBOT s);
HRSDK_API  int __stdcall get_HRSDK_version(char* version);
HRSDK_API int __stdcall get_connection_level(HROBOT s);
HRSDK_API int  __stdcall set_timer(HROBOT s, int index, int time);
HRSDK_API int  __stdcall get_timer(HROBOT s, int index);
HRSDK_API int  __stdcall set_counter(HROBOT s, int index, int co);
HRSDK_API int  __stdcall get_counter(HROBOT s, int index);
HRSDK_API int  __stdcall set_pr_type(HROBOT s, int prNum, int coorType);
HRSDK_API int  __stdcall get_pr_type(HROBOT s, int prNum);
HRSDK_API int  __stdcall set_pr_coordinate(HROBOT s, int prNum, double *coor);
HRSDK_API int  __stdcall get_pr_coordinate(HROBOT s, int pr, double* coor);
HRSDK_API int  __stdcall set_pr_tool_base(HROBOT s, int prNum, int toolNum, int baseNum);
HRSDK_API int  __stdcall get_pr_tool_base(HROBOT s, int pr, int* tool_base);
HRSDK_API int  __stdcall set_pr(HROBOT s, int prNum, int coorType, double *coor, int tool, int base);
HRSDK_API int __stdcall set_acc_dec_ratio(HROBOT s, int acc);
HRSDK_API int  __stdcall get_acc_dec_ratio(HROBOT s);
HRSDK_API int  __stdcall set_ptp_speed(HROBOT s, int vel);
HRSDK_API int  __stdcall get_ptp_speed(HROBOT s);
HRSDK_API int  __stdcall set_lin_speed(HROBOT s, double vel);
HRSDK_API double  __stdcall get_lin_speed(HROBOT s);
HRSDK_API int  __stdcall set_override_ratio(HROBOT s, int vel);
HRSDK_API int  __stdcall get_override_ratio(HROBOT s);
HRSDK_API int  __stdcall get_alarm_code(HROBOT s, int &count, uint64_t* alarm_code);
HRSDK_API int  __stdcall get_DI(HROBOT s, int index);
HRSDK_API int  __stdcall get_DO(HROBOT s, int index);
HRSDK_API int  __stdcall set_DO(HROBOT s, int index, bool v);
HRSDK_API int  __stdcall get_RI(HROBOT s, int index);
HRSDK_API int  __stdcall get_RO(HROBOT s, int index);
HRSDK_API int  __stdcall set_RO(HROBOT s, int index, bool v);
HRSDK_API int  __stdcall get_VO(HROBOT s, int index);
HRSDK_API int  __stdcall set_VO(HROBOT s, int index, bool v);
HRSDK_API int  __stdcall get_FO(HROBOT s, int index);
HRSDK_API int  __stdcall get_FI(HROBOT s, int index);
HRSDK_API int  __stdcall set_base_number(HROBOT s, int state);
HRSDK_API int  __stdcall get_base_number(HROBOT s);
HRSDK_API int  __stdcall define_base(HROBOT s, int baseNum, double *coor);
HRSDK_API int  __stdcall get_base_data(HROBOT s, int num, double* coor);
HRSDK_API int  __stdcall set_tool_number(HROBOT s, int num);
HRSDK_API int  __stdcall get_tool_number(HROBOT s);
HRSDK_API int  __stdcall define_tool(HROBOT s, int toolNum, double *coor);
HRSDK_API int  __stdcall get_tool_data(HROBOT s, int num, double* coor);
HRSDK_API int  __stdcall ext_task_start(HROBOT s, int mode, int select);
HRSDK_API int  __stdcall task_start(HROBOT s, char* task_name);
HRSDK_API int  __stdcall task_hold(HROBOT s);
HRSDK_API int  __stdcall task_continue(HROBOT s);
HRSDK_API int  __stdcall task_abort(HROBOT s);
HRSDK_API int  __stdcall set_motor_state(HROBOT s, int state);
HRSDK_API int  __stdcall get_motor_state(HROBOT s);
HRSDK_API int  __stdcall speed_limit_on(HROBOT s);
HRSDK_API int  __stdcall speed_limit_off(HROBOT s);
HRSDK_API int  __stdcall get_speed_limit_state(HROBOT s);
HRSDK_API int  __stdcall clear_alarm(HROBOT s);
HRSDK_API int  __stdcall ptp_pos(HROBOT s, int mode, double *p);
HRSDK_API int  __stdcall ptp_axis(HROBOT s, int mode, double *p);
HRSDK_API int  __stdcall ptp_rel_pos(HROBOT s, int mode, double *p);
HRSDK_API int  __stdcall ptp_rel_axis(HROBOT s, int mode, double *p);
HRSDK_API int  __stdcall ptp_pr(HROBOT s, int p);
HRSDK_API int  __stdcall lin_pos(HROBOT s, int mode, double smooth_value, double *p);
HRSDK_API int  __stdcall lin_axis(HROBOT s, int mode, double smooth_value, double *p);
HRSDK_API int  __stdcall lin_rel_pos(HROBOT s, int mode, double smooth_value, double *p);
HRSDK_API int  __stdcall lin_rel_axis(HROBOT s, int mode, double smooth_value, double *p);
HRSDK_API int  __stdcall lin_pr(HROBOT s, int mode, double smooth_value, int p);
HRSDK_API int  __stdcall circ_pos(HROBOT s, int mode, double* p_aux, double* p_end);
HRSDK_API int  __stdcall circ_axis(HROBOT s, int mode, double* p_aux, double* p_end);
HRSDK_API int  __stdcall circ_pr(HROBOT s, int mode, int p1, int p2);
HRSDK_API int  __stdcall motion_hold(HROBOT s);
HRSDK_API int  __stdcall motion_continue(HROBOT s);
HRSDK_API int  __stdcall motion_abort(HROBOT s);
HRSDK_API int  __stdcall motion_delay(HROBOT s, int delay);
HRSDK_API int  __stdcall set_smooth_length(HROBOT s, double r);
HRSDK_API int  __stdcall set_command_id(HROBOT s, int id);
HRSDK_API int  __stdcall get_command_id(HROBOT s);
HRSDK_API int  __stdcall get_command_count(HROBOT s);
HRSDK_API int  __stdcall remove_command(HROBOT s, int num);
HRSDK_API int  __stdcall remove_command_tail(HROBOT s, int num);
HRSDK_API int  __stdcall get_motion_state(HROBOT s);
HRSDK_API int  __stdcall get_encoder_count(HROBOT s, int32_t* EncCount);
HRSDK_API int  __stdcall get_current_joint(HROBOT s, double* joint);
HRSDK_API int  __stdcall get_current_position(HROBOT s, double* cart);
HRSDK_API int  __stdcall get_current_rpm(HROBOT s, double* rpm);
HRSDK_API int  __stdcall get_device_born_date(HROBOT s, int* YMD);
HRSDK_API int  __stdcall get_operation_time(HROBOT s, int* YMDHm);
HRSDK_API int  __stdcall get_mileage(HROBOT s, double* mil);
HRSDK_API int  __stdcall get_total_mileage(HROBOT s, double* tomil);
HRSDK_API int  __stdcall get_utilization(HROBOT s, int* utl);
HRSDK_API int _stdcall get_utilization_ratio(HROBOT s);
HRSDK_API int  __stdcall get_motor_torque(HROBOT s, double* cur);
HRSDK_API int __stdcall get_HRSS_version(HROBOT s,  char* ver);
HRSDK_API  int __stdcall get_HRSS_version_v2(HROBOT s, const char*& version);
HRSDK_API void  __stdcall get_robot_type(HROBOT s,  char* robType);
HRSDK_API void  __stdcall get_robot_type_v2(HROBOT s, const char*& robType);
HRSDK_API int  __stdcall jog(HROBOT robot, int space_type, int index, int dir);
HRSDK_API int  __stdcall jog_stop(HROBOT s);
HRSDK_API int  __stdcall jog_home(HROBOT s);
HRSDK_API int __stdcall get_robot_id(HROBOT s, char* robot_id);
HRSDK_API int __stdcall set_robot_id(HROBOT s, char* robot_id);
HRSDK_API  int __stdcall update_hrss(HROBOT s, char* path);
HRSDK_API int __stdcall send_file(HROBOT sock, char* root_folder, char*filename, int opt);
extern uint16_t ts;
#ifdef __cplusplus
}
#endif
#endif // HRSDK_HRSDK_H_
