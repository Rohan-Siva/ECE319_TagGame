#include "Map.h"
#include "../inc/ST7735.h"
#include "Player.h"
#include "images/images.h"

extern const uint16_t RunnerSprite[64];
extern const uint16_t ChaserSprite[64];
extern const uint16_t WallSprite[64];
extern const uint16_t BorderSprite[64];
extern const uint16_t CoinSprite[64];
extern const uint16_t SpeedSprite[64];
extern const uint16_t Black[64];
extern uint8_t map[GRID_HEIGHT][GRID_WIDTH];

Player::Player(uint8_t startX, uint8_t startY, bool isChaser) {
  x = startX;
  y = startY;
  score = 0;
  powerup = PowerupType::None;
  chaser = isChaser;
}


void Player::move(int8_t dx, int8_t dy) {
  if(map[y+dy][x+dx]!=1 && map[y+dy][x+dx]!=2){
    x += dx;
    y += dy;
  }
}

void Player::addScore(uint8_t points) {
  score += points;
}

void Player::setPowerup(PowerupType p) {
  powerup = p;
}

void Player::reset() {
  score = 0;
  powerup = PowerupType::None;
}

uint8_t Player::getX() const { 
    return x; 
}
uint8_t Player::getY() const { 
    return y; 
}
uint8_t Player::getScore() const { 
    return score; 
}
PowerupType Player::getPowerup() const 
{ 
    return powerup; 
}
bool Player::isChaser() const { 
    return chaser; 

}


void Player::draw() const {
  int screenX = x * TILE_SIZE;
  int screenY = y * TILE_SIZE;

  if (chaser) {
    ST7735_DrawBitmap(screenX, screenY, ChaserSprite, TILE_SIZE, TILE_SIZE);
  } else {
    ST7735_DrawBitmap(screenX, screenY, RunnerSprite, TILE_SIZE, TILE_SIZE);
  }
}

void Player::erase() const {
  int screenX = x * TILE_SIZE;
  int screenY = y * TILE_SIZE;

  switch (map[y][x]) {
    case 1:
      ST7735_DrawBitmap(screenX, screenY, WallSprite, TILE_SIZE, TILE_SIZE);
      break;
    case 2:
      ST7735_DrawBitmap(screenX, screenY, BorderSprite, TILE_SIZE, TILE_SIZE);
      break;
    case 3:
      ST7735_DrawBitmap(screenX, screenY, CoinSprite, TILE_SIZE, TILE_SIZE);
      break;
    case 4:
      ST7735_DrawBitmap(screenX, screenY, SpeedSprite, TILE_SIZE, TILE_SIZE);
      break;
    default:
      ST7735_DrawBitmap(screenX, screenY, Black, TILE_SIZE, TILE_SIZE);
      break;
  }
}