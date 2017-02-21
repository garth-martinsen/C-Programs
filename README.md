# C-Program: Arduino sketch_drawBridge1
Listens to 4 sensors which are switches: shouldRise, highEnough,shouldLower, lowEnough.
Activates two Digital OUTS: Motor direction (DPDT switch) and Motor power (SPST Relay).
The shouldRise event occurs when waters rise enough to trigger a float switch causing a LOW.
The shouldLower event occurs when waters subside and float switch goes to open or high.
The highEnough switch is a contact switch which triggers(CLOSES) when the bridge presses it.
The lowEnough switch is a contact switch which triggers(CLOSES)  when the bridge presses it.
The shouldRise event: sets the motor direction to up, and starts the moter to raise the bridge.
The highEnough event shuts down the motor and applies Brakes to hold the bridge up.
The shouldLower event occurs when the flood water subsides. It sets the motor direction to down and powers the motor to lower the bridge.
The lowEnough event shuts down the motor. 
The sensors are read every 20 seconds in the Lowered and Raised bridge positions. As soon as the motor starts to raise or to lower, the sampling rate increases in an exponential manner, so that as the limit switches are approached, the sampling rate is 1/10 second.
