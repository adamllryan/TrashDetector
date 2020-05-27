#include "IRremote.h"
#include <LiquidCrystal.h>
// Pins for the (IR Receiver, Buzzer Input, Red LED, Green LED, Motion Detector Receiver, and LDC Contrast)
const int RECEIVER = 11, AL_IN = 10, RED_PIN = 12, GREEN_PIN = 8, DE_PIN = 13, CON_PIN = 9;
LiquidCrystal lcd(2,3,4,5,6,7); 
IRrecv irrecv(RECEIVER);
decode_results results;

//Config

//Set tick rate (tick/second)
const int tickrate = 16;

//Enable/Disable Debug Box Verbose Output
bool dMode = true;


// Arrays for each button on the remote because it likes to send out different codes with each button, so check() checks if a button was pressed
int PWR [13] = {0xFFA25D,3810010651,255,2010369711,1809563490,1538145539,242624182,2539775107,1047077,1482940742, 2546571346, 2213417950, 3634441721};
int STP [2] = {0xFFE21D,4001918335};
int VUP [2] = {0xFF629D,5316027};
int FBK [2] = {0xFF22DD,1386468383};
int POS [2] = {0xFF02FD,3622325019};
int FFW [2] = {0xFFC23D,553536955};
int DWN [2] = {0xFFE01F,4034314555};
int VDN [2] = {0xFFA857,2747854299};
int UPP [2] = {0xFF906F,3855596927};
int EQU [2] = {0xFF9867,2538093563};
int STR [2] = {0xFFB04F,4039382595};
int ZRO [2] = {0xFF6897,3238126971};
int ONE [2] = {0xFF30CF,2534850111};
int TWO [2] = {0xFF18E7,1033561079};
int TRE [2] = {0xFF7A85,1635910171};
int FOR [2] = {0xFF10EF,2351064443};
int FIV [2] = {0xFF38C7,1217346747};
int SIX [2] = {0xFF5AA5,71952287};
int SVN [2] = {0xFF42BD,851901943};
int ATE [2] = {0xFF4AB5,465573243};
int NIN [2] = {0xFF52AD,1053031451};
// Outputs this if a key is being held down
int REP [1] = {0xFFFFFFFF};
// Last is the last code sent, used to check if a key is being held down
int last = 0;
// Alarm is if the alarm has been set off
// Armed is if the alarm may be set off
bool alarm, armed;


// Tone played when not armed
void alOff() {
  analogWrite(AL_IN,5);
  delay(100);
  analogWrite(AL_IN,0);
  }

  
// Tone played when armed
void alOn() {
  analogWrite(AL_IN,255);
  delay(30);
  analogWrite(AL_IN,0);
  delay(30);
  analogWrite(AL_IN,255);
  delay(30);
  analogWrite(AL_IN,0);
  }

  
// Tone played when arming
void alTOn() {
  armed=true;
  analogWrite(10,2);
  delay(50);
  analogWrite(10,0);
  delay(50);
  analogWrite(10,100);
  delay(50);
  analogWrite(10,0);
  delay(50);
  analogWrite(10,200);
  delay(50);
  analogWrite(10,0);
  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, LOW);
  }

  
// Tone playing when disarming
void alTOff() {
  armed=false;
  analogWrite(10,200);
  delay(50);
  analogWrite(10,0);
  delay(50);
  analogWrite(10,100);
  delay(50);
  analogWrite(10,0);
  delay(50);
  analogWrite(10,2);
  delay(50);
  analogWrite(10,0);
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, HIGH);
  }

  
//LCD Control, outputs if is armed and if motion is detected
void gui() {
// Top row, says if armed or not
  if (!alarm) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.write("System ");
  if (armed) lcd.write("armed");
  else lcd.write("not armed");

  lcd.setCursor(0,1);
// Bottom row, says if motion is detected or not
  if (digitalRead(DE_PIN)) lcd.write("Motion detected");
  else lcd.write("No motion");
   
  } else {
// Override if alarm is triggered
// Displays that alarm is triggered 
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.write("Alarm Triggered");
    lcd.setCursor(0,1);
    lcd.write("Stop to cancel");
    }
  
  }

  
