#include <LiquidCrystal_I2C.h>

// Pin sensor LM35 dan LED
#define lm35 25
#define ledRed 33
#define ledGreen 32

// Konfigurasi LCD
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

void setup() {
  // Konfigurasi pin
  pinMode(lm35, INPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);

  // Serial untuk debugging
  Serial.begin(9600);

  // Inisialisasi LCD
  lcd.init();
  lcd.backlight();
}

void loop() {
  // Membaca nilai analog dari LM35
  float nilaiADC = analogRead(lm35);
  float nilaiDAC = ((nilaiADC / 4095) * 5.0); // Konversi nilai ADC ke tegangan
  float suhuC = nilaiDAC * 100;              // Konversi tegangan ke Celcius

  // Menampilkan suhu di LCD
  lcd.setCursor(0, 0);
  lcd.print("Nilai Suhu: ");
  lcd.print(suhuC, 1);  // Menampilkan suhu dengan 1 angka desimal
  lcd.print(" C   ");

  if (suhuC > 25.0) {
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledGreen, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Status: Bahaya ");

    Serial.println("Status: Bahaya");
    Serial.print("Suhu: ");
    Serial.println(suhuC);
  } else {
    digitalWrite(ledRed, LOW);
    digitalWrite(ledGreen, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Status: Normal ");

    Serial.println("Status: Normal");
    Serial.print("Suhu: ");
    Serial.println(suhuC);
  }

  delay(1000);
}
