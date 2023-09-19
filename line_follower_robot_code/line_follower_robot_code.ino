//pins
#define motor_left_forward_pin 3
#define motor_left_backward_pin 5
#define motor_right_forward_pin 10
#define motor_right_backward_pin 9
#define lineReader_left_pin 2
#define lineReader_right_pin 4
#define enable_1_pin 6
#define enable_2_pin 11

//other
#define blackThreshold 2000
#define maxSpeed 215 //0-255
#define turnSpeed 195 //0-255

void setup(){
  Serial.begin (9600);
  pinMode(motor_left_forward_pin, OUTPUT);
  pinMode(motor_left_backward_pin, OUTPUT);
  pinMode(motor_right_forward_pin, OUTPUT);
  pinMode(motor_right_backward_pin, OUTPUT);
  pinMode(enable_1_pin, OUTPUT);
  pinMode(enable_2_pin, OUTPUT);
  delay(100);
}

void loop(){
  int leftReaderValue = readLineReader(lineReader_left_pin);
  int rightReaderValue = readLineReader(lineReader_right_pin);

  drive(leftReaderValue > blackThreshold, rightReaderValue > blackThreshold);
  delay(10);
}

//returns int in range ~0 (low reflection time, light surface) and ~maxReflectionTime (high reflection time, dark surface)
int readLineReader(int pinToRead){
  int maxReflectionTime = 3000;
  pinMode(pinToRead, OUTPUT);
  digitalWrite(pinToRead, HIGH);
  delayMicroseconds(10);
  pinMode(pinToRead, INPUT);
  
  long startTime = micros();
  
  while (digitalRead(pinToRead) == HIGH && micros() - startTime < maxReflectionTime);
  int diff = micros() - startTime;

  return diff;
}

void drive(boolean reverseLeftMotor, boolean reverseRightMotor){
  //drive(true, true) stops both motors instead of reversing both
  boolean stop = reverseLeftMotor && reverseRightMotor;
  digitalWrite(motor_left_forward_pin, !reverseLeftMotor * !stop);
  digitalWrite(motor_left_backward_pin, reverseLeftMotor * !stop);
  digitalWrite(motor_right_forward_pin, !reverseRightMotor * !stop);
  digitalWrite(motor_right_backward_pin, reverseRightMotor * !stop);

  if (stop)
    return;

  int currentSpeed = reverseLeftMotor == reverseRightMotor ? maxSpeed : turnSpeed;
  analogWrite(enable_1_pin, currentSpeed);
  analogWrite(enable_2_pin, currentSpeed);
}
