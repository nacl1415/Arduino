//----GPS----

#define GpsSerial  Serial
#define DebugSerial Serial
int L = 13; //LED指示灯引脚
char M = 0x4d;
char C = 0x43;
char pointer = 0x2c;

//struct
//{
// char GPS_Buffer[80];
// bool isGetData;   //是否获取到GPS数据
//  bool isParseData; //是否解析完成
//  char UTCTime[11];   //UTC时间
//  char latitude[11];    //纬度
//  char N_S[2];    //N/S
//  char longitude[12];   //经度
//  char E_W[2];    //E/W
//  char mSpeed[6]; //速度
//  bool isUsefull;   //定位信息是否有效
//} Save_Data;
//
//const unsigned int gpsRxBufferLength = 600;
//char gpsRxBuffer[gpsRxBufferLength];
//unsigned int ii = 0;

float mSpeedKm ;

void setup() {
  Serial.begin(9600);

  //GPS
  GpsSerial.begin(9600);      //定义波特率9600，和我们店铺的GPS模块输出的波特率一致
  DebugSerial.begin(9600);
  DebugSerial.println("Ready!!");

//  Save_Data.isGetData = false;
//  Save_Data.isParseData = false;
//  Save_Data.isUsefull = false;

}
int mCount = 0;
String mSpeed;
char gpsTotle;
void loop() {
  while(GpsSerial.available())
  {
    char cc = GpsSerial.read(); //讀取GPS資料
    //Serial.print(cc);
    if(cc == M)  //找M
    {
      if(GpsSerial.read() == C)  //找C
      {
        mSpeed = "";
        mCount = 0;
        for(int i = 0; i < 50; i++)
        {
          if(GpsSerial.read() == pointer)
          {
            mCount++;
            if(mCount == 7)   //資料串的第七個逗點，代表速度的起始點
            {
              for(int k = 0; k < 7; k++)    //取出速度資料
              {
                cc = GpsSerial.read();
                if(cc == pointer)   //速度資料結束
                {
                  break;
                }
                //Serial.print(cc);   
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
    Serial.println("Speed=" + String(mSpeedKm));
  }

  mSpeed = "";
  delay(1000);
}

