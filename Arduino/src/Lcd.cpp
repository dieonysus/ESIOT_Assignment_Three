#include "Lcd.h"
#include "Arduino.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

Lcd::Lcd(uint8_t address, uint8_t columns, uint8_t rows) : lcd(address, columns, rows){
}

void Lcd::init(){
  lcd.begin(20,4);
  lcd.backlight();
}

void Lcd::updateState(String state){
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("state:");
  lcd.print(state);
}

void Lcd::updatePercentage(int percent){
  lcd.setCursor(2,3);
  lcd.print("    ");    
  lcd.setCursor(2, 3);
  lcd.print(percent);
  lcd.print("%");
}

void Lcd::updateTemp(int temp){
  lcd.setCursor(12,3);
  lcd.print("     ");
  lcd.setCursor(12,3);
  lcd.print(temp);
  lcd.print((char)223);
  lcd.print("C");
}

void Lcd::print(const String& line){
  lcd.print(line);
}

void Lcd::print(const int line){
  lcd.print(line);
}

void Lcd::clearScreen(){
  lcd.clear();
}

void Lcd::setCursorTo(int col, int row){
  lcd.setCursor(col, row);
}