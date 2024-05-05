/*=============================================================================
  - Đây là chương trình mô phỏng điều khiển thủ công hoạt động 
    của kết nối dây I2C giữa 2 thiết bị
  - CHương trình này bổ xung thêm đọc tín hiệu NACK, tín hiệu đọc trả về yêu cầu kết nối
    giữa các thiết bị
=============================================================================*/

#include <Wire.h>

#define SCL_PIN 3
#define SDA_PIN 4

#define SDA_OUTPUT DDRD |= (1 << SDA_PIN);
#define SCL_OUTPUT DDRD |= (1 << SCL_PIN);

#define SDA_INPUT DDRD &= ~(1 << SDA_PIN);
#define SCL_INPUT DDRD &= ~(1 << SCL_PIN);

#define SDA_HIGH PORTD |= (1 << PD4); //SDA = 1
#define SCL_HIGH PORTD |= (1 << PD3); //SCL = 1
#define SDA_LOW PORTD &= ~(1 << PD4); //SDA = 0
#define SCL_LOW PORTD &= ~(1 << PD3); //SCL = 0

//I2C: STANDARD (100KHZ ~ 10US), HIGHER
//try: slow(01) f=20KHZ ~50us
//try: slow(02) f=1kHz ~ 1000us
#define FULL 10
#define HALF 5

void setup() {
  //ideal state-----------------//Nen de o muc cao truoc de tranh tinh trang float
  SDA_HIGH;
  SCL_HIGH;  
  //set up hai chan 3 va 4 la ngo ra
  SDA_OUTPUT;
  SCL_OUTPUT;
  delay(100);
}

//CHỉ có thể dùng được khi mô phỏng ở mạch thật
// int main (void) {
//   beginI2C(0x55);
//   delay(1000);
// }
void loop() {
  // put your main code here, to run repeatedly:
  uint8_t ACK;
  ACK = I2C_WriteByte(0xAA); //0x55 + 0 = 1010.1010 => 0xAA
  delay(1000); 
}


//0x55 01010101
//i2c: 8bit: MSB gửi trước
uint8_t I2C_WriteByte(byte data) {
  //ideal state-----------------//Nen de o muc cao truoc de tranh tinh trang float
  SDA_HIGH;
  SCL_HIGH;  
  //set up hai chan 3 va 4 la ngo ra
  SDA_OUTPUT;
  SCL_OUTPUT;
  delayMicroseconds(FULL);

  //START-------------------
  SDA_LOW;  delayMicroseconds(HALF); //START
  SCL_LOW;  delayMicroseconds(HALF);

  //CLOCK X-------------X: 1-8-------------------0x80: 1000.0000
  //data = 1010.1010 & 1000.0000 = 1000.0000
  //dataV2  = 0101.0101 & 1000.0000 = 0000.0000
  //data << 1 =  0101.0100
  //CHÚ Ý: I < 8 
  //NẾU BẠN ĐỂ I << 8 SẼ BỎ QUA VÒNG LẶP FOR VÀ BUG
  for(int i = 0; i < 8; i++) {
    if((data & 0x80) != 0) {//NEU MSB = 1
      SDA_HIGH; 
    }  
    else {
      SDA_LOW;  
    }
    data = data << 1;
    delayMicroseconds(HALF); //LOW(1/2), HIGH(1/2);
    SCL_HIGH;
    delayMicroseconds(HALF);
    SCL_LOW;
  }  

  //SCL_LOW;
  //MÔ PHỎNG TÍN HIỆU NACK
  //SDA_INPUT; //PULLUP RES WILL BRIGD SDA LINE TO HIGH
  SDA_LOW; //FORCE HIGH(NACK)
  delayMicroseconds(HALF); //CLOCK 9: ACK/NACK
  SCL_HIGH;
  uint8_t ACK = (PIND & (1 << PIND4)) >> PIND4; //0, 1(LSB)
  delayMicroseconds(HALF);
  SCL_LOW;

  //STOP(P) Condition--------------------------------- 
  //delayMicroseconds(FULL); //STOP(P) Can delete(may be). Try it later
  SDA_OUTPUT; //Chú ý vì ở bit 9 đã cấu hình là input
  SDA_LOW;  delayMicroseconds(HALF);
  SCL_HIGH; delayMicroseconds(HALF);
  SDA_HIGH;   

  return ACK;
}
