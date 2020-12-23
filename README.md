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
