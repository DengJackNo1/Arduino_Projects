#include <ArduinoJson.h>
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>

const char ssid[] = "baoli2303";  //WIFI名称 修改这2个就可以了
const char pass[] = "wwwhhh123";  //WIFI密码 弃置不用,使用微信配网即可了

// 需要修改

#include "font/myfont_12.h" //启用我的字体 12*12汉字
#include "font/shumaguan_12.h"
#include "font/shumaguan_15.h"

#include "img/weixin_qr_128x128.h" //微信配网二维码


#include "img/Gif/hutaoyao.h"  //胡桃摇 Gif_Mode 1
#include "img/Gif/chuyin_80x80.h"  //初音 Gif_Mode 2
#include "img/Gif/dahutao_80x80.h"  //打胡桃 Gif_Mode 3
#include "img/Gif/girlzhuanzhuan_80x80.h"  //美少女转转 Gif_Mode 4
#include "img/Gif/yuanshenmianyi_80x80.h"  //原神免疫 Gif_Mode 5
//#include "img/Gif/tuniang_80x80.h"  //兔娘 Gif_Mode 6 太多动画




#include "img/weathercode.h" //天气代码对应图标
#include "img/temperature.h"
#include "img/humidity.h"
//#include "img/watch_top.h"
//#include "img/watch_bottom.h"

#include <TFT_eSPI.h>
#include <SPI.h>


TFT_eSPI tft = TFT_eSPI();  // 引脚请自行配置tft_espi库中的 User_Setup.h文件
TFT_eSprite clk = TFT_eSprite(&tft);

#include <TJpg_Decoder.h>






int anniu_status =  0;//状态判断
unsigned int Gif_Mode = 1;  //动画
int prevTime = 0;
uint32_t targetTime = 0;
byte omm = 99;
boolean initial = 1;
byte xcolon = 0;
unsigned int colour = 0;

int BL_PIN = 16; //背光引脚为GPIO16 已经固定了
int bl_value = 80; //默认背光为80
int Change_Gif_Pin = 0; //修改动画的GPIO中断引脚为0


WiFiClient client; //新添加
uint16_t bgColor = 0xFFFF;
String cityCode = "101200101"; //武汉,天气城市代码
String cityName = "武汉"; //得定义城市名称,和字符串分割有关


//NTP服务器
static const char ntpServerName[] = "ntp6.aliyun.com";
const int timeZone = 8;             //东八区

WiFiUDP Udp;
unsigned int localPort = 8000;
time_t getNtpTime();
int weatherCode = 99;
void digitalClockDisplay();
void printDigits(int digits);
String num2str(int digits);
void sendNTPpacket(IPAddress &address);

//展示bmp位图
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
  if ( y >= tft.height() ) return 0;
  tft.pushImage(x, y, w, h, bitmap);
  // Return 1 to decode next block
  return 1;
}


byte loadNum = 6;//进度条初始值
//进度条
void loading(byte delayTime) {
  clk.setColorDepth(8);

  clk.createSprite(120, 30);
  clk.fillSprite(0x0000);

  clk.drawRoundRect(10, 0, 100, 8, 4, 0xFFFF);
  clk.fillRoundRect(10, 0, loadNum, 8, 4, 0xFFFF);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_GREEN, 0x0000);
  clk.drawString("Connecting WiFi", 60, 15, 1);
  clk.pushSprite(4, 40);
  clk.deleteSprite();
  loadNum += 1;
  if (loadNum >= 100) {
    loadNum = 100;
  }
  delay(delayTime);
}


void SmartConfig()
{
  TJpgDec.drawJpg(0, 0, weixin_qr_128x128, sizeof(weixin_qr_128x128)); //显示微信配网图片
  WiFi.mode(WIFI_STA);
  delay(100);
  Serial.println("\r\nWait for Smartconfig...");
  WiFi.beginSmartConfig();
  while (!WiFi.smartConfigDone()) {
    delay(500);
    Serial.print(".");
  }
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("SmartConfig Success");
  Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
  Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
}


