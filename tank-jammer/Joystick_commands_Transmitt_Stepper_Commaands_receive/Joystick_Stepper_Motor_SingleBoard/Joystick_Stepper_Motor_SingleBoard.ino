//Declare pin functions on Arduino
#define xAxis A0
#define yAxis A1
#define dir_pitch 2
#define stp_pitch 3
//#define EN_yaw 6
#define dir_yaw 4
#define stp_yaw 5
//#define EN_pitch 7
#define Fire 8
#define Fire_pulse 9
#define MicroStep 12

//Declare variables for functions
int xValue = 0;
int yValue = 0;
int x;
int dly = 500;
int steps = 20;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(stp_yaw, OUTPUT);
  pinMode(dir_yaw, OUTPUT);
  //pinMode(EN_yaw, OUTPUT);
  pinMode(stp_pitch, OUTPUT);
  pinMode(dir_pitch, OUTPUT);
  //pinMode(EN_pitch, OUTPUT);
  pinMode(MicroStep, OUTPUT);
  resetEDPins();

  pinMode(Fire, INPUT_PULLUP);
  pinMode(Fire_pulse, OUTPUT);
}

void loop() {
  
  //************************************************************* BLE Transmitter  *********************************************************************
  
  xValue = analogRead(xAxis);
  yValue = analogRead(yAxis);
  //Convert the raw data value (0 - 1023) to voltage (0.0V - 5.0V):
  float xVoltage = xValue*(5/1024.0);
  float yVoltage = yValue*(5/1024.0);
  String p1="X:";
  String p2="||";
  String p3="Y:";
  Serial.println(p1 + xVoltage + p2 + p3 + yVoltage);

//Send xVoltage and y Voltage throught BLE
    //
  //************************************************************* BLE Transmitter  *********************************************************************









  //************************************************************* BLE receiver  *********************************************************************
  // ********************* Stepper motor control (Joystick input) **************************

//Receive xVoltage and yVoltage from Transmitter

  //***** UP ***********
  if((xVoltage > 1.0 && xVoltage<2.0) && yVoltage > 3.0)
  {           
    digitalWrite(dir_pitch, LOW); //Pull direction pin high to move in "reverse"
    delayMicroseconds(dly);
    for(x= 0; x<steps; x++)  //Loop the stepping enough times for motion to be visible
    {
      digitalWrite(stp_pitch,HIGH); //Trigger one step
      delayMicroseconds(dly);
      digitalWrite(stp_pitch,LOW); //Pull step pin low so it can be triggered again
      delayMicroseconds(dly);
    }
  }
  //********************
  
  //***** UP-RIGHT *****
  else if(xVoltage > 3.0  && yVoltage > 3.0)
  {           
    digitalWrite(dir_pitch, LOW); //Pull direction pin high to move in "reverse"
    digitalWrite(dir_yaw, HIGH);
    delayMicroseconds(dly);
    for(x= 0; x<steps; x++)  //Loop the stepping enough times for motion to be visible
    {
      digitalWrite(stp_pitch,HIGH); //Trigger one step
      digitalWrite(stp_yaw,HIGH);
      delayMicroseconds(dly);
      digitalWrite(stp_pitch,LOW); //Pull step pin low so it can be triggered again
      digitalWrite(stp_yaw,LOW);
      delayMicroseconds(dly);
    }
  }
  //********************


  //***** RIGHT ********
  //else if(xVoltage < 1.0 && (yVoltage > 1.0 && yVoltage < 3.5)) //RIGHT
  else if(xVoltage > 3.0 && (yVoltage > 1.0 && yVoltage < 2.0))
  {           
    digitalWrite(dir_yaw, HIGH); //Pull direction pin high to move in "reverse"
    delayMicroseconds(dly);
    for(x= 0; x<steps; x++)  //Loop the stepping enough times for motion to be visible
    {
      digitalWrite(stp_yaw,HIGH); //Trigger one step
      delayMicroseconds(dly);
      digitalWrite(stp_yaw,LOW); //Pull step pin low so it can be triggered again
      delayMicroseconds(dly);
    }
  }
  //********************


  //***** DOWN-RIGHT ***
  else if(xVoltage < 1.0 && yVoltage > 3.0)
  {           
    digitalWrite(dir_pitch, HIGH); //Pull direction pin high to move in "reverse"
    digitalWrite(dir_yaw, HIGH);
    delayMicroseconds(dly);
    for(x= 0; x<steps; x++)  //Loop the stepping enough times for motion to be visible
    {
      digitalWrite(stp_pitch,HIGH); //Trigger one step
      digitalWrite(stp_yaw,HIGH);
      delayMicroseconds(dly);
      digitalWrite(stp_pitch,LOW); //Pull step pin low so it can be triggered again
      digitalWrite(stp_yaw,LOW);
      delayMicroseconds(dly);
    }
  }
  //********************


  //***** DOWN *********
  else if((xVoltage > 1.0 && xVoltage < 2.0) &&  yVoltage < 1.0)
  {           
    digitalWrite(dir_pitch, HIGH); //Pull direction pin high to move in "reverse"
    delayMicroseconds(dly);
    for(x= 0; x<steps; x++)  //Loop the stepping enough times for motion to be visible
    {
      digitalWrite(stp_pitch,HIGH); //Trigger one step
      delayMicroseconds(dly);
      digitalWrite(stp_pitch,LOW); //Pull step pin low so it can be triggered again
      delayMicroseconds(dly);
    }
  }
  //********************


  //***** DOWN-LEFT ****
  else if(xVoltage < 1.0 && yVoltage < 1.0)
  {           
    digitalWrite(dir_pitch, HIGH); //Pull direction pin high to move in "reverse"
    digitalWrite(dir_yaw, LOW);
    delayMicroseconds(dly);
    for(x= 0; x<steps; x++)  //Loop the stepping enough times for motion to be visible
    {
      digitalWrite(stp_pitch,HIGH); //Trigger one step
      digitalWrite(stp_yaw,HIGH);
      delayMicroseconds(dly);
      digitalWrite(stp_pitch,LOW); //Pull step pin low so it can be triggered again
      digitalWrite(stp_yaw,LOW);
      delayMicroseconds(dly);
    }
  }
  //********************


  //***** LEFT *********
  else if(xVoltage < 1.0 && (yVoltage > 1.0 && yVoltage < 2.0)) //RIGHT
  //else if(xVoltage > 3.5 && (yVoltage > 1.0 && yVoltage < 3.5))
  {           
    digitalWrite(dir_yaw, LOW); //Pull direction pin high to move in "reverse"
    delayMicroseconds(dly);
    for(x= 0; x<steps; x++)  //Loop the stepping enough times for motion to be visible
    {
      digitalWrite(stp_yaw,HIGH); //Trigger one step
      delayMicroseconds(dly);
      digitalWrite(stp_yaw,LOW); //Pull step pin low so it can be triggered again
      delayMicroseconds(dly);
    }
  }
  //********************


  //***** UP-LEFT ******
  else if(xVoltage < 1.0 && yVoltage > 3.0)
  {           
    digitalWrite(dir_pitch, LOW); //Pull direction pin high to move in "reverse"
    digitalWrite(dir_yaw, LOW);
    delayMicroseconds(dly);
    for(x= 0; x<steps; x++)  //Loop the stepping enough times for motion to be visible
    {
      digitalWrite(stp_pitch,HIGH); //Trigger one step
      digitalWrite(stp_yaw,HIGH);
      delayMicroseconds(dly);
      digitalWrite(stp_pitch,LOW); //Pull step pin low so it can be triggered again
      digitalWrite(stp_yaw,LOW);
      delayMicroseconds(dly);
    }
  }
  //********************
  
}
//*****************************************************************************************
//***************************************************************************BLE Receiver*********************************************************************************************8


//Reset Easy Driver pins to default states
void resetEDPins()
{
  digitalWrite(stp_yaw, LOW);
  digitalWrite(dir_yaw, LOW);
  //digitalWrite(EN_yaw, HIGH);
  digitalWrite(stp_pitch, LOW);
  digitalWrite(dir_pitch, LOW);
  //digitalWrite(EN_pitch, HIGH);
  digitalWrite(MicroStep, HIGH);
}
