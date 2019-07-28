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
#include <Wire.h>
#include <BH1750.h>
#include <i2cdetect.h>
/*
  BH1750 can be physically configured to use two I2C addresses:
    - 0x23 (most common) (if ADD pin had < 0.7VCC voltage)
    - 0x5C (if ADD pin had > 0.7VCC voltage)

  Library uses 0x23 address as default, but you can define any other address.
  If you had troubles with default value - try to change it to 0x5C.

*/
BH1750 lightMeter(0x23);

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards
const int ledPin = 3;
const int buttonPin = 2;
const int buzzerPin = 4;

long CycleTime = 28800000; //Time (in miliseconds) of the feeding cycle ( 8 hours )

int buttonState = 1;  
int prevButtonState = 1;

//TODO: make all constants uppercase
//edit these values based on your case
int catAgeInMonths = 4;
int predictedAdultWeightInKg = 4;
const float gramsPerPortion = 5.3;
const int cyclesPerDay = 3;
const float LUX_THRESHOLD = 0.5;
 
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

  
  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
  Wire.begin();
  i2cdetect();
  // begin returns a boolean that can be used to detect setup problems.
  if (lightMeter.begin()) { //TODO : verify wether I can change mode after the setup 
    Serial.println(F("BH1750 Advanced begin"));
  }
  else {
    Serial.println(F("Error initialising BH1750"));
  }


}


unsigned long StartTime = millis();


void shake(){
    for (int i = 0 ; i < 10 ; i++){
      delay(300);
      myservo.write(300);
      delay(300);
      myservo.write(270);
    } 
}

bool isEmpty = false;
float blinkLedAndGaugeLux(){
  digitalWrite(ledPin , HIGH);
  delay(1000);
  
  float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  delay(1000);
  digitalWrite(ledPin , LOW);
  return lux;   
}


void buzz(){
   digitalWrite(buzzerPin , LOW);
   delay(1000);
   digitalWrite(buzzerPin , HIGH ) ;
  
}

void feed(){
    pos = 90 ;
    myservo.write(pos); 
    delay(1000);
    pos = 300;
    myservo.write(pos);
    delay(1000);
}
int cont = 0;
int currentCycle = 0; 
void loop() {
//Serial.println(dailyFoodPerAge[predictedAdultWeightInKg - 3][catAgeInMonths - 2]);
  //Serial.println(isEmpty);
  buttonState = digitalRead(buttonPin);
  //Serial.println(buttonState\m);
  unsigned long CurrentTime = millis();
  unsigned long ElapsedTime = CurrentTime - StartTime;
  if( !isEmpty ){
    //Serial.println(numberOfPortionsPerFeedingCycle);
    if ( buttonState == LOW && prevButtonState == HIGH ){
      float lux = blinkLedAndGaugeLux(); //TODO : replace delays with proper timers
      if ( lux < LUX_THRESHOLD ){
        isEmpty = false;
      } 
      else {
        isEmpty = true;  
      } 
       
      
      if (isEmpty) 
           buzz();
      feed();
    }
    else if (ElapsedTime > CycleTime){
      shake();
      //feed as many times as the cat needs
      for(int i = 0 ; i < numberOfPortionsPerFeedingCycle[currentCycle] ; i++){
        float lux = blinkLedAndGaugeLux(); //TODO : replace delays with proper timers
        if ( lux < LUX_THRESHOLD ){
          isEmpty = false;
        } 
        else {
          isEmpty = true;  
        }
        feed();
        if (isEmpty) 
           buzz();
      }
      currentCycle = (currentCycle + 1)%3;
      StartTime = millis();
      //Serial.println(ElapsedTime);
    }
    else {
      digitalWrite(buzzerPin , HIGH ) ; 
    }
  }
  //
  //Serial.print("Light: ");
  //Serial.print(lux);
  //Serial.println(" lx");
  //Serial.println(buttonState);
    
  //if vessel is empty, keep led on and reads continuously the light sensor, waiting for a change of state
  if (isEmpty){
    
    digitalWrite(ledPin , HIGH );
    delay(1000);  
    float lux = lightMeter.readLightLevel();
     Serial.print("Light: ");
    Serial.print(lux);
    Serial.println(" lx");
  //Serial.println(buttonState);
        
    if ( lux < LUX_THRESHOLD){
      cont = cont + 1;
      isEmpty = false;
      digitalWrite(ledPin , LOW ); 
    }
    else{
      cont = 0 ;
    //  isEmpty  = true;
    }
    
  }
  
  prevButtonState = buttonState;
  
}
