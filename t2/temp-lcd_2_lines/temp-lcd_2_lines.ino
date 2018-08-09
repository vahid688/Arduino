#include <DHT.h>
#include <DHT_U.h>


#include <Wire.h>
//#include <LiquidCrystal_I2C.h>
//#include <dht.h>

//#include <LiquidCrystal.h>
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x3F,18,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

dht DHT;

#define DHT11_PIN 7

float maxT, minT, maxH, minH;
int scroll;
const int Max_Lines = 3;

void setup() {
  lcd.begin(16, 2);
  maxT = -99999;
  minT = 99999;
  minH = 100;
  maxH = 0;
  scroll = 0;
}

void loop()
{
  int chk = DHT.read11(DHT11_PIN);
  int L = 0;

  float temp;
  float hum;
  String s = String(16);
  if (chk == -1)
  {
    temp = DHT.temperature;
    hum = DHT.humidity;

    if (temp > maxT)
      maxT = temp;
    if (temp < minT)
      minT = temp;

    if (hum > maxH)
      maxH = hum;
    if (hum < minH)
      minH = hum;

    s = (String)temp + (char)223 + "C  " + (String)hum + "%";
    lcd.setCursor(0, 0);
    lcd.print(s);

    //lcd.print(DHT.temperature);
    //lcd.print((char)223);
    //lcd.print("C");
    switch (scroll) {
      case 0:
        s = "T:" + (String)maxT + "  t:" + (String)minT;
        break;
      case 1:
        s = "H:" + (String)maxH + "  h:" + (String)minH;
        break;
      default :
        s = "";
    }


    lcd.setCursor(0, 1);
    lcd.print(s);
    //lcd.print("Humidity: ");
    //lcd.print(DHT.humidity);
    //lcd.print("%");
    scroll = scroll + 1;
  }


  if (scroll > Max_Lines)
  {
    scroll = 0;
  }

  delay(1000);

}
