#include <Arduino.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include "Button.h"
#include "ServoMotor.h"
#include "Potentiometer.h"
#include "MsgService.h"

#define button_pin 2
#define pot_pin A0
#define servo_pin 10

int lastPercentage = 0;

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27,20,4);
Button* btn;
ServoMotor* window;
Potentiometer* potentiometer;

enum {automatic, manual} state ;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  MsgService.init();
  lcd.init();
  lcd.backlight();

  btn = new Button(button_pin);
  window = new ServoMotor(servo_pin);
  potentiometer = new Potentiometer(pot_pin);

  window->openPercentage(0);
  state = automatic;

  lcd.setCursor(6,0);
  lcd.print("state:Automatic");
}

void loop() {
  // put your main code here, to run repeatedly:
  bool buttonIsPressed = btn->isPressed();


  switch(state){

    case automatic:
    
    lcd.setCursor(0, 2);
    lcd.print(" ");
    if(MsgService.isMsgAvailable()){
    Msg* msg = MsgService.receiveMsg();
    int percentage = msg->getContent().toInt();
    lcd.setCursor(2,3);
    lcd.print("    ");
    lcd.setCursor(2,3);
    lcd.print(percentage);
    lcd.print("%");
    window->openPercentage(percentage);
    
  }


    if(buttonIsPressed){
      state = manual;
      lcd.clear();
      lcd.setCursor(6,0);
      lcd.print("state:Manual");
      MsgService.sendMsg("Manual");
    }
    break;


    case manual:

    if(buttonIsPressed){

      state = automatic;
      lcd.clear();
      lcd.setCursor(6,0);
      lcd.print("state:Automatic");
      MsgService.sendMsg("Automatic");

    }else{
      int potPercentage = potentiometer->percentageValue();

      if(potPercentage != lastPercentage){
        lastPercentage = potPercentage;
        lcd.setCursor(2,3);
        lcd.print("    ");    
        lcd.setCursor(2, 3);
        lcd.print(lastPercentage);
        lcd.print("%");
        window->openPercentage(lastPercentage);
        //MsgService.sendMsg();
      }
    }
  }
 delay(200);
}

