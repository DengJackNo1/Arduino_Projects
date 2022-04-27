//使用unl2003驱动板代替l298n驱动板的小车
//只能停止和正转小车,而不能反装小车.

//2WD智能小车,电机控制引脚GPIO4 ,GPIO5,基于PWN调制实现

//整体速度调整

#define ENA   14          // Enable/speed motors Right        GPIO14(D5)
#define ENB   15          // Enable/speed motors Left         GPIO12(D6)


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

String command;             //String to store app command state.
int speedCar = 200;         // 400 - 1023.
int speed_Coeff = 2;

const char* ssid = "NodeMCU Car";
ESP8266WebServer server(80);

void setup() {

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);


  Serial.begin(115200);

  // Connecting WiFi

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // Starting WEB-server
  server.on ( "/", HTTP_handleRoot );
  server.onNotFound ( HTTP_handleRoot );
  server.begin();
}

void goAhead() {
  analogWrite(ENA, speedCar);
  analogWrite(ENB, speedCar);
}

void goBack() {
}

void goRight() {
  digitalWrite(ENA, LOW);
  analogWrite(ENB, speedCar);
}

void goLeft() {
  analogWrite(ENA, speedCar);
  digitalWrite(ENB, LOW);
}

void goAheadRight() {
  analogWrite(ENA, speedCar / speed_Coeff);
  analogWrite(ENB, speedCar);
}

void goAheadLeft() {
  analogWrite(ENA, speedCar);
  analogWrite(ENB, speedCar / speed_Coeff);
}

void goBackRight() {
  analogWrite(ENA, speedCar);
  analogWrite(ENB, speedCar);
}

void goBackLeft() {
  analogWrite(ENA, speedCar);
  analogWrite(ENB, speedCar);
}

void stopRobot() {
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
}

void loop() {
  server.handleClient();

  command = server.arg("State");
  Serial.println(command);
  if (command == "F") goAhead();
  else if (command == "B") goBack();
  else if (command == "L") goLeft();
  else if (command == "R") goRight();
  else if (command == "I") goAheadRight();
  else if (command == "G") goAheadLeft();
  else if (command == "J") goBackRight();
  else if (command == "H") goBackLeft();
  else if (command == "0") {
    speedCar = 50;

  }
  else if (command == "1") {
    speedCar = 100;

  }
  else if (command == "2") {
    speedCar = 150;

  }
  else if (command == "3") {
    speedCar = 200;

  }
  else if (command == "4") {
    speedCar = 250;

  }
  else if (command == "5") {
    speedCar = 300;

  }
  else if (command == "6") {
    speedCar = 350;

  }
  else if (command == "7") {
    speedCar = 400;

  }
  else if (command == "8") {
    speedCar = 450;
  }
  else if (command == "9") {
    speedCar = 500;
  }
  else if (command == "S") stopRobot();
}

void HTTP_handleRoot(void) {

  if ( server.hasArg("State") ) {
    Serial.println(server.arg("State"));
  }
  server.send ( 200, "text/html", "" );
  delay(1);
}
