/*------------------------------------------------------------------------------------------------
  -Đây là chương trình tạo xung pwm trên chân oc1 (PB1)
  -Sử dụng so sánh ở thanh ghi đặt trước dữ liệu OCR1A
  -Nếu timer1 chạy được số xung trùng với giá trị đặt trước thì sẽ set hoặc reset 
  -Từ đó tạo ra xung kích pwm theo cách thủ công
  -Ở bài này, ta sẽ tạo ra xung ton = 1s, toff = 0,5s.
  -Lưu ý hết  sức khi OCR1AH và OCR1AL chỉ có 8 bit và giá trị bit 0:7
  -Nên khi lưu giá trị vào thanh ghi so sánh ta chọn 1 trong hai cách sau
  1. OCR1A = x;
  2. OCR1AH = a;  OCR1AL = b;    (a << 8) + b = x
  vd 1. OCR1A = 0xF423; 
      2.  OCR1AH = (0xF4) ; //62499 giá trị so sánh
          OCR1AL = 0x23; 
          //OCR1A = (OCR1AH << 8) | OCR1AL; // không cần thiết
------------------------------------------------------------------------------------------------*/

#include <avr/io.h>
#include <avr/interrupt.h>

// #define F_CPU 8000000UL

#define BAUD 9600

void usart_init(void) {
  UCSR0B = (1 << TXEN0); //cho phép gửi tín hiệu
  UCSR0C = ((1 << UCSZ01) | (1 << UCSZ00)); //chọn gửi dữ liệu là 8 bit không bit chẳn lẻ 
  //là 103 trong 16MHZ
  UBRR0L = 0X67; // chỉ có thể hoạt động ở 16MHZ
}

void USART_Transmit(unsigned char data) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

void t1Delay() {
  TCNT1 = 3036; //chạy từ 0 -> 65536 giá trị
  TCCR1B = (1 << CS12) ;  
  //chọn pre 256 vì XTAL = 16Mhz => T = 16us
  //muốn delay 1s
  //1/16 = 62500
  //sau khi tính thì cần 62500 xung clock
  //Ngõ ra xung là Toàn bộ xung đủ là 2s nhưng Ton = 1s;
  while((TIFR1 & (1 << TOV1)) == 0); //chờ ngắt xong
  TCCR1B = 0; //ngừng cấp xung
  TIFR1 = (1 << TOV1); //xóa cờ ngắt
}

void t1PWMCTCMode_1s() {
  TCNT1 = 0; //chạy từ 0 -> = giá trị so sánh
  OCR1A = 62500; 
  //OCR1AH = (0xF4 << 8) ; //như vậy gây ra lỗi vì OCR1AH chỉ có 8 bit 0:7
  //Như vậy sẽ trôi bit lúc này OCR1AH = 0xF400 mà chỉ đọc được 8 bit nên còn OCR1AH = 0x00
  OCR1AH = (0xF4) ; //62499 giá trị so sánh
  OCR1AL = 0x23; 
  //OCR1A = (OCR1AH << 8) | OCR1AL; // không cần thiết
  TCCR1A = (1 << COM1A1); //Xóa khi bằng giá trị so sánh (output low)
  TCCR1B = (1 << CS12) | (1 << WGM12);  
  //chọn pre 256 vì XTAL = 16Mhz => T = 16us
  //muốn delay 1s
  //1/16 = 62500
  //sau khi tính thì cần 62500 xung clock
  //Ngõ ra xung là Toàn bộ xung đủ là 2s nhưng Ton = 1s;
  while((TIFR1 & (1 << OCF1A)) == 0); //chờ ngắt xong
  TCCR1B = 0; //ngừng cấp xung
  TIFR1 = (1 << OCF1A); //xóa cờ ngắt
}

void t1PWMCTCMode_05s() {
  //TCNT1 = 0; //chạy từ 0 -> = giá trị so sánh
  OCR1A = 31250;
  OCR1AH = (0x7A ) ; //31249 giá trị so sánh
  OCR1AL = 0x11; 
  //OCR1A = (OCR1AH << 8) | OCR1AL; // Phải công lại
  TCCR1A = (1 << COM1A1) | (1 << COM1A0); //set khi bằng giá trị so sánh (output high)
  TCCR1B = (1 << CS12) | (1 << WGM12);  
  //chọn pre 256 vì XTAL = 16Mhz => T = 16us
  //muốn delay 1s
  //1/16 = 62500
  //sau khi tính thì cần 62500 xung clock
  //65536 - 62500 = 3036
  //Ngõ ra xung là Toàn bộ xung đủ là 2s nhưng Ton = 1s;
  while((TIFR1 & (1 << OCF1A)) == 0); //chờ ngắt xong
  //TCCR1B = 0; //ngừng cấp xung
  TIFR1 = (1 << OCF1A); //xóa cờ ngắt
}

int main (void) {
  usart_init();
  DDRB |= (1<<1); //tạo chân b1 làm ngõ ra 0c1
  
  while (1) {
    USART_Transmit('v');
    t1PWMCTCMode_1s();
    //t1Delay();
    USART_Transmit('\n');
    t1PWMCTCMode_05s();
  }
  return 0;
}



