//This is the Main Code for the colour sensor + Servo Motor Pair!!

#include <Servo.h>
Servo myServo; // creating a Servo object
int potpin = 0; 
int val = 0;
int GreenMark = 0;  
int servo = 0; 
// Define color sensor pins

#define S0 40 //This are the pins I have defined
#define S1 41
#define S2 42
#define S3 44
#define sensorOut 43

// Calibration Values
// Get these from Calibration Sketch

int redMin = 33; // Red minimum value
int redMax = 170; // Red maximum value
int greenMin = 32; // Green minimum value
int greenMax = 164; // Green maximum value
int blueMin = 31;// Blue minimum value
int blueMax = 163; // Blue maximum value

// Variables for Color Pulse Width Measurements

int redPW = 0;
int greenPW = 0;
int bluePW = 0;

// Variables for final Color values

int redValue;
int greenValue;
int blueValue;

void setup() {

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
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  
  // Setup Serial Monitor
  Serial.begin(9600);
}

void loop() {
    digitalWrite(45,LOW); 
    delay(2000); 
    digitalWrite(45,HIGH); 
  // Read Red value
  redPW = getRedPW();
  // Map to value from 0-255
  redValue = map(redPW, redMin,redMax,255,0);
  // Delay to stabilize sensor
  delay(200);
  
  // Read Green value
  greenPW = getGreenPW();
  // Map to value from 0-255
  greenValue = map(greenPW, greenMin,greenMax,255,0);
  // Delay to stabilize sensor
  delay(200);
  
  // Read Blue value
  bluePW = getBluePW();
  // Map to value from 0-255
  blueValue = map(bluePW, blueMin,blueMax,255,0);
  // Delay to stabilize sensor
  delay(200);
  
  // Print output to Serial Monitor
  Serial.print("Red = ");
  Serial.print(redValue);
  Serial.print(" - Green = ");
  Serial.print(greenValue);
  Serial.print(" - Blue = ");
  Serial.println(blueValue);

  if (servo == 0){myServo.write(20);servo++ ;} // this is a line of code that puts the platform horizontal (angle = 0). Change accordingly 
  for(int accuracy = 0; accuracy < 5 ;accuracy++){
    if ((greenValue > redValue+15) && (greenValue > blueValue+15)){
    myServo.write(0); 
    delay(150);  //change this into the millis(), function. 
    myServo.write(20); 
   
    }
  }
  if((greenValue < redValue) || (greenValue < blueValue)){
    myServo.write(60); 
    delay(150); 
    myServo.write(20); 

  }

  


  

   

}
// Function to read Red Pulse Widths
int getRedPW() {

  // Set sensor to read Red only
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
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
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
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
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;

}