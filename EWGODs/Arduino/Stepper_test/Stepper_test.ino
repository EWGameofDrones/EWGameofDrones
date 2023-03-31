//Stepper Yaw
//DIR-BLUE | STEP-GREEN | ENABLE-YELLOW

//Stepper Pitch
//DIR-ORANGE | STEP-RED | ENABLE-BROWN


int pos=0;


// stepper control outputs
#define DirPinYaw0    10
#define StepPinYaw0   11
#define EnablePinYaw0 12

#define DirPinPitch0    16
#define StepPinPitch0   15
#define EnablePinPitch0 14

#define stepsPerRevolution 2000
#define motorInterfaceType 1

// Motor speeds
int fast = 1;
int medium = 5;
int slow = 10;
//************************************************
int speedYaw = fast; // change motor speeds here
int speedPitch = fast;
//************************************************

void YawCCW() {

  for (int x = 0; x < stepsPerRevolution; x++) {
    digitalWrite(EnablePinYaw0, LOW);    
    digitalWrite(DirPinYaw0, LOW);
    digitalWrite(StepPinYaw0, HIGH);
    delay(speedYaw);
    digitalWrite(StepPinYaw0, LOW);
    delay(speedYaw);
  }
}

void YawCW() {
  for (int x = 0; x < stepsPerRevolution; x++) {
    digitalWrite(EnablePinYaw0, LOW);    
    digitalWrite(DirPinYaw0, HIGH);
    digitalWrite(StepPinYaw0, HIGH);
    delay(speedYaw);
    digitalWrite(StepPinYaw0, LOW);
    delay(speedYaw);
    
  }
}

void PitchCCW() {

  for (int x = 0; x < stepsPerRevolution; x++) {
    digitalWrite(EnablePinPitch0, LOW);
    digitalWrite(DirPinPitch0, LOW);
    digitalWrite(StepPinPitch0, HIGH);
    delay(speedPitch);
    digitalWrite(StepPinPitch0, LOW);
    delay(speedPitch);
  }
}

void PitchCW() {
  for (int x = 0; x < stepsPerRevolution; x++) {
    digitalWrite(EnablePinPitch0, LOW);
    digitalWrite(DirPinPitch0, HIGH);
    digitalWrite(StepPinPitch0, HIGH);
    delay(speedPitch);
    digitalWrite(StepPinPitch0, LOW);
    delay(speedPitch);
  }
}

void setup() {
  Serial.begin(9600); // initialize serial communication 
  pinMode(DirPinYaw0, OUTPUT);
  pinMode(StepPinYaw0, OUTPUT);
  pinMode(EnablePinYaw0, OUTPUT);
  pinMode(DirPinPitch0, OUTPUT);
  pinMode(StepPinPitch0, OUTPUT);
  pinMode(EnablePinPitch0, OUTPUT);

  digitalWrite(EnablePinYaw0, HIGH);
  digitalWrite(EnablePinPitch0, HIGH);
}

void loop() {
       
YawCCW();
delay(2000);
YawCW();
delay(2000);
PitchCCW();
delay(2000);
PitchCW();
delay(2000);
digitalWrite(EnablePinYaw0, HIGH);
digitalWrite(EnablePinPitch0, HIGH);
delay(5000);
}
