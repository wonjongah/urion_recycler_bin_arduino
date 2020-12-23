#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <EEPROM.h>
#include <Adafruit_INA219.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define TRIG D1
#define ECHO D8

#define servoPin1 D0 // servo motor pin
#define servoPin2 D1
#define servoPin3 D2

#define RST_PIN   D8  // rfid pin   
#define SS_PIN    D4 

//#define LED_PIN1 10
//#define LED_PIN2 9
//#define LED_PIN3 1

const float CAL_FACTOR = 0.0344; //Calibration factor, this value is based on sound velocity

Servo servo1;
Servo servo2;
Servo servo3;

MFRC522 mfrc(SS_PIN, RST_PIN);

const char *ssid = "YOUR_WIFI";  // 와이파이 이름
const char *pass = "WIFI_PASS";      // 와이파이 비밀번호
const char *mqtt_server = "BROKER_IP"; // 라즈베리파이 서버 주소
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
//    digitalWrite(LED_PIN1, HIGH);
    servo1_ON();
    delay(6000);
    servo1_OFF();
//    digitalWrite(LED_PIN1,LOW);
    delay(100);
    is_published--;
  }
  else if(receivedChar == '2'){
    Serial.print("open 2");
//    digitalWrite(LED_PIN2, HIGH);
    servo2_ON();
    delay(6000);
    servo2_OFF();
//    digitalWrite(LED_PIN2,LOW);
    delay(100);
    is_published--;
    }
    else if(receivedChar == '3'){
    Serial.print("open 3");
//    digitalWrite(LED_PIN3, HIGH);
    servo3_ON();
    delay(6000);
    servo3_OFF();
//    digitalWrite(LED_PIN3,LOW);
    delay(100);
    is_published--;
    }
    else if(receivedChar == '4'){
      is_login = 0;
    }else if(receivedChar == '5'){
      is_login++;
    }
    }
  Serial.println();
}


// MQTT 서버에 접속될 때까지 재접속 시도
void reconnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client.connect("jjongah")) {  // 클라이언트아이드는 보통 시리얼 넘버로
            Serial.println("connected");
            // subscriber로 등록
            client.subscribe("RPi/+"); // 연결이 됐으면 구독신청, 구독자 되는 방법
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
//  ESP.wdtDis                                     
  Serial.begin(115200);
  
  
  servo1.detach();
  servo2.detach();
  servo3.detach();

//  pinMode(LED_PIN1, OUTPUT);
//  digitalWrite(LED_PIN1, LOW);
//    pinMode(LED_PIN2, OUTPUT);
//  digitalWrite(LED_PIN2, LOW);
//    pinMode(LED_PIN3, OUTPUT);
//  digitalWrite(LED_PIN3, LOW);
  
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
    mfrc.PCD_Init(); 

     pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
}


void loop() {

    if (!client.connected()) { // MQTT연결이 안 되어 있으면 재연결
        reconnect();
    }
    
    if ( !mfrc.PICC_IsNewCardPresent() || !mfrc.PICC_ReadCardSerial() ) {   // 태그 접촉이 되지 않았을때 또는 ID가 읽혀지지 않았을때
    delay(100);         

    float vol = sensor_ping();

  } 
  else if(is_login == 0){
    String urRFID = String(mfrc.uid.uidByte[0])+String(mfrc.uid.uidByte[1])+String(mfrc.uid.uidByte[2])+String(mfrc.uid.uidByte[3]);
    Serial.print("your RFID : ");                  // 태그의 ID출력
    Serial.println(urRFID);

    // char message[50];
   //  String 
  
    payload = urRFID;
    payload.toCharArray(message, (payload.length() + 1));
    client.publish("esp8266/rfid", message);
    is_login++;
  }

    client.loop();
    delay(100);

}

float sensor_ping() {
  float duration, distance;
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  duration = pulseIn(ECHO, HIGH);
  distance = (duration / 2) * CAL_FACTOR;
    if(distance < 7.0 && is_published == 0 && is_login == 1){
      is_published++;
    payload = "1";
    payload.toCharArray(message, (payload.length() + 1));
    delay(3000);
    client.publish("esp8266/loadcell", message);
    Serial.println("published on");
    }
  return distance;
}


void servo1_OFF(){
  servo1.attach(servoPin1);  // attaches the servo on pin 9 to the servo object
    for(int angle = 0; angle < 100; angle++) 
  { 
    servo1.write(angle); 
    delay(10); 
  } 
   servo1.detach();
}

void servo1_ON(){
  servo1.attach(servoPin1);
    for(int angle = 100; angle > 0; angle--) 
  { 
    servo1.write(angle); 
    delay(10); 
  } 
  servo1.detach();
}

void servo2_OFF(){
  
  servo2.attach(servoPin2);  // attaches the servo on pin 9 to the servo object
      for(int angle = 0; angle < 100; angle++) 
  { 
    servo2.write(angle); 
    delay(10); 
  } 
  servo2.detach();
}

void servo2_ON(){
    servo2.attach(servoPin2);

      for(int angle = 100; angle > 0; angle--) 
  { 
    servo2.write(angle); 
    delay(10); 
  } 
  servo2.detach();
}

void servo3_OFF(){
  servo3.attach(servoPin3);  // attaches the servo on pin 9 to the servo object
        for(int angle = 0; angle < 100; angle++) 
  { 
    servo3.write(angle); 
    delay(10); 
  } 
    servo3.detach();

}

void servo3_ON(){
  servo3.attach(servoPin3);
        for(int angle = 100; angle > 0; angle--) 
  { 
    servo3.write(angle); 
    delay(10); 
  } 
  servo3.detach();
}
