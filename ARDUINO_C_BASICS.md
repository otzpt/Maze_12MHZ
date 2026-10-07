# Arduino C basics

Arduino code is C/C++ with a small library on top. A program is called a *sketch*.
Every sketch has two functions:

```cpp
void setup() {   // runs once, at power-on or reset
}

void loop() {    // runs forever, over and over
}
```

## 1. First program: blink the LED

```cpp
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);   // pin 13 on an Uno
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  // on
  delay(500);                       // wait 500 ms
  digitalWrite(LED_BUILTIN, LOW);   // off
  delay(500);
}
```

Statements end with `;`. Blocks use `{ }`. Comments: `// line` or `/* block */`.

## 2. Variables and types

| Type            | Holds                         | Example                     |
|-----------------|-------------------------------|-----------------------------|
| `int`           | whole number (-32768..32767 on Uno) | `int speed = 200;`    |
| `long`          | bigger whole number           | `long total = 100000;`      |
| `unsigned long` | big, never negative (use for `millis()`) | `unsigned long t;` |
| `float`         | decimal number                | `float cm = 12.5;`          |
| `bool`          | `true` / `false`              | `bool wall = false;`        |
| `byte`          | 0..255                        | `byte pwm = 128;`           |
| `char`          | one character                 | `char c = 'A';`             |

Constants (preferred for pin numbers, they use no RAM):

```cpp
const int LEFT_MOTOR_PIN = 5;
#define TRIG_PIN 9          // also works, no semicolon
```

## 3. Pins

```cpp
pinMode(pin, OUTPUT);        // or INPUT, INPUT_PULLUP
digitalWrite(pin, HIGH);     // HIGH = 5 V, LOW = 0 V
int v = digitalRead(pin);    // HIGH or LOW
int a = analogRead(A0);      // 0..1023 (0..5 V), analog pins only
analogWrite(pin, 128);       // PWM 0..255, only on ~ pins (3,5,6,9,10,11 on Uno)
```

`INPUT_PULLUP` turns on an internal resistor, so a button wired between the pin and GND
reads `HIGH` when released and `LOW` when pressed. No external resistor needed.

## 4. Operators

```cpp
+  -  *  /  %          // math, % is remainder
==  !=  <  >  <=  >=   // comparison (== is compare, = is assign!)
&&  ||  !              // and, or, not
x++;  x--;  x += 5;    // shortcuts
```

Integer division drops the decimal: `7 / 2` is `3`. Use `7 / 2.0` to get `3.5`.

## 5. Decisions and loops

```cpp
if (distance < 10) {
  stopMotors();
} else if (distance < 30) {
  turn();
} else {
  forward();
}

for (int i = 0; i < 5; i++) {   // runs 5 times
  blink();
}

while (digitalRead(BUTTON) == HIGH) {   // wait until pressed
}
```

## 6. Functions

Break the code into named pieces. Return type, name, parameters:

```cpp
void forward(int speed) {
  analogWrite(LEFT_MOTOR_PIN, speed);
  analogWrite(RIGHT_MOTOR_PIN, speed);
}

int add(int a, int b) {
  return a + b;
}
```

`void` means "returns nothing".

## 7. Serial Monitor (your debugger)

```cpp
void setup() {
  Serial.begin(9600);          // set Serial Monitor to 9600 baud
}

void loop() {
  Serial.print("distance: ");
  Serial.println(42);          // println adds a newline
  delay(200);
}
```

When something does not work, print the values. This solves most problems.

## 8. Time without freezing: `millis()`

`delay()` stops everything. For a robot that must keep reading sensors, use `millis()`
(milliseconds since power-on):

```cpp
unsigned long last = 0;

void loop() {
  if (millis() - last >= 500) {   // every 500 ms
    last = millis();
    // do the periodic thing here
  }
  // the rest of the code keeps running
}
```

## 9. Arrays

```cpp
int sensors[3] = {A0, A1, A2};
for (int i = 0; i < 3; i++) {
  Serial.println(analogRead(sensors[i]));
}
```

Indexes start at 0. Going past the end corrupts memory silently.

## 10. Ultrasonic sensor (HC-SR04), common for mazes

```cpp
const int TRIG = 9;
const int ECHO = 10;

void setup() {
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
}

float readCm() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long us = pulseIn(ECHO, HIGH, 30000);   // 30 ms timeout
  return us * 0.0343 / 2;                 // 0 means no echo
}

void loop() {
  Serial.println(readCm());
  delay(100);
}
```

## 11. Wall-avoiding robot skeleton

```cpp
// Motor driver pins (e.g. L298N). Adjust to your wiring.
const int L_FWD = 5, L_BACK = 6, R_FWD = 3, R_BACK = 11;

void drive(int l, int r) {          // -255..255 each side
  analogWrite(L_FWD,  l > 0 ?  l : 0);
  analogWrite(L_BACK, l < 0 ? -l : 0);
  analogWrite(R_FWD,  r > 0 ?  r : 0);
  analogWrite(R_BACK, r < 0 ? -r : 0);
}

void loop() {
  float d = readCm();
  if (d > 0 && d < 15) {
    drive(-150, 150);   // wall ahead: spin in place
    delay(300);
  } else {
    drive(180, 180);    // clear: go straight
  }
}
```

Motors never connect directly to Arduino pins. Always use a driver board and a
separate battery, with the grounds joined.

## Common mistakes

- `=` instead of `==` inside `if`.
- Missing `;` or an unclosed `{`. The compiler error points near, not at, the problem.
- Using `int` for `millis()`. Use `unsigned long`.
- `analogWrite` on a non-PWM pin does nothing useful.
- Forgetting `pinMode`.
- Serial baud rate in the monitor not matching `Serial.begin`.
- No shared GND between Arduino and motor battery.

## Where to go next

- Built-in examples: Arduino IDE, File > Examples (Blink, Button, AnalogReadSerial, Ping).
- Reference for every function: https://docs.arduino.cc/language-reference/
- Not verified: pin numbers above assume an Arduino Uno. Check them against your board.
