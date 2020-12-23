#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <EEPROM.h>
#include <Adafruit_INA219.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Servo.h>



#define servoPin1 D0 // servo motor pin
#define servoPin2 D1
#define servoPin3 D2


#define LED_PIN1 D3
#define LED_PIN2 D4
#define LED_PIN3 D5


Servo servo1;
Servo servo2;
Servo servo3;

MFRC522 mfrc(SS_PIN, RST_PIN);

//const char *ssid = "SK_WiFiGIGA9A48";  // 와이파이 이름
//const char *pass = "1903003177";      // 와이파이 비밀번호
const char *ssid = "Campus7_Room3_2.4";  // 와이파이 이름
const char *pass = "12345678";      // 와이파이 비밀번호
const char *mqtt_server = "192.168.0.103"; // 라즈베리파이 서버 주소
// MQTT용 WiFi 클라이언트 객체 초기화
int is_published = 0;
int is_login = 0;
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
    servo1_ON();
    delay(3000);
    servo1_OFF();
    digitalWrite(LED_PIN1, LOW);
    delay(1000);
    //pub
//    is_published--;
  }
  else if(receivedChar == '2'){
    Serial.print("open 2");
    digitalWrite(LED_PIN2, HIGH);
    servo2_ON();
    delay(3000);
    servo2_OFF();
    digitalWrite(LED_PIN2, LOW);
    delay(1000);
        //pub

//    is_published--;
    }
    else if(receivedChar == '3'){
    Serial.print("open 3");
    digitalWrite(LED_PIN3, HIGH);
    servo3_ON();
    delay(3000);
    digitalWrite(LED_PIN3, LOW);
    servo3_OFF();
    delay(1000);
        //pub

//    is_published--;
    }
    }
  Serial.println();
}


// MQTT 서버에 접속될 때까지 재접속 시도
void reconnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client.connect("motoresp")) {  // 클라이언트아이드는 보통 시리얼 넘버로
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
  Serial.begin(115200); // PC와 통신
  servo1.detach(servoPin1);
  servo2.detach(servoPin2);
  servo3.detach(servoPin3);

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

void servo1_ON(){
  servo1.attach(servoPin1);  // attaches the servo on pin 9 to the servo object
  servo1.write(90);
}

void servo1_OFF(){
  servo1.write(0);
  servo1.detach();
}

void servo2_ON(){
  servo2.attach(servoPin2);  // attaches the servo on pin 9 to the servo object
  servo2.write(90);
}

void servo2_OFF(){
  servo2.write(0);
  servo2.detach();
}

void servo3_ON(){
  servo3.attach(servoPin3);  // attaches the servo on pin 9 to the servo object
  servo3.write(90);
}

void servo3_OFF(){
  servo3.write(0);
  servo3.detach();
}
