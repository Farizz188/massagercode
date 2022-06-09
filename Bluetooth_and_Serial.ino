

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


//Bluetooth
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial SerialBT;

//definitions
String wifiMacString = WiFi.macAddress();

//(Default LED code from M5 Stack website)
// How many leds in your strip? 
#define NUM_LEDS 1
#define DATA_PIN 27

// Define the array of leds
CRGB leds[NUM_LEDS];

//defining pins
const int EN = 33;                                                                                      //pin 33 on ESP32 connect to EEP on DRV8833
const int PWM1 = 22;                                                                                     //pin 22 on ESP32 connect to output 1 on DRV8833
const int In2 = 21;                                                                                     //pin 21 on ESP32 connect to output 2 on DRV8833
const int PWM3 = 19;                                                                                     //pin 19 on ESP32 connect to output 3 on DRV8833
const int In4 = 18;                                                                                     //pin 18 on ESP32 connect to output 4 on DRV8833

//defining PWM
const int freq = 5000;                                                                                  //defines frequency of PWM in Hz
const int speedChannel = 0;                                                                             //determines the duty cycle of the PWM wave
const int resolution  = 8;  


void setup(){
  //Set the pins as output pins       
  pinMode(PWM1, OUTPUT);                                                                                                                                                   
  pinMode(EN, OUTPUT);
  pinMode(In2, OUTPUT);
  pinMode(PWM3, OUTPUT);
  pinMode(In4, OUTPUT);
  digitalWrite(EN, HIGH);
  Serial.begin(115200);
  Serial.print("Stamp Pico MAC Address:  ");
  Serial.println(WiFi.macAddress());
  SerialBT.begin("THIS_IS_STAMP_PICO");

  //LED
  FastLED.addLeds<SK6812, DATA_PIN, RGB>(leds, NUM_LEDS);
  
  //configure SPEED PWM functions 
  ledcSetup(speedChannel, freq, resolution);                                                           
  ledcAttachPin(PWM1, speedChannel);
  ledcAttachPin(PWM3, speedChannel); 
}

void loop(){     

  if (SerialBT.available()> 0 ) {
      String wifiMacString = WiFi.macAddress();
      char input = SerialBT.read();
      
  
  switch(input){
    
    case '0':
      SerialBT.println("The motor stopped. PWM: 0");
      SerialBT.print("Stamp Pico Mac Address:  ");                                                       //prevents additional input to get mac add. Only shown when motor stop.
      SerialBT.println(wifiMacString);
      ledcWrite(speedChannel, 0);
      leds[0] = 0x00FFFF;                                                                                 //pink
      FastLED.show();
      break;

    case '1':
      SerialBT.println("Motor is moving. PWM: 90 ");
      ledcWrite(speedChannel, 200);                                                                       
      leds[0] = 0x0000f0;                                                                                 //brighter blue
      FastLED.show();
      break;

    case '2':
      SerialBT.println("Motor is moving. PWM: 180");
      ledcWrite(speedChannel, 230);                                                                       
      leds[0] = 0x000ff0;                                                                                   //purple
      FastLED.show();
      break;

     case '3':
      SerialBT.println("Motor is moving. PWM: 255 ");
      ledcWrite(speedChannel, 255);                                                                       
      leds[0] = 0x00f000;                                                                                //bright red
      FastLED.show();
      break;
      

    }
  } 


}
  

  
  
