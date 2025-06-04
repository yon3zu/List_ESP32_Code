#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// === Pin Definitions ===
#define DHTPIN 16
#define DHTTYPE DHT22
#define TDS_PIN 15
#define PH_PIN 34
#define IR_PIN 35

#define RELAY_POMPA1 32
#define RELAY_POMPA2 33
#define RELAY_POMPA4 12
#define RELAY_POMPA5 14
#define RELAY_VALVE 27

// === WiFi & Firebase ===
#define WIFI_SSID "ap-labiot"
#define WIFI_PASSWORD "bumigora123"
#define API_KEY "AIzaSyD8Ybncqp1nKaTmmiM4kYOuE9G220lbcvU"
#define DATABASE_URL "https://dht22-server-193ae-default-rtdb.firebaseio.com/"
#define USER_EMAIL "user@dht22.com"
#define USER_PASSWORD "iotmakerntb"

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 20, 4);

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

float suhuC = 0.0;
float humidity = 0.0;

void setup() {
  Serial.begin(115200);
  dht.begin();
  lcd.init();
  lcd.backlight();

  // === WiFi Setup ===
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  lcd.setCursor(0, 0);
  lcd.print("Connect WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    lcd.print(".");
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());

  // === Firebase Setup ===
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  delay(500);
  lcd.clear();

  // === PinMode Setup ===
  pinMode(IR_PIN, INPUT);
  pinMode(RELAY_POMPA1, OUTPUT);
  pinMode(RELAY_POMPA2, OUTPUT);
  pinMode(RELAY_POMPA4, OUTPUT);
  pinMode(RELAY_POMPA5, OUTPUT);
  pinMode(RELAY_VALVE, OUTPUT);

  digitalWrite(RELAY_POMPA1, LOW);
  digitalWrite(RELAY_POMPA2, LOW);
  digitalWrite(RELAY_POMPA4, LOW);
  digitalWrite(RELAY_POMPA5, LOW);
  digitalWrite(RELAY_VALVE, LOW);
}

void loop() {
  // === Baca Sensor ===
  suhuC = dht.readTemperature();
  humidity = dht.readHumidity();

  int tdsADC = analogRead(TDS_PIN);
  float tdsVoltage = tdsADC * (3.3 / 4095.0);
  float tdsValue = (133.42 * tdsVoltage * tdsVoltage * tdsVoltage
                  - 255.86 * tdsVoltage * tdsVoltage
                  + 857.39 * tdsVoltage) * 0.5;
  if (tdsValue < 0) tdsValue = 0;

  int phADC = analogRead(PH_PIN);
  float phVoltage = (float)phADC / 4095.0 * 3.3;
  float phValue = 7 + ((2.5 - phVoltage) / 0.18);

  int irStatus = digitalRead(IR_PIN); // HIGH: Ter, LOW: terhalang

  // === Log ke Serial Monitor ===
  Serial.print("Suhu: "); Serial.print(suhuC);
  Serial.print(" | Kelembaban: "); Serial.print(humidity);
  Serial.print(" | TDS: "); Serial.print(tdsValue);
  Serial.print(" | pH: "); Serial.print(phValue);
  Serial.print(" | IR: "); Serial.println(irStatus == LOW ? "Tidak Terhalang" : "Terhalang");

  // === Otomatisasi Pompa berdasarkan IR ===
  if (irStatus == LOW) {
  digitalWrite(RELAY_POMPA1, HIGH); // Tidak terhalang → Pompa ON
  } else {
    digitalWrite(RELAY_POMPA1, LOW);  // Terhalang → Pompa OFF
  }

  // === Kirim ke Firebase ===
  if (Firebase.ready()) {
    Serial.println("Firebase Ready!");

    if (!Firebase.RTDB.setFloat(&fbdo, "/sensor/suhu", suhuC)) {
      Serial.print("Error Suhu: ");
      Serial.println(fbdo.errorReason());
    }
    if (!Firebase.RTDB.setFloat(&fbdo, "/sensor/kelembaban", humidity)) {
      Serial.print("Error Kelembaban: ");
      Serial.println(fbdo.errorReason());
    }
    if (!Firebase.RTDB.setFloat(&fbdo, "/sensor/tds", tdsValue)) {
      Serial.print("Error TDS: ");
      Serial.println(fbdo.errorReason());
    }
    if (!Firebase.RTDB.setFloat(&fbdo, "/sensor/ph", phValue)) {
      Serial.print("Error pH: ");
      Serial.println(fbdo.errorReason());
    }
    if (!Firebase.RTDB.setInt(&fbdo, "/sensor/ir", irStatus)) {
      Serial.print("Error IR: ");
      Serial.println(fbdo.errorReason());
    }

    // Relay Control
    if (Firebase.RTDB.getInt(&fbdo, "/relay/relay2")) {
      digitalWrite(RELAY_POMPA2, fbdo.intData() == 1 ? HIGH : LOW);
    }
    if (Firebase.RTDB.getInt(&fbdo, "/relay/relay4")) {
      digitalWrite(RELAY_POMPA4, fbdo.intData() == 1 ? HIGH : LOW);
    }
    if (Firebase.RTDB.getInt(&fbdo, "/relay/relay5")) {
      digitalWrite(RELAY_POMPA5, fbdo.intData() == 1 ? HIGH : LOW);
    }
    if (Firebase.RTDB.getInt(&fbdo, "/relay/valve")) {
      digitalWrite(RELAY_VALVE, fbdo.intData() == 1 ? HIGH : LOW);
    }

  } else {
    Serial.println("Firebase NOT ready");
    Serial.println(fbdo.errorReason());
  }

  // === LCD Output ===
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TDS:");
  lcd.print(tdsValue, 1);
  lcd.print(" ppm");

  lcd.setCursor(0, 1);
  lcd.print("pH:");
  lcd.print(phValue, 2);
  lcd.print(" Suhu:");
  lcd.print(suhuC, 1);

  lcd.setCursor(0, 2);
  lcd.print("Hum:");
  lcd.print(humidity, 0);
  lcd.print("% IR:");
  lcd.print(irStatus == LOW ? "NO" : "YES");

  lcd.setCursor(0, 3);
  lcd.print("Pompa1:");
  lcd.print(digitalRead(RELAY_POMPA1) == HIGH ? "OFF" : "ON");

  delay(500);
}
