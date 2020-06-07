#include <SoftwareSerial.h>

//right motor
int in1 = 4;
int in2 = 5;

//left motor
int in3 = 6;
int in4 = 7;

// Ultrasonic Sensor
int echoPin = 12;
int trigPin = 13;
long duration, distance;

// Bluetooth Module
String order = "";
SoftwareSerial btModule(8,9);

void setup() {
//  pinMode(in1, OUTPUT);
//  pinMode(in2, OUTPUT);
//  pinMode(in3, OUTPUT);
//  pinMode(in4, OUTPUT);

  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);

  Serial.begin(9600);
  btModule.begin(9600);
  
}

void forward() {
  // Both wheels move forward
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void backward() {
  // Both wheels move backwards
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void left() {
  // Left wheel stops, right wheel moves
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void right() {
  // Right wheel stops, left wheel moves
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void turnOff() {
  // Both wheels stop
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void checkDistance() {
  // Get clean high pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read signal
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  //time -> distance
  distance = (duration/2) / 29.1; // cm

  Serial.println(distance);
  delay(50);
}

void loop() {

  while (btModule.available() > 0) {
    order = btModule.read();
    
    if (order == "48") {
      turnOff();
    } else if (order == "49") {
      do {
        checkDistance();
  
        while (distance >= 20 && order != "48") {
          forward();
          checkDistance();
          order = btModule.read();
        }
        turnOff();
        delay(1000);

        backward();
        delay(500);
        
        while (distance <= 20) {
          checkDistance();
          right();
        }
        turnOff();
        delay(1000);
        checkDistance();
        
        while (distance >= 20 && order != "48") {
          forward();
          checkDistance();
          order = btModule.read();
        }
        turnOff();
      } while (order != "48");
    }
    order = "";
  }
}
