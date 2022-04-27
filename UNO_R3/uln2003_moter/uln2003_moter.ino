const int P=A0;//电位器输入引脚
const int U=9;
int sensorValue=0;//电位器电压值
int outputValue=0;

void setup() {
Serial.begin(9600);
}

void loop() {
//读取模拟量值
sensorValue=analogRead(P);
outputValue=map(sensorValue,0,1023,0,255);
analogWrite(U,outputValue);
Serial.print("sensor=");
Serial.print(sensorValue);
Serial.print("\t output=");
Serial.println(outputValue);

delay(2);
}
