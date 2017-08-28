#ifndef __PID_H__
#define __PID_H__

extern float X_PID_angle(int32 measured,int32 target);
extern int32 X_PID_PWM(float measured,float target);
extern float Y_PID_angle(int32 measured,int32 target);
extern int32 Y_PID_PWM(float measured,float target);
extern float X_PID_Circle(int32 measured,int32 target);
extern float Y_PID_Circle(int32 measured,int32 target);

#endif