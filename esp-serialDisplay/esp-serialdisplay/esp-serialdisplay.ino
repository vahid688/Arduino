#include <Wire.h> // responsável pela comunicação com a interface i2c
#include <LiquidCrystal_I2C.h> // responsável pela comunicação com o display LCD
 
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
lcd.setBacklight(HIGH);
//posiciona o cursor para escrita
//.setCursor(coluna, linha)
lcd.setCursor(0,0);
lcd.print("FERNANDOK.COM");
lcd.setCursor(0,1);
lcd.print("ACESSE!!!");
delay(1000); //intervalo de 1s
//desliga o backlight do LCD
lcd.setBacklight(LOW);
delay(1000); //intervalo de 1s
}
