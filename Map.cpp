#include "Map.h"
#include "../inc/ST7735.h"

// 8x8 solid wall tile
const uint16_t WallSprite[64] = {
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000,
  0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000,
  0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000,
  0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000,
  0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000,
  0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};

const uint16_t BorderSprite[64] = {
  0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF,
  0x7BEF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x7BEF,
  0x7BEF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x7BEF,
  0x7BEF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x7BEF,
  0x7BEF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x7BEF,
  0x7BEF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x7BEF,
  0x7BEF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x7BEF,
  0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF, 0x7BEF
};



// 8x8 coin sprite
const uint16_t CoinSprite[64] = {
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0xFD20, 0xFD20, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0xFD20, 0xFD20, 0xFD20, 0xFD20, 0x0000, 0x0000,
  0x0000, 0xFD20, 0xFD20, 0xFFFF, 0xFFFF, 0xFD20, 0xFD20, 0x0000,
  0x0000, 0xFD20, 0xFD20, 0xFFFF, 0xFFFF, 0xFD20, 0xFD20, 0x0000,
  0x0000, 0x0000, 0xFD20, 0xFD20, 0xFD20, 0xFD20, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0xFD20, 0xFD20, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};

const uint16_t SpeedSprite[64] = {
  0x0000, 0x07E0, 0x0000, 0x07E0, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x07E0, 0x07E0, 0x0000, 0x07E0, 0x07E0, 0x0000, 0x0000,
  0x07E0, 0x07E0, 0x0000, 0x0000, 0x07E0, 0x07E0, 0x0000, 0x0000,
  0x0000, 0x07E0, 0x07E0, 0x0000, 0x07E0, 0x07E0, 0x0000, 0x0000,
  0x0000, 0x07E0, 0x07E0, 0x0000, 0x07E0, 0x07E0, 0x0000, 0x0000,
  0x07E0, 0x07E0, 0x0000, 0x0000, 0x07E0, 0x07E0, 0x0000, 0x0000,
  0x0000, 0x07E0, 0x07E0, 0x0000, 0x07E0, 0x07E0, 0x0000, 0x0000,
  0x0000, 0x07E0, 0x0000, 0x07E0, 0x0000, 0x0000, 0x0000, 0x0000
};



// Map definition (0 = floor, 1 = wall, 2 = coin)
uint8_t map[GRID_HEIGHT][GRID_WIDTH] = {
  {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
  {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
  {2,1,0,1,1,1,1,0,1,1,1,1,0,1,0,2},
  {2,1,0,1,4,0,0,1,0,0,0,1,0,0,0,2},
  {2,1,0,1,0,1,0,1,1,1,0,1,1,1,0,2},
  {2,1,0,0,0,1,0,0,0,1,0,0,0,1,0,2},
  {2,1,1,1,0,1,1,1,0,1,1,1,0,1,1,2},
  {2,1,0,0,4,0,0,1,0,0,0,1,0,0,0,2},
  {2,1,0,1,1,1,1,1,1,1,1,1,1,1,0,2},
  {2,1,0,0,0,1,0,0,0,1,0,0,3,1,0,2},
  {2,1,1,1,0,1,1,1,0,1,1,1,0,1,1,2},
  {2,1,0,0,0,0,0,1,0,0,0,1,0,0,0,2},
  {2,1,0,1,1,1,1,1,1,1,1,1,1,1,0,2},
  {2,1,0,0,0,1,4,0,3,1,0,0,0,1,0,2},
  {2,1,1,1,0,1,1,1,0,1,1,1,0,1,1,2},
  {2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
  {2,1,0,1,1,1,1,1,1,1,1,1,1,1,0,2},
  {2,1,0,0,0,0,0,0,0,3,0,0,0,0,0,2},
  {2,0,0,0,0,0,1,1,1,1,0,0,0,0,3,2},
  {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2}
};

// theres an extra layer that doesnt show up down here




void DrawMap(void) {
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
        // case 0: skip drawing
      }
    }
  }
}
// ========== TAG LOGO SPRITES (Tiled 8x8) ==========

const uint16_t TAG_Tile_Empty[64] = {
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};


const uint16_t TAG_Tile_Top[64] = {
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF
};


