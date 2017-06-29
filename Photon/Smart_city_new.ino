#include <Barometer.h>
 
// Version 0.2 -20th july - With out AREF declaration; AREF is defalut
// Version 0.3 -ischarging added
// Version 0.4
// For smart city  and barometer nodes Chandra // October 1st 2014
// Version 0.5  from 4th Nov - Main change - Send in two intervals
//version 0.6 - 15th 2015 for the node  SB-011 with battery 0.05 to know better charging
// Update again on 9th/13th Feb 2015 for battery % and charging
// update 24/2/2015 -for Light sensor value  reset at end of loop.
// Update 25/2/2015 - battery value less 10 %  then set to 10
 
// update on 14th May  2015 - Battery and Rain RAW value + clean the code
// update on last week Ausgust 2015 for new high accuracy Baraometer  from seeed studio
//sound capture frequency = 100ms*SAMPLE
//PIR capture frequency = 1/3 sound capture frequency
#include <Digital_Light_TSL2561.h>
#include <Wire.h>
#include <XBee.h>
#include <math.h>
#include <SHT1x.h>
#include <HP20x_dev.h>
#include "Arduino.h"
#include <KalmanFilter.h>
unsigned char ret = 0;
long int noisesum=0;
/* Instance */
KalmanFilter p_filter;    //pressure filter
 
 
//______________________________PINS ___________________________________________
 int digitallight=0;
//Specify data and clock connections and instantiate SHT1x object
#define dataPin  10
#define clockPin 11
SHT1x sht1x(dataPin, clockPin);
 
#define Rainsensor_Pin A3 
// UV sensor A0
// Baro meter I2C
// Light I2C
// Sound A1
// Solar A2
// Rain Sensor A3
// XBee  Sleep 12
 
byte AREF=20;                       // Changed from 15 .Correct is 20 from schmatic
const byte solar=A2;               // For Solar Voltage Analog A2
int sleepXbee=12;                  // For Xbee sleep
const int inPin =A0;              // For UV Sensor A0
//___________________Variables____________________________________________________
int noice_count, p_count=0;
int SAMPLES = 3000;
int sam_count=0;
float temp_c;
float humidity;
float temperature;
 
// PIR
int m_count=0;
int motionValue=0;                        //Motion value
int count_motion;                         //Count motion
const int pirPin = 8; // Digital pin 8
 
// Noise Sensor
int A,B,C,D,E,F,G,H=0;
const int noisePin=A1;
const int noiseMargin=6;                   //Normal noise level
float noiseT=0; 
int noise_state=0;
 
// UV sensor
int sensorValue;
long  sum=0;
float Vsig;
 
//Rain Sensor
int Rainsensor_Digital = 0;  
int Rainsensor_Min = 0; // Min. value of rain sensor in 5 minutes
 
 
//Battery  
float voltage;
float t;
const String name="SB70";     
static int ADC7=0;
 
 
//_______________________________Setup _____________________________________
 
void setup()
{
  pinMode(sleepXbee,OUTPUT);
  Serial.begin(19200);          // initialize serial communications at 19200 bps
  delay(5000);
  digitalWrite(sleepXbee,LOW);
  delay (500);
  Serial.print(name);
  Serial.println("_joined_network!  ");
  delay (1000);
  digitalWrite(sleepXbee,HIGH);
  pinMode(Rainsensor_Pin,INPUT);
  pinMode(pirPin, INPUT);
  Wire.begin();
  TSL2561.init(); // Digital Light Sensor
  analogReference(INTERNAL);
  digitalWrite(sleepXbee,HIGH);  //Serial.println("Xbee goes to sleep");
  HP20x.begin();
  delay(100);
 
 
}
 
 
 
