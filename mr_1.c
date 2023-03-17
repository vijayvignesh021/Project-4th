#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <BlynkSimpleSerialBLE.h>

#define BLYNK_PRINT Serial
#define GPS_TX 2
#define GPS_RX 3
#define ADXL345_ADDR (0x53)
#define X A0
#define Y A1
#define Z A2

SoftwareSerial gpsSerial(GPS_TX, GPS_RX);
TinyGPS gps;
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

double target_lat = 00.000000; // Enter your target latitude
double target_lon = 00.000000; // Enter your target longitude
double lat, lon;
float x, y, z;
bool obstacle_detected = false;

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
  Blynk.begin(Serial, "YourAuthKey");

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
        digitalWrite(5, HIGH);
        digitalWrite(6, LOW);
      } else {
        digitalWrite(5, LOW);
        digitalWrite(6, HIGH);
      }
    } else {
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
    }
  } else {
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
  }

  Blynk.run();
}

BLYNK_WRITE(V1) {
  int value = param.asInt();
  if (value == 1) {
    digitalWrite(7, HIGH);
  } else {
    digitalWrite(7, LOW);
  }
}
