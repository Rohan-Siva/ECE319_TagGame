#include <ti/devices/msp/msp.h>
#include "../inc/ADC.h"
#include "../inc/Clock.h"

// Initialize ADC0
void MyADC0_Init(uint32_t channel, uint32_t reference){
  ADC0->ULLMEM.GPRCM.RSTCTL = 0xB1000003;
  if(reference == ADCVREF_INT) VREF->GPRCM.RSTCTL = 0xB1000003;

  ADC0->ULLMEM.GPRCM.PWREN = 0x26000001;
  if(reference == ADCVREF_INT) VREF->GPRCM.PWREN = 0x26000001;

  Clock_Delay(24);
  ADC0->ULLMEM.GPRCM.CLKCFG = 0xA9000000;
  ADC0->ULLMEM.CLKFREQ = 7;
  ADC0->ULLMEM.CTL0 = 0x03010000;
  ADC0->ULLMEM.CTL1 = 0x00000000;
  ADC0->ULLMEM.CTL2 = 0x00000000;
  ADC0->ULLMEM.MEMCTL[0] = reference + channel;
  ADC0->ULLMEM.SCOMP0 = 0;

  if(reference == ADCVREF_INT){
    VREF->CLKSEL = 0x00000008;
    VREF->CLKDIV = 0;
    VREF->CTL0 = 0x0001;
    VREF->CTL2 = 0;
    while((VREF->CTL1 & 0x01) == 0){}
  }
}

// Read from ADC0
uint32_t MyADC0_In(void){
  ADC0->ULLMEM.CTL0 |= 0x00000001;
  ADC0->ULLMEM.CTL1 |= 0x00000100;
  while((ADC0->ULLMEM.STATUS & 0x01) == 0x01){}
  return ADC0->ULLMEM.MEMRES[0];
}

// Initialize ADC1
void MyADC1_Init(uint32_t channel, uint32_t reference){
  ADC1->ULLMEM.GPRCM.RSTCTL = 0xB1000003;
  if(reference == ADCVREF_INT) VREF->GPRCM.RSTCTL = 0xB1000003;

  ADC1->ULLMEM.GPRCM.PWREN = 0x26000001;
  if(reference == ADCVREF_INT) VREF->GPRCM.PWREN = 0x26000001;

  Clock_Delay(24);
  ADC1->ULLMEM.GPRCM.CLKCFG = 0xA9000000;
  ADC1->ULLMEM.CLKFREQ = 7;
  ADC1->ULLMEM.CTL0 = 0x03010000;
  ADC1->ULLMEM.CTL1 = 0x00000000;
  ADC1->ULLMEM.CTL2 = 0x00000000;
  ADC1->ULLMEM.MEMCTL[0] = reference + channel;
  ADC1->ULLMEM.SCOMP0 = 0;
  ADC1->ULLMEM.GEN_EVENT.IMASK = 0;

  if(reference == ADCVREF_INT){
    VREF->CLKSEL = 0x00000008;
    VREF->CLKDIV = 0;
    VREF->CTL0 = 0x0001;
    VREF->CTL2 = 0;
    while((VREF->CTL1 & 0x01) == 0){}
  }
}

// Read from ADC1
uint32_t MyADC1_In(uint8_t memIndex){
  ADC1->ULLMEM.CTL0 |= 0x00000001;
  ADC1->ULLMEM.CTL1 |= 0x00000100;
  while((ADC1->ULLMEM.STATUS & 0x01) == 0x01){}
  return ADC1->ULLMEM.MEMRES[memIndex];
}