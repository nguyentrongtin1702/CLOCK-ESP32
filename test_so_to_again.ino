#include <LiquidCrystal.h>
#include <Wire.h>
#include <RTClib.h>
#include <DHT11.h>

// Khởi tạo đối tượng RTC DS3231
RTC_DS3231 rtc;
// khoi tao dht11
DHT11 dht11(2);

// khoi tao mot mang dayoftheweek tro den kieu char
const char* daysOfTheWeek[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

// khoi tao cac chan giao tiep voi LCD_(giao tiep 4-bit)
const int rs = 12, en = 13, d4 = 32, d5 = 33, d6 = 25, d7 = 26;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// khoi tao mang de tao so to
byte zero[8] = {0b00111,0b01111,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111};

byte one[8] = {0b11111,0b11111,0b11111,0b00000,0b00000,0b00000,0b00000,0b00000};

byte two[8] = {0b11100,0b11110,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111};

byte three[8] = {0b00000,0b00000,0b00000,0b00000,0b00000,0b11111,0b11111,0b11111};

byte four[8] = {0b11111,0b11111,0b11111,0b11111,0b11111,0b11111,0b01111,0b00111};

byte five[8] ={0b11111,0b11111,0b11111,0b11111,0b11111,0b11111,0b11110,0b11100};

byte six[8] = {0b11111,0b11111,0b11111,0b00000,0b00000,0b00000,0b11111,0b11111};

byte seven[8] = {0b11111,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111};

// cac so duoc ghep tu cac ki tu duoc tao o tren
unsigned char lcd_so_to[10][6] ={                 
                  0,1,2,4,3,5,    // so 0
                  1,2,32,3,7,3,   // so 1
                  6,6,2,4,3,3,    // so 2
                  6,6,2,3,3,5,    // so 3
                  7,3,7,32,32,7,  // so 4
                  7,6,6,3,3,5,    // so 5                               
                  0,6,6,4,3,5,    // so 6
                  1,1,7,32,32,7,  // so 7
                  0,6,2,4,3,5,    // so 8
                  0,6,2,3,3,5};

// ham lcd hien thi so to
void lcd_hien_thi_so_to(int so, int x, int y)
{
  lcd.setCursor(x,y);
  for(int i=0;i<6;i++)
  {
    if(i==3) lcd.setCursor(x,y+1);
    lcd.write(lcd_so_to[so][i]);
  }
}
// ham xoa so to
void lcd_xoa_so_to(int x, int y)
{
  lcd.setCursor(x,y);
  for(int i =0;i<6;i++)
  {
    if(i==3) lcd.setCursor(x,y+1);
    lcd.write(' ');
  }

}

// ham giai ma 2 so to co xoa vo nghia
void lcd_gm_ht_2so_to_xvn(int so, int x, int y, bool xvn)
{
  int ma_ch, ma_dv;
  lcd.setCursor(x,y);
  ma_dv = so%10;
  ma_ch = so/10%10;
  lcd_hien_thi_so_to(ma_dv,x+3,y);
  if(ma_ch == 0 && xvn == 1) lcd_xoa_so_to(x,y);
  else lcd_hien_thi_so_to(ma_ch,x,y);
}



void setup() {
  // khoi tao lcd 20x4
  lcd.begin(20, 4);

  // setup cac ki tu dac biet vao vung nho cgram cua lcd
  lcd.createChar(0, zero);
  // create a new character
  lcd.createChar(1, one);
  // create a new character
  lcd.createChar(2, two);
  // create a new character
  lcd.createChar(3, three);
  // create a new character
  lcd.createChar(4, four);
  lcd.createChar(5, five);
  lcd.createChar(6,six);
  lcd.createChar(7,seven);
    Wire.begin();

  // Khởi tạo Serial Monitor
  Serial.begin(115200);

  // Kiểm tra kết nối với DS3231
  if (!rtc.begin()) {
    Serial.println("Không tìm thấy RTC DS3231!");
    while (1);
  }

  // Nếu RTC chưa được đặt thời gian, ta sẽ đặt thời gian hiện tại từ máy tính
  if (rtc.lostPower()) {
    Serial.println("DS3231 bị mất nguồn, đang đặt lại thời gian!");
    // Đặt thời gian từ thời gian biên dịch (bạn có thể thay đổi theo nhu cầu)
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

}

void loop() {
  int temp = 0;
  int humi = 0;

  int result = dht11.readTemperatureHumidity(temp,humi);
  DateTime now = rtc.now();
  lcd.setCursor(0, 0);
  lcd.print("MY CLOCK");
  lcd.setCursor(9,0);
  lcd.print(now.day());
  lcd.setCursor(11,0);
  lcd.print("/");
  lcd.setCursor(13,0);
  lcd.print(now.month());
  lcd.setCursor(14,0);
  lcd.print("/");
  lcd.setCursor(16,0);
  lcd.print(now.year());
  lcd.setCursor(0,3);
  lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
  lcd_gm_ht_2so_to_xvn(now.hour(),0,1,0);
  lcd.setCursor(6,1);
  lcd.print(":");
  lcd.setCursor(13,1);
  lcd.print(":");
  lcd_gm_ht_2so_to_xvn(now.minute(),7,1,0);
  lcd_gm_ht_2so_to_xvn(now.second(),14,1,0);
  if(result == 0)
  {
    lcd.setCursor(4, 3);
    lcd.print("temp:");
    lcd.setCursor(9, 3);
    lcd.print(temp);
    lcd.setCursor(12,3);
    lcd.print("/Humi:");
    lcd.setCursor(18,3);
    lcd.print(humi);
  }

}