bool AutoConfig()
{
  WiFi.begin();
  //如果觉得时间太长可改
  for (int i = 0; i < 40; i++)
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      loading(500);
      if (loadNum >= 90) {
        loadNum = 90;
      }
      Serial.print(".");
    } else {
      while (loadNum < 100) { //让动画走完
        loading(1);
      }
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




uint32_t t = 0;
int i = 0;


ICACHE_RAM_ATTR void callback() {

  if (((millis() - t) > 300) || ((millis() - t) < 0))
  {
    i++;  //按键中断计数
    t = millis();

    Gif_Mode++;
    if (Gif_Mode == 6) {
      Gif_Mode = 1;
    }
    Serial.print(">>  Change Gif_Mode as");
    Serial.print(Gif_Mode);
    Serial.println("!!!  <<");

    /*
      bl_value+= 10;
      if (bl_value == 110) {
      bl_value = 10;
      }
      analogWrite(BL_PIN, bl_value);

      Serial.print(">>  Change bl_value as  <<");
      Serial.print(bl_value);
      Serial.println("!!!  <<");
    */
  }
}


struct station_config station_info;
bool flag;  //判断状态,从flash读取ssid,能读取到进入自动重连环节,读取不到直接展示微信配网二维码
void setup() {
  Serial.begin(115200);

  wifi_station_get_config(&station_info); // 读取默认配置
  bool flag = String((char*)station_info.ssid) == "";

  pinMode(BL_PIN, OUTPUT);
  analogWriteRange(100);//背光0-100范围
  analogWrite(BL_PIN, bl_value);







  tft.init();
  tft.setRotation(0);//顺时针旋转
  //tft.invertDisplay(true); //反转颜色
  tft.fillScreen(0x0000);

  TJpgDec.setJpgScale(1);
  TJpgDec.setSwapBytes(true);
  TJpgDec.setCallback(tft_output);
  tft.setTextColor(TFT_BLACK, bgColor);





  Serial.println("start connect WIFI ");
  //  Serial.println(ssid);
  //  WiFi.begin(ssid, pass);

  //  while (WiFi.status() != WL_CONNECTED) {
  //    for (byte n = 0; n < 10; n++) { //每500毫秒检测一次状态
  //      loading(500);
  //      if (loadNum >= 90) {
  //        loadNum = 90;
  //      }      //防止连接时间太长
  //    }
  //  }

  //  while (loadNum < 100) { //让动画走完
  //    loading(1);
  //  }


  if (flag) {
    SmartConfig();
  } else {
    if (!AutoConfig()) {
      SmartConfig();
    }
  }


  targetTime = millis() + 1000;

  Serial.print("本地IP： ");
  Serial.println(WiFi.localIP());
  Serial.println("启动UDP");
  Udp.begin(localPort);
  Serial.print("端口号: ");
  Serial.println(Udp.localPort());
  Serial.println("等待同步...");

  setSyncProvider(getNtpTime);
  setSyncInterval(300);


  //绘制一个视口
  tft.setViewport(0, 0, 128, 128);
  tft.fillScreen(0x0000);
  tft.fillRoundRect(0, 0, 128, 128, 0, bgColor); //实心圆角矩形
  //tft.resetViewport();

  //绘制线框
  tft.drawFastHLine(0, 15, 128, TFT_BLACK); //横线
  tft.drawFastHLine(0, 98, 128, TFT_BLACK);
  tft.drawFastHLine(0, 113, 128, TFT_BLACK);



  tft.drawFastVLine(91, 0, 14, TFT_BLACK); //竖线
  tft.drawFastVLine(35, 98, 14, TFT_BLACK);
  tft.drawFastVLine(87, 98, 14, TFT_BLACK);

  /*
     显示图片的像素

    uint16_t w = 0, h = 0;
    TJpgDec.getJpgSize(&w, &h, temperature, sizeof(temperature));
    Serial.print("Width = "); Serial.print(w); Serial.print(", height = "); Serial.println(h);
    TJpgDec.getJpgSize(&w, &h, humidity, sizeof(humidity));
    Serial.print("Width = "); Serial.print(w); Serial.print(", height = "); Serial.println(h);
  */
  getCityCode();  //获取城市代码
  TJpgDec.drawJpg(88, 100, temperature, sizeof(temperature)); //温度图标
  TJpgDec.drawJpg(88, 85, humidity, sizeof(humidity)); //湿度图标

  //控制动画修改的io脚,连接gpio0
  pinMode(Change_Gif_Pin, INPUT_PULLUP);
  attachInterrupt(Change_Gif_Pin, callback, FALLING); //中断
  //pinMode(interruptPin, INPUT);
  //attachInterrupt(interruptPin, handleInterrupt, FALLING);
  //pinMode(0, INPUT); //修改为输入上拉.默认高电平
  //digitalWrite(4, HIGH);
  //pinMode( pinInterrupt, INPUT);//设置管脚为输入


}


time_t prevDisplay = 0; // 显示时间
unsigned long weaterTime = 0;
unsigned long Gif_ModeTime = 0;
void loop() {
  if (timeStatus() != timeNotSet) {
    if (now() != prevDisplay) { //update the display only if time has changed
      prevDisplay = now();
      digitalClockDisplay();
    }
  }
  if (millis() - weaterTime > 1800000) { //30分钟更新一次天气
    weaterTime = millis();
    getCityWeater();
  }

  scrollBanner(); // 左上角信息展示
  ButtonscrollBanner(); //底部信息展示
  imgDisplay();
}




// 发送HTTP请求并且将服务器响应通过串口输出
void getCityCode() {
  int OldConnectionTimes = millis(), NewConnectionTimes = 0;


  //创建 HTTPClient 对象
  HTTPClient httpClient;
  String URL = "http://wgeo.weather.com.cn/ip/?_=" + String(now());

  //配置请求地址。此处也可以不使用端口号和PATH而单纯的
  httpClient.begin(client, URL);

  //设置请求头中的User-Agent
  httpClient.setUserAgent("Mozilla/5.0 (iPhone; CPU iPhone OS 11_0 like Mac OS X) AppleWebKit/604.1.38 (KHTML, like Gecko) Version/11.0 Mobile/15A372 Safari/604.1");
  httpClient.addHeader("Referer", "http://www.weather.com.cn/");

  //启动连接并发送HTTP请求
  int httpCode = httpClient.GET();
  //Serial.print("Send GET request to URL: ");
  //Serial.println(URL);
  Serial.println("数据请求中...");

  //如果服务器响应OK则从服务器获取响应体信息并通过串口输出
  if (httpCode == HTTP_CODE_OK) {
    String str = httpClient.getString();
    //Serial.println(str);
    int aa = str.indexOf("id=");
    if (aa > -1) {
      //cityCode = str.substring(aa+4,aa+4+9);     //注释掉，不然天气默认地址为北京
      //Serial.println(cityCode);
      Serial.println("获取城市代码成功");
      getCityWeater();
    } else {
      Serial.println("获取城市代码失败，正在重新获取...");
    }
  } else {
    Serial.print("请求城市代码错误：");
    Serial.println(String(httpCode) + "正在重新获取...");
  }
  //连接时长超过5秒，直接重启重新连接
  NewConnectionTimes = millis();
  if ((NewConnectionTimes - OldConnectionTimes) >= 5000) {
    ESP.restart(); //重启ESP32
  }
  //关闭ESP8266与服务器连接
  httpClient.end();
}

// 获取城市天气
int Warn_Number1 = 0, Warn_Value1 = 0, Warn_Number2 = 0, Warn_Value2 = 0, Warn_Flag = 0;
void getCityWeater() {
  int OldConnectionTimes = millis(), NewConnectionTimes = 0;
  HTTPClient httpClient;
  String URL = "http://d1.weather.com.cn/weather_index/" + cityCode + ".html?_=" + String(now());
  //Sing URL = "http://192.168.0.112:7777/index?1=7" + cityCode + ".html?_=" + String(now());

  //创建 HTTPClient 对象

  httpClient.begin(client, URL);

  //设置请求头中的User-Agent
  httpClient.setUserAgent("Mozilla/5.0 (iPhone; CPU iPhone OS 11_0 like Mac OS X) AppleWebKit/604.1.38 (KHTML, like Gecko) Version/11.0 Mobile/15A372 Safari/604.1");
  httpClient.addHeader("Referer", "http://www.weather.com.cn/");

  //启动连接并发送HTTP请求
  int httpCode = httpClient.GET();
  Serial.println("正在获取天气数据");
  Serial.println(URL);

  //如果服务器响应OK则从服务器获取响应体信息并通过串口输出
  if (httpCode == HTTP_CODE_OK) {

    String str = httpClient.getString();
    //Serial.println(str);

    int indexStart = str.indexOf("weatherinfo\":");
    int indexEnd = str.indexOf("};var alarmDZ");
    String jsonCityDZ = str.substring(indexStart + 13, indexEnd);
    Serial.println(jsonCityDZ);

    //气象预警不同时间会发布不同的预警信息，只会显示最新的一个，显示多个也只是显示最新时间的前一个预警，没必要了
    indexStart = str.indexOf("alarmDZ ={\"w\":[");
    indexEnd = str.indexOf("]};var dataSK");
    String jsonDataWarn1 = str.substring(indexStart + 14, indexEnd + 1);
    Serial.println(jsonDataWarn1);
    if (jsonDataWarn1.length() >= 40) {
      Warn_Flag = 1;
    }
    else {
      Warn_Flag = 0;
    }

    indexStart = str.indexOf("dataSK =");
    indexEnd = str.indexOf(";var dataZS");
    String jsonDataSK = str.substring(indexStart + 8, indexEnd);
    Serial.println(jsonDataSK);

    indexStart = str.indexOf("\"f\":[");
    indexEnd = str.indexOf(",{\"fa");
    String jsonFC = str.substring(indexStart + 5, indexEnd);
    Serial.println(jsonFC);

    indexStart = str.indexOf(";var dataZS ={\"zs\":");
    String split_str = ",\"cn\":\"" + cityName + "\"};var fc =";
    indexEnd = str.indexOf(split_str);
    String jsonSuggest = str.substring(indexStart + 19, indexEnd);
    Serial.println(jsonSuggest);

    weaterData(&jsonCityDZ, &jsonDataSK, &jsonFC, &jsonSuggest, &jsonDataWarn1);
    Serial.println("天气数据获取成功");
  } else {
    Serial.print("请求城市天气错误：");
    Serial.println(String(httpCode) + " 正在重新获取...");
  }
  //连接时长超过5秒，直接重启重新连接
  NewConnectionTimes = millis();
  if ((NewConnectionTimes - OldConnectionTimes) >= 5000) {
    //ESP.restart(); //重启ESP32
  }
  //关闭ESP8266与服务器连接
  httpClient.end();
}

void getLunarCalendar() {
  HTTPClient httpClient;
  String URL = "https://api.xlongwei.com/service/datetime/convert.json";
  //创建 HTTPClient 对象

  httpClient.begin(client, URL);

  //设置请求头中的User-Agent
  httpClient.setUserAgent("Mozilla/5.0 (iPhone; CPU iPhone OS 11_0 like Mac OS X) AppleWebKit/604.1.38 (KHTML, like Gecko) Version/11.0 Mobile/15A372 Safari/604.1");
  httpClient.addHeader("Referer", "http://www.weather.com.cn/");

  //启动连接并发送HTTP请求
  int httpCode = httpClient.GET();
  Serial.println("正在获取天气数据");
  Serial.println(URL);

  //如果服务器响应OK则从服务器获取响应体信息并通过串口输出
  if (httpCode == HTTP_CODE_OK) {
    String str = httpClient.getString();
    Serial.println("农历" + str);
  }
}


String scrollText[6];
String ButtonScrollText[9]; //最后一个为天气警报
uint16_t CITY_COLOR = 0x0000;  //城市文字颜色
uint16_t weatherWarnBgColor1;  //天气警报文字颜色
byte ButtoncurrentMaxIndex = 7; //默认为7
//int scrollTextWidth = 0;
//天气信息写到屏幕上
void weaterData(String *cityDZ, String *dataSK, String *dataFC, String *dataSuggest, String *dataWarn1) {

  DynamicJsonDocument doc(8192);

  DeserializationError error = deserializeJson(doc, *dataSK);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
  }
  JsonObject sk = doc.as<JsonObject>();
  /***绘制相关文字***/

  clk.setColorDepth(8);
  //温度
  clk.createSprite(30, 13);
  clk.fillSprite(bgColor);
  clk.loadFont(myfont_12); //加载font/我新建的字体
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_BLACK, bgColor);
  clk.drawString(sk["temp"].as<String>() + "℃", 15, 7);
  clk.pushSprite(99, 99);


  //湿度
  clk.createSprite(30, 13);
  clk.fillSprite(bgColor);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_BLACK, bgColor);
  clk.drawString(sk["SD"].as<String>(), 15, 6);
  //clk.drawString("100%",28,13);
  clk.pushSprite(99, 84);
  clk.deleteSprite();

  //城市名称
  clk.createSprite(36, 14); //88,32
  clk.fillSprite(bgColor);
  clk.setTextDatum(CC_DATUM);
  if (Warn_Flag)
  {
    CITY_COLOR = 0xF800; //红色
    ButtoncurrentMaxIndex = 8;
  }
  else
  {
    CITY_COLOR = 0x0000;
    ButtoncurrentMaxIndex = 7;
  }

  clk.setTextColor(CITY_COLOR, bgColor);
  //clk.drawString(sk["cityname"].as<String>()+"区",44,18);
  clk.drawString(sk["cityname"].as<String>(), 18, 7);
  clk.pushSprite(92, 1);
  clk.deleteSprite();

  //PM2.5空气指数
  uint16_t pm25BgColor;//优
  String aqiTxt;
  int pm25V = sk["aqi"];
  //Serial.println("pm25V:" + String(pm25V));
  if (pm25V >= 301) {
    pm25BgColor = tft.color565(255, 36, 0); //重度
    aqiTxt = "严重";
  } else if (pm25V >= 201 & pm25V <= 300) {
    pm25BgColor = tft.color565(136, 11, 32); //重度
    aqiTxt = "重度";
  } else if (pm25V >= 151 & pm25V <= 200) {
    pm25BgColor = tft.color565(186, 55, 121); //中度
    aqiTxt = "中度";
  } else if (pm25V >= 101 & pm25V <= 160) {
    pm25BgColor = tft.color565(242, 159, 57); //轻
    aqiTxt = "轻度";
  } else if (pm25V >= 51 & pm25V <= 100) {
    pm25BgColor = tft.color565(247, 219, 100); //良
    aqiTxt = "良";
  } else if (pm25V >= 0 & pm25V <= 50) {
    pm25BgColor = tft.color565(156, 202, 127); //优
    aqiTxt = "优";
  }
  clk.createSprite(26, 14);
  clk.fillSprite(bgColor);
  clk.fillRect(0, 0, 24, 15, pm25BgColor);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(0xFFFF);
  clk.drawString(aqiTxt, 12, 6);
  clk.pushSprite(100, 70);
  clk.deleteSprite();

  //左上角滚动字幕
  //解析第二段JSON
  scrollText[0] = "实时天气 " + sk["weather"].as<String>();
  scrollText[1] = "空气质量 " + aqiTxt;

  scrollText[2] = "风向 " + sk["WD"].as<String>() + sk["WS"].as<String>();

  deserializeJson(doc, *cityDZ);

  JsonObject dz = doc.as<JsonObject>();
  scrollText[3] = "今日 " + dz["weather"].as<String>();

  //显示天气图标
  String weatherCodeText = dz["weathercode"].as<String>();
  weatherCode = weatherCodeText.substring(1, weatherCodeText.length() + 1).toInt();

  //Serial.println(weatherCode);
  switch (weatherCode) {
    case 0: TJpgDec.drawJpg(100, 50, d00_26x20, sizeof(d00_26x20)); break;
    case 1: TJpgDec.drawJpg(100, 50, d01_26x20, sizeof(d01_26x20)); break;
    case 2: TJpgDec.drawJpg(100, 50, d02_26x20, sizeof(d02_26x20)); break;
    case 3: TJpgDec.drawJpg(100, 50, d03_26x20, sizeof(d03_26x20)); break;
    case 4: TJpgDec.drawJpg(100, 50, d04_26x20, sizeof(d04_26x20)); break;
    case 5: TJpgDec.drawJpg(100, 50, d05_26x20, sizeof(d05_26x20)); break;
    case 6: TJpgDec.drawJpg(100, 50, d06_26x20, sizeof(d06_26x20)); break;
    case 7: TJpgDec.drawJpg(100, 50, d07_26x20, sizeof(d07_26x20)); break;
    case 8: TJpgDec.drawJpg(100, 50, d08_26x20, sizeof(d08_26x20)); break;
    case 9: TJpgDec.drawJpg(100, 50, d09_26x20, sizeof(d09_26x20)); break;
    case 10: TJpgDec.drawJpg(100, 50, d10_26x20, sizeof(d10_26x20)); break;
    case 11: TJpgDec.drawJpg(100, 50, d11_26x20, sizeof(d11_26x20)); break;
    case 12: TJpgDec.drawJpg(100, 50, d12_26x20, sizeof(d12_26x20)); break;
    case 13: TJpgDec.drawJpg(100, 50, d13_26x20, sizeof(d13_26x20)); break;
    case 14: TJpgDec.drawJpg(100, 50, d14_26x20, sizeof(d14_26x20)); break;
    case 15: TJpgDec.drawJpg(100, 50, d15_26x20, sizeof(d15_26x20)); break;
    case 16: TJpgDec.drawJpg(100, 50, d16_26x20, sizeof(d16_26x20)); break;
    case 17: TJpgDec.drawJpg(100, 50, d17_26x20, sizeof(d17_26x20)); break;
    case 18: TJpgDec.drawJpg(100, 50, d18_26x20, sizeof(d18_26x20)); break;
    case 19: TJpgDec.drawJpg(100, 50, d19_26x20, sizeof(d19_26x20)); break;
    case 20: TJpgDec.drawJpg(100, 50, d20_26x20, sizeof(d20_26x20)); break;
    case 21: TJpgDec.drawJpg(100, 50, d21_26x20, sizeof(d21_26x20)); break;
    case 22: TJpgDec.drawJpg(100, 50, d22_26x20, sizeof(d22_26x20)); break;
    case 23: TJpgDec.drawJpg(100, 50, d23_26x20, sizeof(d23_26x20)); break;
    case 24: TJpgDec.drawJpg(100, 50, d24_26x20, sizeof(d24_26x20)); break;
    case 25: TJpgDec.drawJpg(100, 50, d25_26x20, sizeof(d25_26x20)); break;
    case 26: TJpgDec.drawJpg(100, 50, d26_26x20, sizeof(d26_26x20)); break;
    case 27: TJpgDec.drawJpg(100, 50, d27_26x20, sizeof(d27_26x20)); break;
    case 28: TJpgDec.drawJpg(100, 50, d28_26x20, sizeof(d28_26x20)); break;
    case 29: TJpgDec.drawJpg(100, 50, d29_26x20, sizeof(d29_26x20)); break;
    case 30: TJpgDec.drawJpg(100, 50, d30_26x20, sizeof(d30_26x20)); break;
    case 31: TJpgDec.drawJpg(100, 50, d31_26x20, sizeof(d31_26x20)); break;
    case 32: TJpgDec.drawJpg(100, 50, d32_26x20, sizeof(d32_26x20)); break;
    case 33: TJpgDec.drawJpg(100, 50, d33_26x20, sizeof(d33_26x20)); break;
    case 49: TJpgDec.drawJpg(100, 50, d49_26x20, sizeof(d49_26x20)); break;
    case 53: TJpgDec.drawJpg(100, 50, d53_26x20, sizeof(d53_26x20)); break;
    case 54: TJpgDec.drawJpg(100, 50, d54_26x20, sizeof(d54_26x20)); break;
    case 55: TJpgDec.drawJpg(100, 50, d55_26x20, sizeof(d55_26x20)); break;
    case 56: TJpgDec.drawJpg(100, 50, d56_26x20, sizeof(d56_26x20)); break;
    case 57: TJpgDec.drawJpg(100, 50, d57_26x20, sizeof(d57_26x20)); break;
    case 58: TJpgDec.drawJpg(100, 50, d58_26x20, sizeof(d58_26x20)); break;
    case 301: TJpgDec.drawJpg(100, 50, d301_26x20, sizeof(d301_26x20)); break;
    case 302: TJpgDec.drawJpg(100, 50, d302_26x20, sizeof(d302_26x20)); break;
    default: break;
  }

  deserializeJson(doc, *dataFC);
  JsonObject fc = doc.as<JsonObject>();

  scrollText[4] = "最低温度 " + fc["fd"].as<String>() + "℃";
  scrollText[5] = "最高温度 " + fc["fc"].as<String>() + "℃";

  //scrollText[6] = "PM2.5 "+sk["aqi"].as<String>();  //显示pm2.5指数


  //底部滚动显示不需要
  deserializeJson(doc, *dataSuggest);
  JsonObject dataSuggestJson = doc.as<JsonObject>();

  ButtonScrollText[0] = dataSuggestJson["lk_name"].as<String>() + " " + dataSuggestJson["lk_hint"].as<String>();
  ButtonScrollText[1] = dataSuggestJson["cl_name"].as<String>() + " " + dataSuggestJson["cl_hint"].as<String>();
  ButtonScrollText[2] = dataSuggestJson["uv_name"].as<String>() + " " + dataSuggestJson["uv_hint"].as<String>();
  ButtonScrollText[3] = dataSuggestJson["ct_name"].as<String>() + " " + dataSuggestJson["ct_hint"].as<String>();
  ButtonScrollText[4] = dataSuggestJson["gm_name"].as<String>() + " " + dataSuggestJson["gm_hint"].as<String>();
  ButtonScrollText[5] = dataSuggestJson["ys_name"].as<String>() + " " + dataSuggestJson["ys_hint"].as<String>();
  //ButtonScrollText[6] = dataSuggestJson["gz_name"].as<String>() + " " + dataSuggestJson["gz_hint"].as<String>();
  //ButtonScrollText[6] = dataSuggestJson["cl_name"].as<String>() + " " + dataSuggestJson["cl_hint"].as<String>();
  ButtonScrollText[6] = dataSuggestJson["pl_name"].as<String>() + " " + dataSuggestJson["pl_hint"].as<String>();
  ButtonScrollText[7] = dataSuggestJson["co_name"].as<String>() + " " + dataSuggestJson["co_hint"].as<String>();

  error = deserializeJson(doc, *dataWarn1);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
  }
  JsonArray dataWarnjson1 = doc.as<JsonArray>();
  Serial.print("Warn_Flag is");
  Serial.println(Warn_Flag);
  if ((Warn_Flag)) {
    Serial.print("Warn_Flag is 222   >>");
    Serial.println(Warn_Flag);
    JsonObject item = dataWarnjson1.getElement(0);
    Warn_Number1 = item["w4"].as<int>();

    Warn_Value1 = item["w6"].as<int>();
    Serial.println("气象预警：" + String(Warn_Number1) + " 等级1：" + String(Warn_Value1));
    if (item["w5"].as<String>() != "null") {
      String txtMsg = item["w7"].as<String>() + item["w5"].as<String>(); //黄色大雨
      ButtonScrollText[8] = "警报: " + txtMsg;
    }
  }
  //Serial.println(dataWarnjson);
  Serial.println("气象预警编号1：" + String(Warn_Number1) + " 等级1：" + String(Warn_Value1));
  /* switch(Warn_Number) { // 处理图片太TM烦躁了，而且显示的效果也特别差，而且预警代码一直没找到文档
     case 7: //高温
       if(Warn_Value == 1) {TJpgDec.drawJpg(175,110,gaowen_lan, sizeof(gaowen_lan));break;}
       if(Warn_Value == 2) {TJpgDec.drawJpg(175,110,gaowen_huang, sizeof(gaowen_huang));break;}
       if(Warn_Value == 3) {TJpgDec.drawJpg(175,110,gaowen_cheng, sizeof(gaowen_cheng));break;}
       if(Warn_Value == 4) {TJpgDec.drawJpg(175,110,gaowen_hong, sizeof(gaowen_hong));break;}
     //case 9: //雷电
     //case 0: //雷暴大风
     //case 2: //暴雨
     default:TJpgDec.drawJpg(175,110,BG_Color, sizeof(BG_Color));Serial.println("NULL");break;
    }
  */
  switch (Warn_Value1) { //这等级把我搞蒙了，一会蓝色是0，一会又变成1
    //填充颜色
    case 1: weatherWarnBgColor1 = tft.color565(0, 128, 255); break; //蓝色
    case 2: weatherWarnBgColor1 = tft.color565(255, 204, 51); break; //黄色
    case 3: weatherWarnBgColor1 = tft.color565(255, 153, 0); break; //橙色
    case 4: weatherWarnBgColor1 = tft.color565(255, 0, 0); break; //红色
    default: Serial.println("WarnBgColor is NULL"); break;
  }

}

