#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

int mInput = 16;//D0
int mOutput = 13;//D7
int mCount = 0;

void setup() {

    USE_SERIAL.begin(115200);
    // USE_SERIAL.setDebugOutput(true);

    //USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    WiFiMulti.addAP("不要點我", "12345678");
    //WiFiMulti.addAP("42Y076832", "0423959072");
    
    pinMode(mInput, INPUT);
    pinMode(mOutput, OUTPUT);
    
    digitalWrite(mOutput, LOW);
}

void loop() {
    // wait for WiFi connection

    int pr = digitalRead(mInput);
    USE_SERIAL.print("pr=");
    USE_SERIAL.println(pr);

    if(pr == 0)
    {
      digitalWrite(mOutput, LOW);
    }
    else
    {
      digitalWrite(mOutput, HIGH);
    }
    
    if((WiFiMulti.run() == WL_CONNECTED) && pr == 1) {

        mCount++;
        
        HTTPClient http;

        USE_SERIAL.print("[HTTP] begin...\n");

        String str = String(mCount);
        // configure server and url
        http.begin("http://35.201.230.229/addSQL.php?name=a001&co2=danger&pm25="+str);
        //http.begin("192.168.1.12", 80, "/test.html");

        USE_SERIAL.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {

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

    delay(10000);
}

