/*
  ===========================================================
   SMART CAR PARKING SYSTEM - Arduino UNO
  ===========================================================
  Features:
  - Entry Gate  : Ultrasonic Sensor + Servo Barrier
      -> Triggers immediately upon vehicle detection and displays "Welcome".
      -> Remains open for a set duration, then slowly closes and increments parked count by +1.
  - Exit Gate   : Ultrasonic Sensor (No Servo Gate)
      -> Displays "Thanks, Visit Again" on LCD while vehicle is detected.
      -> Once vehicle moves away, decrements parked count by -1, increments total exited by +1, and updates status display.
  - 16x2 LCD    : Displays real-time status (Parked / Free / Total Exited).
  - Total Slots : 4 slots (Configurable via TOTAL_SLOTS variable).

  ---------------------- WIRING ----------------------------
  Servo Motor:
    GND -> Arduino GND     VCC -> Arduino 5V     Signal -> D3

  Ultrasonic Sensor 1 (Entry Gate):
    VCC -> 5V   GND -> GND   Trig -> D8   Echo -> D9

  Ultrasonic Sensor 2 (Exit Gate):
    VCC -> 5V   GND -> GND   Trig -> D10   Echo -> D11

  16x2 LCD (Normal 16-pin interface, without I2C):
    VSS(1)->GND   VDD(2)->5V   V0(3)->Potentiometer wiper
    RS(4)->D2     RW(5)->GND   E(6)->D4
    D4(11)->D5    D5(12)->D6   D6(13)->D7   D7(14)->D12
    A(15)->5V     K(16)->GND
  ===========================================================
*/

#include <LiquidCrystal.h>
#include <Servo.h>

// ---------------- Pin Configuration ----------------
const int ENTRY_TRIG = 8;
const int ENTRY_ECHO = 9;

const int EXIT_TRIG = 10;
const int EXIT_ECHO = 11;

const int SERVO_PIN = 3;

// LCD Pins: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(2, 4, 5, 6, 7, 12);
Servo gateServo;

// Function prototype
void closeGateSlowly();

// ---------------- Parking Configuration ----------------
const int TOTAL_SLOTS = 4;
int carsParked  = 0;
int totalExited = 0;

const int GATE_CLOSED = 0;
const int GATE_OPEN   = 90;
const int HOLD_TIME   = 2500;    // Gate open duration (ms)

// Distance threshold for vehicle detection (in cm)
const int DETECTION_DISTANCE = 10;

// ---------------- Presence State ----------------
// true = vehicle detected in front of sensor
bool entryPresent = false;
bool exitPresent  = false;

// Debounce filtering to prevent false triggers from noise
const int CONFIRM_COUNT = 3;
int entryDetectCounter = 0, entryClearCounter = 0;
int exitDetectCounter  = 0, exitClearCounter  = 0;

void setup() {
  pinMode(ENTRY_TRIG, OUTPUT);
  pinMode(ENTRY_ECHO, INPUT);
  pinMode(EXIT_TRIG, OUTPUT);
  pinMode(EXIT_ECHO, INPUT);

  gateServo.attach(SERVO_PIN);
  gateServo.write(GATE_CLOSED);

  lcd.begin(16, 2);
  lcd.print("Smart Parking");
  lcd.setCursor(0, 1);
  lcd.print("System Ready..");
  delay(2000);

  updateStatusDisplay();
}

void loop() {
  long entryDistance = getDistance(ENTRY_TRIG, ENTRY_ECHO);
  long exitDistance  = getDistance(EXIT_TRIG, EXIT_ECHO);

  bool entryDetectedNow = (entryDistance > 0 && entryDistance <= DETECTION_DISTANCE);
  bool exitDetectedNow  = (exitDistance  > 0 && exitDistance  <= DETECTION_DISTANCE);

  handleEntrySensor(entryDetectedNow);
  handleExitSensor(exitDetectedNow);

  delay(100);   // Sensor polling interval
}

// ---------------- Entry Gate Logic ----------------
void handleEntrySensor(bool detectedNow) {
  if (detectedNow) {
    entryDetectCounter++;
    entryClearCounter = 0;
  } else {
    entryClearCounter++;
    entryDetectCounter = 0;
  }

  // Vehicle newly detected -> trigger gate action immediately
  if (!entryPresent && entryDetectCounter >= CONFIRM_COUNT) {
    entryPresent = true;   // Prevent multiple triggers for the same vehicle
    entryDetectCounter = 0;

    if (carsParked < TOTAL_SLOTS) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Welcome!");
      lcd.setCursor(0, 1);
      lcd.print("Gate Opening..");

      gateServo.write(GATE_OPEN);
      delay(HOLD_TIME);           // Hold gate open for defined time
      closeGateSlowly();          // Smoothly close gate
      carsParked++;
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Sorry!");
      lcd.setCursor(0, 1);
      lcd.print("Parking Full");
      delay(1500);
    }

    updateStatusDisplay();
  }

  // Vehicle cleared -> reset flag for next car
  if (entryPresent && entryClearCounter >= CONFIRM_COUNT) {
    entryPresent = false;
    entryClearCounter = 0;
  }
}

// ---------------- Exit Gate Logic ----------------
void handleExitSensor(bool detectedNow) {
  if (detectedNow) {
    exitDetectCounter++;
    exitClearCounter = 0;
  } else {
    exitClearCounter++;
    exitDetectCounter = 0;
  }

  // Vehicle newly detected at exit -> display thank you message
  if (!exitPresent && exitDetectCounter >= CONFIRM_COUNT) {
    exitPresent = true;
    exitDetectCounter = 0;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Thanks!");
    lcd.setCursor(0, 1);
    lcd.print("Visit Again");
  }

  // Vehicle moved away -> update count and return to normal display
  if (exitPresent && exitClearCounter >= CONFIRM_COUNT) {
    exitPresent = false;
    exitClearCounter = 0;

    if (carsParked > 0) {
      carsParked--;
      totalExited++;
    }

    updateStatusDisplay();
  }
}

// ---------------- Slow Gate Close Function ----------------
void closeGateSlowly() {
  // Smooth transition from OPEN to CLOSED angle
  for (int pos = GATE_OPEN; pos >= GATE_CLOSED; pos--) {
    gateServo.write(pos);
    delay(30);   // Lower delay value = faster closing speed
  }
}

// ---------------- Ultrasonic Distance Function ----------------
long getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 20000);  // 20ms timeout
  if (duration == 0) return -1;

  long distanceCm = duration * 0.034 / 2;
  return distanceCm;
}

// ---------------- Status Display ----------------
void updateStatusDisplay() {
  int slotsFree = TOTAL_SLOTS - carsParked;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Parked:");
  lcd.print(carsParked);
  lcd.print(" Free:");
  lcd.print(slotsFree);

  lcd.setCursor(0, 1);
  lcd.print("Total Exited:");
  lcd.print(totalExited);
}