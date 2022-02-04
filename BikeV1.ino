//Bike Controller v1
//Created By: Brad Sanders
//MF: 11/8/2017
//Arduino Pro Mini


/*
*N/O = Relay has to be on for circuit to be complete
*N/C = Relay has to be off for circuit to be complete

*Relay Board
  Relay 1 Pin 2 //Left Blinker N/O
  Relay 2 Pin 3 //Right Blinker N/O
  Relay 3 Pin 4 //Headlight N/C
  Relay 4 Pin 5 //Taillight N/C
  Relay 5 Pin 6 //Ext LED N/O
  Relay 6 Pin 7 //Activate Taillight N/O
  Relay 7 Pin 8 //n/a 
  Relay 8 Pin 0 //n/a

*Switches
  Switch 1 Pin 9 //switch 1 = Normal/All Off, Switch GND to Arduino GND
  Switch 2 Pin 10 //switch 2 = Blinker Left, Switch GND to Arduino GND
  Switch 3 Pin 11 //switch 3 = Blinker Right, Switch GND to Arduino GND
  Switch 4 Pin 12 //switch 4 = Brake Peddal, Switch GND to Arduino GND

*Security Light (optional)
  5v Red Led pin 13, Led GND to Arduino GND

*/

//##################################################################################//
//################################# Configuration ##################################//
//##################################################################################//
//##################################################################################//

//################################ Relays ######################################//
#define relayPin1 2 //Left Blinker
#define relayPin2 3 //Right Blinker
#define relayPin3 4 //Headlight
#define relayPin4 5 //Taillight
#define relayPin5 6 //Ext LED
#define relayPin6 7 //Activate Taillight (increse brightness)
#define relayPin7 8 //n/a
#define relayPin8 0 //n/a

//############################### Switches #####################################//
#define switch1Pin 9  //Switch 1 = Normal/All Off
#define switch2Pin 10  //Switch 2 = Blinker Left
#define switch3Pin 11 //Switch 3 = Blinker Right
#define switch4Pin 12 //Switch 4 = Brake Peddel

//########################## Startup Relay State ###############################//
#define relayPin1_StartupState HIGH //LOW = relay on, HIGH = relay off
#define relayPin2_StartupState HIGH //LOW = relay on, HIGH = relay off
#define relayPin3_StartupState LOW  //HIGH = relay on, LOW = relay off
#define relayPin4_StartupState LOW  //HIGH = relay on, LOW = relay off
#define relayPin5_StartupState HIGH //LOW = relay on, HIGH = relay off
#define relayPin6_StartupState HIGH //LOW = relay on, HIGH = relay off
#define relayPin7_StartupState HIGH //LOW = relay on, HIGH = relay off
#define relayPin8_StartupState HIGH //LOW = relay on, HIGH = relay off

//########################### Left/Right Blinker ###############################//
#define BlinkerDelay 500 //(ms) Blinker Delay
#define EnableBlinkersOnAllOffMode 0 //1 = enable blinkers, 0 = disable blinkers 

//############################ Brake/Taillight #################################//
#define EnableBrakeLightEffect 1 //1 = effect on, 0 = effect off
#define BrakeLightEffectBlinkCount 5 //(count) times to blink lights when braking
#define BrakeLightEffectDelay 60 //(ms) delay between flashes in Brake Effect

//########################### Startup Effect ###################################//
#define startupEffectPerLightDelay 200 //(ms) delay between lights in startup effect

//############################ Security Light ##################################//
#define securityLightPin 13 //pin 13 and onboard led
#define securityLightInterval 40000 //Blink Light every 40 seconds or so

//################################################################################//
//############################# Dont Pass This Line ##############################//
//#################################################################################//
//#################################################################################//

int leftBlinkerOn = 0; //keeps blinker left state
int rightBlinkerOn = 0; //keeps blinker right state
int hazzardsOn = 0; //keeps hazzards state
int brakeOn = 0; //keeps brake state
int normalMode = 0; //keeps mode
int enableBlinkers = 0;
int securityLedTime = 0;
int blinkerLeftTime = 0;
int blinkerRightTime = 0;
int hazzardsTime = 0;

