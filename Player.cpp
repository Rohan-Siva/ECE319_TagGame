#include "Player.h"

Player::Player(uint8_t startX, uint8_t startY, bool isChaser)
  : x(startX), y(startY), score(0), powerup(PowerupType::None), chaser(isChaser) {}

void Player::move(int8_t dx, int8_t dy) {
  x += dx;
  y += dy;
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

uint8_t Player::getX() const { return x; }
uint8_t Player::getY() const { return y; }
uint8_t Player::getScore() const { return score; }
PowerupType Player::getPowerup() const { return powerup; }
bool Player::isChaser() const { return chaser; }
