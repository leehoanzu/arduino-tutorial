/*-------------------------------------------------------------------
  - Đây là bài GPIO cơ bản
  - Thực hiện cấu hình các chân dưới dạng DDR PORT & PIN
  - DDR là cấu hình cho phép chân là ngõ vào hoặc ngõ ra
  - PORT là đọc và cũng có thể ghi giá trị lên các chân
  - PIN là chỉ đọc giá trị trên chân
--------------------------------------------------------------------*/

/*-------------------------------------------------------------------
***LƯU Ý***
- Để đọc gía trị trên chân bằng thanh ghi PIN
- Giá trị mặc ban đầu của PINx là vô định (gọi là * hay N/A)
- Để đọc giá trị trên chân mong muốn ta cần làm như sau
- lấy ví dụ chân PIND5:
- Chọn vị trí
      0010 0000         <- (1 << 5)    BITWISE 
      **** ****         <- (PIND)        AND
      ( 00*0 0000 >> 5) -> * (VỊ TRÍ CẦN TÌM) 
- Vị trí này (*) bằng 0 khi giá trị chân D5 LOW
- Vị trí này (*) bằng 1 khi giá trị chân D5 HIGH
  VD: PIND & (1 << PIND5)) >> PIND5 == 0 khi chân D5 ở mức LOW
- Ta có thể sử dụng hàm trong thư viện để ngắn gọn và nhanh hơn
  VD: bit_is_clear(PIND, PIND5) 
        trong thư viện #include<avr/sfr_defs.h>
-------------------------------------------------------------------*/

#include<avr/io.h>
#include<util/delay.h>

//Hàm chống dội nút nhấn
//Vì khi nhấn sẽ bị lội khoảng 20ms gây ra nhiễu
//Để tránh tình trạng nhiễu gây thay đổi trạng thái liên tục
//Ta sẽ delay và xem xét liệu có thật sự nhấn chưa
void clrNoiseBtn () {
  //Kiểm tra nếu nút nhấn đã nhấn
  if ((PIND & (1 << PIND5)) >> PIND5 == 0) {
    _delay_ms(12);
    if(bit_is_clear(PIND, PIND5)) { //chắc chắn đang nhấn
      _delay_ms(12);
      ///Tiến hành đảo chân ngõ ra trên PINB3
      PORTB ^= (1 << PINB3);
      while(bit_is_clear(PIND, PIND5)); //Nếu còn nhấn thì giữ không thay đổi chương trình
      //Ta có thể kết hợp Watchdog timer để kiểm tra nếu nhấn giữ quá lâu
      //Ở bài này ta giả sử nhấn và thả nên bỏ qua watchdog
    }
  }
}

/************************* Setting UART *************************/
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

//Ở bài này ta sẽ thí nghiệm trên Port B
int main (void) {
  //cấu hình nút nhấn trên PinD3 và ngõ ra trên chân PINB3 (chân 11 trên Arduino)
  //B1: cấu hình chân PINB3 là ngõ ra
  DDRB |= (1 << PINB3);
  //B2: cấu hình chân PIND5 là ngõ vào (chân số 3 trên Arduino)
  DDRD &= ~(1 << PIND5);
  DDRD &= ~(1 << PIND7);
  //Tương đương nhau 
  //Ban đầu DDRD = 0000 01000 sau khi đảo lại thành 1111 1011
  //Nhân chập với các giá trị hiện có
  //DDRD |= (0 << PIND5);
  usart_init(); 
  USART_Transmit('V');
  //Cấu hình điện trở kéo lên bên trong
  //Để an toàn và tránh hiện tượng float (không xác định được giá trị)
  //SET 1 Ở NGÕ VÀO TƯƠNG ỨNG VỚI THIẾT LẬP ĐIỆN TRỞ KÉO LÊN BÊN TRONG
  PORTD |= (1 << PIND5);
  PORTD |= (1 << PIND7);
  //B2 set ngõ ra là 1 trong 1s
  //              là 9 trong 1s
  int val = (PIND & (1 << PIND7)) >> PIND7;
  while(1) {
    val = (PIND & (1 << PIND7)) >> PIND7; //Hien tuong float
    // Đọc trạng thái của chân D7
    if (PIND & (1 << PIND7)) {
        USART_Transmit('val');
        _delay_ms(600);
    } 

    else {
        // Nếu chân D7 đang ở trạng thái thấp
        // Thực hiện các hành động tương ứng ở đây
        USART_Transmit('L');
        _delay_ms(600);
    }
    USART_Transmit(' ');
    //Khi nào nhấn sẽ thay đổi trạng thái trên chân PINB3
    clrNoiseBtn();
    
  }
  return 0;
}