//###############################################################################################
//########################################### Setup #############################################

void setup() {
  Serial.begin(115200); //Start Serial Connection For Debug
  Serial.println("Inital startup, Bike Controller v1, MF: 11/8/2017"); //Debug
  Serial.println("Created By: Brad Sanders"); //Debug
  Serial.println("");//empty line
  Serial.println(".................. Debug Output .................."); //Debug
  Serial.println("");//empty line
  
  // put your setup code here, to run once:
  pinMode(relayPin1, OUTPUT); //Left Blinker
  pinMode(relayPin2, OUTPUT); //Right Blinker
  pinMode(relayPin3, OUTPUT); //Headlight
  pinMode(relayPin4, OUTPUT); //Taillight
  pinMode(relayPin5, OUTPUT); //Ext Led
  pinMode(relayPin6, OUTPUT); //Activate Taillight (increse brightness)
  pinMode(relayPin7, OUTPUT); //n/a
  pinMode(relayPin8, OUTPUT); //n/a

  pinMode(switch1Pin, INPUT_PULLUP); //Switch 1 = Normal/All Off
  pinMode(switch2Pin, INPUT_PULLUP); //Switch 2 = Blinker Left
  pinMode(switch3Pin, INPUT_PULLUP); //Switch 3 = Blinker Right
  pinMode(switch4Pin, INPUT_PULLUP); //Switch 4 = Brake Peddel

  digitalWrite(switch1Pin, HIGH); //Switch 1 = Normal/All Off
  digitalWrite(switch2Pin, HIGH); //Switch 2 = Blinker Left
  digitalWrite(switch3Pin, HIGH); //Switch 3 = Blinker Right
  digitalWrite(switch4Pin, HIGH); //Switch 4 = Brake Peddel

  pinMode(13, OUTPUT);//Arduino onboard led
  digitalWrite(13, LOW); //off

  //security light
  pinMode(securityLightPin, OUTPUT);
  digitalWrite(securityLightPin, LOW); //led off

  digitalWrite(relayPin1, relayPin1_StartupState); //Left Blinker
  digitalWrite(relayPin2, relayPin2_StartupState); //Right Blinker
  digitalWrite(relayPin3, relayPin3_StartupState); //Headlight
  digitalWrite(relayPin4, relayPin4_StartupState); //Taillight
  digitalWrite(relayPin5, relayPin5_StartupState); //Ext led
  digitalWrite(relayPin6, relayPin6_StartupState); //Activate Taillight (increse brightness)
  digitalWrite(relayPin7, relayPin7_StartupState); //n/a
  digitalWrite(relayPin8, relayPin8_StartupState); //n/a

  delay(500); //delay startup effect for .5 second(s)
  
  //run the startup effect x2
  startupEffectx1(); 
  startupEffectx1();  
}

//############################ Startup Effect ############################

void startupEffectx1(){
  Serial.println("StartUp Effect/Light Loop Started"); //Debug
  
  digitalWrite(relayPin1, LOW); //left blinker on
  delay(startupEffectPerLightDelay); //wait
  digitalWrite(relayPin1, HIGH); //left blinker off
  digitalWrite(relayPin3, HIGH); //headlight on
  delay(startupEffectPerLightDelay); //wait
  digitalWrite(relayPin3, LOW); //headlight off
  digitalWrite(relayPin2, LOW); //right bliker on
  delay(startupEffectPerLightDelay); //wait
  digitalWrite(relayPin2, HIGH); //right bliker off
  digitalWrite(relayPin5, LOW); //ext led on
  delay(startupEffectPerLightDelay); //wait
  digitalWrite(relayPin5, HIGH); //ext led off
  digitalWrite(relayPin4, HIGH); //taillight on
  delay(startupEffectPerLightDelay); //wait
  digitalWrite(relayPin4, LOW); //taillight power off
  delay(startupEffectPerLightDelay); //wait
  Serial.println("StartUp Effect/Light Loop Done"); //Debug
}

