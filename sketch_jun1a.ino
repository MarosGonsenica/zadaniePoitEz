#include <ESP8266WiFi.h>
#include <WiFiClient.h>

// defines pins numbers
const int trigPin = 12;
const int echoPin = 14;
const int lampPin = 2;
const int lampPinClose = 16;
// defines variables
long duration;
int distance;

int distanceFar = 250;
int distanceCloser1 = 200;
int distanceCloser2 = 150;
int distanceCloser3 = 100;
int distanceClose = 50;

void setup() {
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input
    pinMode(lampPin, OUTPUT); // Sets the lampPin as an Output
    pinMode(lampPinClose, OUTPUT); // Sets the lampPinClose as an Output
    Serial.begin(9600); // Starts the serial communication
    Serial.setTimeout(2000);
}

void loop() {
    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n');
        int delimiter1 = input.indexOf(',');
        int delimiter2 = input.indexOf(',', delimiter1 + 1);
        int delimiter3 = input.indexOf(',', delimiter2 + 1);
        int delimiter4 = input.indexOf(',', delimiter3 + 1);
        
        distanceFar = input.substring(0, delimiter1).toInt();
        distanceCloser1 = input.substring(delimiter1 + 1, delimiter2).toInt();
        distanceCloser2 = input.substring(delimiter2 + 1, delimiter3).toInt();
        distanceCloser3 = input.substring(delimiter3 + 1, delimiter4).toInt();
        distanceClose = input.substring(delimiter4 + 1).toInt();
    }

    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    digitalWrite(lampPinClose, HIGH);
    digitalWrite(lampPin, HIGH);
    // Sets the trigPin on HIGH state for 10 microseconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);

    // Calculating the distance
    distance = duration * 0.034 / 2;

    // Prints the distance on the Serial Monitor
    Serial.println(distance);

    // Adjust lamp behavior based on distance
    if (distance > distanceFar) {
        digitalWrite(lampPinClose, HIGH);
        digitalWrite(lampPin, HIGH);
    } else if (distance < distanceFar && distance >= distanceCloser1) {
        digitalWrite(lampPin, LOW);
        delay(500);
        digitalWrite(lampPin, HIGH);
        delay(500);
    } else if (distance < distanceCloser1 && distance >= distanceCloser2) {
        digitalWrite(lampPinClose, HIGH);
        digitalWrite(lampPin, HIGH);
        delay(500);
        digitalWrite(lampPinClose, LOW);
        digitalWrite(lampPin, LOW);
        delay(500);
    } else if (distance < distanceCloser2 && distance >= distanceCloser3) {
        digitalWrite(lampPinClose, HIGH);
        digitalWrite(lampPin, HIGH);
        delay(250);
        digitalWrite(lampPinClose, LOW);
        digitalWrite(lampPin, LOW);
        delay(250);
    } else if (distance < distanceCloser3 && distance >= distanceClose) {
        digitalWrite(lampPinClose, HIGH);
        digitalWrite(lampPin, HIGH);
        delay(125);
        digitalWrite(lampPinClose, LOW);
        digitalWrite(lampPin, LOW);
        delay(125);    
    } else {
        digitalWrite(lampPinClose, LOW);
        digitalWrite(lampPin, LOW);

    }

    delay(100);
}
