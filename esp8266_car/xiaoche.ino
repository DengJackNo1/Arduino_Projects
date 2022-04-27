#define BLINKER_PRINT Serial
#define BLINKER_WIFI
#include<Servo.h>
#include <Blinker.h>
#define IN1 5 //D1
#define IN2 4  //D2
#define IN4 0  //D3
#define IN3 2   //D4
Servo ServoTop;  //定义Servo对象来控制
Servo ServoBottom;  //定义Servo对象来控制
Servo ServoLeft;  //定义Servo对象来控制
Servo ServoRight;  //定义Servo对象来控制

int pos = 0;  //角度存储变量
char auth[] = "a5c3d4d0cc08";//秘钥
char ssid[] = "HONOR V20";//这你替换为您的无线网名称
char pswd[] = "lhw040306";//这你替换为您的无线网密码

// 新建组件对象
BlinkerButton Button1("run");
BlinkerButton Button2("back");
BlinkerButton Button3("stop");
BlinkerButton Button4("left");
BlinkerButton Button5("right");
BlinkerButton Button7("180");

#define Slider_1 "ran_T"
#define Slider_2 "ran_B"
#define Slider_3 "ran_L"
#define Slider_4 "ran_R"
#define Slider_5 "rest"
BlinkerSlider Slider1(Slider_1);
BlinkerSlider Slider2(Slider_2);
BlinkerSlider Slider3(Slider_3);
BlinkerSlider Slider4(Slider_4);
BlinkerSlider Slider5(Slider_5);
// 前转按钮
void button1_run(const String & state) {
   BLINKER_LOG("get button state: ", state);
   left_qianzhuan();
   right_qianzhuan();
}

//后转按钮
void button2_back(const String & state) {
   BLINKER_LOG("get button state: ", state);
   right_houzhuan();
   left_houzhuan();
   delay(100);
}
// 停止按钮
void button3_stop(const String & state) {
   BLINKER_LOG("get button state: ", state);
   left_stop();
   right_stop();
}

// 左转按钮
void button4_left(const String & state) {
   BLINKER_LOG("get button state: ", state);
    right_qianzhuan();
    left_houzhuan();
    delay(200);
    left_stop();
    right_stop();
}
// 右转按钮
void button5_right(const String & state) {
   BLINKER_LOG("get button state: ", state);
   right_houzhuan();
   left_qianzhuan();
   delay(200);
   left_stop();
   right_stop();
}

// 右转180按钮
void button7_180(const String & state) {
   BLINKER_LOG("get button state: ", state);
   right_houzhuan();
   left_qianzhuan();
   delay(1000);
   left_stop();
   right_stop();
}
void slider1_callback(int32_t value)
{
    BLINKER_LOG("get slider value: ", value);
        ServoTop.write(value);
}
void slider2_callback(int32_t value)
{
    BLINKER_LOG("get slider value: ", value);
        ServoBottom.write(180-value);
}
void slider3_callback(int32_t value)
{
    BLINKER_LOG("get slider value: ", value);
        ServoLeft.write(value);
}
void slider4_callback(int32_t value)
{
    BLINKER_LOG("get slider value: ", value);
        ServoRight.write(value);
}
void slider5_callback(int32_t value)
{
    BLINKER_LOG("get slider value: ", value);
        ToRestoreDefaultState();
}

void dataRead(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);

    Blinker.vibrate();
    
   uint32_t BlinkerTime = millis();
}

void ToRestoreDefaultState(){
    ServoTop.write(0);
    delay(1000);
    ServoBottom.write(90);
    delay(1000);
    ServoLeft.write(163);
    delay(1000);
    ServoRight.write(40);
    delay(1000);
  }

void setup() {
    // 初始化串口
    Serial.begin(115200);

    #if defined(BLINKER_PRINT)
        BLINKER_DEBUG.stream(BLINKER_PRINT);
    #endif
      pinMode(IN1,OUTPUT);
      pinMode(IN2,OUTPUT);
      pinMode(IN3,OUTPUT);
      pinMode(IN4,OUTPUT);  
    BLINKER_DEBUG.stream(Serial);
    ServoTop.attach(16);  //伺服电机控制接口
    ServoBottom.attach(12);  //伺服电机控制接口
    ServoLeft.attach(13);  //伺服电机控制接口
    ServoRight.attach(14);  //伺服电机控制接口
    // 初始化有LED的IO
    // 初始化blinker
    Blinker.begin(auth, ssid, pswd);
    Button1.attach(button1_run);
    Button2.attach(button2_back);
    Button3.attach(button3_stop);
    Button4.attach(button4_left);
    Button5.attach(button5_right);
    Button7.attach(button7_180);
    Blinker.attachData(dataRead);
    Slider1.attach(slider1_callback);
    Slider2.attach(slider2_callback);
    Slider3.attach(slider3_callback);
    Slider4.attach(slider4_callback);
    Slider5.attach(slider5_callback);
    ToRestoreDefaultState();
}

void loop() {
    Blinker.run();
}

//停止
void left_stop()
{
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
}

void right_stop()
{
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
}

void left_qianzhuan(){        //左车轱辘向前转
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
}

void right_qianzhuan(){        //右车轱辘向前转
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
}

void left_houzhuan(){          //左车轱辘向后转
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
}

void right_houzhuan(){         //右车轱辘向后转
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
}
