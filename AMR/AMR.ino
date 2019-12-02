#include <ServoTimer2.h>

//========================================================
// Constant variables
//========================================================
const int LIGHT_THRESHOLD  = 570;
const int FOLLOW_THRESHOLD = 750;

const int MAX_DISTANCE     = 11;
const int MAX_HEIGHT       = 20;
const int MAX_SPEED        = 128;

//========================================================
// Motor & other variables
//========================================================
int buttonPin        = 13; // Pin D13
int buzzerPin        = 12; // Pin D12

int left_motor_en    = 10; // Pin D10
int left_motor_back  = 9;  // Pin D9
int left_motor_go    = 8;  // Pin D8

int right_motor_back = 7;  // Pin D7
int right_motor_go   = 6;  // Pin D6
int right_motor_en   = 5;  // Pin D5

//=========================================================
// Ultrasonic object avoidance variables
//=========================================================
int ObjectRightEcho = A1; // Set right Echo port
int ObjectRightTrig = 4;  // Set right Trig port
int ObjectRightDist = 0;

int ObjectLeftEcho = A5;  // Set left Echo port
int ObjectLeftTrig = A4;  // Set left Trig port
int ObjectLeftDist = 0;

int ObjectMidEcho = A2;   // Set middle Echo port
int ObjectMidTrig = A3;   // Set middle Trig port
int ObjectMidDist = 0;

//=========================================================
// Servo variables
//=========================================================
int ServoPort = 0;  // Servo port
ServoTimer2 servo;  // Servo var

//=========================================================
// IR sensor variables
//=========================================================
const int IRSensorRight = 1;  // IR sensor right port (D1)
const int IRSensorMid   = 3;  // IR sensor mid port (D3)
const int IRSensorLeft  = 11; // IR sensor left port (D11)
int SL, SM, SR;               // IR sensor states

//=========================================================
// Light sensor variables
//=========================================================
int LightSensorPin = A0;                   // Light sensor port
int LightSensorVal = 0;                    // Current light sensor value
int LightSensorValues[NUM_POSITIONS] = {}; // Light sensor values for the different servo positions

// Initial setup code
void setup() {
  // Begin serial output
  Serial.begin(9600); // Do not uncomment this unless you want debugging and aren't using D0 and/or D1
  
  // Initialize left and right motor drive for output mode.
  pinMode(left_motor_go,    OUTPUT);
  pinMode(left_motor_back,  OUTPUT);
  pinMode(right_motor_go,   OUTPUT);
  pinMode(right_motor_back, OUTPUT);
  pinMode(right_motor_en,   OUTPUT);
  pinMode(left_motor_en,    OUTPUT);

  pinMode(buttonPin, INPUT);        // Set button as input
  //pinMode(beep, OUTPUT);          // Set buzzer as output

  pinMode(LightSensorPin, INPUT);   // Set light sensor as input

  pinMode(ObjectRightEcho, INPUT);  // Set Echo port mode
  pinMode(ObjectRightTrig, OUTPUT); // Set Trig port mode

  pinMode(ObjectLeftEcho, INPUT);   // Set Echo port mode
  pinMode(ObjectLeftTrig, OUTPUT);  // Set Trig port mode

  pinMode(ObjectMidEcho, INPUT);    // Set Echo port mode
  pinMode(ObjectMidTrig, OUTPUT);   // Set Trig port mode

  servo.attach(ServoPort);          // Attach the servo to the servo port

  digitalWrite(buttonPin, HIGH);    // Initialize button
  //digitalWrite(buzzerPin, LOW);   // Set buzzer mute

  pinMode(IRSensorRight, INPUT);    // Set right Line Walking IR sensor as input
  pinMode(IRSensorLeft,  INPUT);    // Set left Line Walking IR sensor as input
  pinMode(IRSensorMid,   INPUT);    // Set middle Line Walking IR sensor as input
}

// Move forward
void run() {
  digitalWrite(left_motor_en, HIGH);   // Left motor enable
  analogWrite(left_motor_en, 130);
  digitalWrite(right_motor_en, HIGH);  // Right motor enable

  digitalWrite(right_motor_go, HIGH);  // Right motor go ahead
  digitalWrite(right_motor_back, LOW);
  analogWrite(right_motor_go, 130);    // PWM--Pulse Width Modulation (0~255). right motor go speed is 130
  analogWrite(right_motor_back, 0);

  digitalWrite(left_motor_go, HIGH);   // Left motor go ahead
  digitalWrite(left_motor_back, LOW);
  analogWrite(left_motor_go, 130);     // PWM--Pulse Width Modulation (0~255). left motor go speed is 130
  analogWrite(left_motor_back, 0);
}

