/*-------------------------------------------------------------------
  - Đây là bài GPIO cơ bản
  - Thực hiện cấu hình các chân dưới dạng DDR PORT & PIN
  - DDR là cấu hình cho phép chân là ngõ vào hoặc ngõ ra
  - PORT là đọc và cũng có thể ghi giá trị lên các chân
  - PIN là chỉ đọc giá trị trên chân
--------------------------------------------------------------------*/

#include <avr/io.h>
#include <util/delay.h>

//Ở bài này ta sẽ thí nghiệm trên Port B
int main (void) {
  //cấu hình nhấp nháy trên chân PINB3 (chân 11 trên Arduino)
  //B1: cấu hình chân PINB3 là ngõ ra
  DDRB |= (1 << PINB3);
  //B2 set ngõ ra là 1 trong 1s
  //              là 9 trong 1s
  while(1) {
    //Set ngõ ra là 1 (tương ứng với sáng đèn)
    PORTB |= (1 << 3);
    //Delay 1s
    _delay_ms(1000);
    //Set ngõ ra là 0 (tương ứng với tắt đèn)
    PORTB &= ~(1 << 3);
    _delay_ms(1000);
  }
  return 0;
}