//########################## Normal Mode #################################

void normalState(){
   Serial.println("Normal Mode Started"); //Debug
   digitalWrite(relayPin1, HIGH); //left blinker off
   digitalWrite(relayPin2, HIGH); //right blinker off
   digitalWrite(relayPin3, HIGH); //headlight on N/C
   digitalWrite(relayPin4, HIGH); //taillight power on N/C
   digitalWrite(relayPin5, LOW); //ext led on
   digitalWrite(relayPin6, HIGH); //deactivate taillight (decrease brightness)
   digitalWrite(relayPin7, HIGH); //n/a off
   digitalWrite(relayPin8, HIGH); //n/a off
   normalMode = 1;
   enableBlinkers = 1; //make it so we can use blinkers
}

//############################ All Lights Off ############################

void AllLightsOff(){
   Serial.println("All Lights Off Started");
   if(hazzardsOn==0){ //if hazzards on, then don't interfear and just turn off extra functions
     digitalWrite(relayPin1, HIGH); //left blinker off
     digitalWrite(relayPin2, HIGH); //right blinker off
     digitalWrite(relayPin3, LOW); //headlight off N/C
     digitalWrite(relayPin4, LOW); //taillight power off N/C
     digitalWrite(relayPin6, HIGH); //deactivate taillight (decrease brightness)
     Serial.println("Hazzards On, Ignoring Hazzard Lights");
   }
   digitalWrite(relayPin5, HIGH); //ext led off
   digitalWrite(relayPin7, HIGH); //n/a off
   digitalWrite(relayPin8, HIGH); //n/a off
   Serial.println("Brake Switch Disabled"); //Debug
   normalMode = 0;
   
   if(EnableBlinkersOnAllOffMode==0){
    enableBlinkers = 0;
    Serial.println("Blinkers Disabled, Turn Signals Will No Longer Function"); //Debug
   }else{
    enableBlinkers = 1;
    Serial.println("Blinkers Still Enabled, Turn Signals Will Function As Normal"); //Debug
   }   
}

//############################ Brake Effect ############################

void brakeEffect(){
  
  if(EnableBrakeLightEffect==1){ //check if enabled
    Serial.println("Brake Effect Started"); //Debug
    int brakeLightBlinkCount = 0;
    while( brakeLightBlinkCount<= BrakeLightEffectBlinkCount && digitalRead(switch4Pin) == LOW){
       digitalWrite(relayPin4, HIGH); //Taillight On N/C
       delay(BrakeLightEffectDelay);
       digitalWrite(relayPin4, LOW); //Taillight Off N/C
       delay(BrakeLightEffectDelay);
       brakeLightBlinkCount++;
    }
    delay(BrakeLightEffectDelay);
    Serial.println("Brake Effect Done"); //Debug
  }
  
  digitalWrite(relayPin4, HIGH); //Taillight On, Make sure its on N/C
  digitalWrite(relayPin6, LOW); //Activate Taillight (increse brightness)
}

//###############################################################################################
//########################################## Main Loop ##########################################

