#define BLINKER_WIFI               //支持wifi
#define BLINKER_ESP_SMARTCONFIG    //Smartconfig或者手机微信扫码自动配网，代码配网请注释此条
#define BLINKER_MIOT_MULTI_OUTLET  //支持多路插座，最多4个插孔
#define BLINKER_PRINT Serial       //串口协议库
#define BLINKER_WITHOUT_SSL        //blinker默认使用加密方式进行远程通信，但通信加密会消耗大量的RAM，如果您对通信安全性无要求
//可以添加宏BLINKER_WITHOUT_SSL用以获得更多可用RAM,BLINKER_WITHOUT_SSL目前仅可用于ESP8266，其他设备的RAM足以进行加密通信

#include <Blinker.h>               //使用第三方Blinker库

char auth[] = "e5621a7be632";     //Blinker APP（连接小爱同学必须阿里云服务器）中获取到的Secret Key(密钥)
//代码配网用这段
//const char* ssid     = "baoli2303";//无线名称
//const char* password = "wwwhhh123";//

//定义继电器信号
#define Relay_1 2         //定义继电器1信号接入GPIO5，D1
#define Relay_2 0         //定义继电器2信号接入GPIO4，D2
#define Relay_3 4         //定义继电器3信号接入GPIO0，D3
#define Relay_4 5         //定义继电器4信号接入GPIO2，D4




//这里是贝壳物联
#include <aJSON.h>
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
const unsigned long updateInterval = 5000; // 每60秒向服务器发送一次数据
boolean isCheckIn = false;

const char* host = "www.bigiot.net";
const int httpPort = 8181;
String PPP = "play";
String SSS = "stop";

WiFiClient biglotclient;



