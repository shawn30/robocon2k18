#include<Servo.h>

int pos = 0, cpos = 0;                                  //position and current position
String cmd;                                             //command to be entered
bool cac[2] = {false, false};                           //cac[0] to toggle relative positioning or absolute; cac[1] to toggle anti-clockwise,clockwise
Servo myservo;                                          //servo object
bool mode = true;                                       //toggle smooth=true,staggered=false
int stepsize, steptime, x, y, ac, c;                    //supportive variables

void setup() {
  myservo.attach(9);                                    //servo attached to pin 9
  Serial.begin(9600);                                   //initiating serial communication
  Serial.println("My Servo interpreter");               //Startup message
  myservo.write(pos);                                   //initialing servo position to 0

}

void mov(int pos) {                                          //write function; to write pos to servo
  if (cac[0] == false);                                      //if clock and anticlockwise is false, do nothing
  else {                                                     //else if clock and anticlockwise is on
    if (cac[1] == false)                                     //if anti-clockwise is on
      pos = cpos + ac;                                       //calculate relative anticlock pos
    else.                                                    //If clockwise is on
      pos = cpos - c;                                        //calculate relarive clockwise pos
  }
  if (mode == true) {                                        //toggle smooth write
    myservo.write(pos);
    cpos = pos;
  }
  else {                                                     // toggle staggered write
    if (cpos <= pos) {                                       //with the help of stepsize and steptime
      for (int i = cpos; i <= pos; i += stepsize) {
        myservo.write(i);
        delay(steptime);
      }
    }
    else if (cpos > pos) {
      for (int i = cpos; i >= pos; i -= stepsize) {
        myservo.write(i);
        delay(steptime);
      }
    }
    cpos = pos;                                             //updating current position to pos
  }
}

void loop() {
  if (Serial.available()) {                                 
    cmd = Serial.readStringUntil(';');                       //read the command
    if (cmd.startsWith("servo")) {    
      if (cmd.startsWith("mode", 6)) {                      
        if (cmd.startsWith("smooth", 11)) {
          mode = true;
          Serial.print("Setting Servo mode to smooth");      //servo mode smooth;
        }
        else if (cmd.startsWith("staggered", 11)) {
          Serial.print("Setting Servo mode to staggered");   //servo mode staggered <stepsize><steptime>;
          x = cmd.indexOf('<');
          y = cmd.indexOf('<', x + 1);
          stepsize = cmd.substring(x + 1).toInt();
          steptime = cmd.substring(y + 1).toInt();
          Serial.print(" with stepsize: ");
          Serial.print(stepsize);
          Serial.print(" and steptime: ");
          Serial.println(steptime);
          mode = false;
        }
      }
      else if (cmd.startsWith("ac", 6)) {                     //servo ac <relative-position>;
        x = cmd.indexOf('<');
        ac = cmd.substring(x + 1).toInt();
        Serial.print("Moving anti-clockwise to: ");
        Serial.print(ac);
        Serial.println(" Degree");
        cac[0] = true;
        cac[1] = false;
      }

      else if (cmd.startsWith("c", 6)) {                      //servo c <relative-position>;
        x = cmd.indexOf('<');
        c = cmd.substring(x + 1).toInt();
        Serial.print("Moving clockwise to: ");
        Serial.print(c);
        Serial.println(" Degree");
        cac[0] = true;
        cac[1] = true;
      }
      else if (cmd.substring(6) == "set") {                   //servo set;
        Serial.println("Servo set");
        pos = 179;
        cac[0] = false;
      }
      else if (cmd.substring(6) == "reset") {                  //servo reset;
        Serial.println("Servo reset");
        pos = 0;
        cac[0] = false;
      }
      else if (cmd.substring(6) == "halfset") {               //servo halfset;
        Serial.println("Servo halfset");
        pos = 90;
        cac[0] = false;
      }
      else if (cmd.startsWith("abs", 6)) {                      //servo abs <absolute-position>;
        x = cmd.indexOf('<');
        pos = cmd.substring(x + 1).toInt();
        Serial.print("Servo abs to position ");
        Serial.println(pos);
        cac[0] = false;
      }
      mov(pos);
    }
    else Serial.println(cpos);                                  //; to know current postition
  }                                                             //the postion and other variables can go out of their range/limit
                                                                //the servo simply wont move though current position would be changed
}
