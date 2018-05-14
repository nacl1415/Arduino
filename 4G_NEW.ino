#include <SoftwareSerial.h>

#define RXPIN 2
#define TXPIN 3
SoftwareSerial mSS(RXPIN, TXPIN);

void setup() {
  Serial.begin(9600);
  mSS.begin(115200);
  delay(2000);
  Serial.println("Ready!!");
  mSS.print("H77.88i66.88C");
}

void loop() {
  
  while (Serial.available()) {  
    mSS.write(Serial.read());    
  }
  while(mSS.available()){
    Serial.write(mSS.read());
  }
  delay(100);
}
