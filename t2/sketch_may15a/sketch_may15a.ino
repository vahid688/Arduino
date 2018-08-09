#include <dht.h>

#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

dht DHT;

#define DHT11_PIN 7

void setup() {
  lcd.begin(16, 2);
}

void loop()
{
  int chk = DHT.read11(DHT11_PIN);
  int L = 0;
  String s = String(16);
  if (chk == -1)
  {
    s = ""+ (String)DHT.temperature;
    s = s+ (char)223 + "C  "+ (String)DHT.humidity + "%";
    lcd.setCursor(0, 0);
    lcd.print(s);
    //lcd.print(DHT.temperature);
    //lcd.print((char)223);
    //lcd.print("C");
    //lcd.setCursor(0, 1);
    //lcd.print("Humidity: ");
    //lcd.print(DHT.humidity);
    //lcd.print("%");
  }

  if (L > 0)
  {
    L = 0;
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    L += 1;
    digitalWrite(LED_BUILTIN, LOW);
  }
  delay(1000);

}
