#ifndef _6050_H_
#define _6050_H_

#define limit_up_roll -5//上下限幅 1.6
#define limit_middle 0
#define limit_down_roll -11//5

#define limit_up_pitch 3//上下限幅 3
#define limit_middle 0
#define limit_down_pitch -3//3

extern void Roll_Angle_Control();
extern void Pitch_Angle_Control();

#endif