/*首先，谢谢您的支持！下面有几个小细节需要说明一下：
   1.注意60位（或较长的）的WS2812可能会造成开发板供电不足，不断重启，所以建议灯带额外供电。具体看开发板的支持情况，当然也可以用其它位数的灯带。
   2，开发板可以选择ESP32 ESP8266 ESP8266 01S ESP8266-12F模块（需焊接），注意使用的引脚定义。灯带WS2812 WS2812B，在按照SDK时ESP32与8266系列的有冲突。
   3.长灯带有分输入输出端，开发板和灯带需要在同一供电设备下，否则会对控制信号异常干扰。
   4.灯光效果目前的循环模式是，每循环一次效果检测一下blinker的新命令，所以会有blinker app按键不能及时同步的情况，同样语音助手也可能迟钝。blinker频次限制暂时做不到随时中断灯效下的循环。
   5.代码同时使用了fastled和adafruit_nexopixel库。大家可以看自己的喜好添加效果。
   6.某些模式需要配合调色盘和两个滑条使用。
   7.创建设备时选择阿里云，赋值auth，并在下面代码中更改。智能配网方法：烧录成功之后在blinker app.
   8.相关教程参考点灯科技的官方开发文档与教程，也欢迎交流qq群1147147694。哔哩哔哩 https://space.bilibili.com/45978823 ，如果转载请勿修改此行，谢谢。

   更新日志20200916呼吸灯效果，新版点灯科技库支持
   更新日志20200930修复掉线、响应迟钝问题
   9.将delay大部分替换为Blinker.delay(),单位和delay一致
*/
#define BLINKER_WIFI
#define BLINKER_MIOT_LIGHT
#define BLINKER_ESP_SMARTCONFIG
#define BLINKER_PRINT Serial       //串口协议库
#define BLINKER_WITHOUT_SSL
#define BLINKER_ARDUINOJSON
#include <FastLED.h>
#define DEBUG_ON//clock
#include <Blinker.h>
#include <Adafruit_NeoPixel.h>
////////////////////////////////////////五个开关按键 一个滑条 一个调色盘
BlinkerButton Button1("ButtonKey1");
BlinkerButton Button2("ButtonKey2");
BlinkerButton Button3("ButtonKey3");
BlinkerButton Button4("ButtonKey4");
BlinkerButton Button5("ButtonKey5");
BlinkerButton Button6("ButtonKey6"); //阅读模式 openState = 7
BlinkerSlider Slider1("SliderKey");
BlinkerRGB RGBWS2812("RGBKey");



/////////////////////////////////////修改blinker设备的授权码
char auth[] = "";
//填写自己的授权码

#define PIN            4                             /////ws2812 DAT 接的引脚编号，注意开发板不同，=====请更改=====
#define NUMPIXELS      60                           ////ws2812 灯数  =====请更改=====



//clock part

unsigned long timeZone = 8.0;                     // 东八区北京时间
const char* NTPServerName = "ntp7.aliyun.com";    // Change this to a ntpserver nearby, check this site for a list of servers: https://www.pool.ntp.org/en/
unsigned long intervalNTP = 24 * 60 * 60000;      // Request a new NTP time every 24 hours
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);//////////////////////Adafruit_NeoPixel
int openState = 2;/////////////////////////////灯效模式，默认时钟
int freq_flash = 25;/////////////////////////灯效节奏
uint8_t colorR, colorG, colorB, colorW;
bool wsState;
uint8_t wsMode = BLINKER_CMD_MIOT_DAY;
int brt_set = 100;



CRGB leds[NUMPIXELS];//////////////FASTLED
CRGB colorHour = CRGB(200,0,55);
CRGB colorMinute = CRGB(0, 200, 55);
CRGB colorSecond =CRGB(0,55,200);
CRGB colorHourMinute = CRGB::Yellow;
CRGB colorHourSecond = CRGB::Magenta;
CRGB colorMinuteSecond = CRGB::Cyan;
CRGB colorAll = CRGB::White;


