/*------------------------------------------------------------------------------------------------
- Đây là chương trình tạo phase pwm trên chân oc1 (PB1) tự động
- Phase delay có các loại được chia theo TOP 8bit, 9 bit, 10 bit
- Datasheet/109 bảng 15-5
- lấy giá trị trên thanh ghi OCR01 để so sánh với sóng tam giác
- Ở chương trình này ta mô phỏng điều chế phase pwm bằng độ sáng của đèn
- nên có delay nếu không hoạt động diễn ra nhanh và khó thấy
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


//hàm tạo xung pwm tam giác
void t1PhasePWM() {
  TCCR1A = (1 << WGM10) | (1 << COM1A1); //phase pwm, 8bit, top = 0x00ff
  TCCR1B = (1 << CS12) ;//pre là 256  
  //OCR1A = 230; //đặt giá trị tham chiếu là 230 so với sóng tam giác
}

int main (void) {
  usart_init();
  DDRB |= (1<<1); //tạo chân b1 làm ngõ ra 0c1
  t1PhasePWM();
  while (1) {
    for (int i = 0; i < 255; i ++) {
      OCR1A = i; //ĐẶT GIÁ TRỊ THAM CHIẾU TĂNG DẦN => DUTY CÀNG LỚN
                  //Đèn sẽ sáng dần
      _delay_ms(10); 
    }
    for (int i = 255; i > 0; i--) {
      OCR1A = i; //Đèn sẽ tối dần
      _delay_ms(10);
    }

  }
  return 0;
}



