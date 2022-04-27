/*
   串口打印
*/
void setup() {
  Serial.begin(115200); //初始化串口并设置波特率为9600
}

void loop() {
  Serial.println("Hello World!"); //打印Hello World!并回车换行 
  delay(1000);                      
}
