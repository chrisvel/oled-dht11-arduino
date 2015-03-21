/*

  Oled sensor
  Copyright (c) me, myself and I
  
*/

#include "U8glib.h"
#include <dht11.h>
#include <MemoryFree.h>
#include "LowPower.h"

dht11 DHT11;
#define DHT11PIN 4
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);	// Display which does not send ACK

double humidity = 0;
double temperature = 0;

struct READINGS {
    double humidity;
    double temperature;
};

void setup(void) {
  pinMode(13, OUTPUT);    
  Serial.begin(9600);
  Serial.println(F("DHT11 TEST PROGRAM "));
  Serial.print(F("LIBRARY VERSION: "));
  Serial.println(DHT11LIB_VERSION);
  Serial.println();     
}

void loop(void) {
  digitalWrite(13, HIGH);
    
  int chk = DHT11.read(DHT11PIN);

  Serial.print(F("\nRead sensor: "));
  switch (chk)
  {
    case DHTLIB_OK: 
		Serial.println(F("OK")); 
		break;
    case DHTLIB_ERROR_CHECKSUM: 
		Serial.println(F("Checksum error")); 
		break;
    case DHTLIB_ERROR_TIMEOUT: 
		Serial.println(F("Time out error")); 
		break;
    default: 
		Serial.println(F("Unknown error")); 
		break;
  }
  
  READINGS now = {(double)DHT11.humidity, (double)DHT11.temperature };
  
  // OLED display loop
  u8g.firstPage();  
  do {
       u8g.setColorIndex(1);
       u8g.setFont(u8g_font_fixed_v0r);
       u8g.setPrintPos(1,10);
       u8g.print(F("----| Dashboard |----"));
       // --------------------------------
       u8g.setPrintPos(1,23);
       u8g.print(F("Temperature:"));
       u8g.setPrintPos(80,23);
       u8g.print(now.temperature);
       u8g.setPrintPos(112,23);
       u8g.print("'C");
       // --------------------------------
       u8g.setPrintPos(1,36);
       u8g.print(F("Humidity:"));
       u8g.setPrintPos(80,36);
       u8g.print(now.humidity);
       u8g.setPrintPos(112,36);
       u8g.print("%");
       // --------------------------------
       u8g.setPrintPos(1,62);
       u8g.print(freeMemory());
  } while( u8g.nextPage() );
  
  delay(1000);    
  digitalWrite(13, LOW);     
  LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART0_OFF, TWI_OFF);   
}

