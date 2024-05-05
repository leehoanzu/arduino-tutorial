/*--------------------------------------------------------------------------------------------------------------
- Đây là bài mô phỏng hoạt động của SPI không sử dụng các chân có sẵn trên Arduino Rev3
- Tại đây ta sẽ mô phỏng bằng các chân D4 : D7 để tạo ra các dạng sóng
-                             Theo thứ tự: SCK...MOSI...MISO...SS
- Trong bài này ta sẽ mô phỏng bằng proteus do đó các arduino chỉ hiểu được và đọc các hàm setup và loop
- Ta không thể sử dụng hàm main như những bài trước hay mô phỏng proteus này
- Ở bài này có rất nhiều chú ý đáng quan trọng nên làm hay thật cẩn thận
- Đây là thư viện cho Master nên ta sẽ chủ động cài đặt các chân ngõ ra được phân tích theo dạng sóng với CPHA = 0 | CPOL = 0
--------------------------------------------------------------------------------------------------------------*/
/*      CHÚ Ý:
Có thể xuất hiện lỗi truyền k đc dữ liệu
có thể do IDE tối ưu không tốt và gât mất dữ liệu
Thì mình sẽ báo cho compiler báo biến đó sẽ thay đổi liên tucn
Cách 1: Option dont optimize
Cách 2: Biến nào bị optimize hạn chế thì ép kiểu (valtile)
                   rằng biến này thay đổi kích thước liên tục nên k tối ưu
VD: volatile int result;
   (volatile) PORTD = ((x) ? (PORTD |= (1 << PD5)) : (PORTD &= ~(1 << PD5)))
*/

//Khai báo thư viện
#include <avr/io.h>
#include <avr/interrupt.h>

//Định nghĩa các chân
#define SCK_PIN   4
#define MOSI_PIN  5
#define MISO_PIN  6
#define SS_PIN    7

//SPI speed: MHz(real), kHz(Simualation)
//Slow_01: f = 20kHz -> T = 50us (Nên dùng)
//Slow-02: f = 1kHz   -> T = 1000us
//Slow-03: f = 2Mhz   -> T = 0.5us
//Slow-04: f = 0.5Mhz   -> T = 2us
#define TFull 50
#define THalf TFull / 2

//Định nghĩa macro: là các hàm bằng cách định nghĩa thông qua definr
//Đây ta sẽ macro in/out pin
#define SCK_OUTPUT  DDRD |=  (1 << DDD4)
#define MOSI_OUTPUT DDRD |=  (1 << DDD5)
#define SS_OUTPUT   DDRD |=  (1 << DDD7)
#define MISO_INPUT  DDRD &= ~(1 << DDD6)

//Macro ghi dữ liệu mức cao hay thấp
//#define MOSI_HIGH_PORTD PORTD |= (1 << PD5)   //digitalWrite(MOSI_PIN, HIGH)
//#define MOSI_LOW_PORTD  PORTD &= ~(1 << PD5)  //digitalWrite(MOSI_PIN, LOW)
//Để gọn hơn ta sẽ dùng tam đẳng thức
//a = value ? x : y; //if(value == true(!= 0))  a = x; else a = y;
//Nếu x là high thì PORTD |= (1 << PD5)
//         low       PORTD &= ~(1 << PD5)
#define write_MOSI(x)  ((x) ? (PORTD |= (1 << PD5)) : (PORTD &= ~(1 << PD5)))
#define write_SS(x)    ((x) ? (PORTD |= (1 << PD7)) : (PORTD &= ~(1 << PD7)))
#define write_SCK(x)   ((x) ? (PORTD |= (1 << PD4)) : (PORTD &= ~(1 << PD4)))

//Macro đọc dữ liệu
//Nếu nhận về từ chân d6 là 1 thì đọc dữ liệu sẽ trả về là high
//                           0                             low
#define read_MISO()   ((PIND & (1 << PIND6)) ? HIGH : LOW)  //digitalRead(MISO, PIN)

void setup() {
  SPI_config();
  Serial.begin(9600);
}

void loop() {
  uint8_t rev[3];
  SPI_start();
  rev[0] = SPI_transfer('a');  //0x61 97 gửi kí tự dùng ''
  rev[1] = SPI_transfer('b');
  rev[2] = SPI_transfer('c');
  SPI_end();
  //Serial.println(rev);
  Serial.println("M: " + String((char)rev[0]));
  Serial.println("M: " + String((char)rev[1]));
  Serial.println("M: " + String((char)rev[2]));
  delay(1000);
}

//Initial IN MODE 0
//CPOL =0 | CPHASE = 0
//Ban đâu tín hiệu chân SS = HIGH và SCK = LOW
void SPI_config(void) {
  MOSI_OUTPUT;
  MISO_INPUT;  
  write_SCK(LOW);
  SCK_OUTPUT;
  //NÊN WRITE MOSI HIGH TRƯỚC
  //ĐỂ TRÁNH KHI VỪA CẤU HÌNH CHÂN LÀ NGÕ RA CÓ THỂ LÀ MỨC THẤP
  write_SS(HIGH);
  SS_OUTPUT;
  delay(1); //May dont need
}

//START SPI
//Muốn bắt đầu SPI chỉ cần kéo chân SS về mức thấp sau
//khi đã cấu hình các chân
void SPI_start(void) {
  write_SS(LOW);  //có thể thay đổi khi: 1 MASTER - MANY SLAVE
}

//Đưa về trạng thái ban đầu (lý tưởng)
//Lúc này SCK sẽ về LOW và SS sẽ lên mức HIGH
void SPI_end(void) {
  write_SCK(LOW);
  write_SS(HIGH);
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
  //Việc lặp lại 8 lần là vì cần 8 xung để
  //có thể truyền/nhận đủ 8bit trong 1 chu kì nhận/gửi

  for (ibit = 0x80; ibit > 0; ibit = ibit >> 1) {  //ibit = 1000 0000
    reslt = byte_out & ibit;                       //(!= 0 -- true) or (== 0 -- false)
    write_MOSI(reslt);
    //Vì khi data ổn định ở nữa chu kì thì SPI sẽ đọc giá trị từ thanh ghi
    //Hoặc gửi giá trị đó đến với slave
    delayMicroseconds(THalf);
    write_SCK(HIGH);

    if (read_MISO() == HIGH)
      byte_in = byte_in | ibit;  //0000 0000 | 1000 0000 = 1000 0000
    //Không cần else vì ban đầu mặc địng byte_in = 0 (0000 0000)
    //Ta chỉ cần thay đổi ở bit nào thay đổi (lên HIGH)
    //else
    //  byte_in = byte_in & ~(ibit);
    delayMicroseconds(THalf);
    write_SCK(LOW);  //End 1 clock cycle
  }
  return byte_in;
}

// int main(void) { 
//   SPI_config();
//   //SPI_start(); 
//   while (1) {
//     uint8_t rev[3];
//     SPI_start();
//     rev[0] = SPI_transfer('a');  //0x61 97 gửi kí tự dùng ''
//     rev[1] = SPI_transfer('b');
//     rev[2] = SPI_transfer('c');
//     SPI_end();
//     //Serial.println(rev);
//     Serial.println("M: " + String((char)rev[0]));
//     Serial.println("M: " + String((char)rev[1]));
//     Serial.println("M: " + String((char)rev[2]));
//     delay(1000);
//   }
//   return 0;
// }


