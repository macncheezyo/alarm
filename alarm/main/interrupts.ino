


void modeSelect()
{

   static unsigned long lastTime = 0;
   unsigned long interruptTime = millis();

   if(interruptTime - lastTime > 200)
   {
    
      //CHANGE STATE
      if(state == 0)
      {
        state = 2;
        loadAlarmSet(true);
      }
      
      else if(state == 1)
      {
        state = 0;
        alarmMinutes += 5;
        normalizeTime();
        snoozed = true;
      }
      
      else if(state == 2)
      {
        state = 3;
        loadLightSet(true);
      }
      
      else if(state == 3)
      {
        state = 4;
        loadTimeSet(true);
      }

      else if(state == 4)
      {
        state = 0;
        loadHome();
      }
    
      //INITIALIZE SUBSTATES
      if(state == 0) subState = 0;
      if(state == 1) subState = 0;
      if(state == 2) subState = &alarmEnabled;
      if(state == 3) subState = &lightInterval;
      if(state == 4) subState = &timeHours;    
   }
   
   lastTime = interruptTime;

}

void oneSecond()
{
  //INCREMENT ONE SECOND
  timeSeconds++;
  normalizeTime();

  //CHECK TIMER
  if(alarmEnabled)
  {
    if(timeHours == alarmHours && timeMinutes == alarmMinutes) state = 1;
  
    //CHECK IF WERE IN THE LIGHT ZONE
    int16_t alarm = alarmHours*60 + alarmMinutes;
    int16_t timeMin = timeHours*60 + timeMinutes;
    int16_t light = (alarm + 1440 - lightInterval)%1440;
    uint16_t shift = 1440 - light;
  
    light = (light + shift)%1440;
    alarm = (alarm + shift)%1440;
    timeMin = (timeMin + shift)%1440;
    
    if(timeMin >= light && timeMin <= alarm)
      analogWrite(lightWire, 230*(timeMin + 1 - light)/lightInterval );
  
    if(snoozed) analogWrite(lightWire, 230); 
  }

  
  //UPDATE THE PROGRAM
  if(state == 0 || state == 1) loadHome();
}
