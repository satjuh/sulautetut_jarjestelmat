#include "SevSeg.h"
SevSeg sevseg; 

int value = 0;
float voltage = 0;

void setup(){
  byte numDigits = 4;
  byte digitPins[] = {10, 11, 12, 13};
  byte segmentPins[] = {9, 2, 3, 5, 6, 8, 7, 4};

  bool resistorsOnSegments = true; 
  bool updateWithDelaysIn = true;
  byte hardwareConfig = COMMON_CATHODE; 
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(90);
}

void loop(){
    value = analogRead(A0);
    voltage = value * 5.0/1023;
    sevseg.setNumber(voltage, 2);
    sevseg.refreshDisplay();
}
