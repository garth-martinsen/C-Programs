//pin definitions
int wu = 8;     // waterUp Sensor
int he = 9;     // highEnough sensor
int wd = 10;    // waterDown Sensor
int le = 12;    // lowEnough Sensor.
int dir = 5;    // direction Relay HIGH -> up;  LOW -> down.
int motor = 6;  // motor power relay

double longestPeriod=20000.0;
volatile double loopPeriod= longestPeriod;  //delay will approach 0 as bridge nears max height.

int debounceTime=20; //20ms

// boolean values set by the ISRs and used to control actuators.
volatile boolean shouldRise;
volatile boolean atHigh;
volatile boolean shouldLower;
volatile boolean atLow;
volatile boolean direkshun;
volatile boolean motorsOn;
volatile boolean isRising=false;
volatile boolean isLowering=false;
volatile boolean stopRise=false;
volatile boolean stopLower=false;
volatile boolean bridgeUp=false;
volatile boolean bridgeDown=false;


void readSensors(){
  //Serial.print("  Reading digital inputs...\n");
  shouldRise =   !digitalRead(wu);
  atHigh =   !digitalRead(he);
  shouldLower = !digitalRead(wd);
  atLow =    !digitalRead(le);
  direkshun = !digitalRead(dir);
  motorsOn = !digitalRead(motor);
 

 
}
void takeAction(){
  if( shouldRise && !isRising && !atHigh) watersUp();
  else if(atHigh && isRising) highEnough();
  else if(!atLow && shouldLower && !isLowering) watersDown();
  else if(atLow && isLowering) lowEnough();
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
   Serial.print("watersUp function called. Started raising the bridge.\n");
    if(!isRising)   isRising   = !atHigh && shouldRise;
    if(isRising) startRaising();
}

  //called when maximum height switch is activated. Set brakes, deactivate relay, stop motor.
void highEnough(){ 
  Serial.print("highEnough function called.Stopped raising the bridge.\n");
  if(!stopRise)   stopRise   =  atHigh && isRising;
  if(stopRise) stopRaising();
  bridgeUp = true;
  bridgeDown=false;
  isRising=false;
  loopPeriod = longestPeriod;
  
}

//called when water is low enough to lower bridge. Activates relay to start motor to lower the bridge
void watersDown(){ 
     Serial.print("watersDown function called. Started lowering the bridge.\n");
   if(!isLowering) isLowering = !atLow && shouldLower;
   if(isLowering) startLowering();
}

// called when minimum height switch is activated.Deactivate relay, stopping the motor
void lowEnough(){  
     Serial.print("lowEnough function called.Stopped bridge lowering.\n");
       if(!stopLower)  stopLower  =  atLow && shouldLower;
       if(stopLower) stopLowering();
       bridgeUp=false;
       bridgeDown=true;
 
}

void startRaising(){
  Serial.print("Setting the motor direction:up and motor power:on.\n");
  digitalWrite(dir,HIGH); //up direction
  digitalWrite(motor,HIGH);
  loopPeriod = longestPeriod; 
}

void stopRaising(){
  Serial.print("Setting motor power: off.");
  digitalWrite(motor,LOW); 
  loopPeriod=longestPeriod;
}
 void   startLowering(){
  Serial.print("Setting motor direction:down and motor power: on.\n");
  digitalWrite(dir,LOW); //direction: down.
  digitalWrite(motor,HIGH); //start motor
  loopPeriod = longestPeriod; 
 }
 void stopLowering(){
  Serial.print("Setting motor power: OFF.\n");
  digitalWrite(motor,LOW);
  loopPeriod = longestPeriod;
  isLowering = false; 
  bridgeUp=false;
  bridgeDown=true;
 }

 String bridgeStatus(){
  String stat;
 if(isRising) stat="Rising";
 else if(isLowering) stat= "Lowering";
 else if(bridgeUp) stat="Raised";
 else if(bridgeDown) stat="Lowered";
 else stat= "Waiting for Events!";
 return stat + " ";
 }

void setup() {
  // put your setup code here, to run once:
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
