#include <AccelStepper.h>

// Debug
#define DEBUG
#ifdef DEBUG
  #define DEBUG_PRINTL(x)  Serial.println(x)
  #define DEBUG_PRINT(x)  Serial.print(x)
#else
  #define DEBUG_PRINTL(x)
  #define DEBUG_PRINT(x)
#endif


// ##### ##### ##### ##### ##### ##### ##### ##### ##### DEBOUNCED BUTTON (NO, INPUT_PULLUP)
class Button {
  public:
  unsigned long DebounceDelay;
  uint8_t State;
  bool Pressed;
  bool Released;

  Button(uint8_t pin){
    DebounceDelay = 50;
    _Pin = pin;    
    _LastState = false;
    pinMode(_Pin, INPUT_PULLUP);
  }

  void Update(){
    Released = false;
    Pressed = false;

    uint8_t state = digitalRead(_Pin);

    if (state != _LastState){
      _LastStateTime = millis();      
    }

    if ((millis() - _LastStateTime) > DebounceDelay){
      // PRESSED OR RELEASED
      if (state != State){
        State = state;

        // INPUT_PULLUP = 1 = RELEASED
        if (State == 1){
          Released = true;
          Pressed = false;
        }
        // PRESSED
        else{
          Released = false;
          Pressed = true;
        }
      }
    }

    // NO CHANGE
    _LastState = state;
  }

  private:
  uint8_t _Pin;
  unsigned long _LastStateTime;  
  uint8_t _LastState;
};

// ##### ##### ##### ##### ##### ##### ##### ##### ##### END OF BUTTON CLASS

Button Button1(A4);
Button Button2(A3);
Button Button3(A2);
Button Button4(A1);

AccelStepper StepperMotor(1, 2, 3);

void setup() {
  // Debug
  #ifdef DEBUG
    Serial.begin(115200);
  #endif

  // PIN A5 IS COMMON CONNECTION FOR BUTTONS
  pinMode(A5, OUTPUT);
  digitalWrite(A5, 0);

  StepperMotor.setMaxSpeed(1000);
  StepperMotor.setAcceleration(1000);
  StepperMotor.moveTo(0);

}

void loop() {

  // Update button states
  Button1.Update();
  Button2.Update();
  Button3.Update();
  Button4.Update();

  // Motor Not Busy
  if (StepperMotor.distanceToGo() == 0)
  {
    // CLOCKWISE 1 STEP
    if (Button1.Pressed == true){      
      StepperMotor.moveTo(StepperMotor.currentPosition() + 1);
    }

    // COUNTER-CLOCKWISE 1 STEP
    if (Button2.Pressed == true){
      StepperMotor.moveTo(StepperMotor.currentPosition() - 1);
    }

    // CLOCKWISE 1 TURN
    if (Button3.Pressed == true){
      StepperMotor.moveTo(StepperMotor.currentPosition() + 50);
    }

    // COUNTER-CLOCKWISE 1 TURN
    if (Button4.Pressed == true){
      StepperMotor.moveTo(StepperMotor.currentPosition() - 50);
    }
  }

  StepperMotor.run();

}
