//SECOND DRIVE SYSTEM
// Uncomment keywords to enable debugging output
//#define DEBUG_DRIVE_SPEED    1
#define DEBUG_ENCODER_COUNT 1

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <MSE2202_Lib.h>
// Function declarations
void Indicator();  // for mode/heartbeat on Smart LED

// Port pin constants
#define LEFT_MOTOR_A 35        // GPIO35 pin 28 (J35) Motor 1 A
#define LEFT_MOTOR_B 36        // GPIO36 pin 29 (J36) Motor 1 B
#define ENCODER_LEFT_A 15      // left encoder A signal is connected to pin 8 GPIO15 (J15)
#define ENCODER_LEFT_B 16      // left encoder B signal is connected to pin 8 GPIO16 (J16)
#define POT_R1 1               // when DIP Switch S1-3 is on, Analog AD0 (pin 39) GPIO1 is connected to Poteniometer R1


// Constants
const int cPWMRes = 8;                    // bit resolution for PWM
const int cMinPWM = 150;                  // PWM value for minimum speed that turns motor
const int cMaxPWM = pow(2, cPWMRes) - 1;  // PWM value for maximum speed

// Variables
unsigned char leftDriveSpeed;         // motor drive speed (0-255)
// Motor, encoder, and IR objects (classes defined in MSE2202_Lib)
Motion Bot = Motion();                // Instance of Motion for motor control
Encoders LeftEncoder = Encoders();    // Instance of Encoders for left encoder data

void setup() {

#if defined DEBUG_DRIVE_SPEED || DEBUG_ENCODER_COUNT
  Serial.begin(115200);
#endif

  // Set up motors and encoders
  Bot.driveBegin("D1", LEFT_MOTOR_A, LEFT_MOTOR_B,37,38);  // set up motors as Drive 1
  LeftEncoder.Begin(ENCODER_LEFT_A, ENCODER_LEFT_B, &Bot.iLeftMotorRunning);       // set up left encoder
}

void loop() {

  long pos[] = { 0, 0 };  // current motor positions
  int pot = 0;
  //  LeftEncoder.getEncoderRawCount();                            // read left encoder count
  Serial.print("Left Encoder count = ");
  Serial.print(LeftEncoder.lRawEncoderCount);
  pot = analogRead(POT_R1);  // Read pot to update drive motor speed
  leftDriveSpeed = map(pot, 0, 4095, cMinPWM, cMaxPWM);
  Bot.Forward("D1", leftDriveSpeed,0);

}
