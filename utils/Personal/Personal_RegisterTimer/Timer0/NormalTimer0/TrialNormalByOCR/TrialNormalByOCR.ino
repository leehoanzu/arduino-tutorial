/*--------------------------------------------------------------------------------------------------------------
  - Đây là chương trình delay 5ms dùng chế độ normal
    Với XTAL = 16MHZ, viết chương trình delay tác động chân đèn led pinB3
  - Xung nhịp sẽ bắt đầu chạy từ con số chỉ định đến 255 (bit cao nhất) thì sẽ có cờ 
  - Ứng dụng này sẽ có các tùy chọn cho việc thời gian delay mong muốn
------------------------------------------------------------------------------------------ */


#include "avr/io.h"

void t0CTCDelay2ms() {
  //Chọn giá trị prescaler
  // => chọn prescaler là 1024
  //Dựa vào datasheet/86 chọn cs02 : cs00 = 1 0 1;  
  TCNT0 = 0; //giá trị timer đếm bắt đầu từ 0
  TCCR0B = (1 << CS02) | (1 << CS00);  //chọn giá trị prescale là 1024
  TCCR0A = (1 << WGM01) | (1 << COM0A1); //Chọn chế độ CTC , CLEAR ON MATCH (XÓA VỀ 0 KHI BẰNG OCR0)
  OCR0A = 0X20; //Chọn đếm 20 xung sẽ được 2ms
  //OCR0A = 250;
  while((TIFR0 & (1 << OCF0A)) == 0); //chờ ngắt xong
  TCCR0B = 0; //ngừng cấp xung
  TIFR0 = (1 << OCF0A); // Xóa cờ ngắt
  //Tại datasheet/86 TOP của việc ngắt này là OCR0A nên việc sử dụng OCRB chỉ để tạo dạng sóng
  //và khó đo được dạng sóng ngõ ra 
}

void t0CTCDelay1ms() {
  //Chọn giá trị prescaler
  // => chọn prescaler là 1024
  //Dựa vào datasheet/86 chọn cs02 : cs00 = 1 0 1;  
  TCNT0 = 0; //giá trị timer đếm bắt đầu từ 0
  TCCR0B = (1 << CS02) | (1 << CS00);  //chọn giá trị prescale là 1024
  TCCR0A = (1 << WGM01) | (1 << COM0A1) | (1 << COM0A0); //Chọn chế độ CTC , SET ON MATCH (SO SÁNH BẰNG GIÁ TRỊ OCR THÌ SET 1)
  OCR0A = 0X10; //Chọn đếm 20 xung sẽ được 1ms
  //OCR0A = 250;
  while((TIFR0 & (1 << OCF0A)) == 0); //chờ ngắt xong
  TCCR0B = 0; //ngừng cấp xung
  TIFR0 = (1 << OCF0A); // Xóa cờ ngắt
  //Tại datasheet/86 TOP của việc ngắt này là OCR0A nên việc sử dụng OCRB chỉ để tạo dạng sóng
  //và khó đo được dạng sóng ngõ ra 
}

void t0CTCDelay5ms() {
  //Chọn giá trị prescaler
  // 5/(256 * 0.0625u) = 312.5 => chọn prescaler là 1024
  //Dựa vào datasheet/86 chọn cs02 : cs00 = 1 0 1;  
  // TIMSK0 = (1 << OCIE0A); DÙNG CHO VIỆC NGẮT
  TCNT0 = 0; //giá trị timer đếm bắt đầu từ 0
  TCCR0B = (1 << CS02) | (1 << CS00);  //chọn giá trị prescale là 1024
  TCCR0A = (1 << WGM01)| (1 << COM0A1); //Chọn chế độ CTC 
  OCR0A = 0X50; //Chọn đếm 78 xung sẽ được 5ms
  //OCR0A = 250;
  while((TIFR0 & (1 << OCF0A)) == 0); //chờ ngắt xong
  TCCR0B = 0; //ngừng cấp xung
  TIFR0 = (1 << OCF0A); // Xóa cờ ngắt
  //Tại datasheet/86 TOP của việc ngắt này là OCR0A nên việc sử dụng OCRB chỉ để tạo dạng sóng
  //và khó đo được dạng sóng ngõ ra 
}


int main ( )
{
	DDRB = DDRB^(1<<3);	
	while (1){
    //mức cao 2ms và mức thấp 1ms
    PORTB = PORTB ^ (1 << 3); //BAN ĐẦU LÀ 1
    t0CTCDelay2ms(); //delay 2ms sau đó set xuống 0 
    PORTB = PORTB ^ (1 << 3);
    t0CTCDelay1ms(); //delay 1ms ở mức thấp sau đó set lên 1
    //đó là quá trình tạo xung pwm với ton = 2ms trong t = 3ms

  }
	return 0;
}



// int main ( )
// {
// 	DDRB = DDRB^(1<<3);	
// 	while (1){
//     PORTB = PORTB ^ (1 << 3); 
//     TCCR0A = (1 << COM0A0);	/* normal mode, clk- no pre-scaling */
//     TCCR0B = (1 << CS00);
//     OCR0A = 0x64;	/* compare value */ //100
//   }
// 	return 0;
// }