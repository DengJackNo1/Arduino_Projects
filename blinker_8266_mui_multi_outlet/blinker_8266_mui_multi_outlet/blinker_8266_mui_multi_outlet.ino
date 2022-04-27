#define BLINKER_PRINT Serial
#define BLINKER_WIFI
#define BLINKER_MIOT_MULTI_OUTLET  //同步设备后可显示一个插座和四个插孔(名为插孔的插座设备)
//#define BLINKER_ESP_SMARTCONFIG


#include <Blinker.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <EEPROM.h>
#include <string.h>
char auth[] = "e5621a7be632";



//这里是贝壳物联
#include <aJSON.h>
//=============  此处必须修该============
String DEVICEID = "1"; // 你的设备编号   ==
String  APIKEY = "1"; // 设备密码==
String INPUTID = "21";
const char* ssid     = "1";//无线名称
const char* password = "1";//无线密码
//=======================================
unsigned long lastCheckInTime = 0; //记录上次报到时间
unsigned long lastUpdateTime = 0; //记录上次更新数据时间
const unsigned long postingInterval = 40000; // 每隔40秒向服务器报到一次
const unsigned long updateInterval = 5000; // 每60秒向服务器发送一次数据
boolean isCheckIn = false;

const char* host = "www.bigiot.net";
const int httpPort = 8181;
String PPP = "play";
String SSS = "stop";

WiFiClient biglotclient;

template <class T>int array2int(T& arr)
{
  int sum = 0, n = sizeof(arr) / sizeof(arr[0]);
  for (int i = 0; i < n; i++)
    sum += (arr[i] - '0') * pow(2, n - 1 - i);
  return sum + 1; //arduino编译器有奇怪的问题，这里需要+1
}




BlinkerButton Button1("switch1");
BlinkerButton Button2("switch2");
BlinkerButton Button3("switch3");
BlinkerButton Button4("switch4");
BlinkerButton Button5("switch_all");
BlinkerNumber Number1("num-abc");



bool oState[5] = { false };
#define OUTLET_ALL   0  //所有插孔
#define OUTLET_NO_1  1  //插座插孔一
#define OUTLET_NO_2  2  //插座插孔二
#define OUTLET_NO_3  3  //插座插孔三
#define OUTLET_NO_4  4  //插座插孔四

int OUTLET1 = 0; //对应继电器GPIO
int OUTLET2 = 2; //D2
int OUTLET3 = 4; //D5
int OUTLET4 = 5; //D6


bool autoConfig()
{
  WiFi.begin();
  for (int i = 0; i < 20; i++)
  {
    int wstatus = WiFi.status();
    if (wstatus == WL_CONNECTED)
    {
      Serial.println("AutoConfig Success");
      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
      WiFi.printDiag(Serial);
      return true;
      //break;
    }
    else
    {
      Serial.print("AutoConfig Waiting......");
      Serial.println(wstatus);
      delay(1000);
    }
  }
  Serial.println("AutoConfig Faild!" );
  return false;
  //WiFi.printDiag(Serial);
}

void smartConfig()
{
  WiFi.mode(WIFI_STA);
  Serial.println("\r\nWait for Smartconfig");
  WiFi.beginSmartConfig();
  while (1)
  {
    Serial.print(".");
    if (WiFi.smartConfigDone())
    {
      Serial.println("SmartConfig Success");
      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
      WiFi.setAutoConnect(true);  // 设置自动连接
      break;
    }
    delay(1000); // 这个地方一定要加延时，否则极易崩溃重启
  }
}




// 按下按键即会执行该函数
void button1_callback(const String & state) {
  BLINKER_LOG("button1_callback get button state: ", state);

  if (state == "on") {
    digitalWrite(OUTLET1, HIGH);
    Button1.icon("toggle-on");
    Button1.text("开");
    Button1.color("#1E90FF");
    Button1.print("on");
  } else {
    digitalWrite(OUTLET1, LOW);
    Button1.icon("toggle-off");
    Button1.text("关");
    Button1.color("#FF0000");
    Button1.print("off");
  }
}


// 按下按键即会执行该函数
void button2_callback(const String & state) {
  BLINKER_LOG("button2_callback get button state: ", state);

  if (state == "on") {
    digitalWrite(OUTLET2, HIGH);
    Button2.icon("toggle-on");
    Button2.color("#1E90FF");
    Button2.text("开");
    Button2.print("on");
  } else {
    digitalWrite(OUTLET2, LOW);
    Button2.icon("toggle-off");
    Button2.text("关");
    Button2.color("#FF0000");
    Button2.print("off");
  }
}

void button3_callback(const String & state) {
  BLINKER_LOG("button3_callback get button state: ", state);

  if (state == "on") {
    digitalWrite(OUTLET3, HIGH);
    Button3.icon("toggle-on");
    Button3.color("#1E90FF");
    Button3.text("开");
    Button3.print("on");
  } else {
    digitalWrite(OUTLET3, LOW);
    Button3.icon("toggle-off");
    Button3.color("#FF0000");
    Button3.text("关");
    Button3.print("off");
  }
}

