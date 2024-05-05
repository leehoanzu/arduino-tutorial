/*=============================================================================
  - Đây là chương trình thư viện tham khảo kết nối I2C
  - Được viết bỏi Nicholas Zambetti <http://www.zambetti.com>
=============================================================================*/

#include <Wire.h>

void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
}

byte x = 0;

void loop()
{
  Wire.beginTransmission(0x55); // transmit to device #4
  Wire.write("012345");        // sends five bytes
  //Wire.write(x);              // sends one byte  
  Wire.endTransmission();    // stop transmitting

  x++;
  delay(500);
}
