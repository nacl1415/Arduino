#include <SoftwareSerial.h>

SoftwareSerial co2Serial(A0, A1);
//Reqest Gas concentration command
byte cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79}; 
char response[9]; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  co2Serial.begin(9600);
  delay(2000);
}

void loop() {

  //Pinging CO2, we read, reply and translate intoPPM
  co2Serial.write(cmd,9);
  co2Serial.readBytes(response, 9);
  int responseHigh = (int) response[2];
  int responseLow = (int) response[3];
  int ppm = (256*responseHigh)+responseLow; //公式 ppm = high*256 + Low

  Serial.println("High:" + (String)responseHigh + " Low:" + (String)responseLow);
  Serial.print("PPM ");
  Serial.println(ppm);
  Serial.println("-----------------------------------");

  delay(3000);
 }
//其他
// Calibrate zero point command  (零點校正)
//byte cmd[9] = {0xFF,0x01,0x87,0x00,0x00,0x00,0x00,0x00,0x78};
//another calibrate zero point command, not sure which is correct
//byte cmd[9] = {0xff, 0x87, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf2}; 

//Calibrate span point command (計算斜率校正)
//byte cmd[9] = {0xFF,0x01,0x88,0x07,0xD0,0x00,0x00,0x00,0xA0};
//another calibrate span point command, not sure which is correct
//byte cmd[9] = {0xff, 0x88, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0}; 
