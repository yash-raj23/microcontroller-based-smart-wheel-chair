// Include the library files
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL3KNqtdMcc"
#define BLYNK_TEMPLATE_NAME "wifi car"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// Define the motor pins
#define ENA D0
#define IN1 D1
#define IN2 D2
#define IN3 D3
#define IN4 D4
#define ENB D5
const int vibrationSensorPin = A0;

// Variables for the Blynk widget values
int x = 50;
int y = 50;
int Speed;
BlynkTimer timer;

char auth[] = "WpqZOHpO5aTP3Y_K1be4IrKTNBTzDJ5V"; //Enter your Blynk auth token
char ssid[] = "YASH RAJ"; //Enter your WIFI name
char pass[] = "yash2301"; //Enter your WIFI passowrd

// Your Telegram credentials.
#define BOTtoken "6842283036:AAGTYR4dqqIayImEM94xOL7RFCyguNPRt5A" // Enter the bottoken you got from botfather
#define CHAT_ID "1415326666" // Enter your chatID you got from chatid bot

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);


void setup() {
  Serial.begin(9600);
  //Set the motor pins as output pins
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  timer.setInterval(1000L, sendSensorData);
  pinMode(vibrationSensorPin, INPUT);

  configTime(0, 0, "pool.ntp.org");      
  client.setTrustAnchors(&cert);

  // Initialize the Blynk library
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

  bot.sendMessage(CHAT_ID, "Wifi Connected!", "");
  bot.sendMessage(CHAT_ID, "System has Started!!", "");
}

// Get the joystick values
BLYNK_WRITE(V0) {
  x = param[0].asInt();
}
// Get the joystick values
BLYNK_WRITE(V1) {
  y = param[0].asInt();
}
//Get the slider values
BLYNK_WRITE(V2) {
  Speed = param.asInt();
}

// Check these values using the IF condition
void smartcar() {
  if (y > 70) {
    carForward();
    Serial.println("carForward");
  } else if (y < 30) {
    carBackward();
    Serial.println("carBackward");
  } else if (x < 30) {
    carLeft();
    Serial.println("carLeft");
  } else if (x > 70) {
    carRight();
    Serial.println("carRight");
  } else if (x < 70 && x > 30 && y < 70 && y > 30) {
    carStop();
    Serial.println("carstop");
  }
}
void loop() {

  int sensorValue,outputValue=0;  

  Blynk.run();// Run the blynk function
  timer.run(); // Initiates BlynkTimer
  smartcar();// Call the main function

  if (sensorValue >= 30) {
    bot.sendMessage(CHAT_ID, "Vibration Detected", "");
  }
  else if(outputValue >= 30){
    bot.sendMessage(CHAT_ID, "Vibration Detected", "");
  }

}

/**************Motor movement functions*****************/
void carForward() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void carBackward() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void carLeft() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void carRight() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void carStop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
void sendSensorData()
{
  // Read the analog value from sensor
  int sensorValue = analogRead(vibrationSensorPin);
  // Map the value to the range 0-255
  int outputValue = map(sensorValue, 0, 1023, 0, 255);
  // Send the mapped value to Blynk's Virtual Pin (V1)
  Blynk.virtualWrite(V7, outputValue);
}