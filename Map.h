#ifndef __MAP_H__
#define __MAP_H__

#include "Player.h"
#include <stdint.h>

#define TILE_SIZE 8
#define GRID_WIDTH 16
#define GRID_HEIGHT 20


extern uint8_t map[GRID_HEIGHT][GRID_WIDTH];
extern uint8_t basemap[GRID_HEIGHT][GRID_WIDTH];


enum MenuSelection { MENU_PLAY, MENU_LANGUAGE, MENU_RULES };
extern uint8_t currentSelection;
extern uint8_t language;
#define MENU_COUNT 3
#define RULES_PAGE_COUNT 3
extern uint8_t currentRulesPage;
extern volatile int gameTicks; // 1 minute at 30Hz
extern volatile bool roundOver;
extern volatile bool returnToMenu;


void ResetMap(void);
void DrawMap(void);
void DrawMenu(void);

void EndRound(void);
void EndGame(uint8_t winnerID);

void NavigateMenu(int direction);
void SelectMenuItem(void);
void DrawRules(void);
void NextRulesPage(void);
void DrawScores(void);
void DrawScoreBoard(void);
void DrawPowerUp(PowerupType powerup, bool isP1);
void UpdateCoin(int id, int coins);
void placePowerups(uint8_t map[GRID_HEIGHT][GRID_WIDTH]);
void placeRandomCoins(uint8_t map[GRID_HEIGHT][GRID_WIDTH], int coinCount);

#endif // __MAP_H__
