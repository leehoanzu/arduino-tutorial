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
#define SCK_Input    DDRB &= ~(1 << SCK_PIN)
#define MOSI_Input   DDRB &= ~(1 << MOSI_PIN)
#define SS_Input     DDRB &= ~(1 << SS_PIN)
#define MISO_Output  DDRB |= (1 << MISO_PIN)

//Macro input
#define read_SS()   PINB = ((PINB & (1 << SS_PIN)) ? HIGH : LOW)

void setup() {
  // put your setup code here, to run once:
  configSlave();   
  Serial.begin(9600);
}

void loop() {
  startSlave();
  while (read_SS() == HIGH); //Phải chờ tín hiệu
  //Nếu không gây ghi đè và mất dữ liệu
  char data = transferData('V'); //56
  Serial.println(data);
  //SPSR = (1 << SPIF); //Xóa cờ 
  delay(100);
}

void configSlave(void) {
  SCK_Input;
  MOSI_Input;
  SS_Input;
  MISO_Output;
}

void startSlave() {
    //Set slave with f = fosc/64
  SPCR = (1 << SPE);
}

char transferData(char data) {
  SPDR = data; 
  while(!(SPSR & (1<<SPIF))); 
  return SPDR;
}
