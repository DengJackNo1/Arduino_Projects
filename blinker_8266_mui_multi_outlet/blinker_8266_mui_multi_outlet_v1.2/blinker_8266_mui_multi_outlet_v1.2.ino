#define BLINKER_WIFI               //支持wifi
#define BLINKER_ESP_SMARTCONFIG    //Smartconfig或者手机微信扫码自动配网，代码配网请注释此条
#define BLINKER_MIOT_MULTI_OUTLET  //支持多路插座，最多4个插孔
#define BLINKER_PRINT Serial       //串口协议库
#define BLINKER_WITHOUT_SSL        //blinker默认使用加密方式进行远程通信，但通信加密会消耗大量的RAM，如果您对通信安全性无要求
//可以添加宏BLINKER_WITHOUT_SSL用以获得更多可用RAM,BLINKER_WITHOUT_SSL目前仅可用于ESP8266，其他设备的RAM足以进行加密通信
//#define BLINKER_WIFI_SUBDEVICE
#define BLINKER_ARDUINOJSON
//更新,自主接入homeassistant的mqtt服务器传感器
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#define mqtt_server "1"   // 设置的mqtt服务端地址（我这与HA的IP一样）
// mqtt 用户名和密码  服务端设置那个
#define mqtt_user "1"
#define mqtt_password "1"
//设置mqtt服务的发布主题,上传温湿度信息
#define state_topic "office/sensor1"



#include <Blinker.h>               //使用第三方Blinker库
#include <EEPROM.h>

//接入DHT11 库
#include "DHTesp.h"

#define DHTPIN 4  //制定DHT11数据引脚
DHTesp dht;

char auth[] = "1";      //Blinker APP（连接小爱同学必须阿里云服务器）中获取到的Secret Key(密钥)
//代码配网用这段
//char ssid[] = "Your WIFI SSID";        //自己wifi名称
//char pswd[] = "********";        //自己wifi密码

//定义继电器信号
#define Relay_1 2         //定义继电器1信号接入GPIO2.低电平触发
#define Relay_2 0         //定义继电器2信号接入GPIO0 低电平触发
#define Relay_3 15         //定义继电器3信号接入GPIO4 高电平触发
#define Relay_4 5         //定义继电器4信号接入GPIO5 高电平触发


bool flag; //继电器是否写入标志位
bool relay1; //记录继电器状态
bool relay2;
bool relay3;
bool relay4;

unsigned long lastUpdateTime = 0; //记录上次更新数据时间
const unsigned long updateInterval = 600000; // 每10分钟保存一次数据(300秒)
String relay_status;
String save_status;

unsigned long lastUpdate_dht_Time = 0; //记录上次更新数据时间
const unsigned long update_dht_Interval = 60000; // 每1分钟保存一次数据(上传一次数据)

WiFiClient espClient;
PubSubClient mqtt_client(espClient);


void save_or_check_config() {
  relay_status = String(digitalRead(Relay_1)) + String(digitalRead(Relay_2)) + String(digitalRead(Relay_3)) + String(digitalRead(Relay_4));
  if (save_status) {
    if (relay_status == save_status) {//不需要保存到缓存中
      BLINKER_LOG("check config sucess, status code 1");
      return;
    }
  }
  EEPROM.begin(4008);
  flag = EEPROM.read(4000);
  if (flag == 1) {
    relay1 = EEPROM.read(4001 );
    relay2 = EEPROM.read(4002 );
    relay3 = EEPROM.read(4003 );
    relay4 = EEPROM.read(4004 );
    save_status = String(relay1) + String(relay2) + String(relay3) + String(relay4);
    if (relay_status == save_status) {//不需要保存到缓存中
      BLINKER_LOG("check config sucess, status code 2");
      return;
    }
  }
  EEPROM.write(4000, 1);
  EEPROM.write(4001, digitalRead(Relay_1));
  EEPROM.write(4002, digitalRead(Relay_2));
  EEPROM.write(4003, digitalRead(Relay_3));
  EEPROM.write(4004, digitalRead(Relay_4));
  EEPROM.end();
  BLINKER_LOG("save config sucess");
  save_status = relay_status;
}