//_______________________________LOOP_____________________________________
void loop()
{ 
 
 
    A=0;
    B=0;
    C=0;
    D=0;
    E=0;
    F=0;
    G=0;
    H=0;
    m_count=0;
while (sam_count < SAMPLES) 
  {
    // Sound Sensor
    int valN=analogRead(noisePin);  // Reading Noise Pin A1
    noice_count++;
   
    if(valN>=0)
      {
             if(valN<=6)
         {
            A++;
         }
            if(valN>6&&valN<=10)
         {
           B++;
         }
             if(valN>10&&valN<=20)
         {
           C++;
         }
               if(valN>20&&valN<=35)
         {
           D++;
         }
               if(valN>35&&valN<=50)
         {
           E++;
         }
               if(valN>50&&valN<70)
         {
           F++;
         }
               if(valN>70&&valN<=100)
         {
           G++;
         }
            if(valN>100)
         {
           H++;
         }
     }     
     noisesum+=valN;
    // Serial.println(valN);
         
      m_count++;
 
         
            if(m_count>29)
            {
              // PIR Motion Sensor
              motionValue = digitalRead(pirPin);
              p_count++;
             
                   if(motionValue==HIGH)
                   {
                     count_motion++;             // Sending count Motion i.e Total count in 5 min
                   }
              m_count=0;
            } 
 
 
    sam_count++;
    delay(100);
 
}
// BARO METER
      long Pressure = HP20x.ReadPressure();
      Pressure = HP20x.ReadPressure();
      float t = Pressure/100.0;
      float baro=p_filter.Filter(t);
      baro=p_filter.Filter(t);
      baro=p_filter.Filter(t);
      baro=p_filter.Filter(t);
      baro=p_filter.Filter(t);
      baro=p_filter.Filter(t);
      baro=p_filter.Filter(t);
      baro=p_filter.Filter(t);
      baro=p_filter.Filter(t);
 
// Digital Light
    Wire.beginTransmission(41);
    char error = Wire.endTransmission();
    long lightValue=0;
    if (!error)
    {
    lightValue = (TSL2561.readVisibleLux());
    }

      
// Read values from the Temperature and Humidity sensor
    temp_c = sht1x.readTemperatureC();
    humidity = sht1x.readHumidity();
 
// For UV sensor
    Vsig=analogRead(inPin);
 
// For Rain
    
    int Rainsensor = analogRead(Rainsensor_Pin);
      
//For Battery
    analogReference(DEFAULT);
    ADC7=analogRead(A7);
  
// For Solar  
    float solarV =  analogRead(solar);  // Solar Voltage
    analogReference(INTERNAL);

    digitalWrite(sleepXbee,LOW);
    
    delay (500);
    Serial.print("xxxx ");
    Serial.print(name);
    Serial.print(" 1 ");
    Serial.print(temp_c);
    Serial.print(" 2 ");
    Serial.print(lightValue);
    Serial.print(" 4 ");
    Serial.print(humidity);
    Serial.print(" 7 ");
   
    Serial.print(count_motion); 
    Serial.print(" 12 ");
    Serial.print(A); Serial.print("."); Serial.print(B); Serial.print(".");Serial.print(C); Serial.print(".");Serial.print(D);
    Serial.print(".");Serial.print(E); Serial.print("."); Serial.print(F); Serial.print(".");Serial.print(G); Serial.print(".");Serial.print(H);
        Serial.println(" yyyy");
    delay (500);
    digitalWrite(sleepXbee,HIGH);
    delay (5000);
  
    digitalWrite(sleepXbee,LOW);
    delay (500);
    Serial.print("xxxx ");
    Serial.print(name);
    Serial.print(" 20 ");
    Serial.print(ADC7);
    Serial.print(" 21 ");
    Serial.print(solarV);
    Serial.print(" 28 ");
    Serial.print(Vsig); // UV
    Serial.print(" 30 ");
    Serial.print(Rainsensor);
    Serial.print(" 31 ");
    Serial.print(baro);
    Serial.print(" 37 ");
    Serial.print(noisesum);
    Serial.println("  yyyy");
    delay (500);
    digitalWrite(sleepXbee,HIGH);  //Serial.println("Xbee goes to sleep");
    delay (5000);
     
 
    sum =0;
    sensorValue=0;
    count_motion=0;
    noiseT=0;
    Vsig=0;
    sam_count=0;
    noice_count=0;
    p_count=0;
 
  } 
