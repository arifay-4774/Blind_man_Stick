// Blind Man Stick
// Uses Ultrasonic Sensor to detect obstacles
// Plays Morse code "ALRT" on buzzer if an obstacle is within the threshold distance

// Pin Definitions
const int trigPin = 9;   // Ultrasonic Sensor Trigger Pin
const int echoPin = 10;  // Ultrasonic Sensor Echo Pin
const int buzzerPin = 8; // Buzzer Pin

// Variables
const int distFar = 70;     // Max distance for 1 beep (far distance)
const int distMedium = 50;  // Max distance for 2 beeps (medium distance)
const int distNear = 30;    // Max distance for Morse ALRT (near distance)
const int dotDuration = 100;     // Duration of a Morse code "dot" in milliseconds
const int buzzerFreq = 1000;       // Frequency of the buzzer tone in Hz

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600); // For debugging
}

void loop() {
  long duration, distance;
  
  // Clear the trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Send a 10 microsecond pulse to trigger the sensor
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Read the echo pin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance in cm
  // Speed of sound is 343 m/s or 0.0343 cm/microsecond
  // Distance = (Time x Speed) / 2
  distance = (duration * 0.0343) / 2;
  
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  // Check distance and play corresponding alert level
  if (distance > 0) {
    if (distance <= distNear) {
      // Too near -> Morse code ALRT
      playALRT();
      delay(dotDuration * 7); // Pause (word gap) before repeating
    } else if (distance <= distMedium) {
      // Just near -> 2 beeps
      playTwoBeeps();
      delay(500); // Pause before repeating
    } else if (distance <= distFar) {
      // Too long (Far) -> 1 beep
      playOneBeep();
      delay(1000); // Pause before repeating
    }
  }
  
  delay(100); // Short delay before next measurement
}

// Custom function for 1 beep (far object)
void playOneBeep() {
  tone(buzzerPin, buzzerFreq);
  delay(200);
  noTone(buzzerPin);
}

// Custom function for 2 beeps (medium object)
void playTwoBeeps() {
  tone(buzzerPin, buzzerFreq);
  delay(150);
  noTone(buzzerPin);
  delay(150);
  tone(buzzerPin, buzzerFreq);
  delay(150);
  noTone(buzzerPin);
}

// Custom function to play a "dot" (.)
void playDot() {
  tone(buzzerPin, buzzerFreq);
  delay(dotDuration);
  noTone(buzzerPin);
  delay(dotDuration); // Gap between parts of the same letter
}

// Custom function to play a "dash" (-)
void playDash() {
  tone(buzzerPin, buzzerFreq);
  delay(dotDuration * 3); // Dash is 3 times the length of a dot
  noTone(buzzerPin);
  delay(dotDuration); // Gap between parts of the same letter
}

// Custom function for the gap between letters
void letterSpace() {
  // We already wait 1 dot duration at the end of each dot/dash
  // Standard letter space is 3 dot durations, so we wait 2 more
  delay(dotDuration * 2);
}

void playA() {
  // A -> .-
  playDot();
  playDash();
  letterSpace();
}

void playL() {
  // L -> .-..
  playDot();
  playDash();
  playDot();
  playDot();
  letterSpace();
}

void playR() {
  // R -> .-.
  playDot();
  playDash();
  playDot();
  letterSpace();
}

void playT() {
  // T -> -
  playDash();
  letterSpace();
}

void playALRT() {
  // Play the full sequence
  playA();
  playL();
  playR();
  playT();
}
