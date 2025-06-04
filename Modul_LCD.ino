#include <LiquidCrystal_I2C.h>

int lcdColumns = 16;
int lcdRows = 2;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

// Definisikan karakter hati
// byte heart[8] = {
//   B00000,
//   B01010,
//   B11111,
//   B11111,
//   B01110,
//   B00100,
//   B00000,
//   B00000
// };

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  // Buat karakter hati
  // lcd.createChar(0, heart);
}

void loop() {

  lcd.setCursor(0, 0);
  lcd.print("Algi");
  // lcd.setCursor(5, 0);
  // lcd.write(byte(0));
  
  // lcd.setCursor(6, 0);
  // lcd.print("Ali");

  delay(500);
  lcd.clear();
}