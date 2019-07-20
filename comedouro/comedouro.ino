/* 
 There is a buzzer that plays everytime the 
 food is about to be deposited. Food can be deposited 
 in 2 distinct ways. 
 1. The user requests by pressing a button 
 2. Every 8 hours automatically, so the cat won't starve if you are not home :)
 The depositing happens thanks to a servo that makes the holes in both 
 the depositing box and the storage box align for a moment. 
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards
const int buttonPin = 13;
const int buzzerPin = 3;

int buttonState = 1;  
int prevButtonState = 1;
 
int pos = 0;    // variable to store the servo position

void setup() {
  pinMode(buttonPin , INPUT);
  pinMode(buzzerPin , OUTPUT);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  Serial.println(buttonState);
  if ( buttonState == LOW && prevButtonState == HIGH ){
    digitalWrite(buzzerPin , LOW);
    delay(1000);
    pos = 0 ;
    myservo.write(pos); 
    delay(1000);
    pos = 90;
    myservo.write(pos);
  
  }
  else {
    digitalWrite(buzzerPin , HIGH ) ; 
  }
  prevButtonState = buttonState;
  
  
}

