#include <Servo.h>
#define servoPin D0
Servo servo;

void setup() {
servo.attach(servoPin);
}

void loop() {
servo.write(-90);
delay(2000);
servo.write(90);
delay(2000);
}
