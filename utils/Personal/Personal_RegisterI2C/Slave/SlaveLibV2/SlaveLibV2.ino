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

#define SDA_OUTPUT DDRD |= (1 << DDB4)
#define SCL_OUTPUT DDRD |= (1 << DDB3)

#define SDA_INPUT DDRD &= ~(1 << DDB4)
#define SCL_INPUT DDRD &= ~(1 << DDB3)

#define SDA_HIGH PORTD |= (1 << PD4)//SDA = 1
#define SCL_HIGH PORTD |= (1 << PD3)//SCL = 1
#define SDA_LOW PORTD &= ~(1 << PD4) //SDA = 0
#define SCL_LOW PORTD &= ~(1 << PD3) //SCL = 0

#define SDA_READ ((PIND & (1 << PIND4)) >> PIND4) //1/0
#define SCL_READ ((PIND & (1 << PIND3)) >> PIND3) //1/0



void setup() {
  byte rev[10];
  int8_t I2CBegin;
  Serial.begin(9600);
  delay(100);
  I2CBegin = I2C_begin(0x55);

  if(I2CBegin == 0) {
    Serial.println("Master wants to read data");
    I2C_read(rev, 6);
    for(int i = 0; i < 6; i++)
      Serial.println(rev[i]); //48...
  }    
  else if(I2CBegin == 1)
    Serial.println("Master wants to read data.");
  else if(I2CBegin == 2)
    Serial.println("Wrong address. (Not my Address)");
  else 
    Serial.println("Other uncontrol case!!!");
}

void loop() {
  // put your main code here, to run repeatedly:
  //Chua chya dc----------------------
  delay(100);
}

uint8_t I2C_begin(byte address) {
  //(1) Right Address
  //    w:  return 0
  //    r:  return 1
  //(2) Wrong Address: 
  //        return 2
  //(3) NOT START CONDITION: LOOP AGAIN....
  uint8_t i , rw = 0;
  uint8_t data = 0;
  while (true) {
    SDA_INPUT;
    SCL_INPUT;
    //waiting for ideal condition----
    while((SDA_READ != 1) || (SCL_READ != 1)); //Waiting dỏ ideal condition--
    while(SDA_READ == 1); //waitning until SDA = 0
    delayMicroseconds(HALF/2); //Try to change------------
    if(SCL_READ == 1) //This is the Start condition
    {
      //read 8bit data
      while(SCL_READ == 1); //waiting until SCL = 0
      //Read address, R(1)/W(0)
      for(int i = 0; i < 8; i++) {
        while (SCL_READ == 0); //Waiting SCL =1
        data = ((data << 1) | SDA_READ); //Vì ta đọc từ bit MSB nên dịch trái khi đọc
        while(SCL_READ == 1); //Waiting SCL = 0
      }
      //data = aaaa.aaaw
      //Serial.println(data);
      rw = data & 1; //aaaa.aaaw & 0000.0001      
      data = data >> 1; //aaaa.aaaw >> 1 = 0aaa.aaaa
      if(data == address) { //It's me. 
        //Send ACK to Master
        //SUCCESS to receive all 8bit data (7 address + 1 r/w) (8 bit data)
        SDA_OUTPUT;
        SDA_LOW;
        while(SCL_READ == 0); //waiting until SCL = 1
        while(SCL_READ == 1); //waiting until scl = 0
        SDA_INPUT; //SHOULD CALL AFTER CALLING "SDA_OUTPUT"
        return rw;
      } 
      else { //This address is not me
        return 2;
      }
    }
    else { //No, it's not the Start condition
       //return -1;
    }
  }
}

byte I2C_ReadByte(void) {
  byte data;
  SDA_INPUT;
  SCL_INPUT;
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
  SDA_INPUT; //SHOULD CALL AFTER CALLING "SDA_OUTPUT"
  return data;
}

void I2C_read(byte *data, uint8_t count) {
  for(int i = 0; i < count; i++) {
      *(data + i) = I2C_ReadByte();
  }
}



#if 0
-----------------------------NOT USE-------------------
//Check return type with char, byte.
// uint8_t I2C_readByteBackup(byte address, byte count) {
//   uint8_t data = 0, i , rw = 0;
//   while (true) {
//     SDA_INPUT;
//     SCL_INPUT;
//     //waiting for ideal condition----
//     while((SDA_READ != 1) || (SCL_READ != 1));
//     while(SDA_READ == 1); //waitning until SDA = 0
//     delayMicroseconds(HALF/2);
//     if(SCL_READ == 1) //This is the Start condition
//     {
//       //read 8bit data
//       while(SCL_READ == 1); //waiting until SCL = 0
//       //Read address, R(1)/W(0)
//       for(int i = 0; i < 8; i++) {
//         while (SCL_READ == 0); //Waiting SCL =1
//         data = ((data << 1) | SDA_READ); //Vì ta đọc từ bit MSB nên dịch trái khi đọc
//         while(SCL_READ == 1); //Waiting SCL = 0
//       }
//       //data = aaaa.aaaw
//       rw = data & 1; //aaaa.aaaw & 0000.0001
//       data = data >> 1; //aaaa.aaaw >> 1 = 0aaa.aaaa
//       if(data == address) { //It's me. 
//         //Send ACK to Master
//         //SUCCESS to receive all 8bit data (7 address + 1 r/w) (8 bit data)
//         SDA_OUTPUT;
//         SDA_LOW;
//         while(SCL_READ == 0); //waiting until SCL = 1
//         while(SCL_READ == 1); //waiting until scl = 0
//         //SDA_INPUT; //Check it later
//       } 
//       else { //Thí address is not me
//         //Do nothing....
//       }






//       //read 8bit data
//       for(int i = 0; i < 8; i++) {
//         while (SCL_READ == 0); //Waiting SCL =1
//         data = ((data << 1) | SDA_READ); //Vì ta đọc từ bit MSB nên dịch trái khi đọc
//         // data 1 = 1: 0000.0000 + 1 = 0000.0001
//         // data 2 = 0: 0000.0010 + 0 = 0000.0010
//         // data 2 = 1: 0000.0100 + 1 = 0000.0101
//         while(SCL_READ == 1); //Waiting SCL = 0
//       }

//       //SUCCESS to receive all 8bit data (7 address + 1 r/w) (8 bit data)
//       SDA_OUTPUT;
//       SDA_LOW;
//       while(SCL_READ == 0); //waiting until SCL = 1
//       while(SCL_READ == 1); //waiting until scl = 0
//       SDA_INPUT;
      
//       return data;
//     }
//     else //No, it's not the Start Condition .....:(
//     {
//     }
//   }
// }
#endif