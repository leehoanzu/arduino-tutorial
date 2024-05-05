/*--------------------------------------------------------------------------------------------------------------
- Đây là bài mô phỏng hoạt động của SPI không sử dụng các chân có sẵn trên Arduino Rev3
- Tại đây ta sẽ mô phỏng bằng các chân D4 : D7 để tạo ra các dạng sóng
-                             Theo thứ tự: SCK...MOSI...MISO...SS
- Trong bài này ta sẽ mô phỏng bằng proteus do đó các arduino chỉ hiểu được và đọc các hàm setup và loop
- Ta không thể sử dụng hàm main như những bài trước hay mô phỏng proteus này
- Ở bài này có rất nhiều chú ý đáng quan trọng nên làm hay thật cẩn thận
- Đây là thư viện cho Slave nên ta sẽ chủ động cài đặt các chân ngõ ra là MISO được phân tích theo dạng sóng với CPHA = 0 | CPOL = 0
--------------------------------------------------------------------------------------------------------------*/

//Khai báo thư viện
#include <avr/io.h>
#include <avr/interrupt.h>

#define SCK_PIN   4
#define MOSI_PIN  5
#define MISO_PIN  6
#define SS_PIN    7

//SPI speed: MHz(real), kHz(Simualation)
//Slow_01: f = 20kHz -> T = 50us
//Slow-02: f = 1kHz   -> T = 1000us
#define TFull 50
#define THalf TFull / 2

//Định nghĩa macro: là các hàm bằng cách định nghĩa thông qua definr
//Đây ta sẽ macro in/out pin
#define SCK_INPUT   DDRD &= ~(1 << DDD4)
#define MOSI_INPUT  DDRD &= ~(1 << DDD5)
#define SS_INPUT    DDRD &= ~(1 << DDD7)
#define MISO_OUTPUT DDRD |=  (1 << DDD6)

//Macro ghi dữ liệu mức cao hay thấp
//#define MOSI_HIGH_PORTD PORTD |= (1 << PD5)   //digitalWrite(MOSI_PIN, HIGH)
//#define MOSI_LOW_PORTD  PORTD &= ~(1 << PD5)  //digitalWrite(MOSI_PIN, LOW)
//Để gọn hơn ta sẽ dùng tam đẳng thức
//a = value ? x : y; //if(value == true(!= 0))  a = x; else a = y;
//Nếu x là high thì PORTD |= (1 << PD5)
//         low       PORTD &= ~(1 << PD5)

//*** CHÚ Ý LỖI:
//vIỆC ĐỌC PORTD = ((PIND & (1 << PIND5)) ? HIGH : LOW) LÀ SAI
//PORTD LÀ MACRO DÙNG ĐỂ CẤU HÌNH NGÕ RA CHỨ KHÔNG PHẢI LÀ ĐỌC GIÁ TRỊ TRÊN CHÂN NÀO ĐÓ
//NẾU CẤU HÌNH SẼ GÂY RA BUG CHUYỂN VỀ TOÀN 0
//VIỆC ĐOC KHÔNG NHẤT THIẾT PHẢI GHI PIND = KHI DỮ LIỆU TRẢ VỀ CÓ THỂ LÀ TRỰC TIẾP
//                                                         HIGH OR LOW
#define read_MOSI() PIND = ((PIND & (1 << PIND5)) ? HIGH : LOW)
#define read_SS()   PIND = ((PIND & (1 << PIND7)) ? HIGH : LOW)
#define read_SCK()  PIND = ((PIND & (1 << PIND4)) ? HIGH : LOW) //HOẶC LÀ KHÔNG CẦN PIND = 

//Macro đọc dữ liệu
//Nếu nhận về từ chân d6 là 1 thì đọc dữ liệu sẽ trả về là high
//                           0                             low
#define write_MISO(x) ((x) ? (PORTD |= (1 << 6)) : (PORTD &= ~(1 << 6)))  //digitalRead(MISO, PIN)

