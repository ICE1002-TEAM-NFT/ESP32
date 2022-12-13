int r = 0, g = 0, b = 0;

void setup() {
  pinMode(13, INPUT);
  pinMode(12, INPUT);
  pinMode(14, INPUT);
}

void loop() {
  if (digitalRead(13) == HIGH)  {
    r = 0
    b = 255
    g = 0 
 }

 if (digitalRead(12) == HIGH)  {
    r = 0
    b = 0
    g = 255
 }

 if (digitalRead(14) == HIGH)  {
    r = 255
    b = 0
    g = 0
 }
}
