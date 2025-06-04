#define pinIR 2
#define ledKuning 3  // Pin untuk LED kuning
#define ledHijau 4   // Pin untuk LED hijau


void setup() {
  pinMode(pinIR, INPUT);
  pinMode(ledKuning, OUTPUT);
  pinMode(ledHijau, OUTPUT);
  
  Serial.begin(9600); // Inisialisasi serial monitor
}

void loop() {
  int dataIR = digitalRead(pinIR); // Membaca data dari sensor IR
  
  Serial.print("Data sensor IR: ");
  Serial.println(dataIR); // Menampilkan data sensor IR
  
  if (dataIR == HIGH) { // Jika permukaan air terdeteksi
    digitalWrite(ledHijau, HIGH);  // LED hijau menyala
    digitalWrite(ledKuning, LOW);  // LED kuning mati
  } else { // Jika permukaan air tidak terdeteksi
    digitalWrite(ledHijau, LOW);   // LED hijau mati
    digitalWrite(ledKuning, HIGH); // LED kuning menyala
  }
  
  delay(1000); // Delay 1 detik
}
