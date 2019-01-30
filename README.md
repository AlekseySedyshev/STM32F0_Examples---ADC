# STM32F0_Examples---ADC
This is a simple example how to use ADC on STM32F0
Pa7 - ADC input, PB0 - PWM output (led).
In the Main cycle every 10 ms programm scan ADC channel 
(connected to Adj. Resistor), got result and put it into 
CCR3 register which for Adjust Led Light level.
