//Sorting and Pickup

#include <ESP32Servo.h>
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <MSE2202_Lib.h>
// Uncomment keywords to enable debugging output
//#define DEBUG_DRIVE_SPEED    1
#define DEBUG_ENCODER_COUNT 1

// Function declarations
void Indicator();  // for mode/heartbeat on Smart LED

// Port pin constants
#define LEFT_MOTOR_A 35    // GPIO35 pin 28 (J35) Motor 1 A
#define LEFT_MOTOR_B 36    // GPIO36 pin 29 (J36) Motor 1 B
#define ENCODER_LEFT_A 15  // left encoder A signal is connected to pin 8 GPIO15 (J15)
#define ENCODER_LEFT_B 16  // left encoder B signal is connected to pin 8 GPIO16 (J16)
#define POT_R1 1           // when DIP Switch S1-3 is on, Analog AD0 (pin 39) GPIO1 is connected to Poteniometer R1


// Constants
const int cPWMRes = 8;                    // bit resolution for PWM
const int cMinPWM = 150;                  // PWM value for minimum speed that turns motor
const int cMaxPWM = pow(2, cPWMRes) - 1;  // PWM value for maximum speed

// Variables
unsigned char leftDriveSpeed;  // motor drive speed (0-255)
// Motor, encoder, and IR objects (classes defined in MSE2202_Lib)
Motion Bot = Motion();              // Instance of Motion for motor control
Encoders LeftEncoder = Encoders();  // Instance of Encoders for left encoder data

Servo myServo;  // creating a Servo object
int potpin = 0;
int val = 0;
int GreenMark = 0;
int servo = 0;
// Define color sensor pins

#define S0 40  //This are the pins I have defined
#define S1 41
#define S2 42
#define S3 44
#define sensorOut 43

// Calibration Values
// Get these from Calibration Sketch

int redMin = 164;    // Red minimum value
int redMax = 455;    // Red maximum value
int greenMin = 169;  // Green minimum value
int greenMax = 456;  // Green maximum value
int blueMin = 167;   // Blue minimum value
int blueMax = 448;   // Blue maximum value

// Variables for Color Pulse Width Measurements

int redPW = 0;
int greenPW = 0;
int bluePW = 0;

// Variables for final Color values

int redValue;
int greenValue;
int blueValue;

void setup() {

#if defined DEBUG_DRIVE_SPEED || DEBUG_ENCODER_COUNT
  Serial.begin(115200);
#endif

  // Set up motors and encoders
  Bot.driveBegin("D1", LEFT_MOTOR_A, LEFT_MOTOR_B, 37, 38);                   // set up motors as Drive 1
  LeftEncoder.Begin(ENCODER_LEFT_A, ENCODER_LEFT_B, &Bot.iLeftMotorRunning);  // set up left encoder
}
// Set S0 - S3 as outputs
pinMode(S0, OUTPUT);
pinMode(S1, OUTPUT);
pinMode(S2, OUTPUT);
pinMode(S3, OUTPUT);
pinMode(45, OUTPUT);
myServo.attach(39);

// Set Sensor output as input
pinMode(sensorOut, INPUT);

// Set Frequency scaling to 20%
digitalWrite(S0, HIGH);
digitalWrite(S1, LOW);

// Setup Serial Monitor
Serial.begin(9600);
}

void loop() {
  long pos[] = { 0, 0 };  // current motor positions
  int pot = 0;
  //  LeftEncoder.getEncoderRawCount();                            // read left encoder count
  Serial.print("Left Encoder count = ");
  Serial.print(LeftEncoder.lRawEncoderCount);
  pot = analogRead(POT_R1);  // Read pot to update drive motor speed
  leftDriveSpeed = map(pot, 0, 4095, cMinPWM, cMaxPWM);
  Bot.Forward("D1", leftDriveSpeed, 0);

  digitalWrite(45, LOW);
  delay(2000);
  digitalWrite(45, HIGH);
  // Read Red value
  redPW = getRedPW();
  // Map to value from 0-255
  redValue = map(redPW, redMin, redMax, 255, 0);
  // Delay to stabilize sensor
  delay(200);

  // Read Green value
  greenPW = getGreenPW();
  // Map to value from 0-255
  greenValue = map(greenPW, greenMin, greenMax, 255, 0);
  // Delay to stabilize sensor
  delay(200);

  // Read Blue value
  bluePW = getBluePW();
  // Map to value from 0-255
  blueValue = map(bluePW, blueMin, blueMax, 255, 0);
  // Delay to stabilize sensor
  delay(200);

  // Print output to Serial Monitor
  Serial.print("Red = ");
  Serial.print(redValue);
  Serial.print(" - Green = ");
  Serial.print(greenValue);
  Serial.print(" - Blue = ");
  Serial.println(blueValue);

  if (servo == 0) {
    myServo.write(90);
    servo++;
  }  // this is a line of code that puts the platform horizontal (angle = 0). Change accordingly
  for (int accuracy = 0; accuracy < 5; accuracy++) {
    if ((greenValue > redValue + 15) && (greenValue > blueValue + 15)) {
      myServo.write(30);
      delay(150);  //change this into the millis(), function.
      myServo.write(90);
    }
  }
  if ((greenValue < redValue) || (greenValue < blueValue)) {
    myServo.write(150);
    delay(150);
    myServo.write(90);
  }
}
// Function to read Red Pulse Widths
int getRedPW() {

  // Set sensor to read Red only
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;
}

// Function to read Green Pulse Widths
int getGreenPW() {

  // Set sensor to read Green only
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;
}

// Function to read Blue Pulse Widths
int getBluePW() {

  // Set sensor to read Blue only
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;
}