/*========================================================================
  - Chương trình sử dụng biến trở để điều khiển sáng tắt
  - Biến trở sẽ được đọc ở chân A0 và đem so sánh giá trị mong muốn
  - Nếu lớn hơn giá trị mong muốn thì sáng đèn ở chân 13 và ngược lại
========================================================================*/

int pinA0Value = 0; // Biến này lưu giá trị chân A0, từ 0..1023

#define pinLed  13 // Đặt chân 13 là chân sáng đèn
#define PIN_A0 A0 // Đặt chân biến trở là chân A0

void setup() {
  pinMode(pinLed, OUTPUT);
  Serial.begin(9600); //thiết lập kênh truyền với PC
}

void loop() {
  // Đọc giá trị chân A0 xem là mấu in ra thử cái gì
  pinA0Value = analogRead(PIN_A0);
  // In ra màn hình giá trị đọc được từ chân A0
  Serial.println(pinA0Value);
  // So sánh giá trị đọc được
  // nếu lớn hơn 500 thì đèn sáng và ngược lại
  if (pinA0Value >= 500) 
    digitalWrite(13, HIGH);
  else
    digitalWrite(13, LOW);
}
