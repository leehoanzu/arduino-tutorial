/*===================================================================
  - Đây là chương trình đọc giá trị analog chuyển đổi và hiển thị giá trị digiatl
  - Chương trình mô phỏng biến trở làm thay đổi giá trị analog
    sau đó hiển thị lên màn hình serial 
  - Đây là chuỗi chương trình điều khiển bằng thanh ghi
===================================================================*/

const int channel = 0; // Chân A0

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Giống với 
  // analogReference(DEFAULT);
  ADMUX |= 0x40; // set Vref là AVcc
  // ADMUX |= B11000000; // set Vref là 1.1V ( nguy hiểm vì step size nhỏ ảnh hưởng kết quả) 
  // Là 1 với câu lệnh analogReference(INTERNAL)
  
  // ADMUX |= 0x20; set thêm nếu chỉ muốn đọc giá trị 8 bit  
  // unsigned int val = ADCH; đọc giá trị ở thanh ghi HIGH

  ADMUX |= (channel & 0x07); // Chọn kênh làm ngõ vào và ở đây ta chọn kênh số 0
  ADCSRA |= 0xc7; // chỉ nên set cho phép bit 7 và bit 6 trạng thái lên 1
          // 1100 0111
          // Ta chọn chia trước 128 lần vì xung nội của Arduino là 16Mhz, mà ADC chỉ cần xung
          // dao động từ 50khz -> 200khz. Nên chọn chia trước lớn để phù hợp với ADC
  while(bit_is_set(ADCSRA, ADSC)); // bit_is_set sẽ xem xét nếu ADSC của ACSRA đang lên 1 thì là true
                                  // nếu ADSC xuống 0 sẽ trả về giá trị false
                                  // có nghĩa là chờ cho chuyển đổi hết (chạy xong 13 chu kì đối với chế độ thường)                                                                          
  
  // int có 2 byte <=>  0000 0000  0000 0000 
  //                  15       8  ...4 3210
  // Tương đương các vị trí: từ 0 -> 7 là byte Thấp, 8 ->15 là byte cao 
  unsigned int val = ADCL | (ADCH << 8); // ADCH dời về trái 8 bit là vì
  // ADCH có 8 bit ví dụ : 0000 00001. Vì là bit ở HIGH nên 8 bit này phải nằm ở vị trí từ 8 -> 15
  // vì ban đầu là mặc định luôn nằm ở byte thấp (vị trí từ 0 -> 7) nên cần dời qua 8 bit 
  // ví dụ ADCL: 0000 1001 và ADCH: 0000 0011
  // ADCH << 8: 0000 0011 0000 0000 | ADCL:0000 1001
  // val = 0000 0011   0000 1001
  //        ADCH        ADCL
  Serial.print("Your value is:");
  Serial.println(val);
  delay(10);
}
