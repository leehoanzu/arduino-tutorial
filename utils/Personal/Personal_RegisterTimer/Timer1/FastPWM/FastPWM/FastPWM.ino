/*------------------------------------------------------------------------------------------------
- Đây là chương trình tạo fast pwm trên chân oc1 (PB1) tự động
- Phase delay có các loại được chia theo TOP 8bit, 9 bit, 10 bit
- Datasheet/109 bảng 15-5
- lấy giá trị trên thanh ghi OCR01 để so sánh với sóng tam giác
- Ở chương trình này ta mô phỏng điều chế phase pwm bằng độ sáng của đèn
- nên có delay nếu không hoạt động diễn ra nhanh và khó thấy
------------------------------------------------------------------------------------------------*/

#include <avr/io.h>
#include <avr/interrupt.h>

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


//hàm tạo xung fast pwm
void t1FastPWM() {
  TCCR1A = (1 << WGM11) | (1 << WGM10) | (1 << COM1A1); //fast pwm, 10bit, top = 0x03ff, non-inverted
  TCCR1B = (1 << WGM12) | (1 << CS12) ;//pre là 256  
 // OCR1A = 511; //đặt giá trị tham chiếu là 511 so với sóng tam giác để D = 50%
}

int main (void) {
  usart_init();
  DDRB |= (1<<1); //tạo chân b1 làm ngõ ra OC1
  t1FastPWM();
  while (1) {
    // for (int i = 0; i < 1023; i ++) {
    //   OCR1A = i; //ĐẶT GIÁ TRỊ THAM CHIẾU TĂNG DẦN => DUTY CÀNG LỚN
    //               //Đèn sẽ sáng dần
    //   _delay_ms(10); 
    // }
    // for (int i = 1023; i > 0; i--) {
    //   OCR1A = i; //Đèn sẽ tối dần
    //   _delay_ms(10);
    // }
  }
  return 0;
}



