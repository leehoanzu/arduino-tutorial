/*--------------------------------------------------------------------------------------------------------------
  - Đây là chương trình chuyển dữ liệu qua UART
  - Chương trình này dừng lại tại việc truyền và hiển thị trên serial monitor
  - Khi đưa câu lệnh thì câu lệnh sẽ được truyền và hiển thị lên màn hình Serial
  - Các bước đơn giản chỉ truyền 1 kí tự.
------------------------------------------------------------------------------------------ */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// #define F_CPU 16000000UL

// unsigned char receivedData;
// // Xử lý ngắt UART khi có dữ liệu nhận được
// ISR(USART_RX_vect) {
//   receivedData = UDR0;
//   UCSR0A &= ~(1 << RXC0);
// }

void usart_init(void) {
  //DDRB = 0XFF;
  UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);  // Bật truyền, nhận, và ngắt nhận
  UCSR0C = ((1 << UCSZ01) | (1 << UCSZ00));              //chọn chế độ truyền dữ liệu là 8 bit
  UBRR0L = 0X67;                                         //chọn thông số baud là 9600
  //sei(); // Cho phép ngắt toàn cục
}

void usart_send(unsigned char i) {
  //kiểm tra và thông báo bộ đệm truyền trống đã sẵn sàng nhận dữ liệu
  while (!(UCSR0A & (1 << UDRE0)));
  // for(int i = 0; i < 1000; i++){
  //   //asm("");
  // };
  //delay(1000); //sử dụng delya chỉ chạy trong loop
  UDR0 = i;  //hiên thị ra màn hình serial kí tự i
}

int main(void) {
  usart_init(); //cài đặt các thông số của thanh ghi UART
  unsigned char str; //kí hiệu lưu sau khi nhận
  //có thể gửi thằng bằng bộ đệm nhận
  // unsigned char j = 0; //kí tự index
  while (1) {
    do {
      while (!(UCSR0A & (1 << RXC0))); //chờ nhận dữ liệu xong
      str = UDR0;
      //gưi ki tu dc
      usart_send(str); //hiển thị dữ liệu lên màn hình serial 
      //nếu ta delay thì sẽ làm dừng hoạt động gây ra mất dữ liệu cần thiết
      //_delay_ms(100);
      // for(int i = 0; i < 10000; i++){
      //   asm("");
      // }
    } while (str != '\n');
    //usart_send(0x2A);  //thêm dấu * ở đầu dòng
    usart_send(0x0A);  //xuống dòng
  }
  return 0;
}

