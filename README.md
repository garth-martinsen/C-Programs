# C-Program: Arduino sketch_drawBridge1
This Arduino code is used to control a footbridge position: raising (when flooding) and lowering (for normal use). 
When raised, the bridge is about 5 feet above its normal level. This prevents flood waters from ripping it out and destroying it. 
Control program listens to 4 sensors which are switches and triggers events: shouldRise, highEnough,shouldLower, lowEnough. 
Events activates two Digital OUTPUTS: Motor direction (DPDT switch) and Motor power (SPST Relay). 
The "shouldRise" event occurs when the water rises enough to trigger a float switch causing a LOW. 
The shouldRise event: sets the motor direction to HIGH (or UP), and starts the moter to raise the bridge. 
The "shouldLower" event occurs when waters subside and float switch goes to open or HIGH. 
The shouldLower event sets the motor direction to LOW (or DOWN) and powers the motor to lower the bridge. 
When rising and approaching the upper limit, the bridge will CLOSE a contact switch triggering the "highEnough" event. 
It turns the motor off and applies Brakes to hold the bridge up. 
When the bridge isLowering and approaching the lower limit, it will CLOSE a contact switch triggering the "lowEnough" event. 
"lowEnough" eventHandler turns the motor off. 
The bridge has four states: Lowered, Raised, isRising, and isLowering. 
In the "Lowered" and "Raised" bridge positions, the sensors are read every 20 seconds (0.05 HZ). 
As soon as the motor starts to raise or to lower, the sampling rate increases in an exponential manner, 
so that as the limit switches are approached, the sampling frequency is 10 HZ. (sampling every 1/10 second).
