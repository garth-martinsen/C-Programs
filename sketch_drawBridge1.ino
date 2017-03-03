//pin definitions
int wu = 8;     // waterUp Sensor
int he = 9;     // highEnough sensor
int wd = 10;    // waterDown Sensor
int le = 11;    // lowEnough Sensor.
int dir = 5;    // direction DPDT
int motor = 6;  // motor power relay

double longestPeriod=20000.0;
volatile double loopPeriod= longestPeriod;  //delay will approach 100 ms as bridge nears limit switches.



// boolean values from reading sensors
volatile boolean shouldRise;
volatile boolean atHigh;
volatile boolean shouldLower;
volatile boolean atLow;

//actuator values
volatile boolean direkshun;  //Relay HIGH -> up;  LOW -> down.
volatile boolean motorsOn;

//Bridge States
volatile boolean isRising=false;
volatile boolean isLowering=false;
volatile boolean bridgeUp=false;
volatile boolean bridgeDown=false;



void readSensors(){
  //Serial.print("  Reading digital inputs...\n");
  shouldRise  =   !digitalRead(wu);
  atHigh      =   !digitalRead(he);
  shouldLower =   !digitalRead(wd);
  atLow       =   !digitalRead(le);
  direkshun   =   !digitalRead(dir);
  motorsOn    =   !digitalRead(motor);
  Serial.print( "Word: " + String((shouldRise << 3)+ (atHigh << 2) + (shouldLower << 1) + atLow) + " ");
}

void takeAction(){
  if( shouldRise && !isRising && !atHigh) 
    watersUp();
  else if(atHigh && isRising) 
    highEnough();
  else if( shouldLower && !isLowering && !atLow ) 
    watersDown();
  else if(atLow && isLowering) 
    lowEnough();
  else Serial.print("No Action.");
}
 
 // if (isRising or isLowering ) loopPeriod= loopPeriod/1.5 until reaches 100 ms
void updatePeriod(){
  if(isRising || isLowering)
    if(loopPeriod > 100.0) loopPeriod = loopPeriod/1.5; else loopPeriod= 100.0;
}
void showStatus(){
  Serial.print("loopPeriod: " + String(loopPeriod) + " Bridge is : " + bridgeStatus() + "  " +
         String(shouldRise) + "," + 
         String(atHigh) + "," + 
         String(shouldLower) + "," + 
         String(atLow)+ "," + 
         String(digitalRead(dir))+ "," + 
         String(digitalRead(motor))+"\n");
}


void watersUp(){  //called when high water switch indicates that bridge shouldRise.
   //Serial.print("watersUp function called. Started raising the bridge.\n");
    if(!isRising)   isRising   = !atHigh && shouldRise;
    Serial.print("Setting the motor direction:up and motor power:on. Raising the bridge\n");
    digitalWrite(dir,HIGH); //up direction
    digitalWrite(motor,HIGH);
    loopPeriod = longestPeriod; 
}

  //called when maximum height switch is closed. Set brakes, deactivate relay, stop motor.
void highEnough(){ 
 // Serial.print("highEnough function called.\n");
    Serial.print("Setting motor power: off. Stopped raising the bridge\n");
    digitalWrite(motor,LOW); 
    digitalWrite(dir,LOW); 
  
    loopPeriod=longestPeriod;
    bridgeUp = true;
    bridgeDown=false;
    isRising=false;
    loopPeriod = longestPeriod;
}

//called when water is low enough to lower bridge. Activates relay to start motor to lower the bridge
void watersDown(){ 
  // Serial.print("watersDown function called. Started lowering the bridge.\n");
    if(!isLowering) isLowering = !atLow && shouldLower;
    Serial.print("Setting motor direction:down and motor power: on. Lowering the bridge.\n");
    digitalWrite(dir,LOW); //direction: down.
    digitalWrite(motor,HIGH); //start motor
    loopPeriod = longestPeriod; 
}

// called when minimum height switch is closed. Deactivates relay, stopping the motor
void lowEnough(){  
  //   Serial.print("lowEnough function called.");
       Serial.print("Setting motor power: OFF. Stopped lowering bridge.\n");
       digitalWrite(dir,HIGH); //direction: down.
       digitalWrite(motor,LOW);
       loopPeriod = longestPeriod;
       isLowering = false; 
       bridgeUp=false;
       bridgeDown=true;
}

 String bridgeStatus(){
   String stat;
   if(isRising) 
    stat="Rising";
   else if(isLowering) 
    stat= "Lowering";
   else if(bridgeUp) 
    stat="Raised";
   else if(bridgeDown) 
    stat="Lowered";
   else 
    stat= "Waiting for Events!";
 return stat + " ";
 }

void setup() {
  //set pins as input or output, pullup so quiescent state is all HIGH.
  Serial.begin(9600);
  pinMode(wu, INPUT);
  pinMode(he, INPUT);
  pinMode(wd, INPUT);
  pinMode(le, INPUT);
  pinMode(dir, OUTPUT);
  pinMode(motor, OUTPUT);
  
  digitalWrite(wu,HIGH);  //pullup
  digitalWrite(he,HIGH);  //pullup
  digitalWrite(wd,HIGH);  //pullup
  digitalWrite(le,HIGH);  //pullup
}


void loop() {
  // put your main code here, to run repeatedly:
   readSensors();
   takeAction();
   delay(loopPeriod);
   updatePeriod();
   showStatus();
  }
