#include <ESP8266WiFi.h>
#include <aJSON.h>

//=============  此处必须修该============
String DEVICEID="23079"; // 你的设备编号   ==
String  APIKEY = "0b8673de4"; // 设备密码==
const char* ssid     = "baoli2303";//无线名称
const char* password = "wwwhhh123";//无线密码
//=======================================
unsigned long lastCheckInTime = 0; //记录上次报到时间
const unsigned long postingInterval = 40000; // 每隔40秒向服务器报到一次

const char* host = "www.bigiot.net";
const int httpPort = 8181;
int jiange=0;
String zhuangtai;
#define pins 2  //继电器连接在8266的GPIO0上 

int inputValue = 0;

WiFiClient client; 

void setup() {
  Serial.begin(115200);
  delay(10);
  //默认输出关闭电频

  pinMode(pins, OUTPUT);
  digitalWrite(pins, LOW);
  //pinMode(7, INPUT);
  //pinMode(0,OUTPUT);
  //digitalWrite(0,HIGH);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  while (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    delay(500);
  }
  Serial.print("connecting to ");
  Serial.println(host);
  }

void loop() {
/*
 inputValue = digitalRead(7);//读取IO15脚的电平状态  
 delay(200);
 if(inputValue == HIGH)//按键按下
  {
  digitalWrite(16, HIGH);
  }else{        //按键释放
  digitalWrite(16, LOW);
  }
*/


  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  // Use WiFiClient class to create TCP connections
  if (!client.connected()) {
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      delay(5000);
      return;
    }
  }

  if(millis() - lastCheckInTime > postingInterval || lastCheckInTime==0) {
    checkIn();
  }

  // Read all the lines of the reply from server and print them to Serial
  if (client.available()) {
    String inputString = client.readStringUntil('\n');
    inputString.trim();
    Serial.println(inputString);
    int len = inputString.length()+1;
    if(inputString.startsWith("{") && inputString.endsWith("}")){
      char jsonString[len];
      inputString.toCharArray(jsonString,len);
      aJsonObject *msg = aJson.parse(jsonString);
      processMessage(msg);
      aJson.deleteItem(msg);          
    }
  }
}

void processMessage(aJsonObject *msg){
  aJsonObject* method = aJson.getObjectItem(msg, "M");
  aJsonObject* content = aJson.getObjectItem(msg, "C");     
  aJsonObject* client_id = aJson.getObjectItem(msg, "ID");
  if (!method) {
    return;
  }
    String M = method->valuestring;
    if(M == "say"){
      String C = content->valuestring;
      String F_C_ID = client_id->valuestring;
      if(C == "play"){

          digitalWrite(pins, HIGH);
         
        sayToClient(F_C_ID,"LED  on!");    
      }else if(C == "stop"){

          digitalWrite(pins, LOW);
          

        sayToClient(F_C_ID,"LED   off!");    
      }else{
        int pin = C.toInt();
       /* if(pin > 0 && pin <= arr_len){
          pin--;
          state[pin] = !state[pin];
          digitalWrite(pins[pin], state[pin]);
        }*/
        sayToClient(F_C_ID,"LED pin:"+pin); 
      }
    }
}

void checkIn() {
    String msg = "{\"M\":\"checkin\",\"ID\":\"" + DEVICEID + "\",\"K\":\"" + APIKEY + "\"}\n";
    client.print(msg);
    lastCheckInTime = millis(); 
}

void sayToClient(String client_id, String content){
  String msg = "{\"M\":\"say\",\"ID\":\"" + client_id + "\",\"C\":\"" + content + "\"}\n";
  client.print(msg);
  lastCheckInTime = millis();
}
