// Lab9HMain.cpp
// Runs on MSPM0G3507
// Lab 9 ECE319H
// Your name
// Last Modified: 12/26/2024

#include <stdio.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "../inc/ST7735.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/TExaS.h"
#include "../inc/Timer.h"
#include "../inc/SlidePot.h"
#include "../inc/DAC5.h"
#include "Joystick.h"
#include "SmallFont.h"
#include "LED.h"
#include "Switch.h"
#include "Sound.h"
#include "images/images.h"
#include "Map.h"
#include "Player.h"
#include "Switch.h"
#include "Game_ADC.h"
#include "JoyStick.h"
extern "C" void __disable_irq(void);
extern "C" void __enable_irq(void);
extern "C" void TIMG12_IRQHandler(void);
// ****note to ECE319K students****
// the data sheet says the ADC does not work when clock is 80 MHz
// however, the ADC seems to work on my boards at 80 MHz
// I suggest you try 80MHz, but if it doesn't work, switch to 40MHz
void PLL_Init(void){ // set phase lock loop (PLL)
  // Clock_Init40MHz(); // run this line for 40MHz
  Clock_Init80MHz(0);   // run this line for 80MHz
}

uint32_t M=1;
uint32_t Random32(void){
  M = 1664525*M+1013904223;
  return M;
}
uint32_t Random(uint32_t n){
  return (Random32()>>16)%n;
}

SlidePot Sensor(1500,0); // copy calibration from Lab 7
Player player1(2,17, false,1);  // Runner
Player player2(14, 2, true,2); // Chaser


// games  engine runs at 30Hz
void TIMG12_IRQHandler(void){uint32_t pos,msg;
  if((TIMG12->CPU_INT.IIDX) == 1){ // this will acknowledge
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
// game engine goes here
    // 1) sample slide pot
    // 2) read input switches
    // 3) move sprites
    // 4) start sounds
    // 5) set semaphore
    // NO LCD OUTPUT IN INTERRUPT SERVICE ROUTINES
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)

    player1.tickPowerups();
    player2.tickPowerups();

    if (gameTicks > 0) gameTicks--;
    if (gameTicks == 0) roundOver = true;
  }
}

uint8_t TExaS_LaunchPadLogicPB27PB26(void){
  return (0x80|((GPIOB->DOUT31_0>>26)&0x03));
}

typedef enum {English, Spanish, Portuguese, French} Language_t;
Language_t myLanguage=English;
typedef enum {HELLO, GOODBYE, LANGUAGE} phrase_t;
const char Hello_English[] ="Hello";
const char Hello_Spanish[] ="\xADHola!";
const char Hello_Portuguese[] = "Ol\xA0";
const char Hello_French[] ="All\x83";
const char Goodbye_English[]="Goodbye";
const char Goodbye_Spanish[]="Adi\xA2s";
const char Goodbye_Portuguese[] = "Tchau";
const char Goodbye_French[] = "Au revoir";
const char Language_English[]="English";
const char Language_Spanish[]="Espa\xA4ol";
const char Language_Portuguese[]="Portugu\x88s";
const char Language_French[]="Fran\x87" "ais";
const char *Phrases[3][4]={
  {Hello_English,Hello_Spanish,Hello_Portuguese,Hello_French},
  {Goodbye_English,Goodbye_Spanish,Goodbye_Portuguese,Goodbye_French},
  {Language_English,Language_Spanish,Language_Portuguese,Language_French}
};
// use main1 to observe special characters
int main1(void){ // main1
    char l;
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
  ST7735_FillScreen(0x0000);            // set screen to black
  for(int myPhrase=0; myPhrase<= 2; myPhrase++){
    for(int myL=0; myL<= 3; myL++){
         ST7735_OutString((char *)Phrases[LANGUAGE][myL]);
      ST7735_OutChar(' ');
         ST7735_OutString((char *)Phrases[myPhrase][myL]);
      ST7735_OutChar(13);
    }
  }
  Clock_Delay1ms(3000);
  ST7735_FillScreen(0x0000);       // set screen to black
  l = 128;
  while(1){
    Clock_Delay1ms(2000);
    for(int j=0; j < 3; j++){
      for(int i=0;i<16;i++){
        ST7735_SetCursor(7*j+0,i);
        ST7735_OutUDec(l);
        ST7735_OutChar(' ');
        ST7735_OutChar(' ');
        ST7735_SetCursor(7*j+4,i);
        ST7735_OutChar(l);
        l++;
      }
    }
  }
}

