#ifndef __MAP_H__
#define __MAP_H__

#include <stdint.h>

#define TILE_SIZE 8
#define GRID_WIDTH 16
#define GRID_HEIGHT 20

extern uint8_t map[GRID_HEIGHT][GRID_WIDTH];

extern const uint16_t WallSprite[64];   // 8x8 wall sprite
extern const uint16_t CoinSprite[64];   // 8x8 coin sprite
extern const uint16_t BorderSprite[64];   // 8x8 coin sprite
extern const uint16_t SpeedSprite[64];

void DrawMap(void);
void DrawMenu(void);
void NextRulesPage(void);
void DrawRules(void);

#endif // __MAP_H__
