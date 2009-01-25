#ifndef __MOTOR_H__
#define __MOTOR_H__


/** disable default prescaler to make processor speed 8 MHz. */

#define BACKWARD	200
#define FORWARD		201

void motor_init(void);
void setMotorDirection(int);
void setMotorSpeed(uint8_t);
void setMotorOFF(void) ;
void setMotorON(void);

#endif
