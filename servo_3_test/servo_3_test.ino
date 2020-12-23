#include <Servo.h>

#define servoPin1 D0 // servo motor pin
#define servoPin2 D1
#define servoPin3 D2

//#define LED_PIN1 D3
//#define LED_PIN2 D4
//#define LED_PIN3 D5
Servo servo1;
Servo servo2;
Servo servo3;

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

void setup(){
    Serial.begin(115200); // PC와 통신
      servo1.detach();
  servo2.detach();
  servo3.detach();

//    pinMode(LED_PIN1, OUTPUT);
//  digitalWrite(LED_PIN1, LOW);
//    pinMode(LED_PIN2, OUTPUT);
//  digitalWrite(LED_PIN2, LOW);
//    pinMode(LED_PIN3, OUTPUT);
//  digitalWrite(LED_PIN3, LOW);

}

void loop(){
  delay(1000);
//  digitalWrite(LED_PIN1, HIGH);
  servo1_OFF();
    delay(1000);

  servo1_ON();
//      digitalWrite(LED_PIN1,LOW);
  delay(1000);

//    digitalWrite(LED_PIN2, HIGH);

  servo2_OFF();
  delay(1000);

  servo2_ON();
//        digitalWrite(LED_PIN2,LOW);
  delay(1000);

//    digitalWrite(LED_PIN3, HIGH);
  delay(1000);


  servo3_OFF();
    delay(1000);

  servo3_ON();

//        digitalWrite(LED_PIN3,LOW);
  delay(1000);

}
