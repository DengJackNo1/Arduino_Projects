//贝壳物联4路继电器,用于ESP-01/01s
//使用4个引脚GPIO0 , GPIO2 , TX(GPIO1) , RX(GPIO3)
//这样就不能调试输出了,因为占用了TX和RX端口
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <aJSON.h>
#include <dht11.h>
dht11 DHT;
#define DHT11_PIN 15

//=============  此处必须修该============
String DEVICEID = "23079"; // 你的设备编号   ==
String  APIKEY = "0b8673de4"; // 设备密码==
String INPUTID = "21038";
const char* ssid     = "baoli2303";//无线名称
const char* password = "wwwhhh123";//无线密码

//=======================================
unsigned long lastCheckInTime = 0; //记录上次报到时间
unsigned long lastUpdateTime = 0; //记录上次更新数据时间
const unsigned long postingInterval = 40000; // 每隔40秒向服务器报到一次
const unsigned long updateInterval = 10000; // 每10秒向服务器发送一次数据
boolean isCheckIn = false;

const char* host = "www.bigiot.net";
const int httpPort = 8181;
String PPP = "play";
String SSS = "stop";


#define PIN1 2  //对应板子上绿灯,由于继电器干扰,重启等原因,调整为低电平触发
#define PIN2 0  //低电平触发,由于继电器干扰,重启等原因,调整为低电平触发
#define PIN3 4  //高电平触发
#define PIN4 5  //高电平触发


bool flag; //继电器是否写入标志位
bool relay1;
bool relay2;
bool relay3;
bool relay4;

template <class T>int array2int(T& arr)
{
  int sum = 0, n = sizeof(arr) / sizeof(arr[0]);
  for (int i = 0; i < n; i++)
    sum += (arr[i] - '0') * pow(2, n - 1 - i);

  return sum + 1; //arduino编译器有奇怪的问题，这里需要+1
}




WiFiClient client;


//断网自动重启
uint32_t con_time = 0;    //断网记时
int con_flag = 0;    //断网标记，1为断网
void(*resetFunc) (void) = 0;


void setup() {
  Serial.begin(115200);


  delay(100);

  //初始化引脚低电平

  pinMode(PIN1, OUTPUT);
  pinMode(PIN2, OUTPUT);
  pinMode(PIN3, OUTPUT);
  pinMode(PIN4, OUTPUT);

  digitalWrite(PIN1, HIGH);
  digitalWrite(PIN2, HIGH);
  digitalWrite(PIN3, LOW);
  digitalWrite(PIN4, LOW);
  load_config();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  while (!client.connect(host, httpPort)) {
    delay(500);
  }
}

void save_config() {
  EEPROM.begin(4008);
  EEPROM.write(4000, 1);
  EEPROM.write(4001, digitalRead(PIN1));
  EEPROM.write(4002, digitalRead(PIN2));
  EEPROM.write(4003, digitalRead(PIN3));
  EEPROM.write(4004, digitalRead(PIN4));
  EEPROM.end();
  Serial.println("save config sucess\t");
}

void load_config() {
  EEPROM.begin(4008);
  flag = EEPROM.read(4000);
  Serial.print("flag   is   ");
  Serial.println(flag);
  if (flag == 1) {
    relay1 = EEPROM.read(4001 );
    relay2 = EEPROM.read(4002 );
    relay3 = EEPROM.read(4003 );
    relay4 = EEPROM.read(4004 );
    digitalWrite(PIN1, relay1);
    digitalWrite(PIN2, relay2);
    digitalWrite(PIN3, relay3);
    digitalWrite(PIN4, relay4);

    Serial.println(relay1);
    Serial.println(relay2);
    Serial.println(relay3);
    Serial.println(relay4);
    Serial.println("load config sucess\t");

  } else {
    Serial.println("load config failed\t");
  }
  EEPROM.end();
}


void loop() {
  // Use WiFiClient class to create TCP connections
  if (!client.connected()) {
    if (!client.connect(host, httpPort)) {
      delay(5000);
      //return;
    }
  }
  if (client.connected())
  {
    con_flag = 0;
  } else {
    if (con_flag == 0)
    {
      con_time = millis();    //给断网时间赋初始值
      con_flag = 1;
    } else
    {
      if ((millis() - con_time) >= 90000)    //判断断网时间超90秒后执行重启，这个时间可根据实际需要调整
      {
        save_config();
        Serial.println("save config \t");
        digitalWrite(PIN1, HIGH);
        digitalWrite(PIN2, HIGH);
        resetFunc();
      }
    }
  }

  if (millis() - lastCheckInTime > postingInterval || lastCheckInTime == 0) {
    checkIn();
  }
  if ((millis() - lastUpdateTime > updateInterval && isCheckIn)) {
    get_temp();
  }
  if (client.available()) {
    String inputString = client.readStringUntil('\n');
    inputString.trim();
    int len = inputString.length() + 1;
    if (inputString.startsWith("{") && inputString.endsWith("}")) {
      char jsonString[len];
      inputString.toCharArray(jsonString, len);
      aJsonObject *msg = aJson.parse(jsonString);
      processMessage(msg);
      aJson.deleteItem(msg);
    }
  }
}

