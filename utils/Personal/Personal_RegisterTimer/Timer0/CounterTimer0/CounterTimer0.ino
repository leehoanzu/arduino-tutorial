/*--------------------------------------------------------------------------------------------------------------
- Đây là bài mô phỏng code về sử dụng timer 0 thành biến đếm counter 0
- Trong bài này không nên sử dụng delay bằng tiemr 0 như trước đó vì điều đó 
- gây ra xung đột khi đọc dữ liệu từ thanh ghi điều khiển timer 0
- Ta sẽ đếm giá trị xung kích cạnh xuống từ chân t0 hoặc t1 tùy vào timer 0 hay 1
- Ở đây ta sử dụng timer 0 nên chọn xung kích cạnh xuống timer 0 nên chọn chân t0 (ghi chú trong datasheet/87)
- Trong schematic thì chân t0 là chân D4 trên arduino (PD4)
- Kết hợp với Uart để hiển thị lên Serial monitor để trực quan
--------------------------------------------------------------------------------------------------------------*/

//Khai báo thư viện
#include <avr/io.h>
#include <avr/interrupt.h>

#define BAUD 9600 //khai báo số baud truyền uart để hiển thị lên màn hình
unsigned int count = 0; //Khởi tạo biến đếm cục bộ

//Khởi tạo thông số UART
void usart_init(void) {
  UCSR0B = (1 << TXEN0); 
  UCSR0C = ((1 << UCSZ01) | (1 << UCSZ00));
  UBRR0L = 0X67; // chỉ có thể hoạt động ở 16MHz
}

//CHỉ nhận được 1 kí tự lần lượt vì chỉ có 8 bit nhận/ truyền dữ liệu
void USART_Transmit(unsigned char data) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

//Không thể sử dụng delay bằng timer 0
//Vì sẽ gây ra xung đột với biến đếm trước đó

// void t0CTCDelay() {
//   // Tính toán giá trị OCR0A để có thời gian trễ chính xác là 5ms
//   OCR0A = (F_CPU / (1024UL * 200UL)) - 1; // 200 là số xung tương ứng với 5ms
//   TCNT0 = 0; // Đặt giá trị timer đếm về 0
//   TCCR0A = (1 << WGM01); // Chọn chế độ CTC
//   TCCR0B = (1 << CS02) | (1 << CS00); // Chọn prescaler là 1024
//   while (!(TIFR0 & (1 << OCF0A))); // Chờ cho đến khi đếm đủ số xung tương ứng với 5ms
//   TIFR0 = (1 << OCF0A); // Xóa cờ ngắt
//   TCCR0A = 0; // Tắt chế độ CTC
// }

void initT0Counter() {
  TCNT0 = 0; // Đặt giá trị timer đếm về 0
  TCCR0B = (1 << CS02) | (1 << CS01) |(1 << CS00);
}

int main(void) {
  usart_init();

  //initT0Counter();
  TCCR0B = (1 << CS02) | (1 << CS01) |(1 << CS00);  // Chọn đếm giá trị từ xung ngoài

  DDRB |= (1<<3); //tạo chân b3 làm ngõ ra để quan sát tín hiệu sóng
  
  //dat gia tri ban dau
  TCNT0 = 240; //kiem chung nhanh nên để là 240
  
  // Bước 1: Đặt chân PD4 là đầu vào
  DDRD &= ~(1 << PD4);
  // Bước 2: Bật điện trở kéo lên cho PD4
  PORTD |= (1 << PD4);
  
  while (1) {
    //while((PIND & (1 << PD4)));
    if (!(PIND & (1 << PD4))) {
      // Nút nhấn được nhấn
      delayMicroseconds(30);
      if (!(PIND & (1 << PD4))) {
        // Nút nhấn vẫn được nhấn
        delayMicroseconds(20);
        PORTB = PORTB ^ (1 << 3); //đảo tín hiệu ngõ ra 1s
        count = TCNT0; //đọc giá trị counter 0
        USART_Transmit('0' + (count / 100)); // Hiển thị hàng trăm
        USART_Transmit('0' + ((count % 100) / 10)); // Hiển thị hàng chục
        USART_Transmit('0' + (count % 10)); // Hiển thị hàng đơn vị
        USART_Transmit('\n');
      }
      while(!(PIND & (1 << PD4)));
    }
    
  }  
  return 0;
}
