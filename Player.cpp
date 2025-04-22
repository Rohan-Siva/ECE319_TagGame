#include "Map.h"
#include "../inc/ST7735.h"
#include "Player.h"
#include "Sound.h"
#include "images/images.h"

extern const uint16_t RunnerSprite[64];
extern const uint16_t ChaserSprite[64];
extern const uint16_t WallSprite[64];
extern const uint16_t BorderSprite[64];
extern const uint16_t CoinSprite[64];
extern const uint16_t SpeedSprite[64];
extern const uint16_t Black[64];
extern uint8_t map[GRID_HEIGHT][GRID_WIDTH];

Player::Player(uint8_t startX, uint8_t startY, bool isChaser, uint8_t id) {
  x = startX;
  y = startY;
  score = 0;
  powerup = PowerupType::None;
  chaser = isChaser;
  playerID = id;
  coins = 0;
  hasSpeed = false;
  hasGhost = false;
  speedTimer = 0; 
  ghostTimer = 0;

}

uint8_t Player::getID() const {
  return playerID;
}

uint8_t Player::getCoins() const {
  return coins;
}

void Player::move(int8_t dx, int8_t dy) {
  if(hasGhost && map[y+dy][x+dx]!=2){
    x += dx;
    y += dy;
    return;
  }
  if(map[y+dy][x+dx]!=1 && map[y+dy][x+dx]!=2){
    if(hasSpeed && map[y+2*dy][x+2*dx]!=2 && map[y+2*dy][x+2*dx]!=1){
      x+=2*dx;
      y+=2*dy;
      return;
    }
    x += dx;
    y += dy;
    return;
  }
  }


void Player::collectItem(){
  if(map[y][x]==3 && !isChaser()){
    coins+=1;
    map[y][x]=0;
    UpdateCoin(playerID,coins);
    ST7735_DrawBitmap(x*TILE_SIZE, y*TILE_SIZE, Black, TILE_SIZE, TILE_SIZE);
    draw();
    Sound_Collect();
    return;
  }
  if(getPowerup()!=PowerupType::None){ // already has powerup
    //play error sound
    return;
  }

  if(map[y][x]==4){
    setPowerup(PowerupType::Speed);
  }
  else if(map[y][x]==5 && isChaser()){
    setPowerup(PowerupType::Mine);
  }
  else if(map[y][x]==6){
    setPowerup(PowerupType::Ghost);
  }
  map[y][x]=0;
  ST7735_DrawBitmap(x*TILE_SIZE, y*TILE_SIZE, Black, TILE_SIZE, TILE_SIZE);
  draw();
}

void Player::addScore(uint8_t points) {
  score += points;
}

void Player::setPowerup(PowerupType p) {
  DrawPowerUp(p, (playerID==1));
  powerup = p;
}

void Player::usePowerup(){
  if(getPowerup()==PowerupType::None){
    return;
  }
  if(getPowerup()==PowerupType::Mine && isChaser()) ST7735_DrawBitmap(x*TILE_SIZE, y*TILE_SIZE, MineSprite, TILE_SIZE, TILE_SIZE);
  if(getPowerup()==PowerupType::Speed) {
    hasSpeed = true;
    hasGhost = false;
    speedTimer = 90; // 3 seconds since we use timerg12 30 hz
  }
  if(getPowerup()==PowerupType::Ghost) {
    hasGhost = true;
    hasSpeed = false; // only one power on at a time
    ghostTimer = 90;
  }

  setPowerup(PowerupType::None);
}

void Player::reset() {
  coins = 0;
  powerup = PowerupType::None;
  hasSpeed = false;
  hasGhost = false;
  speedTimer = 0;
  ghostTimer = 0;
  chaser = !chaser; // swap role


  if (chaser) {
    setPosition(14, 2);  // chaser start position
  } else {
    setPosition(2, 17);  // runner start position
  }
}

void Player::setPosition(uint8_t newX, uint8_t newY) {
  x = newX;
  y = newY;
}


void Player::tickPowerups() {
  if (hasSpeed && speedTimer > 0) {
    speedTimer--;
    if (speedTimer == 0) {
      hasSpeed = false;
    }
  }
  if (hasGhost && ghostTimer > 0) {
    ghostTimer--;
    if (ghostTimer == 0) {
      hasGhost = false;
    }
  }
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

  if (getID()==1) {
    ST7735_DrawBitmap(screenX, screenY, P1Sprite, TILE_SIZE, TILE_SIZE);
  } else {
    ST7735_DrawBitmap(screenX, screenY, P2Sprite, TILE_SIZE, TILE_SIZE);
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
    case 5:
      ST7735_DrawBitmap(screenX, screenY, MineSprite, TILE_SIZE, TILE_SIZE);
      break;
    case 6:
      ST7735_DrawBitmap(screenX, screenY, GhostSprite, TILE_SIZE, TILE_SIZE);
      break;
    default:
      ST7735_DrawBitmap(screenX, screenY, Black, TILE_SIZE, TILE_SIZE);
      break;
  }
}