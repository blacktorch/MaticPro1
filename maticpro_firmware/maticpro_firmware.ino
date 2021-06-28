#include <EEPROM.h>
//#include "HID-Project.h"
#include "Joystick.h"
#include <RotaryEncoder.h>

#define ROT1_IN1 6
#define ROT1_IN2 5
#define ROT2_IN1 7
#define ROT2_IN2 8

#define ROTARYSTEPS 1
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

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, 
  JOYSTICK_TYPE_JOYSTICK, 16, 0,
  true, true, true, //X,Y,Z
  true, true, true,//Rx,Ry,Rz
  false, false, false, false, false);

Gains mygains[2];
EffectParams myeffectparams[2];
int32_t forces[2] = {0};

void setup() {
  // put your setup code here, to run once:
  
  mygains[0].totalGain = 100;//0-100
  mygains[0].springGain = 100;//0-100
  //enable gains REQUIRED
  Joystick.setGains(mygains);
  Joystick.begin(false);
  pinMode(ioSelect, OUTPUT);
  pinMode(clockPulse, OUTPUT);
  pinMode(dataOut, INPUT);
  pinMode(xAxis, INPUT);
  pinMode(yAxis, INPUT);
  pinMode(rxAxis, INPUT);
  pinMode(ryAxis, INPUT);
  pinMode(9, OUTPUT);

  EEPROM.get(rot1_addr, lastPos1);
  EEPROM.get(rot2_addr, lastPos2);
  
  encoder1.setPosition(lastPos1 / ROTARYSTEPS);
  encoder2.setPosition(lastPos2 / ROTARYSTEPS);
  
  Joystick.setXAxisRange(-512, 511);
  Joystick.setYAxisRange(-512, 511);
  Joystick.setZAxisRange(-512, 511);
  Joystick.setRxAxisRange(-512, 511);
  Joystick.setRyAxisRange(-128, 127);
  Joystick.setRzAxisRange(-128, 127);

 // Serial.begin(115200);
  
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
    Joystick.setXAxis(0);
  }else{
    Joystick.setXAxis(map(x, 0, 1023, -512, 511));
  }
  if(y >= 500 && y <= 520) {
    Joystick.setYAxis(0);
  }else{
    Joystick.setYAxis(map(y, 0, 1023, -512, 511));
  }
  if(rx >= 500 && rx <= 512) {
    Joystick.setZAxis(0);
  }else{
    Joystick.setZAxis(map(rx, 0, 1023, -512, 511));
  }
  
  if(ry >= 500 && ry <= 520) {
    Joystick.setRxAxis(0);
  }else{
    Joystick.setRxAxis(map(ry, 0, 1023, -512, 511));
  }

  Joystick.setRyAxis(lastPos1);
  Joystick.setRzAxis(lastPos2);

  if(btn0) {
    Joystick.pressButton(0);
  } else {
    Joystick.releaseButton(0);
  }
  if(btn1) {
    Joystick.pressButton(1);
  } else {
    Joystick.releaseButton(1);
  }
  if(!btn2) {
    Joystick.pressButton(2);
  } else {
    Joystick.releaseButton(2);
  }
  if(!btn3) {
    Joystick.pressButton(3);
  } else {
    Joystick.releaseButton(3);
  }
  if(btn4) {
    Joystick.pressButton(4);
  } else {
    Joystick.releaseButton(4);
  }if(btn5) {
    Joystick.pressButton(5);
    Joystick.releaseButton(6);
  } else {
    Joystick.pressButton(6);
    Joystick.releaseButton(5);
  }if(btn6) {
    Joystick.pressButton(7);
  } else {
    Joystick.releaseButton(7);
  }
  if(btn7) {
    Joystick.pressButton(8);
    Joystick.releaseButton(9);
  } else {
    Joystick.releaseButton(8);
    Joystick.pressButton(9);
  }
  if(btn8) {
    Joystick.pressButton(10);
    Joystick.releaseButton(11);
  } else {
    Joystick.releaseButton(10);
    Joystick.pressButton(11);
  }
  if(btn9) {
    Joystick.pressButton(12);
  } else {
    Joystick.releaseButton(12);
  }
  if(btn10) {
    Joystick.pressButton(13);
    Joystick.releaseButton(14);
  } else {
    Joystick.releaseButton(13);
    Joystick.pressButton(14);
  }
  if(btn11) {
    Joystick.pressButton(15);
  } else {
    Joystick.releaseButton(15);
  }
  Joystick.sendState();

  myeffectparams[0].springMaxPosition = 1023;
  myeffectparams[0].springPosition = 1023;//0-1023
  Joystick.setEffectParams(myeffectparams);
  Joystick.getForce(forces);
  if(forces[0] > 0){
    analogWrite(9,abs(forces[0]));
  }else{
    analogWrite(9,abs(forces[0]));
  }

  delay(1);

}