//竖直显示文字
void shuzhiwenzi() {
  clk.createSprite(12, 68); //竖直方向上最多只有70可以用
  clk.fillSprite(bgColor);
  clk.setTextWrap(true, true); // 竖直显示,通过设置水平自动换行
  clk.setTextDatum(0);
  clk.setTextColor(TFT_BLACK, bgColor);
  String txtMsg = "黄色大雨";
  ButtonScrollText[8] = "警报: " + txtMsg;
  int lastStringLength = txtMsg.length();//文字长度,汉字个数*3,和编码有关
  int prefex_length = (int)((68 - lastStringLength / 3 * 10.5) / 2); //设置竖直居中的数据
  clk.drawString(txtMsg, 0, prefex_length);
  clk.pushSprite(114, 16);
  clk.deleteSprite();
  clk.unloadFont();
}


void digitalClockDisplay()
{

  clk.setColorDepth(8);

  /***中间时间区***/
  //时分
  clk.loadFont(shumaguan_15);
  clk.createSprite(48, 16);
  clk.fillSprite(bgColor);

  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_BLACK, bgColor);
  clk.drawString(hourMinute(), 23, 8); //绘制时和分
  //clk.unloadFont();
  clk.pushSprite(81, 16);
  clk.deleteSprite();
  clk.unloadFont();




  //秒
  clk.createSprite(18, 13);
  clk.fillSprite(bgColor);

  clk.loadFont(shumaguan_12);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_BLACK, bgColor);
  clk.drawString(num2str(second()), 9, 7);
  clk.pushSprite(109, 32);
  clk.deleteSprite();



  /***底部***/

  clk.loadFont(myfont_12);
  //星期
  clk.createSprite(34, 13);
  clk.fillSprite(bgColor);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_BLACK, bgColor);
  clk.drawString(week(), 17, 7);
  clk.pushSprite(1, 99);
  clk.deleteSprite();

  //月日
  clk.createSprite(50, 13);
  clk.fillSprite(bgColor);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_BLACK, bgColor);
  clk.drawString(monthDay(), 25, 7);
  clk.pushSprite(36, 99);
  clk.deleteSprite();


  clk.unloadFont();
  /***底部***/
}

