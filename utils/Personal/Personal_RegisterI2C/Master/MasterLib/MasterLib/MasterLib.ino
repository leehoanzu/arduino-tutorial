/*=============================================================================
  - Đây là chương trình mô phỏng điều khiển thủ công hoạt động 
    của kết nối dây I2C giữa 2 thiết bị
=============================================================================*/

#include <Wire.h>

#define SCL_PIN 3
#define SDA_PIN 4

#define SDA_OUTPUT DDRD |= (1 << SDA_PIN);
#define SCL_OUTPUT DDRD |= (1 << SCL_PIN);

#define SDA_INPUT DDRD &= ~(1 << SDA_PIN);
#define SCL_INPUT DDRD &= ~(1 << SCL_PIN);

#define SDA_HIGH PORTD |= (1 << PD4);  //SDA = 1
#define SCL_HIGH PORTD |= (1 << PD3);  //SCL = 1
#define SDA_LOW PORTD &= ~(1 << PD4);  //SDA = 0
#define SCL_LOW PORTD &= ~(1 << PD3);  //SCL = 0

//I2C: STANDARD (100KHZ ~ 10US), HIGHER
//try: slow(01) f=20KHZ ~50us
//try: slow(02) f=1kHz ~ 1000us
#define HALF 500
#define FULL 1000

void setup() {
  //Nên cho ở trạng thái ở mức cao trước
  //Để tránh tình trạng gây nhiễu tín hiệu
  //ideal state-----------------
  SDA_HIGH;
  SCL_HIGH;

  //set up hai chan 3 va 4 la ngo ra
  SDA_OUTPUT;
  SCL_OUTPUT;
  delay(1000);
  //beginI2C(0x55);
}

//CHỉ có thể dùng được khi mô phỏng ở mạch thật
// int main (void) {
//   beginI2C(0x55);
//   delay(1000);
// }
void loop() {
  // put your main code here, to run repeatedly:
  I2C_WriteByte(0x55);
  delay(1000);
}


//0x55 01010101
//i2c: 8bit: MSB gửi trước
void I2C_WriteByte(byte data) {
  //START-------------------
  //ideal state-----------------//Nen de o muc cao truoc de tranh tinh trang float
  SDA_HIGH;
  SCL_HIGH;
  //set up hai chan 3 va 4 la ngo ra
  SDA_OUTPUT;
  SCL_OUTPUT;
  delayMicroseconds(FULL);

  SDA_LOW;
  delayMicroseconds(HALF);  //START
  SCL_LOW;
  delayMicroseconds(HALF);

  SDA_HIGH;
  delayMicroseconds(HALF);  //CLOCK 2
  SCL_HIGH;
  delayMicroseconds(HALF);

  SCL_LOW;
  SDA_LOW;
  delayMicroseconds(HALF);  //CLOCK 2
  SCL_HIGH;
  delayMicroseconds(HALF);

  SCL_LOW;
  SDA_HIGH;
  delayMicroseconds(HALF);  //CLOCK 3
  SCL_HIGH;
  delayMicroseconds(HALF);

  SCL_LOW;
  SDA_LOW;
  delayMicroseconds(HALF);  //CLOCK 4
  SCL_HIGH;
  delayMicroseconds(HALF);

  SCL_LOW;
  SDA_HIGH;
  delayMicroseconds(HALF);  //CLOCK 5
  SCL_HIGH;
  delayMicroseconds(HALF);

  SCL_LOW;
  SDA_LOW;
  delayMicroseconds(HALF);  //CLOCK 6
  SCL_HIGH;
  delayMicroseconds(HALF);

  SCL_LOW;
  SDA_HIGH;
  delayMicroseconds(HALF);  //CLOCK 7
  SCL_HIGH;
  delayMicroseconds(HALF);

  SCL_LOW;
  SDA_LOW;
  delayMicroseconds(HALF);  //CLOCK 8
  SCL_HIGH;
  delayMicroseconds(HALF);

  SCL_LOW;
  //MÔ PHỎNG TÍN HIỆU NACK
  SDA_INPUT;  //PULLUP RES WILL BRIGD SDA LINE TO HIGH
  //SDA_HIGH; //FORCE HIGH(NACK)
  delayMicroseconds(HALF);  //CLOCK 9: ACK/NACK
  SCL_HIGH;
  uint8_t ACK = (PIND & (1 << PIND4)) >> PIND4;  //0, 1(LSB)
  delayMicroseconds(HALF);

  SCL_LOW;
  delayMicroseconds(FULL);  //STOP(P)
  SDA_OUTPUT;
  SDA_LOW;
  delayMicroseconds(HALF);
  SCL_HIGH;
  delayMicroseconds(HALF);
  SDA_HIGH;
}
