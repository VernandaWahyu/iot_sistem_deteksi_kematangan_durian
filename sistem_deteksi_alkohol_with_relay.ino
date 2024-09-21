#include <Ultrasonic.h> //Mengambil Library Sensor Ultrasonik
#include <LiquidCrystal_I2C.h> //Mengambil Library Lcd L2C

#define RELAY_PIN D5 //Mendefinisikan Pin Relay

const int triggerPin = D6; // Menggunakan pin D6 untuk trigger HC-SR04
const int echoPin = D7;    // Menggunakan pin D7 untuk echo HC-SR04
Ultrasonic ultrasonic(triggerPin, echoPin); //Inisialisasi Pin Triggers dan Echo

const int sensorPin = A0;  // Pin analog untuk sensor gas TGS2620
int sensorValue = -3;
int lowestValue = 1023; // Nilai terendah yang mungkin

LiquidCrystal_I2C lcd(0x27, 16, 2); //Inisialisasi L2C Yang Digunakan

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);//Output Relay
  lcd.begin(16, 2); //Output LCD 16 Huruf 2 Kolom
  lcd.init();
}

void loop() {
  long distance = ultrasonic.read(); //Membaca Sensor Ultrasonik
  Serial.print("Jarak: "); //Menampilkan Pada Serial Monitor Sensor Ultrasonik
  Serial.print(distance); 
  Serial.println(" cm");

  // int sensorValue = analogRead(sensorPin); //Membaca Sensor TGS2620
  // // Normalisasi nilai sensor dan kurangi sebesar 19%
  // Serial.print("Nilai sensor gas: ");//Menampilkan Pada Serial Monitor Sensor TGS2620
  // Serial.println(sensorValue);

  sensorValue = analogRead(sensorPin);
  
  // Memperbarui nilai terendah jika nilai saat ini lebih rendah
  if (sensorValue < lowestValue) {
    lowestValue = sensorValue;
  }
  
  // Normalisasi nilai sensor
  int calibratedValue = sensorValue - lowestValue;
  
  // Menampilkan nilai sensor yang dikalibrasi pada Serial Monitor
  Serial.print("Nilai sensor gas (dikalibrasi): ");
  Serial.println(calibratedValue);

  //Menampilkan Pada L2C
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Alkohol:");
  lcd.setCursor(8, 0);
  lcd.print(calibratedValue);
  lcd.setCursor(10, 0);
  lcd.print("%");
  lcd.setCursor(0, 1);
  

  if (distance < 20) { //Mendefinisikan jika jarak < 20 maka Kipas akan Menyala
    digitalWrite(RELAY_PIN, LOW); //Menyalakan Relay dengan variabel high
    Serial.println("Fan:ON"); //Menampilkan Pada Serial Monitor 
    lcd.setCursor(0, 1); 
    lcd.print("DurianAda Fan:ON"); //Menampilkan Pada L2C
  } else {
    digitalWrite(RELAY_PIN, HIGH); //Mematikan Relay dengan variabel low
    Serial.println("Fan:OFF"); //Menampilkan Pada Serial Monitor 
    lcd.print("Tidak Ada Durian"); //Menampilkan Pada L2C
  }
  
  delay(1000);
}
