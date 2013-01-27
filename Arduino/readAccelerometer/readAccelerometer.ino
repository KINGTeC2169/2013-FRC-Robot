/************************************************************************
* Bare bones ADXL345 i2c example for Arduino 1.0
* by Jens C Brynildsen <http://www.flashgamer.com>
* 
* Adapted for use on the 2013 Ultimate Ascent Robot
*
* Accelerometer: ADXL345 (using i2c)
*    Datasheet: http://www.sparkfun.com/datasheets/Sensors/Accelerometer/ADXL345.pdf
*
* If you need more advanced features such as freefall and tap *
* detection, check out: *
* https://github.com/jenschr/Arduino-libraries *
***********************************************************************
*        Pinouts
*            //Arduino Uno//
* Arduino         <->        Breakout board
* Gnd              -              GND
* 5V               -              5V
* Analog 4         -              SDA
* Analog 5         -              SLC
*
*            //Arduino Mega//
* Arduino         <->        Breakout board
* Gnd              -              GND
* 5V               -              5V
* 20               -              SDA
* 21               -              SLC
***************************************************************************/

#include <Wire.h>

#define DEVICE (0x1D) // Device address as specified in data sheet

byte _buff[6];

char POWER_CTL = 0x2D;	//Power Control Register
char DATA_FORMAT = 0x31;
char DATAX0 = 0x32;	// X-Axis Data 0
char DATAX1 = 0x33;	// X-Axis Data 1
char DATAY0 = 0x34;	// Y-Axis Data 0
char DATAY1 = 0x35;	// Y-Axis Data 1
char DATAZ0 = 0x36;	// Z-Axis Data 0
char DATAZ1 = 0x37;	// Z-Axis Data 1
char RANGE2G = 0x00;    // +/- 2g
char RANGE4G = 0x01;    // +/- 4g
char RANGE8G = 0x10;    // +/- 8g
char RANGE16G = 0x11;   // +/- 16g


void setup()
{
  // join i2c bus (address optional for master)
  Wire.begin();
  // Start serial at 57000 baud. If the serial console is returning strange characters, make sure you are on 57600 baud.
  Serial.begin(57000); 
  Serial.print("Initializing");
  //Put the ADXL345 into +/- 4G range by writing the value 0x01 to the DATA_FORMAT register.
  writeTo(DATA_FORMAT, 0x01);
  //Put the ADXL345 into Measurement Mode by writing 0x08 to the POWER_CTL register.
  writeTo(POWER_CTL, 0x08);
}

void loop()
{
  readAccel(); // read the x/y/z tilt
  sPrintAccel(); //prints tilt data to serial
  delay(50); // sample at 20Hz
}
