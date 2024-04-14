#include <Servo.h>

// Define pins and variables which are adjustable.
#define GUN_HORIZONTAL_PIN 13
#define GUN_VERTICAL_PIN 12
#define GUN_SHOOT_PIN 11
#define JOYSTICK_VRX_PIN A4
#define JOYSTICK_VRY_PIN A5
#define JOYSTICK_SW_PIN 4
#define JOYSTICK_TOLERANCE 50

// Create servo objects for the servomotors which are in charge of the horizontal and vertical movement of the gun.
Servo GunHorizontalServo;
Servo GunVerticalServo;

int isShooting = 0; // Variable for toggling between a idle and shooting state.
int prevJoystickSW = 0; // Variable for storing the previous joystick switch state.

void setup() {
  // Set pin modes for gun shoot pin, joystick VRX and VRY pins, and joystick switch pin.
  pinMode(GUN_SHOOT_PIN, OUTPUT);
  pinMode(JOYSTICK_VRX_PIN, INPUT);
  pinMode(JOYSTICK_VRY_PIN, INPUT);
  pinMode(JOYSTICK_SW_PIN, INPUT_PULLUP);

  // Make sure the gun is not shooting on start.
  digitalWrite(GUN_SHOOT_PIN, LOW);

  // Attach gun servomotors to their respective pins.
  GunHorizontalServo.attach(GUN_HORIZONTAL_PIN);
  GunVerticalServo.attach(GUN_VERTICAL_PIN);

  Serial.begin(9600);
}

void loop() {
  // Read values from the joystick.
  int joystickX = analogRead(JOYSTICK_VRX_PIN);
  int joystickY = analogRead(JOYSTICK_VRY_PIN);
  int joystickSW = digitalRead(JOYSTICK_SW_PIN);

  // Ensure joystick axis values stay within a stable range around center position.
  joystickX = constrain(joystickX < 500 - JOYSTICK_TOLERANCE || joystickX > 500 + JOYSTICK_TOLERANCE ? joystickX : 500, 0, 1000);
  joystickY = constrain(joystickY < 500 - JOYSTICK_TOLERANCE || joystickY > 500 + JOYSTICK_TOLERANCE ? joystickY : 500, 0, 1000);

  // Map the read joystick values into values which the gun servomotors accept.
  int gunRotationX = map(joystickX, 0, 1000, 0, 180);
  int gunRotationY = map(joystickY, 0, 1000, 0, 180);

  joystickSW = joystickSW == 0 ? 1 : 0; // Flip around the joystick switch input.
  if (joystickSW != prevJoystickSW && joystickSW == 1) isShooting = isShooting == 1 ? 0 : 1; // Change the isShooting variable so that it toggles.
  prevJoystickSW = joystickSW; // Save the previous joystick switch state.

  // Write the calculated rotation values to the gun servomotors.
  GunHorizontalServo.write(gunRotationX);
  GunVerticalServo.write(gunRotationY);

  digitalWrite(GUN_SHOOT_PIN, isShooting); // Shoot the gun if the isShooting variable is 1.

  delay(100); // Delay the program to stop the gun servomotors from shaking.
}