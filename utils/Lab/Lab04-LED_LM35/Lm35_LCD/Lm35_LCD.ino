#include <LiquidCrystal_I2C.h>
#include <Wire.h>
/*=============================================================================================
  - Đây là chương trình hiển thị nhiệt độ lên màn hình LCD
  - Ta cần thư viện wire.h để đọc và truyền dữ liệu cho LCD
  - Ở đây ta sử dụng LCD 16x2
=============================================================================================*/
#define LCD_ADDRESS 0x27 // Địa chỉ khởi tạo LCD

// Số cột và số dòng của màn hình LCD
#define LCD_COLUMNS 16
#define LCD_ROWS 2

// Khởi tạo đối tượng LiquidCrystal_I2C với địa chỉ I2C và kích thước màn hình
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

int sensorPin = A0;  //tên chân nhận tín hiệu điện
                      // nhận vào điện từ 0...5V
                      //đọc ra thi là con số 0....1023
                      //mạch ADC trên con vi điều khiển này lo
int sensorValue = 0;                      
float vol = 0.0;

void setup() {
  //chuẩn bị đường truyền với PC/ cổng USB/serial/com
  Wire.begin();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Temperature:");

}

void loop() {
  sensorValue = analogRead(sensorPin);  //0...1023
  //công thức tỉ lệ thuận với Vol, Vol ra T, vì 1 độ ~~~~ 10mV
  //đổi ngược value từ chân thnahf vol
  vol = sensorValue * 5.0 / 1023;
  //5v     -------> 1023
  //vol?   <------- val
  //10mv, 0,01V ứng với 1 độ c
  //        0,o1  -------> 1 độ c
  //          vol --------> ?
  float temp = vol * 100;
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print(" C     ");
  delay(1000);
}
