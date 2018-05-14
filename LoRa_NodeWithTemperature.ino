#include "DHT.h"
#define DHTPIN A0     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 11

#include <SoftwareSerial.h>
SoftwareSerial mLoraPort(2, 3); // RX, TX
int mM0 = 4;
int mM1 = 5;

DHT dht(DHTPIN, DHTTYPE);
void setup() {
  dht.begin();
  
  // Open serial communications and wait for port to open:
//  Serial.begin(9600);
//  while (!Serial) {
//    ;
//  }
//  Serial.println("Serial Ready");

  pinMode(mM0, OUTPUT);
  pinMode(mM1, OUTPUT);
  
  digitalWrite(mM0, LOW);
  digitalWrite(mM1, LOW);
  
  // set the data rate for the SoftwareSerial port
  mLoraPort.begin(9600);
  mLoraPort.println("Hello world");
}

void loop() { // run over and over
  // Wait a few seconds between measurements.
  delay(10000);
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("濕度: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("溫度: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print("體感溫度: ");
  Serial.print(hic);
  Serial.println(" *C ");
  mLoraPort.print("H");
  mLoraPort.print(h);
  mLoraPort.print("i");
  mLoraPort.print(t);
  mLoraPort.println("C");
//  while (mLoraPort.available()) {
//    char readChar = mLoraPort.read();
//    Serial.write(readChar);
//  }

//  while (Serial.available()) {
//    mLoraPort.write(Serial.read());
//  }
  delay(100);
}
