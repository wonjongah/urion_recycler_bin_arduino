#include <SPI.h>
#include <MFRC522.h>
 
#define RST_PIN   D8              // reset핀은 D8으로 설정 , 하드웨어적으로 연결X (연결할 시 오류 뜸)
#define SS_PIN    D4              // SS핀은 D4으로 설정
                        // SS핀은 데이터를 주고받는 역할의 핀( SS = Slave Selector )
                        
String myRFID[] = { "154 66 186 121",     // 내 카드를 문자열 배열로 저장
                  "48 207 16 168" };
 
MFRC522 mfrc(SS_PIN, RST_PIN);          // MFR522를 이용하기 위해 mfrc객체를 생성해 줍니다.
 
void setup(){
  Serial.begin(115200);                         // 시리얼 통신, 속도는 115200
  SPI.begin();                  // SPI 초기화  (SPI : 하나의 마스터와 다수의 SLAVE(종속적인 역활)간의 통신 방식)
  mfrc.PCD_Init();                // PCD 초기화
}
 
void loop(){
  if ( !mfrc.PICC_IsNewCardPresent() || !mfrc.PICC_ReadCardSerial() ) {   // 태그 접촉이 되지 않았을때 또는 ID가 읽혀지지 않았을때
    delay(500);                                // 0.5초 딜레이 
    return;                                    // return
  } 
  
  String urRFID = String(mfrc.uid.uidByte[0])+" "+String(mfrc.uid.uidByte[1])+" "+String(mfrc.uid.uidByte[2])+" "+String(mfrc.uid.uidByte[3]);
  Serial.print("your RFID : ");                  // 태그의 ID출력
  Serial.println(urRFID);
  
  for (int i=0;i<sizeof(myRFID)/sizeof(myRFID[0]);i++){
    if (urRFID == myRFID[i]){
      Serial.println("Welcome");                  // myRFID에 있는 카드와 맞을 경우, Welcome
    }
  }
}