void button4_callback(const String & state) {
  BLINKER_LOG("button4_callback get button state: ", state);
  if (state == "on") {
    digitalWrite(OUTLET4, HIGH);
    Button4.icon("toggle-on");
    Button4.color("#1E90FF");
    Button4.text("开");
    Button4.print("on");
  } else {
    digitalWrite(OUTLET4, LOW);
    Button4.icon("toggle-off");
    Button4.color("#FF0000");
    Button4.text("关");
    Button4.print("off");
  }
}
// 如果未绑定的组件被触发，则会执行其中内容
void dataRead(const String & data)
{
  BLINKER_LOG("Blinker readString: ", data);
}
//小爱同学处理函数

//小爱电源类回调，例如：“打开1号插座”、“打开1号插座插孔一”、“打开1号插座插孔二”
void miotPowerState(const String & state, uint8_t num)
{
  BLINKER_LOG("need set outlet: ", num, ", power state: ", state);
  if (state == BLINKER_CMD_ON)
  {

    BlinkerMIOT.powerState("on", num);
    BlinkerMIOT.print();

    oState[num] = true;
  }
  else if (state == BLINKER_CMD_OFF)
  {


    BlinkerMIOT.powerState("off", num);
    BlinkerMIOT.print();

    oState[num] = false;

  }
}


void setup()
{
  Serial.begin(115200);


#if defined(BLINKER_PRINT)
  BLINKER_DEBUG.stream(BLINKER_PRINT);
#endif

  pinMode(OUTLET1, OUTPUT);
  pinMode(OUTLET2, OUTPUT);
  pinMode(OUTLET3, OUTPUT);
  pinMode(OUTLET4, OUTPUT);
  digitalWrite(OUTLET1, LOW);
  digitalWrite(OUTLET2, LOW);
  digitalWrite(OUTLET3, LOW);
  digitalWrite(OUTLET4, LOW);

  if (!autoConfig())
  {
    Serial.println("Start smartConfig");
    smartConfig();
  }
  while (!biglotclient.connect(host, httpPort)) {
    delay(500);
  }
  // 初始化blinker
  Blinker.begin(auth, WiFi.SSID().c_str(), WiFi.psk().c_str());
  Blinker.attachData(dataRead);
  Button1.attach(button1_callback);//按键1
  Button2.attach(button2_callback);//按键2
  Button3.attach(button3_callback);//按键2
  Button4.attach(button4_callback);//按键2
  //小爱回调函数
  BlinkerMIOT.attachPowerState(miotPowerState);
}

void loop()
{
  Blinker.run();
  bigiot_run();
}

void bigiot_run() {
  if (!biglotclient.connected()) {
    if (!biglotclient.connect(host, httpPort)) {
      delay(5000);
      return;
    }
  }
  if (millis() - lastCheckInTime > postingInterval || lastCheckInTime == 0) {
    checkIn();
  }
  /*
    if ((millis() - lastUpdateTime > updateInterval && isCheckIn)) {
    String con = String(digitalRead(PIN0)) + String(digitalRead(PIN1)) + String(digitalRead(PIN2)) + String(digitalRead(PIN3));
    char num[] = { con.charAt(0), con.charAt(1), con.charAt(2), con.charAt(3)};
    int dat = array2int(num);
    Serial.println(dat);
    update1(DEVICEID, INPUTID, dat);
    }*/
  if (biglotclient.available()) {
    String inputString = biglotclient.readStringUntil('\n');
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
      digitalWrite(0, HIGH);
      sayToClient(F_C_ID, "PIN0 on!");
    } else if (C == "stop") {
      digitalWrite(0, LOW);
      sayToClient(F_C_ID, "PIN0 off!");
    } else if (C == "status") {
      String con = String(digitalRead(PIN0)) + String(digitalRead(PIN1)) + String(digitalRead(PIN2)) + String(digitalRead(PIN3));
      sayToClient(F_C_ID, con);
    }
    else if (C.length() == 5) {
      //play0-3 or stop0-3
      if (C.substring(0, 4) == PPP) {
        digitalWrite(String(C.charAt(4)).toInt(), HIGH);
        sayToClient(F_C_ID, "PIN" + String(C.charAt(4)) + "on");
      } else if (C.substring(0, 4) == SSS) {
        digitalWrite(String(C.charAt(4)).toInt(), LOW);
        sayToClient(F_C_ID, "PIN" + String(C.charAt(4)) + "off");
      }
    } else {
      sayToClient(F_C_ID, "Error Code");
    }
  }
  else if (M == "checkinok") {
    isCheckIn = true;
  }
}


void checkIn() {
  String msg = "{\"M\":\"checkin\",\"ID\":\"" + DEVICEID + "\",\"K\":\"" + APIKEY + "\"}\n";
  biglotclient.print(msg);
  lastCheckInTime = millis();
}

void sayToClient(String client_id, String content) {
  String msg = "{\"M\":\"say\",\"ID\":\"" + client_id + "\",\"C\":\"" + content + "\"}\n";
  biglotclient.print(msg);
  lastCheckInTime = millis();
}

void update1(String did, String inputid, int value) {
  biglotclient.print("{\"M\":\"update\",\"ID\":\"");
  biglotclient.print(did);
  biglotclient.print("\",\"V\":{\"");
  biglotclient.print(inputid);
  biglotclient.print("\":\"");
  biglotclient.print(value);
  biglotclient.println("\"}}");
  lastCheckInTime = millis();
  lastUpdateTime = millis();
}
