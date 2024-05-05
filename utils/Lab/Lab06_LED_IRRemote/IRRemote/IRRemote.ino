/*========================================================================
  - Đây là chương trình điều khiển động cơ từ xa
  - Thông qua việc điều khiển qua IR remote, đọc tín hiệu và giả trị
    Sau đó phân tích liệu tín hiệu đọc được có trùng khớp thì quay động cơ.
========================================================================*/
#include <Servo.h>
#include <IRremote.hpp>

//khai báo servo
Servo myServo; //tạo biến tham chiếu/nói về/refer đến cái động cơ

int servoPin = 8; //chân 8 sẽ dẩy diện ra để ta điều khiển motor
int ptmValue = 0;
int ptmPin = A0;


int ledPin = 12;
const int RECV_PIN = 11; // Chân kết nối cảm biến IR
IRrecv irrecv(RECV_PIN);

void setup() {
  pinMode(servoPin, OUTPUT); //ta chủ động đẩy điện ra chân này
  myServo.attach(servoPin);//gắn động cơ chơi với chân 8
  myServo.write(0);;

  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  irrecv.enableIRIn(); // Khởi động cảm biến IR
}

void loop() {
  if (irrecv.decode()) {
    delay(500);
    if (irrecv.decode()) {
      // In mã nhận được lên cổng Serial
      Serial.print("Received IR Code: 0x");
      Serial.println(irrecv.decodedIRData.decodedRawData, HEX);

      // Kiểm tra có nhấn nút POWER hay không
      if (irrecv.decodedIRData.decodedRawData == 0xF708FF00) {
        digitalWrite(ledPin, HIGH); // Bật đèn LED
        myServo.write(180);
        //delay(100);
      } 
      else {
        myServo.write(0);
        //delay(100);
        digitalWrite(ledPin, LOW); // Tắt đèn LED
      }
      delay(1000);
      //while(irrecv.decode());
      irrecv.resume(); // Tiếp tục chờ lệnh IR mới
      
    }
  }
	// ptmValue = analogRead(ptmPin); //tui muốn value chân A0
  // 									//0..1023 đó
  // Servo nó lại chỉ chơi với biên độ 0..180
  // ptmValue dựa trên tay vặn của ta có biên độ 0...1023
  // lí thuyết ta phải vặn 1 đoạn nào đó thì mới ứng được 1 dộ Servo
  // Arduino cung cấp cho 1 món đồ, hàm toán học map 2 tập này
  // int servoPos = map(ptmValue, 0, 1023, 0 , 180);
	// //servoPos lúc này sẽ là từ 0...180 rồi
  // myServo.write(servoPos);
  // delay(100);
}

//https://github.com/Arduino-IRremote/Arduino-IRremote#examples-for-this-library
//https://github.com/Arduino-IRremote/Arduino-IRremote#converting-your-2x-program-to-the-4x-version