//星期
String week() {
  String wk[7] = {"日", "一", "二", "三", "四", "五", "六"};
  String s = "周" + wk[weekday() - 1];
  return s;
}

//月日
String monthDay() {
  String s = String(month());
  s = s + "月" + day() + "日";
  return s;
}
//时分
String hourMinute() {
  String s = num2str(hour());
  s = s + ":" + num2str(minute());
  return s;
}

String num2str(int number) {
  if (number >= 10) {
    return String(number);
  } else {
    return String("0" + String(number));
  }
}
/*
  String num2str(int digits)
  {
  String s = "";
  if (digits < 10)
    s = s + "0";
  s = s + digits;
  return s;
  }*/

void printDigits(int digits)
{
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP时间在消息的前48字节中
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  //Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  //Serial.print(ntpServerName);
  //Serial.print(": ");
  //Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      //Serial.println(secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR);
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // 无法获取时间时返回0
}

// 向NTP服务器发送请求
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}


int currentIndex = 0;
//int prevTime = 0;
TFT_eSprite clkb = TFT_eSprite(&tft);


void scrollBanner() {
  if (millis() - prevTime > 3000) { //3秒切换一次

    if (scrollText[currentIndex]) {

      clkb.setColorDepth(8);
      clkb.loadFont(myfont_12);


      for (int pos = 20; pos > 0 ; pos--) {
        scrollTxt(pos);
      }

      clkb.deleteSprite();
      clkb.unloadFont();


      if (currentIndex >= 5) {
        currentIndex = 0;  //回第一个
      } else {
        currentIndex += 1;  //准备切换到下一个
      }

      //Serial.println(currentIndex);

    }
    prevTime = millis();

  }
}

