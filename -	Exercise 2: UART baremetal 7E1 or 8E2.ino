//HC-SR04 distance measurement 
const int TRIG = 9;
const int ECHO = 8;

void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  Serial.begin(9600);
}

float read_cm() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);          // 10us trig pulse
  digitalWrite(TRIG, LOW);

  unsigned long us = pulseIn(ECHO, HIGH, 30000); 
  if (us == 0) return -1;        // no echo
  return us / 58.0;              // conversion to cm
}

void loop() {
  float cm = read_cm();
  Serial.print("Distance (cm): ");
  Serial.println(cm);
  delay(500);
}
