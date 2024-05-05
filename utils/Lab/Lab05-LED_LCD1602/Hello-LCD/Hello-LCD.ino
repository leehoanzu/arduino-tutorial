/*==========================================================================
  - Đây là chương trình hiển thị câu chào đơn giản lên màn hình LCD
==========================================================================*/

//sử dụng k cần i2c
#include <LiquidCrystal.h>

// Khai báo chân kết nối màn hình LCD
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // Khởi tạo màn hình LCD
  lcd.begin(16, 2);  // 16 cột và 2 dòng

  // Hiển thị văn bản mặc định
  lcd.print("Hello, Arduino!");
}

void loop() {
  // Không cần làm gì trong loop
}

