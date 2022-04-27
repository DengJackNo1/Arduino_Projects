/*
   AutoLed
   通过光敏电阻检测环境光，进而控制LED灯亮度
*/
#define PIN_A 0
#define PIN_D 2
 
void setup() 
{
  Serial.begin(9600);
}
 
void loop() 
{
  int val;
  val=analogRead(PIN_A);
  Serial.print("a:");
  Serial.print(val);
  Serial.print(", d:");
  val=digitalRead(PIN_D);
  Serial.println(val);
  delay(500);
}
