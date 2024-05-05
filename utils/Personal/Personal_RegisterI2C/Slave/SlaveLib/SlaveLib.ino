/*=============================================================================
  - Đây là chương trình mô phỏng điều khiển thủ công hoạt động 
    của kết nối dây I2C giữa 2 thiết bị
  - CHương trình này bổ xung thêm đọc tín hiệu NACK, tín hiệu đọc trả về yêu cầu kết nối
    giữa các thiết bị
=============================================================================*/

#include <Wire.h>

//I2C: STANDARD (100KHZ ~ 10US), HIGHER (fast: 400khz)
//try: slow(01) f=20KHZ ~50us
//try: slow(02) f=1kHz ~ 1000us
#define FULL 50 //ở Mô phỏng nên chọn tần số thấp
#define HALF FULL/2

#define SCL_PIN 3
#define SDA_PIN 4

#define SDA_OUTPUT DDRD |= (1 << SDA_PIN)
#define SCL_OUTPUT DDRD |= (1 << SCL_PIN)

#define SDA_INPUT DDRD &= ~(1 << SDA_PIN)
#define SCL_INPUT DDRD &= ~(1 << SCL_PIN)

#define SDA_HIGH PORTD |= (1 << PD4)//SDA = 1
#define SCL_HIGH PORTD |= (1 << PD3)//SCL = 1
#define SDA_LOW PORTD &= ~(1 << PD4) //SDA = 0
#define SCL_LOW PORTD &= ~(1 << PD3) //SCL = 0

#define SDA_READ ((PIND & (1 << SDA_PIN)) >> SDA_PIN) //1/0
#define SCL_READ ((PIND & (1 << SCL_PIN)) >> SCL_PIN) //1/0



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(100);
  Serial.println("Start..");
  uint8_t data = I2C_readByte(0); //Detect Start condition, read 8 bit data feedback ACK
  //Serial.print("Data: " + data); CHecj later
  Serial.print("Data: ");
  Serial.println(data);
}

void loop() {
  // put your main code here, to run repeatedly:

}

//Check return type with char, byte.
int8_t I2C_readByte(byte address) {
  int8_t data = 0;
  while (true) {
    SDA_INPUT;
    SCL_INPUT;
    //waiting for ideal condition----
    while((SDA_READ != 1) || (SCL_READ != 1));
    while(SDA_READ == 1); //waitning until SDA = 0
    delayMicroseconds(HALF/2);
    if(SCL_READ == 1) //This is the Start condition
    {
      //read 8bit data
      while(SCL_READ == 1); //waiting until SCL = 0
      //read 8bit data
      for(int i = 0; i < 8; i++) {
        while (SCL_READ == 0); //Waiting SCL =1
        data = ((data << 1) | SDA_READ); //Vì ta đọc từ bit MSB nên dịch trái khi đọc
        // data 1 = 1: 0000.0000 + 1 = 0000.0001
        // data 2 = 0: 0000.0010 + 0 = 0000.0010
        // data 2 = 1: 0000.0100 + 1 = 0000.0101
        while(SCL_READ == 1); //Waiting SCL = 0
      }

      //SUCCESS to receive all 8bit data (7 address + 1 r/w) (8 bit data)
      SDA_OUTPUT;
      SDA_LOW;
      while(SCL_READ == 0); //waiting until SCL = 1
      while(SCL_READ == 1); //waiting until scl = 0
      SDA_INPUT;
      
      return data;
    }
    else //No, it's not the Start Condition .....:(
    {
    }
  }
}
