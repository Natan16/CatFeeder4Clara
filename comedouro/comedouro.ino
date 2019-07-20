/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
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
    pos = 180;
    myservo.write(pos);
  
  }
  else {
    digitalWrite(buzzerPin , HIGH ) ; 
  }
  prevButtonState = buttonState;
  
  
}

