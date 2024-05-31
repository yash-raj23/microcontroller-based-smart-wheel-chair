#define echoPin 9 
#define trigPin 10 
long duration; // Variable to store time taken to the pulse to reach receiver 
int distance; // Variable to store distance calculated using formula
//ultrasonic sensor

#include <Servo.h>
Servo servo1;
int pot = A2;
int valPot;
//Servo and Pot

#define enA 7
#define in1 6
#define in2 5

#define in3 4
#define in4 3
#define enB 2

int motorSpeedA = 0;
int motorSpeedB = 0;
//motor
void setup() {
  // put your setup code here, to run once:
    pinMode(enA, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);
    pinMode(enB, OUTPUT);

    servo1.attach(13);

    pinMode(trigPin,OUTPUT); // Sets the trigPin as an OUTPUT
    pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
    pinMode(11,OUTPUT);
    // Serial Communication is starting with 9600 of
    // baudrate speed
    Serial.begin(9600); 
    // The text to be printed in serial monitor
    Serial.println("Distance measurement using Arduino Uno.");
    delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2); // wait for 2 ms to avoid
                          // collision in serial monitor
 
    digitalWrite(trigPin,HIGH); // turn on the Trigger to generate pulse
    delayMicroseconds(10); // keep the trigger "ON" for 10 ms to generate
             // pulse for 10 ms.
 
    digitalWrite(trigPin,LOW); // Turn off the pulse trigger to stop
                       // pulse generation
 
    // If pulse reached the receiver echoPin
    // become high Then pulseIn() returns the
    // time taken by the pulse to reach the
    // receiver
 
    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.0344 / 2; // Expression to calculate
                                 // distance using time
 
    Serial.print("Distance: ");
    Serial.print(distance); // Print the output in serial monitor
    Serial.println(" cm");
    if(distance<=65)
    {
    digitalWrite(11,HIGH);
    delay(200);
    }
    else
    {
      digitalWrite(11,LOW);
    delay(200);
    }
    //delay(500);





  int xAxis = analogRead(A0); // Read Joysticks X-axis
  int yAxis = analogRead(A1); // Read Joysticks Y-axis
  Serial.println(yAxis);
  // Y-axis used for forward and backward control
  if (yAxis < 470) {
    // Set Motor A backward
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    // Set Motor B backward
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    // Convert the declining Y-axis readings for going backward from 470 to 0 into 0 to 130 value for the PWM signal for increasing the motor speed
    motorSpeedA = map(yAxis, 470, 0, 0, 130);
    motorSpeedB = map(yAxis, 470, 0, 0, 130);
  }
  else if (yAxis > 550) {
    // Set Motor A forward
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    // Set Motor B forward
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    // Convert the increasing Y-axis readings for going forward from 550 to 1023 into 0 to 130 value for the PWM signal for increasing the motor speed
    motorSpeedA = map(yAxis, 550, 1023, 0, 130);
    motorSpeedB = map(yAxis, 550, 1023, 0, 130);
  }
  // If joystick stays in middle the motors are not moving
  else {
    motorSpeedA = 0;
    motorSpeedB = 0;
  }

  // X-axis used for left and right control
  if (xAxis < 470) {
    // Convert the declining X-axis readings from 470 to 0 into increasing 0 to 130 value
    int xMapped = map(xAxis, 470, 0, 0, 130);
    // Move to left - decrease left motor speed, increase right motor speed
    motorSpeedA = motorSpeedA - xMapped;
    motorSpeedB = motorSpeedB + xMapped;
    // Confine the range from 0 to 130
    if (motorSpeedA < 0) {
      motorSpeedA = 0;
    }
    if (motorSpeedB > 130) {
      motorSpeedB = 130;
    }
  }

  if (xAxis > 550) {
    // Convert the increasing X-axis readings from 550 to 1023 into 0 to 130 value
    int xMapped = map(xAxis, 550, 1023, 0, 130);
    // Move right - decrease right motor speed, increase left motor speed
    motorSpeedA = motorSpeedA + xMapped;
    motorSpeedB = motorSpeedB - xMapped;
    // Confine the range from 0 to 130
    if (motorSpeedA > 130) {
      motorSpeedA = 130;
    }
    if (motorSpeedB < 0) {
      motorSpeedB = 0;
    }
  }
  // Prevent buzzing at low speeds (Adjust according to your motors. My motors couldn't start moving if PWM value was below value of 70)
  if (motorSpeedA < 70) {
    motorSpeedA = 0;
  }
  if (motorSpeedB < 70) {
    motorSpeedB = 0;
  }
  analogWrite(enA, motorSpeedA); // Send PWM signal to motor A
  analogWrite(enB, motorSpeedB); // Send PWM signal to motor B




  valPot = analogRead(pot);
  valPot = map (valPot, 0, 1023, 0, 180);   //scale it to use it with the servo (value between 0 and 180)
  servo1.write(valPot);                      //set the servo position according to the scaled value
  delay(5);
}