// Stop
void brake() {
  digitalWrite(right_motor_go,   LOW); // Stop the right motor
  digitalWrite(right_motor_back, LOW);

  digitalWrite(left_motor_go,    LOW); // Stop the left motor
  digitalWrite(left_motor_back,  LOW);
}

// Turn left
void left() {
  digitalWrite(right_motor_go, HIGH);  // Right motor go ahead
  digitalWrite(right_motor_back, LOW);
  analogWrite(right_motor_go, 190);    // PWM--Pulse Width Modulation(0~255) control speed，right motor go speed is 190.
  analogWrite(right_motor_back, 0);
  
  digitalWrite(left_motor_go, LOW);    // Left motor stop
  digitalWrite(left_motor_back, LOW); 
  analogWrite(left_motor_go, 0); 
  analogWrite(left_motor_back, 0);
  delay(150); 
}

// Rotate left
void spin_left(int time) {
  digitalWrite(right_motor_go,  HIGH);     // Right motor go ahead
  digitalWrite(right_motor_back, LOW);
  analogWrite(right_motor_go, MAX_SPEED);  // PWM--Pulse Width Modulation(0~255) control speed, right motor go speed is 200
  analogWrite(right_motor_back, 0);

  digitalWrite(left_motor_go,   LOW);      // Left motor back off
  digitalWrite(left_motor_back, HIGH);
  analogWrite(left_motor_go, 0);
  analogWrite(left_motor_back, MAX_SPEED); // PWM--Pulse Width Modulation(0~255) control speed, left motor back speed is 200.

  delay(time * 100);
}

// Turn right
void right() {
  digitalWrite(right_motor_go, LOW);   // Right motor stop
  digitalWrite(right_motor_back, LOW);
  analogWrite(right_motor_go, 0); 
  analogWrite(right_motor_back, 0);
  
  digitalWrite(left_motor_go, HIGH);   // Left motor go ahead
  digitalWrite(left_motor_back, LOW);
  analogWrite(left_motor_go, 230);     // PWM--Pulse Width Modulation(0~255) control speed ,left motor go speed is 230.
  analogWrite(left_motor_back, 0);
  delay(150);
}

// Rotate right
void spin_right(int time) {
  digitalWrite(right_motor_go,    LOW);     // Right motor back off
  digitalWrite(right_motor_back, HIGH);
  analogWrite(right_motor_go, 0);
  analogWrite(right_motor_back, MAX_SPEED); // PWM--Pulse Width Modulation(0~255) control speed

  digitalWrite(left_motor_go,  HIGH);       // Left motor go ahead
  digitalWrite(left_motor_back, LOW);
  analogWrite(left_motor_go, MAX_SPEED);    // PWM--Pulse Width Modulation(0~255) control speed
  analogWrite(left_motor_back, 0);

  delay(time * 100);
}

// Go backwards
void back(int time) {
  digitalWrite(right_motor_go,    LOW);     // Right motor back off
  digitalWrite(right_motor_back, HIGH);
  analogWrite(right_motor_go, 0);
  analogWrite(right_motor_back, MAX_SPEED); // PWM--Pulse Width Modulation(0~255) control speed
  analogWrite(right_motor_en, 200);

  digitalWrite(left_motor_go,    LOW);      // Left motor back off
  digitalWrite(left_motor_back, HIGH);
  analogWrite(left_motor_go, 0);
  analogWrite(left_motor_back, MAX_SPEED);  // PWM--Pulse Width Modulation(0~255) control speed

  delay(time * 100);
}

// Right ultrasonic sensor distance test
void ObjectRightDistanceTest() {
  digitalWrite(ObjectRightTrig, LOW);               // Set trig port low level for 2μs
  delayMicroseconds(2);
  digitalWrite(ObjectRightTrig, HIGH);              // Set trig port high level for 10μs(at least 10μs)
  delayMicroseconds(10);
  digitalWrite(ObjectRightTrig, LOW);               // Set trig port low level
  float Fdistance = pulseIn(ObjectRightEcho, HIGH); // Read echo port high level time(unit:μs)
  Fdistance= Fdistance/58;                          // Distance(m) =(time(s) * 344(m/s)) / 2     /****** The speed of sound is 344m/s.*******/
                                                    //  ==> 2*Distance(cm) = time(μs) * 0.0344(cm/μs)
                                                    //  ==> Distance(cm) = time(μs) * 0.0172 = time(μs) / 58
  //Serial.print("Object right Distance:");             // Output Distance(cm)
  //Serial.println(Fdistance);                        // Display distance
  ObjectRightDist = Fdistance;
}  

