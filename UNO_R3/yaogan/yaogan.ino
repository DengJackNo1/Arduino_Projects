/*遥感模块
x,y 模拟信号
z 数字信号
*/

//在这里VRx(x)连的是模拟端口0，VRy(y)连的是模拟端口1，SW(z)连接到了数字端口7
int value = 0; 
void setup() { 
 pinMode(7, INPUT_PULLUP); //注意 Z 轴输入一定要上拉，不然电平不稳。
 Serial.begin(9600); 
} 
void loop() { 
 value = analogRead(A0); 
 Serial.print("X:"); 
 Serial.print(value, DEC); 
 value = analogRead(A1); 
 Serial.print(" | Y:"); 
 Serial.print(value, DEC); 
 value = digitalRead(7); 
 Serial.print(" | Z: "); 
 Serial.println(value, DEC); 
 delay(100); 
} 
