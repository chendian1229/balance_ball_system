#ifndef _CONTROL_H_
#define _CONTROL_H_

#define MOTOR_FTM   FTM0
#define MOTOR1_PWM  FTM_CH0
#define MOTOR2_PWM  FTM_CH1
#define MOTOR3_PWM  FTM_CH2
#define MOTOR4_PWM  FTM_CH3

#define MOTOR_HZ    10000//(20*1000)
#define PWM_OutPut_test 500    //电机测试输出，占空比50%


extern int32 TOP;
extern int32 DOWN;

extern uint8 allow_control_flag;
extern uint8 stop_Roll_flag;
extern uint8 stop_Pitch_flag;
extern int forbid_test;
extern int32 Pid_position_x;
extern int32 Pid_position_y;
extern uint32 posx_last;
extern uint32 posy_last;
extern uint32 direct;

extern void Platform_Control();

extern void Digital_filtering();


#endif