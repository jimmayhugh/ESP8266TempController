void scanChips(void)
{
  if ( !ds.search(addr)) {
    Serial.println("No more addresses.");
    Serial.println();
    ds.reset_search();
    delay(250);
    return;
  }
  
  Serial.print("ROM =");
  for( i = 0; i < 8; i++)
  {
    Serial.write(' ');
    Serial.print(addr[i], HEX);
  }

  if (ds.crc8(addr, 7) != addr[7])
  {
      Serial.println("CRC is not valid!");
      return;
  }

  Serial.println();
 
  // the first ROM byte indicates which chip
  switch (addr[0]) {
    case ds18b20ID:
      Serial.println("  Chip = DS18B20");
      getTemp();
      break;

    case ds2406ID:
      Serial.println("  Chip = DS2406+");
      getState();
      break;

    default:
      Serial.println("Device is not a valid family device.");
      return;
  } 

  chipCnt++;
  if(chipCnt >= 3) chipCnt = 0;
}

