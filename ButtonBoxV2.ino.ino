#include <RotaryEncoder.h>
#include <Keypad.h>
#include <Joystick.h>

//Pins for Rotary Encoder Switch
#define rot1IntPin1 0
#define rot1IntPin2 1
#define rot2IntPin1 2
#define rot2IntPin2 3
#define rot3IntPin1 8
#define rot3IntPin2 7

//Pins for Rotary Encoder Push Button
#define rot1Button 6
#define rot2Button 5
#define rot3Button 4

#define ENABLE_PULLUPS

#define NUMCOLS 5 //amount of buttons in the matrix row
#define NUMROWS 3 // amount of button rows

//Button values on the Joystick for the Encoder Push buttons
#define Rot1BtnNum 14 
#define Rot2BtnNum 15
#define Rot3BtnNum 16

const bool initAutoSendState = true;

//Determines the button order per row
byte buttons[NUMROWS][NUMCOLS] = {
{4,3,2,1,0},
{9,8,7,6,5},
{10,11,12,13,14}
};

byte rowPins[NUMROWS] = {20,19,18}; //Per Row Arduino Pins (Pins A0-A2)
byte colPins[NUMCOLS] = {9,10,11,12,13}; //Per Column Arduino Pins (Digital Pins 9-13)

Joystick_ butJoystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD, 24, 0,
false, false, false, false, false, false,
false, false, false, false, false); 

Keypad buttbx = Keypad(makeKeymap(buttons), rowPins, colPins, NUMROWS, NUMCOLS);

RotaryEncoder encoder1(rot1IntPin1,rot1IntPin2);
RotaryEncoder encoder2(rot2IntPin1,rot2IntPin2);
RotaryEncoder encoder3(rot3IntPin1,rot3IntPin2);

void setup() {
  pinMode(rot1Button, INPUT);
  pinMode(rot2Button, INPUT);
  pinMode(rot3Button, INPUT);
  digitalWrite(rot1Button, HIGH);
  digitalWrite(rot2Button, HIGH);
  digitalWrite(rot3Button, HIGH);
  attachInterrupt(digitalPinToInterrupt(rot1IntPin1), getTick1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(rot1IntPin2), getTick1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(rot2IntPin1), getTick2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(rot2IntPin2), getTick2, CHANGE); 
  attachInterrupt(digitalPinToInterrupt(rot3IntPin2), getTick3, CHANGE); 
  attachInterrupt(digitalPinToInterrupt(rot3IntPin1), getTick3, CHANGE); 
  butJoystick.begin(true);
}

void getTick1(){
  encoder1.tick();    
}
void getTick2(){
  encoder2.tick();
}
void getTick3(){
  encoder3.tick();    
}
void loop() {
  CheckEncoders();
  CheckAllButtons();
}

void CheckAllButtons(void) 
{  
  //Detecting button presses on the button matrix
  if (buttbx.getKeys())
  {
    for (int i=0; i<LIST_MAX; i++)
    {
   //   Serial.println(buttbx.key[i].kstate);
      if ( buttbx.key[i].stateChanged )
      {    
        //Serial.println("Button Pressed: " + String(i));
        switch (buttbx.key[i].kstate) {
          case PRESSED:
          case HOLD:
            butJoystick.setButton(buttbx.key[i].kchar, 1);
            break;
          case RELEASED:
          case IDLE:
            butJoystick.setButton(buttbx.key[i].kchar, 0);
            break;
        }
      }
    }
  }
  

  // Detecting the Rotary Encoder Push Button events
  if (digitalRead(rot1Button)==LOW)  butJoystick.setButton(Rot1BtnNum,1);
  else butJoystick.setButton(Rot1BtnNum,0);
  

  if (digitalRead(rot2Button)==LOW)  butJoystick.setButton(Rot2BtnNum,1);
  else butJoystick.setButton(Rot2BtnNum,0);
  

  if (digitalRead(rot3Button)==LOW)  butJoystick.setButton(Rot3BtnNum,1);
  else butJoystick.setButton(Rot3BtnNum,0);
  
}


//Detect Encoder changes we have a short delay for Interupt based pins, and a slightly longer delay for direct reading (button 22) to allow it to register properly
void CheckEncoders(void)
{  
  int dir1= (int)encoder1.getDirection();
  int dir2= (int)encoder2.getDirection();
  int dir3= (int)encoder3.getDirection();
 
  if (dir1 != 0) {
    if (dir1 < 0) { butJoystick.setButton(18,1); delay(100); }
      else if (dir1 > 0) { butJoystick.setButton(19,1); delay(100);}
    }
    else 
    {
      butJoystick.setButton(18,0);
      butJoystick.setButton(19,0);
    }

    if (dir2 != 0) {
    if (dir2 < 0) { butJoystick.setButton(20,1); delay(100);}   
      else if (dir2 > 0) { butJoystick.setButton(21,1); delay(100);}
    }
    else 
    {
      butJoystick.setButton(20,0);
      butJoystick.setButton(21,0);
    }

    if (dir3 != 0) {
      if (dir3 < 0) { butJoystick.setButton(22,1); delay(200);}   
      else if (dir3 > 0) { butJoystick.setButton(23,1); delay(100);}   
    }
    else 
    {
       butJoystick.setButton(22,0);
       butJoystick.setButton(23,0);
    }
}
