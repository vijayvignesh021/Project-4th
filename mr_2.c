#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_PRINT Serial
#define GPS_TX D1
#define GPS_RX D2
#define ADXL345_ADDR (0x53)
#define X A0
#define Y A1
#define Z A2

SoftwareSerial gpsSerial(GPS_TX, GPS_RX);
TinyGPS gps;
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";
char auth[] = "Your_BLYNK_AUTH_TOKEN";

double target_lat = 00.000000; // Enter your target latitude
double target_lon = 00.000000; // Enter your target longitude
double lat, lon;
float x, y, z;
bool obstacle_detected = false;

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
  Blynk.begin(auth, ssid, password);

  if(!accel.begin(ADXL345_ADDR)) {
    Serial.println("Could not find a valid ADXL345 sensor, check wiring!");
    while (1);
  }
  accel.setRange(ADXL345_RANGE_16_G);
}

void loop() {
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      gps.get_position(&lat, &lon);
    }
  }

  sensors_event_t event;
  accel.getEvent(&event);
  x = event.acceleration.x;
  y = event.acceleration.y;
  z = event.acceleration.z;

  if (abs(x) > 6 || abs(y) > 6 || abs(z) > 6) {
    obstacle_detected = true;
  } else {
    obstacle_detected = false;
  }

  if (!obstacle_detected) {
    float distance = TinyGPS::distance_between(lat, lon, target_lat, target_lon) / 1000.0;
    float bearing = TinyGPS::course_to(lat, lon, target_lat, target_lon);

    Serial.print("Distance to target: ");
    Serial.print(distance);
    Serial.print(" km, bearing: ");
    Serial.print(bearing);
    Serial.println(" deg");

    if (distance > 0.01) {
      if (bearing < 180) {
        digitalWrite(D5, HIGH);
        digitalWrite(D6, LOW);
      } else {
        digitalWrite(D5, LOW);
        digitalWrite(D6, HIGH);
      }
    } else {
      digitalWrite(D5, LOW);
      digitalWrite(D6, LOW);
    }
  } else {
    digitalWrite(D5, LOW);
    digitalWrite(D6, LOW);
  }

  Blynk.run();
}

BLYNK_WRITE(V1) {
  int value = param.asInt();
  if (value == 1) {
    digitalWrite(D7, HIGH);
  } else {
    digitalWrite(D7, LOW);
  }
}