// When a code is received, checks which button was pressed from the arrays of codes
void tIR() {

       if (check(PWR,results.value)==true) {if (armed) alTOff(); else alTOn();}
  else if (check(STP,results.value)==true) {Serial.println("FUNC/STOP ");alarm=false;analogWrite(10,0);}
  else if (check(VUP,results.value)==true) {Serial.println("VOL+ ");}
  else if (check(FBK,results.value)==true) {Serial.println("FAST BACK ");}
  else if (check(POS,results.value)==true) {Serial.println("PAUSE ");}
  else if (check(FFW,results.value)==true) {Serial.println("FAST FORWARD "); }
  else if (check(DWN,results.value)==true) {Serial.println("DOWN ");}
  else if (check(VDN,results.value)==true) {Serial.println("VOL- ");}
  else if (check(UPP,results.value)==true) {Serial.println("UP ");}
  else if (check(EQU,results.value)==true) {Serial.println("EQ ");if (!armed) alOff(); else alOn();}
  else if (check(STR,results.value)==true) {Serial.println("ST/REPT ");}
  else if (check(ZRO,results.value)==true) {Serial.println("0 ");}
  else if (check(ONE,results.value)==true) {}
  else if (check(TWO,results.value)==true) {}
  else if (check(TRE,results.value)==true) {}
  else if (check(FOR,results.value)==true) {}
  else if (check(FIV,results.value)==true) {}
  else if (check(SIX,results.value)==true) {}
  else if (check(SVN,results.value)==true) {Serial.println("7 ");}
  else if (check(ATE,results.value)==true) {Serial.println("8 ");}
  else if (check(NIN,results.value)==true) {Serial.println("9 ");}
  else if (check(REP,results.value)==true) {Serial.println("HOLDING");}
// If an unknown code is sent, so I can account for that
  else {Serial.println(results.value);}
}


// Check function returns if the code matches the key
static boolean check(int arr [], int check) {
  for (int i = 0; i<sizeof(arr);i++) {
    if (arr[i]==check) {
      last = arr[i];
      return true;
    }
  }
  return false;
}

// Debug mode
void debug() {
  Serial.println("\n\n\n\n\n\n\tDebug");
  Serial.println("---------------------------------------------");
  Serial.print("\tarmed: \t"); if (armed) Serial.println("true\n"); else Serial.println("false\n");
  Serial.print("\talarm: \t");if (alarm) Serial.println("true\n"); else Serial.println("false\n");
  Serial.print("\tmotion: ");if (digitalRead(DE_PIN)) Serial.println("true\n"); else Serial.println("false\n");
  Serial.print("");Serial.println();
  Serial.print("");Serial.println();
  Serial.print("");Serial.println();
  Serial.print("");Serial.println();
  Serial.print("");Serial.println();
  Serial.print("");Serial.println();
  Serial.print("---------------------------------------------");
  }


// Set up pinModes and starts serial output
void setup(){   
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(DE_PIN, INPUT);
  pinMode(CON_PIN, OUTPUT);
  // Defaults to disabled
  digitalWrite(GREEN_PIN, HIGH);
  irrecv.enableIRIn();
  Serial.begin(9600);
  Serial.println("Trasg Detectore lol"); 
  // Sets the display size to 2x16
  lcd.begin(16, 2);
  // Sets default contrast of display to 100/255
  analogWrite(CON_PIN, 100);
}


void loop(){
  // Display
  gui();
  // Debug
  if (dMode)debug();
  //Check if armed and if the alarm should be set off
  if (digitalRead(DE_PIN)==1 && armed) {
    alarm = true;
    //Buzzer
    analogWrite(AL_IN,255);
    }
  // Check if IR Receiver got anything
  if (irrecv.decode(&results))   
  {
    tIR(); 
    delay(50);         
    irrecv.resume();            
  }
  // Tickrate
  delay(1000/tickrate);
}
