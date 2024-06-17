#include "LiquidCrystal.h"

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

float input_voltage_bat = 15.0;
float input_voltage_charge = 15.0;

float temp1=0.0;
float temp2=0.0;

float r1=20200.0;
float r2=9600.0;

float r1Charge = 30000.0;
float r2Charge = 8000.0;

int relay = 10;
int button = 9;

bool button_flag = false;

bool status = false;

void setup()
{
  pinMode(relay, OUTPUT);
  pinMode(button, INPUT_PULLUP);

  Serial.begin(9600);     //  opens serial port, sets data rate to 9600 bps   
  lcd.begin(16, 2);       //// set up the LCD's number of columns and rows:    
  lcd.print("Hello");
}

void loop() 
{ 
  //Conversion formula    
  int analog_value1 = analogRead(A0);     
  temp1 = (analog_value1 * 5.0) / 1024.0;   
  input_voltage_bat = temp1 / (r2/(r1+r2));        
  if (input_voltage_bat < 0.1)
  {     
    input_voltage_bat=0.0;    
  }

  int analog_value2 = analogRead(A2);     
  temp2 = (analog_value2 * 3.75) / 1024.0;   
  input_voltage_charge = temp2 / (r2Charge/(r1Charge+r2Charge));    

  if (input_voltage_charge < 0.1)
  {     
    input_voltage_charge=0.0;    
  }
  status = true;
  digitalWrite(relay, status);

  if(digitalRead(button) == LOW)
  {
    delay(100);
    button_flag = true;
  }

  check_voltage();
}

void check_voltage()
{
  if (input_voltage_bat <= 11.5) //якщо акум розрядився - зарядка вкл
  {
    status = false;
    digitalWrite(relay, status);
  }

  if (input_voltage_bat >= 13.0) //якщо акум перезаряджено - зарядка викл
  {
    status = true;
    digitalWrite(relay, status);
  }

  if (input_voltage_bat < 13.0 && input_voltage_bat > 11.5) //якщо акум норм - зарядка викл (навантаження вкл)
  {
    status = true;
    digitalWrite(relay, status);
  }

  if (button_flag == true && input_voltage_bat < 13.0 && input_voltage_bat > 11.5 && input_voltage_chrg < 5) // якщо батарея норм, є живлення ЗУ, але захотілося підзарядити - натисни кнопку
  {
    button_flag = false;
    status = true;
    digitalWrite(relay, status);
  }

  Serial.print("v_bat = ");    
  Serial.print(input_voltage_bat);

  Serial.print(";  v_chrg = ");    
  Serial.println(input_voltage_charge);
  delay(300); 

  lcd.setCursor(0, 1);
  lcd.print("Voltage= ");
  lcd.print(input_voltage_bat);
}



