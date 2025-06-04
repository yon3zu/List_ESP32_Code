#define LDR A0

void setup() {
  pinMode(LDR, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  float dataLDR = analogRead(LDR);
  float TeganganLDR = ((dataLDR/1023) * 5);

  Serial.print("ADC LDR : ");
  Serial.println(dataLDR);

  Serial.print("Vout LDR: ");
  Serial.println(TeganganLDR);

  delay(1000);

  Serial.println("-----------------");
}
