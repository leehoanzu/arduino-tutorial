/*--------------------------------------------------------------------------------------------------------------
  - Đây là chương trình chuyển dữ liệu qua UART
  - Chương trình này sẽ hiển thị chuỗi dữ liệu có sẵn vô hạn 
    lên màn hình serial monitor
------------------------------------------------------------------------------------------ */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void usart_init(void) {
  UCSR0B = (1 << TXEN0); //cho phép gửi tín hiệu
  UCSR0C = ((1 << UCSZ01) | (1 << UCSZ00)); //chọn gửi dữ liệu là 8 bit không bit chẳn lẻ 
  //là 103 trong 16MHZ
  UBRR0L = 0X67; //
}

void usart_send(unsigned char i) {
  //kiểm tra và thông báo bộ đệm truyền trống đã sẵn sàng nhận dữ liệu
  UDR0 = UCSR0A;
  while(!(UCSR0A & (1 << UDRE0)));

  for(int i = 0; i < 1000; i++){
    //asm("");
  };
  //delay(1000); //sử dụng delya chỉ chạy trong loop 
  UDR0 = i; //hiên thị ra màn hình serial kí tự i
}

int main(void) {
  unsigned char str[] = "The Word is beautiful. ";  //tạo chuỗi muốn hiển thị
  unsigned char i = 0; //kí tự index
  usart_init();
  while(1) {
    usart_send(str[i++]); //gửi kí tự và sau đó tăng chỉ số
    if(str[i] == '\0') { //chạy tới khi gặp kí tự kết thúc mảng
      //khi chạy hết chuỗi mặc định là 0
      i = 0;
      usart_send(0x0A); //xuống dòng 
      usart_send(0x2A); //thêm dấu * ở đầu dòng
    }
  }
  return 0;
}