#define USE_LED_MOVE_BETWEEN_HOURS true

// Cutoff times for day / night brightness.
#define USE_NIGHTCUTOFF true   // Enable/Disable night brightness
#define MORNINGCUTOFF 6         // When does daybrightness begin?   24小时 
#define NIGHTCUTOFF 22           // When does nightbrightness begin? 24小时
#define NIGHTBRIGHTNESS 25     // Brightness level from 0 (off) to 255 (full brightness) 夜间的亮度


WiFiUDP UDP;
IPAddress timeServerIP;
const int NTP_PACKET_SIZE = 48;
byte NTPBuffer[NTP_PACKET_SIZE];

unsigned long prevNTP = 0;
unsigned long lastNTPResponse = millis();
uint32_t timeUNIX = 0;
unsigned long prevActualTime = 0;

#define LEAP_YEAR(Y) ( ((1970+Y)>0) && !((1970+Y)%4) && ( ((1970+Y)%100) || !((1970+Y)%400) ) )
static const uint8_t monthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

struct DateTime {
  int  year;
  byte month;
  byte day;
  byte hour;
  byte minute;
  byte second;
  byte dayofweek;
};

DateTime currentDateTime;//////////////////////////////////////

byte hourLED;
int a ,b , c , d = hourLED;
byte getLEDHour(byte hours, byte minutes) {
  if (hours > 12)
    hours = hours - 12;
  if (hours <= 5)
    hourLED = (hours * 5) + 30;
  else
    hourLED = (hours * 5) - 30;

  if (USE_LED_MOVE_BETWEEN_HOURS == true) {
    if        (minutes >= 12 && minutes < 24) {
      hourLED += 1; a=hourLED-1;b=hourLED-1;c=hourLED-1;d=hourLED-1;
    } else if (minutes >= 24 && minutes < 36) {
      hourLED += 2; a=hourLED-1;b=hourLED-2;c=hourLED-1;d=hourLED-1;
    } else if (minutes >= 36 && minutes < 48) {
      hourLED += 3; a=hourLED-1;b=hourLED-2;c=hourLED-3;d=hourLED-1;
    } else if (minutes >= 48) {
      hourLED += 4; a=hourLED-1;b=hourLED-2;c=hourLED-3;d=hourLED-4;
     } else if (minutes < 12) {
      a=hourLED;b=hourLED;c=hourLED;d=hourLED;
    }
  }

  return hourLED;
}

byte getLEDMinuteOrSecond(byte minuteOrSecond) {
  if (minuteOrSecond < 30)
    return minuteOrSecond + 30;
  else
    return minuteOrSecond - 30;
}



void startUDP() {
  Serial.println("Starting UDP");
  UDP.begin(123);                          // Start listening for UDP messages on port 123
  Serial.print("Local port:\t");
  Serial.println(UDP.localPort());
  Serial.println();
}

