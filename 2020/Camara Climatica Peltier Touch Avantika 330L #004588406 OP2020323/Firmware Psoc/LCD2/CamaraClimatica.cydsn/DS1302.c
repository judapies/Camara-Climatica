/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <DS1302.h>

// --------------------------------------------------------
// _DS1302_start
//
// A helper function to setup the start condition.
//
// An 'init' function is not used.
// But now the pinMode is set every time.
// That's not a big deal, and it's valid.
// At startup, the pins of the Arduino are high impedance.
// Since the DS1302 has pull-down resistors,
// the signals are low (inactive) until the DS1302 is used.
void _DS1302_start()
{
  DS1302_CE_PIN_Write(LOW); // default, not enabled
  //pinMode( DS1302_CE_PIN, OUTPUT);  

  DS1302_SCLK_PIN_Write(LOW); // default, clock low
  //pinMode( DS1302_SCLK_PIN, OUTPUT);

  DS1302_CE_PIN_Write( HIGH); // start the session
  CyDelayUs( 4);           // tCC = 4us
}

// --------------------------------------------------------
// _DS1302_stop
//
// A helper function to finish the communication.
//
void _DS1302_stop(void)
{
  // Set CE low
  DS1302_CE_PIN_Write(LOW);

  CyDelayUs( 4);           // tCWH = 4us
}

// --------------------------------------------------------
// _DS1302_togglewrite
//
// A helper function for writing a byte with bit toggle
//
// The 'release' parameter is for a read after this write.
// It will release the I/O-line and will keep the SCLK high.
//
void _DS1302_togglewrite( uint8_t data, uint8_t release)
{
  int i;

  for( i = 0; i <= 7; i++)
  {
    // set a bit of the data on the I/O-line
    DS1302_IO_PIN_Write( bitRead(data, i));  
    CyDelayUs( 1);     // tDC = 200ns

    // clock up, data is read by DS1302
    DS1302_SCLK_PIN_Write( HIGH);    
    CyDelayUs( 1);     // tCH = 1000ns, tCDH = 800ns

    if( release && i == 7)
    {
      // If this write is followed by a read,
      // the I/O-line should be released after
      // the last bit, before the clock line is made low.
      // This is according the datasheet.
      // I have seen other programs that don't release
      // the I/O-line at this moment,
      // and that could cause a shortcut spike
      // on the I/O-line.
      //pinMode( DS1302_IO_PIN, INPUT);

      // For Arduino 1.0.3, removing the pull-up is no longer needed.
      // Setting the pin as 'INPUT' will already remove the pull-up.
      // digitalWrite (DS1302_IO, LOW); // remove any pull-up  
    }
    else
    {
      DS1302_SCLK_PIN_Write( LOW);
      CyDelayUs( 1);       // tCL=1000ns, tCDD=800ns
    }
  }
}

// --------------------------------------------------------
// _DS1302_toggleread
//
// A helper function for reading a byte with bit toggle
//
// This function assumes that the SCLK is still high.
//
uint8_t _DS1302_toggleread( void)
{
  uint8_t i, data;

  data = 0;
  for( i = 0; i <= 7; i++)
  {
    // Issue a clock pulse for the next databit.
    // If the 'togglewrite' function was used before
    // this function, the SCLK is already high.
    DS1302_SCLK_PIN_Write(HIGH);
    CyDelayUs( 1);

    // Clock down, data is ready after some time.
    DS1302_SCLK_PIN_Write(LOW);
    CyDelayUs( 1);        // tCL=1000ns, tCDD=800ns

    // read bit, and set it in place in 'data' variable
    bitWrite( data, i, DS1302_IO_PIN_Read());
  }
  return( data);
}

// --------------------------------------------------------
// DS1302_clock_burst_write
//
// This function writes 8 bytes clock data in burst mode
// to the DS1302.
//
// This function may be called as the first function,
// also the pinMode is set.
//
void DS1302_clock_burst_write( uint8_t *p)
{
  int i;

  _DS1302_start();

  // Instead of the address,
  // the CLOCK_BURST_WRITE command is issued.
  // the I/O-line is not released
  _DS1302_togglewrite( DS1302_CLOCK_BURST_WRITE, false);  

  for( i=0; i<8; i++)
  {
    // the I/O-line is not released
    _DS1302_togglewrite( *p++, false);  
  }
  _DS1302_stop();
}


