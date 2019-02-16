#include <PWM_SamD21.h>


// alias for the usb debug channel of arduino zero
#define SERIAL SerialUSB

//*******************************************************

// demo a fast pwm channel
PWM_SamD21 pwm38khz(2, 38000, false);
PWM_SamD21 pwm56khz(5, 56000, false);

// set as non inverting
PWM_SamD21 pwmA(4,  1000, false);
PWM_SamD21 pwmB(3,  1000, false);

// setup as inverting
PWM_SamD21 pwmC(11, 1000, true);
PWM_SamD21 pwmD(13, 1000, true);

//*******************************************************

void setup() 
{
  
    SERIAL.begin(115200);

    SERIAL.println("******************************");
    SERIAL.println("Program Started:");
  
  	//*************
  	// show that all channels are off by default until set
    delay(5000);

    //*************
	  // demonstrate different ways to set the duty cycle
   
    pwmA.setValue(pwmA.getMaxValue() / 4);  //set non-inverted channels, 1/4 duty cycle
    pwmB.setDutyCycle(0.25);                //set non-inverted channels, 1/4 duty cycle
    
    pwmC.setValue(pwmC.getMaxValue() / 4);  //set inverted channels, 3/4 duty cycle
    pwmD.setDutyCycle(0.25);                //set inverted channels, 3/4 duty cycle

    // set to 50 percent duty cycle
    pwm38khz.setDutyCycle(0.50);
    pwm56khz.setDutyCycle(0.50);
    
    delay(10000);

    //*************
    
	  pwmB.setDutyCycle(0.90);  //set non-inverted channels 90% duty cycle

	  // turn afew of the pwm channels off for demonstration
    pwmA.setOff();
    pwmD.setOff();
    
    //*************
    
    delay(10000);

    
}

//*******************************************************

void loop() 
{

//	  // print the max value of the pwm, double check the hardware math didn't truncate or round anything wrong
//    SERIAL.println(pwmC.getMaxValue());
//    SERIAL.println(pwmD.getMaxValue());
//    SERIAL.println(pwm38khz.getMaxValue());
//  
//    //fade in
//    for (int c = pwmC.getMaxValue(); c >= 0; c-=50)
//    {
//        pwmC.setValue(c);
//        pwmD.setValue(c);
//        delay(20);
//    }
//    delay(100);
//
//    //fade out
//    for (int c = 0; c <= pwmC.getMaxValue(); c+=50)
//    {
//        pwmC.setValue(c);
//        pwmD.setValue(c);
//        delay(20);
//    }
//    delay(100);
//
//    //*************

    //fade out inverted signals
    for(double c=0; c<=1; c=c+0.01)
    {
       pwmC.setDutyCycle(c);
       pwmD.setDutyCycle(c);
       delay(100);
    }
    delay(100);
    
    //fade in inverted signals
    for(double c=1; c>=0; c=c-0.01)
    {
       pwmC.setDutyCycle(c);
       pwmD.setDutyCycle(c);
       delay(100);
    }
    delay(100);

}