int mainmenu(void) {

  DrawMenu();

  while (1) {
    JoystickDirection d1 = GetDiscreteJoystickDirection(Joystick1_ReadY(), Joystick1_ReadX());
    JoystickDirection d2 = GetDiscreteJoystickDirection(Joystick2_ReadX(), Joystick2_ReadY());


    bool selected =
      Switch_P1B1() || Switch_P1B2() || Switch_P2B1() || Switch_P2B2();

    NavigateMenu(d1.dy);
    Clock_Delay1ms(150); // debounce

    if (selected) {
      SelectMenuItem();
      Clock_Delay1ms(500); // delay to avoid rapid re-entry

      // Wait for rules screen if selected
      if (currentSelection == MENU_RULES) {
        while (1) {
          bool next = Switch_P1B1() || Switch_P1B2() || Switch_P2B1() || Switch_P2B2();
          if (next) {
            NextRulesPage();
            Clock_Delay1ms(300);
          }
          if (currentRulesPage == 0) break;
        }
      }

      if (currentSelection == MENU_PLAY) {
        return 0; // Exit menu and allow main game to run
      }
    }
  }
}




int main(void) {
  __disable_irq();
  PLL_Init(); 
  TimerG12_Init();
  LaunchPad_Init();
  ST7735_InitPrintf();
  Switch_Init();
  Joystick_Init();
  Sound_Init();
  TimerG12_IntArm(2666666, 0); 
  __enable_irq();

  while (1) {
    // Start at the main menu
    mainmenu();  // <- Waits for player to select "Play"

    // Reset state before game begins
    player1.reset(); 
    player2.reset();
    player1.addScore(-player1.getScore());  // clear score
    player2.addScore(-player2.getScore());
    roundOver = false;
    gameTicks = 1350;

    DrawMap();
    player1.draw();
    player2.draw();
    DrawScoreBoard();

    // Main game loop
    while (!returnToMenu) {
      JoystickDirection dir1 = GetDiscreteJoystickDirection(Joystick1_ReadY(), Joystick1_ReadX());
      JoystickDirection dir2 = GetDiscreteJoystickDirection(Joystick2_ReadX(), Joystick2_ReadY());

      if (dir1.dx || dir1.dy) {
        player1.erase();
        player1.move(dir1.dx, dir1.dy);
        player1.draw();
      }

      if (dir2.dx || dir2.dy) {
        player2.erase();
        player2.move(dir2.dx, dir2.dy);
        player2.draw();
      }

      if (roundOver || player1.getCoins() >= 5 || player2.getCoins() >= 5) {
        if (player1.isChaser()) {
          if (player2.getCoins() >= 5) player2.addScore(2);
          else if (roundOver) player2.addScore(1);
        } else {
          if (player1.getCoins() >= 5) player1.addScore(2);
          else if (roundOver) player1.addScore(1);
        }
        EndRound();
        continue;
      }

      if (player1.getX() == player2.getX() && player1.getY() == player2.getY()) {
        if (player1.isChaser()) player1.addScore(1);
        else player2.addScore(1);
        Sound_Killed();
        Clock_Delay1ms(300);
        EndRound();
        continue;
      }

      if (Switch_P1B1()) player1.collectItem();
      if (Switch_P1B2()) player1.usePowerup();
      if (Switch_P2B1()) player2.collectItem();
      if (Switch_P2B2()) player2.usePowerup();

      Clock_Delay1ms(200);
    }

    // After EndGame() is triggered and returnToMenu == true
    returnToMenu = false; // reset before next loop iteration
  }
}





