#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <time.h>
#include <ArduinoJson.h>



const char *ssid = "SK_WiFiGIGA9A48";  // 와이파이 이름
//const char *ssid = "Campus7_Room3_2.4";  // 와이파이 이름
//const char *pass = "";      // 와이파이 비밀번호
const char *pass = "1903003177";      // 와이파이 비밀번호

const char *thingId = "arduino_test";          // 사물 이름 (thing ID) 
const char *host = "ab4gkq3c78vlu-ats.iot.us-east-1.amazonaws.com"; // AWS IoT Core 주소

// 사물 인증서 (파일 이름: xxxxxxxxxx-certificate.pem.crt)
const char cert_str[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDWjCCAkKgAwIBAgIVANtB5SA5FMJolkh+uIOVFNSNeq1zMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMDEyMDcwNTM0
NTZaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCk1ecxK4yrB2rUfD+K
OUEmqe7t0khs3wd55Y21tVuc1cP8OCEVz5RZEN/vXUGyOHo+/mDhX2HBsY3oxUOW
SgrOWL5qWg3OQNkTPStXdXiOZH66PGs/2JKK065ZkMfXR5aazvgeno9yJufZi+3a
ZcG5X7vCQ8huN0llGB0nQMFcADwUcOr7/XJsR6JxXoeE4i6MxwJf7yYRXjEZ8uYZ
wgelk+QMoRdCXSvBANbJWTemwgQi8hgs4kMT4F5qhi9l6mBZefg39y6s4lZThyy9
CLf3Rq71ZsSlQDNaOx3k7IQlktv70CiGeyHg2PCLcstpuf7dC5nuvF4FRtwKqKpQ
HOZjAgMBAAGjYDBeMB8GA1UdIwQYMBaAFP1XuzmqY/6pOMNr6nWPRqAEMW2RMB0G
A1UdDgQWBBQOUzPX8KiTXNnR2yQoUsJqPF8MWjAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAYyrdYctxcLgVIXRNK0iF7pak
aXjMMQv44jy2c5W1pjHbqKVFrW0wNyn2N4B0yLt8K1gTZ0o0yaeU53Iwk09xFH3V
u3qAfybaoRkReLwIGysLXzOiK7LGw5Xate65v0vcngjYPLQRP8OaCOKrcy4wQSmf
mqxTW5Q3lL4N79Aq/H2JcH6JSU8jL0mVhyJMi4nsBbBQ/vubIlEXRTByBzxe0EqX
7ARAoVgxdgaJ4fz7cDGBXW3mo3Twabelf2VuTjXj3oDVh53ejwHRkW6vYphht50M
CaSBGga2ujNx/YIwAw1co/Xr2641d7OQjUu2hjlinJ+3TVfh9LuIrUkD8/C8iA==
-----END CERTIFICATE-----
)EOF";
// 사물 인증서 프라이빗 키 (파일 이름: xxxxxxxxxx-private.pem.key)
const char key_str[] PROGMEM = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
MIIEowIBAAKCAQEApNXnMSuMqwdq1Hw/ijlBJqnu7dJIbN8HeeWNtbVbnNXD/Dgh
Fc+UWRDf711Bsjh6Pv5g4V9hwbGN6MVDlkoKzli+aloNzkDZEz0rV3V4jmR+ujxr
P9iSitOuWZDH10eWms74Hp6Pcibn2Yvt2mXBuV+7wkPIbjdJZRgdJ0DBXAA8FHDq
+/1ybEeicV6HhOIujMcCX+8mEV4xGfLmGcIHpZPkDKEXQl0rwQDWyVk3psIEIvIY
LOJDE+BeaoYvZepgWXn4N/curOJWU4csvQi390au9WbEpUAzWjsd5OyEJZLb+9Ao
hnsh4Njwi3LLabn+3QuZ7rxeBUbcCqiqUBzmYwIDAQABAoIBADiDUalM+gRlHZ5O
Hua/7HNaDZ2h28zfxktnhWdID2m87F2bzNQSgBTtMSl6upxkupzAT+zU68ohmrq0
bxCmv/+w80+5IkSVfRcYgLSShMuk7sHhFxAjq1JHct4oQiuv0pWu4eh1ik6jwheW
3uxiuMFabiATZzNKZzJuywf7Lo/gw88cr5Wq3E+VGfT6N4gycbKQU69PMOR5pid2
zPlXY/ROBs9ubQlr3MCeS6imKvOo4DntZAbsWgtT774UNLhC9dQGMYiVRRvcS5Mn
zkTL5CVJy+43Nd2VIq7Qug26H8+630SZHRKQzJAn6HMXTbPe5Plpwg2Kcdm9bzcQ
2n8FYxECgYEA0zFhcLGvWswtKRzoPSWLTbx+VhNoN0S9wmIQpZqJuDDV8jKczqnd
5b+jv6BaTvOJzefATbq2bm3fiGLWVixxlMAnw06xDel08ty94wDgz5tCYHjkrzVm
jYcPVul3poFyUrbEcGoQdSi8fquDn6eAopP9/lm8Zrq7x4DOBp6uVMsCgYEAx86x
1pLJBzPNwWphkkWc+NekzyrH8RIHkglqgGPTbd5h6xcPbrVnT52ue3gLr4SoCkQb
yl/BSmdW4Pez08AT+TIeZ9TKa8+LnEcz6ivJjxbqgl9CbW5bjZpY2fJ84XK9b4+I
L21YlJOxM9lCr/1H8ibRnaQObmV/O0VGpKbNmckCgYAZbcGwwwQ9J4Zsg9mGu0WJ
p5+Cl7Wdqf47MHmWpwPcFK0+C1AMJjHcnvWFdjZJeVNnYncqJMgghcQ7dMMf/xvx
8C0ZfBkfdrOpJmEKuylWdfhJC2Vln0Vo1xFGwrewabetM+p1KqAuqyf/DIG9sHpx
mOYlQQLJT8SaYezsqSCOfwKBgES81O1unf3+UmChymNZ04DAHxCuNQ64FyakWqFE
tm3rvV/1FPgPLDV1l5seV1bQmka925mpi4OfPYIzdQf54mUuWst5pxGj2vGuhSug
WL0DMzYas22WM6TpXnvMMi4VHRqOFFnMkla64JFnwstspR2nQSGsWNKd1EF9uanX
QJARAoGBAMElTYdD7NJkPzuYxQdH/BROkgZXMzlfkrSekhfSqxId1ufshHmNp8Ob
E9EKVpTYU7ls8d8VJOzJW3FeClyywHdK4uTdIBR57a4eLM11Wioh2f3KuOd4HyTP
YdBPnSiumwr2DusgM6Muu3wQSfu4WH59RIFV7xa/kZkbqZn0tHd0
-----END RSA PRIVATE KEY-----
)EOF";
// Amazon Trust Services(ATS) 엔드포인트 CA 인증서 (서버인증 > "RSA 2048비트 키: Amazon Root CA 1" 다운로드)
const char ca_str[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

}


X509List ca(ca_str);
X509List cert(cert_str);
PrivateKey key(key_str);
WiFiClientSecure wifiClient;
PubSubClient client(host, 8883, callback, wifiClient); //set  MQTT port number to 8883 as per //standard

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(thingId)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("bang/jinsil", "hello jinsil");
      // ... and resubscribe
      client.subscribe("bang/jinsil");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      char buf[256];
      wifiClient.getLastSSLError(buf,256);
      Serial.print("WiFiClientSecure SSL error: ");
      Serial.println(buf);

      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// Set time via NTP, as required for x.509 validation
void setClock() {
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();
  Serial.println();

  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
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

  wifiClient.setTrustAnchors(&ca);
  wifiClient.setClientRSACert(&cert, &key);
  Serial.println("Certifications and key are set");

  setClock();
  //client.setServer(host, 8883);
  client.setCallback(callback);
}

long lastMsg = 0;
char msg[50];
int value = 0;

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
    StaticJsonDocument<200> doc;
    JsonObject root = doc.to<JsonObject>();
    root["message"] = "yeah!!!!!!!!!!";
    serializeJson(root, msg);
    //snprintf (msg, 75, "{\'message':'jinsil_zzang'\}", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("bang/jinsil", msg);
    delay(10000);

}