void load_config() {
  EEPROM.begin(4008);
  flag = EEPROM.read(4000);
  if (flag == 1) {
    relay1 = EEPROM.read(4001 );
    relay2 = EEPROM.read(4002 );
    relay3 = EEPROM.read(4003 );
    relay4 = EEPROM.read(4004 );
    digitalWrite(Relay_1, relay1);
    digitalWrite(Relay_2, relay2);
    digitalWrite(Relay_3, relay3);
    digitalWrite(Relay_4, relay4);
    BLINKER_LOG("\nload config sucess\t");
  } else {
    BLINKER_LOG("\nload config failed\t");
  }
  EEPROM.end();
}
// 新建Blinker软件组件对象
BlinkerButton Button0("MainSwitch");     //组件对象,要和APP组件中的“数据键名”一致,总开关
BlinkerButton Button1("Switch1");       //组件对象,要和APP组件中的“数据键名”一致，开关1
BlinkerButton Button2("Switch2");       //组件对象,要和APP组件中的“数据键名”一致，开关2
BlinkerButton Button3("Switch3");       //组件对象,要和APP组件中的“数据键名”一致，开关3
BlinkerButton Button4("Switch4");       //组件对象,要和APP组件中的“数据键名”一致，开关4
BlinkerButton Button5("Refresh");       //APP端按钮状态刷新
BlinkerButton Button6("RefreshDHT11");       //APP端DHT状态刷新

//新建Blinker软件组件数据条
BlinkerNumber HUMI("humi");  //湿度
BlinkerNumber TEMP("temp"); //温度
float humi_read = 0, temp_read = 0; //定义温湿度变量存储数据

//定义插座状态，用于小爱同学状态反馈
bool oState[5] = { false };
#define OUTLET_ALL   0  //所有插孔
#define OUTLET_NO_1  1  //插座插孔一
#define OUTLET_NO_2  2  //插座插孔二
#define OUTLET_NO_3  3  //插座插孔三
#define OUTLET_NO_4  4  //插座插孔四



//反馈继电器状态函数
void RelayState(int num)
{
  switch (num)
  {
    case 1:   //插座插孔一状态
      if (digitalRead(Relay_1) == LOW)
      {
        Button1.color("#FFFF00");   //设置app按键是纯黄色，16进制颜色码
        Button1.text("插座1开");          //设置app按键注释“开”
        Button1.print("on");
        oState[1] = true;
      }
      else if (digitalRead(Relay_1 == HIGH))
      {
        Button1.color("#808080");   //设置app按键是灰色，16进制颜色码
        Button1.text("插座1关");          //设置app按键注释“关”
        Button1.print("off");
        oState[1] = false;
      }
      break;
    case  2:
      if (digitalRead(Relay_2) == LOW)
      {
        Button2.color("#FFFF00");   //设置app按键是纯黄色，16进制颜色码
        Button2.text("插座2开");          //设置app按键注释“开”
        Button2.print("on");
        oState[2] = true;
      }
      else if (digitalRead(Relay_2 == HIGH))
      {
        Button2.color("#808080");   //设置app按键是灰色，16进制颜色码
        Button2.text("插座2关");          //设置app按键注释“关”
        Button2.print("off");
        oState[2] = false;
      }
      break;
    case  3:
      if (digitalRead(Relay_3) == HIGH)
      {
        Button3.color("#FFFF00");   //设置app按键是纯黄色，16进制颜色码
        Button3.text("插座3开");          //设置app按键注释“开”
        Button3.print("on");
        oState[3] = true;
      }
      else if (digitalRead(Relay_3 == LOW))
      {
        Button3.color("#808080");   //设置app按键是灰色，16进制颜色码
        Button3.text("插座3关");          //设置app按键注释“关”
        Button3.print("off");
        oState[3] = false;
      }
      break;
    case  4:
      if (digitalRead(Relay_4) == HIGH)
      {
        Button4.color("#FFFF00");   //设置app按键是纯黄色，16进制颜色码
        Button4.text("插座4开");          //设置app按键注释“开”
        Button4.print("on");
        oState[4] = true;
      }
      else if (digitalRead(Relay_4 == LOW))
      {
        Button4.color("#808080");   //设置app按键是灰色，16进制颜色码
        Button4.text("插座4关");          //设置app按键注释“关”
        Button4.print("off");
        oState[4] = false;
      }
      break;
    default:
      break;
  }
}



