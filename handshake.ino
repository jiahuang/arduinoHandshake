/*
  Handshake matching
 */

#include <SoftwareSerial.h>

SoftwareSerial impSerial(8, 9);

const int TIME_THRESHOLD_MIN = 20;
const int TIME_THRESHOLD_MAX = 400;
const int MIN_CROSSINGS = 4;
const int AVG_SIZE = 5; // average the first 5 values for a baseline
const int DELAY_TIME = 3; 
const int THRESHOLD = 2; // accelerometer values must be greater than this
const char * UUID = "ABC123";

long timestamp = 0;
long timestamps[MIN_CROSSINGS];
int timestamp_pos = 0;
int prev = 0;
int current = 0;
int avg = 0;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
//  Serial.print("test");
  int avgPos = 0;
  while (avgPos < AVG_SIZE) {
    avgPos++;
    avg += analogRead(A5);
    current = prev = analogRead(A5);
    delay(DELAY_TIME);
  }
  avg = avg/AVG_SIZE;
}

boolean crossedAvg() {
  return ((prev < avg && current > avg) || (prev > avg && current < avg));
}

void reset() {
//  Serial.println("reset");
  timestamp_pos = 0;
  timestamp = 0;
}

// the loop routine runs over and over again forever:
void loop() {
  // get current value
  current = analogRead(A5);
//  Serial.println(current);
  // check if value has crossed over median
  if (crossedAvg()) {
    
    if (timestamp_pos == 0) {
      timestamp = 0;
      timestamps[timestamp_pos] = 0;
      timestamp_pos++;
    } else if (timestamp_pos < MIN_CROSSINGS) {
//      for (int i = 0; i < MIN_CROSSINGS; i++) {
//        Serial.print(timestamps[i]);
//        Serial.print(" ");
//      }
//      Serial.println("crossed");
      if (timestamp < TIME_THRESHOLD_MIN && timestamp_pos != 0) {
        reset();
      }
      if (timestamp < TIME_THRESHOLD_MAX && timestamp > TIME_THRESHOLD_MIN) {
//        Serial.println(timestamp);
        timestamps[timestamp_pos] = timestamp;
        timestamp = 0;
        timestamp_pos++;
      }
      if (timestamp_pos == MIN_CROSSINGS) {

        impSerial.print("{\"id\": ");
        impSerial.print(UUID);
        impSerial.print(", \"values\":[");
////        impSerial.println();
        for (int i = 0; i< MIN_CROSSINGS; i++) {
          impSerial.print(timestamps[i]);
          if (i < MIN_CROSSINGS - 1) {
            impSerial.print(", ");
          }
        }
        impSerial.println("]}");
        Serial.println("Handshake");
        reset();
      }
    } else {
      reset();
    }
  }
  
  // if its gone over the max time threshold reset everything
  if (timestamp > TIME_THRESHOLD_MAX) {
    reset();
  }
  if (timestamp_pos != 0) {
    timestamp++; // make sure this isn't overflowing
  }
  prev = current;
  
  delay(DELAY_TIME);
}
  