uint32_t getTime() {
  if (UDP.parsePacket() == 0) { // If there's no response (yet)
    return 0;
  }
  UDP.read(NTPBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
  // Combine the 4 timestamp bytes into one 32-bit number
  uint32_t NTPTime = (NTPBuffer[40] << 24) | (NTPBuffer[41] << 16) | (NTPBuffer[42] << 8) | NTPBuffer[43];
  // Convert NTP time to a UNIX timestamp:
  // Unix time starts on Jan 1 1970. That's 2208988800 seconds in NTP time:
  const uint32_t seventyYears = 2208988800UL;
  // subtract seventy years:
  uint32_t UNIXTime = NTPTime - seventyYears;
  return UNIXTime;
}

void sendNTPpacket(IPAddress& address) {
  memset(NTPBuffer, 0, NTP_PACKET_SIZE);  // set all bytes in the buffer to 0
  // Initialize values needed to form NTP request
  NTPBuffer[0] = 0b11100011;   // LI, Version, Mode
  // send a packet requesting a timestamp:
  UDP.beginPacket(address, 123); // NTP requests are to port 123
  UDP.write(NTPBuffer, NTP_PACKET_SIZE);
  UDP.endPacket();
}

void convertTime(uint32_t time) {
  // Correct time zone
  time += (3600 * timeZone);

  currentDateTime.second = time % 60;
  currentDateTime.minute = time / 60 % 60;
  currentDateTime.hour   = time / 3600 % 24;
  time  /= 60;  // To minutes
  time  /= 60;  // To hours
  time  /= 24;  // To days
  currentDateTime.dayofweek = ((time + 4) % 7) + 1;
  int year = 0;
  int days = 0;
  while ((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= time) {
    year++;
  }
  days -= LEAP_YEAR(year) ? 366 : 365;
  time  -= days; // To days in this year, starting at 0
  days = 0;
  byte month = 0;
  byte monthLength = 0;
  for (month = 0; month < 12; month++) {
    if (month == 1) { // February
      if (LEAP_YEAR(year)) {
        monthLength = 29;
      } else {
        monthLength = 28;
      }
    } else {
      monthLength = monthDays[month];
    }

    if (time >= monthLength) {
      time -= monthLength;
    } else {
      break;
    }
  }

  currentDateTime.day = time + 1;
  currentDateTime.year = year + 1970;
  currentDateTime.month = month + 1;

  // Correct European Summer time
  if (summerTime()) {
    currentDateTime.hour += 0;
  }

#ifdef DEBUG_ON
  Serial.print(currentDateTime.year);
  Serial.print(" ");
  Serial.print(currentDateTime.month);
  Serial.print(" ");
  Serial.print(currentDateTime.day);
  Serial.print(" ");
  Serial.print(currentDateTime.hour);
  Serial.print(" ");
  Serial.print(currentDateTime.minute);
  Serial.print(" ");
  Serial.print(currentDateTime.second);
  Serial.print(" day of week: ");
  Serial.print(currentDateTime.dayofweek);
  Serial.print(" summer time: ");
  Serial.print(summerTime());
  Serial.print(" night time: ");
  Serial.print(night());
  Serial.println();
#endif
}

boolean summerTime() {

  if (currentDateTime.month < 3 || currentDateTime.month > 10) return false;  // No summer time in Jan, Feb, Nov, Dec
  if (currentDateTime.month > 3 && currentDateTime.month < 10) return true;   // Summer time in Apr, May, Jun, Jul, Aug, Sep
  if (currentDateTime.month == 3 && (currentDateTime.hour + 24 * currentDateTime.day) >= (3 +  24 * (31 - (5 * currentDateTime.year / 4 + 4) % 7)) || currentDateTime.month == 10 && (currentDateTime.hour + 24 * currentDateTime.day) < (3 +  24 * (31 - (5 * currentDateTime.year / 4 + 1) % 7)))
    return true;
  else
    return false;
}

boolean night() {

  if (currentDateTime.hour >= MORNINGCUTOFF  && currentDateTime.hour < NIGHTCUTOFF) 
   return false;
   else
  return true;
}



void clock_leds() {
  unsigned long currentMillis = millis();

  if (currentMillis - prevNTP > intervalNTP) { // If a minute has passed since last NTP request
    prevNTP = currentMillis;
    Serial.println("\r\nSending NTP request ...");
    sendNTPpacket(timeServerIP);               // Send an NTP request
  }

  uint32_t time = getTime();                   // Check if an NTP response has arrived and get the (UNIX) time
  if (time) {                                  // If a new timestamp has been received
    timeUNIX = time;
    Serial.print("NTP response:\t");
    Serial.println(timeUNIX);
    lastNTPResponse = currentMillis;
  } else if ((currentMillis - lastNTPResponse) > 3600000) {
    Serial.println("More than 1 hour since last NTP response. Rebooting.");
    Serial.flush();
    ESP.reset();
  }

  uint32_t actualTime = timeUNIX + (currentMillis - lastNTPResponse) / 1000;
  if (actualTime != prevActualTime && timeUNIX != 0) { // If a second has passed since last update
    prevActualTime = actualTime;
    convertTime(actualTime);

    for (int i = 0; i < NUMPIXELS; i++)
      leds[i] = CRGB::Black;

    int second = getLEDMinuteOrSecond(currentDateTime.second);
    int minute = getLEDMinuteOrSecond(currentDateTime.minute);
    int hour = getLEDHour(currentDateTime.hour, currentDateTime.minute);
    
    // Set "Hands"
    leds[second] = colorSecond;
    leds[minute] = colorMinute;
    leds[hour] = colorHour;
    leds[a] = colorHour;
    leds[b] = colorHour;
    leds[c] = colorHour;
    leds[d] = colorHour;

    // Hour and min are on same spot
    if ( hour == minute)
      leds[hour] = colorHourMinute;
        if ( a == minute)
      leds[a] = colorHourMinute;
        if ( b == minute)
      leds[b] = colorHourMinute;
        if ( c == minute)
      leds[c] = colorHourMinute;
        if ( d == minute)
      leds[d] = colorHourMinute;
 


    // Hour and sec are on same spot
    if ( hour == second)
      leds[hour] = colorHourSecond;
        if ( a == second)
      leds[a] = colorHourSecond;
        if ( b == second)
      leds[b] = colorHourSecond;
        if ( c == second)
      leds[c] = colorHourSecond;
        if ( d == second)
      leds[d] = colorHourSecond;

    // Min and sec are on same spot
    if ( minute == second)
     leds[minute] = colorMinuteSecond;

    // All are on same spot
    if ( minute == second && minute == hour)
      leds[minute] = colorAll;
   if ( minute == second && minute == a)
      leds[minute] = colorAll;
      if ( minute == second && minute == b)
      leds[minute] = colorAll;
      if ( minute == second && minute == c)
      leds[minute] = colorAll;
      if ( minute == second && minute == d)
      leds[minute] = colorAll;
     

  
    
    if ( night() && USE_NIGHTCUTOFF == true )
      FastLED.setBrightness (NIGHTBRIGHTNESS);
   else
      FastLED.setBrightness (255);
    FastLED.show();
  }
}





//////////////////////////////////////颜色写入



void pixelShow()
{

  pixels.setBrightness(colorW);

  for (int i = 0; i < NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, colorR, colorG, colorB);

  }
  pixels.show();

}
/////////////////////////////////////////频率读取


void tip_led()
{
  digitalWrite(2, LOW);
  digitalWrite(2, HIGH);
  Blinker.delay(50);
  digitalWrite(2, LOW);
  Blinker.vibrate();
}

void slider1_callback(int32_t value)
{
  tip_led();
  Blinker.vibrate();///震动
  BLINKER_LOG("get slider value: ", value);
  freq_flash = value;
}


////////////////////////////////////////////////调色盘

void ws2812_callback(uint8_t r_value, uint8_t g_value, uint8_t b_value, uint8_t bright_value)
{
  tip_led();
  button_clear();
  BLINKER_LOG("R value: ", r_value);
  BLINKER_LOG("G value: ", g_value);
  BLINKER_LOG("B value: ", b_value);
  BLINKER_LOG("Rrightness value: ", bright_value);
  /////颜色和亮度赋值
  colorR = r_value;
  colorG = g_value;
  colorB = b_value;
  colorW = bright_value;
  openState = 6;
  //Text1.print("灯光模式：" , "单颜色");
}



void button_clear()//按键相应
{
  Button1.print("off");
  Button2.print("off");
  Button3.print("off");
  Button4.print("off");
  Button5.print("off");
  Button6.print("off");
  Button1.color("#008000");
  Button2.color("#008000");
  Button3.color("#008000");
  Button4.color("#008000");
  Button5.color("#008000");
  Button6.print("#008000");
}

///////////////////////////////////////////////////////////////////btn1
void button1_callback(const String & state)
{
  tip_led();
  BLINKER_LOG("get button state: ", state);

  if (state == BLINKER_CMD_ON) {
    BLINKER_LOG("日光模式开启");
    tip_led();
    button_clear();
    Button1.color("#DC143C");
    Button1.print("on");
    openState = 1;
  }
  else if (state == BLINKER_CMD_OFF) {
    BLINKER_LOG("日光模式关闭!");
    tip_led();
    button_clear();
    Button1.color("#008000");
    openState = 0;
  }

}




//////////////////////////////////////////////////////////btn2

void button2_callback(const String & state)
{
  tip_led();
  BLINKER_LOG("get button state: ", state);

  if (state == BLINKER_CMD_ON) {
    BLINKER_LOG("月光模式开启");
    tip_led();
    button_clear();
    Button2.print("on");
    Button2.color("#DC143C");
    openState = 2;
  }
  else if (state == BLINKER_CMD_OFF) {
    BLINKER_LOG("月光模式关闭!");
    tip_led();
    button_clear();
    openState = 0;
  }

}


///////////////////////////////////////////////////////////////////3
void button3_callback(const String & state)
{
  tip_led();
  BLINKER_LOG("get button state: ", state);

  if (state == BLINKER_CMD_ON) {
    BLINKER_LOG("温馨模式开启");
    tip_led();
    button_clear();
    Button3.print("on");
    Button3.color("#DC143C");
    openState = 3;
    brt_set = colorW;
  }
  else if (state == BLINKER_CMD_OFF) {
    BLINKER_LOG("温馨模式关闭!");
    tip_led();
    button_clear();
    openState = 0;
  }

}

////////////////////////////////////////////////////////////////////////4

void button4_callback(const String & state)
{
  tip_led();
  BLINKER_LOG("get button state: ", state);

  if (state == BLINKER_CMD_ON) {
    BLINKER_LOG("电脑模式开启");
    tip_led();
    button_clear();
    Button4.print("on");
    Button4.color("#DC143C");
    openState = 4;
  }
  else if (state == BLINKER_CMD_OFF) {
    BLINKER_LOG("电脑模式关闭!");
    tip_led();
    button_clear();
    openState = 0;
  }

}

////////////////////////////////////////////////////////////////////////
void button5_callback(const String & state)
{
  tip_led();
  BLINKER_LOG("get button state: ", state);

  if (state == BLINKER_CMD_ON) {
    BLINKER_LOG("电视模式开启");
    tip_led();
    button_clear();
    Button5.print("on");
    Button5.color("#DC143C");
    openState = 5;
  }
  else if (state == BLINKER_CMD_OFF) {
    BLINKER_LOG("电视模式关闭!");
    tip_led();
    button_clear();
    openState = 0;

  }

}

////////////////////////////////////////////////////////////////////////
void button6_callback(const String & state)
{
  tip_led();
  BLINKER_LOG("get button state: ", state);

  if (state == BLINKER_CMD_ON) {
    BLINKER_LOG("阅读模式开启");
    tip_led();
    button_clear();
    Button6.print("on");
    Button5.color("#DC143C");
    openState = 7;
  }
  else if (state == BLINKER_CMD_OFF) {
    BLINKER_LOG("阅读模式关闭!");
    tip_led();
    button_clear();
    openState = 0;

  }

}

void dataRead(const String & data)
{
  BLINKER_LOG("Blinker readString: ", data);

  Blinker.vibrate();

  uint32_t BlinkerTime = millis();

  Blinker.print("millis", BlinkerTime);
}


uint32_t getColor()
{

  uint32_t color = colorR << 16 | colorG << 8 | colorB;

  return color;
}


void miotPowerState(const String & state)//语音助手开关灯
{
  BLINKER_LOG("need set power state: ", state);

  if (state == BLINKER_CMD_ON) {
    tip_led();
    BlinkerMIOT.powerState("on");
    BlinkerMIOT.print();

    wsState = true;

    if (colorW == 0) colorW = 255;
    openState = 6;
  }
  else if (state == BLINKER_CMD_OFF) {
    tip_led();

    BlinkerMIOT.powerState("off");
    BlinkerMIOT.print();

    wsState = false;
    openState = 0;
  }
}

void miotColor(int32_t color)
{
  BLINKER_LOG("need set color: ", color);

  colorR = color >> 16 & 0xFF;
  colorG = color >>  8 & 0xFF;
  colorB = color       & 0xFF;

  BLINKER_LOG("colorR: ", colorR, ", colorG: ", colorG, ", colorB: ", colorB);
  openState = 6;
  //pixelShow();

  BlinkerMIOT.color(color);
  BlinkerMIOT.print();
}

void miotMode(uint8_t mode)
{
  BLINKER_LOG("need set mode: ", mode);

  if (mode == BLINKER_CMD_MIOT_DAY) {
    // Your mode function
    button_clear();
    Button1.print("on");
    Button1.color("#DC143C");
    openState = 1;

  }
  else if (mode == BLINKER_CMD_MIOT_NIGHT) {
    // Your mode function
    button_clear();
    Button2.print("on");
    Button2.color("#DC143C");
    openState = 2;
  }
  else if (mode == BLINKER_CMD_MIOT_COLOR) {
    // Your mode function
    button_clear();
    openState = 6;

  }
  else if (mode == BLINKER_CMD_MIOT_WARMTH) {
    // Your mode function
    button_clear();
    Button3.print("on");
    Button3.color("#DC143C");
    openState = 3;
  }
  else if (mode == BLINKER_CMD_MIOT_TV) {
    // Your mode function
    button_clear();
    Button5.print("on");
    Button5.color("#DC143C");
    openState = 5;
  }
  else if (mode == BLINKER_CMD_MIOT_READING) {

    // Your mode function
    button_clear();
    Button6.print("on");
    Button6.color("#DC143C");
    openState = 7;
  }
  else if (mode == BLINKER_CMD_MIOT_COMPUTER) {
    // Your mode function
    button_clear();
    Button4.print("on");
    Button4.color("#DC143C");
    openState = 4;
  }

  wsMode = mode;

  BlinkerMIOT.mode(mode);
  BlinkerMIOT.print();
}

void miotBright(const String & bright)
{
  BLINKER_LOG("need set brightness: ", bright);
  colorW = map(bright.toInt(), 0, 100, 0, 255);
  BLINKER_LOG("now set brightness: ", colorW);

  pixelShow();

  BlinkerMIOT.brightness(colorW);
  BlinkerMIOT.print();
  openState = 6;
}

void miotColoTemp(int32_t colorTemp)
{
  BLINKER_LOG("need set colorTemperature: ", colorTemp);

  BlinkerMIOT.colorTemp(colorTemp);
  BlinkerMIOT.print();
}

void miotQuery(int32_t queryCode)
{
  BLINKER_LOG("MIOT Query codes: ", queryCode);

  switch (queryCode)
  {
    case BLINKER_CMD_QUERY_ALL_NUMBER :
      BLINKER_LOG("MIOT Query All");
      BlinkerMIOT.powerState(wsState ? "on" : "off");
      BlinkerMIOT.color(0);
      BlinkerMIOT.mode(0);
      BlinkerMIOT.colorTemp(1000);
      BlinkerMIOT.brightness(1);
      BlinkerMIOT.print();
      break;
    case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
      BLINKER_LOG("MIOT Query Power State");
      BlinkerMIOT.powerState(wsState ? "on" : "off");
      BlinkerMIOT.print();
      break;
    case BLINKER_CMD_QUERY_COLOR_NUMBER :
      BLINKER_LOG("MIOT Query Color");
      BlinkerMIOT.color(0);
      BlinkerMIOT.print();
      break;
    case BLINKER_CMD_QUERY_MODE_NUMBER :
      BLINKER_LOG("MIOT Query Mode");
      BlinkerMIOT.mode(0);
      BlinkerMIOT.print();
      break;
    case BLINKER_CMD_QUERY_COLORTEMP_NUMBER :
      BLINKER_LOG("MIOT Query ColorTemperature");
      BlinkerMIOT.colorTemp(1000);
      BlinkerMIOT.print();
      break;
    case BLINKER_CMD_QUERY_BRIGHTNESS_NUMBER :
      BLINKER_LOG("MIOT Query Brightness");
      BlinkerMIOT.brightness(1);
      BlinkerMIOT.print();
      break;
    default :
      BlinkerMIOT.powerState(wsState ? "on" : "off");
      BlinkerMIOT.color(0);
      BlinkerMIOT.mode(0);
      BlinkerMIOT.colorTemp(1000);
      BlinkerMIOT.brightness(1);
      BlinkerMIOT.print();
      break;
  }
}






/**************************灯光效果程序**************************
 * **************************************************
 * **************************************************/

////////////////////////////////////////////////彩虹灯
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel((i + j) & 255));
    }
    if (active())
    {
      break;
    }
    pixels.show();
    Blinker.delay(freq_flash);
  }
}



