/*=============================================================================
  - Đây là chương trình mô phỏng điều khiển thủ công hoạt động 
    của kết nối dây I2C giữa 2 thiết bị
  - CHương trình này bổ xung thêm đọc tín hiệu NACK, tín hiệu đọc trả về yêu cầu kết nối
    giữa các thiết bị và tín hiệu dừng, bắt đầu.
=============================================================================*/

#include <Wire.h>

//I2C: STANDARD (100KHZ ~ 10US), HIGHER (fast: 400khz)
//try: slow(01) f=20KHZ ~50us
//try: slow(02) f=1kHz ~ 1000us
#define FULL 50 //ở Mô phỏng nên chọn tần số thấp
#define HALF FULL/2

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

#define SDA_READ (PIND & (1 << SDA_PIN)) >> SDA_PIN; //1/0
#define SCL_READ (PIND & (1 << SCL_PIN)) >> SCL_PIN; //1/0

void setup() {
  delay(100);
  Serial.begin(9600);
} 

//CHỉ có thể dùng được khi mô phỏng ở mạch thật
// int main (void) {
//   beginI2C(0x55);
//   delay(1000);
// }
void loop() {
  int8_t reslt;
  reslt = I2C_WriteData(0x55, "012345"); //TRUE: S AA A 61 A 62 A 63 A P
                                      //FALSE: S AB N P        
  //                                0x56 = 1010.1100 = AC (dec)                                     
  Serial.println(reslt);
  delay(1000); 
}


int8_t I2C_WriteData(byte address, char *data) {
  int8_t ACK;
  //(1)START-------------------
  I2C_StartCondition();

  //(2) sEND ADDRESS (7BIT) + W(0)--- 7bit Adr + w-----------
  //address = address << 1 + 0; 0101.0101 << 1 = 1010.1010
  //                => Không cần cộng bit W(0)
  address = address << 1;
  I2C_Write8bit(address);

  //Check ACK---------
  ACK = I2C_ReadACK();
  if(ACK == 1) {
    I2C_StopCondition();
    return -1; //FAIL
  }

  //(3)SEND DATAs-------"abc" = 'a' 'b' 'c' '\0' -----
  while(*data  != '\0') { //~data[0]
    I2C_Write8bit(*data);
    //Check ACK---------
    ACK = I2C_ReadACK(); //check ACK ------
    if(ACK == 1) {
      I2C_StopCondition();
      return -1; //FAIL
    }
    data++;  //(data + 1) = &data[1]
  }

  //(4)STOP-------------------------------------------
  I2C_StopCondition();

  return 1;
}

/*
  Nếu không sử dụng con trỏ để gửi data
    int i = 0
  //SEND DATAs-------"abc" = 'a' 'b' 'c' '\0' -----
  while(data[i]  != '\0') { //~data[0]
    I2C_Write8bit(*data);
    //Check ACK---------
    ACK = I2C_ReadACK(); //check ACK ------
    if(ACK == 1) {
      I2C_StopCondition();
      return -1; //FAIL
    }
    i++;  //(data + 1) = &data[1]
  }
*/

void I2C_StartCondition(void) {
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
}


void I2C_StopCondition(void) { //ACK/NACK (0, 1)
  //Lúc này sẽ đọc được 1 tín hiệu HIGH nửa chu kì
  SDA_OUTPUT; //Chú ý vì ở bit 9 đã cấu hình là input
  //Tín hiệu SCL delay này sẽ xuất hiện sau SDA output nữa chu kì

  //STOP(P) Condition--------------------------------- 
  SDA_LOW;  
  delayMicroseconds(HALF);
  SCL_HIGH; 
  delayMicroseconds(HALF);
  SDA_HIGH;
  //SDA_INPUT; //CHECK IT LATER...    
  //SCL_INPUT; //CHECK IT LATER... 
}

void I2C_Write8bit(byte data) { //only send 8bit with 8 clock, NO START/STOP/ACK
  
  //Lúc này sẽ đọc được 1 tín hiệu HIGH nửa chu kì
  SDA_OUTPUT; //Chú ý vì ở bit 9 đã cấu hình là input
  //Tín hiệu SCL delay này sẽ xuất hiện sau SDA output nữa chu kì

  //Nếu là đọc data thì bắt đầu sau dạng sóng SDA trong khoảng thời gian
  //chia cắt(kéo dài nữa/1 chu kì)
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
}

uint8_t I2C_ReadACK(void) {
  //READ ACK/NACK-------CLOCK(9
  SDA_INPUT; //PULLUP RES WILL BRIGD SDA LINE TO HIGH. SLAVE CONTROL SDA_LINE
  delayMicroseconds(HALF); //CLOCK 9: 
  SCL_HIGH;
  delayMicroseconds(HALF);
  uint8_t ACK = SDA_READ; //0, 1(LSB)
  SCL_LOW;  //AT THIS POINT, SLAVE START RELEASE SDA_LINE ****
  delayMicroseconds(HALF); //Must have. Seperate between each 8bit
  return ACK; 
}

//0x55 0101.0101
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
  SDA_OUTPUT; //Chú ý vì ở bit 9 đã cấu hình là input
  SCL_LOW;
  //delayMicroseconds(FULL); //STOP(P) Can delete(may be). Try it later
  
  //STOP(P) Condition--------------------------------- 
  SDA_LOW;  delayMicroseconds(HALF);
  SCL_HIGH; delayMicroseconds(HALF);
  SDA_HIGH;    

  return ACK;
}
