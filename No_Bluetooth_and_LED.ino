   
/*
Without Bluetooth and LED
*/

//library for stamp pico
#include "Arduino.h"

//Serial monitor
int w = Serial.parseInt();                                                                              

//defining pins
const int EN = 33;                                                                                      //pin 12 on ESP32 connect to EEP on DRV8833
const int PWM = 19;                                                                                     //pin 16 on ESP32 connect to input 1 on DRV8833
const int In2 = 18;                                                                                     //pin 17 on ESP32 connect to input 2 on DRV8833

//defining PWM
const int freq = 5000;                                                                                  //defines frequency of PWM in Hz
const int speedChannel = 0;                                                                             //determines the duty cycle of the PWM wave
const int resolution  = 8;                                                                              //maximum of 8 bits duty resolution

void setup() {
//Set the pins as output pins       
    pinMode(PWM, OUTPUT);                                                                                                                                                   
    pinMode(EN, OUTPUT);
    pinMode(In2, OUTPUT);
    digitalWrite(EN, HIGH);                                                                             //enable the driver
    Serial.begin(9600);                                                                                 //start the serial monitor in 9600 baud
    while (! Serial);                                                                                   //check if the serial port is available, excute the while loop
    
    ledcSetup(speedChannel, freq, resolution);                                                          //configure SPEED PWM functions 
    ledcAttachPin(PWM, speedChannel);                                                                   //attaching the channel to the PWM pin to be controlled
}

void loop() {

  if (Serial.available()) {                                                                             //get the number of bytes/characters available for stamp pico to read from the serial port. 
    int w = Serial.parseInt(); //if remove, keep on looping
   
  if (w == 0);
    Serial.println("0% PWM");                 
    ledcWrite(speedChannel, 0);                                                                          //ledcWrite is a built-in library that is equivalent to AnalogWrite
    delay(500);
  
  
    if (w == 1){
      Serial.println("25% PWM");
      ledcWrite(speedChannel, 64);                                                                        
      delay(5000);
  
    }
    if (w == 2){
      Serial.println("50% PWM");
      ledcWrite(speedChannel, 128);                                                                       
      delay(5000);
    }

    if (w == 3){
      Serial.println("100% PWM");
      ledcWrite(speedChannel, 255);                                                                       
      delay(5000);
  }
    
  }

}
