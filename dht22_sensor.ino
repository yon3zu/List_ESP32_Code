#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#define RELAY_PIN 4
#define RELAY_PIN1 16
#define RELAY_PIN2 17
#define RELAY_PIN3 5

#define WIFI_SSID "26"
#define WIFI_PASSWORD "12345678"

#define API_KEY "AIzaSyD8Ybncqp1nKaTmmiM4kYOuE9G220lbcvU"  // Firebase Web API Key
#define DATABASE_URL "https://dht22-server-193ae-default-rtdb.firebaseio.com/"  // Firebase URL

#define USER_EMAIL "user@dht22.com"
#define USER_PASSWORD "iotmakerntb"

#define DHTPIN 25
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

float suhuC = 0.0;
float humidity = 0.0;

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.init();
  lcd.backlight();

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

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  delay(2000);
  lcd.clear();

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(RELAY_PIN1, OUTPUT);
  pinMode(RELAY_PIN2, OUTPUT);
  pinMode(RELAY_PIN3, OUTPUT);

  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(RELAY_PIN1, LOW);
  digitalWrite(RELAY_PIN2, LOW);
  digitalWrite(RELAY_PIN3, LOW);
}

void loop() {
  suhuC = dht.readTemperature();
  humidity = dht.readHumidity();

  lcd.setCursor(0, 0);
  lcd.print("Suhu: ");
  lcd.print(suhuC);
  lcd.print("C ");

  lcd.setCursor(0, 1);
  lcd.print("Lembab: ");
  lcd.print(humidity);
  lcd.print("% ");

  if (Firebase.ready()) {
    if (Firebase.RTDB.setFloat(&fbdo, "/sensor/suhu", suhuC)) {
      Serial.println("Suhu dikirim!");
    } else {
      Serial.print("Gagal kirim suhu: ");
      Serial.println(fbdo.errorReason());
    }

    if (Firebase.RTDB.setFloat(&fbdo, "/sensor/kelembaban", humidity)) {
      Serial.println("Kelembaban dikirim!");
    } else {
      Serial.print("Gagal kirim kelembaban: ");
      Serial.println(fbdo.errorReason());
    }

    // Baca status relay 1-4 dari Firebase (0 atau 1)
    if (Firebase.RTDB.getInt(&fbdo, "/sensor/relay1"))
      digitalWrite(RELAY_PIN, fbdo.intData() == 1 ? HIGH : LOW);

    if (Firebase.RTDB.getInt(&fbdo, "/relay/relay2"))
      digitalWrite(RELAY_PIN1, fbdo.intData() == 1 ? HIGH : LOW);

    if (Firebase.RTDB.getInt(&fbdo, "/relay/relay3"))
      digitalWrite(RELAY_PIN2, fbdo.intData() == 1 ? HIGH : LOW);

    if (Firebase.RTDB.getInt(&fbdo, "/relay/relay4"))
      digitalWrite(RELAY_PIN3, fbdo.intData() == 1 ? HIGH : LOW);
    }

  delay(2000);
}