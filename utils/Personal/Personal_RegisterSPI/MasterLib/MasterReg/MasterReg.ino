/*--------------------------------------------------------------------------------------------------------------
- Đây là bài mô phỏng hoạt động của SPI không sử dụng các chân có sẵn trên Arduino Rev3
- Tại đây ta sẽ mô phỏng bằng các chân D4 : D7 để tạo ra các dạng sóng
-                             Theo thứ tự: SCK...MOSI...MISO...SS
- Trong bài này ta sẽ mô phỏng bằng proteus do đó các arduino chỉ hiểu được và đọc các hàm setup và loop
- Ta không thể sử dụng hàm main như những bài trước hay mô phỏng proteus này
- Ở bài này có rất nhiều chú ý đáng quan trọng nên làm hay thật cẩn thận
- Đây là thư viện cho Master nên ta sẽ chủ động cài đặt các chân ngõ ra được phân tích theo dạng sóng với CPHA = 0 | CPOL = 0
--------------------------------------------------------------------------------------------------------------*/

//Khai báo thư viện
#include <avr/io.h>
#include <avr/interrupt.h>

//Định nghĩa các chân
#define SCK_PIN   5
#define MOSI_PIN  3
#define MISO_PIN  4
#define SS_PIN    2

//Macro i/o các chân
#define SCK_Output    DDRB |= (1 << SCK_PIN)
#define MOSI_Output   DDRB |= (1 << MOSI_PIN)
#define SS_Output     DDRB |= (1 << SS_PIN)
#define MISO_Input    DDRB &= ~(1 << MISO_PIN)

#define write_SS(x)   ((x) ? (PORTB |= 1 << SS_PIN) : (PORTB &= ~(1 << SS_PIN)))

void setup() {
  // put your setup code here, to run once:
  configMaster();   
  Serial.begin(9600);
}

void loop() {
  startSPI();
  char data = transferData('G'); //Gửi cho slave kí tự G 47
  Serial.println(data);
  write_SS(HIGH);
  SPSR = (1 << SPIF); //Xóa cờ 
  delay(1000);
}

void configMaster(void) {
  SCK_Output;
  MOSI_Output;
  SS_Output;
  MISO_Input;
}

void startSPI() {
  //Set master with f = fosc/64
  SPCR = (1 << SPE) | (1 << MSTR) | (1 <<SPR1);
  write_SS(LOW);
}

uint8_t transferData(char data) {
  uint8_t byte_in;
  SPDR = data;
  while(!(SPSR & (1<<SPIF)));
  return ((char)SPDR);
}
