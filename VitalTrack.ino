#include <SoftwareSerial.h>
#include <PulseSensorPlayground.h>

SoftwareSerial xbee(0,1); // RX, TX
int sensorPin = A4; //temperator sensor
float tempThreshMax = 37.5; //temp trigger thresh
float tempThreshMin = 35.5; //temp trigger thresh
const int xpin = A1; //accerlerometer 
const int ypin = A2;//accerlerometer 
const int zpin = A0;//accerlerometer 
int alertLEDRed = 13;
int alertLEDGreen = 12;
const int PulseWire = A3; //Pulse snesor
const int LED = LED_BUILTIN;
int pulseThresh = 550; //invalid pulse
int bpmThreshMax = 180; //bpm trigger thresh
int bpmThreshMin = 60; //bpm trigger thresh
PulseSensorPlayground pulseSensor;
int sampleDelay = 100;
int steps = 0;
int sensorValue = 0;

void setup() {
  Serial.begin(9600);
  xbee.begin(9600);
  pinMode(sensorPin, INPUT);
  pinMode(alertLEDRed, OUTPUT);
  pinMode(xpin, INPUT);
  pinMode(ypin, INPUT);
  pinMode(zpin, INPUT);
  pulseSensor.analogInput(PulseWire);
  pulseSensor.blinkOnPulse(LED);
  pulseSensor.setThreshold(pulseThresh);
  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");
  }
}

void loop() {
  //temp
  long rawTemp;
  float voltage;
  float fahrenheit;
  float celsius;
  rawTemp = analogRead(sensorPin);
  voltage = rawTemp * (3.3 / 1023.0);
  celsius = (voltage - 0.5) * 100;
  Serial.print("celsius: ");
  Serial.print(celsius);
  Serial.print("; ");
  xbee.print("celsius:");
  xbee.print(celsius);
  xbee.print("; ");
  fahrenheit = (celsius * 9.0 / 5.0) + 32.0;
  //red led trigger by temp
  if ((celsius < tempThreshMin) or (celsius > tempThreshMax)) {
    digitalWrite(alertLEDRed, HIGH);
    Serial.print("TEMPERATURE ALERT!!!");
    Serial.print("; ");
    xbee.print("TEMPERATURE ALERT!!!");
    xbee.print("; ");
  } else {
    digitalWrite(alertLEDRed, LOW);
  }
  //Accerlerometer
  int x = analogRead(xpin);
  delay(1);
  int y = analogRead(ypin);
  delay(1);
  int z = analogRead(zpin);
  // y value when hold still
  float zero_G =512;
  
  float scale = 102.3;
  Serial.print("x: ");
  Serial.print(x);
  Serial.print(", ");
  Serial.print("y: ");
  Serial.print(y);
  Serial.print(", ");
  Serial.print("z: ");
  Serial.print(z);
  Serial.print("; ");

  xbee.print("x: ");
  xbee.print(x);
  xbee.print(", ");
  xbee.print("y: ");
  xbee.print(y);
  xbee.print(", ");
  xbee.print("z: ");
  xbee.print(z);
  xbee.print("; ");
  float accelThresh = -((float)y - zero_G) / scale
  //Serial.print("y = ");
  //Serial.print(((float)y - zero_G) / scale);
  Serial.print("; accelThresh = ");
  Serial.print(accelThresh);
  Serial.print("; ");
  xbee.print("; accelThresh = ");
  xbee.print(accelThresh);
  xbee.print("; ");
  if (accelThresh > 2) {
    steps++;
    Serial.print("steps = ");
    Serial.print(steps);
    Serial.print("; ");
    xbee.print("steps = ");
    xbee.println(steps);
    Serial.print("; ");
  }
  if (pulseSensor.sawStartOfBeat()) {
    int myBPM = pulseSensor.getBeatsPerMinute();

    Serial.print("; BPM: ");
    Serial.print(myBPM);
    Serial.print("; ");

    xbee.print("; BPM: ");
    xbee.print(myBPM);
    xbee.print("; ");

    if ((myBPM < bpmThreshMin) or (myBPM > bpmThreshMax)) {
    digitalWrite(alertLEDGreen, HIGH);
    Serial.println("BPM ALERT!!!");
    Serial.print("; ");
    xbee.println("BPM ALERT!!!");
    xbee.print("; ");
  } else {
    digitalWrite(alertLEDGreen, LOW);
  }

  }
  Serial.print("\n");
  delay(100);
}
