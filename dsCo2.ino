#include <SoftwareSerial.h>

SoftwareSerial co2Serial(9, 8);
#define START_1 0x42
#define START_2 0x4d

void setup() {
  Serial.begin(9600);
  co2Serial.begin(9600);
}

void loop() {

  unsigned char co2L = 0x00;
  unsigned char co2H = 0x00;
  byte request[] = {0x42, 0x4d, 0xe3, 0x00, 0x00, 0x01, 0x72};
  co2Serial.write(request, 7);
  unsigned char chrData[12];
  delay(500);  
  while(co2Serial.available())
  {
    if(co2Serial.read() == START_1 && co2Serial.read() == START_2)
    {
      Serial.println("Find Start-----------------");
      for(int i = 0; i < 12; i++)
      {
        chrData[i] = co2Serial.read();
        Serial.print(String(chrData[i], HEX));
        Serial.print("-");
        
        if(i == 2)
          co2H = chrData[i];//high byte
        if(i == 3)
          co2L = chrData[i];//low byte
      }
    }
    //這個baeak要看情況開啓 如果不會有錯誤數值就可以開
    //break;
  }
  uint16_t pm = co2H<<8 | co2L;
  if(pm <= 0)
    return;
    
  Serial.println("");
  Serial.print("============= CO2:");
  Serial.print(pm, DEC);
  Serial.println(" ================");
  Serial.println("");
  delay(1000);
}
