//mobilhanem deki ultrasonik mesafe ölçer örneği üzerinden oluşturulmuştur https://www.mobilhanem.com/arduino-ultrasonic-mesafe-sensoru-hc-sr04/ 
// alperian 31.03.2020 

#include <Servo.h>
#define Servopin 6
Servo myservo;
byte trigger = 10; // Sensörün Trigger bacağının bağlı olduğu pin
byte echo = 11; // Sensörün Echo bacağının bağlı olduğu pin

unsigned long sure; // Echo bacağının kac mikro saniyede aktif olduğunu saklayacak olan değişken
double toplamYol;
int aradakiMesafe;
int servoSay = 0;
int bekleSay = 0;
bool alarm = false;
void setup() {
  pinMode(trigger, OUTPUT); // Sensörün Trigger bacağına gerilim uygulayabilmemiz için OUTPUT yapıyoruz.
  pinMode(echo, INPUT); // Sensörün Echo bacağındaki gerilimi okuyabilmemiz için INPUT yapıyoruz.
  Serial.begin(74880);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  myservo.attach(Servopin);
  myservo.write(0);
  delay(1000);
}
void loop()
{
  /* Başlangıçta LOW durumda olan trigger bacağına gerilim uygulayıp ardından gerilimi keserek bir ses dalgası
    oluşturmuş oluyoruz. Bu işlem arasında 10 mikro saniye beklenmenin sebebi HC-SR04'ün en az 10 mikro saniyelik
    dalgalar ile çalışmasıdır. */
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);

  // Dalga üretildikten sonra geri yansıyıp Echo bacağının HIGH duruma geçireceği süreyi pulseIn fonksiyonu ile
  // kaydediyoruz.
  sure = pulseIn(echo, HIGH);

  // Aşağıda yapılan süre-mesafe dönüşüm işlemleri yazımızda açıklanmıştır.
  toplamYol = (double)sure * 0.034;
  aradakiMesafe = toplamYol / 2;

  Serial.println(String(aradakiMesafe));
  if (alarm )
  {
    alarm = false;
    myservo.write(0);//sprayleme işlemi için servo yu başlangıç konumuna geri alıyoruz.
    delay(450);//servo nun konumlanması için bekliyoruz.
  }
  if (aradakiMesafe <= 10) {//ne kadar yakınken spray i çeksin.
    servoSay++;
    bekleSay = 0;
    if (servoSay > 1)
    {      //servoSay=0;
      digitalWrite(LED_BUILTIN, HIGH);
      alarm = true;
      myservo.write(180);//Servonun ne kadar açmasını istiyorsanız parantez içindeki değeri değiştirin. aşağıdaki delay daki değeri aynı oranla küçültebilirsiniz.
      delay(350);//servo konumunu alsın diye biraz zaman tanıyalım
    }
  }
  if (servoSay > 0 && aradakiMesafe > 15) //ne kadar uzaktayken sprayleme işlemi bitsin
  {
    bekleSay++;
    servoSay = 0;
    if (bekleSay > 2)
    {
      bekleSay = 0;
      digitalWrite(LED_BUILTIN, LOW);
      delay(200);
    }
  }
  delay(100);
}
