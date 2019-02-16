/*
  PWM_SamD21.cpp - Library for pwm application with SAMD21G18A (e.g. Arduino Zero/M0).
  Created by E.Burkowski, February 18, 2017.
  Updated by S.Briscoe, June 1, 2017.
  Released into the public domain.
*/

#include "Arduino.h"
#include "PWM_SamD21.h"

/*
Recommended pwm pins: 0-13, A3, A4
*/

PWM_SamD21::PWM_SamD21(int pin, int freq)
{
  _pin = pin;
  _invert = false;
  _frequency = freq;
  _div = 2;
  init();
}

PWM_SamD21::PWM_SamD21(int pin, int freq, bool invert)
{
  _pin = pin;
  _invert = invert;
  _frequency = freq;
  _div = 2;
  init();
}

PWM_SamD21::PWM_SamD21(int pin, int freq, bool invert, byte divider)
{
  _pin = pin;
  _invert = invert;
  _frequency = freq;
  _div = divider;
  init();
}

// Initializes the pwm hardware based on class settings
// Pwm is set to off by default
void PWM_SamD21::init()
{
	int timerTCC = 0; // updated based on pin used later in code
	
	// calculate the max value that this timer will count to to generate the pwm signal
	_maxValue = (48000000) / (_frequency * _div * 2);
	
	REG_GCLK_GENDIV = GCLK_GENDIV_DIV(_div) |		// Divide the 48MHz clock source by divisor div: e.g. 48MHz/4=12MHz
						GCLK_GENDIV_ID(4);			// Select Generic Clock (GCLK) 4
	while (GCLK->STATUS.bit.SYNCBUSY);				// Wait for synchronization

	REG_GCLK_GENCTRL = GCLK_GENCTRL_IDC |			// Set the duty cycle to 50/50 HIGH/LOW
						GCLK_GENCTRL_GENEN |		// Enable GCLK4
						GCLK_GENCTRL_SRC_DFLL48M |	// Set the 48MHz clock source
						GCLK_GENCTRL_ID(4);			// Select GCLK4
	while (GCLK->STATUS.bit.SYNCBUSY);				// Wait for synchronization
 
	PORT->Group[g_APinDescription[_pin].ulPort].PINCFG[g_APinDescription[_pin].ulPin].bit.PMUXEN = 1;
	switch (_pin) 
	{
		case 0:
			PORT->Group[g_APinDescription[1].ulPort].PMUX[g_APinDescription[1].ulPin >> 1].reg |= PORT_PMUX_PMUXO_E;
			timerTCC = 1;
			break;
		case 1:
			PORT->Group[g_APinDescription[1].ulPort].PMUX[g_APinDescription[1].ulPin >> 1].reg |= PORT_PMUX_PMUXE_E;
			timerTCC = 1;
			break;
		case 2:
			PORT->Group[g_APinDescription[2].ulPort].PMUX[g_APinDescription[2].ulPin >> 1].reg |= PORT_PMUX_PMUXE_F;
			timerTCC = 0;
			break;
		case 3:
			PORT->Group[g_APinDescription[4].ulPort].PMUX[g_APinDescription[4].ulPin >> 1].reg |= PORT_PMUX_PMUXO_F;
			timerTCC = 1;
			break;
		case 4:
			PORT->Group[g_APinDescription[4].ulPort].PMUX[g_APinDescription[4].ulPin >> 1].reg |= PORT_PMUX_PMUXE_F;
			timerTCC = 1;
			break;
		case 5:
			PORT->Group[g_APinDescription[2].ulPort].PMUX[g_APinDescription[2].ulPin >> 1].reg |= PORT_PMUX_PMUXO_F;
			timerTCC = 0;
			break;
		case 6:
			PORT->Group[g_APinDescription[6].ulPort].PMUX[g_APinDescription[6].ulPin >> 1].reg |= PORT_PMUX_PMUXE_F;
			timerTCC = 0;
			break;
		case 7:
			PORT->Group[g_APinDescription[6].ulPort].PMUX[g_APinDescription[6].ulPin >> 1].reg |= PORT_PMUX_PMUXO_F;
			timerTCC = 0;
			break;
		case 8:
			PORT->Group[g_APinDescription[8].ulPort].PMUX[g_APinDescription[8].ulPin >> 1].reg |= PORT_PMUX_PMUXO_E;
			timerTCC = 1;
			break;
		case 9:
			PORT->Group[g_APinDescription[8].ulPort].PMUX[g_APinDescription[8].ulPin >> 1].reg |= PORT_PMUX_PMUXE_E;
			timerTCC = 1;
			break;
		case 10:
			PORT->Group[g_APinDescription[10].ulPort].PMUX[g_APinDescription[10].ulPin >> 1].reg |= PORT_PMUX_PMUXE_F;
			timerTCC = 0;
			break;
		case 11:
			PORT->Group[g_APinDescription[11].ulPort].PMUX[g_APinDescription[11].ulPin >> 1].reg |= PORT_PMUX_PMUXE_E;
			timerTCC = 2;
			break;
		case 12:
			PORT->Group[g_APinDescription[10].ulPort].PMUX[g_APinDescription[10].ulPin >> 1].reg |= PORT_PMUX_PMUXO_F;
			timerTCC = 0;
			break;
		case 13:
			PORT->Group[g_APinDescription[11].ulPort].PMUX[g_APinDescription[11].ulPin >> 1].reg |= PORT_PMUX_PMUXO_E;
			timerTCC = 2;
			break;
		case A3:
			PORT->Group[g_APinDescription[A3].ulPort].PMUX[g_APinDescription[A3].ulPin >> 1].reg |= PORT_PMUX_PMUXO_E;
			timerTCC = 0;
			break;
		case A4:
			PORT->Group[g_APinDescription[A3].ulPort].PMUX[g_APinDescription[A3].ulPin >> 1].reg |= PORT_PMUX_PMUXE_E;
			timerTCC = 0;
			break;
		default:
//			#error "Not supported pin!"
			break;
	}
	
	// setup the timer based on what pin it is connected to
	if (timerTCC == 0)
	{
		// Feed GCLK4 to TCC0
		REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN |		// Enable GCLK4 to TCC0 and TCC1
		GCLK_CLKCTRL_GEN_GCLK4 |					// Select GCLK4
		GCLK_CLKCTRL_ID_TCC0_TCC1;					// Feed GCLK4 to TCC0 and TCC1
		while (GCLK->STATUS.bit.SYNCBUSY);			// Wait for synchronization
		
		if(_invert) 
		{
			REG_TCC0_WAVE |= TCC_WAVE_WAVEGEN_DSBOTH;
			while (TCC0->SYNCBUSY.bit.WAVE);
		}
		else
		{
			REG_TCC0_WAVE |= TCC_WAVE_POL(0xF) | TCC_WAVE_WAVEGEN_DSBOTH;	// Setup dual slope PWM on TCC0
			while (TCC0->SYNCBUSY.bit.WAVE);  
		}
		
		REG_TCC0_PER = _maxValue;					// Set the frequency of the PWM on TCC0 
		while (TCC0->SYNCBUSY.bit.PER);				// Wait for synchronization
		
		REG_TCC0_CTRLA |= TCC_CTRLA_PRESCALER_DIV1 |// Divide GCLK4 by 1
							TCC_CTRLA_ENABLE;		// Enable the TCC0 output
		while (TCC0->SYNCBUSY.bit.ENABLE);			// Wait for synchronization

	}
	else if (timerTCC == 1)
	{
		// Feed GCLK4 to TCC1
		REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN |		// Enable GCLK4 to TCC0 and TCC1
		GCLK_CLKCTRL_GEN_GCLK4 |					// Select GCLK4
		GCLK_CLKCTRL_ID_TCC0_TCC1;					// Feed GCLK4 to TCC0 and TCC1
		while (GCLK->STATUS.bit.SYNCBUSY);			// Wait for synchronization
		
		if(_invert) 
		{
			REG_TCC1_WAVE |= TCC_WAVE_WAVEGEN_DSBOTH;
			while (TCC1->SYNCBUSY.bit.WAVE);
		}
		else
		{
			REG_TCC1_WAVE |= TCC_WAVE_POL(0xF) | TCC_WAVE_WAVEGEN_DSBOTH;	// Setup dual slope PWM on TCC1
			while (TCC1->SYNCBUSY.bit.WAVE);  
		} 
		
		REG_TCC1_PER = _maxValue;						// Set the frequency of the PWM on TCC0 
		while (TCC1->SYNCBUSY.bit.PER);				// Wait for synchronization
		
		REG_TCC1_CTRLA |= TCC_CTRLA_PRESCALER_DIV1 |// Divide GCLK4 by 1
							TCC_CTRLA_ENABLE;		// Enable the TCC1 output
		while (TCC1->SYNCBUSY.bit.ENABLE);			// Wait for synchronization

	}
	else if (timerTCC == 2)
	{
		// Feed GCLK4 to TCC2
		REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN |		// Enable GCLK4 to TCC0 and TCC1
		GCLK_CLKCTRL_GEN_GCLK4 |					// Select GCLK4
		GCLK_CLKCTRL_ID_TCC2_TC3;					// Feed GCLK4 to TCC0 and TCC1
		while (GCLK->STATUS.bit.SYNCBUSY);			// Wait for synchronization
		
		if(_invert) {
			REG_TCC2_WAVE |= TCC_WAVE_WAVEGEN_DSBOTH;
			while (TCC0->SYNCBUSY.bit.WAVE);
		}
		else
		{
			REG_TCC2_WAVE |= TCC_WAVE_POL(0xF) | TCC_WAVE_WAVEGEN_DSBOTH;	// Setup dual slope PWM on TCC2
			while (TCC2->SYNCBUSY.bit.WAVE);  
		}
		
		REG_TCC2_PER = _maxValue;					// Set the frequency of the PWM on TCC0 
		while (TCC2->SYNCBUSY.bit.PER);				// Wait for synchronization
		
		REG_TCC2_CTRLA |= TCC_CTRLA_PRESCALER_DIV1 |// Divide GCLK4 by 1
							TCC_CTRLA_ENABLE;		// Enable the TCC2 output
		while (TCC2->SYNCBUSY.bit.ENABLE);			// Wait for synchronization

	}
	else
	{
		// error! timer does not exist!
	}
	
	// make sure that pwm output is off by default until the user specifies what the duty cycle is
	setOff();
	
#ifndef __SAMD21G18A__
    #error "This library only supports SAMD21G18A based boards (e.g. Zero/M0...)"
#endif

}