//小爱同学控制插座多个插孔
void ctrl_multi_outlet(uint8_t num, uint8_t io_level)
{
  switch (num)
  {
    case 0:  //所有插孔
      digitalWrite(Relay_1, io_level);//控制继电器1
      digitalWrite(Relay_2, io_level);//控制继电器2
      digitalWrite(Relay_3, !io_level);//控制继电器3
      digitalWrite(Relay_4, !io_level);//控制继电器4
      break;
    case 1:  //插座插孔一
      digitalWrite(Relay_1, io_level);//控制继电器1
      break;
    case 2:  //插座插孔二
      digitalWrite(Relay_2, io_level);//控制继电器2
      break;
    case 3:  //插座插孔三
      digitalWrite(Relay_3, !io_level);//控制继电器3
      break;
    case 4:  //插座插孔四
      digitalWrite(Relay_4, !io_level);//控制继电器4
      break;
    default:
      break;
  }
}

//小爱电源类回调，例如：“打开插座”、“打开插座插孔一”、“打开插座插孔二”
void miotPowerState(const String & state, uint8_t num)
{
  BLINKER_LOG("need set outlet: ", num, ", power state: ", state);

  if (state == BLINKER_CMD_ON)
  {
    ctrl_multi_outlet(num, LOW);//打开继电器，num表示是多少路（继电器低电平出发）
    BlinkerMIOT.powerState("on", num);
    BlinkerMIOT.print();
    RelayState(num);

    oState[num] = true;
  }
  else if (state == BLINKER_CMD_OFF)
  {
    ctrl_multi_outlet(num, HIGH);//关闭继电器，num表示是多少路

    BlinkerMIOT.powerState("off", num);
    BlinkerMIOT.print();
    RelayState(num);

    oState[num] = false;
  }
}

//小爱设备查询的回调函数，查询设备状态，例如：“插座插孔一状态”
void miotQuery(int32_t queryCode, uint8_t num)
{
  BLINKER_LOG("插孔", num, "状态", ",codes:", queryCode);

  switch (num)
  {
    case 0 :
      BLINKER_LOG("状态：");
      BlinkerMIOT.powerState(oState[1] ? "on" : "off");
      BlinkerMIOT.powerState(oState[2] ? "on" : "off");
      BlinkerMIOT.powerState(oState[3] ? "on" : "off");
      BlinkerMIOT.powerState(oState[4] ? "on" : "off");
      BlinkerMIOT.print();
      break;
    case 1 :
      BLINKER_LOG("插孔1状态：");
      BlinkerMIOT.powerState(oState[1] ? "on" : "off");
      BlinkerMIOT.print();
      break;
    case 2 :
      BLINKER_LOG("插孔2状态：");
      BlinkerMIOT.powerState(oState[2] ? "on" : "off");
      BlinkerMIOT.print();
      break;
    case 3 :
      BLINKER_LOG("插孔3状态：");
      BlinkerMIOT.powerState(oState[3] ? "on" : "off");
      BlinkerMIOT.print();
      break;
    case 4 :
      BLINKER_LOG("插孔4状态：");
      BlinkerMIOT.powerState(oState[4] ? "on" : "off");
      BlinkerMIOT.print();
      break;
    default :
      BlinkerMIOT.powerState(oState[1] ? "on" : "off");
      BlinkerMIOT.powerState(oState[2] ? "on" : "off");
      BlinkerMIOT.powerState(oState[3] ? "on" : "off");
      BlinkerMIOT.powerState(oState[4] ? "on" : "off");
      BlinkerMIOT.print();
      break;
  }
}

// 在APP控制，按下MainSwitch按键即会执行该函数
void button0_callback(const String & state)
{
  BLINKER_LOG("操作了MainSwitch: ", state);//APP中的Monitor控件打印的信息
  if (state == "on")
  {
    ctrl_multi_outlet(OUTLET_ALL, LOW);//打开继电器--所有
    // 反馈继电器状态
    Button0.color("#FFFF00");   //设置app按键是纯黄色，16进制颜色码
    Button0.text("开");          //设置app按键注释“开”
    Button0.print("on");
  } else if (state == "off")
  {
    ctrl_multi_outlet(OUTLET_ALL, HIGH);//关闭继电器--所有
    // 反馈继电器状态
    Button0.color("#808080");   //设置app按键是纯黄色，16进制颜色码
    Button0.text("关");          //设置app按键注释“开”
    Button0.print("off");
  }
}

