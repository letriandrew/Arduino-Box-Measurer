#include <LiquidCrystal.h>

LiquidCrystal lcd(1, 2, 4, 5, 6, 7); // Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)

const int trigPin = 9;
const int echoPin = 10;
long duration;
int distanceCm;

const int buttonPin = 11;
int buttonStatus = 0;

int angle1 = 0;
int angle3 = 0;
int bangle1 = 0;
int bangle3 = 0;

bool saveActivate1 = false;
bool saveActivate2 = false;
bool saveActivate3 = false;
bool saveActivate4 = false;
bool doContinue = false;
bool finisher = false;

void setup() {
  //Serial.begin(9600);
  lcd.begin(16,2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

void distanceDisplay(int distance){
  lcd.setCursor(0,0); // Sets the location at which subsequent text written to the LCD will be displayed
  lcd.print("Distance: "); // Prints string "Distance" on the LCD
  lcd.print(distance); // Prints the distance value from the sensor
  lcd.print("  cm");
  delay(10);
  lcd.setCursor(0,1);
  lcd.print("Prs Buttn = Save");
  delay(10);
}

int distance(long duration){
  return duration*0.034/2;
}

void save(String sidename, int side, int distance){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(sidename);
  lcd.print(" saved");
  delay(10);
  lcd.setCursor(0,1);
  lcd.print(distance);
  lcd.print(" cm");
  delay(10);
}

void readybut(){
  lcd.clear();
  lcd.setCursor(0,0); 
  lcd.print("Press button "); 
  lcd.setCursor(0,1);
  lcd.print("when ready"); 
  delay(10);
}

bool check(int angle1, int angle3, int bangle1, int bangle2){
  if(angle1==angle3==bangle1==bangle3==0){
    return true;
  }
  else{
    return false;
  }
}

void loop() {
  if(doContinue == false){
    readybut();
    doContinue = true;
  }
  
  int pinValue = digitalRead(buttonPin);
  delay(10);
  while(buttonStatus == pinValue){
    buttonStatus = 1;
    lcd.clear();
    
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    distanceDisplay(distance(pulseIn(echoPin,HIGH)));

    pinValue = digitalRead(buttonPin);
    delay(100);
    if(buttonStatus != pinValue){
      if(bangle1 > 0){
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
        saveActivate4 = true;
        bangle3 = distance(pulseIn(echoPin,HIGH));
        break;
      }
      else if(angle3 > 0){
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
        saveActivate3 = true;
        bangle1 = distance(pulseIn(echoPin,HIGH));
        break;
      }
      else if(angle1 > 0){
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
        saveActivate2 = true;
        angle3 = distance(pulseIn(echoPin,HIGH));
        break;
      }
      if(angle1 == 0){
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
        saveActivate1 = true;
        angle1 = distance(pulseIn(echoPin,HIGH));
        break;
      }
    }
  }
  
  delay(100);
  
  if(saveActivate1 == true){
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    save("a1", angle1, distance(pulseIn(echoPin,HIGH)));
    delay(5000);
    saveActivate1 = false;
  }
  else if(saveActivate2 == true){
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    save("a3", angle3, distance(pulseIn(echoPin,HIGH)));
    delay(5000);
    saveActivate2 = false;
  }
  else if(saveActivate3 == true){
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    save("b1", bangle1, distance(pulseIn(echoPin,HIGH)));
    delay(5000);
    saveActivate3 = false;
  }
  else if(saveActivate4 == true){
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    save("b3", bangle3, distance(pulseIn(echoPin,HIGH)));
    delay(5000);
    saveActivate4 = false;
    finisher = true;
  }
  
  if(finisher == true){
    bool finish = true;
    int sqra1 = angle1 * angle1;
    int sqra3 = angle3 * angle3;
    int sqrb1 = bangle1 * bangle1;
    int sqrb3 = bangle3 * bangle3;
    int x = sqrt(sqra3-sqra1);
    int y = sqrt(sqrb3-sqrb1);
    
    while(finish){
      lcd.clear();
      lcd.setCursor(0,0); 
      lcd.print("X =  "); 
      lcd.print(x);
      lcd.setCursor(0,1);
      lcd.print("Y = "); 
      lcd.print(y);
      delay(10);
    }
  }
}
