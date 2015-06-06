void updateLCD(void)
{
  Serial.println("Writing to LCD");
  lcd.home();
  IPAddress ip = WiFi.localIP();
  lcd.print(ip);
  delay(delayVal);
  lcd.setCursor(0, 1);
  sprintf(lcdBuffer, "Port: %d", localPort);
  lcd.print(lcdBuffer);
  delay(delayVal);
  lcd.setCursor(0, 2);
  sprintf(lcdBuffer, "Temp: %d F, %d C", fahrenheit, celsius);
  lcd.print(lcdBuffer);
  delay(delayVal);
  lcd.setCursor(0, 3);
  sprintf(lcdBuffer, "Switch1:%c Switch2:%c", chipStatus[1], chipStatus[2]);
  lcd.print(lcdBuffer);
  delay(delayVal);
  Serial.println("Finished writing to LCD");
}
