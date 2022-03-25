//khai báo thư viện cho màn hình led LCD I2C và module thời gian thực DS3231
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Servo.h>

RTC_DS3231 rtc; //tạo biến rtc có kiểu dữ liệu là RTC_DS3231

LiquidCrystal_I2C lcd(0x27, 40, 2); //thiết lập biến lcd có kiểu dữ liệu là LiquidCrystal_I2C. thiết lập địa chỉ LCD là 0x27
                                    //thiết lập loại LCD sử dụng là LCD 40x2

Servo myServo; //tạo một biến myServo có kiểu dữ liệu là Servo

bool temp2 = false;

int BTN1 = 7; //tạo nút nhất cho ăn tại chân D7 của Arduino

int mucNuoc = A2; //tạo biến đọc dữ liệu từ cảm biến mực chất lỏng

int doDuc = A0; //tạo biến đọc dữ liệu từ cảm biến độ đục

int WarningLed = 12; //led cảnh báo cho chân D4

//tạo biến btn vào led để trang trí
int BTN2 = 3;
int LedDecorate = 10;

//thiết lập mảng nhằm tạo biến cho các thứ trong tuần
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

//khai báo các biến chưa ngày giờ
int Days;
int Months;
int Years;
int Seconds;
int Minutes;
int Hours;

String DayOfWeek; //biến chứa chuỗi ngày trong tuần
String NowDate; //biến chứa chuỗi ngày tháng năm
String NowTime; //biên chứa chuỗi giờ phút giây

void SetRTC_Time() //hàm khởi tạo RTC cho DS3231
{
    //khởi động RTC in DS3231
    rtc.begin();
    Serial.println("\n* * * * * * RTC * * * * * *");
    
    //hàm check xem DS3231 đã kết nối với Arduino chưa
    Serial.print ("Is RTC Operating (Waiting!!!) :  ");
      if(!rtc.begin()) 
      {
          Serial.println("Could't fine RTC");
          while(1);
      }
      else 
      {
          Serial.println("Connected Success :)");
          Serial.println("");
      }
      
      //check xem pin từ DS3231 có bị mất hay hết pin, nếu bị thì cần cập nhật lại thời gian
      if(rtc.lostPower())
      {
          Serial.print("RTC lost power or pin is out of date, lets set the time!!!");
          rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
          // for example to set January 27 2017 at 12:56 you would call:
          // rtc.adjust(DateTime(2017, 1, 27, 12, 56, 0));
      }
}

void readTimeRTC_PrintLCD() 
{
    DateTime now = rtc.now();
    //lcd.clear();       
    Days = now.day(); 
    Months = now.month(); 
    Years = now.year();
    Seconds = now.second(); 
    Hours = now.hour(); 
    Minutes = now.minute(); 
    DayOfWeek = daysOfTheWeek[now.dayOfTheWeek()];  
    
    NowDate = NowDate + DayOfWeek + ","+ Days + "/" + Months + "/" + Years ; 
    NowTime = NowTime + Hours + "h:" + Minutes + "m:" + Seconds + "s"; 
    
    // send to serial monitor
    Serial.print("Today is: "); 
    Serial.println(NowDate);
    Serial.print("Time is : ");
    Serial.println(NowTime);
    Serial.println("");
    
    //Print on lcd
    lcd.setCursor(0,0);
    lcd.print("Now : ");
    lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
    lcd.print(",");
    if(now.day()<=9)
    {
      lcd.print("0");
      lcd.print(now.day());
    }
    else {
     lcd.print(now.day()); 
    }
    lcd.print('/');
    if(now.month()<=9)
    {
      lcd.print("0");
      lcd.print(now.month());
    }
    else {
     lcd.print(now.month()); 
    }
    lcd.print('/');
    if(now.year()<=9)
    {
      lcd.print("0");
      lcd.print(now.year());
    }
    else {
     lcd.print(now.year()); 
    }
    lcd.setCursor(0,1);
    lcd.print("Time: "); 
   if(now.hour()<=9)
    {
      lcd.print("0");
      lcd.print(now.hour());
      lcd.print("h");
    }
    else {
     lcd.print(now.hour()); 
     lcd.print("h"); 
    }
    lcd.print(':');
    if(now.minute()<=9)
    {
      lcd.print("0");
      lcd.print(now.minute());
      lcd.print("m"); 
    }
    else {
     lcd.print(now.minute());
     lcd.print("m"); 
    }
    lcd.print(':');
    if(now.second()<=9)
    {
      lcd.print("0");
      lcd.print(now.second());
      lcd.print("s");
    }
    else {
     lcd.print(now.second()); 
     lcd.print("s");
    }
}

void Init_Servo() {
    myServo.attach(5); //cấu hình chân D5 là chân OUT cho Servo
}

