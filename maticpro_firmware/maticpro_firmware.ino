#include <EEPROM.h>
#include "HID-Project.h"
#include <RotaryEncoder.h>

#define ROT1_IN1 6
#define ROT1_IN2 5
#define ROT2_IN1 7
#define ROT2_IN2 8

#define ROTARYSTEPS 2
#define ROTARYMIN -128
#define ROTARYMAX 127

int rot1_addr = 0;
int rot2_addr = 5;

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
int btn8;
int btn9;
int btn10;
int btn11;

int x;
int y;
int rx;
int ry;


byte switchVar = 0;

RotaryEncoder encoder1(ROT1_IN1, ROT1_IN2, RotaryEncoder::LatchMode::TWO03);
RotaryEncoder encoder2(ROT2_IN1, ROT2_IN2, RotaryEncoder::LatchMode::TWO03);
// Last known rotary position.
int lastPos1 = 0;
int lastPos2 = 0;

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
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(16, OUTPUT);

  EEPROM.get(rot1_addr, lastPos1);
  EEPROM.get(rot2_addr, lastPos2);
  
  encoder1.setPosition(lastPos1 / ROTARYSTEPS);
  encoder2.setPosition(lastPos2 / ROTARYSTEPS);
  //Serial.begin(115200);
  
}

void loop() {
  encoder1.tick();
  encoder2.tick();
  int newPos1 = encoder1.getPosition() * ROTARYSTEPS;
  int newPos2 = encoder2.getPosition() * ROTARYSTEPS;
  uint16_t dataIn = 0;
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

  if (newPos2 < ROTARYMIN) {
    encoder2.setPosition(ROTARYMIN / ROTARYSTEPS);
    newPos2 = ROTARYMIN;

  } else if (newPos2 > ROTARYMAX) {
    encoder2.setPosition(ROTARYMAX / ROTARYSTEPS);
    newPos2 = ROTARYMAX;
  } // if

  if (lastPos2 != newPos2) {
    lastPos2 = newPos2;
    EEPROM.put(rot2_addr, lastPos2);
  } // if

  for(j = 0; j < 16; j++) {
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
      case 8:
        btn8 = value;
        break;
      case 9:
        btn9 = value;
        break;
      case 10:
        btn10 = value;
        break;
      case 11:
        btn11 = value;
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
  
  if(x >= 500 && x <= 512) {
    Gamepad.xAxis(0);
  }else{
    Gamepad.xAxis(map(x,0, 1023, -32768, 32767));
  }
  if(rx >= 500 && rx <= 512) {
    Gamepad.rxAxis(0);
  }else{
    Gamepad.rxAxis(map(rx,0, 1023, -32768, 32767));
  }
  if(y >= 500 && y <= 520) {
    Gamepad.yAxis(0);
  }else{
    Gamepad.yAxis(map(y,0, 1023, -32768, 32767));
  }
  if(ry >= 500 && ry <= 520) {
    Gamepad.ryAxis(0);
  }else{
    Gamepad.ryAxis(map(ry,0, 1023, -32768, 32767));
  }

  Gamepad.zAxis(lastPos1);
  Gamepad.rzAxis(lastPos2);

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
  if(btn8) {
    Gamepad.press(11);
    Gamepad.release(12);
  } else {
    Gamepad.release(11);
    Gamepad.press(12);
  }
  if(btn9) {
    Gamepad.press(13);
  } else {
    Gamepad.release(13);
  }
  if(btn10) {
    Gamepad.press(14);
    Gamepad.release(15);
  } else {
    Gamepad.release(14);
    Gamepad.press(15);
  }
  if(btn11) {
    Gamepad.press(16);
  } else {
    Gamepad.release(16);
  }
  Gamepad.write();

  delay(1);

}
