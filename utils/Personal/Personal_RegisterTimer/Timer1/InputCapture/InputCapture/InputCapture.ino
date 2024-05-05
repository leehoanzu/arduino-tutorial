/*------------------------------------------------------------------------------------------------
-Đây là chương trình input Capture
-Nhận tín hiệu từ chân ICP1 (PB1) 
-có thể là xung kích cạnh lên hoặc cạnh xuống
-Ở Arduino Uno Rev3 chỉ timer 1 có input capture
-thì sẽ lấy dữ liệu của thanh ghi TCNT1 
-Đưa vào ICR1 và hiển thi
------------------------------------------------------------------------------------------------*/

#include <avr/io.h>
#include <avr/interrupt.h>

// #define F_CPU 8000000UL

#define BAUD 9600
uint16_t temp;

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

void t1Capture() {
  TCCR1B = (1 << ICES1) | (1 << CS10);			/* capture on rising edge */ 
  
  while((TIFR1 & (1 << ICF1)) == 0); //chờ ngắt xong
  TIFR1 = (1 << ICF1); //xóa cờ ngắt
}


int main (void) {
  usart_init();
  DDRB &= ~(1 << 0); // Đặt PB0 thành ngõ vào
  DDRB |= (1 << 3); // Đặt PB3 thành ngõ vào
  // while (1) {
  //   USART_Transmit('v');
  //   t1PWMCTCMode_1s();
  //   //t1Delay();
  //   USART_Transmit('\n');
  //   t1PWMCTCMode_05s();
  // }
  TIFR1 = (1<<ICF1);
  TCCR1B = (0 << ICES1) | (1 << CS10);			/* capture on rising edge */ 
  while(1) {
    while((TIFR1 & (1 << ICF1)) == 0); //chờ ngắt xong
    temp = (uint16_t)ICR1;
    // USART_Transmit('0' + (temp % 10));
    // USART_Transmit('0' + (temp /10 % 10));
    // USART_Transmit('0' + (temp /100 % 10));
    // USART_Transmit('0' + (temp /1000 % 10));
    // USART_Transmit('0' + (temp /1000 /10 % 10));
    // USART_Transmit('0' + (temp /100000 % 10));
    char tempStr[10];
    snprintf(tempStr, 10, "%d\n", temp);
    for (int i = 0; tempStr[i] != '\0'; i++) {
      USART_Transmit(tempStr[i]);
  }
    USART_Transmit('\n');
    PORTB ^= (1 << PB3); // Toggle PB3
    TIFR1 = (1 << ICF1); //xóa cờ ngắt
    // while((TIFR1 & (1 << ICF1)) == 0); //chờ ngắt xong
    // temp = ICR1;
    // USART_Transmit('0' + (temp % 10));
    // USART_Transmit('0' + (temp /10 % 10));
    // //USART_Transmit('0' + (temp /100));
    // USART_Transmit('\n');
    // PORTB ^= (1 << PB3); // Toggle PB3
    // TIFR1 = (1<<ICF1);  
  }

  return 0;
}