void setup() {
  SPI_config();
  Serial.begin(9600);

}

void loop() {
  uint8_t rev[3];
  rev[0] = SPI_transfer('V');  //0x38 | 56
  rev[1] = SPI_transfer('U'); 
  rev[2] = SPI_transfer('Z'); 
  //Serial.println(rev);
  Serial.println("S: " + String((char)rev[0]));
  Serial.println("S: " + String((char)rev[1]));
  Serial.println("S: " + String((char)rev[2]));
}

//Initial IN MODE 0
//CPOL =0 | CPHASE = 0
void SPI_config(void) {
  SCK_INPUT;   
  MOSI_INPUT;  
  SS_INPUT;  
  write_MISO(LOW);
  MISO_OUTPUT;
}

//Truyền nhận data
//MODE 0: CPOL =0 | CPHASE = 0. bitOder: MSB(truyền bit cao trước)
//VD: byte_out = 0x55 (0101 0101)
//Step1 (bit7): byte_out & 0x80 (1000 0000) =
//          0101 0101  &  1000 0000 = 0000 0000
//            byte_out = 0xAA (1010 1010)
//          1010 1010  &  1000 0000 = 1000 0000
//  if bit7 = HIGH -> write_MOSI(HIGH)
//Step2 (bit6): byte_out & 0x40 = 1010 1010 & 0100 0000 = 0x00 0000
//  if bit6 = LOW -> write_MOSI(LOW)

//1000 0000 (0x80) -> 0100 0000 -> 0010 0000 -> .... 0000 0001 -> 0000 0000 (Stop condition)
// ibit = 0x80, ibit = ibit >>1
//-> byte_out & ibit = (!= 0) or (== 0)
//Phân tích từ hình 18-3 datasheet
uint8_t SPI_transfer(uint8_t byte_out) {
  uint8_t byte_in = 0;  //0000 0000
  uint8_t ibit, reslt;

  //Slave đợi tin hiệu từ chân SS
  //đến khi Slave được chọn (SS bit == 0). Nếu không dữ liệu sẽ nằm chờ
  while (read_SS() == HIGH);  //Waiting until ss = 0 (LOW)

  //Việc lặp lại 8 lần là vì cần 8 xung để
  //có thể truyền/nhận đủ 8bit trong 1 chu kì nhận/gửi

  for (ibit = 0x80; ibit > 0; ibit = ibit >> 1) {  //ibit = 1000 0000
                                                //(!= 0 -- true) or (== 0 -- false)
    //***Chú ý dễ sai
    //Nếu reslt =| byte_out & ibit; thì sẽ gây ra là lỗi ghi bit mới 
    //và cộng lại thì => tất cả các bit đầu ra đều bằng 1   
    reslt = byte_out & ibit;                                                 
    write_MISO(reslt); 
    //printf("reslt: %x\n", reslt); 
 
    while (read_SCK() == LOW);  //Waitiing util SCK = 1
    // //Vì khi data ổn định ở nữa chu kì thì SPI sẽ đọc giá trị từ thanh ghi
    // //Hoặc gửi giá trị đó đến với slave
    // delayMicroseconds(THalf);
    // write_SCK(HIGH);

    if (read_MOSI() == HIGH)
      byte_in = byte_in | ibit;

    while (read_SCK() == HIGH);  //waiting sck = 0 (xuống mức thấp)
    //until here, this is the "end of 1 clock cycle"
  }
  return byte_in;
}


// int main(void) {
//   SPI_config();
//   while (1) {
//     uint8_t rev[3];
//     rev[0] = SPI_transfer('V');  //0x38 | 56
//     rev[1] = SPI_transfer('U'); 
//     rev[2] = SPI_transfer('Z'); 
//     //Serial.println(rev);
//     Serial.println("S: " + String((char)rev[0]));
//     Serial.println("S: " + String((char)rev[1]));
//     Serial.println("S: " + String((char)rev[2]));
//   }
//   return 0;
// }




