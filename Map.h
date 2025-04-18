#ifndef __MAP_H__
#define __MAP_H__

#include <stdint.h>

#define TILE_SIZE 8
#define GRID_WIDTH 16
#define GRID_HEIGHT 20

extern const uint16_t WallSprite[64];     // 8x8 wall sprite
extern const uint16_t CoinSprite[64];     // 8x8 coin sprite
extern const uint16_t BorderSprite[64];   // 8x8 border sprite
extern const uint16_t SpeedSprite[64];    // 8x8 speed powerup sprite
extern const uint16_t ChaserSprite[64];   // 8x8 chaser enemy sprite
extern const uint16_t RunnerSprite[64];   // 8x8 runner player sprite
extern const uint16_t Black[64];          // 8x8 black tile sprite

extern uint8_t map[GRID_HEIGHT][GRID_WIDTH];

enum MenuSelection { MENU_PLAY, MENU_LANGUAGE, MENU_RULES };
extern uint8_t currentSelection;
extern uint8_t language;
#define MENU_COUNT 3

#define RULES_PAGE_COUNT 3
extern uint8_t currentRulesPage;

void DrawMap(void);
void DrawMenu(void);
void NavigateMenu(int direction);
void SelectMenuItem(void);
void DrawRules(void);
void NextRulesPage(void);
void DrawScores(void);

#endif // __MAP_H__
