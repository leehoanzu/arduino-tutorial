/*------------------------------------------------------------------------------------------------
Đây là chương trình delay timer 1 mode normal

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

int main (void) {
  usart_init();
  TCCR1B = (1 << CS12) ;  
  TCNT1 = 3036; 
  DDRB |= (1<<3); //tạo chân b3 làm ngõ ra
  while (1) {
    USART_Transmit('V');
    PORTB = PORTB ^ (1 << 3); //đảo tín hiệu ngõ ra 1s
    //có nghĩa là 1s ở mức HIGH (1) 1s còn lại là mức LOW (2) => T = 2s
    t1Delay(); //delay 1s;
    USART_Transmit('\n');
  }
  return 0;
}



