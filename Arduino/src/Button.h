#ifndef __BUTTON__
#define __BUTTON__

class Button{
  public:
   Button(int pin, unsigned long debounceTime = 20);
   
   virtual bool isPressed();

  protected:
   int pin;
   unsigned long lastPressTime;
   unsigned long debounceTime;
   int buttonState;
   int lastButtonState;
   
};

#endif