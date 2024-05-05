/*========================================================================
  - Đây là chương trình chạy sáng tắt dần led tự động
  - Đèn sẽ sáng dần lên tới cực đại, sau đó sẽ tắt dần 
  - Ở chương trình này ta dùng chân số 11 vì đây là chân pwm 
========================================================================*/

int ledPin = 11; //chân này điện áp dao động từ 0 đến 5 v
                  //khi mình đẩy vào nó 1 con số + từ 0...255
                  //256 = 2^8
                  //mình đẩy liên tục các con sô 0...255 vào chân 11
                  //điện sẽ trhay đổi 0, 0..., 4.9999..., 5V
                  //bóng đèn sẽ sáng lên từ từ

void setup() {
}

void loop() {
  // Đèn sẽ sáng dần lên, rồi tắt dần
  for (int bar = 0; bar <= 255; bar+= 5) {
    analogWrite(ledPin, bar);
    delay(30);
  } //lệnh này chay 256 lần

  delay(100);

  for (int bar = 255; bar > 0; bar-= 5) {
    analogWrite(ledPin, bar);
    delay(30);
  } //lệnh này chay 256 lần

  delay(100);
}