//hàm quay Servo cho cá ăn (chưa tối ưu)
void CheckTimes()
{  
    /*Cho cá ăn tùy thích*/
    int btnStatus = digitalRead(BTN1); //đọc trạng thái Button cho cá ăn
    if(btnStatus == HIGH)
    {
        myServo.write(0); 
        delay(3000);
        myServo.write(150);
        delay(1000);
    }

    /*nên cho cá ăn 1 ngày 2 lần đó là 7h30 sáng và 16h30 chiều*/  

    //gọi thời gian
    DateTime now = rtc.now();
    Hours = now.hour(); 
    Minutes = now.minute();
    Seconds = now.second();

    if( ((Hours == 7) && (Minutes == 30) && (Seconds == 00)) || ((Hours == 16) && (Minutes == 30) && (Seconds == 00)) )
    {
        myServo.write(0);
        delay(3000);
        myServo.write(150);
        delay(1000); 
    }
}

//hàm led sáng
void LedWarning_ON() 
{
    digitalWrite(WarningLed,HIGH);
}

//hàm led tắt
void LedWarning_OFF()
{
    digitalWrite(WarningLed, LOW);
}

//hàm cảm biến độ mực chất lỏng (chưa tối ưu)
void Read_CamBienMucNuoc()
{
    int WaterLevel = analogRead(mucNuoc);//đọc dữ liệu
    lcd.setCursor(20,0);
    lcd.print("WaterLevel: ");
    // send to monitor serial
    Serial.print("\nWater Level is: ");
    Serial.println(WaterLevel);

    //nếu mức nước dưới 100 thì sẽ hiển thị lên lcd
    if(WaterLevel <= 100)
    {
        lcd.setCursor(32,0);
        lcd.print("Unstable"); //thông báo mực nước không ổn định
        LedWarning_ON();
//        Serial.print("Water Level: Unstable");
    }
    else 
    {
        lcd.setCursor(32,0);
        lcd.print("Stable"); //thông báo mực nước ổn định
        LedWarning_OFF();
//        Serial.print("Water Level: Stable");
    }
}

//hàm cảm biến đọ đục (chưa tối ưu)
void Read_CamBienDoDuc()
{
    int Turbidity = analogRead(doDuc); //đọc cảm biến

    float voltages = Turbidity * (5.0/1023.0); //chuyển thành độ đục thành điện áp
    
    int turbidity = ((voltages * 100) / 5); //tính ra phần trăm độ đục

    lcd.setCursor(20,1);
    lcd.print("Turbidity: ");
    
    //send to monitor serial
    Serial.print("\nTurbidity: ");
    Serial.println(turbidity);

    //send to lcd
    if(turbidity < 59)
    {
        lcd.setCursor(30,1);
        lcd.print(turbidity);
        lcd.print("% ");
        lcd.print("dirty");
        LedWarning_ON();
    }
    else 
    {
        lcd.setCursor(30,1);
        lcd.print(turbidity);
        lcd.print("% ");
        lcd.print("clean");
        LedWarning_OFF();
    }
}

void Decorate()
{
    DateTime now = rtc.now();
    //đọc trạng thái chân button 
    int buttonStatus2 = digitalRead(BTN2);
      if((digitalRead(BTN2)==HIGH)){
        temp2 = !temp2;          
        delay(200);
      }
      
      if(temp2) digitalWrite(LedDecorate, HIGH);
      else digitalWrite(LedDecorate, LOW);
      
    //hiển thị led
    if((now.hour() > 19) && (now.hour() < 21))
    {
        digitalWrite(LedDecorate, HIGH);
    }
    else 
    {
        digitalWrite(LedDecorate, LOW);
    } 
}

void setup() {
  
    //khởi tạo màn hình monitor Serial để in kết quả muốn hiển thị
    Serial.begin(9600); 

    //cấu hình chân cho BTN1
    pinMode(BTN1,INPUT);

    //cấu hình chân cho mucNuoc 
    pinMode(mucNuoc,INPUT);

    //cấu hình chân cho doDuc
    pinMode(doDuc, INPUT);

    //cấu hình chân cho led cảnh báo
    pinMode(WarningLed, OUTPUT);

    //cấu hình cho led và btn trang trí
    pinMode(LedDecorate,OUTPUT);
    pinMode(BTN2,INPUT);
    
    //hàm khởi tạo hoạt động LCD
    lcd.init();  
    
    //khởi tạo hoạt động I2C
    Wire.begin(); 

    //cho LCD đèn nền sáng lên
    lcd.backlight();

    //cài đặt thời gian của bạn
    SetRTC_Time();    

    //khởi động servo
    Init_Servo();
}

void loop() {
    readTimeRTC_PrintLCD();
    CheckTimes();
    Read_CamBienMucNuoc();
    Read_CamBienDoDuc();
    Decorate();
}