// 新建Blinker软件组件对象
BlinkerButton Button("MainSwitch");     //组件对象,要和APP组件中的“数据键名”一致,总开关
BlinkerButton Button1("Switch1");       //组件对象,要和APP组件中的“数据键名”一致，开关1
BlinkerButton Button2("Switch2");       //组件对象,要和APP组件中的“数据键名”一致，开关2
BlinkerButton Button3("Switch3");       //组件对象,要和APP组件中的“数据键名”一致，开关3
BlinkerButton Button4("Switch4");       //组件对象,要和APP组件中的“数据键名”一致，开关4
BlinkerButton Button5("Refresh");       //APP端按钮状态刷新

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
      if (digitalRead(Relay_1) == HIGH)
      {
        Button1.color("#FFFF00");   //设置app按键是纯黄色，16进制颜色码
        Button1.text("插座1开");          //设置app按键注释“开”
        Button1.print("off");
        oState[1] = true;
      }
      else if (digitalRead(Relay_1 == LOW))
      {
        Button1.color("#808080");   //设置app按键是灰色，16进制颜色码
        Button1.text("插座1关");          //设置app按键注释“关”
        Button1.print("on");
        oState[1] = false;
      }
      break;
    case  2:
      if (digitalRead(Relay_2) == HIGH)
      {
        Button2.color("#FFFF00");   //设置app按键是纯黄色，16进制颜色码
        Button2.text("插座2开");          //设置app按键注释“开”
        Button2.print("off");
        oState[2] = true;
      }
      else if (digitalRead(Relay_2 == LOW))
      {
        Button2.color("#808080");   //设置app按键是灰色，16进制颜色码
        Button2.text("插座2关");          //设置app按键注释“关”
        Button2.print("on");
        oState[2] = false;
      }
      break;
    case  3:
      if (digitalRead(Relay_3) == HIGH)
      {
        Button3.color("#FFFF00");   //设置app按键是纯黄色，16进制颜色码
        Button3.text("插座3开");          //设置app按键注释“开”
        Button3.print("off");
        oState[3] = true;
      }
      else if (digitalRead(Relay_3 == LOW))
      {
        Button3.color("#808080");   //设置app按键是灰色，16进制颜色码
        Button3.text("插座3关");          //设置app按键注释“关”
        Button3.print("on");
        oState[3] = false;
      }
      break;
    case  4:
      if (digitalRead(Relay_4) == HIGH)
      {
        Button4.color("#FFFF00");   //设置app按键是纯黄色，16进制颜色码
        Button4.text("插座4开");          //设置app按键注释“开”
        Button4.print("off");
        oState[4] = true;
      }
      else if (digitalRead(Relay_4 == LOW))
      {
        Button4.color("#808080");   //设置app按键是灰色，16进制颜色码
        Button4.text("插座4关");          //设置app按键注释“关”
        Button4.print("on");
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
      digitalWrite(Relay_3, io_level);//控制继电器3
      digitalWrite(Relay_4, io_level);//控制继电器4
      break;
    case 1:  //插座插孔一
      digitalWrite(Relay_1, io_level);//控制继电器1
      break;
    case 2:  //插座插孔二
      digitalWrite(Relay_2, io_level);//控制继电器2
      break;
    case 3:  //插座插孔三
      digitalWrite(Relay_3, io_level);//控制继电器3
      break;
    case 4:  //插座插孔四
      digitalWrite(Relay_4, io_level);//控制继电器4
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
    ctrl_multi_outlet(num, HIGH);//打开继电器，num表示是多少路（继电器高电平出发）
    BlinkerMIOT.powerState("on", num);
    BlinkerMIOT.print();
    RelayState(num);

    oState[num] = true;
  }
  else if (state == BLINKER_CMD_OFF)
  {
    ctrl_multi_outlet(num, LOW);//关闭继电器，num表示是多少路

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
void button_callback(const String & state)
{
  BLINKER_LOG("操作了MainSwitch: ", state);//APP中的Monitor控件打印的信息
  if (state == "on")
  {
    ctrl_multi_outlet(OUTLET_ALL, HIGH);//打开继电器--所有
    // 反馈继电器状态
    Button.color("#FFFF00");   //设置app按键是纯黄色，16进制颜色码
    Button.text("开");          //设置app按键注释“开”
    Button.print("on");
  } else if (state == "off")
  {
    ctrl_multi_outlet(OUTLET_ALL, LOW);//关闭继电器--所有
    // 反馈继电器状态
    Button.color("#808080");   //设置app按键是纯黄色，16进制颜色码
    Button.text("关");          //设置app按键注释“开”
    Button.print("off");
  }
}

// 在APP控制，按下Switch1按键即会执行该函数--第1路开关
void button1_callback(const String & state)
{
  BLINKER_LOG("get button state: ", state);//APP中的Monitor控件打印的信息
  if (state == "on")
  {
    ctrl_multi_outlet(OUTLET_NO_1, HIGH);//打开继电器--第1路
    // 反馈继电器1状态
    RelayState(1);    //调用继电器反馈程序
  } else if (state == "off")
  {
    ctrl_multi_outlet(OUTLET_NO_1, LOW);//关闭继电器--第1路
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
    ctrl_multi_outlet(OUTLET_NO_2, HIGH);//打开继电器--第2路
    // 反馈继电器状态
    RelayState(2);    //调用继电器反馈程序
  } else if (state == "off")
  {
    ctrl_multi_outlet(OUTLET_NO_2, LOW);//关闭继电器--第2路
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
    ctrl_multi_outlet(OUTLET_NO_3, HIGH);//打开继电器--第3路
    // 反馈继电器状态
    RelayState(3);    //调用继电器反馈程序
  } else if (state == "off")
  {
    ctrl_multi_outlet(OUTLET_NO_3, LOW);//关闭继电器--第3路
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
    ctrl_multi_outlet(OUTLET_NO_4, HIGH);//打开继电器--第4路
    // 反馈继电器状态
    RelayState(4);    //调用继电器反馈程序
  } else if (state == "off")
  {
    ctrl_multi_outlet(OUTLET_NO_4, LOW);//关闭继电器--第4路
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

//心跳包刷新状态
void heartbeat()
{
  for (int i = 0; i < 5; i++)
  {
    RelayState(i);
  }
}

template <class T>int array2int(T& arr)
{
  int sum = 0, n = sizeof(arr) / sizeof(arr[0]);
  for (int i = 0; i < n; i++)
    sum += (arr[i] - '0') * pow(2, n - 1 - i);
  return sum;
}

void setup()
{
  // 初始化串口，用于调试，后期可删除
  Serial.begin(115200);
#if defined(BLINKER_PRINT)
  BLINKER_DEBUG.stream(BLINKER_PRINT);
#endif

  // 初始化有继电器的IO
  pinMode(Relay_1, OUTPUT);
  pinMode(Relay_2, OUTPUT);
  pinMode(Relay_3, OUTPUT);
  pinMode(Relay_4, OUTPUT);
  //初始化继电器初始状态
  digitalWrite(Relay_1, LOW);       //继电器为低电平触发，初始化为LOW
  digitalWrite(Relay_2, LOW);
  digitalWrite(Relay_3, LOW);
  digitalWrite(Relay_4, LOW);


  //初始化blinker
  Blinker.begin(auth);              //手机配网用这段
  //Blinker.begin(auth, ssid, pswd);//代码配网用这段

  //已连接上wifi了
  //处理贝壳物联wifi连接
  while (!biglotclient.connect(host, httpPort)) {
    delay(500);
  }

  Button.attach(button_callback);   //绑定按键回调
  Button1.attach(button1_callback); //绑定按键回调
  Button2.attach(button2_callback); //绑定按键回调
  Button3.attach(button3_callback); //绑定按键回调
  Button4.attach(button4_callback); //绑定按键回调
  Button5.attach(button5_callback);

  //小爱同学注册回调
  BlinkerMIOT.attachPowerState(miotPowerState); //注册小爱电源回调
  BlinkerMIOT.attachQuery(miotQuery);           //小爱设备查询的回调函数

  //心跳包，初始化
  Blinker.attachHeartbeat(heartbeat);           //app定时向设备发送心跳包, 设备收到心跳包后会返回设备当前状态
}


void biglot_run() {
  if (!biglotclient.connected()) {
    if (!biglotclient.connect(host, httpPort)) {
      delay(5000);
      return;
    }
  }

  if (millis() - lastCheckInTime > postingInterval || lastCheckInTime == 0) {
    checkIn();
  }
//  if ((millis() - lastUpdateTime > updateInterval && isCheckIn)) {
//    String con = String(digitalRead(Relay_1)) + String(digitalRead(Relay_2)) + String(digitalRead(Relay_3)) + String(digitalRead(Relay_4));
//    char num[] = { con.charAt(0), con.charAt(1), con.charAt(2), con.charAt(3)};
//    int dat = array2int(num);
//    Serial.println(dat);
//    update1(DEVICEID, INPUTID, dat);
//  }
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
      digitalWrite(0, LOW);
      sayToClient(F_C_ID, "PIN0 on!");
    } else if (C == "stop") {
      digitalWrite(0, HIGH);
      sayToClient(F_C_ID, "PIN0 off!");
    } else if (C == "status") {
      String con = String(digitalRead(Relay_1)) + String(digitalRead(Relay_2)) + String(digitalRead(Relay_3)) + String(digitalRead(Relay_4));
      sayToClient(F_C_ID, con);
    }
    else if (C.length() == 5) {
      //play0-3 or stop0-3
      if (C.substring(0, 4) == PPP) {
        digitalWrite(String(C.charAt(4)).toInt(), LOW);
        sayToClient(F_C_ID, "PIN" + String(C.charAt(4)) + "on");
      } else if (C.substring(0, 4) == SSS) {
        digitalWrite(String(C.charAt(4)).toInt(), HIGH);
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

void loop()
{
  //biglot_run();         //贝壳物联连接
  Blinker.run();
}
