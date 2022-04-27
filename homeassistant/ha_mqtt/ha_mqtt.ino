#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include <dht11.h>

dht11 DHT11;

// wifi+密码   2.4G频段的
#define wifi_ssid "xxxx"
#define wifi_password "xxxx"

#define mqtt_server "192.168.0.125"   // 设置的mqtt服务端地址（我这与HA的IP一样）
// mqtt 用户名和密码  服务端设置那个
#define mqtt_user "xxx"
#define mqtt_password "xxx"

#define state_topic "office/sensor11"
#include <dht11.h>
dht11 DHT;
#define DHT11_PIN 4  //制定DHT11数据引脚
WiFiClient espClient;
PubSubClient mqttclient(espClient);

void setup() {
  Serial.begin(115200);
  setup_wifi();
  mqttclient.setServer(mqtt_server, 1883);
  mqttclient.subscribe("inTopic");
  mqttclient.setCallback(callback);
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}


void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
    while (!mqttclient.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client002-";
    clientId += String(random(0xffff), HEX);
    if (mqttclient.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      Serial.println("connected");
      mqttclient.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttclient.state());
      Serial.println(" try again in 5 seconds");
      
      delay(5000);
    }
  }
}

// 检查数据是否有变化  
bool checkBound(float newValue, float prevValue, float maxDiff) {
  return !isnan(newValue) &&
         (newValue < prevValue - maxDiff || newValue > prevValue + maxDiff);
}

long lastMsg = 0;
float temp = 0.0;
float hum = 0.0;
float diff = 1.0;

void loop() {
  if (!mqttclient.connected()) {
    reconnect();
  }
  mqttclient.loop();

  int chk = DHT11.read(DHT11_PIN);

  Serial.print("Read sensor: ");
  String msg;
  switch (chk)
  {
    case DHTLIB_OK: 
                Serial.println("OK"); 
                msg = "ok";
                break;
    case DHTLIB_ERROR_CHECKSUM: 
                Serial.println("Checksum error");
                msg = "Checksum error"; 
                break;
    case DHTLIB_ERROR_TIMEOUT: 
                Serial.println("Time out error"); 
                msg = "Time out error"; 
                break;
    default: 
                Serial.println("Unknown error"); 
                msg = "Unknown error"; 
                break;
  }
  
    
  const size_t CAPACITY = JSON_OBJECT_SIZE(3);
  StaticJsonDocument<CAPACITY> doc; 
  // create an object
  JsonObject object = doc.to<JsonObject>();
  object["humidity"] = (float)DHT11.humidity;
  object["temperature"] = (float)DHT11.temperature;
  object["status"] = msg.c_str();
  
  String output;
  serializeJson(doc, output);
  Serial.println(output);
  mqttclient.publish(state_topic, output.c_str(), true);  
  delay(5000);

  
}
