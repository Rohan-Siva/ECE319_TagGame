#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "Sound.h"
#include "sounds/sounds.h"
#include "../inc/DAC5.h"
#include "../inc/Timer.h"


#define PB0INDEX  11
#define PB1INDEX  12
#define PB2INDEX  14
#define PB3INDEX  15
#define PB4INDEX  16

// Global variables for sound playback
static const uint8_t* SoundData;
static uint32_t SoundIndex;
static uint32_t SoundLength;

// SysTick period for 11kHz
#define SYSTICK_FREQ 11000
#define SYSTICK_PERIOD (80000000 / SYSTICK_FREQ) // Assuming 80MHz clock



void SysTick_IntArm(uint32_t period, uint32_t priority){
  SysTick->CTRL = 0;         // disable SysTick
  SysTick->LOAD = period - 1;// reload value
  SysTick->VAL = 0;          // any write to VAL clears current
  SCB->SHP[1] = (SCB->SHP[1] & ~0xC0000000) | (priority << 30); // priority 0 to 3
  SysTick->CTRL = 0x0007;    // enable with core clock and interrupts
}

void Sound_Init(void){
  // Setup GPIO for DAC output
  IOMUX->SECCFG.PINCM[PB0INDEX] = 0x00000081;
  IOMUX->SECCFG.PINCM[PB1INDEX] = 0x00000081;
  IOMUX->SECCFG.PINCM[PB2INDEX] = 0x00000081;
  IOMUX->SECCFG.PINCM[PB3INDEX] = 0x00000081;
  IOMUX->SECCFG.PINCM[PB4INDEX] = 0x00000081;
  GPIOB->DOE31_0 |= 0x01F;

  DAC5_Out(0); // initialize DAC output
  SoundIndex = 0;
  SoundLength = 0;
  SoundData = 0;
  SysTick->CTRL = 0; // disable SysTick initially
}

extern "C" void SysTick_Handler(void){
  if (SoundIndex < SoundLength) {
    DAC5_Out(SoundData[SoundIndex++]);
  } else {
    Sound_Stop();
  }
}

void Sound_Start(const uint8_t *pt, uint32_t count){
  if (pt == 0 || count == 0) return;
  SoundData = pt;
  SoundLength = count;
  SoundIndex = 0;

  SysTick_IntArm(SYSTICK_PERIOD, 0); // priority 0
}

void Sound_Shoot(void){
  Sound_Start(shoot, 4080);
}
void Sound_Killed(void){
  Sound_Start(invaderkilled, 3377); // Replace with actual length
}
void Sound_Collect(void){
  Sound_Start(collect, 10424); // Replace with actual length
}
void Sound_GameOver(void){
  Sound_Start(shoot, 4080); // Replace with actual length
}

void Sound_Explosion(void){
  Sound_Start(explosion, 2000);
}

void Sound_Stop(void){
  SysTick->CTRL = 0; // disable SysTick
}
