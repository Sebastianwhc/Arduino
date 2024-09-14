
#include <SPI.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include "ArduinoBlue.h"

SoftwareSerial hc06(2,3);


ArduinoBlue remoteBLE(hc06);

/*int up_button = 2; 
int down_button =4;
int left_button = 5;
int rigth_button = 3;
int star_button = 4;
int select_button = 7;
int joystick_button = 8;

int buttons[] = {up_button,down_button,left_button,rigth_button,star_button,select_button,joystick_button,joystick_axis_x,joystick_axis_y}; */
volatile int vd, vz;    
int joystick_axis_x = A0;
int joystick_axis_y = A1;
String cc;
String ca;


void setup() {

  hc06.begin(9600);
  /* for(int i=0; i<7; i++)
  {
    pinMode(buttons[i],INPUT_PULLUP);
    digitalWrite(buttons[i],LOW);
  }*/
  Serial.begin(9600);

}

void loop() {

  
   vd = analogRead(joystick_axis_x);   // realizar la lectura
   vz = analogRead(joystick_axis_y);   // realizar la lectura

   Serial.println("d" + String(vd));
   cc="d" + String(vd);
    hc06.print(cc);
    Serial.println(cc);
  
   Serial.println("z" + String(vz));
  
   ca= "z" + String(vz);
       hc06.println(ca);
       Serial.println(ca);

   delay(1000);
  

 
}