const uint16_t TAG_Tile_TStem[64] = {
  0x0000,0x0000,0xFFFF,0xFFFF,0x0000,0x0000,0x0000,0x0000,
  0x0000,0x0000,0xFFFF,0xFFFF,0x0000,0x0000,0x0000,0x0000,
  0x0000,0x0000,0xFFFF,0xFFFF,0x0000,0x0000,0x0000,0x0000,
  0x0000,0x0000,0xFFFF,0xFFFF,0x0000,0x0000,0x0000,0x0000,
  0x0000,0x0000,0xFFFF,0xFFFF,0x0000,0x0000,0x0000,0x0000,
  0x0000,0x0000,0xFFFF,0xFFFF,0x0000,0x0000,0x0000,0x0000,
  0x0000,0x0000,0xFFFF,0xFFFF,0x0000,0x0000,0x0000,0x0000,
  0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
};

const uint16_t TAG_Tile_ATop[64] = {
  0x0000,0x0000,0x07E0,0x07E0,0x07E0,0x07E0,0x0000,0x0000,
  0x0000,0x07E0,0x0000,0x0000,0x0000,0x0000,0x07E0,0x0000,
  0x07E0,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x07E0,
  0x07E0,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x07E0,
  0x07E0,0x07E0,0x07E0,0x07E0,0x07E0,0x07E0,0x07E0,0x07E0,
  0x07E0,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x07E0,
  0x07E0,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x07E0,
  0x07E0,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x07E0
};

const uint16_t TAG_Tile_GBottom[64] = {
  0x001F,0x001F,0x001F,0x001F,0x001F,0x001F,0x001F,0x001F,
  0x001F,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x001F,
  0x001F,0x0000,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0x0000,0x001F,
  0x001F,0x0000,0xFFFF,0x0000,0x0000,0xFFFF,0x0000,0x001F,
  0x001F,0x0000,0xFFFF,0x0000,0xFFFF,0xFFFF,0x0000,0x001F,
  0x001F,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x001F,
  0x001F,0x001F,0x001F,0x001F,0x001F,0x001F,0x001F,0x001F,
  0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
};

const uint16_t* TAG_Tiles[] = {
  TAG_Tile_Empty,     // 0
  TAG_Tile_Top,       // 1
  TAG_Tile_TStem,     // 2
  TAG_Tile_ATop,      // 3
  TAG_Tile_GBottom    // 4
};

const uint8_t TAGLogoMap[2][6] = {
  {0, 1, 0, 0, 3, 0},
  {0, 2, 0, 0, 4, 0}
};

void DrawLogo(int x0, int y0) {
  for (int row = 0; row < 2; row++) {
    for (int col = 0; col < 6; col++) {
      uint8_t tileId = TAGLogoMap[row][col];
      const uint16_t* tile = TAG_Tiles[tileId];
      ST7735_DrawBitmap(x0 + col * TILE_SIZE, y0 + row * TILE_SIZE, tile, TILE_SIZE, TILE_SIZE);
    }
  }
}
// MENU STUFF
enum MenuSelection { MENU_PLAY, MENU_LANGUAGE, MENU_RULES };
uint8_t currentSelection = MENU_PLAY;
uint8_t language = 0; // 0 = English, 1 = Español

void DrawMenu(void) {
  ST7735_FillScreen(0x0000); // black background

  // Draw "TAG" logo
  // Option 1: As text
  // ST7735_SetTextColor(0x07FF); // Cyan
  // ST7735_SetCursor(6, 1);
  // ST7735_OutString((char *)"== TAG ==");

  // Option 2: Draw sprite instead (if you create one)
  DrawLogo(28, 6);

  // Reset color for menu
  ST7735_SetTextColor(0xFFFF); // White

  // Draw buttons
  ST7735_SetCursor(6, 4);
  ST7735_OutString((char *)(currentSelection == MENU_PLAY ? "> Play" : "  Play"));

  ST7735_SetCursor(1, 5);
  ST7735_OutString((char *)(currentSelection == MENU_LANGUAGE ? "> Language: " : "  Language: "));
  ST7735_OutString((char *)(language == 0 ? "English" : "Español"));

  ST7735_SetCursor(6, 6);
  ST7735_OutString((char *)(currentSelection == MENU_RULES ? "> Rules" : "  Rules"));
}

void DrawRules(void){
  //wip
}

