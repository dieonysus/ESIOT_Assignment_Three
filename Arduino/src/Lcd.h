#ifndef _LCD_
#define _LCD_

#include "Arduino.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

class Lcd{
  public:
  Lcd(uint8_t address, uint8_t columns, uint8_t rows);
  void init();
  void updateMode(String state);
  void updatePercentage(int percent);
  void updateTemp(int temp);
  void print(const String& line);
  void print(const int line);
  void clearScreen();
  void setCursorTo(int col, int row);

  private:
  LiquidCrystal_I2C lcd;
};
#endif