void scrollTxt(int pos) {
  clkb.createSprite(90, 14);
  clkb.fillSprite(bgColor);
  clkb.setTextWrap(false);
  clkb.setTextDatum(CC_DATUM);
  clkb.setTextColor(TFT_BLACK, bgColor);
  clkb.drawString(scrollText[currentIndex], 45, 7);
  clkb.pushSprite(1, 1);
}

/**
  底部生活信息滚动显示
*/

byte ButtoncurrentIndex = 0;
unsigned long ButtonprevTime = 0;

void ButtonscrollBanner() {
  if (millis() - ButtonprevTime > 5000) { //5秒切换一次

    if (ButtonScrollText[ButtoncurrentIndex]) {
      clkb.loadFont(myfont_12);

      ButtonScrollTxt();
      clkb.unloadFont();
      clkb.deleteSprite();
      if (ButtoncurrentIndex >= ButtoncurrentMaxIndex) {
        ButtoncurrentIndex = 0;  //回第一个
      } else {
        ButtoncurrentIndex += 1;  //准备切换到下一个
      }

      //Serial.println(ButtoncurrentIndex);

    }
    ButtonprevTime = millis();

  }
}

void ButtonScrollTxt() {
  clkb.createSprite(128, 13);
  clkb.fillSprite(bgColor);
  clkb.setTextDatum(CC_DATUM);
  clkb.setTextColor(TFT_BLACK, bgColor);
  clkb.drawString(ButtonScrollText[ButtoncurrentIndex], 64, 7);
  clkb.pushSprite(0, 114);
}




