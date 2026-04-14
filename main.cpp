#include <LiquidCrystal.h> // includes LCD library
#include <Keypad.h> // includes keypad library (separate install)
#include <Servo.h> // include servo library

unsigned long lastTick = 0;

// Servo
Servo servo_01; // create servo object
int servo_01_pin = 6; // servo 1 pin - MUST BE PWM PIN
int servo_01_current_pos = 0; // variable to store current position of servo 01
int servo_01_start_pos = 0; // variable to store starting position of servo 01
int servo_01_dispense_pos = 180; // variable to store dispensing position of servo 01

// LCD pins
const int rs = 13, en = 12, d4 = A0, d5 = A1, d6 = A2, d7 = A3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Keypad setup
const byte ROWS = 4;
const byte COLS = 4;
const char enter = '#';
const char clear = '*';

// Keypad pins (first 4 L to R on keypad are rows, then cols)
byte rowPins[ROWS] = {A4, A5, 2, 3};
byte colPins[COLS] = {4, 5, 7, 8};

// Array of keys
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Create Keypad Object
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// Function Declarations
void showCounters();
int getInterval();

// variable to store time until dispense
int counter_01 = getInterval();

// variable to store keypress
char key;

void setup() {
  servo_01.attach(servo_01_pin); // connects servo_01 to servo_01_pin
  servo_01.write(0); // Moves servo to 0 degree position

  lcd.begin(16, 2); // 16 columns, 2 rows on LCD screen
  lcd.print("Making Progress");

  // Setup Serial Monitor for debugging (9600 baud rate)
  Serial.begin(9600);

}

void loop() {

  showCounters();

  if (millis() - lastTick >= 1000) {
    lastTick = millis();
    if (counter_01 >= 0) {
      counter_01--;
      showCounters();
    }
  }

  if (key)
    counter_01 = getInterval();
  else if (counter_01 == 0) {
    servo_01.write(servo_01_dispense_pos);
    delay(1000);
    servo_01.write(servo_01_start_pos);
  }
  else
    showCounters();



  //  lcd.setCursor(0, 1); // set cursor to col 0, row 1
  //  lcd.print(millis() / 1000);

  // char customKey = customKeypad.getKey();
  // if (customKey) {
  //   // lcd.clear(); // clears the lcd
  //   lcd.setCursor(0, 1);
  //   lcd.print(customKey);
  // }

  // Testing Servo Rotation
  // for (servo_01_pos = 0; servo_01_pos <= 180; servo_01_pos += 1) {
  //   servo_01.write(servo_01_pos);
  //   delay(15);
  // }
  // for (servo_01_pos = 180; servo_01_pos >= 0; servo_01_pos -= 1) {
  //   servo_01.write(servo_01_pos);
  //   delay(15);
  // }

  // // Debugging keypad
  // char customKey = customKeypad.getKey();
  // if (customKey) {
  //   Serial.println(customKey);
  // }

}

// Show Counters Function
void showCounters() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Counter:");
  lcd.setCursor(0, 1);
  lcd.print(counter_01);
}

// Interval Entry Function
int getInterval() {
  lcd.clear();

  char key = customKeypad.getKey();
  String input = "";

  while (true) {
    lcd.setCursor(0, 0);
    lcd.print("Interval in sec:");
    if (key) {
      if (key == enter) { // confirm string
        return input.toInt(); // what happens if user confirms without entering a number?
      }
      else if (key == clear) { // clear string
        input = "";
        lcd.clear();
      }
      else if (key == 'A' || key == 'B' || key == 'C' || key == 'D') {
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("Enter a number");
      }
      else { // add to string
        input += key;
        lcd.setCursor(0, 1);
        lcd.print(input);
      }
    }
  }
}