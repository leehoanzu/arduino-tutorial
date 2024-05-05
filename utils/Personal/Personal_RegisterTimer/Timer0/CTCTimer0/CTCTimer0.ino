/*--------------------------------------------------------------------------------------------------------------
  - Đây là chương trình delay 5ms dùng chế độ ctc
    Với XTAL = 16MHZ, viết chương trình delay tác động chân đèn led pinB3
    Khác với chế độ normal, ta sẽ tính toán giá trị cụ thể (sô xung tương ứng) 
    là thời gian delay mong muốn
    sau đó timer0 chạy và kiểm tra. Nếu giá trị timer = giá trị tính
    Cờ sẽ ngắt báo hiệu
--------------------------------------------------------------------------------------------------------------*/

#include <avr/io.h>
#include <avr/interrupt.h>

#define BAUD 9600

void usart_init(void) {
  UCSR0B = (1 << TXEN0); 
  UCSR0C = ((1 << UCSZ01) | (1 << UCSZ00));
  UBRR0L = 0X67; // chỉ có thể hoạt động ở 16MHZ
}

void USART_Transmit(unsigned char data) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

void t0CTCDelay() {
  //Chọn giá trị prescaler
  // 5/(256 * 0.0625u) = 312.5 => chọn prescaler là 1024
  //Dựa vào datasheet/86 chọn cs02 : cs00 = 1 0 1;  
  // TIMSK0 = (1 << OCIE0A); DÙNG CHO VIỆC NGẮT
  TCNT0 = 0; //giá trị timer đếm bắt đầu từ 0
  TCCR0B = (1 << CS02) | (1 << CS00);  //chọn giá trị prescale là 1024
  TCCR0A = (1 << WGM01); //Chọn chế độ CTC 
  OCR0A = 0X4e; //Chọn đếm 78 xung sẽ được 5ms
  //OCR0A = 250;
  while((TIFR0 & (1 << OCF0A)) == 0); //chờ ngắt xong
  TCCR0B = 0; //ngừng cấp xung
  TIFR0 = (1 << OCF0A); // Xóa cờ ngắt
  //Tại datasheet/86 TOP của việc ngắt này là OCR0A nên việc sử dụng OCRB chỉ để tạo dạng sóng
  //và khó đo được dạng sóng ngõ ra 
}

//delay kết hợp nút nhấn
void checkBtn() {
  if (!(PIND & (1 << PD4))) {
    // Nút nhấn được nhấn
    t0CTCDelay();
    if (!(PIND & (1 << PD4))) {
      t0CTCDelay();
      // Nút nhấn vẫn được nhấn (hãy kiểm tra thêm để đảm bảo nút vẫn được nhấn)
      USART_Transmit('5');
      USART_Transmit('\n');
    }
    while (!(PIND & (1 << PD4)));
  }
}



int main (void) {
  usart_init();
  TCCR0B = (1 << CS02) | (1 << CS00);  //chọn giá trị prescale là 1024
  TCNT0 = 0;
  DDRB |= (1 << PB2); //tạo chân b3 làm ngõ ra

  while (1) {
    checkBtn();
    t0CTCDelay();
  }
  return 0;
}

// int main (void) {
//   usart_init();
//   TCCR0B = (1 << CS02) | (1 << CS00);  //chọn giá trị prescale là 1024
//   TCNT0 = 0;
//   DDRB |= (1 << PB2); //tạo chân b3 làm ngõ ra

//   while (1) {
//     USART_Transmit('V');
//     // Đảo trạng thái của chân B3
//     PORTB ^= (1 << PB2);
//     // for(int i =  0; i < 62; i++)
//     //   t0CTCDelay(); //delay 0.3s với vòng lặp này
//     USART_Transmit('\n');
//   }
//   return 0;
// }



