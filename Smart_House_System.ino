#include <SoftwareSerial.h> 
SoftwareSerial mySerial(6, 7); // RX, TX
#include <LCD5110_Basic.h>  //Lcd ekran için kütüphane tanımladık
// LCD pinleri
// SCK  - Pin 8
// MOSI - Pin 9
// DC   - Pin 10
// RST  - Pin 11
// CS   - Pin 12
//Lcd pinleri haberleşme contrast gibi değerlere komut verebilmek için arduino'ya bağladık
LCD5110 myGLCD(8,9,10,11,12); // Lcd yi tanımşadık
extern uint8_t SmallFont[]; // Yazı fontu-Bu yazı fontu önceden kütüphanede tanımlanan bir sistematik alfabedir
String ses; // gelen karakterlerin satır değişkenine atamayıp onu elimizde tutmamıza yarar

int gelendegertoplam = 0;//başta gelen ses değişkenimize 0'a eşitledik
int led1 = 2; 
int led2 = 3; 
int buzzer = 5; //Devre elemanlarımızı aarduino ile haberleşmesi için sisteme bağladık

void setup() {
  Serial.begin(9600);
mySerial.begin(9600); // serial iletişm hızı
myGLCD.InitLCD(); // lcd ekrana bağlatma fonksiyonu
pinMode(buzzer,OUTPUT);//buzzerımıza output olarak sinyal göndereceğimiz belirttik.

myGLCD.setFont(SmallFont);
 myGLCD.print(ses,LEFT,10);//yazı ayarları
}
void ledyakma() // led yakma görev tanımı için fonksiyon tanımladık
{
 if(ses == "kontrol") {digitalWrite(led1, HIGH), digitalWrite(led2, HIGH); }  //kontrol denirse ledlerin hepsini aç
  else if(ses == "tamam"){digitalWrite(led1, LOW), digitalWrite(led2,LOW),digitalWrite(buzzer, LOW);} //tamam denirse sistemi kapat
  
  //----------Adım Adım Açma komutlar----------// 
  else if(ses == "guvenli") {digitalWrite(led1, HIGH);} 
  else if(ses == "tehlikeli") {digitalWrite(led1, LOW);}
  else if(ses == "alarm") {digitalWrite(led2, HIGH),digitalWrite(buzzer, HIGH),digitalWrite(led1, LOW);}  
  else if(ses == "iptal") {digitalWrite(led2, LOW),digitalWrite(buzzer, LOW);} 
  ses="";//en son komuttan sonra ses değişkenini sıfırladık
}

void loop() {//sistem komple bu denklem içinde dönmektedir

while (mySerial.available()){ //Okumak için kullanılabilir bayt olup olmadığını kontrol et
delay(10); //10 milisaniye bekle
char c = mySerial.read(); //Seri okuma

// türkçe karakterler lcd kütüphanesinde olmadığı için aşağıdaki gibi utf8 deki integer değerleri alınmıştır. 
int a = (int) c; // char tipi karakteri integer karaktere dönüştürme


if(a<0) // a değeri negatif ise
{
  
  if ( a == -60) {  // a değeri -60 ise 
    c = mySerial.read();  // sonraki gelen veriyi oku
    a = (int) c;  // char tipi karakteri integer karaktere dönüştürme
    if( a == -97 ) { // a değeri -97 ise
      ses += 'g';  // ses karakterine g harfini ekle.
      } 
    else { ses += 'i';  // a değeri -97 değil ise ses karakterine i harfini ekle
    }
    a = 0; // a değerini sıfırla
    } 
    
  gelendegertoplam +=a; // gelendegertoplam değerine a değerini ekle
   
  if (gelendegertoplam < 0 ) // gelendegertoplam değeri negatif ise 
    {
       
        
       if ( gelendegertoplam == -157)  { ses += "S";gelendegertoplam =0; }  // Ş harfini S harfine dönüştürme işlemi 
       if ( gelendegertoplam == -156)  { ses += "s";gelendegertoplam =0; }  // ş harfini s harfine dönüştürme işlemi 
       if ( gelendegertoplam == -150)  { ses += "c";gelendegertoplam =0; }  // ç harfini c harfine dönüştürme işlemi 
       if ( gelendegertoplam == -182) {  ses += "C";gelendegertoplam =0; }  // Ç harfini C harfine dönüştürme işlemi 
       if ( gelendegertoplam == -135) {  ses += "o"; gelendegertoplam =0;}  // ö harfini o harfine dönüştürme işlemi 
       if ( gelendegertoplam == -167)  { ses += "O"; gelendegertoplam =0;}  // Ö harfini O harfine dönüştürme işlemi 
       if ( gelendegertoplam == -129)  { ses += "u"; gelendegertoplam =0;}  // u harfini u harfine dönüştürme işlemi 
       if ( gelendegertoplam == -161)  { ses += "U";gelendegertoplam =0;}   // U harfini U harfine dönüştürme işlemi 
      
       //bütün bu işlermleri türk alfabesini ingiliz alfabesine eşitlemek için yaptık
       
      }
  }
  else
  {

  
if (c == '#') {break;} // # tespit edildiğinde döngüden çık

  ses += c; // gelen karakterleri ses tanımında topla
 
    }

}


if (ses.length()>0)  // gelen veri var ise
{
 
Serial.println(ses);
myGLCD.clrScr(); // Ekranı temizle
delay(1000);  // 1 saniye bekle
myGLCD.print(ses.substring(0,12),LEFT,0); // ses karakterinin ilk 12 harfini Sola hizalı ve en üst pikselden  yazıyı konumlandır.
myGLCD.print(ses.substring(12,24),LEFT,10); // ses karakteri 12 harften de büyükse  12. harften sonrasını  Sola hizalı ve 10 piksel üstten aşağı doğru yazıyı konumlandır.
myGLCD.print(ses.substring(24,36),LEFT,20); // ses karakteri 24 harften de  büyükse  24. harften sonrasını  Sola hizalı ve 10 piksel üstten aşağı doğru yazıyı konumlandır.
myGLCD.print(ses.substring(36,48),LEFT,30);  // ses karakteri 36 harften de  büyükse  36. harften sonrasını  Sola hizalı ve 10 piksel üstten aşağı doğru yazıyı konumlandır.
delay(10);  // 10 milisaniye bekle
myGLCD.print("Samim", CENTER, 18);
    myGLCD.print("Örsoğlu", CENTER, 24);
    myGLCD.print("XXXXXXXXXXXXXX", CENTER, 42);
    // Üst taraftaki komutlarda lcd ekranına yazı yazzma sistemini ve düzenin ayarladık
    
ledyakma();//led yakma işlemleri için fonksiyonu döngümüze çağırdık
}

}



//Fonksiyonlar temel olarak döngüye çağırıldıklarında çalışırlar,sistemimize yaptırmak istediğimiz eylemi belirli bir fonksiyon halinde yazıp sistemimize soktuğumuzda istediğimiz belirli işlemleri sistemimizde elde edebilriz
//Led ile bilgilendirme
//Buzzer ile aktif sesli uyarı
//Verilen komutları Lcd ekranını yansıtma
//Projenin genel amacı İşitme engelli bireylerin iletişim konusunda iyileştirmek,Acil durumlar hakkında anlık bilgiler vermek.
