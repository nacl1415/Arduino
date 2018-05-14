
/*
  Software serial multple serial test
 通透範例程式，讓LORA 模組通過軟體序列埠能直接轉到arduion 硬體的序列埠
 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.

 The circuit:
 * RX is digital pin 10 (connect to TX of other device)
 * TX is digital pin 11 (connect to RX of other device)

 */
#include <SoftwareSerial.h>

SoftwareSerial mLoraPort(10, 11); // RX, TX
int mM0 = 12;
int mM1 = 13;
int mAUX = 2;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  Serial.println("Serial Ready");


  pinMode(mAUX, INPUT);
  
  pinMode(mM0, OUTPUT);
  pinMode(mM1, OUTPUT);
  
  digitalWrite(mM0, LOW);
  digitalWrite(mM1, LOW);
  
  // set the data rate for the SoftwareSerial port
  mLoraPort.begin(9600);
  mLoraPort.println("Hello, world?");
}

void loop() { // run over and over

  //int aux = digitalRead(mAUX);
  while (mLoraPort.available()) {
    int read = mLoraPort.read();
//    read = read + 1;
//    mLoraPort.write(read);
    Serial.write(read);
  }
  
  while (Serial.available()) {
    mLoraPort.write(Serial.read());
  }
  
  //Serial.println(String("aux=") + digitalRead(mAUX));
  
  delay(100);
}
