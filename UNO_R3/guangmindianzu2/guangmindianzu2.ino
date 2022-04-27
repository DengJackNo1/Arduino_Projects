/*
   AutoLed
   通过光敏电阻检测环境光，进而控制LED灯亮度
*/
int ledPin = 10;
int sensorValue = 0; //保存读到的传感器模拟值

void setup()
{  Serial.begin(115200); 
  pinMode(ledPin, INPUT);     //数字口要选择带~号的具有pwm功能的输出口
}

void loop()
{
  sensorValue = digitalRead(ledPin);     //读取A0模拟口的数值
  //ledValue = map(sensorValue, 400, 800, 0, 200); //将400到800之间的数据映射成0到200之间的数据
  Serial.println(sensorValue); 
  delay(100);
}
