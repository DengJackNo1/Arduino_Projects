//使用l298n迷你驱动板的小车
//停止和正转反转小车.

//2WD智能小车,电机控制引脚GPIO4 ,GPIO5,基于PWN调制实现
#define IN_1  15          // L298N in1 motors Right           GPIO15(D8)
#define IN_2  13          // L298N in2 motors Right           GPIO13(D7)
#define IN_3  2           // L298N in3 motors Left            GPIO2(D4)
#define IN_4  0           // L298N in4 motors Left            GPIO0(D3)

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

String command;             //String to store app command state.
int speedCar = 800;         // 400 - 1023.
int speed_Coeff = 3;

const char* ssid = "NodeMCU Car";
ESP8266WebServer server(80);

void setup() { 
 pinMode(IN_1, OUTPUT);
 pinMode(IN_2, OUTPUT);
 pinMode(IN_3, OUTPUT);
 pinMode(IN_4, OUTPUT); 
  
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

void goAhead(){ 

      digitalWrite(IN_1, LOW);
      analogWrite(IN_2, speedCar);

      digitalWrite(IN_3, LOW);
      analogWrite(IN_4, speedCar);
  }

void goBack(){ 

      analogWrite(IN_1, speedCar);
      digitalWrite(IN_2, LOW);

      analogWrite(IN_3, speedCar);
      digitalWrite(IN_4, LOW);
  }

void goRight(){ 

      analogWrite(IN_1, speedCar);
      digitalWrite(IN_2, LOW);


      digitalWrite(IN_3, LOW);
      analogWrite(IN_4, speedCar);

  }

void goLeft(){

      analogWrite(IN_1, LOW);
      digitalWrite(IN_2, speedCar);


      analogWrite(IN_3, speedCar);
      digitalWrite(IN_4, LOW);

  }

void goAheadRight(){
      
      digitalWrite(IN_1, LOW);
      analogWrite(IN_2, speedCar/speed_Coeff);

 
      digitalWrite(IN_3, LOW);
      analogWrite(IN_4, speedCar);
   }

void goAheadLeft(){
      
      digitalWrite(IN_1, LOW);
      analogWrite(IN_2, speedCar);
      digitalWrite(IN_3, LOW);
      analogWrite(IN_4, speedCar/speed_Coeff);

  }

void goBackRight(){ 

      analogWrite(IN_1, speedCar/speed_Coeff);
      digitalWrite(IN_2, LOW);

      analogWrite(IN_3, speedCar);
      digitalWrite(IN_4, LOW);

  }

void goBackLeft(){ 

      analogWrite(IN_1, speedCar);
      digitalWrite(IN_2, LOW);
      

      analogWrite(IN_3, speedCar/speed_Coeff);
      digitalWrite(IN_4, LOW);
  }

void stopRobot(){  

      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, LOW);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, LOW);

 }

void loop() {
    server.handleClient();
    
      command = server.arg("State");
      if (command == "F") goAhead();
      else if (command == "B") goBack();
      else if (command == "L") goLeft();
      else if (command == "R") goRight();
      else if (command == "I") goAheadRight();
      else if (command == "G") goAheadLeft();
      else if (command == "J") goBackRight();
      else if (command == "H") goBackLeft();
      else if (command == "0") speedCar = 400;
      else if (command == "1") speedCar = 470;
      else if (command == "2") speedCar = 540;
      else if (command == "3") speedCar = 610;
      else if (command == "4") speedCar = 680;
      else if (command == "5") speedCar = 750;
      else if (command == "6") speedCar = 820;
      else if (command == "7") speedCar = 890;
      else if (command == "8") speedCar = 960;
      else if (command == "9") speedCar = 1023;
      else if (command == "S") stopRobot();
}

void HTTP_handleRoot(void) {

if( server.hasArg("State") ){
       Serial.println(server.arg("State"));
  }
  server.send ( 200, "text/html", "" );
  delay(1);
}
