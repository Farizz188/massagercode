/*******************************************************************************************************************************************************************************************
 Bluetooth control, Serial monitor control
 
 Get MacAddress 

Key:
 m     = show Mac Address
 0     = Stop
 1 - 4 = move the motor
 3 - 4 = not recommended for small vibration motor 
*******************************************************************************************************************************************************************************************/
 
//Libraries
#include "Arduino.h"
#include <FastLED.h>
#include <WiFi.h>
#include "BluetoothSerial.h"
#include "CountDown.h"

CountDown CD(CountDown::SECONDS);
uint32_t start, stop;

//Bluetooth
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial SerialBT;

//definitions
String wifiMacString = WiFi.macAddress();

// How many leds in your strip?
#define NUM_LEDS 1
#define DATA_PIN 27

// Define the array of leds
CRGB leds[NUM_LEDS];

//defining pins
const int EN = 33;                                                                                      //pin 12 on ESP32 connect to EEP on DRV8833
const int PWM = 19;                                                                                      //pin 16 on ESP32 connect to input 1 on DRV8833
const int In2 = 18;                                                                                     //pin 17 on ESP32 connect to input 2 on DRV8833

//defining PWM
const int freq = 5000;                                                                                  //defines frequency of PWM in Hz
const int speedChannel = 0;                                                                             //determines the duty cycle of the PWM wave
const int resolution  = 8;  

void setup(){
  //Set the pins as output pins       
  pinMode(PWM, OUTPUT);                                                                                                                                                   
  pinMode(EN, OUTPUT);
  pinMode(In2, OUTPUT);
  digitalWrite(EN, HIGH);
  Serial.begin(115200);
  Serial.print("Stamp Pico MAC Address:  ");
  Serial.println(WiFi.macAddress());
  SerialBT.begin("THIS_IS_STAMP_PICO");
  Serial.println(__FILE__);
  //Serial.print("COUNTDOWN_LIB_VERSION: ");
  //Serial.println(COUNTDOWN_LIB_VERSION);
  //delay(random(1));
  //start = millis();
  CD.start(1);
  Serial.println(start);

  while (CD.remaining() > 0 )
  {
     Serial.print(millis());
     Serial.print("\t");
     Serial.println(CD.remaining() );
     delay(100);
  }
  Serial.println(millis() - start);  // SHOULD PRINT 180000

  Serial.println(CD.remaining());
  Serial.println("done...");
  //LED
  FastLED.addLeds<SK6812, DATA_PIN, RGB>(leds, NUM_LEDS);
  
  //configure SPEED PWM functions 
  ledcSetup(speedChannel, freq, resolution);                                                           
  ledcAttachPin(PWM, speedChannel); 
}

void loop(){     

  if (SerialBT.available()> 0 ) {
      String wifiMacString = WiFi.macAddress();
      char input = SerialBT.read();
      
  
  switch(input){

    case 'm':
      Serial.print("Stamp Pico MAC Address:  ");
      Serial.println(WiFi.macAddress());
      SerialBT.print("Stamp Pico Mac Address:  ");
      SerialBT.println(wifiMacString); 
      break;
      
    case '0':
      Serial.println("The motor stopped. PWM: 0");
      ledcWrite(speedChannel, 0);
      leds[0] = 0x000f00;                                                                                //dim red
      FastLED.show();
      //delay(500);
      break;

    case '1':
      Serial.println("Motor is moving. PWM: 30 ");
      ledcWrite(speedChannel, 30);                                                                       
      //delay(5000);
      leds[0] = 0x00FFFF;                                                                                 //pink
      FastLED.show();
      //delay(500);
      break;

    case '2':
      Serial.println("Motor is moving. PWM: 90 ");
      ledcWrite(speedChannel, 90);                                                                       
      //delay(5000);
      leds[0] = 0x000ff0;                                                                                 //purple
      FastLED.show();
      //delay(500);
      break;

    case '3':
      Serial.println("Motor is moving. PWM: 180");
      ledcWrite(speedChannel, 180);                                                                       
      //delay(5000);
      leds[0] = 0x00000f;                                                                                 //dim blue
      FastLED.show();
      //delay(500);
      break;

     case '4':
      Serial.println("Motor is moving. PWM: 255 ");
      ledcWrite(speedChannel, 255);                                                                       
      //delay(5000);
      leds[0] = 0x0f0000;                                                                                 //dim green
      FastLED.show();
      //delay(500);
      break;
      

    }
  } 

  if (Serial.available()) {   
                                                          
    int w = Serial.parseInt();                                                                             //if remove, keep on looping
    SerialBT.write(Serial.read());
    Serial.print("Stamp Pico MAC Address:  ");
    Serial.println(WiFi.macAddress());                        


    if (w == 0){
      Serial.println("Motor is moving. PWM: 0");
      ledcWrite(speedChannel, 0);                                                                        //ledcWrite is a built-in library that is equivalent to AnalogWrite                                                                      
      //delay(5000);
      leds[0] = 0x00f000;                                                                                 //bright red
      FastLED.show();
      //delay(500);
    }
    
    if (w == 1){
      Serial.println("Motor is moving. PWM: 30");
      ledcWrite(speedChannel, 30);                                                                        
      //delay(5000);
      leds[0] = 0x000ff0;                                                                                 //purple
      FastLED.show();
      //delay(500);
    }

    if (w == 2){
      Serial.println("Motor is moving. PWM: 90");
      ledcWrite(speedChannel, 90);                                                                       
      //delay(5000);
      leds[0] = 0xf00000;                                                                                 //brighter green
      FastLED.show();
      //delay(500);
    }

    if (w == 3){
      Serial.println("Motor is moving. PWM: 180");
      ledcWrite(speedChannel, 180);                                                                       
      //delay(5000);
      leds[0] = 0xff000;                                                                                   //orange
      FastLED.show();
      //delay(500);
    }
    
    if (w == 4){
      Serial.println("Motor is moving. PWM: 255");
      ledcWrite(speedChannel, 255);                                                                        
      //delay(5000);
      leds[0] = 0x0000f0;                                                                                 //bright blue
      FastLED.show();
      //delay(500);
    }

  }

}
  

  
