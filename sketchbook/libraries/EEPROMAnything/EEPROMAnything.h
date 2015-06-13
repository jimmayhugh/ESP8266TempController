#include <EEPROM.h>
#include <Arduino.h>  // for type definitions

template <class T> int EEPROM_writeAnything(uint16_t ee, const T& value)
{
  const byte* p = (const byte*)(const void*)&value;
  uint32_t i;
  for (i = 0; i < sizeof(value); i++)
  {
    const byte b = *p;
    if( EEPROM.read( ee ) != b )
      EEPROM.write( ee++, b ), ++p;
    else
      ee++, p++;
  }
  return i;
}

template <class T> int EEPROM_readAnything(uint16_t ee, T& value)
{
  byte* p = (byte*)(void*)&value;
  uint32_t i;
  for (i = 0; i < sizeof(value); i++)
  *p++ = EEPROM.read(ee++);
  return i;
}
