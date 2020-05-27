//#include <Keyboard.h>
#include "IRremote.h"
#include <LiquidCrystal.h>
LiquidCrystal lcd(2, 3, 4, 5, 6, 7); 
const int RECEIVER = 11;
const int AL_IN = 10;
const int IR_OUT = 5;
const int IR_IN = 3;
const int BR = A0;
const int RED_PIN = 12;
const int GREEN_PIN = 8;
const int DE_PIN = 13;
const int CON_PIN = 9;
IRrecv irrecv(RECEIVER);
decode_results results;
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
int REP [1] = {0xFFFFFFFF};
int last = 0, pMD = 0;
bool alarm, armed, pAl = false;

void alOff() {
  analogWrite(AL_IN,5);
  delay(100);
  analogWrite(AL_IN,0);
  }
void alOn() {
  analogWrite(AL_IN,255);
  delay(30);
  analogWrite(AL_IN,0);
  delay(30);
  analogWrite(AL_IN,255);
  delay(30);
  analogWrite(AL_IN,0);
  }
void alTOn() {
  armed=true;
  Serial.println(armed);
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
void alTOff() {
  armed=false;
  Serial.println(armed);
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
  void gui() {
  if (!alarm) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.write("System ");
  if (armed) lcd.write("armed");
  else lcd.write("not armed");
  Serial.print("Status: ");
  Serial.println(armed);
  
  lcd.setCursor(0,1);
  
  if (digitalRead(DE_PIN)) lcd.write("Motion detected");
  else lcd.write("No motion");
  //(digitalRead(13)==1) 
  } else {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.write("Alarm Triggered");
    lcd.setCursor(0,1);
    lcd.write("Stop to cancel");
    }
  
  }
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
  else {Serial.println(results.value);}
  //Keyboard.write("shutdown /s /f /t 0");
  





} 
static boolean check(int arr [], int check) {
  for (int i = 0; i<sizeof(arr);i++) {
    if (arr[i]==check) {
      last = arr[i];
      return true;
      }
    }
  return false;
  }
void setup(){   
  pinMode(BR, INPUT); 
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(IR_OUT, OUTPUT);
  pinMode(IR_IN, INPUT);
  pinMode(DE_PIN, INPUT);
  pinMode(CON_PIN, OUTPUT);
  int base = analogRead(BR);
  digitalWrite(GREEN_PIN, HIGH);
  irrecv.enableIRIn();
  Serial.begin(9600);
  Serial.println("Security System"); 
  //Keyboard.begin();
  lcd.begin(16, 2);
  analogWrite(CON_PIN, 100);
}
void loop(){
  
  //if (pAl != armed || pMD != digitalRead(DE_PIN)) {
  gui();
  //pAl = armed;
  //pMD = digitalRead(DE_PIN);
  //}
  if (digitalRead(DE_PIN)==1 && armed) {
    alarm = true;
    analogWrite(10,255);
    }
  if (alarm == false) {
    delay(300);
  } else if (alarm == false){
    delay(60);  
  }
  
  if (irrecv.decode(&results))   
  {
    tIR(); 
    delay(50);                 
    irrecv.resume();            
  }  
}
