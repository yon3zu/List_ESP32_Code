#include <LiquidCrystal_I2C.h>
#define lm35 25

int lcdColumns = 16;
int lcdRows = 2;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

void setup() {
  pinMode(lm35, INPUT);
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
}

void loop() {
  float nilaiADC = analogRead(lm35);
  float nilaiDAC = ((nilaiADC / 4095) * 5.0);
  float suhuC = nilaiDAC * 100;  // Konversi tegangan ke derajat Celcius

  lcd.setCursor(0,0);
  lcd.print("Nilai Suhu: ");
  lcd.print(suhuC);

  delay(1000);
}
