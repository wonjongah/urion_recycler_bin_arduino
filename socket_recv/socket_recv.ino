#include <ESP8266WiFi.h>
#include <WebSocketServer.h>
 
WiFiServer server(80);
WebSocketServer webSocketServer;
 
char *ssid = "YOUR_WIFI";  // 와이파이 이름
char *pass = "WIFI_PASS";      // 와이파이 비밀번호
void setup() {
 
  Serial.begin(115200);
 
  WiFi.begin(ssid, pass); 
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
  Serial.println(WiFi.localIP());
 
  server.begin();
  Serial.println("server start");
  delay(100);
}
 
void loop() {
 
  WiFiClient client = server.available();
 
  if (client.connected() && webSocketServer.handshake(client)) {
 
    String data;      
 
    while (client.connected()) {
 
      data = webSocketServer.getData();
 
      if (data.length() > 0) {
         Serial.println(data);
         webSocketServer.sendData(data);
      }
 
      delay(10); // Delay needed for receiving the data correctly
   }
 
   Serial.println("The client disconnected");
   delay(100);
  }
 
  delay(100);
}
