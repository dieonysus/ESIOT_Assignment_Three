#include <Arduino.h>
#include "WindowControllerTask.h"

#define button_pin 2
#define pot_pin A0
#define servo_pin 10

WindowControllerTask* windowControllerTask;

void setup() {

 windowControllerTask = new WindowControllerTask(button_pin, pot_pin, servo_pin);
 windowControllerTask->init();
}

void loop() {

 windowControllerTask->tick();
 delay(50);
}