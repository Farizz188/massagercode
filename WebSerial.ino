/******************************************************************************************************************************* 
ESP32 = https://dl.espressif.com/dl/package_esp32_index.json, http://arduino.esp8266.com/stable/package_esp8266com_index.json
M5Stack = https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/arduino/package_m5stack_index.json

 WebSerial control
 Serial monitor control
 Get Ip Address
 Get MacAddress 

Key:
 m     = show Mac Address
 0     = Stop
 1 - 4 = move the motor
 3 - 4 = not recommended for small vibration motor 

 Asiatel Network:
 ssid = Asiatel
 pass = !qaz@wsx#edc
*********************************************************************************************************************************/

//libraries 
#include "Arduino.h"
#include <FastLED.h>
#include <WiFi.h>
#include "AsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include <WebSerial.h>

AsyncWebServer server(80); 


//Wifi credentials
char ssid[] = "Asiatel";                                                                            //Change accordingly to ur own network & password
char pass[] = "!qaz@wsx#edc";

// How many leds in your strip?
#define NUM_LEDS 1
#define DATA_PIN 27

// Define the array of leds
CRGB leds[NUM_LEDS];

//defining pins
const int EN = 33;                                                                                      //pin 33 on Stamp-Pico connect to EEP on DRV8833
const int PWM = 19;                                                                                     //pin 19 on Stamp-Pico connect to input 1 on DRV8833
const int In2 = 18;                                                                                     //pin 18 on Stamp-Pico connect to input 2 on DRV8833

//defining PWM
const int freq = 5000;                                                                                  //defines frequency of PWM in Hz
const int speedChannel = 0;                                                                             //determines the range of PWM from 0 to 255
const int resolution  = 8;  


//webserver set-up
void recvMsg(uint8_t *data, size_t len){
  String h = "";  
  for(int i=0; i < len; i++){
    h += char(data[i]);

  }
  
  WebSerial.println(h);
  if (h == "0"){
      Serial.println("Motor is not moving. PWM: 0");
      WebSerial.println("Motor is not moving. PWM: 0");
      ledcWrite(speedChannel, 0);
      leds[0] = 0x0000f0;                                                                                 //Blue
      FastLED.show();
      
  }

  if (h == "1"){
      Serial.println("Motor is moving. PWM: 30");
      WebSerial.println("Motor is moving. PWM: 30");
      ledcWrite(speedChannel, 30);
      leds[0] = 0x000ff0;                                                                                 //Purple
      FastLED.show();
  }

  if (h == "2"){
      Serial.println("Motor is moving. PWM: 90");
      WebSerial.println("Motor is moving. PWM: 90");
      ledcWrite(speedChannel, 90);
      leds[0] = 0xf00000;                                                                                  //Green
      FastLED.show();
      
  }    

  if (h == "3"){
      Serial.println("Motor is moving. PWM: 180");
      WebSerial.println("Motor is moving. PWM: 180");
      ledcWrite(speedChannel, 180);
      leds[0] = 0xff000;                                                                                   //orange
      FastLED.show();
      
  }   

  if (h == "4"){
      Serial.println("Motor is moving. PWM: 255");
      WebSerial.println("Motor is moving. PWM: 255");
      ledcWrite(speedChannel, 255);
      leds[0] = 0x00FFFF;                                                                                 //Pink
      FastLED.show();
  }

  if (h == "m"){
      Serial.println("Stamp-Pico's Information");
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());
      Serial.print("MAC Address:  ");
      Serial.println(WiFi.macAddress());
      WebSerial.println("Stamp-Pico's Information");                                                     //this command will show the device's mac address on the WebSerial
      WebSerial.print("MAC Address:  ");
      WebSerial.println(WiFi.macAddress());
   
  }
}



void setup() {
  
  //Set the pins as output pins       
  pinMode(PWM, OUTPUT);                                                                                                                                                   
  pinMode(EN, OUTPUT);
  pinMode(In2, OUTPUT);
  digitalWrite(EN, HIGH);

  //Serial monitor & Wifi Serial
  Serial.begin(115200);
  
  //LED
  FastLED.addLeds<SK6812, DATA_PIN, RGB>(leds, NUM_LEDS);
  
  //configure SPEED PWM functions 
  ledcSetup(speedChannel, freq, resolution);                                                           
  ledcAttachPin(PWM, speedChannel);

  //Wifi and Webserver Connection
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("WiFi Failed!\n");
    return;
  }
  Serial.println("Stamp-Pico's Information");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC Address:  ");
  Serial.println(WiFi.macAddress());
  
  // WebSerial is accessible at "<IP Address>/webserial" in browser
  WebSerial.begin(&server);
  WebSerial.msgCallback(recvMsg);
  server.begin();
 }




void loop() {
  
  if (Serial.available()) {                                                                                 
    int w = Serial.parseInt();                                                                             //if remove, keep on looping

    if (w == 0){
      Serial.println("Motor is moving. PWM: 0");
      ledcWrite(speedChannel, 0);
      leds[0] = 0x0000f0;                                                                                 //Blue
      FastLED.show();
      
  }

  if (w == 1){
      Serial.println("Motor is moving. PWM: 30");
      ledcWrite(speedChannel, 30);
      leds[0] = 0x000ff0;                                                                                 //Purple
      FastLED.show();
  }

  if (w == 2){
      Serial.println("Motor is moving. PWM: 90");
      ledcWrite(speedChannel, 90);
      leds[0] = 0xf00000;                                                                                  //Green
      FastLED.show();
      
  }    

  if (w == 3){
      Serial.println("Motor is moving. PWM: 180");
      ledcWrite(speedChannel, 180);
      leds[0] = 0xff000;                                                                                   //orange
      FastLED.show();
      
  }   

  if (w == 4){
      Serial.println("Motor is moving. PWM: 255");
      ledcWrite(speedChannel, 255);
      leds[0] = 0x00FFFF;                                                                                 //Pink
      FastLED.show();
  }
  }
}
