#ifndef _POTENTIOMETER_ 
#define _POTENTIOMETER_

class Potentiometer{
  public:
  Potentiometer(int pin);
  int getPercentageValue();

  private:
  int pin;
};

#endif