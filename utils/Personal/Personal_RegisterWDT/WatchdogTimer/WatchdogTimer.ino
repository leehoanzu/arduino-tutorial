/*------------------------------------------------------------------------------------------------
  - Đây là chương trình Watchdog timer của Arduno rev 3
  - Việc sử dụng watch dog có ý nghĩa bảo vệ phần cứng của bạn
  - Giúp tránh bị nghẽn hệ thống khi có lỗi (vd: bị delay 5s, nhưng chương trình thưc của bạn chỉ có 2s)
  - Do watchgof chạy với xung riêng biệt nên không bị ảnh hưởng bởi delay của chương tỉnh.
  - Lúc này watchdog sẽ can thiệp reset lại hệ thống và chạy lại từ đầu.
  - Ở chương trình này sẽ lấy ví dụ watchdog thời gian là 2s
  - Có nghĩa là nếu chương trình bị delay hơn 2s thì watchdog sẽ chạy lại từ đầu
------------------------------------------------------------------------------------------------*/

/* Khai báo thư viện */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include<avr/wdt.h> 

#define BAUD 9600

/*Ở đây ta sẽ thêm chương trình gửi tín hiệu UART
  Để dễ dàng thấy được chương trình đang chạy lại từ đầu
  Vì tín hiệu UART chỉ được gửi ở đầu chương trình*/

//Cài đặt uart
void usart_init(void) {
  UCSR0B = (1 << TXEN0); //cho phép gửi tín hiệu
  UCSR0C = ((1 << UCSZ01) | (1 << UCSZ00)); //chọn gửi dữ liệu là 8 bit không bit chẳn lẻ 
  //là 103 trong 16MHZ
  UBRR0L = 0X67; // chỉ có thể hoạt động ở 16MHZ
}

//CHỉ nhận được 1 kí tự lần lượt vì chỉ có 8 bit nhận/ truyền dữ liệu
void USART_Transmit(unsigned char data) {
    while (!(UCSR0A & (1 << UDRE0))); //Chờ khi tín hiệu nhận xong
    UDR0 = data;
}

//Hàm để mở WDT
// void WDTEnable (void) {
//   MCUSR &= (1 << WDRF);
//   WDTCSR |= (1 << WDCE) | (1 << WDE);
//   WDTCSR |= (1 << WDE);
// }

//Khởi động watchdog
void onWDT() {
  /*Lưu ý:
  - Để thay đổi được WDT như bật hay tắt (Để tránh hỏng hệ thống)
  - Arduino bắt buộc phải set cả bit WDCE & WDE cùng 1 lúc
  - Và sau 4 chu kì xung bit WDCE sẽ được xóa bởi phần cứng 
  - Nghĩa là luôn mặc định bằng 0 */
  WDTCSR |= (1 << WDCE) | (1 << WDE); 
  //Thay đổi thời gian watchdog
  WDTCSR = (1 << WDE) | (1 << WDP2) | (1 << WDP1) | (1 << WDP0); //set on wdt with 2s time
}

//Tắt watchdog
void offWDT() {
  /*
	This function use for disable the watchdog timer.
	*/
  /*Điều này có nghĩa là WDE luôn được đặt khi WDRF được đặt. 
    Để xóa WDE, WDRF trước tiên phải được xóa WDRF trong MCUSR */
  MCUSR &= ~(1 << WDRF);
  //Thay đổi cài đặt
  WDTCSR = (1<<WDCE) | (1<<WDE); 
  //Xóa WDT
  WDTCSR = 0x00;
}

int main (void) {
  //Mở WDT
  onWDT();
  //Khai báo UART và gửi tín hiệu
  usart_init(); 
  USART_Transmit('V');
  USART_Transmit('\n');
  DDRB |= (1<<3); //tạo chân b1 làm ngõ ra OC1

  //Chương trình sáng 1s và tắt 1s bởi WDT 
  // Bật chân PB1 (OC1A) - trạng thái cao
  PORTB |= (1 << PB3);
  // Đợi 1000ms
  _delay_ms(1000);
  // Tắt chân PB1 (OC1A) - trạng thái thấp
  PORTB &= ~(1 << PB3);
  // _delay_ms(1000);
  // break;
  while(1); //Quá thời gian mà vẫn chưa xóa WDT hệ thống sẽ tự chạy lại
  offWDT();  
  return 0;
}

// 1 version khác khi của wdt khi không sử dụng register
// void setup() {
//   wdt_disable();  /* Disable the watchdog and wait for more than 2 seconds */
//   delay(3000);  /* Done so that the Arduino doesn't keep resetting infinitely in case of wrong configuration */
//   wdt_enable(WDTO_2S);  /* Enable the watchdog with a timeout of 2 seconds */
//   //Khai báo UART và gửi tín hiệu
//   usart_init(); 
//   DDRB |= (1<<3); //tạo chân b1 làm ngõ ra OC1
// }

// void loop() {

//   USART_Transmit('V');
//   USART_Transmit('\n');  

//   // Chương trình sáng 1s và tắt 1s bởi WDT 
//   // Bật chân PB1 (OC1A) - trạng thái cao
//   PORTB |= (1 << PB3);
//   // Đợi 1000ms
//   _delay_ms(2000);
//   // Tắt chân PB1 (OC1A) - trạng thái thấp
//   PORTB &= ~(1 << PB3);
//   // _delay_ms(500);
//   // break;
//   while(1); //Quá thời gian mà vẫn chưa xóa WDT hệ thống sẽ tự chạy lại
//   offWDT(); 
//   while(1); /* Infinite loop. Will cause watchdog timeout and system reset. */
// }
