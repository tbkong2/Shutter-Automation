#define Servo1 3 //This is to control the first motor
#define Servo2 5 //This is to contorl the second motor
char command; 
String string;
int onTime1=500; //onTime is the amount of time that the motors are on High and low from
int onTime2=500;
int ind1,ind2; // LabVIEW will send a string that looks like Angle1, #, Angle2, #, and the ind1 and ind2 are indicators ',' to concatenate the strings
String strangle1, strangle2; 
int Angle1, Angle2;
int Angle=0;
void setup() {
  Serial.begin(9600); //This is necessary for the baud rate
  pinMode(Servo1, OUTPUT);
  pinMode(Servo2, OUTPUT); //this function makes the pin work as an output, this case pin 3 and 5 are outputs so that the motors turn
}

void loop() {
  if (Serial.available()>0) { //This checks if there is an input given to the serial port
    string = ""; //creates an empty string 
  }
  while (Serial.available()>0) {
    command = ((byte)Serial.read()); //reads each byte of the serial input
    if (command== ':') { //breaks the function if it is at the end of input
      break;
    } else {
      string += command; //combines all of the input strings together
      Action(onTime1, onTime2); //This function is what turns the motors, needs the constant calling for it to stay at a certain angle
    }
    delay(1);
  }
  ind1 = string.indexOf(','); //Finds the index of the first indicator
  strangle1 = string.substring(0, ind1); //finds the string of the first angle
  ind2 = string.indexOf(',', ind1+1); //finds the index of the second indicator
  strangle2 = string.substring(ind1+1, ind2); //finds the string of the second angle
  Angle1 = strangle1.toInt(); //changes the string angle to an integer 
  Angle2 = strangle2.toInt();  //changes to integer
  if ((Angle1>0)&&(Angle1<90)) { //Since the motors are piecewise linear, we need two different cases.
    onTime1=(int) ((Angle1+103.4)/0.17233); //This is the formula of how long the motors have to be in HIGH for to get the specific angle
  } else if ((Angle1>=90)&&(Angle1<=270)) {
    onTime1=(int) ((Angle1+62.808)/0.1359);//This is the formula of the same thing for angles between 90 and 270
  } else {
    onTime1 = 500;
  }
  if ((Angle2<90)&&(Angle2>0)) {//This is to find the onTime for the second motor
    onTime2=(int) ((Angle2+103.4)/0.17233); //This is the formula of how long the motors have to be in HIGH for to get the specific angle
  } else if ((Angle2>=90)&&(Angle2<=270)) {
    onTime2=(int) ((Angle2+62.808)/0.1359);//This is the formula for the same thing but for angles between 90 and 270
  } else {
    onTime2 = 500; //This onTime keeps the angle at 0 degrees angle
  }
  Action(onTime1, onTime2); //After finding the onTime for each motor, we call a function that implements the onTime to the motors
}

void Action(int onTime1, int onTime2) {
  if (onTime1>onTime2) { //There are 3 cases for which the onTimes can be, and depending on which case it is, they must be structured in this format
    digitalWrite(Servo1, HIGH);
    digitalWrite(Servo2, HIGH);
    delayMicroseconds(onTime2);
    digitalWrite(Servo2, LOW);
    delayMicroseconds(onTime1-onTime2);
    digitalWrite(Servo1, LOW);
    delayMicroseconds(10000-onTime1);//This entire if statement is for when the first motor has a greater angle than the second motor. The first motor must be on digital high for longer.
  } else if (onTime2>onTime1) {
    digitalWrite(Servo1, HIGH);
    digitalWrite(Servo2, HIGH);
    delayMicroseconds(onTime1);
    digitalWrite(Servo1, LOW);
    delayMicroseconds(onTime2-onTime1);
    digitalWrite(Servo2, LOW);
    delayMicroseconds(10000-onTime2);//This entire else if statement is for when the second motor has a greater angle than the first motor.
  } else {
    digitalWrite(Servo1, HIGH);
    digitalWrite(Servo2, HIGH);
    delayMicroseconds(onTime1);
    digitalWrite(Servo1, LOW);
    digitalWrite(Servo2, LOW);
    delayMicroseconds(10000-onTime1); // This else statement is for when they are the same angle.
  }
}



