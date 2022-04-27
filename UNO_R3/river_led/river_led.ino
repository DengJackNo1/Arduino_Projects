//流水灯
int led = 7;  //给13号引脚连接的设备起一个别名"led"

void setup() {
  pinMode(led, OUTPUT); //将"led"引脚设置为输出状态
}

void loop() {
  digitalWrite(led,  HIGH); //引脚输出高电平1,点亮LED
  delay(1000);  //等待1s
  digitalWrite(led, LOW); //引脚输出低电平0,熄灭LED
  delay(1000);  //等待1s
}
