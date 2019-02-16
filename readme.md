### Library for dimmer application with SAMD21G18A (e.g. Arduino Zero/M0).

##### Supported pwm pins (only one pin on the same channel is supported, e.g. 6):

|timer    | channel  | recommended pin | alternative pins |
|:--------|:---------|:----------------|:-----------------|
|	Timer0	|	0	|	2	|	A3	|
|	Timer0	|	1	|	5	|	A4	|
|	Timer0	|	2	|	6	|	10	|
|	Timer0	|	3	|	7	|	12	|
|	|	|	|
|	Timer1	|	0	|	4	|	1, 8	|
|	Timer1	|	1	|	3	|	0, 9	|
|	|	|	|
|	Timer2	|	0	|	6	|	11	|
|	Timer2	|	1	|	7	|	13	|

Listed pins are for Arduino Zero. On Arduino M0 (Pro) pins 2 and 4 are reversed!

##### ATTENTION:
if you change frequency or invert output, you should do it for **all** pins on same timer. E.g. if 11 is inverted, 13 must be inverted too!


************************************************************************************

Create a PWM_SamD21 object and initialize it with your desired pwm settings <br/>
<br/>
demo a fast pwm channel <br/>
PWM_SamD21 pwm38khz(2, 38000, false);<br/>
PWM_SamD21 pwm56khz(5, 56000, false);<br/>
<br/>
set as non inverting<br/>
PWM_SamD21 pwmA(4,  1000, false);<br/>
PWM_SamD21 pwmB(3,  1000, false);<br/>
<br/>
setup as inverting<br/>
PWM_SamD21 pwmC(11, 1000, true);<br/>
PWM_SamD21 pwmD(13, 1000, true);<br/>

************************************************************************************

demonstrate different ways to set the duty cycle and control the output<br/>
<br/>
pwmA.setValue(pwmA.getMaxValue() / 4)    set non-inverted channels, 1/4 duty cycle <br/>
pwmB.setDutyCycle(0.25)                  set non-inverted channels, 1/4 duty cycle <br/>
<br/>
pwmC.setValue(pwmC.getMaxValue() / 4)    set inverted channels, 3/4 duty cycle <br/>
pwmD.setDutyCycle(0.25)                  set inverted channels, 3/4 duty cycle <br/>
<br/>
set to 50 percent duty cycle <br/>
pwm38khz.setDutyCycle(0.50) <br/>
pwm56khz.setDutyCycle(0.50) <br/>
<br/>
turn off the pwm channels <br/>
pwmA.setOff() <br/>
pwmD.setOff() <br/>



