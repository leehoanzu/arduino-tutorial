/*=====================================================================
  - Chương trình này sẽ làm sáng tắt đèn led bằng nút nhấn
  - Khi nhấn thì đèn sẽ sáng và khi thả ra thì mặc định đèn sẽ tối
  - Nhưng vì ở đây ta không chống dội nên khi nhấn sẽ bị dội
    dẫn đến đèn sáng tắt vài lần trước khi tắt hẳn. 
=====================================================================*/

int btnPin = 10; //chân 10 là chân nút bấm
int ledPin = 13; //nhan diện on-off cái LED
int btnValue = 0; //lưu điện tỏng chân  10 - chân nút bấm, điện trở kéo lên
                  //pull-up value này sẽ phải HIGH default

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  btnValue = digitalRead(btnPin);
  Serial.println(btnValue);  //HIGH default

  if (digitalRead(btnPin) == LOW) {
    delay(200);
    if(digitalRead(btnPin) == LOW) {      
      digitalWrite(ledPin, HIGH);
    }   
  }
  else
    digitalWrite(ledPin, LOW);
}