//unsigned int Gif_Mode = 1;
unsigned long oldTime = 0, imgNum = 1;
void imgDisplay() {
  int x, y, dt;
  /*
    switch(Warn_Flag) {//如果有气象预警信息，图标自动左移
    case 0:x = 70;break;
    case 1:
      if(Gif_Mode == 4) {
        x = 65;y = 85;
      }
      if(Gif_Mode == 3) {
        x = 65;y = 85;
      }
      else if(Gif_Mode == 2) {
        x = 80;y = 94;
      }
      else if(Gif_Mode == 2) {
        x = 64;y = 20;
      }
      else if(Gif_Mode == 1) {
        x = 60;y = 94;
      }
      break;
    }
  */
  x = 0;
  y = 16;
  switch (Gif_Mode) { //修改动画的播放速度
    case 1: dt = 50; break;
    case 2: dt = 100; break;
    case 3: dt = 100; break;
    case 4: dt = 100; break;
    case 5: dt = 100; break;
    case 6: dt = 100; break;
  }
  if (millis() - oldTime >= dt) {
    imgNum = imgNum + 1;
    oldTime = millis();
  }

  if (Gif_Mode == 1) { //动画-胡桃摇
    switch (imgNum) {
      case 1: TJpgDec.drawJpg(x, y, hutaoyao_00, sizeof(hutaoyao_00)); break;
      case 2: TJpgDec.drawJpg(x, y, hutaoyao_01, sizeof(hutaoyao_01)); break;
      case 3: TJpgDec.drawJpg(x, y, hutaoyao_02, sizeof(hutaoyao_02)); break;
      case 4: TJpgDec.drawJpg(x, y, hutaoyao_03, sizeof(hutaoyao_03)); break;
      case 5: TJpgDec.drawJpg(x, y, hutaoyao_04, sizeof(hutaoyao_04)); break;
      case 6: TJpgDec.drawJpg(x, y, hutaoyao_05, sizeof(hutaoyao_05)); break;
      case 7: TJpgDec.drawJpg(x, y, hutaoyao_06, sizeof(hutaoyao_06)); break;
      case 8: TJpgDec.drawJpg(x, y, hutaoyao_07, sizeof(hutaoyao_07)); break;
      case 9: TJpgDec.drawJpg(x, y, hutaoyao_08, sizeof(hutaoyao_08)); break;
      case 10: TJpgDec.drawJpg(x, y, hutaoyao_09, sizeof(hutaoyao_09)); break;
      case 11: TJpgDec.drawJpg(x, y, hutaoyao_10, sizeof(hutaoyao_10)); break;
      case 12: TJpgDec.drawJpg(x, y, hutaoyao_11, sizeof(hutaoyao_11)); break;
      case 13: TJpgDec.drawJpg(x, y, hutaoyao_12, sizeof(hutaoyao_12)); break;
      case 14: TJpgDec.drawJpg(x, y, hutaoyao_13, sizeof(hutaoyao_13)); break;
      case 15: TJpgDec.drawJpg(x, y, hutaoyao_14, sizeof(hutaoyao_14)); break;
      case 16: TJpgDec.drawJpg(x, y, hutaoyao_15, sizeof(hutaoyao_15)); break;
      case 17: TJpgDec.drawJpg(x, y, hutaoyao_16, sizeof(hutaoyao_16)); break;
      case 18: TJpgDec.drawJpg(x, y, hutaoyao_17, sizeof(hutaoyao_17)); break;
      case 19: TJpgDec.drawJpg(x, y, hutaoyao_18, sizeof(hutaoyao_18)); break;
      case 20: TJpgDec.drawJpg(x, y, hutaoyao_19, sizeof(hutaoyao_19)); break;
      case 21: TJpgDec.drawJpg(x, y, hutaoyao_20, sizeof(hutaoyao_20)); break;
      case 22: TJpgDec.drawJpg(x, y, hutaoyao_21, sizeof(hutaoyao_21)); break;
      case 23: TJpgDec.drawJpg(x, y, hutaoyao_22, sizeof(hutaoyao_22)); break;
      case 24: TJpgDec.drawJpg(x, y, hutaoyao_23, sizeof(hutaoyao_23)); break;
      case 25: TJpgDec.drawJpg(x, y, hutaoyao_24, sizeof(hutaoyao_24)); break;
      case 26: TJpgDec.drawJpg(x, y, hutaoyao_25, sizeof(hutaoyao_25)); break;
      case 27: TJpgDec.drawJpg(x, y, hutaoyao_26, sizeof(hutaoyao_26)); break;
      case 28: TJpgDec.drawJpg(x, y, hutaoyao_27, sizeof(hutaoyao_27)); break;
      case 29: TJpgDec.drawJpg(x, y, hutaoyao_28, sizeof(hutaoyao_28)); break;
      case 30: TJpgDec.drawJpg(x, y, hutaoyao_29, sizeof(hutaoyao_29)); break;
      case 31: TJpgDec.drawJpg(x, y, hutaoyao_30, sizeof(hutaoyao_30)); break;
      case 32: TJpgDec.drawJpg(x, y, hutaoyao_31, sizeof(hutaoyao_31)); break;
      case 33: TJpgDec.drawJpg(x, y, hutaoyao_32, sizeof(hutaoyao_32)); break;
      case 34: TJpgDec.drawJpg(x, y, hutaoyao_33, sizeof(hutaoyao_33)); break;
      case 35: TJpgDec.drawJpg(x, y, hutaoyao_34, sizeof(hutaoyao_34)); break;
      case 36: TJpgDec.drawJpg(x, y, hutaoyao_35, sizeof(hutaoyao_35)); imgNum = 1; break;
      default: imgNum = 1; break;
    }
  }
  else if (Gif_Mode == 2) { //动画-初音
    switch (imgNum) {
      case 1: TJpgDec.drawJpg(x, y, chuyin_00, sizeof(chuyin_00)); break;
      case 2: TJpgDec.drawJpg(x, y, chuyin_01, sizeof(chuyin_01)); break;
      case 3: TJpgDec.drawJpg(x, y, chuyin_02, sizeof(chuyin_02)); break;
      case 4: TJpgDec.drawJpg(x, y, chuyin_03, sizeof(chuyin_03)); break;
      case 5: TJpgDec.drawJpg(x, y, chuyin_04, sizeof(chuyin_04)); break;
      case 6: TJpgDec.drawJpg(x, y, chuyin_05, sizeof(chuyin_05)); break;
      case 7: TJpgDec.drawJpg(x, y, chuyin_06, sizeof(chuyin_06)); break;
      case 8: TJpgDec.drawJpg(x, y, chuyin_07, sizeof(chuyin_07)); break;
      case 9: TJpgDec.drawJpg(x, y, chuyin_08, sizeof(chuyin_08)); break;
      case 10: TJpgDec.drawJpg(x, y, chuyin_09, sizeof(chuyin_09)); break;
      case 11: TJpgDec.drawJpg(x, y, chuyin_10, sizeof(chuyin_10)); break;
      case 12: TJpgDec.drawJpg(x, y, chuyin_11, sizeof(chuyin_11)); break;
      case 13: TJpgDec.drawJpg(x, y, chuyin_12, sizeof(chuyin_12)); break;
      case 14: TJpgDec.drawJpg(x, y, chuyin_13, sizeof(chuyin_13)); break;
      case 15: TJpgDec.drawJpg(x, y, chuyin_14, sizeof(chuyin_14)); break;
      case 16: TJpgDec.drawJpg(x, y, chuyin_15, sizeof(chuyin_15)); break;
      case 17: TJpgDec.drawJpg(x, y, chuyin_16, sizeof(chuyin_16)); break;
      case 18: TJpgDec.drawJpg(x, y, chuyin_17, sizeof(chuyin_17)); break;
      case 19: TJpgDec.drawJpg(x, y, chuyin_18, sizeof(chuyin_18)); break;
      case 20: TJpgDec.drawJpg(x, y, chuyin_19, sizeof(chuyin_19)); break;
      case 21: TJpgDec.drawJpg(x, y, chuyin_20, sizeof(chuyin_20)); break;
      case 22: TJpgDec.drawJpg(x, y, chuyin_21, sizeof(chuyin_21)); break;
      case 23: TJpgDec.drawJpg(x, y, chuyin_22, sizeof(chuyin_22)); break;
      case 24: TJpgDec.drawJpg(x, y, chuyin_23, sizeof(chuyin_23)); imgNum = 1; break;
      default: imgNum = 1; break;
    }
  }
  else if (Gif_Mode == 3) { //动画-打胡桃
    switch (imgNum) {
      case 1: TJpgDec.drawJpg(x, y, dahutao_00, sizeof(dahutao_00)); break;
      case 2: TJpgDec.drawJpg(x, y, dahutao_01, sizeof(dahutao_01)); break;
      case 3: TJpgDec.drawJpg(x, y, dahutao_02, sizeof(dahutao_02)); break;
      case 4: TJpgDec.drawJpg(x, y, dahutao_03, sizeof(dahutao_03)); break;
      case 5: TJpgDec.drawJpg(x, y, dahutao_04, sizeof(dahutao_04)); break;
      case 6: TJpgDec.drawJpg(x, y, dahutao_05, sizeof(dahutao_05)); imgNum = 1; break;
      default: imgNum = 1; break;
    }
  }
  else if (Gif_Mode == 4) { //动画-美少女转转
    switch (imgNum) {
      case 1: TJpgDec.drawJpg(x, y, meishaonvzhuanzhuan_00, sizeof(meishaonvzhuanzhuan_00)); break;
      case 2: TJpgDec.drawJpg(x, y, meishaonvzhuanzhuan_01, sizeof(meishaonvzhuanzhuan_01)); break;
      case 3: TJpgDec.drawJpg(x, y, meishaonvzhuanzhuan_02, sizeof(meishaonvzhuanzhuan_02)); break;
      case 4: TJpgDec.drawJpg(x, y, meishaonvzhuanzhuan_03, sizeof(meishaonvzhuanzhuan_03)); break;
      case 5: TJpgDec.drawJpg(x, y, meishaonvzhuanzhuan_04, sizeof(meishaonvzhuanzhuan_04)); break;
      case 6: TJpgDec.drawJpg(x, y, meishaonvzhuanzhuan_05, sizeof(meishaonvzhuanzhuan_05)); break;
      case 7: TJpgDec.drawJpg(x, y, meishaonvzhuanzhuan_06, sizeof(meishaonvzhuanzhuan_06)); break;
      case 8: TJpgDec.drawJpg(x, y, meishaonvzhuanzhuan_07, sizeof(meishaonvzhuanzhuan_07)); break;
      case 9: TJpgDec.drawJpg(x, y, meishaonvzhuanzhuan_08, sizeof(meishaonvzhuanzhuan_08)); break;
      case 10: TJpgDec.drawJpg(x, y, meishaonvzhuanzhuan_09, sizeof(meishaonvzhuanzhuan_09)); imgNum = 1; break;
      default: imgNum = 1; break;
    }
  }
  else if (Gif_Mode == 5) { //动画-原神免疫
    switch (imgNum) {
      case 1: TJpgDec.drawJpg(x, y, yuanshen_00, sizeof(yuanshen_00)); break;
      case 2: TJpgDec.drawJpg(x, y, yuanshen_01, sizeof(yuanshen_01)); break;
      case 3: TJpgDec.drawJpg(x, y, yuanshen_02, sizeof(yuanshen_02)); break;
      case 4: TJpgDec.drawJpg(x, y, yuanshen_03, sizeof(yuanshen_03)); break;
      case 5: TJpgDec.drawJpg(x, y, yuanshen_00, sizeof(yuanshen_02)); break;
      case 6: TJpgDec.drawJpg(x, y, yuanshen_01, sizeof(yuanshen_01)); break;
      case 7: TJpgDec.drawJpg(x, y, yuanshen_02, sizeof(yuanshen_00)); break;
      case 8: TJpgDec.drawJpg(x, y, yuanshen_03, sizeof(yuanshen_03)); break;
      case 9: TJpgDec.drawJpg(x, y, yuanshen_04, sizeof(yuanshen_04)); break;
      case 10: TJpgDec.drawJpg(x, y, yuanshen_05, sizeof(yuanshen_05)); break;
      case 11: TJpgDec.drawJpg(x, y, yuanshen_06, sizeof(yuanshen_06)); break;
      case 12: TJpgDec.drawJpg(x, y, yuanshen_07, sizeof(yuanshen_07)); break;
      case 13: TJpgDec.drawJpg(x, y, yuanshen_04, sizeof(yuanshen_06)); break;
      case 14: TJpgDec.drawJpg(x, y, yuanshen_05, sizeof(yuanshen_05)); break;
      case 15: TJpgDec.drawJpg(x, y, yuanshen_06, sizeof(yuanshen_04)); break;
      case 16: TJpgDec.drawJpg(x, y, yuanshen_07, sizeof(yuanshen_07)); break;
      case 17: TJpgDec.drawJpg(x, y, yuanshen_08, sizeof(yuanshen_08)); break;
      case 18: TJpgDec.drawJpg(x, y, yuanshen_09, sizeof(yuanshen_09)); break;
      case 19: TJpgDec.drawJpg(x, y, yuanshen_10, sizeof(yuanshen_10)); break;
      case 20: TJpgDec.drawJpg(x, y, yuanshen_11, sizeof(yuanshen_11)); break;
      case 21: TJpgDec.drawJpg(x, y, yuanshen_08, sizeof(yuanshen_10)); break;
      case 22: TJpgDec.drawJpg(x, y, yuanshen_09, sizeof(yuanshen_09)); break;
      case 23: TJpgDec.drawJpg(x, y, yuanshen_10, sizeof(yuanshen_08)); break;
      case 24: TJpgDec.drawJpg(x, y, yuanshen_11, sizeof(yuanshen_11)); break;
      case 25: TJpgDec.drawJpg(x, y, yuanshen_12, sizeof(yuanshen_12)); break;
      case 26: TJpgDec.drawJpg(x, y, yuanshen_13, sizeof(yuanshen_13)); break;
      case 27: TJpgDec.drawJpg(x, y, yuanshen_14, sizeof(yuanshen_14)); break;
      case 28: TJpgDec.drawJpg(x, y, yuanshen_15, sizeof(yuanshen_15)); break;
      case 29: TJpgDec.drawJpg(x, y, yuanshen_12, sizeof(yuanshen_14)); break;
      case 30: TJpgDec.drawJpg(x, y, yuanshen_13, sizeof(yuanshen_13)); break;
      case 31: TJpgDec.drawJpg(x, y, yuanshen_14, sizeof(yuanshen_12)); break;
      case 32: TJpgDec.drawJpg(x, y, yuanshen_15, sizeof(yuanshen_15)); break;
      case 33: TJpgDec.drawJpg(x, y, yuanshen_16, sizeof(yuanshen_16)); break;
      case 34: TJpgDec.drawJpg(x, y, yuanshen_17, sizeof(yuanshen_17)); break;
      case 35: TJpgDec.drawJpg(x, y, yuanshen_18, sizeof(yuanshen_18)); break;
      case 36: TJpgDec.drawJpg(x, y, yuanshen_19, sizeof(yuanshen_19)); break;
      case 37: TJpgDec.drawJpg(x, y, yuanshen_16, sizeof(yuanshen_18)); break;
      case 38: TJpgDec.drawJpg(x, y, yuanshen_17, sizeof(yuanshen_17)); break;
      case 39: TJpgDec.drawJpg(x, y, yuanshen_18, sizeof(yuanshen_16)); break;
      case 40: TJpgDec.drawJpg(x, y, yuanshen_19, sizeof(yuanshen_19)); break;
      case 41: TJpgDec.drawJpg(x, y, yuanshen_20, sizeof(yuanshen_20)); break;
      case 42: TJpgDec.drawJpg(x, y, yuanshen_21, sizeof(yuanshen_21)); break;
      case 43: TJpgDec.drawJpg(x, y, yuanshen_22, sizeof(yuanshen_22)); break;
      case 44: TJpgDec.drawJpg(x, y, yuanshen_23, sizeof(yuanshen_23)); break;
      case 45: TJpgDec.drawJpg(x, y, yuanshen_20, sizeof(yuanshen_20)); break;
      case 46: TJpgDec.drawJpg(x, y, yuanshen_21, sizeof(yuanshen_21)); break;
      case 47: TJpgDec.drawJpg(x, y, yuanshen_22, sizeof(yuanshen_22)); break;
      case 48: TJpgDec.drawJpg(x, y, yuanshen_23, sizeof(yuanshen_23)); imgNum = 1; break;

      default: imgNum = 1; break;
    }
  }
  /*
    else if (Gif_Mode == 6) { //动画-兔娘
    switch (imgNum) {
      case 1: TJpgDec.drawJpg(x, y, tuzhi_00, sizeof(tuzhi_00)); break;
      case 2: TJpgDec.drawJpg(x, y, tuzhi_01, sizeof(tuzhi_01)); break;
      case 3: TJpgDec.drawJpg(x, y, tuzhi_02, sizeof(tuzhi_02)); break;
      case 4: TJpgDec.drawJpg(x, y, tuzhi_03, sizeof(tuzhi_03)); break;
      case 5: TJpgDec.drawJpg(x, y, tuzhi_04, sizeof(tuzhi_04)); break;
      case 6: TJpgDec.drawJpg(x, y, tuzhi_05, sizeof(tuzhi_05)); break;
      case 7: TJpgDec.drawJpg(x, y, tuzhi_06, sizeof(tuzhi_06)); break;
      case 8: TJpgDec.drawJpg(x, y, tuzhi_07, sizeof(tuzhi_07)); break;
      case 9: TJpgDec.drawJpg(x, y, tuzhi_08, sizeof(tuzhi_08)); break;
      case 10: TJpgDec.drawJpg(x, y, tuzhi_09, sizeof(tuzhi_09)); break;
      case 11: TJpgDec.drawJpg(x, y, tuzhi_10, sizeof(tuzhi_10)); break;
      case 12: TJpgDec.drawJpg(x, y, tuzhi_11, sizeof(tuzhi_11)); break;
      case 13: TJpgDec.drawJpg(x, y, tuzhi_12, sizeof(tuzhi_12)); break;
      case 14: TJpgDec.drawJpg(x, y, tuzhi_13, sizeof(tuzhi_13)); break;
      case 15: TJpgDec.drawJpg(x, y, tuzhi_14, sizeof(tuzhi_14)); break;
      case 16: TJpgDec.drawJpg(x, y, tuzhi_15, sizeof(tuzhi_15)); break;
      case 17: TJpgDec.drawJpg(x, y, tuzhi_16, sizeof(tuzhi_16)); break;
      case 18: TJpgDec.drawJpg(x, y, tuzhi_17, sizeof(tuzhi_17)); break;
      case 19: TJpgDec.drawJpg(x, y, tuzhi_18, sizeof(tuzhi_18)); break;
      case 20: TJpgDec.drawJpg(x, y, tuzhi_19, sizeof(tuzhi_19)); break;
      case 21: TJpgDec.drawJpg(x, y, tuzhi_20, sizeof(tuzhi_20)); break;
      case 22: TJpgDec.drawJpg(x, y, tuzhi_21, sizeof(tuzhi_21)); break;
      case 23: TJpgDec.drawJpg(x, y, tuzhi_22, sizeof(tuzhi_22)); break;
      case 24: TJpgDec.drawJpg(x, y, tuzhi_23, sizeof(tuzhi_23)); break;
      case 25: TJpgDec.drawJpg(x, y, tuzhi_24, sizeof(tuzhi_24)); break;
      case 26: TJpgDec.drawJpg(x, y, tuzhi_25, sizeof(tuzhi_25)); break;
      case 27: TJpgDec.drawJpg(x, y, tuzhi_26, sizeof(tuzhi_26)); break;
      case 28: TJpgDec.drawJpg(x, y, tuzhi_27, sizeof(tuzhi_27)); break;
      case 29: TJpgDec.drawJpg(x, y, tuzhi_28, sizeof(tuzhi_28)); break;
      case 30: TJpgDec.drawJpg(x, y, tuzhi_29, sizeof(tuzhi_29)); break;
      case 31: TJpgDec.drawJpg(x, y, tuzhi_30, sizeof(tuzhi_30)); break;
      default: imgNum = 1; break;
    }
    }
  */
}
