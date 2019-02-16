/*
  PWM_SamD21.h - Library for pwm application on SAMD21G18A (e.g. Arduino Zero/M0).
  Created by E.Burkowski, February 18, 2017.
  Updated by S.Briscoe, June 1, 2017.
  Released into the public domain.
*/
#ifndef PWM_SamD21_h
#define PWM_SamD21_h

#include "Arduino.h"

/*
Recommended pwm pins: 0-13, A3, A4
*/

class PWM_SamD21
{
  public:
  
	// class constructors
	PWM_SamD21(int pin, int freq);
	PWM_SamD21(int pin, int freq, bool invert);
	PWM_SamD21(int pin, int freq, bool invert, byte divider);
	
	// initializes the pwm hardware
	void init();
	
	void setValue(int value);
	int  getMaxValue();
	void setOff();
	void setDutyCycle(double percentDutyCycle);
	
  private:
	int _maxValue;
	int _pin;
	int _frequency;
	bool _invert;
	byte _div;
};

#endif