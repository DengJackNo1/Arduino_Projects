/*
工作电压：5V

工作电流：≤25mA

光斑模式：可见点状

产品尺寸：16*20mm

光源波长：650mm

输出型号类别：数字传感器
激光传感器
*/
void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);

  delay(1000);

}