void loop() {
  delay(50); //required do not decrease, increase if issues reading switch states
  unsigned long now = millis(); //get ardunio uptime in milliseconds
  
  //########################### Mode Switch ###############################
  
  if(digitalRead(switch1Pin) == LOW && normalMode==0){
    Serial.println("Normal Mode Switch On, Normal Mode"); //Debug
    normalState();
  }else if(digitalRead(switch1Pin) == HIGH && normalMode==1){//all lights off
    Serial.println("Normal Mode Switch Off, All Lights Off"); //Debug
    AllLightsOff();
  }

  //####################### Brake Trigger Switch ##########################
  
  if(digitalRead(switch4Pin) == LOW && brakeOn==0 && normalMode==1){ //braking
    Serial.println("Brake Switch On"); //Debug
    brakeEffect(); //blink lights x times, then solid on
    Serial.println("Brake Increase Brightness");//Debug
    brakeOn = 1;
  }else if(digitalRead(switch4Pin) == HIGH && brakeOn==1){//not braking
     Serial.println("Brake Switch Off"); //Debug
     Serial.println("Brake Decrease Brightness"); //Debug
     digitalWrite(relayPin6, HIGH); //Deactivate taillight (decrease brightness)
     brakeOn = 0;
  }
  
  //############################ Hazzards Switch ############################
  
  //hazzards will work in any mode
  if(digitalRead(switch2Pin)==LOW && digitalRead(switch3Pin)==LOW){
      delay(BlinkerDelay + 100);
      if(hazzardsOn==0){
        Serial.println("Hazzards On: Headlight, Blinkers and Taillight"); //Debug
        hazzardsOn = 1;
      }
      
      digitalWrite(relayPin4, HIGH); //taillight power on N/C
      
      if(digitalRead(relayPin1)==HIGH || digitalRead(relayPin2)==HIGH){
         digitalWrite(relayPin1, LOW); //left on 
         digitalWrite(relayPin2, LOW); //right on
         digitalWrite(relayPin3, HIGH); //headlight on
         digitalWrite(relayPin6, LOW); //activate taillight
         delay(20);
      }else if(digitalRead(relayPin1)==LOW || digitalRead(relayPin2)==LOW){
         digitalWrite(relayPin1, HIGH); //left off
         digitalWrite(relayPin2, HIGH); //right off
         digitalWrite(relayPin3, LOW); //headlight off
         digitalWrite(relayPin6, HIGH); //deactivate taillight
      }
  
  }else if((digitalRead(switch2Pin)==HIGH || digitalRead(switch3Pin)==HIGH) && hazzardsOn==1){
    Serial.println("Hazzards Off"); //Debug
    hazzardsOn = 0;
    
    if(normalMode==1){ //keep taillight and headlight on if normal mode
      normalState();
    }else{ // turn taillight and headlight off if hazzards off and not in normal mode
      AllLightsOff();
    }
  }

  //########################## Left Blinker  Switch ############################
  
  if(digitalRead(switch2Pin)==LOW && enableBlinkers==1 && hazzardsOn==0){
      delay(BlinkerDelay + 100);
      if(leftBlinkerOn==0){
        leftBlinkerOn = 1;
        Serial.println("Left Blinker On"); //Debug
      }
      //toggle blinker
      digitalWrite(relayPin1, !digitalRead(relayPin1));
  
  }else if(leftBlinkerOn==1){
    Serial.println("Left Blinker Off"); //Debug
    digitalWrite(relayPin1, HIGH); //off
    leftBlinkerOn = 0;
  }

  //########################## Right Blinker Switch ############################
  
  if(digitalRead(switch3Pin)==LOW && enableBlinkers==1 && hazzardsOn==0){
    delay(BlinkerDelay + 100);
    if(rightBlinkerOn==0){
      rightBlinkerOn = 1;
      Serial.println("Right Blinker On"); //Debug
    }
    //toggle blinker
    digitalWrite(relayPin2, !digitalRead(relayPin2));
  
  }else if(rightBlinkerOn==1){
     Serial.println("Right Blinker Off"); //Debug
     digitalWrite(relayPin2, HIGH); //off
     rightBlinkerOn = 0;
  }

  //######################## Fake Secuirty Light Timer ########################
  
  //Security light only works in normal mode
  if((now - securityLedTime > securityLightInterval) && normalMode==1){
    securityLedTime = now; //set last blink time
    //Serial.println("Security Light Just Blinked"); //Debug
    //digitalWrite(securityLightPin, HIGH); //led on
    //delay(300); //wait
    //digitalWrite(securityLightPin, LOW); //led off
  }
  
}



