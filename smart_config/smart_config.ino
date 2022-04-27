#include <ESP8266WiFi.h>

void smartConfig()
{
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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  if(!AutoConfig())
  {
  smartConfig();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.SSID());
  Serial.println(WiFi.psk());
}
