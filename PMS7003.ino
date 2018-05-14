#include <SoftwareSerial.h>

#define START_1 0x42
#define START_2 0x4d

int PIN_GREEN_Rx = 9;
int PIN_PURPLE_Tx = 10;
SoftwareSerial myPMS7003(PIN_GREEN_Rx, PIN_PURPLE_Tx); 

void setup() {
  Serial.begin(9600);
  myPMS7003.begin(9600);
}

void loop() {
   readPM25();
   delay(1000);
}

void readPM25()
{
  unsigned char pm25L = 0x00;
  unsigned char pm25H = 0x00;
  unsigned char chrData[12];
  int count = myPMS7003.available();

  while(myPMS7003.available())
  {
    if(myPMS7003.read() == START_1 && myPMS7003.read() == START_2)
    {
      Serial.println("Find Start-----------------");
      for(int i = 0; i < 12; i++)
      {
        chrData[i] = myPMS7003.read();
        Serial.print(String(chrData[i], HEX));
        Serial.print("-");
        if(i == 10)
          pm25H = chrData[i];//high byte
        if(i == 11)
          pm25L = chrData[i];//low byte
      }
    }
    //這個baeak要看情況開啓 如果不會有錯誤數值就可以開
    //break;
  }
  
  uint16_t pm = pm25H<<8 | pm25L;
  if(pm <= 0)
    return;
    
  Serial.println("");
  Serial.print("============= PM:");
  Serial.print(pm, DEC);
  Serial.println(" ================");
  Serial.println("");
}
