
/*******************************************************************************************************************************************************************************************
 Bluetooth control, Serial monitor control
 
 
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
const int EN = 33;                                                                                      //pin 12 on ESP32 connect to EEP on DRV8833
const int PWM = 19;                                                                                     //pin 16 on ESP32 connect to input 1 on DRV8833
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
  Serial.println("Speed range is 0 - 255. Enter speed:");
  SerialBT.println("Speed range is 0 - 255. Enter speed:");
  
  //LED
  FastLED.addLeds<SK6812, DATA_PIN, RGB>(leds, NUM_LEDS);
  
  //configure SPEED PWM functions 
  ledcSetup(speedChannel, freq, resolution);                                                           
  ledcAttachPin(PWM, speedChannel); 
}
void loop() {

  if (SerialBT.available()> 0 ) {
      String wifiMacString = WiFi.macAddress();
      char input = SerialBT.read();

      SerialBT.print("Device's Mac Address:  ");
      SerialBT.println(wifiMacString);

      if (input < 256);
        ledcWrite(speedChannel, input);
        SerialBT.println("The current speed is ");  
        SerialBT.println(input);
            
      if (input > 256);
        SerialBT.println("You have keyed in an invalid value. The speed change is 0 - 255.");
        
  }

  /*
  int w = Serial.parseInt();
  
  if (Serial.available() > 0) {
    if (w < 256){
      Serial.print("Speed: ");
      Serial.println(w);
      ledcWrite(speedChannel, w);                                                                        
      leds[0] = 0x0000f0;                                                                                 //bright blue
      FastLED.show();
    }
    if (w > 256){
      Serial.println("You have keyed in an invalid value. The speed change is 0 - 255.");
    }
    if (w == 0){
      ledcWrite(speedChannel, 0);
      Serial.println("Motor stopped"); 
    }
    else{
      ledcWrite(speedChannel, 0);
      Serial.println("Motor stopped");
    }
  }
  */
}
