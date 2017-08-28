#ifndef __GY_901_H__
#define __GY_901_H__


void JY_901_Init();
void JY_901_GetData(float *roll_angle,float *pitch_angle,float *angular_velocity_z);
void Angle_filtering(float *roll_date,float *pitch_date);


#endif