#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <EEPROM.h>
#include <Adafruit_INA219.h>
#include <SoftwareSerial.h>
#include <SPI.h>

#define LED_PIN1 D3
#define LED_PIN2 D4
#define LED_PIN3 D5


//const char *ssid = "SK_WiFiGIGA9A48";  // 와이파이 이름
//const char *pass = "1903003177";      // 와이파이 비밀번호
const char *ssid = "Campus7_Room3_2.4";  // 와이파이 이름
const char *pass = "12345678";      // 와이파이 비밀번호
const char *mqtt_server = "192.168.0.103"; // 라즈베리파이 서버 주소
// MQTT용 WiFi 클라이언트 객체 초기화
char message[50];
String payload = "";

WiFiClient espClient;
PubSubClient client(espClient);


void mqtt_init() {
    client.setServer(mqtt_server, 1883);
    // subscriber인경우 메시지 수신시 호출할 콜백 함수 등록
   client.setCallback(callback);
}

void callback(char* topic, byte* payload, unsigned int length) {
 Serial.print("Message arrived [");
 Serial.print(topic);
 Serial.print("] ");
 for (int i=0;i<length;i++) {
  char receivedChar = (char)payload[i];
  Serial.println(receivedChar);
  
  if (receivedChar == '1')
  {
    Serial.print("open 1");
    digitalWrite(LED_PIN1, HIGH);
    delay(8000);
    digitalWrite(LED_PIN1,LOW);
    delay(100);
  }
  else if(receivedChar == '2'){
    Serial.print("open 2");
    digitalWrite(LED_PIN2, HIGH);
    delay(8000);
    digitalWrite(LED_PIN2,LOW);
    delay(100);
    }
    else if(receivedChar == '3'){
    Serial.print("open 3");
    digitalWrite(LED_PIN3, HIGH);
    delay(8000);
    digitalWrite(LED_PIN3,LOW);
    delay(100);
    }
    }
  Serial.println();
}


// MQTT 서버에 접속될 때까지 재접속 시도
void reconnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client.connect("jjongahriri")) {  // 클라이언트아이드는 보통 시리얼 넘버로
            Serial.println("connected");
            // subscriber로 등록
            client.subscribe("RPi/result"); // 연결이 됐으면 구독신청, 구독자 되는 방법
            delay(100);
            Serial.println("subscribed");
        } else { // 연결 안 되면 아무것도 안 한다
            Serial.print("failed, rc=");
            Serial.print(client.state()); // 실패 이유
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}


void setup() {
//  ESP.wdtDisa                                                                                                                                      ble();
  Serial.begin(115200);
  


  pinMode(LED_PIN1, OUTPUT);
  digitalWrite(LED_PIN1, LOW);
    pinMode(LED_PIN2, OUTPUT);
  digitalWrite(LED_PIN2, LOW);
    pinMode(LED_PIN3, OUTPUT);
  digitalWrite(LED_PIN3, LOW);
  
  WiFi.mode(WIFI_STA);
  
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
   // wifi.init(ssid, password);

    mqtt_init();
    SPI.begin();                  // SPI 초기화  (SPI : 하나의 마스터와 다수의 SLAVE(종속적인 역활)간의 통신 방식)

}


void loop() {

    if (!client.connected()) { // MQTT연결이 안 되어 있으면 재연결
        reconnect();
    }
    
    client.loop();
    delay(100);

}
