void getState(void)
{
uint16_t chipCRCval, chipBufferCRC;
  ds.reset();
  ds.select(addr);
  ds.write(ds2406MemRd);
  data[0] = ds2406MemRd;
  ds.write(0x0); //2406 Addr Low
  data[1] = 0;
  ds.write(0x0); //2406 Addr Hgh
  data[2] = 0;
  for(int i = 3; i <  13; i++)
  {
    data[i] = ds.read();
  }
  ds.reset();

  chipCRCval = ~(ds.crc16(data, 11)) & 0xFFFF;
  chipBufferCRC = ((data[12] << 8) | data[11]);
  
  Serial.print("Data = ");
  for(int i = 3; i <  13; i++)
  {
    if(data[i] < 0x0f)
    {
      Serial.print("0x0");
    }else{
      Serial.print("0x");
    }
    Serial.print(data[i], HEX);
    Serial.print(", ");    
  }
  Serial.print("chipCRCval = 0x");
  Serial.print(chipCRCval, HEX);
  Serial.print(", chipBufferCRC = 0x");
  Serial.print(chipBufferCRC, HEX);

  if(data[10] & dsPIO_A)
  {
    chipStatus[chipCnt] = switchStatusOFF;
  }else{
    chipStatus[chipCnt] = switchStatusON;
  }
  Serial.print(", chipStatus[");
  Serial.print(chipCnt);
  Serial.print("] = ");
  Serial.println( (char) chipStatus[chipCnt] );
}