// Left ultrasonic sensor distance test
void ObjectLeftDistanceTest() {
  digitalWrite(ObjectLeftTrig, LOW);               // Set trig port low level for 2μs
  delayMicroseconds(2);
  digitalWrite(ObjectLeftTrig, HIGH);              // Set trig port high level for 10μs(at least 10μs)
  delayMicroseconds(10);
  digitalWrite(ObjectLeftTrig, LOW);               // Set trig port low level
  float Fdistance = pulseIn(ObjectLeftEcho, HIGH); // Read echo port high level time(unit:μs)
  Fdistance= Fdistance/58;                         // Distance(m) =(time(s) * 344(m/s)) / 2     /****** The speed of sound is 344m/s.*******/
                                                   //  ==> 2*Distance(cm) = time(μs) * 0.0344(cm/μs)
                                                   //  ==> Distance(cm) = time(μs) * 0.0172 = time(μs) / 58
  //Serial.print("Object left Distance:");           // Output Distance(cm)
  //Serial.println(Fdistance);                       // Display distance
  ObjectLeftDist = Fdistance;
}  

// Middle ultrasonic sensor distance test
void ObjectMidDistanceTest() {
  digitalWrite(ObjectMidTrig, LOW);               // Set trig port low level for 2μs
  delayMicroseconds(2);
  digitalWrite(ObjectMidTrig, HIGH);              // Set trig port high level for 10μs(at least 10μs)
  delayMicroseconds(10);
  digitalWrite(ObjectMidTrig, LOW);               // Set trig port low level
  float Fdistance = pulseIn(ObjectMidEcho, HIGH); // Read echo port high level time(unit:μs)
  Fdistance= Fdistance/58;                        // Distance(m) =(time(s) * 344(m/s)) / 2     /****** The speed of sound is 344m/s.*******/
                                                  //  ==> 2*Distance(cm) = time(μs) * 0.0344(cm/μs)
                                                  //  ==> Distance(cm) = time(μs) * 0.0172 = time(μs) / 58
  //Serial.print("Object middle Distance:");        // Output Distance(cm)
  //Serial.println(Fdistance);                      // Display distance
  ObjectMidDist = Fdistance;
}

// Store the light sensor value based on the current servo position.
void StoreLightValue(int pos) {
  LightSensorVal = analogRead(LightSensorPin);
  if (pos < NUM_POSITIONS) {
    LightSensorValues[pos] = LightSensorVal;
  }
}

// Returns whether or not there is a candle in front of the robot.
boolean isCandlePresent() {
  LightSensorVal = analogRead(LightSensorPin);

  if (LightSensorVal < LIGHT_THRESHOLD) {
    //Serial.print("Candle is present: ");
    //Serial.println(LightSensorVal);
    return true;
  }
  //Serial.print("No candle present: ");
  //Serial.println(LightSensorVal);
  return false;
}

// Check for button press
void keyscan() {
  int val = digitalRead(buttonPin);     // Reads the button, the level value assigns to val.
  while (digitalRead(buttonPin)) {      // When the button is not pressed
    val = digitalRead(buttonPin);
  }
  while (!digitalRead(buttonPin)) {     // When the button is pressed
    delay(10); // delay 10ms
    val = digitalRead(buttonPin);       // Read
    if (val == LOW) {                   // Double check the button is pressed
      digitalWrite(buzzerPin, HIGH);
      delay(50);                        // Delay 50ms
      while (!digitalRead(buttonPin)) { // Determine if button is released or not
        digitalWrite(buzzerPin, LOW);   // Mute
      }
    }
    else {
      digitalWrite(buzzerPin, LOW);     // Mute
    }
  }
}

void loop() {
  keyscan(); // Press the button to start
  while (true) {
    SM = digitalRead(IRSensorMid);
    if (SM == LOW) {
      Serial.println("LOW");
    }
    else {
      Serial.println("HIGH");
    }
  }
}
