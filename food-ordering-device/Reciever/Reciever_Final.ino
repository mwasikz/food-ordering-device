//#include <VirtualWire.h>
#include <SPI.h> // Not actualy used but needed to compile
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <RH_ASK.h>

RH_ASK driver;
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int buzzer = 9;
int LED = 6;


void setup()
{
    Serial.begin(9600);
    Serial.println("Device is ready");
    lcd.init();
    lcd.backlight();
    pinMode(buzzer, OUTPUT);
    pinMode(LED, OUTPUT);
   
    if (!driver.init())
    {
       Serial.println("init failed");
    }
      driver.init();
}
void loop()

{
 
    //uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t buf[10];
    uint8_t buflen = sizeof(buf);
    

    if (driver.recv(buf, &buflen))
    {
      digitalWrite(LED, HIGH);
      tone(buzzer, 1000);
      delay(1000);        
      noTone(buzzer);     
      delay(1000);
     
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("From Table 1");
      lcd.setCursor(2,01);

      String order = (char*)buf;
      
      lcd.println(order);
      delay(3000);

          

/* //Scrolling
   for(int i =0; i<2; i++)
      {
      for(int pos = 0; pos <= buflen; pos++)
      {
        lcd.scrollDisplayLeft();
        delay(500);
      }
      for(int pos = 0; pos <= buflen; pos++)
      {
        lcd.scrollDisplayRight();
        delay(500);
      }
    }
*/ 
    digitalWrite(LED, LOW);
    
      
    }
    
}
  
