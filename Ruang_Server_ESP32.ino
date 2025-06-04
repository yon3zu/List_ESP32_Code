#include <Arduino.h>

// Pin definisi
const int PIN_SENSOR_SUHU = 13; // Pin ADC ESP32 untuk sensor LM35
const int PIN_LED_HIJAU = 12; // Pin untuk LED hijau
const int PIN_LED_KUNING = 14; // Pin untuk LED kuning

// Konstanta
const float VREF = 3.3; // Tegangan referensi ADC ESP32
const int RESOLUSI_ADC = 4095; // Resolusi ADC ESP32
const float AMBANG_SUHU = 29.0; // Ambang suhu dalam derajat Celsius

void setup() {
  // Inisialisasi pin LED
  pinMode(PIN_LED_HIJAU, OUTPUT);
  pinMode(PIN_LED_KUNING, OUTPUT);

  // Mulai komunikasi serial untuk debugging
  Serial.begin(115200);
}

void loop() {
  // Membaca nilai analog dari sensor suhu (LM35)
  int nilaiAnalog = analogRead(PIN_SENSOR_SUHU);

  // Menghitung suhu dalam derajat Celsius (LM35: 10 mV per derajat Celsius)
  // float suhu = tegangan * 100;

  float nilai_dac = (nilaiAnalog * VREF) / 4095;
  float suhu = nilai_dac * 100;

  // Menampilkan suhu pada serial monitor
  Serial.print("Suhu: ");
  Serial.print(suhu);
  Serial.println(" °C");

  // Logika kontrol LED berdasarkan suhu
  if (suhu < AMBANG_SUHU) {
    digitalWrite(PIN_LED_HIJAU, HIGH); // LED hijau menyala
    digitalWrite(PIN_LED_KUNING, LOW); // LED kuning mati
  } else {
    digitalWrite(PIN_LED_HIJAU, LOW);  // LED hijau mati
    digitalWrite(PIN_LED_KUNING, HIGH); // LED kuning menyala
  }

  // Tunggu 1 detik sebelum pembacaan berikutnya
  delay(1000);
}
