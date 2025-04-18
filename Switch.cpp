/*
 * Switch.cpp
 *
 *  Created on: Nov 5, 2023
 *      Author:
 */
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
// LaunchPad.h defines all the indices into the PINCM table
void Switch_Init(void){
    // write this
  IOMUX->SECCFG.PINCM[PA25INDEX] = 0x00040081; // Player 1 Button 1
  IOMUX->SECCFG.PINCM[PA26INDEX] = 0x00040081; // Player 1 Button 2
  IOMUX->SECCFG.PINCM[PA27INDEX] = 0x00040081; // Player 2 Button 1
  IOMUX->SECCFG.PINCM[PA28INDEX] = 0x00040081; // Player 2 Button 2
}

// return current state of switches
uint32_t Switch_In(void) {
  uint32_t allPins = GPIOA->DIN31_0;         // Read all 32 bits from GPIOA
  uint32_t switchBits = (allPins >> 25) & 0x0F; // Grab just bits 25 to 28
  return switchBits;                         // Bits 0–3: PA25, PA26, PA27, PA28
}

// Returns 1 if PA25 (Menu Down) is pressed
uint8_t Switch_MenuDownPressed(void) {
  uint32_t checkval = Switch_In();
  checkval = checkval & (1 << 2);
  return (checkval != 0);    // Bit 0 corresponds to PA25
}

// Returns 1 if PA26 (Menu Select) is pressed
uint8_t Switch_MenuSelectPressed(void) {
  uint32_t checkval = Switch_In();
  return ((checkval & (1 << 3)) != 0);    // Bit 1 corresponds to PA26
}