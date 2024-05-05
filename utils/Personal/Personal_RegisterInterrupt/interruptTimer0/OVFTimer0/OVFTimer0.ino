/* ---------------------------------------------------------------------------------
- Đây là chương trình ngắt dùng timer 0
- Sử dụng đếm timer 0 normal
- Khi tràn sẽ ngắt và điều khiển ngõ ra pinB3
- Ở bài này ta sử dụng timer 0 ngắt đảo ngõ ra 10ms dùng chia trước 1024
- Vậy 1 xung cần 64us => 10ms cần 156 xung ta sẽ chọn TCTN0 = 256 - 156 = 100
------------------------------------------------------------------------------------*/

//Khai báo thư viện
#include <avr/io.h>
#include <avr/interrupt.h>

//Chương trình ngắt timer 0
ISR(TIMER0_OVF_vect) {
  //Chọn pre = 1024
  TCCR0B = (1 << CS02) | (1 << CS00);
  //Đặt giá trị bắt đầu 
  TCNT0 = 100;
  //Đảo chân ngõ ra
  PORTB ^= (1 << 3);
  _delay_ms(100);
}

//Cài đặt ngắt tràn timer 0
void initINT0 () {
  //Chọn pre = 1024
  TCCR0B = (1 << CS02) | (1 << CS00);
  //Đặt giá trị bắt đầu 
  TCNT0 = 100;
  TIMSK0 = (1 << TOIE0); //Chọn ngất tràn timer 0 
  //Cho phép ngắt toàn cục
  SREG |= (1 << 7);
  //sei(); tương đương hàm trên 
}

int main (void) {
  DDRB |= (1 << 3); //Chọn chân pinB3 làm ngõ ra
  initINT0();

  while(1){
    // Không có gì trong vòng lặp
  }
  return 0;
}

