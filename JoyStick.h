#ifndef __OURJOYSTICK_H__
#define __OURJOYSTICK_H__

#include <stdint.h>

void Joystick_Init(void);

uint16_t Joystick1_ReadX(void);
uint16_t Joystick1_ReadY(void);
bool Joystick1_ButtonPressed(void);

uint16_t Joystick2_ReadX(void);
uint16_t Joystick2_ReadY(void);
bool Joystick2_ButtonPressed(void);

typedef struct 
{
    int8_t dx;
    int8_t dy;
} JoystickDirection;

JoystickDirection GetDiscreteJoystickDirection(uint32_t adcX, uint32_t adcY);

#endif