// use main2 to observe graphics
int main2(void){ // main2
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
    //note: if you colors are weird, see different options for
    // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
  //DrawMap();
  bool switchpressed = false;
  DrawMap();
  while(!switchpressed){}
  DrawRules();
  Clock_Delay1ms(1000);
  NextRulesPage();
  Clock_Delay1ms(1000);
  NextRulesPage();

  
  //player1.move(1, 0); // move right
  player2.addScore(2);

  DrawScores();
  // ST7735_FillScreen(ST7735_BLACK);
  //ST7735_DrawBitmap(22, 159, PlayerShip0, 18,8); // player ship bottom
  // ST7735_DrawBitmap(53, 151, Bunker0, 18,5);
  // ST7735_DrawBitmap(42, 159, PlayerShip1, 18,8); // player ship bottom
  // ST7735_DrawBitmap(62, 159, PlayerShip2, 18,8); // player ship bottom
  // ST7735_DrawBitmap(82, 159, PlayerShip3, 18,8); // player ship bottom
  // ST7735_DrawBitmap(0, 9, SmallEnemy10pointA, 16,10);
  // ST7735_DrawBitmap(20,9, SmallEnemy10pointB, 16,10);
  // ST7735_DrawBitmap(40, 9, SmallEnemy20pointA, 16,10);
  // ST7735_DrawBitmap(60, 9, SmallEnemy20pointB, 16,10);
  // ST7735_DrawBitmap(80, 9, SmallEnemy30pointA, 16,10);

  // for(uint32_t t=500;t>0;t=t-5){
  //   SmallFont_OutVertical(t,104,6); // top left
  //   Clock_Delay1ms(50);              // delay 50 msec
  // }
  // ST7735_FillScreen(0x0000);   // set screen to black
  // ST7735_SetCursor(1, 1);
  // ST7735_OutString((char *)"GAME OVER");
  // ST7735_SetCursor(1, 2);
  // ST7735_OutString((char *)"Nice try,");
  // ST7735_SetCursor(1, 3);
  // ST7735_OutString((char *)"Earthling!");
  // ST7735_SetCursor(2, 4);
  // ST7735_OutUDec(1234);
  while(1){
  }
}

// use main3 to test switches and LEDs
int main3(void){ // main3
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  Switch_Init(); // initialize switches
  LED_Init(); // initialize LED
  while(1){
    // write code to test switches and LEDs

  }
}
// use main4 to test sound outputs
int main4(void){ uint32_t last=0,now;
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  Switch_Init(); // initialize switches
  LED_Init(); // initialize LED
  Sound_Init();  // initialize sound
  TExaS_Init(ADC0,6,0); // ADC1 channel 6 is PB20, TExaS scope
  __enable_irq();
  while(1){
    now = Switch_In(); // one of your buttons
    if((last == 0)&&(now == 1)){
      Sound_Shoot(); // call one of your sounds
    }
    if((last == 0)&&(now == 2)){
      Sound_Killed(); // call one of your sounds
    }
    if((last == 0)&&(now == 4)){
      //Sound_Explosion(); // call one of your sounds
    }
    if((last == 0)&&(now == 8)){
      //Sound_Fastinvader1(); // call one of your sounds
    }
    // modify this to test all your sounds
  }
}
// ALL ST7735 OUTPUT MUST OCCUR IN MAIN
int main5(void){ // final main
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
    //note: if you colors are weird, see different options for
    // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
  ST7735_FillScreen(ST7735_BLACK);
  Sensor.Init(); // PB18 = ADC1 channel 5, slidepot
  Switch_Init(); // initialize switches
  LED_Init();    // initialize LED
  Sound_Init();  // initialize sound
  TExaS_Init(0,0,&TExaS_LaunchPadLogicPB27PB26); // PB27 and PB26
    // initialize interrupts on TimerG12 at 30 Hz
  
  // initialize all data structures
  __enable_irq();

  while(1){
    // wait for semaphore
       // clear semaphore
       // update ST7735R
    // check for end game or level switch
  }
}