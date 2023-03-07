#ifndef websocket_H
#define websocket_H
#include <ArduinoJson.h>
//#include<SoftwareSerial.h>
char input='S';
String command;
//SoftwareSerial serial(10,11);
DynamicJsonDocument doc (1024);

void initCommand() {
  if (Serial.available() > 0) {
    command = Serial.readStringUntil('*');
  }
//  DeserializationError errc = deserializeJson(doc,  command);
//  //ReadBufferingStream bufferedFile(command, 64);
//
//  if (errc) {
//    // Serial.println(command);
//    return;
//  }

    if (command == "A")  //Anticlockwise
    {
      input = 'A';
    }
    else if (command == "C")  //Clockwise
    {

      input = 'C';
    }
    else if (command == "B")  //Clockwise
    {

      input = 'B';
    }

 

}
#endif
