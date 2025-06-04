#define myled 2

void setup() {
  // put your setup code here, to run once:
  pinMode(myled, OUTPUT);
  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial1.available()){
    char inchar = Serial1.read();
    Serial.println(inchar);
    if (inchar == '1'){
      digitalWrite(myled, HIGH);
      Serial.println("LED ON");
    } else{
      digitalWrite(myled, LOW);
      Serial.println("LED OFF");
    }
  }
}

// jika  1 read jika 0 write

// tugas merangkum tentang i2c / inter integrated circuit