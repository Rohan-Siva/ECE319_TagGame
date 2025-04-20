#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <stdint.h>

enum class PowerupType {
  None,
  Speed,
  Mine,
  Ghost
};

class Player {
public:
  Player(uint8_t startX, uint8_t startY, bool isChaser);

  void move(int8_t dx, int8_t dy);
  void addScore(uint8_t points);
  void setPowerup(PowerupType p);
  void reset();

  uint8_t getX() const;
  uint8_t getY() const;
  uint8_t getScore() const;
  void draw() const;
  void erase() const;
  PowerupType getPowerup() const;
  bool isChaser() const;

private:
  uint8_t x, y;
  uint8_t score;
  PowerupType powerup;
  bool chaser;
};

#endif