// --------------------------------------------------------
// DS1302_read
//
// This function reads a byte from the DS1302
// (clock or ram).
//
// The address could be like "0x80" or "0x81",
// the lowest bit is set anyway.
//
// This function may be called as the first function,
// also the pinMode is set.
//
uint8_t DS1302_read(int address)
{
  uint8_t data;

  // set lowest bit (read bit) in address
  bitSet( address, DS1302_READBIT);  

  _DS1302_start();
  // the I/O-line is released for the data
  _DS1302_togglewrite( address, true);  
  data = _DS1302_toggleread();
  _DS1302_stop();

  return (data);
}


// --------------------------------------------------------
// DS1302_write
//
// This function writes a byte to the DS1302 (clock or ram).
//
// The address could be like "0x80" or "0x81",
// the lowest bit is cleared anyway.
//
// This function may be called as the first function,
// also the pinMode is set.
//
void DS1302_write( int address, uint8_t data)
{
  // clear lowest bit (read bit) in address
  bitClear( address, DS1302_READBIT);  

  _DS1302_start();
  // don't release the I/O-line
  _DS1302_togglewrite( address, false);
  // don't release the I/O-line
  _DS1302_togglewrite( data, false);
  _DS1302_stop();  
}


// --------------------------------------------------------
// DS1302_clock_burst_read
//
// This function reads 8 bytes clock data in burst mode
// from the DS1302.
//
// This function may be called as the first function,
// also the pinMode is set.
//
void DS1302_clock_burst_read( uint8_t *p)
{
  int i;

  _DS1302_start();

  // Instead of the address,
  // the CLOCK_BURST_READ command is issued
  // the I/O-line is released for the data
  _DS1302_togglewrite( DS1302_CLOCK_BURST_READ, true);  

  for( i=0; i<8; i++)
  {
    *p++ = _DS1302_toggleread();
  }
  _DS1302_stop();
}

void rtc_set_datetime(uint8 dayofmonth, uint8 month, uint8 year, uint8 dayofweek, uint8 hours, uint8 minutes){
    // Fill the structure with zeros to make
    // any unused bits zero
    memset ((char *) &rtc2, 0, sizeof(rtc2));
    
    rtc2.Seconds    = bin2bcd_l( 0);
    rtc2.Seconds10  = bin2bcd_h( 0);
    rtc2.CH         = 0;      // 1 for Clock Halt, 0 to run;
    rtc2.Minutes    = bin2bcd_l( minutes);
    rtc2.Minutes10  = bin2bcd_h( minutes);
    // To use the 12 hour format,
    // use it like these four lines:
    //    rtc.h12.Hour   = bin2bcd_l( hours);
    //    rtc.h12.Hour10 = bin2bcd_h( hours);
    //    rtc.h12.AM_PM  = 0;     // AM = 0
    //    rtc.h12.hour_12_24 = 1; // 1 for 24 hour format
    rtc2.h24.Hour   = bin2bcd_l( hours);
    rtc2.h24.Hour10 = bin2bcd_h( hours);
    rtc2.h24.hour_12_24 = 0; // 0 for 24 hour format
    rtc2.Date       = bin2bcd_l( dayofmonth);
    rtc2.Date10     = bin2bcd_h( dayofmonth);
    rtc2.Month      = bin2bcd_l( month);
    rtc2.Month10    = bin2bcd_h( month);
    rtc2.Day        = dayofweek;
    rtc2.Year       = bin2bcd_l( year);
    rtc2.Year10     = bin2bcd_h( year);
    rtc2.WP = 0;  
    
    // Write all clock data at once (burst mode).
    DS1302_clock_burst_write( (uint8_t *) &rtc2);
}

void rtc_get_date(uint8* day, uint8* mth, uint8* year, uint8* dow) {
    // Read all clock data at once (burst mode).
    DS1302_clock_burst_read( (uint8_t *) &rtc2);
   *day=bcd2bin( rtc2.Date10, rtc2.Date);
   *mth=bcd2bin( rtc2.Month10, rtc2.Month);
   *year=bcd2bin( rtc2.Year10, rtc2.Year);
   *dow=rtc2.Day;
}

void rtc_get_time(uint8* hr, uint8* min, uint8* sec) {
    // Read all clock data at once (burst mode).
    DS1302_clock_burst_read( (uint8_t *) &rtc2);
   *hr=bcd2bin( rtc2.h24.Hour10, rtc2.h24.Hour);
   *min=bcd2bin( rtc2.Minutes10, rtc2.Minutes);
   *sec=bcd2bin( rtc2.Seconds10, rtc2.Seconds);
}

/* [] END OF FILE */