////////////////////////////////////////////////////////////////
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < pixels.numPixels(); i++) {
    pixels.setPixelColor(i, c);
    pixels.show();
    Blinker.delay(wait);
    if (active())
    {
      break;
    }
  }
}

void colorScan()//跑马灯
{
  colorWipe(pixels.Color(255, 0, 0), freq_flash); // Red
  colorWipe(pixels.Color(0, 255, 0), freq_flash); // Green
  colorWipe(pixels.Color(0, 0, 255), freq_flash);
  colorWipe(pixels.Color(0, 255, 255), freq_flash);
  colorWipe(pixels.Color(255, 0, 255), freq_flash);
  colorWipe(pixels.Color(255, 0, 0), freq_flash);
}

void colorScan2()//跑马灯2,类似贪吃蛇形式
{ 
  for(int i=0;i<NUMPIXELS;i++){       
        pixels.setPixelColor(i, pixels.Color(0,0,0));
        pixels.setPixelColor((i+1>59)?(i-59):(i+1), pixels.Color(139, 0, 255));     
        pixels.setPixelColor((i+2>59)?(i-58):(i+2), pixels.Color(0, 127, 255 ));
        pixels.setPixelColor((i+3>59)?(i-57):(i+3), pixels.Color(0, 0, 255));
        pixels.setPixelColor((i+4>59)?(i-56):(i+4), pixels.Color(0, 255, 0));
        pixels.setPixelColor((i+5>59)?(i-55):(i+5), pixels.Color(255, 255, 0));
        pixels.setPixelColor((i+6>59)?(i-54):(i+6), pixels.Color(255, 165, 0));
        pixels.setPixelColor((i+7>59)?(i-53):(i+7), pixels.Color(255, 0, 0));
        pixels.show();
        if (active())
        {
          break;
        }
        Blinker.delay(freq_flash);
  }
}

