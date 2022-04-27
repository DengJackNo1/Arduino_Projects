/*
红外避障传感器具有一对红外线发射与接收管，发射管发射出一定频率的红外线，当检测方向遇到障碍物（反射面）时，红外线反射回来被接收管接收。它常用于安装在小车上，判断前方是否有障碍物。可通过电位器设置阀值。正前方有障碍时绿灯亮起，OUT引脚为低电平，反之为高电平。
由于日光是也含红外线，所以大多数便宜红外模块在户外使用就会遇到问题
————————————————
版权声明：本文为CSDN博主「xieyan0811」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/xieyan0811/article/details/56011900
红外避障传感器模块
*/
int PIN_SENSOR = 13;
 
void setup() {
  pinMode(PIN_SENSOR, INPUT);
  Serial.begin(115200);
}
 
void loop() {
  int x = digitalRead(PIN_SENSOR);
  Serial.println(x);
  delay(100);
}
