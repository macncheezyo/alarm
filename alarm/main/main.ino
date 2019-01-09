#include <LiquidCrystal.h>
#include <MsTimer2.h>
#include <TimerFreeTone.h>
#include <Metro.h>

/*
STATES
==================
0 = time
1 = alarm high
2 = set alarm
*/

/*LCD SETUP LCD SETUP LCD SETUP LCD SETUP LCD SETUP LCD SETUP */
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//BUTTONS
const int plusButton = 7;
const int soundWire = 3;
const int minusButton = 6;
const int subModeSelectButton = 4;
const int lightWire = 5;
const int modeSelectButton = 2;

uint8_t state = 0;
uint8_t *subState; 

uint8_t timeHours = 22;
uint8_t timeMinutes = 5;
uint8_t timeSeconds = 0;

uint8_t alarmHours = 23;
uint8_t alarmMinutes = 5;

uint8_t lightInterval = 10;
uint8_t alarmEnabled = 1;
bool snoozed = false;

void setup() 
{

  //LCD ==========================
  lcd.begin(16, 2);

  //INTERRUPTS ===================
  attachInterrupt(digitalPinToInterrupt(modeSelectButton), modeSelect, FALLING);
  MsTimer2::set(1000, oneSecond);
  MsTimer2::start(); 

  //BUTTONS ======================
  pinMode(plusButton, INPUT);
  pinMode(minusButton, INPUT);
  pinMode(subModeSelectButton, INPUT);
  pinMode(soundWire, OUTPUT);
  pinMode(lightWire, OUTPUT);

  loadHome();
}


void normalizeTime()
{

  //ALARM ENABLED
  if(alarmEnabled%2 == 1) alarmEnabled = 1;
  if(alarmEnabled%2 == 0) alarmEnabled = 0;

  //LIGHT
  if(lightInterval < 5) lightInterval = 5;
  if(lightInterval > 60) lightInterval = 60;

  //TIME
  if(timeSeconds > 59)
  {
    timeMinutes += timeSeconds/60;
    timeSeconds %= 60;
  }

  if(timeMinutes > 59)
  {
    timeHours += timeMinutes/60;
    timeMinutes %= 60;
  }

  if(timeHours > 23)
    timeHours %= 24;


  //ALARM
  if(alarmMinutes > 59)
  {
    alarmHours += alarmMinutes/60;
    alarmMinutes %= 60;
  }

  if(alarmHours > 23)
    alarmHours %= 24;

}

void loop() 
{

  //ALARM IS SOUNDING AND LIGHTS ARE FLASHING
  if(state == 1)
  {
    static Metro blink1Metro = Metro(100);

    if(blink1Metro.check())
    {
      static int y = 0;
      y = !y;
  
      //LIGHT OFF TONE ON
      if(y)
      {
        TimerFreeTone(soundWire, 1000, 100);
        digitalWrite(lightWire, LOW);
      }
  
      //LIGHT ON TONE OFF
      else
      {
        TimerFreeTone(soundWire, 0, 100);
        digitalWrite(lightWire, HIGH);   
      } 
    }
  }

  while(state == 2)
  {
    static int minusState = 0;
    static int plusState = 0;
    static int subModeChangeState = 0;
    static Metro blinkTimer = Metro(500);
    static bool blinkTimerFlag = false;

    if(blinkTimer.check())
    {
      blinkTimerFlag = !blinkTimerFlag;
      loadAlarmSet(blinkTimerFlag);
    }
    
    if(digitalRead(plusButton) && !minusState)
    {
      if(subState) (*subState)++;
      minusState = 1;
      normalizeTime();
      loadAlarmSet(false);
    }

    if(digitalRead(minusButton) && !plusState)
    {
      if(subState) (*subState)--;
      plusState = 1;
      normalizeTime();
      loadAlarmSet(false);
    }

    if(digitalRead(subModeSelectButton) && !subModeChangeState)
    {
      if(subState == &alarmEnabled) subState = &alarmHours;
      else if(subState == &alarmHours) subState = &alarmMinutes;
      else if(subState == &alarmMinutes) subState = &alarmEnabled;
      subModeChangeState = 1;
      loadAlarmSet(false);
    }

    if(!digitalRead(plusButton)) minusState = 0;
    if(!digitalRead(minusButton)) plusState = 0;
    if(!digitalRead(subModeSelectButton)) subModeChangeState = 0;
  }

  while(state == 3)
  {
    static int minusState = 0;
    static int plusState = 0;
    static Metro blinkTimer = Metro(500);
    static bool blinkTimerFlag = false;
    
    if(blinkTimer.check())
    {
      blinkTimerFlag = !blinkTimerFlag;
      loadLightSet(blinkTimerFlag);
    }

    if(digitalRead(plusButton) && !minusState)
    {
      if(subState) (*subState)++;
      minusState = 1;
      normalizeTime();
      loadLightSet(false);
    }

    if(digitalRead(minusButton) && !plusState)
    {
      if(subState) (*subState)--;
      plusState = 1;
      normalizeTime();
      loadLightSet(false);
    }

    if(!digitalRead(plusButton)) minusState = 0;
    if(!digitalRead(minusButton)) plusState = 0;
    
  }


  while(state == 4)
  {
    static int minusState = 0;
    static int plusState = 0;
    static int subModeChangeState = 0;
    static Metro blinkTimer = Metro(500);
    static bool blinkTimerFlag = false;

    if(blinkTimer.check())
    {
      blinkTimerFlag = !blinkTimerFlag;
      loadTimeSet(blinkTimerFlag);
    }
    
    if(digitalRead(plusButton) && !minusState)
    {
      if(subState) (*subState)++;
      minusState = 1;
      normalizeTime();
      loadTimeSet(false);
    }

    if(digitalRead(minusButton) && !plusState)
    {
      if(subState) (*subState)--;
      plusState = 1;
      normalizeTime();
      loadTimeSet(false);
    }

    if(digitalRead(subModeSelectButton) && !subModeChangeState)
    {
      if(subState == &timeHours) subState = &timeMinutes;
      else if(subState == &timeMinutes) subState = &timeHours;
      subModeChangeState = 1;
      loadTimeSet(false);
    }

    if(!digitalRead(plusButton)) minusState = 0;
    if(!digitalRead(minusButton)) plusState = 0;
    if(!digitalRead(subModeSelectButton)) subModeChangeState = 0;
  }
                       
}