void clear_eeprom() {
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.end();

}
void get_temp() {
  int chk;
  chk = DHT.read(DHT11_PIN);    // READ DATA
  switch (chk) {
    case DHTLIB_OK:
      Serial.print("OK,\t");
      break;
    case DHTLIB_ERROR_CHECKSUM:
      Serial.print("Checksum error,\t");
      break;
    case DHTLIB_ERROR_TIMEOUT:
      Serial.print("Time out error,\t");
      break;
    default:
      Serial.print("Unknown error,\t");
      break;
  }
  Serial.print(DHT.humidity, 1);
  Serial.print(",\t");
  Serial.println(DHT.temperature, 1);
  if (chk == DHTLIB_OK) {
    float dat = DHT.temperature;
    update1(DEVICEID, INPUTID, dat);
  }
  lastUpdateTime = millis();
}
void processMessage(aJsonObject *msg) {
  aJsonObject* method = aJson.getObjectItem(msg, "M");
  aJsonObject* content = aJson.getObjectItem(msg, "C");
  aJsonObject* client_id = aJson.getObjectItem(msg, "ID");
  if (!method) {
    return;
  }
  String M = method->valuestring;
  if (M == "say") {
    String C = content->valuestring;
    String F_C_ID = client_id->valuestring;
    if (C == "play") {
      digitalWrite(PIN1, LOW);
      sayToClient(F_C_ID, "PIN1 on!");
    } else if (C == "stop") {
      digitalWrite(PIN1, HIGH);
      sayToClient(F_C_ID, "PIN1 off!");
    } else if (C == "status") {
      String con = String(!digitalRead(PIN1)) + String(!digitalRead(PIN2)) + String(digitalRead(PIN3)) + String(digitalRead(PIN4));
      sayToClient(F_C_ID, con);
    } else if (C.length() == 5) {
      //play0-4 or stop0-4
      if (C.substring(0, 4) == PPP) {
        RelayState(String(C.charAt(4)).toInt(), "on");
        sayToClient(F_C_ID, "PIN" + String(C.charAt(4)) + "on");
      } else if (C.substring(0, 4) == SSS) {
        RelayState(String(C.charAt(4)).toInt(), "off");
        sayToClient(F_C_ID, "PIN" + String(C.charAt(4)) + "off");
      } else {
        sayToClient(F_C_ID, "Error Code");
      }

    } else if (C == "saveconfig") {
      save_config();
      sayToClient(F_C_ID, "save config done");
    } else {
      sayToClient(F_C_ID, "Error Code");
    }
  }
  else if (M == "checkinok") {
    isCheckIn = true;
  }
}


void RelayState(int num, String state)
{
  switch (num)
  {
    case 0:   //全部插座开关
      if (state == "on")
      {
        digitalWrite(PIN1, LOW);
        digitalWrite(PIN2, LOW);
        digitalWrite(PIN3, HIGH);
        digitalWrite(PIN4, HIGH);
      }
      else if (state == "off")
      {
        digitalWrite(PIN1, HIGH);
        digitalWrite(PIN2, HIGH);
        digitalWrite(PIN3, LOW);
        digitalWrite(PIN4, LOW);
      }
      break;
    case 1:   //插座插孔一状态
      if (state == "on")
      {
        digitalWrite(PIN1, LOW);
      }
      else if (state == "off")
      {
        digitalWrite(PIN1, HIGH);
      }
      break;
    case  2:
      if (state == "on")
      {
        digitalWrite(PIN2, LOW);
      }
      else if (state == "off")
      {
        digitalWrite(PIN2, HIGH);
      }
      break;
    case  3:
      if (state == "on")
      {
        digitalWrite(PIN3, HIGH);
      }
      else if (state == "off")
      {
        digitalWrite(PIN3, LOW);
      }
      break;
    case  4:
      if (state == "on")
      {
        digitalWrite(PIN4, HIGH);
      }
      else if (state == "off")
      {
        digitalWrite(PIN4, LOW);
      }
      break;
    default:
      break;
  }
}



void checkIn() {
  String msg = "{\"M\":\"checkin\",\"ID\":\"" + DEVICEID + "\",\"K\":\"" + APIKEY + "\"}\n";
  client.print(msg);
  lastCheckInTime = millis();
}

void sayToClient(String client_id, String content) {
  String msg = "{\"M\":\"say\",\"ID\":\"" + client_id + "\",\"C\":\"" + content + "\"}\n";
  client.print(msg);
  lastCheckInTime = millis();
}

void update1(String did, String inputid, float value) {
  client.print("{\"M\":\"update\",\"ID\":\"");
  client.print(did);
  client.print("\",\"V\":{\"");
  client.print(inputid);
  client.print("\":\"");
  client.print(value);
  client.println("\"}}");
  lastCheckInTime = millis();
  lastUpdateTime = millis();
}
