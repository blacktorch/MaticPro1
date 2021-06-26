#include <EEPROM.h>
#include "HID-Project.h"
#include <RotaryEncoder.h>

#define ROT1_IN1 6
#define ROT1_IN2 5

#define ROTARYSTEPS 2
#define ROTARYMIN -128
#define ROTARYMAX 127

int rot1_addr = 0;

int ioSelect = 2;
int clockPulse = 3;
int dataOut = 4;
int xAxis = A0;
int yAxis = A1;
int rxAxis = A2;
int ryAxis = A3;

int j;
int value;

//controller values;
int btn0;
int btn1;
int btn2; //L3
int btn3; //R3
int btn4;
int btn5;
int btn6;
int btn7;

int x;
int y;
int rx;
int ry;


byte switchVar = 0;

RotaryEncoder encoder1(ROT1_IN1, ROT1_IN2, RotaryEncoder::LatchMode::TWO03);
// Last known rotary position.
int lastPos1 = 0;

void setup() {
  // put your setup code here, to run once:
  Gamepad.begin();
  pinMode(ioSelect, OUTPUT);
  pinMode(clockPulse, OUTPUT);
  pinMode(dataOut, INPUT);
  pinMode(xAxis, INPUT);
  pinMode(yAxis, INPUT);
  pinMode(rxAxis, INPUT);
  pinMode(ryAxis, INPUT);

  EEPROM.get(rot1_addr, lastPos1);
  
  encoder1.setPosition(lastPos1 / ROTARYSTEPS);
  //Serial.begin(9600);
  
}

void loop() {
  encoder1.tick();
  int newPos1 = encoder1.getPosition() * ROTARYSTEPS;
  byte dataIn = 0;
  digitalWrite(ioSelect, 0);
  digitalWrite(clockPulse, 0);
  digitalWrite(clockPulse, 1);
  digitalWrite(ioSelect, 1);

  x = analogRead(xAxis);
  y = analogRead(yAxis);
  rx = analogRead(rxAxis);
  ry = analogRead(ryAxis);

  if (newPos1 < ROTARYMIN) {
    encoder1.setPosition(ROTARYMIN / ROTARYSTEPS);
    newPos1 = ROTARYMIN;

  } else if (newPos1 > ROTARYMAX) {
    encoder1.setPosition(ROTARYMAX / ROTARYSTEPS);
    newPos1 = ROTARYMAX;
  } // if

  if (lastPos1 != newPos1) {
    lastPos1 = newPos1;
    EEPROM.put(rot1_addr, lastPos1);
  } // if

  for(j = 0; j < 8; j++) {
    value = digitalRead(dataOut);

    switch(j) {
      case 0:
        btn0 = value;
        break;
      case 1:
        btn1 = value;
        break;
      case 2:
        btn2 = value;
        break;
      case 3:
        btn3 = value;
        break;
      case 4:
        btn4 = value;
        break;
      case 5:
        btn5 = value;
        break;
      case 6:
        btn6 = value;
        break;
      case 7:
        btn7 = value;
        break;
    }

    if(value) {
      int a = (1 << j);
      
      dataIn = dataIn | a;
    }
    digitalWrite(clockPulse, LOW);
    digitalWrite(clockPulse, HIGH);
  }
  if(switchVar != dataIn) {
    switchVar = dataIn;
  }
  
  if(x == 507) {
    Gamepad.xAxis(0);
  }else{
    Gamepad.xAxis(map(x,0, 1023, -32768, 32767));
  }
  if(rx >= 505 && rx <= 507) {
    Gamepad.rxAxis(0);
  }else{
    Gamepad.rxAxis(map(rx,0, 1023, -32768, 32767));
  }
  if(y == 517) {
    Gamepad.yAxis(0);
  }else{
    Gamepad.yAxis(map(y,0, 1023, -32768, 32767));
  }
  if(ry <= 514) {
    Gamepad.ryAxis(0);
  }else{
    Gamepad.ryAxis(map(ry,0, 1023, -32768, 32767));
  }

  Gamepad.zAxis(lastPos1);

  if(btn0) {
    Gamepad.press(1);
  } else {
    Gamepad.release(1);
  }
  if(btn1) {
    Gamepad.press(2);
  } else {
    Gamepad.release(2);
  }
  if(!btn2) {
    Gamepad.press(3);
  } else {
    Gamepad.release(3);
  }
  if(!btn3) {
    Gamepad.press(4);
  } else {
    Gamepad.release(4);
  }
  if(btn4) {
    Gamepad.press(5);
  } else {
    Gamepad.release(5);
  }if(btn5) {
    Gamepad.press(6);
    Gamepad.release(7);
  } else {
    Gamepad.press(7);
    Gamepad.release(6);
  }if(btn6) {
    Gamepad.press(8);
  } else {
    Gamepad.release(8);
  }
  if(btn7) {
    Gamepad.press(9);
    Gamepad.release(10);
  } else {
    Gamepad.release(9);
    Gamepad.press(10);
  }
  Gamepad.write();
  delay(1);

}
