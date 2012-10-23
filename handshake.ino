/*
  Handshake matching
 */

const int WINDOW_SIZE = 1000;
//const int MIN_WINDOW_SIZE = 50;
const int SLOPE_WINDOW_SIZE = 50; // we don't allow more than 1 slope change to happen during this duration
const int SLOPE_NUMBER = 4;
const int AVG_SIZE = 5; // average the first 5 values for a baseline
const int DELAY_TIME = 1; 
const int THRESHOLD = 1; // accelerometer values must be greater than this

int slopes[SLOPE_NUMBER]; // slope derivative
//int slope_dir[SLOPE_NUMBER]; // slope directon. 1 for going up, -1 for going down
long slopeTimeDiff[SLOPE_NUMBER]; // difference in time between hitting the slopes
int currentSlopePos = 0;
//int currentVals[WINDOW_SIZE];
int currentWindowPos = 0;
long timeDiff = 0;
boolean shakeDirection = true; // look for the going up slope first
boolean goingUp = false;
boolean startHandshake = false;
int prev = 0;
int current = 0;
int avg = 0;
int avgPos = 0;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  while (avgPos < AVG_SIZE) {
    avgPos++;
    avg += analogRead(A0);
    current = prev = analogRead(A0);
    delay(DELAY_TIME);
  }
  avg = avg/AVG_SIZE;
}

boolean crossedAvg() {
  return ((prev < avg && current > avg) || (prev > avg && current < avg));
}

// Return true if the direction changed.
boolean changeInDirection() {
  // goingUp : True if current value is going up,
  //           False if current value is going down
  return (current - prev) > 0;
}

//int calculateRunningAvg() {
//  int tempAvg = 0;
//  for (int i = 0; i < WINDOW_SIZE; i++) {
//    tempAvg += currentVals[i];
//  }
//  int avg = tempAvg/(sizeof(currentVals)/sizeof(int));
//  //Serial.println(avg);
//  return avg;
//}

boolean checkForHandshake() {
  // check for change in direction. make sure the change doesn't happen too quickly
  if (crossedAvg() && (shakeDirection == changeInDirection())
    && (timeDiff > SLOPE_WINDOW_SIZE) && (abs(current-prev) >= THRESHOLD)
    ) {
    
    // get the slope of the change in direction
    // check to make sure the slope is alternating
    // save it
    
//    Serial.print("Check ");
//    Serial.print(shakeDirection);
//    Serial.print(" ");
//    Serial.print(timeDiff);
//    Serial.print(" ");
//    Serial.print(avg);
//    Serial.print(" ");
//    Serial.print(prev);
//    Serial.print(" ");
//    Serial.println(current);
    
    slopes[currentSlopePos] = current - prev;
    slopeTimeDiff[currentSlopePos] = timeDiff; 
    timeDiff = 0;
    shakeDirection = !shakeDirection;
    
//    Serial.println(slopes[currentSlopePos]);
    currentSlopePos++;
    
    return true;
  }
  return false;
}

void resetWindow() {
//  Serial.println("reset");
  startHandshake = false;
  currentWindowPos = 0;
  for (int i = 0; i < SLOPE_NUMBER; i++) {
    slopes[currentSlopePos] = 0; 
    slopeTimeDiff[currentSlopePos] = 0; 
  }
  currentSlopePos = 0;
  timeDiff = 0;
}

// the loop routine runs over and over again forever:
void loop() {
  current = analogRead(A0);
//  Serial.print(current);
//  Serial.print(" ");
//  Serial.println(prev);
  // check for first shake
  if (checkForHandshake() && currentSlopePos == 1) {
    // if it is, have window start
    currentWindowPos = 0;
    startHandshake = true;
  } else if (startHandshake){
//    Serial.print("window pos");
//    Serial.println(currentWindowPos);
    currentWindowPos++;
  }
//  Serial.println(currentWindowPos);
  // check array of changed slopes
  // if we have a certain amount of alternating slopes
  if (currentSlopePos >= SLOPE_NUMBER) {
    // we have  a handshake
//    for(int i = 0; i < SLOPE_NUMBER; i++) {
//      Serial.print(slopes[i]);
//      Serial.print(" ");
//      Serial.print(slopeTimeDiff[i]);
//      Serial.print(" ");
//    }
//    Serial.println(currentWindowPos);
    Serial.println("Handshake");
    resetWindow();
  }
  
    
  // check that we haven't passed by the window
  // if we have, reset everything
  if (currentWindowPos >= WINDOW_SIZE) {
    resetWindow();
  }
  
  timeDiff++; // make sure this isn't overflowing
  prev = current;
  
  delay(DELAY_TIME);
}

// advance window by 1
//  currentVals[currentWindowPos] = current;
//  if (currentWindowPos == (WINDOW_SIZE-1)){
//    currentWindowPos = 0;
//  } else {
//    currentWindowPos++;
//  }
//  
//  // calculate running average
//  int avg = calculateRunningAvg();
  
