ultra_RPi_rfid 실행

- RPi로부터 서보모터 MQTT 받으면 각 상황에 맞는 서보모터 동작
- RPi로부터 서보모터 MQTT 받으면 각 상황에 맞는 LED 동작
- rfid 인식 시 RPi로 MQTT pub -> is_logined 활성
- 로드셀 or 초음파 센서 활성 시 RPi로 MQTT pub -> is_published 활성

![스크린샷(83)](https://user-images.githubusercontent.com/50413112/103846563-bdf8e080-50e1-11eb-837d-3cd8effb5952.png)
![스크린샷(84)](https://user-images.githubusercontent.com/50413112/103846570-c18c6780-50e1-11eb-8a02-d51cee56face.png)
![스크린샷(85)](https://user-images.githubusercontent.com/50413112/103846576-c3eec180-50e1-11eb-90e6-1ed5e22e42d2.png)
![스크린샷(86)](https://user-images.githubusercontent.com/50413112/103847778-4f695200-50e4-11eb-907f-cfa7083ddaef.png)

- 메시지 정보

RPi -> AWS (가득 찼을 때)

topic : full

message : 1


RPi -> AWS (rfid 회원정보)

topic : user/rfid

message : rfid 번호 (띄어쓰기 X)


AWS -> RPi (종료 버튼 눌렀을 때)

topic : user/end

message : end


RPi -> esp8266 (ai 결과값)

topic : RPi/result

message : 결과값(ex. 1 or 2 or 3)


esp8266 -> RPi (rfid 찍고(is_published == 0일때만) 병 무게 감지)

topic : esp8266/loadcell

message : 1


esp8266 -> RPi (rfid 찍었을 떄(is_login == 0일때만))

topic : esp8266/rfid

message : rfid 번호 (띄어쓰기 X)


esp8266 -> RPi (가득 찼을 때)

topic : esp8266/full

message : 1


esp8266 -> RPi (회원가입 안 됐을 때)

topic : user/unknown

message : 1