///////////////////////////////////FASTLEDS的示例效果
void fadeall() {
  for (int i = 0; i < NUMPIXELS; i++) {
    leds[i].nscale8(250);
  }
}


void cylon() {
  static uint8_t hue = 0;

  // First slide the led in one direction
  for (int i = 0; i < NUMPIXELS; i++) {
    // Set the i'th led to red
    leds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    if (active())
    {
      break;
    }
    Blinker.delay(freq_flash);
  }

  // Now go in the other direction.
  for (int i = (NUMPIXELS) - 1; i >= 0; i--) {
    // Set the i'th led to red
    leds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    if (active())
    {
      break;
    }
    Blinker.delay(freq_flash);
  }
}


void breath()//呼吸灯
{
  wsState = true;

  for (int brt = 1; brt < brt_set + 1; brt++) {
    colorW = brt;
    if (colorW % 5 == 0)
    {
      pixelShow();
    }
    if (active())
    {
      break;
    }
    Blinker.delay(freq_flash * 2);
  }
  for (int brt = brt_set; brt > 0 ; brt--) {
    colorW = brt;
    if (colorW % 5 == 0)
    {
      pixelShow();
    }
    if (active())
    {
      break;
    }
    Blinker.delay(freq_flash);

  }

}

//LED圆形时钟显示初始化
void ledShowInit()
{
  //LED全部点亮为白色 10%亮度
  for (int i=0; i<NUMPIXELS; i++)
  {
      leds[i] = CRGB::White;
  }
  FastLED.setBrightness (100); 
  FastLED.show();
}

