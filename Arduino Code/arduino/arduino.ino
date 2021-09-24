// Whether or not to show helpful print statements. Should be false for runs.
const bool DEBUG = false;

// Whether or not the motors can spin.
bool isEStopped = false;

// Include other Files
#include "Motors.hpp"
#include "Fans.hpp"
#include "LED.hpp"

void performCommand(char action, int value);
void toggleFans(char value);
void handleRCSignal();

// Motor pins
const int PIN_THROTTLE_LEFT = 2;
const int PIN_THROTTLE_RIGHT = 3;

//Relay pins
const int RELAY1 = 22;
const int RELAY2 = 23;

// Fans pin
const int PIN_FANS = 4;
// Limit for throttle power
double LIMIT = 0.25;

// Range of PWM signals one way.
const double RANGE = 400.0;

//Minimum Battery Voltage
const double MINIMUM_VOLTAGE = 16;

//Voltage Sensor Pin
const int VOLTAGE_SENSOR = A7;

const int CH1 = A1; // Right stick, horizontal
const int CH2 = A2; // Right stick, vertical
const int CH3 = A3; // Left stick,  vertical
const int CH4 = A4; // Left stick,  horizontal
const int CH5 = A5; // Top right switch, 2 positions
const int CH6 = A6; // Top left switch, 2 positions

Motors motors(PIN_THROTTLE_LEFT, PIN_THROTTLE_RIGHT);
Fans fans(PIN_FANS);
LED led_buildin(LED_BUILTIN);
// LED led_red(53);
// LED led_green(51);
// LED led_blue(49);
bool isManual = false;
bool isRecording = false;
bool noThrottle = true;

//Signal Tower Pins
enum LightState {LIGHT_GREEN, LIGHT_AMBER, LIGHT_RED};
LightState lightState = LIGHT_GREEN;
const int SIGNAL_GREEN = 5; //Green means autonomous
const int SIGNAL_AMBER = 6; //Amber means remote control
const int SIGNAL_RED = 7; //Red means E-stopped

// Initialize motors, set fans as on, set LED statuses
void setup() {
  // Begin Serial Comms
  Serial.begin(19200);
  Serial.setTimeout(10);
  if (DEBUG) Serial.println("##### Setting Up #####");
  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH3, INPUT);
  pinMode(CH4, INPUT);
  pinMode(CH5, INPUT);
  pinMode(CH6, INPUT);
  pinMode(VOLTAGE_SENSOR, INPUT);
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(SIGNAL_GREEN, OUTPUT);
  pinMode(SIGNAL_AMBER, OUTPUT);
  pinMode(SIGNAL_RED, OUTPUT);

  // Set initial states
  motors.init();
  fans.on();
  led_buildin.off();
  // led_red.on();
  // led_green.off();
  // led_blue.off();

  // Print Ready for Debugging
  if (DEBUG) Serial.println("##### Ready #####");

  // Sometimes the NUC doesn't get the first command, so send a dummy one that does nothing
  Serial.println("SETUP;");
}

// previous left and right pwms for running mean calc to smooth out the signal. Without this, it stutters and stops.
int l1 = 1500, l2 = 1500, l3 = 1500;
int r1 = 1500, r2 = 1500, r3 = 1500;
void loop() {  
  long tic = millis();
  isManual = pulseIn(CH6, HIGH) < 1400;
  isEStopped = pulseIn(CH5, HIGH) < 1400;
  noThrottle = pulseIn(CH3, HIGH) < 1050;
  Serial.println(pulseIn(CH3, HIGH));

  //stops the boat if measured battery voltage is less than MINIMUM_VOLTAGE
  if(analogRead(VOLTAGE_SENSOR) / 40.0 < MINIMUM_VOLTAGE){ //divide by 40.0 to get actual voltage from analog signal
    noThrottle = true;
    isEStopped = true;
  }
  
  //control relays
  if (!isEStopped){
    digitalWrite(RELAY1, HIGH);
    digitalWrite(RELAY2, HIGH);
  } else {
    digitalWrite(RELAY1, LOW);
    digitalWrite(RELAY2, LOW);
  }
  
  //handle signal tower
  //determine state
  if(isEStopped){
    //turn on red light
    lightState = LIGHT_RED;
  } else {
    if(isManual){
      //turn on amber light
      lightState = LIGHT_AMBER;
    } else {
      //turn on green light
      lightState = LIGHT_GREEN;
    }
  }
  //set pins
  digitalWrite(SIGNAL_GREEN, LOW);
  digitalWrite(SIGNAL_AMBER, LOW);
  digitalWrite(SIGNAL_RED, LOW);
  switch(lightState){
    case LIGHT_RED:
      digitalWrite(SIGNAL_RED, HIGH);
      break;
    case LIGHT_AMBER:
      digitalWrite(SIGNAL_AMBER, HIGH);
      break;
    case LIGHT_GREEN:
      digitalWrite(SIGNAL_GREEN, HIGH);
      break;
  }
  
  if (isManual) handleRecording();

  if (noThrottle) motors.setSpeed(1500, 1500);
  
  // handle Manual controls
  if (isManual && !isEStopped && !noThrottle) handleRCSignal();

  // Handle input commands
  if (Serial.available()) {
    String command = Serial.readStringUntil(';');
    if (DEBUG) {
      Serial.print("Command: ");
      Serial.println(command);
    }

    char action = command.charAt(0);
    String value = command.substring(1);
    performCommand(action, value);
  }
  long tok = millis() - tic;
  if (DEBUG) {
    Serial.print("• time: ");
    Serial.print(tok);
    Serial.println(" ms");
  }
}

