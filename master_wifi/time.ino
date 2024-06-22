void printTime() 
{
  int hours = actualTime / 3600;
  int minutes = (actualTime - hours*3600) / 60;
  int seconds = actualTime % 60;

  String timeStr = "";
  (hours < 10) ? timeStr = "0" + hours : timeStr = hours;
  (minutes < 10) ? timeStr = timeStr + ":" + "0" + minutes : timeStr = timeStr + ":" + minutes;
  (seconds < 10) ? timeStr = timeStr + ":" + "0" + seconds : timeStr = timeStr + ":" + seconds;

  //Serial.println(timeStr);
 //TODO print to display
}