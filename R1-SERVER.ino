#define WEBSOCKETS_LOGLEVEL     2
#include <WiFiClientSecure.h>
#include <WebSocketsServer_Generic.h>
#define WS_PORT 80
#include <ArduinoJson.h>

//serial communication channel number - 1
#define RXD1 18
#define TXD1 19
int D2 = 2;
WebSocketsServer  webSocket = WebSocketsServer(WS_PORT);
//String data_R;           //Serially data receive on ESP-32 (CV-data & MPU data)
String command;
String channel;          //channel name received from payload
int arr[5];             //client number

void hexdump(const void *mem, const uint32_t& len, const uint8_t& cols = 16)
{
  const uint8_t* src = (const uint8_t*) mem;
  //  Serial.printf("\n[HEXDUMP] Address: 0x%08X len: 0x%X (%d)", (ptrdiff_t)src, len, len);
  for (uint32_t i = 0; i < len; i++)
  {
    if (i % cols == 0)
    {
      //      Serial.printf("\n[0x%08X] 0x%08X: ", (ptrdiff_t)src, i);
    }
    //    Serial.printf("%02X ", *src);
    src++;
  }
  //  Serial.printf("\n");
}

void webSocketEvent(const uint8_t& num, const WStype_t& type, uint8_t * payload, const size_t& length)
{
  switch (type)
  {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", arr[(int)num]);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(arr[(int)num]);
        webSocket.sendTXT(arr[(int)num], "Connected");

        //payload is "/MPU" , "/COM"
        //arr[(int)num] is client number
        //Sorting channels to respected client
        channel = (char *)payload;

        if (channel == "/COM" )
        {
          arr[(int)num] = 1;
        }
        if (channel == "/SMPU" )
        {
          arr[(int)num] = 2;
        }

        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", arr[(int)num], ip[0], ip[1], ip[2], ip[3], payload);
      }
      break;
    case WStype_TEXT:
      //      Serial.printf("[%u] get Text: %s\n", arr[(int)num], payload);
      if ( arr[(int)num] == 1 )
      {
        //store /COM channel data in "commands"
        command =  String((char*)payload);
        Serial1.print(command + String ('*'));
        Serial.println(command);
      }
      if ( arr[(int)num] == 2 )
      {
        //print cv data & MPU data on Serial monitor
        //        Serial.print("send");
      }
      break;

    case WStype_BIN:
      //      Serial.printf("[%u] get binary length: %u\n", num, length);
      hexdump(payload, length);

      // send message to client
      webSocket.sendBIN(arr[(int)num], payload, length);
      break;
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
      break;

    default: {}
      break;
  }
}


void setup()
{
  pinMode(D2 , OUTPUT);
  digitalWrite(D2 , LOW);
  Serial.begin(38400);
  Serial2.begin(38400);
  Serial1.begin(38400 , SERIAL_8N1 , RXD1 , TXD1);
  Serial.print("Connecting to WiFi");
  WiFi.begin("R1-Server", "Server_changla_chalel");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.print("WebSockets Server started @ IP address: ");
  Serial.print(WiFi.localIP());
  Serial.print(", port: ");
  Serial.println(WS_PORT);
}

void loop()
{
//  if (Serial2.available() > 0)
//  {
//    data_R = Serial2.readStringUntil('*');
//  }
//  webSocket.sendTXT(2, data_R);
  webSocket.loop();

 // Serial.println(data_R);
  if (WiFi.status() == WL_CONNECTED)
  {
    digitalWrite(D2 , HIGH);
  }
  else
  {
    digitalWrite(D2 , LOW);
    WiFi.begin("R1-Server", "Server_changla_chalel");
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(200);
      webSocket.begin();
      webSocket.onEvent(webSocketEvent);
    }
  }

  delay(15);
//  command = "";
  //  data_R = "";
//  Serial2.flush();
}
