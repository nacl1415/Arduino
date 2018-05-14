int measurePin = A0;
int ledPower = 12;
 
int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;
 
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;
 
void setup(){
  Serial.begin(9600);
  pinMode(ledPower,OUTPUT);
}
 
void loop(){
  digitalWrite(ledPower,LOW); // power on the LED
  delayMicroseconds(samplingTime);
 
  voMeasured = analogRead(measurePin); // read the dust value
  //voMeasured = voMeasured + 100;
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,HIGH); // turn the LED off
  delayMicroseconds(sleepTime);
 
  calcVoltage = voMeasured * (5.0 / 1024.0);
  dustDensity = 0.17 * calcVoltage - 0.1;

  //改成常用單位
  dustDensity = dustDensity * 1000;
  if(dustDensity < 0)
    dustDensity = 0;
  
//  Serial.print("Raw:");
//  Serial.print(voMeasured);
// 
//  Serial.print(",V:");
//  Serial.print(calcVoltage);
 
  Serial.print(", Dust:");
  Serial.print(dustDensity);
  Serial.println(" [mg/m3]");
 
  delay(3000);
}
