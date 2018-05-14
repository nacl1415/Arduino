#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#include <SoftwareSerial.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

SoftwareSerial mLoraPort(5, 4); // RX, TX
int mM0 = 2;
int mM1 = 0;
int mCount = 0;

String loraStr = "";
String loraStrBackup = "";
int MsgCount = 0;

void setup() {
  
    pinMode(mM0, OUTPUT);
    pinMode(mM1, OUTPUT);
  
    digitalWrite(mM0, LOW);
    digitalWrite(mM1, LOW);
  
    mLoraPort.begin(9600);
    USE_SERIAL.begin(115200);
    // USE_SERIAL.setDebugOutput(true);

    //USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

//    WiFiMulti.addAP("CHT3196", "048242959");
//  WiFiMulti.addAP("不要點我", "12345678");
//    WiFiMulti.addAP("yoyoyo", "0921594076");
    WiFiMulti.addAP("SHINYO-NOTEBOOK 4415", "0921594076");
}

void loop() {
      
  //呼叫ID 0~9
  for (int i = 1 ; i < 10 ;i++) 
  {
    MsgCount = 0;
    //每個ID叫3次
    for (int j = 0 ; j< 3 ;j++)
    {
      //發送
      String mCallNum = "ID"+String(i) +"SENDDATA" ;
//      Serial.println(mCallNum);
      mLoraPort.print(mCallNum);
      MsgCount ++ ;
//      Serial.println("Count = " + String(MsgCount));
      delay(3000);
      // 接收Node訊息
      loraStr = "";
      while(mLoraPort.available())
      {
        char prchar = mLoraPort.read();
        loraStr = loraStr + prchar;
    //    loraStr = "H12i34C";
      }
      loraStrBackup = loraStr;
      
      USE_SERIAL.println("recv = " + loraStr);

      //判斷開頭結尾
      if (loraStr.startsWith("H") && loraStr.endsWith("C"))
      {
//        const char *loraChar = loraStr.c_str();
        char *loraChar = &loraStr[0u];
        char dst[7][7];
   
        int cnt = split(dst, loraChar, "i");
        //判斷長度及ID是否與接收的資料一樣
        if (String(dst[5]).equals(String(loraStr.length())) && String(dst[1]).equals(String(i)) )
        {
          USE_SERIAL.println("split = " + loraStrBackup);
          runWifi();  
          mLoraPort.print("ID"+String(i)+"...OK.............");
          break;
        }
      }
    }
    if (MsgCount == 3)
    {
      USE_SERIAL.println("ID"+String(i)+"ERROR.............");
    }
    
    
    delay(1000);
  }
  
  // wait for WiFi connection

//    if((WiFiMulti.run() == WL_CONNECTED) && !loraStr.equals("")) {
//
//        HTTPClient http;
//
//        USE_SERIAL.print("[HTTP] begin...\n");
//
//        // configure server and url
////        http.begin("http://www.bdoctw.com/addSqlTemperature.php?temperature="+loraStr);
//        http.begin("http://www.bdoctw.com/forTest.php?temperature="+loraStr);
//        //http.begin("192.168.1.12", 80, "/test.html");
//
//        USE_SERIAL.print("[HTTP] GET...\n");
//        // start connection and send HTTP header
//        int httpCode = http.GET();
//        if(httpCode > 0) {
//            // HTTP header has been send and Server response header has been handled
//            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);
//
//            // file found at server
//            if(httpCode == 200) {
//
//                // get lenght of document (is -1 when Server sends no Content-Length header)
//                int len = http.getSize();
//
//                // create buffer for read
//                uint8_t buff[128] = { 0 };
//
//                // get tcp stream
//                WiFiClient * stream = http.getStreamPtr();
//
//                // read all data from server
//                while(http.connected() && (len > 0 || len == -1)) {
//                    // get available data size
//                    size_t size = stream->available();
//
//                    if(size) {
//                        // read up to 128 byte
//                        int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));
//
//                        // write it to Serial
//                        USE_SERIAL.write(buff, c);
//
//                        if(len > 0) {
//                            len -= c;
//                        }
//                    }
//                    delay(1);
//                }
//
//                USE_SERIAL.println();
//                USE_SERIAL.print("[HTTP] connection closed or file end.\n");
//
//            }
//        } else {
//            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
//        }
//
//        http.end();
//
//    }
//    mCount ++ ;
}
void runWifi()
{
    if((WiFiMulti.run() == WL_CONNECTED) && !loraStr.equals("")) {

        HTTPClient http;

        USE_SERIAL.print("[HTTP] begin...\n");

        // configure server and url
//        http.begin("http://www.bdoctw.com/addSqlTemperature.php?temperature="+loraStr);
        http.begin("http://www.bdoctw.com/forTest.php?temperature="+loraStrBackup);
        USE_SERIAL.println("http://www.bdoctw.com/forTest.php?temperature="+loraStrBackup);
        //http.begin("192.168.1.12", 80, "/test.html");

        USE_SERIAL.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == 200) {

                // get lenght of document (is -1 when Server sends no Content-Length header)
                int len = http.getSize();

                // create buffer for read
                uint8_t buff[128] = { 0 };

                // get tcp stream
                WiFiClient * stream = http.getStreamPtr();

                // read all data from server
                while(http.connected() && (len > 0 || len == -1)) {
                    // get available data size
                    size_t size = stream->available();

                    if(size) {
                        // read up to 128 byte
                        int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));

                        // write it to Serial
                        USE_SERIAL.write(buff, c);

                        if(len > 0) {
                            len -= c;
                        }
                    }
                    delay(1);
                }

                USE_SERIAL.println();
                USE_SERIAL.print("[HTTP] connection closed or file end.\n");

            }
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();

    }
}
int split(char dst[][7], char* str, const char* spl)
{
    int n = 0;
    char *result = NULL;
    result = strtok(str, spl);

    while( result != NULL )
    {
        strcpy(dst[n++], result);
        result = strtok(NULL, spl);
    }
    return n;
}
