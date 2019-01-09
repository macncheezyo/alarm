#include <Metro.h>


void loadHome()
{
  String topText = "";
  String bottomText = "";

  /* TOP TEXT ============================ */
  if(timeHours < 10) topText += '0';
  topText = topText + timeHours + ':';

  if(timeMinutes < 10) topText += '0';
  topText = topText + timeMinutes + ':';

  if(timeSeconds < 10) topText += '0';
  topText = topText + timeSeconds + "        ";

  /* BOTTOM TEXT ========================= */
  if(alarmEnabled == 1)
  {
    if(alarmHours < 10) bottomText += '0';
    bottomText = bottomText + alarmHours + ":";

    if(alarmMinutes < 10) bottomText += '0';
    bottomText = bottomText + alarmMinutes + "           ";
  }
  else bottomText = "                ";
  
  //UPLOAD TEXT
  lcd.setCursor(0, 0);
  lcd.print(topText);
  
  lcd.setCursor(0, 1);
  lcd.print(bottomText);
  
}

void loadAlarmSet(bool blinkValue)
{
  String topText = "";
  String bottomText = "";

  /* TOP TEXT ============================*/
  topText += "Set Alarm: ";

  if(blinkValue && subState == &alarmEnabled) topText = topText + "     ";
  
  else
  {
    if(alarmEnabled == 1) topText += "   on";
    else if(alarmEnabled == 0) topText += "  off"; 
  }

  /* BOTTOM TEXT ========================= */
  if(blinkValue && subState == &alarmHours) bottomText = bottomText + "  ";

  else 
  {
    if(alarmHours < 10) bottomText += '0';
    bottomText = bottomText + alarmHours;  
  }
  
  bottomText += ':';
  
  if(blinkValue && subState == &alarmMinutes) bottomText = bottomText + "  ";

  else 
  {
    if(alarmMinutes < 10) bottomText += '0';
    bottomText = bottomText + alarmMinutes; 
  }
  
  bottomText += "           ";
  
  //UPLOAD TEXT
  lcd.setCursor(0, 0);
  lcd.print(topText);
  lcd.setCursor(0, 1);
  lcd.print(bottomText);
}

void loadLightSet(bool blinkValue)
{
  String topText = "";
  String bottomText = "";

  /* TOP TEXT ============================*/
  topText += "Light Interval  ";

  /* BOTTOM TEXT ========================= */
  if(blinkValue) bottomText = bottomText + "                :";
  else bottomText = bottomText + lightInterval + " min           ";
  
  //UPLOAD TEXT
  lcd.setCursor(0, 0);
  lcd.print(topText);
  lcd.setCursor(0, 1);
  lcd.print(bottomText);
}


void loadTimeSet(bool blinkValue)
{
  String topText = "";
  String bottomText = "";

  /* TOP TEXT ============================*/
  topText += "Set Time:       ";

  /* BOTTOM TEXT ========================= */
  if(blinkValue && subState == &timeHours) bottomText = bottomText + "  ";

  else 
  {
    if(timeHours < 10) bottomText += '0';
    bottomText = bottomText + timeHours;  
  }
  
  bottomText += ':';
  
  if(blinkValue && subState == &timeMinutes) bottomText = bottomText + "  ";

  else 
  {
    if(timeMinutes < 10) bottomText += '0';
    bottomText = bottomText + timeMinutes; 
  }
  
  bottomText += "           ";
  
  //UPLOAD TEXT
  lcd.setCursor(0, 0);
  lcd.print(topText);
  lcd.setCursor(0, 1);
  lcd.print(bottomText);
}