// set the register that controls pwm duty cycle
void PWM_SamD21::setValue(int value)
{
	int normalizedValue = value;
	
	// make sure input is within a valid range
	if (normalizedValue > _maxValue)
	{
		normalizedValue = _maxValue;
	}
	else if (normalizedValue < 0)
	{
		normalizedValue = 0;
	}
	
	switch (_pin) 
	{
		case 0:
			REG_TCC1_CC1 = normalizedValue;		// Set the PWM signal
			while (TCC1->SYNCBUSY.bit.CC1);		// Wait for synchronization
			break;
		case 1:
			REG_TCC1_CC0 = normalizedValue;		// Set the PWM signal
			while (TCC1->SYNCBUSY.bit.CC0);		// Wait for synchronization
			break;
		case 2:
			REG_TCC0_CC0 = normalizedValue;		// Set the PWM signal
			while (TCC0->SYNCBUSY.bit.CC0);		// Wait for synchronization
			break;
		case 3:
			REG_TCC1_CC1 = normalizedValue;		// Set the PWM signal
			while (TCC1->SYNCBUSY.bit.CC1);		// Wait for synchronization
			break;
		case 4:
			REG_TCC1_CC0 = normalizedValue;		// Set the PWM signal
			while (TCC1->SYNCBUSY.bit.CC0);		// Wait for synchronization
			break;
		case 5:
			REG_TCC0_CC1 = normalizedValue;		// Set the PWM signal
			while (TCC0->SYNCBUSY.bit.CC1);		// Wait for synchronization
			break;
		case 6:
			REG_TCC0_CC2 = normalizedValue;		// Set the PWM signal
			while (TCC0->SYNCBUSY.bit.CC2);		// Wait for synchronization
			break;
		case 7:
			REG_TCC0_CC3 = normalizedValue;		// Set the PWM signal
			while (TCC0->SYNCBUSY.bit.CC3);		// Wait for synchronization
			break;
		case 8:
			REG_TCC1_CC0 = normalizedValue;		// Set the PWM signal
			while (TCC1->SYNCBUSY.bit.CC0);		// Wait for synchronization
			break;
		case 9:
			REG_TCC1_CC1 = normalizedValue;		// Set the PWM signal
			while (TCC1->SYNCBUSY.bit.CC1);		// Wait for synchronization
			break;
		case 10:
			REG_TCC0_CC2 = normalizedValue;		// Set the PWM signal
			while (TCC0->SYNCBUSY.bit.CC2);		// Wait for synchronization
			break;
		case 11:
			REG_TCC2_CC0 = normalizedValue;		// Set the PWM signal
			while (TCC2->SYNCBUSY.bit.CC0);		// Wait for synchronization
			break;
		case 12:
			REG_TCC0_CC3 = normalizedValue;		// Set the PWM signal
			while (TCC0->SYNCBUSY.bit.CC3);		// Wait for synchronization
			break;
		case 13:
			REG_TCC2_CC1 = normalizedValue;		// Set the PWM signal
			while (TCC2->SYNCBUSY.bit.CC1);		// Wait for synchronization
			break;
		case A3:
			REG_TCC0_CC0 = normalizedValue;		// Set the PWM signal
			while (TCC0->SYNCBUSY.bit.CC0);		// Wait for synchronization
			break;
		case A4:
			REG_TCC0_CC1 = normalizedValue;		// Set the PWM signal
			while (TCC0->SYNCBUSY.bit.CC1);		// Wait for synchronization
			break;
		default:
			break;
	}
}

// get the max register value possible for setting pwm duty cycle
int PWM_SamD21::getMaxValue()
{
	return _maxValue;
}

// turns off the pwm output, pin will be low
void PWM_SamD21::setOff()
{
	
	if(_invert == true)
	{
		// inverting outputs need to be at max value for off
		setValue( _maxValue);
	}
	else
	{
		// non inverted outputs set to zero for off
		setValue( 0 );
	}
}

// sets the duty cycle to a decimal percent value
// not super accurate due to integer truncation, but very convenient
void PWM_SamD21::setDutyCycle(double percentDutyCycle)
{

	setValue(  (int)( (double)_maxValue * (double)percentDutyCycle ) );
	
}


