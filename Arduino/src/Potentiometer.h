#ifndef _POTENTIOMETER_ 
#define _POTENTIOMETER_

class Potentiometer{
  public:
  Potentiometer(int pin);
  int percentageValue();

  private:
  int pin;
};

#endif