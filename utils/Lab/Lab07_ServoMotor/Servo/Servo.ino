/*==============================================================================
  - Đây là chương trình điều khiển động cơ bằng xung pwm
    hoặc là tự quay luôn phiên (thuận và nghịch)
  - Điều khiển hướng đi của của servo thông qua điều khiển biến trở
  - Có 2 chương trình chính. 
      1 là quay theo biến trở 
      2 là quay theo vòng lặp vô hạn
==============================================================================*/
#include <Servo.h>

Servo myServo; //tạo biến tham chiếu/nói về/refer đến cái động cơ

int servoPin = 9; //chân 8 sẽ dẩy diện ra để ta điều khiển motor
int ptmValue = 0; //value của chân A0 analog điện vào từ 0 - 5V
					//đọc ra thì lại thấy thành 0...1023
					//mạch ADC có sẵn trên Arduino
int ptmPin = A0; //chân A0 nhận điện từ chiết áp	

void setup()
{
  pinMode(servoPin, OUTPUT); //ta chủ động đẩy điện ra chân này
  myServo.attach(servoPin); //gắn động cơ chơi với chân 8
}

void loop() {
	ptmValue = analogRead(ptmPin); //tui muốn value chân A0
  									//0..1023 đó
  //Servo nó lại chỉ chơi với biên độ 0..180
  //ptmValue dựa trên tay vặn của ta có biên độ 0...1023
  //lí thuyết ta phải vặn 1 đoạn nào đó thì mới ứng được 1 dộ Servo
  //Arduino cung cấp cho 1 món đồ, hàm toán học map 2 tập này
  int servoPos = map(ptmValue, 0, 1023, 0 , 180);
	//servoPos lúc này sẽ là từ 0...180 rồi
  myServo.write(servoPos);
  delay(100);
}

//tự quay theo vòng lặp
/*
void loop()
{
  for (int pos = 0; pos <= 180; pos++) {
  	myServo.write(pos);
    delay(50);
  }
  
  for (int pos = 180; pos >= 0; pos--) {
  	myServo.write(pos);
    delay(50);
  }
}
*/