void ledShowAllRed(void)
{
  for (int i=0; i<NUMPIXELS; i++)
  {
      leds[i] = CRGB::Red;
  }
  FastLED.setBrightness (100); 
  FastLED.show();
}

void smartConfig()
{
  ledShowAllRed();
  WiFi.mode(WIFI_STA);
  Serial.println("\r\nWait for Smartconfig...");
  WiFi.beginSmartConfig();//等待手机端发出的用户名与密码
  while (1)
  {
    Serial.print(".");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
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
  ledShowInit();
  WiFi.begin();
  //如果觉得时间太长可改,20秒时间
  for (int i = 0; i < 40; i++)
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
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


void setup()
{
  Serial.begin(115200);
  BLINKER_DEBUG.stream(Serial);
  FastLED.addLeds<WS2812B, PIN, GRB>(leds, NUMPIXELS);
  //Blinker.begin(auth, ssid, pswd);
  if(!AutoConfig())
  {
  smartConfig();
  }
  Blinker.begin(auth);
  startUDP();
  pixels.begin();
  Blinker.attachData(dataRead);
  BlinkerMIOT.attachPowerState(miotPowerState);
  BlinkerMIOT.attachColor(miotColor);
  BlinkerMIOT.attachMode(miotMode);
  BlinkerMIOT.attachBrightness(miotBright);
  BlinkerMIOT.attachColorTemperature(miotColoTemp);
  BlinkerMIOT.attachQuery(miotQuery);
  pinMode(14, OUTPUT);
  digitalWrite(14, HIGH);
  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH);
  Slider1.attach(slider1_callback);
  RGBWS2812.attach(ws2812_callback);
  Button1.attach(button1_callback);
  Button2.attach(button2_callback);
  Button3.attach(button3_callback);
  Button4.attach(button4_callback);
  Button5.attach(button5_callback);
  Button6.attach(button6_callback);
  pixels.setBrightness(0);
  button_clear();
  while (WiFi.status() != WL_CONNECTED){
    delay(50);
    }
  if (!WiFi.hostByName(NTPServerName, timeServerIP))  {
    Serial.println("DNS lookup failed. Rebooting.");
    Serial.flush(); 
  }
  Serial.print("Time server IP:\t");
  Serial.println(timeServerIP);

  Serial.println("\r\nSending NTP request ...");
  sendNTPpacket(timeServerIP);
}


void mode_1()
{
  colorW = 50;
  cylon();
}

void mode_2()
{

  clock_leds();
}

void mode_3()
{

  breath();
}

void mode_4()
{
  rainbow(freq_flash);
  //colorW = 100;
}


void mode_5()
{
  //colorW = 50;
  colorScan();
}

void mode_7()
{
  //colorW = 50;
  colorScan2();
}

unsigned int nowState = 0;

bool active()
{
  Blinker.run();
  if (openState != nowState)
  {
    nowState = openState;
    Serial.println("Operating in new mode " + String(openState));
    return true;
  }
  if (openState == nowState)
  {
    //Serial.println("Operating in same mode " + String(openState));
    return false;

  }
}












void loop()
{
  Blinker.run();
  switch (openState)
  {
    case 0:
      wsState = false;
      openState = 0;
      colorR = 255;
      colorG = 255;
      colorB = 255;
      colorW = 0;
      pixelShow();
      break;
    case 1:
      mode_1();
      break;
    case 2:
      mode_2();
      break;
    case 3:
      mode_3();
      break;
    case 4:
      mode_4();
      break;
    case 5:
      mode_5();
      break;
    case 6:
      pixelShow();
      break;
    case 7:
      mode_7();
      break;
    default:
      break;
  }
}
