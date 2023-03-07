#include"websocket.h"
#include<Servo.h>

Servo myServo0;
Servo myServo1;
Servo myServo2;

void setup() {
  Serial.begin(38400);

  myServo0.attach(D1);
  myServo1.attach(D2);
  myServo2.attach(D3);
  myServo0.write(0);
  myServo1.write(180);
  myServo2.write(150);

}

void loop() {

  initCommand();

  switch (input) {


    case 'A':
      myServo2.write(0);
      delay(100);
      myServo0.write(180);
      delay(100);
      myServo1.write(180);
      delay(100);
      break;



    case 'B':
      myServo2.write(150);
      delay(100);
      myServo0.write(0);
      delay(100);
      myServo1.write(0);
      delay(100);
      break;

    case 'C':
      myServo0.write(0);
      myServo1.write(0);
      myServo2.write(0);



      break;
    case 'D':

      myServo0.write(150);
      break;
    case 'E':

      myServo1.write(180);
      break;
    case 'F':

      myServo2.write(180);
      break;

    case 'S':
      //      myServo.write(180);
      break;
    default:
      //   myServo.write(0);
      break;


  }

}
