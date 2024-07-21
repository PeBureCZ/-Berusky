void printTime() 
{
  unsigned int elapsed = actualTime - lastTime;
  unsigned int hours = elapsed / 3600;
  unsigned int minutes = (elapsed % 3600) / 60;
  unsigned int seconds = elapsed % 60;

  // Use sprintf to format the string with leading zeros
  char timeStr[9]; // Format: "HH:MM:SS\0"
  sprintf(timeStr, "%02u:%02u:%02u", hours, minutes, seconds);

  t0.setText(timeStr);
}