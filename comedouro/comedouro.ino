/* 
 There is a buzzer that plays everytime the 
 food is about to be deposited. Food can be deposited 
 in 2 distinct ways. 
 1. The user requests by pressing a button 
 2. Every 8 hours automatically, so the cat won't starve if you are not home :)
 The depositing happens thanks to a servo that makes the holes in both 
 the depositing box and the storage box align for a moment. 
*/


//the next step is to control the amount of food deposited based on the cat's needs
//later introduce sensors that remind the user its time to buy more cat food and 
//sends text message automatically to the petshop. Also, a report can be generated 
//so the vet can have more information.
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards
const int buttonPin = 13;
const int buzzerPin = 3;
long CycleTime = 28800000; //Time (in miliseconds) of the feeding cycle ( 8 hours )

int buttonState = 1;  
int prevButtonState = 1;
 
int pos = 0;    // variable to store the servo position

void setup() {
  pinMode(buttonPin , INPUT);
  pinMode(buzzerPin , OUTPUT);
  digitalWrite(buzzerPin , HIGH ) ;
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);
}


unsigned long StartTime = millis();


void shake(){
    for (int i = 0 ; i < 10 ; i++){
      delay(300);
      myservo.write(120);
      delay(300);
      myservo.write(90);
    } 
}


void feed(){
    
    digitalWrite(buzzerPin , LOW);
    delay(1000);
    digitalWrite(buzzerPin , HIGH ) ; 
    pos = 0 ;
    myservo.write(pos); 
    delay(1000);
    pos = 90;
    myservo.write(pos);
}


void loop() {
  buttonState = digitalRead(buttonPin);
  //Serial.println(buttonState);
  unsigned long CurrentTime = millis();
  unsigned long ElapsedTime = CurrentTime - StartTime;
  
  if ( buttonState == LOW && prevButtonState == HIGH ){
    feed();
  }
  else if (ElapsedTime > CycleTime){
    shake();
    feed();
    StartTime = millis();
    Serial.println(ElapsedTime);
  }
  else {
    digitalWrite(buzzerPin , HIGH ) ; 
  }
  
  prevButtonState = buttonState;
   
  
}

