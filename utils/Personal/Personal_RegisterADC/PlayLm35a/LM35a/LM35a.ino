const int analogInPin = 0; // Chọn chân ADC 0 (A0)
int sensorValue = 0;      // Lưu trữ giá trị đọc từ ADC

void setup() {
  // Khởi tạo Serial Monitor để hiển thị giá trị ADC
  Serial.begin(9600);
  // Cấu hình thanh ghi ADMUX để chọn chân ADC
  ADMUX = 0x40; //0100 0000
  //set bit 6 lên 1 để lấy điện áp tham chiếu mặc định là Vcc
  //bit0 - bit3 mà set 0 là kênh 0 hay  (analogInPin & 0x07) vì mình chỉ có 6 chân analog hoi

  // Cấu hình thanh ghi ADCSRA để bật ADC và chọn tốc độ chuyển đổi
  ADCSRA = 0xe7; //0x87
  //1110 0111
  //bit 7 & 6 nên đê mặc dịnh là 1 để kích hoạt, bit 5 k cần cũng dc
  //3 bit  cuôi để set hệ số chia (chia cang lớn thì chu kì xung càng nhiều)
}

void loop() {
  // Chờ cho đến khi chuyển đổi ADC hoàn thành
  //while (bit_is_set(ADCSRA, ADSC)); //chưa biết sử dụng

  //các thanh ghi chỉ cần ghi tên hoa là sử dụng đc

  // Đọc giá trị từ thanh ghi ADC và lưu vào biến sensorValue
  sensorValue = ADC; 
  int digitVal = sensorValue * 0.02;
  // In giá trị lên Serial Monitor
  Serial.print("Giá trị ADC: ");
  Serial.println(sensorValue);
  Serial.print("Giá trị digit: ");
  Serial.println(digitVal);


  // Tạm dừng một khoảng thời gian trước khi đọc giá trị tiếp theo
  delay(1000); // Đợi 1 giây (1000 mili giây)
}