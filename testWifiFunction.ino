#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#include <SoftwareSerial.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;
HTTPClient http;

//SoftwareSerial mLoraPort(5, 4); // RX, TX
//int mM0 = 2;
//int mM1 = 0;
//int mCount = 0;
void setup() {
  
//    pinMode(mM0, OUTPUT);
//    pinMode(mM1, OUTPUT);
//  
//    digitalWrite(mM0, LOW);
//    digitalWrite(mM1, LOW);
//  
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
    WiFiMulti.addAP("yoyoyo", "0921594076");
//    WiFiMulti.addAP("SHINYO-NOTEBOOK 4415", "0921594076");

}

void loop() {
    
    // wait for WiFi connection
    
//  String loraStr = "";
//  while(mLoraPort.available())
//  {
//    char prchar = mLoraPort.read();
////    loraStr = loraStr + prchar;
//    loraStr = "H12i34C";
//  }
//
//    if (mCount >730)
//    {
//      loraStr = "NodeError";
//      USE_SERIAL.println("Error");
//      mCount = 0 ;
//    }
//    USE_SERIAL.println(loraStr);
//    USE_SERIAL.println(mCount);
//    
//    if((WiFiMulti.run() == WL_CONNECTED) && !loraStr.equals("")) 
    if((WiFiMulti.run() == WL_CONNECTED)) 
    {
        USE_SERIAL.print("[HTTP] begin...\n");

        // configure server and url
//        http.begin("http://www.bdoctw.com/addSqlTemperature.php?temperature="+loraStr);
        http.begin("140.120.160.62",80,"/forTest.php?temperature=H12i34C");
        //http.begin("192.168.1.12", 80, "/test.html");
        http.addHeader("Content-Type", "text/html");

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
//        if (loraStr.startsWith("H") && loraStr.endsWith("C")) //收到H開頭C結尾就清空計數
//        {
//           mCount = 0 ;
//        }
    }
//    mCount ++ ;

    delay(10*1000);
}

