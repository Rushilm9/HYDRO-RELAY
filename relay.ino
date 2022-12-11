#include <EEPROM.h>
#include "GravityTDS.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <SoftwareSerial.h>

SoftwareSerial espSerial(5, 6);  //tx,rx
int relayPin = 8;                // define output pin for relay

#define TdsSensorPin A1
#define ONE_WIRE_BUS 2

GravityTDS gravityTds;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

static float calibration_value = 21.34 - 0.46 + 0.50;
int phval = 0;
unsigned long int avgval;
int buffer_arr[10], temp;
float temperature = 25, tdsValue = 29;



void setup() {

  pinMode(relayPin, OUTPUT);  // define pin 8 as output
  sensors.begin();            // Start up the library
  Serial.begin(115200);
  espSerial.begin(115200);
  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0);       //reference voltage on ADC, default 5.0V on Arduino UNO
  gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
  gravityTds.begin();            //initialization
}



void loop() {
  {


    {
      digitalWrite(relayPin, HIGH);
       delay(5000);

      for (int i = 0; i < 10; i++) {
        buffer_arr[i] = analogRead(A0);
        delay(300);
      }
      for (int i = 0; i < 9; i++) {
        for (int j = i + 1; j < 10; j++) {
          if (buffer_arr[i] > buffer_arr[j]) {
            temp = buffer_arr[i];
            buffer_arr[i] = buffer_arr[j];
            buffer_arr[j] = temp;
          }
        }
      }
      avgval = 0;
      for (int i = 2; i < 8; i++)
        avgval += buffer_arr[i];
      float volt = (float)avgval * 5.0 / 1024 / 6;
      float ph_act = -5.70 * volt + calibration_value;

      delay(1000);
      Serial.print("pH Val:");
      Serial.println(ph_act);
      Serial.print("voltage:");
      Serial.println(volt);
    }

   
  }


  //esp[pass]
  // espSerial.print("pH Val:");
  // espSerial.println(ph_act);
  // espSerial.print("voltage:");
  // espSerial.println(volt);

  // sensors.requestTemperatures();
  // Serial.print("Temperature: ");
  // Serial.print(sensors.getTempCByIndex(0));
  //Serial.print((char)176);//shows degrees character
  // Serial.print("C  |  ");


  //esp pass
  // espSerial.print("Temperature: ");
  // espSerial.print(sensors.getTempCByIndex(0));
  //  espSerial.println(ph_act);
  //  espSerial.print(tdsValue,0);
  // espSerial.print((char)176);//shows degrees character
  // espSerial.print("C  |  ");

  //print the temperature in Fahrenheit
  // Serial.print((sensors.getTempCByIndex(0) * 9.0) / 5.0 + 32.0);
  // Serial.print((char)176);//shows degrees character
  // Serial.println("F");

  //temperature = readTemperature();  //add your temperature sensor and read it

 
 
  {
     digitalWrite(relayPin, LOW);
      delay(5000);
    gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
    gravityTds.update();                     //sample and calculate
    tdsValue = gravityTds.getTdsValue();     // then get the value
    Serial.print(tdsValue, 0);
    Serial.println("ppm");
   
  }


  //esp pass
  // espSerial.print(tdsValue,0);
  // espSerial.println("ppm");

  //to pass in esp


  delay(500);
}
// #include <OneWire.h>
// #include <DallasTemperature.h>

// // Data wire is plugged into digital pin 2 on the Arduino
// #define ONE_WIRE_BUS 2

// // Setup a oneWire instance to communicate with any OneWire device
// OneWire oneWire(ONE_WIRE_BUS);

// // Pass oneWire reference to DallasTemperature library
// DallasTemperature sensors(&oneWire);

// void setup(void)
// {
//   sensors.begin();	// Start up the library
//   Serial.begin(9600);
// }

// void loop(void)
// {
//   // Send the command to get temperatures
//   sensors.requestTemperatures();

//   //print the temperature in Celsius
//   Serial.print("Temperature: ");
//   Serial.print(sensors.getTempCByIndex(0));
//   Serial.print((char)176);//shows degrees character
//   Serial.print("C  |  ");

//   //print the temperature in Fahrenheit
//   Serial.print((sensors.getTempCByIndex(0) * 9.0) / 5.0 + 32.0);
//   Serial.print((char)176);//shows degrees character
//   Serial.println("F");

//   delay(500);
// }
// #include <Wire.h>
// #include <LiquidCrystal_I2C.h>
// LiquidCrystal_I2C lcd(0x27, 16, 2);
// static float calibration_value = 21.34-0.46+0.50;
// int phval = 0;
// unsigned long int avgval;
// int buffer_arr[10],temp;
// void setup()
// {
//  Serial.begin(9600);
//   // lcd.init();
//   // lcd.begin(16, 2);
//   // lcd.backlight();
//   // lcd.setCursor(0, 0);
//   // lcd.print("   Welcome to      ");
//   // lcd.setCursor(0, 1);
//   // lcd.print(" Circuit Digest    ");
//   delay(2000);
//   // lcd.clear();
// }
// void loop() {
//  for(int i=0;i<10;i++)
//  {
//  buffer_arr[i]=analogRead(A0);
//  delay(300);
//  }
//  for(int i=0;i<9;i++)
//  {
//  for(int j=i+1;j<10;j++)
//  {
//  if(buffer_arr[i]>buffer_arr[j])
//  {
//  temp=buffer_arr[i];
//  buffer_arr[i]=buffer_arr[j];
//  buffer_arr[j]=temp;
//  }
//  }
//  }
//  avgval=0;
//  for(int i=2;i<8;i++)
//  avgval+=buffer_arr[i];
//  float volt=(float)avgval*5.0/1024/6;
//  float ph_act = -5.70 * volt + calibration_value;
//  Serial.print("pH Val:");
//  Serial.println(ph_act);
//  Serial.print("voltage:");
//  Serial.println(volt);


//  lcd.print("pH Val:");
//  lcd.setCursor(8, 0);
//  lcd.print(ph_act);
//  delay(1000);
// }