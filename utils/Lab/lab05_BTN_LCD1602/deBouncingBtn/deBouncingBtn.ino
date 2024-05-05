/*=================================================================================
  - Chương trình nút nhấn và hiển thị lên màn hình LCD
  - Lúc này đã được chống dội, nên khi tác động quá nhanh sẽ không còn thực thi
    cho tới khi nhấn đủ lâu thì mới được làm việc tiếp theo
===================================================================================*/

#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define pinBtn  12
#define LCD_ADDRESS 0x26
#define LCD_COLUMNS 16
#define LCD_ROWS 2

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

void setup() {
  Wire.begin();

  lcd.init();
  lcd.backlight();
  pinMode(pinBtn, INPUT);
  digitalWrite(12, HIGH); //keo dien tro len
  lcd.setCursor(3, 0);
  lcd.print("Your num is: ");
}

int num = 0;
void loop() {
  //int readPin = digitalRead(pinBtn);
  checkPin();
}

void checkPin () {
  if (digitalRead(pinBtn) == LOW) {
    delay(200);
    if(digitalRead(pinBtn) == LOW) {      
      num++;
      if (num == 11)
        num = 0;
      lcd.setCursor(3, 1);
      lcd.print(num);
    }
    while(!digitalRead(pinBtn));
  }
}
