/*
 * MotorControl.h
 */

#ifndef MOTORCONTROL_H_
#define MOTORCONTROL_H_

void SetupMotorControl();
void MotorControlUpdate();
int MotorContolGetCurrentMilliamps(int motor);
bool MotorContolGetFault(int motor);

#endif /* MOTORCONTROL_H_ */
