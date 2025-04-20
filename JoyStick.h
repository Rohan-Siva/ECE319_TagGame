#ifndef __OURJOYSTICK_H__
#define __OURJOYSTICK_H__

#include <stdint.h>

void Joystick_Init(void);

uint32_t Joystick1_ReadX(void);
uint32_t Joystick1_ReadY(void);
uint8_t  Joystick1_ButtonPressed(void);

uint32_t Joystick2_ReadX(void);
uint32_t Joystick2_ReadY(void);
uint8_t  Joystick2_ButtonPressed(void);

#endif