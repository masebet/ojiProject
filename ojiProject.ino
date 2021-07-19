#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>                                        
#include <LiquidCrystal_I2C.h>                            
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 4);

int garamTDS      = A0;
int pHSense       = A1;
int SuhuSense     = A2;
int SuhuDs        = A3;
int kontak1       = 2;    //pelampung
int kontak2       = 3;    //pelampung

int relay1        = 4;    //pemanas
int relay2        = 5;    //buzzer

OneWire oneWire(SuhuDs);
DallasTemperature sensors(&oneWire);

int samples = 10;
float adc_resolution = 1024.0;
char sTnk1 = 'H';
char sTnk2 = 'H';

bool tabungSatuPenuh(){
  bool val = 0;
  if(digitalRead(kontak1)==0) val = 1;    //!= atau ==
  return val;
  }
bool tabungDuaPenuh(){
  bool val = 0;
  if(digitalRead(kontak2)!=0) val = 1;    //!= atau ==
  return val;
  }

void PemanasMenyala(){
  digitalWrite(relay1,0);
}
void PemanasMati(){
  digitalWrite(relay1,1);
}

void PompaMenyala(){
  digitalWrite(relay1,0);
}
void PompaMati(){
  digitalWrite(relay1,1);
}

void BuzerMenyala(){
  digitalWrite(relay2,0);
}
void BuzerMati(){
  digitalWrite(relay2,1);
}

float ph (float voltage) {
    return 7 + ((2.5 - voltage) / 0.18);
}

void setup()
{   
        
    lcd.init();
    lcd.backlight();
    Serial.begin(9600);
    delay(100);
    sensors.begin();
    pinMode(garamTDS,INPUT);
    pinMode(pHSense,INPUT);
    pinMode(SuhuSense,INPUT);
    pinMode(SuhuDs,INPUT);
    pinMode(kontak1,INPUT_PULLUP);
    pinMode(kontak2,INPUT_PULLUP);
    pinMode(relay1,OUTPUT);
    pinMode(relay2,OUTPUT);
}

void loop()
{
    int measurings=0;
    for (int i = 0; i < samples; i++)
    {
        measurings += analogRead(pHSense);
        delay(10);
    }
    float voltage = 5 / adc_resolution * measurings/samples;

    int measuringsSuhu=0;
    for (int i = 0; i < samples; i++)
    {
        measuringsSuhu += analogRead(SuhuSense);
        delay(10);
    }
    float voltageSuhu = (measuringsSuhu/samples)/26.9;


    int measuringsSalt=0;
    for (int i = 0; i < samples; i++)
    {
        measuringsSalt += analogRead(garamTDS);
        delay(10);
    }
    float voltageSalt = measuringsSalt/samples;

    sensors.requestTemperatures();  
    float suhuDsSensor = sensors.getTempCByIndex(0);
    
    //=========================================================//
    delay(2000);
    
    lcd.clear();
    lcd.setCursor(0, 0);                                    
    lcd.print("TDS : ");
    String dTDS = String(voltageSalt,2);
    lcd.print(dTDS);
                                  
    lcd.setCursor(0, 1);                                     
    lcd.print("PH Meter : ");
    String dPH = String(ph(voltage),2);
    lcd.print(dPH);
    
    lcd.setCursor(0, 2); 
    lcd.print("+Tnk1> ");
    lcd.print(sTnk1);                                   
    lcd.print(" Sh : ");
    String dSUHU1 = String(voltageSuhu,2);
    lcd.print(dSUHU1);
                                  
    lcd.setCursor(0, 3);                                     
    lcd.print("+Tnk2> ");
    lcd.print(sTnk2);
    lcd.print(" Sh : ");
    String dSUHU2 = String(suhuDsSensor,2);
    lcd.print(dSUHU2);
    
    delay(1000);
    if(tabungSatuPenuh()){
      PemanasMenyala();
      PompaMati();
      sTnk1 = 'P';
    }else{
      PemanasMati();
      PompaMenyala();
      sTnk1 = 'H';      
    }

    if(tabungDuaPenuh()){
      BuzerMenyala();
      sTnk2 = 'P';
    }else{
      BuzerMati();
      sTnk2 = 'P';
    }
}
