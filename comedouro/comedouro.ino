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
#include <math.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards
const int ledPin = 7;
const int buttonPin = 3;
const int buzzerPin = 8;
const int groundPin = 12;

long CycleTime = 28800000; //Time (in miliseconds) of the feeding cycle ( 8 hours )

int buttonState = 1;  
int prevButtonState = 1;


//edit these values based on your case
int catAgeInMonths = 4;
int predictedAdultWeightInKg = 4;
const float gramsPerPortion = 3.6;
const int cyclesPerDay = 3;
//according to the packet 
int dailyFoodPerAge[3][11] = {{40 , 51 , 55 , 57 , 55 , 53 , 50 , 47 , 44 , 42 , 40},
                   {47 , 60 , 65 , 67 , 65 , 63 , 60 , 56 , 54 , 51 , 50},
                   {54 , 69 , 76 , 80 , 77 , 74 , 70 , 66 , 63 , 60 , 58} };              
int numberOfPortionsPerDay = round(dailyFoodPerAge[predictedAdultWeightInKg - 3][catAgeInMonths - 2]/gramsPerPortion);

int rest = numberOfPortionsPerDay%cyclesPerDay ;

int numberOfPortionsPerFeedingCycle[cyclesPerDay];
  

int pos = 0;    // variable to store the servo position
void setup() {
  for(int i = 0  ; i < cyclesPerDay ; i++ ){
    if ( rest > i){
      numberOfPortionsPerFeedingCycle[i] = ceil(numberOfPortionsPerDay/cyclesPerDay) ;
    }
    else {
      numberOfPortionsPerFeedingCycle[i] = floor(numberOfPortionsPerDay/cyclesPerDay) ;  
    } 
     
  }
  
  pinMode(buttonPin , INPUT);
  pinMode(ledPin , OUTPUT );
  digitalWrite(ledPin , HIGH );
  pinMode(buzzerPin , OUTPUT);
  digitalWrite(buzzerPin , HIGH ) ;
  myservo.attach(10);  // attaches the servo on pin 10 to the servo object
  Serial.begin(9600);
  pinMode(groundPin , OUTPUT);
  digitalWrite(groundPin , LOW ) ;

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


void buzz(){
   digitalWrite(ledPin , HIGH);
   digitalWrite(buzzerPin , LOW);
   delay(1000);
   digitalWrite(buzzerPin , HIGH ) ;
   digitalWrite(ledPin , LOW); 
}

void feed(){
    pos = 0 ;
    myservo.write(pos); 
    delay(1000);
    pos = 90;
    myservo.write(pos);
    delay(1000);
}

int currentCycle = 0; 
void loop() {
Serial.println(dailyFoodPerAge[predictedAdultWeightInKg - 3][catAgeInMonths - 2]);

  buttonState = digitalRead(buttonPin);
  //Serial.println(buttonState);
  unsigned long CurrentTime = millis();
  unsigned long ElapsedTime = CurrentTime - StartTime;
  
    //Serial.println(numberOfPortionsPerFeedingCycle);
  if ( buttonState == LOW && prevButtonState == HIGH ){
    buzz(); 
    feed();
  }
  else if (ElapsedTime > CycleTime){
    shake();
    buzz();
    //feed as many times as the cat needs
    for(int i = 0 ; i < numberOfPortionsPerFeedingCycle[currentCycle] ; i++){
      feed();
    }
    currentCycle = (currentCycle + 1)%3;
    StartTime = millis();
    //Serial.println(ElapsedTime);
  }
  else {
    digitalWrite(buzzerPin , HIGH ) ; 
  }
  
  prevButtonState = buttonState;
  
}
