#define lm35 A0

void setup() {
  pinMode(lm35, INPUT);
  Serial.begin(9600);
}

void loop() {
  float nilaiADC = analogRead(lm35);
  float nilaiDAC = ((nilaiADC / 1023) * 5.0);
  float suhuC = nilaiDAC * 100;  // Konversi tegangan ke derajat Celcius

  Serial.print("ADC : ");
  Serial.println(nilaiADC);
  Serial.print("DAC (V) : ");
  Serial.println(nilaiDAC);
  Serial.print("Suhu (°C) : ");
  Serial.println(suhuC);

  delay(1000);
}
