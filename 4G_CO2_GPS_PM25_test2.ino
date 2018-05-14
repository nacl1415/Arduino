#include <SoftwareSerial.h>
//----4G----

#define RXPIN 2
#define TXPIN 3
SoftwareSerial mSS(RXPIN, TXPIN);

String mPM25;
String mCO2;
String mGPS;

extern uint8_t SmallFont[];//原始文件在库文件的DefaultFonts.c中
extern uint8_t BigFont[];//原始文件在库文件的DefaultFonts.c中
extern uint8_t SevenSegNumFont[];//原始文件在库文件的DefaultFonts.c中

//----GPS----

#define GpsSerial  Serial
#define DebugSerial Serial
int L = 13; //LED指示灯引脚
char M = 0x4d;
char C = 0x43;
char pointer = 0x2c;
float mSpeedKm ;
int mCount = 0;
String mSpeed;

//----CO2----
#define         MG_PIN                       (A0)     //define which analog input channel you are going to use
#define         DC_GAIN                      (8.5)   //define the DC gain of amplifier
#define         BOOL_PIN                     (2)
#define         READ_SAMPLE_INTERVAL         (50)    //define how many samples you are going to take in normal operation
#define         READ_SAMPLE_TIMES            (5)     //define the time interval(in milisecond) between each samples in
#define         ZERO_POINT_VOLTAGE           (0.220) //define the output of the sensor in volts when the concentration of CO2 is 400PPM
#define         REACTION_VOLTGAE             (0.020) //define the voltage drop of the sensor when move the sensor from air into 1000ppm CO2
float           CO2Curve[3]  =  {2.602,ZERO_POINT_VOLTAGE,(REACTION_VOLTGAE/(2.602-3))};  

//----PM25----
#define START_1 0x42
#define START_2 0x4d

int PIN_GREEN_Rx = 9;
int PIN_PURPLE_Tx = 10;
SoftwareSerial myPMS7003(PIN_GREEN_Rx, PIN_PURPLE_Tx); 

void setup() {
  Serial.begin(9600);
  mSS.begin(115200);
  myPMS7003.begin(9600);

  //GPS
  GpsSerial.begin(9600);      //定义波特率9600，和我们店铺的GPS模块输出的波特率一致
  DebugSerial.begin(9600);
  DebugSerial.println("Ready!!");

  //CO2
  pinMode(BOOL_PIN, INPUT);                        //set pin to input
  digitalWrite(BOOL_PIN, HIGH);                    //turn on pullup resistors

}

void loop() {
  printGPS();

  Serial.println("Loop END===========================");
  delay(3000);
}

void printGPS () {
  while(GpsSerial.available())
  {
    char cc = GpsSerial.read(); //讀取GPS資料
    delayMicroseconds(1500);
    //Serial.print(cc);
    if(cc == M)  //找M
    {
      if(GpsSerial.read() == C)  //找C
      {
        //Serial.println("FindMC");
        mSpeed = "";
        mCount = 0;
        for(int i = 0; i < 50; i++)
        {
          if(GpsSerial.read() == pointer)
          {
            mCount++;
            //Serial.println("counter="+String(mCount));
            if(mCount == 7)   //資料串的第七個逗點，代表速度的起始點
            {
              for(int k = 0; k < 7; k++)    //取出速度資料
              {
                cc = GpsSerial.read();
                //Serial.print(cc);
                if(cc == pointer)   //速度資料結束
                {
                  break;
                }
                mSpeed = mSpeed + String(cc);     
              }
              break;
            }
          }
        }
      }
    }
  }

  if(!mSpeed.equals(""))
  {
    float mSpeedKm = mSpeed.toFloat() * 1.6093 ;  //英里轉換為公里
//    Serial.println("");
//    Serial.println("Speed=" + String(mSpeedKm));
    
    readPM25();
    printCO2();
//    mSS.print("H18i"+mCO2+"i"+String(mSpeedKm)+"C");
    mSS.print("H"+mPM25+"i"+mCO2+"i"+String(mSpeedKm)+"C");
    Serial.println("H"+mPM25+"i"+mCO2+"i"+String(mSpeedKm)+"C");
  }
  else
  {
    Serial.println("NOSPEED");
  }
  mSpeed = "";
}

//CO2
void printCO2() 
{
    int percentage;
    float volts;
    volts = MGRead(MG_PIN);

    percentage = MGGetPercentage(volts,CO2Curve);

//    Serial.print("CO2:");
    if (percentage == -1) {
//        Serial.println( "<400" );
    } else {
//        Serial.println(percentage);
        mCO2 = percentage;
    }
}

float MGRead(int mg_pin)
{
    int i;
    float v=0;
    for (i=0;i<READ_SAMPLE_TIMES;i++) {
        v += analogRead(mg_pin);
        delay(READ_SAMPLE_INTERVAL);
    }
    v = (v/READ_SAMPLE_TIMES) *5/1024 ;
    return v;  
}

int  MGGetPercentage(float volts, float *pcurve)
{
   if ((volts/DC_GAIN )>=ZERO_POINT_VOLTAGE) {
      return -1;
   } else {
      return pow(10, ((volts/DC_GAIN)-pcurve[1])/pcurve[2]+pcurve[0]);
   }
}  
//PM25
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
//      Serial.println("Find Start-----------------");
      for(int i = 0; i < 12; i++)
      {
        chrData[i] = myPMS7003.read();
//        Serial.print(String(chrData[i], HEX));
//        Serial.print("-");
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
  mPM25 = String (pm, DEC);
  if(pm <= 0)
    return;
    
//  Serial.println("");
//  Serial.print("============= PM:");
//  Serial.print(pm, DEC);
//  Serial.println(" ================");
//  Serial.println("");
//  Serial.println("PM2.5===" + mPM25);
}
