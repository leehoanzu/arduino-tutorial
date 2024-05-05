/*--------------------------------------------------------------------------------------------------------------
  - Đây là chương trình delay 5ms dùng chế độ normal
    Với XTAL = 16MHZ, viết chương trình delay tác động chân đèn led pinB3
  - Xung nhịp sẽ bắt đầu chạy từ con số chỉ định đến 255 (bit cao nhất) thì sẽ có cờ 
------------------------------------------------------------------------------------------ */

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

void t0Delay() {
  TCNT0 = 0; //chạy từ 0 -> 256 giá trị
  TCCR0B = (1 << CS02) | (1 << CS00);  
  //chọn pre 1024 vì XTAL = 16Mhz => T = 64us
  //muốn delay 1s
  //1/64 = 15625
  //sau khi tính thì cần 15625 xung clock
  //Ngõ ra xung là Toàn bộ xung đủ là 2s nhưng Ton = 1s;
  while((TIFR0 & 0x01) == 0); //chờ ngắt xong
  TCCR0B = 0; //ngừng cấp xung
  TIFR0 = 0x01; //xóa cờ ngắt
}

int main (void) {
  usart_init();
  TCCR0B = (1 << CS02) | (1 << CS00); 
  TCNT0 = 0;
  DDRB |= (1<<3); //tạo chân b3 làm ngõ ra
  while (1) {
    USART_Transmit('V');
    PORTB = PORTB ^ (1 << 3); //đảo tín hiệu ngõ ra 1s
    //có nghĩa là 1s ở mức HIGH (1) 1s còn lại là mức LOW (2) => T = 2s
    for(int i = 0; i < 62; i++) 
    //vì cần chạy 15625 xung
    //mà timer 0 chạy 256 giá trị(xung) => cần chạy 15625/256 = 62 
    //chạy 62 lần delay tương đướng 1s
      t0Delay(); //delay 64us

    //USART_Transmit(cycle);
    USART_Transmit('\n');
    // for(int i = 0; i < 62; i++) 
    //   t0Delay();
  }
  return 0;
}



