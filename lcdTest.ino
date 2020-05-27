#include <LiquidCrystal.h> // includes the LiquidCrystal Library 
LiquidCrystal lcd(2, 3, 4, 5, 6, 7); // Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7)

void setup() {
  lcd.begin(16, 2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display }
  pinMode(9, OUTPUT);
  analogWrite(9, 100);
}
void gui_default(bool armed, bool motion) {
  if (true) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.write("System ");
  if (armed) lcd.write("armed");
  else lcd.write("not armed");
  
  lcd.setCursor(0,1);
  
  if (motion) lcd.write("Motion detected");
  else lcd.write("No motion");
  //(digitalRead(13)==1) 
  } else {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.write("Alarm Triggered");
    lcd.setCursor(0,1);
    lcd.write("Stop to cancel");
    }
  delay(250);
  }




void loop() {
  gui_default(false, true);
  gui_default(true, true);
  gui_default(false, false);
  gui_default(true, false);
}
