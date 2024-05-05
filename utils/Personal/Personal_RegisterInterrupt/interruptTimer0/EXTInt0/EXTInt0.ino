/* ---------------------------------------------------------------------------------
- Đây là chương trình ngắt dùng ngắt ngoài Chân INT 0
- Sử dụng việc này ứng dụng cho cảm biến hoặc phát hiện đồ vật
- Ở chương trình nay mô phỏng theo kiểu nút nhấn
- Để thay thế cho cảm biến
------------------------------------------------------------------------------------*/

//Khai báo thư viện
#include <avr/io.h>
#include <avr/interrupt.h>

//Chương trình ngắt nút nhấn 1
//Chú ý vector ngắt cho đúng
//Tra vector trong avr hoặc tên của tên source + _vect
//Ex: tên source là ADC => vector là: ADC_vect (định nghĩa trong <avr/interrupt.h>)
//INT: pinD2
ISR(INT0_vect) {  
  _delay_ms(20);
  if (bit_is_clear(PIND, PD2)) {
    _delay_ms(20);
  }
  PORTB ^= (1 << PINB3);
}

//Cài đặt ngắt ngoài
void initExtInt0 () {
  //Ngắt toàn cục
  SREG |= (1 << 7); 
  //Chọn chế đột ngắt INT0
  EIMSK = (1 << INT0);
  //Chọn mode ngắt ngoài mong muốn
  //Ngắt ngoài tín hiệu cạnh xuống
  //Nghĩa là không nhấn thì ở mức cao, sau khi nhấn xuống mức thấp
  //Và thực hiện ngắt
  EICRA = (1 << ISC01);
}

int main (void) {
  DDRB |= (1 << 3) ; //Chọn chân pinB3 làm ngõ ra
  DDRD &= ~(1 << 2); //Chọn chân pinD2 làm ngõ vào tin hiệu
  PORTD |= (1 << 2); //Bật điện trở kéo lên, tránh gây tổn hại 
  initExtInt0();
  while(1){
    //do not thing
  }
  return 0;
}



