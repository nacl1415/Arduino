
#include <JeeLib.h>
#include "DHT.h"
#include <SoftwareSerial.h>

#define DHTPIN 2//D2 pin is DHT11's singal
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

SoftwareSerial mLoraPort(6, 7); // RX, TX
int mM0 = 4;
int mM1 = 5;

ISR(WDT_vect)
{
  Sleepy::watchdogEvent();
}

int mCount = 0;
int mLedPin = 9;

String mSendStr = "";

void setup() {

  Serial.begin(9600);
  pinMode(mLedPin, OUTPUT);

  //dht11
  dht.begin();
  
  //lora
  pinMode(mM0, OUTPUT);
  pinMode(mM1, OUTPUT);
  
  digitalWrite(mM0, LOW);
  digitalWrite(mM1, LOW);

  mLoraPort.begin(9600);
  mLoraPort.println("Hello, world");
}

void loop() {
  //Serial.println("LED:" + String(mCount));

  //lora wakeup
  digitalWrite(mM0, LOW);
  digitalWrite(mM1, LOW);
  delay(1000);
  sendCoor();
  
  digitalWrite(mLedPin, HIGH);
  delay(10*1000);
  digitalWrite(mLedPin, LOW);

  delay(1000);
  //lora sleep
  digitalWrite(mM0, HIGH);
  digitalWrite(mM1, HIGH);
  delay(100);
  
  Sleepy::loseSomeTime(60*1000);//60 * 60 = 3600 = 1小時
  mCount++;
}

void sendCoor()
{
  mSendStr = "";
  int h = dht.readHumidity();
  int t = dht.readTemperature();
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    //Serial.println("Failed to read from DHT sensor!");
    delay(10000);
    return;
  }

//  float hif = dht.computeHeatIndex(f, h);
//  float hic = dht.computeHeatIndex(t, h, false);

  mSendStr = "H" + String(h) + "i" + String(t) + "C" + String(mCount);
  mLoraPort.print(mSendStr);
//  Serial.print("H");
//  Serial.print(h);
//  Serial.print("i");
//  Serial.print(t);
//  Serial.print("C");
}

