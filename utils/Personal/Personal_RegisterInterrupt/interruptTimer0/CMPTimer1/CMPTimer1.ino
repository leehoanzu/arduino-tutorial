/* ---------------------------------------------------------------------------------
- Đây là chương trình ngắt dùng timer 1
- Sử dụng đếm timer 1 compare với giá trị đặt trước ở OCR0A
- Khi tràn sẽ ngắt và điều khiển ngõ ra pinB3 đảo
- Ở bài này ta sử dụng timer 1 ngắt đảo ngõ ra 30ms dùng chia trước 1024
- Vậy 1 xung cần 64us => 30ms cần 470 xung ta sẽ chọn OCRA = 470;
------------------------------------------------------------------------------------*/

//Khai báo thư viện
#include <avr/io.h>
#include <avr/interrupt.h>

//Chương trình ngắt timer 1
ISR(TIMER1_COMPA_vect) {
  //Đảo chân ngõ ra pinB3
  PORTB ^= (1 << PINB3);
}

//Cài đặt ngắt tràn timer 0
void initCmpT1 () {
  //Cho phép ngắt toàn cục
  SREG |= (1 << 7);
  //Chọn pre = 1024, chế độ ctc 1A
  TCCR1B = (1<< CS12) | (1 << CS10) | (1 << WGM12);
  //Toggle ngõ ra OC1, kết hợp cả 2 chân ngõ ra
  TCCR1A = (1 << COM1A0); 
  //Cài đặt ngõ ra so sánh
  OCR1A = 470;
  //Cài đặt cho phép ngắt CTC 1
  TIMSK1 = (1 << OCIE1A);
}

int main (void) {
  DDRB |= (1 << 3) | (1 << 1); //Chọn chân pinB3 & b1làm ngõ ra
  initCmpT1();
  while(1){

  }
  return 0;
}