// Performs code based on the action and value from the command
void performCommand(char action, String value) {
  if (action == 'm' ) {
    if (isManual) return;
    int l = (value.substring(0, 4).toInt() - 1500) * LIMIT + 1500;
    int r = (value.substring(4, 8).toInt() - 1500) * LIMIT + 1500;
    motors.setSpeed(l, r);
  } else if (action == 'f') {
    toggleFans(value.charAt(0));
  }
}

// Turn fans on and off
void toggleFans(char value) {
  Serial.println(value);
  if (value == '0') {
    fans.off();
  } else if (value == '1') {
    fans.on();
  }
}

// Handle signal and convert vector to 2 thrust values.
void handleRCSignal() {
  int ch1 = pulseIn(CH1, HIGH);
  int ch2 = pulseIn(CH2, HIGH);
  if (ch1 <= 1530 && ch1 >= 1470) ch1 = 1500;
  if (ch2 <= 1530 && ch2 >= 1470) ch2 = 1500;
  //  int ch3 = pulseIn(CH3, HIGH);
  //  int ch4 = pulseIn(CH4, HIGH);
  //  int ch5 = pulseIn(CH5, HIGH);
  if (ch1 == 1500 && ch2 == 1500) {
    int ch3 = pulseIn(CH3, HIGH);
    LIMIT = (ch3 / 100.0 - 11) / 9.0;
    if (LIMIT < 0.05) LIMIT = 0.05;
    else if (LIMIT > 1) LIMIT = 1.0;
  }
  
  int l;
  int r;
  if (ch2 == 1500) {
    // handle point turns
    double h = (ch1 - 1500) * LIMIT / RANGE;
    int offset = RANGE * h;
    l = 1500 + offset;
    r = 1500 - offset;
  } else {
    // handle non point turns
    double t = (ch2 - 1500) * LIMIT / RANGE;
    double d = abs(ch1 - 1500) * LIMIT / RANGE * (t < 0 ? -1 : 1);
    double min = t - d;
    double max = t;
    if (t > 0) {
      l = (ch1 < 1500 ? min : max) * RANGE + 1500;
      r = (ch1 < 1500 ? max : min) * RANGE + 1500;
    } else {
      r = (ch1 < 1500 ? max : min) * RANGE + 1500;
      l = (ch1 < 1500 ? min : max) * RANGE + 1500;
    }
  }
  l3 = l2;
  l2 = l1;
  l1 = l;
  r3 = r2;
  r2 = r1;
  r1 = r;
  // running mean calculated here.
  motors.setSpeed((l1 + l2 + l3) / 3, (r1 + r2 + r3) / 3);
}

// Start and stop the recording by sending a command to the NUC.
void handleRecording() {
  int ch4 = pulseIn(CH4, HIGH);
  if (ch4 < 1200 && !isRecording) { // 1100 is full left
    isRecording = true;
    if (DEBUG) Serial.print("R1;");
  } else if (ch4 > 1800 && isRecording) { // 1900 is full right
    isRecording = false;
    if (DEBUG) Serial.print("R0;");
  }
}
