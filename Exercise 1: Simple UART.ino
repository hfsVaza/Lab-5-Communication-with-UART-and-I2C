//UART 38400, 8N1 
void setup() {
  Serial.begin(38400);   // 8N1=default
}

void loop() {
  Serial.println("Lab 5 EX1: UART 38400 8N1");
  delay(1000);
}
