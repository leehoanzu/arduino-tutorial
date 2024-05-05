/*=========================================================================================
  - Đây là chương trình hiển thị sáng tắt đèn led thông qua câu lệnh. Giao tiếp bằng kết nối
    UART.
  - Màn hình sẽ hiển thị và gõ "ON" để đèn sáng, "OFF" đèn sẽ tắt
=========================================================================================*/

int ledPin = 13; // chân sáng đèn led
String cmd = ""; //lưu lệnh goc từ PC sang, chứa được kí tự, kiểu dữ liệu String

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT); //chân 13 nhận điện
  //pair thiết bị
  Serial.begin(9600);
  //mới mở board, điện tắt, đến khi mình gõ lệnh
  digitalWrite(ledPin, LOW);
}

void loop() {
  //kiểm tra bufer, vùng ram trên board có data gì ko? có thì đọc và kiểm tra cần làm gì?
  if (Serial.available()) {
    cmd = Serial.readStringUntil('\n'); //đọc hết cả câu gõ vào, vì gõ vào xong phải enter
    if (cmd == "ON") {
      digitalWrite(ledPin, HIGH);
      Serial.println("LED is on on"); //báo ngược trở lại PC rằng board đã on đèn
    }
    else if (cmd == "OFF") {
      digitalWrite(ledPin, LOW);
      Serial.println("LED is off off"); //báo ngược trở lại PC rằng board đã off đèn
    }
    else {
      Serial.println("Please type correct uppercase word!");
    }
  } 
  
}
