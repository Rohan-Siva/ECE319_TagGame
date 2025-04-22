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

#include <cstdlib>   // for rand()
#include <ctime> 

extern "C" void __disable_irq(void);
extern "C" void __enable_irq(void);
extern "C" void TIMG12_IRQHandler(void);
extern Player player1;
extern Player player2;

volatile int gameTicks = 1350; // 30s * 30Hz
volatile bool roundOver = false;
volatile bool returnToMenu = false;



// Map definition (0 = floor, 1 = wall, 2 = coin)
uint8_t basemap[GRID_HEIGHT][GRID_WIDTH] = {
  {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
  {2,0,1,0,0,0,0,0,0,0,0,0,0,0,1,2},
  {2,0,1,0,1,1,1,0,1,1,1,0,1,0,1,2},
  {2,0,1,0,0,0,1,0,0,0,1,0,0,0,1,2},
  {2,0,1,1,1,0,1,1,1,0,1,1,1,0,1,2},
  {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
  {2,1,1,1,0,1,1,1,0,1,1,1,0,1,1,2},
  {2,0,0,1,0,0,0,1,0,0,0,1,0,0,0,2},
  {2,0,1,1,1,1,0,1,1,1,0,1,1,1,0,2},
  {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
  {2,1,0,1,1,1,1,1,1,1,1,1,1,1,0,2},
  {2,1,0,0,0,0,0,4,0,0,0,0,0,0,0,2},
  {2,1,1,1,1,1,0,1,1,1,1,1,1,1,0,2},
  {2,0,0,0,0,0,0,0,0,0,0,0,5,0,0,2},
  {2,0,1,1,1,1,1,1,1,1,1,1,1,1,1,2},
  {2,0,0,0,0,0,0,6,0,0,0,0,0,0,0,2},
  {2,1,1,0,1,1,1,1,1,1,1,1,1,1,0,2},
  {2,0,0,0,0,0,0,0,0,0,0,3,0,0,0,2},
  {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};
uint8_t map[GRID_HEIGHT][GRID_WIDTH] = {
  {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
  {2,0,1,0,0,0,0,0,0,0,0,0,0,0,1,2},
  {2,0,1,0,1,1,1,0,1,1,1,0,1,0,1,2},
  {2,0,1,0,0,0,1,0,0,0,1,0,0,0,1,2},
  {2,0,1,1,1,0,1,1,1,0,1,1,1,0,1,2},
  {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
  {2,1,1,1,0,1,1,1,0,1,1,1,0,1,1,2},
  {2,0,0,1,0,0,0,1,0,0,0,1,0,0,0,2},
  {2,0,1,1,1,1,0,1,1,1,0,1,1,1,0,2},
  {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
  {2,1,0,1,1,1,1,1,1,1,1,1,1,1,0,2},
  {2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
  {2,1,1,1,1,1,0,1,1,1,1,1,1,1,0,2},
  {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
  {2,0,1,1,1,1,1,1,1,1,1,1,1,1,1,2},
  {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
  {2,1,1,0,1,1,1,1,1,1,1,1,1,1,0,2},
  {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
  {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};



// theres an extra layer that doesnt show up down here

void ResetMap(void) {
  for (int row = 0; row < GRID_HEIGHT; row++) {
    for (int col = 0; col < GRID_WIDTH; col++) {
      map[row][col] = basemap[row][col];
    }
  }
}


void DrawMap(void) {
  ST7735_FillScreen(0x0000);
  ResetMap();
  placeRandomCoins(map, 7);
  placePowerups(map);
  for (int row = 0; row < GRID_HEIGHT; row++) {
    for (int col = 0; col < GRID_WIDTH; col++) {
      int x = col * TILE_SIZE;
      int y = row * TILE_SIZE;
      switch (map[row][col]) {
        case 1:
          ST7735_DrawBitmap(x, y, WallSprite, TILE_SIZE, TILE_SIZE);
          break;
        case 2:
          ST7735_DrawBitmap(x, y, BorderSprite, TILE_SIZE, TILE_SIZE);
          break;
        case 3:
          ST7735_DrawBitmap(x, y, CoinSprite, TILE_SIZE, TILE_SIZE);
          break;
        case 4:
          ST7735_DrawBitmap(x, y, SpeedSprite, TILE_SIZE, TILE_SIZE);
          break;
        case 5:
          ST7735_DrawBitmap(x, y, MineSprite, TILE_SIZE, TILE_SIZE);
          break;
        case 6:
          ST7735_DrawBitmap(x, y, GhostSprite, TILE_SIZE, TILE_SIZE);
          break;

        // case 0: skip drawing
      }
    }
  }
}


// MENU STUFF
uint8_t currentSelection = MENU_PLAY;
uint8_t language = 0; // 0 = English, 1 = Español

void DrawMenu(void) {
  ST7735_FillScreen(0x0000); // black background
  ST7735_DrawBitmap(0, 160, mainScreen, 128, 160);

  // Draw "TAG" logo
  // Option 1: As text
  // ST7735_SetTextColor(0x07FF); // Cyan
  // ST7735_SetCursor(6, 1);
  // ST7735_OutString((char *)"== TAG ==");

  // Option 2: Draw sprite instead (if you create one)
  //DrawLogo(28, 6);

  //ST7735_DrawBitmap(50, 80, TAGSprite, 64, 64);


  // Reset color for menu
  ST7735_SetTextColor(0xFFFF); // White

  // Draw buttons
  ST7735_SetCursor(6, 12);
  ST7735_OutString((char *)(currentSelection == MENU_PLAY ? "> Play" : "  Play"));

  ST7735_SetCursor(1, 13);
  ST7735_OutString((char *)(currentSelection == MENU_LANGUAGE ? "> Language: " : "  Language: "));
  ST7735_OutString((char *)(language == 0 ? "English" : "Espa\xA4ol"));

  ST7735_SetCursor(6, 14);
  ST7735_OutString((char *)(currentSelection == MENU_RULES ? "> Rules" : "  Rules"));
}

void NavigateMenu(int direction) {
  if (direction == 1) {
    currentSelection = (currentSelection + 1) % MENU_COUNT;
  } else if (direction == -1) {
    currentSelection = (currentSelection - 1 + MENU_COUNT) % MENU_COUNT;
  }
  else{
    return;
  }
  DrawMenu();
}

void SelectMenuItem(void) {
  switch (currentSelection) {
    case MENU_PLAY:
      DrawMap(); // Implement this function to transition to game
      //StartGame();
      break;
    case MENU_LANGUAGE:
      language = (language + 1) % 2; // Toggle between 0 and 1
      DrawMenu(); // Refresh menu to show new language
      break;
    case MENU_RULES:
      NextRulesPage(); // Implement this function to show rules screen
      break;
  }
}


#define RULES_PAGE_COUNT 3
uint8_t currentRulesPage = 0;

const char* rulesText[RULES_PAGE_COUNT][10] = {
  {
    "TAG-2P Chasing Game",
    "",
    "1 player is Chaser,",
    "other is Runner.",
    "",
    "Collect coins or ",
    "chase!",
    "",
    "",
    "" // 4 extra lines
  },
  {
    "Controls:",
    "",
    " 2 joysticks to move",
    " 2 buttons per",
    " player",
    "",
    "Powerups:",
    "  > Speed Boost",
    "  > Plant mines",
    "  > Ghost" 
  },
  {
    "Scoring:",
    "",
    "+1: Chaser catches",
    "",
    "+2: Runner collects ",
    "    all coins",
    "",
    "+1: Runner survives",
    "",
    "First to 3 wins!" 
  }
};

void DrawRules(void) {
  ST7735_FillScreen(0x0000); // Clear to black
  ST7735_SetTextColor(0xFFFF); // White

  for (int i = 0; i < 10; i++) {
    ST7735_SetCursor(0, i);
    ST7735_OutString((char *)rulesText[currentRulesPage][i]);
  }

  ST7735_SetCursor(0, 11);
  ST7735_SetTextColor(0x07E0); // Green
  ST7735_OutString((char *)"Press Btn -> Next");
}

void NextRulesPage(void) {
  if(currentRulesPage==RULES_PAGE_COUNT){
    DrawMenu();
    currentRulesPage = 0;
    return;
  }
  DrawRules();
  currentRulesPage = (currentRulesPage + 1);
}

void DrawScores(void){
  ST7735_FillScreen(0x0000); // Clear to black
  ST7735_SetCursor(0, 0);
  ST7735_OutString((char*)"P1:");
  ST7735_OutUDec(player1.getScore());
  
  ST7735_SetCursor(10, 0);
  ST7735_OutString((char*)"P2:");
  ST7735_OutUDec(player2.getScore());
}

void DrawPowerUp(PowerupType powerup, bool isP1){
  int y = 160 - TILE_SIZE+4;
  int offset = 0;
  if(!isP1){
    offset+=8*TILE_SIZE;
  }
  int x = 6*TILE_SIZE+offset;
  if (powerup == PowerupType::Speed) {
    ST7735_DrawBitmap(x, y, SpeedSprite, TILE_SIZE, TILE_SIZE);
    return;
  }
  if(powerup == PowerupType::Mine){
    ST7735_DrawBitmap(x, y, MineSprite, TILE_SIZE, TILE_SIZE);
    return;
  }
  if(powerup==PowerupType::Ghost){
    ST7735_DrawBitmap(x, y, GhostSprite, TILE_SIZE, TILE_SIZE);
    return;
  }
  if(powerup==PowerupType::None){
    ST7735_DrawBitmap(x, y, Black, TILE_SIZE, TILE_SIZE);

  }
}

void DrawScoreBoard(void){
  int y = 160 - TILE_SIZE+4;
  int x = 2*TILE_SIZE+4;

  ST7735_SetCursor(0, 15);
  ST7735_OutString((char*)"P1:");
  ST7735_DrawBitmap(x, y, CoinSprite, TILE_SIZE, TILE_SIZE);
  ST7735_SetCursor(5, 15);
  ST7735_OutUDec(0);

  ST7735_SetCursor(10, 15); // 15 represents how far right it is on the screen from joystick view
  ST7735_OutString((char*)"P2:");
  x = 10*TILE_SIZE;
  ST7735_DrawBitmap(x, y, CoinSprite, TILE_SIZE, TILE_SIZE);
  ST7735_SetCursor(15, 15);
  ST7735_OutUDec(0);
}

void UpdateCoin(int id,int coins){
  if(id==1){
    ST7735_SetCursor(5, 15);
    ST7735_OutUDec(coins);
  }
  if(id == 2){
    ST7735_SetCursor(15, 15);
    ST7735_OutUDec(coins);
  }
}

void placeRandomCoins(uint8_t map[GRID_HEIGHT][GRID_WIDTH], int coinCount) {
    srand(time(0));  // Seed the RNG with current time
    
    int placed = 0;
    while (placed < coinCount) {
        int row = rand() % (GRID_HEIGHT - 1); // exclude last row
        int col = rand() % GRID_WIDTH;

        if (map[row][col] == 0) {
            map[row][col] = 3;  // Place coin
            placed++;
        }
    }
}

void placePowerups(uint8_t map[GRID_HEIGHT][GRID_WIDTH]) {
    srand(time(0)); // optional if already seeded elsewhere

    struct PlacePowerup {
        uint8_t value;
        int count;
    };

    PlacePowerup powerups[] = {
        {4, 3}, // Speed
        {5, 3}, // Mine
        {6, 2}  // Ghost
    };

    for (const PlacePowerup& p : powerups) {
        int placed = 0;
        while (placed < p.count) {
            int row = rand() % (GRID_HEIGHT - 1); // exclude last row
            int col = rand() % GRID_WIDTH;

            if (map[row][col] == 0) {
                map[row][col] = p.value;
                placed++;
            }
        }
    }
}

void EndRound() {
  __disable_irq();
  ST7735_FillScreen(ST7735_BLACK);
  ST7735_SetTextColor(0xFFFF);
  ST7735_SetCursor(0, 0);

  ST7735_OutString((char*)"--- Round Over ---");

  // Draw scores
  ST7735_SetCursor(0, 2);
  ST7735_OutString((char*)"P1 Score: ");
  ST7735_OutUDec(player1.getScore());

  ST7735_SetCursor(0, 3);
  ST7735_OutString((char*)"P2 Score: ");
  ST7735_OutUDec(player2.getScore());



  // Handle end game
  if (player1.getScore() >= 3) {
    Clock_Delay1ms(5000);
    EndGame(1);
  } else if (player2.getScore() >= 3) {
    Clock_Delay1ms(5000);
    EndGame(2);
  } else {
      player1.reset();
      player2.reset();
      
      // Show upcoming roles
      ST7735_SetCursor(0, 5);
      ST7735_OutString((char*)"Next Round Roles:");

      ST7735_SetCursor(0, 7);
      ST7735_OutString((char*)"Runner: ");
      if (!player1.isChaser()) {
        ST7735_OutString((char*)"P1");
        ST7735_DrawBitmap(70, 77, P1Sprite, TILE_SIZE, TILE_SIZE);
      } else {
        ST7735_OutString((char*)"P2");
        ST7735_DrawBitmap(70, 77, P2Sprite, TILE_SIZE, TILE_SIZE);
      }

      ST7735_SetCursor(0, 9);
      ST7735_OutString((char*)"Chaser: ");
      if (player1.isChaser()) {
        ST7735_OutString((char*)"P1");
        ST7735_DrawBitmap(70, 97, P1Sprite, TILE_SIZE, TILE_SIZE);
      } else {
        ST7735_OutString((char*)"P2");
        ST7735_DrawBitmap(70, 97, P2Sprite, TILE_SIZE, TILE_SIZE);
      }
      ST7735_SetCursor(0, 13);
      ST7735_OutString((char*)"Switching Roles...");
      Clock_Delay1ms(7000);

      DrawMap();
      player1.draw();
      player2.draw();
      DrawScoreBoard();
      gameTicks = 1350;
      roundOver = false;
  }

  // Reset state for next round

  __enable_irq();
}



void EndGame(uint8_t winnerID) {
  __disable_irq();
  ST7735_FillScreen(ST7735_BLACK);
  ST7735_SetTextColor(0xFFFF);
  ST7735_SetCursor(2, 3);
  ST7735_OutString((char *)"*** GAME OVER ***");

  ST7735_SetCursor(2, 5);
  if (winnerID == 1) {
    ST7735_OutString((char *)"Player 1 Wins!");
  } else {
    ST7735_OutString((char *)"Player 2 Wins!");
  }

  ST7735_SetCursor(2, 7);
  ST7735_OutString((char *)"Press any btn...");

  while (!(Switch_P1B1() || Switch_P1B2() || Switch_P2B1() || Switch_P2B1())); // wait for a button press
  Clock_Delay1ms(1000);
  // go to menu page feature here
  returnToMenu = true;

}
