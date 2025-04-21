// Sound.h
// Runs on MSPM0
// Play sounds on 5-bit DAC
// Author: Your Name
// Date: 11/5/2023

#ifndef SOUND_H
#define SOUND_H
#include <stdint.h>

// Initializes the 5-bit DAC and configures SysTick for 11kHz timing,
// but does not start playing any sound.
void Sound_Init(void);

// Starts playback of a sound sample via the SysTick interrupt.
// This function sets the sound pointer and counter but does not directly output to DAC.
// Sound plays once and stops automatically.
// Inputs:
//   pt: pointer to an array of 5-bit DAC output values
//   count: number of samples in the array
void Sound_Start(const uint8_t *pt, uint32_t count);

// Stops any currently playing sound by disabling SysTick.
void Sound_Stop(void);

// Helper functions that load specific sound assets
// and call Sound_Start with appropriate array and size.
void Sound_Shoot(void);
void Sound_Killed(void);
void Sound_Collect(void);
void Sound_GameOver(void);

// Internal DAC output function - writes 5-bit data to GPIO.
// You may keep this in a separate DAC module if preferred.
void DAC5_Out(uint32_t data);


extern const uint8_t collect[16]; // Example short sound


#endif // SOUND_H
