#ifndef __MOTOR_H__
#define __MOTOR_H__

/** The point at which to switch the motor to operate backward. */
#define BACKWARD	200

/** The point at which to switch the motor to operate forward. */
#define FORWARD		201

/** The point at which the joystick position is neutral. */
#define MIDDLE_Y	179

void motor_init(void);
void setMotorDirection(int);
void setMotorSpeed(uint8_t);
void setMotorOFF(void) ;
void setMotorON(void);

#endif