// 在APP控制，按下Switch1按键即会执行该函数--第1路开关
void button1_callback(const String & state)
{
  BLINKER_LOG("get button state: ", state);//APP中的Monitor控件打印的信息
  if (state == "on")
  {
    ctrl_multi_outlet(OUTLET_NO_1, LOW);//打开继电器--第1路
    // 反馈继电器1状态
    RelayState(1);    //调用继电器反馈程序
  } else if (state == "off")
  {
    ctrl_multi_outlet(OUTLET_NO_1, HIGH);//关闭继电器--第1路
    // 反馈继电器状态
    RelayState(1);    //调用继电器反馈程序
  }
}

// 在APP控制，按下Switch2按键即会执行该函数--第2路开关
void button2_callback(const String & state)
{
  BLINKER_LOG("get button state: ", state);//APP中的Monitor控件打印的信息
  if (state == "on")
  {
    ctrl_multi_outlet(OUTLET_NO_2, LOW);//打开继电器--第2路
    // 反馈继电器状态
    RelayState(2);    //调用继电器反馈程序
  } else if (state == "off")
  {
    ctrl_multi_outlet(OUTLET_NO_2, HIGH);//关闭继电器--第2路
    // 反馈继电器状态
    RelayState(2);    //调用继电器反馈程序
  }
}

// 在APP控制，按下Switch3按键即会执行该函数--第3路开关
void button3_callback(const String & state)
{
  BLINKER_LOG("get button state: ", state);//APP中的Monitor控件打印的信息
  if (state == "on")
  {
    ctrl_multi_outlet(OUTLET_NO_3, LOW);//打开继电器--第3路
    // 反馈继电器状态
    RelayState(3);    //调用继电器反馈程序
  } else if (state == "off")
  {
    ctrl_multi_outlet(OUTLET_NO_3, HIGH);//关闭继电器--第3路
    // 反馈继电器状态
    RelayState(3);    //调用继电器反馈程序
  }
}

// 在APP控制，按下Switch4按键即会执行该函数--第4路开关
void button4_callback(const String & state)
{
  BLINKER_LOG("get button state: ", state);//APP中的Monitor控件打印的信息
  if (state == "on")
  {
    ctrl_multi_outlet(OUTLET_NO_4, LOW);//打开继电器--第4路
    // 反馈继电器状态
    RelayState(4);    //调用继电器反馈程序
  } else if (state == "off")
  {
    ctrl_multi_outlet(OUTLET_NO_4, HIGH);//关闭继电器--第4路
    // 反馈继电器状态
    RelayState(4);    //调用继电器反馈程序
  }
}

//APP端状态手动刷新按钮
void button5_callback(const String & state)
{
  for (int i = 0; i < 5; i++)
  {
    RelayState(i);
  }
}


void getHUMITEMP(const String & state)
{
  HUMI.print(humi_read);
  //设置ui组件图标和颜色
  HUMI.icon("fas fa-humidity");
  HUMI.color("#fddb10");

  //反馈湿度数据
  TEMP.print(temp_read);
  TEMP.icon("fas fa-thermometer-half");
  HUMI.color("#fddb01");
}

void mqtt_publish()
{
  //构建json发送mqtt服务器
  // 定义温度变量名
  String  temperature = "\"temperature\":";
  // 定义湿度变量名
  String  humidity = "\"humidity\":";
  String output = "{" + temperature + String(temp_read,2) + "," + humidity + String(humi_read,2) + "}";
  mqtt_client.publish(state_topic, output.c_str(), true);
}

void getdht11(const String & state) {
  float h = dht.getHumidity();
  float t = dht.getTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return ;
  }
  char s1[20],s2[20];   
  dtostrf(t,2,2,s1);
  dtostrf(h,2,2,s2);
  temp_read = t;
  humi_read = h;
}


//心跳包刷新状态
void heartbeat()
{
  for (int i = 0; i < 5; i++)
  {
    RelayState(i);
  }
  getHUMITEMP("Tap");
}


void dataStorage()
{
  Blinker.dataStorage("temp", temp_read);
  Blinker.dataStorage("humi", humi_read);
}


