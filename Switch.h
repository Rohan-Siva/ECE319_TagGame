/*
 * Switch.h
 *
 *  Created on: Nov 5, 2023
 *      Author: jonat
 */

#ifndef SWITCH_H_
#define SWITCH_H_

#include <stdint.h>  // Make sure to include this for uint32_t and uint8_t

// Initialize your switches
void Switch_Init(void);

// Return current state of switches (bits 0–3 = PA25–PA28)
uint32_t Switch_In(void);

// Check if specific menu switches are pressed (active-low)
uint8_t Switch_MenuDownPressed(void);     // PA25
uint8_t Switch_MenuSelectPressed(void);   // PA26

#endif /* SWITCH_H_ */