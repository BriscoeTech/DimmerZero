#include <PWM_SamD21.h>

//change frequency to 500Hz for both channel, because pins 11 and 13 both on timer2
//Create an instance for channel1 on pin D11 in inverted mode
PWM_SamD21 channel1(11, 500, true);
//Create an instance for channel2 on pin D13 in inverted mode
PWM_SamD21 channel2(13, 500, true);

PWM_SamD21 pwm38khz(3, 500, true);

void setup() 
{
    
    //initialize channel1 and channel2
    channel1.init();
    channel2.init();
    pwm38khz.init();

    //set channel1 to max value to turn it off, because it inverted 
    channel1.setValue(channel1.getMaxValue());
    //set channel2 to max value to turn it off, because it inverted 
    channel2.setValue(channel2.getMaxValue());

    // set to 50 percent duty cycle
    channel2.setValue( channel2.getMaxValue() / 2);
}

void loop() 
{
    //fade in
    for (int c1 = channel1.getMaxValue(); c1 >= 0;c1-=50){
        channel1.setValue(c1);
        delay(20);
    }
    delay(100);

    //fade out
    for (int c1 = 0; c1 <= channel1.getMaxValue();c1+=50){
        channel1.setValue(c1);
        delay(20);
    }
    delay(100);
    
    //fade in
    for (int c2 = channel2.getMaxValue(); c2 >= 0;c2-=50){
        channel2.setValue(c2);
        delay(20);
    }
    delay(100);

    //fade out
    for (int c2 = 0; c2 <= channel2.getMaxValue();c2+=50){
        channel2.setValue(c2);
        delay(20);
    }
    delay(100);

}