void smartConfig()
{
  WiFi.mode(WIFI_STA);
  Serial.println("\r\nWait for Smartconfig...");
  WiFi.beginSmartConfig();//等待手机端发出的用户名与密码
  while (1)
  {
    Serial.print(".");
    digitalWrite(Relay_1, HIGH);
    Blinker.delay(1000);
    digitalWrite(Relay_1, LOW);
    Blinker.delay(1000);
    if (WiFi.smartConfigDone())//退出等待
    {
      Serial.println("SmartConfig Success");
      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
      break;
    }
  }
}

bool AutoConfig()
{
  WiFi.begin();
  //如果觉得时间太长可改
  for (int i = 0; i < 40; i++)
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      Blinker.delay(500);
      Serial.print(".");
    } else {
      Serial.println("WIFI SmartConfig Success");
      Serial.print("本地IP： ");
      Serial.println(WiFi.localIP());
      WiFi.setAutoConnect(true);
      return true;
    }
  }
  Serial.println("WIFI AutoConfig Faild!" );
  return false;
}


void mqtt_reconnect() {
  // Loop until we're reconnected
  if(!mqtt_client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client001-";
    clientId += String(random(0xffff), HEX);
    if (mqtt_client.connect(clientId.c_str(),mqtt_user, mqtt_password)) {
      Serial.println("mqtt connected");
      mqtt_client.publish("outTopic", "hello mqtt server");
      // ... and resubscribe
      // mqtt_client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 2 seconds before retrying
      Blinker.delay(2000);
    }
  }
}

void setup()
{
  // 初始化串口，用于调试，后期可删除
  Serial.begin(115200);
  BLINKER_DEBUG.stream(Serial);
  BLINKER_DEBUG.debugAll();

  // 初始化有继电器的IO
  pinMode(Relay_1, OUTPUT);
  pinMode(Relay_2, OUTPUT);
  pinMode(Relay_3, OUTPUT);
  pinMode(Relay_4, OUTPUT);
  //初始化继电器初始状态
  digitalWrite(Relay_1, HIGH);       //继电器为低电平触发，初始化为HIGH
  digitalWrite(Relay_2, HIGH);
  digitalWrite(Relay_3, LOW);
  digitalWrite(Relay_4, LOW);

  //使用代码配网
  Blinker.delay(10);
  if(!AutoConfig())
  {
  //若开始配网了,gpio2的灯开始闪烁,对应的继电器通断
  smartConfig();
  }
  
  load_config();//从EEEprom读取数据,调整继电器状态为断电前状态

  //初始化blinker
  Blinker.begin(auth);              //手机配网用这段
  //Blinker.begin(auth, ssid, pswd);//代码配网用这段
  Button0.attach(button0_callback);   //绑定按键回调
  Button1.attach(button1_callback); //绑定按键回调
  Button2.attach(button2_callback); //绑定按键回调
  Button3.attach(button3_callback); //绑定按键回调
  Button4.attach(button4_callback); //绑定按键回调
  Button5.attach(button5_callback);
  Button6.attach(getHUMITEMP);

  //小爱同学注册回调
  BlinkerMIOT.attachPowerState(miotPowerState); //注册小爱电源回调
  BlinkerMIOT.attachQuery(miotQuery);           //小爱设备查询的回调函数

  //心跳包，初始化
  Blinker.attachHeartbeat(heartbeat);           //app定时向设备发送心跳包, 设备收到心跳包后会返回设备当前状态
  Blinker.attachDataStorage(dataStorage);

  //mqtt固定服务器ip和端口号
  
  dht.setup(DHTPIN, DHTesp::DHT11); // Connect DHT sensor to GPIO 17
  mqtt_client.setServer(mqtt_server, 1883);
}

void loop()
{
  if (!mqtt_client.connected()) {
    mqtt_reconnect();
  }
  Blinker.run();
  mqtt_client.loop();
  if ((millis() - lastUpdateTime > updateInterval)) {
    save_or_check_config();
    lastUpdateTime = millis();
  }
  if ((millis() - lastUpdate_dht_Time > update_dht_Interval)) {

    getdht11("Tap'");
    mqtt_publish();
    lastUpdate_dht_Time = millis();
  }
}
