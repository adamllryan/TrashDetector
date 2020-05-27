const int DE_PIN = 3;
void setup() {
  pinMode(DE_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
Serial.println(digitalRead(DE_PIN));
}
