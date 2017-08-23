SYSTEM_MODE(SEMI_AUTOMATIC);
#include "SparkJson.h"
#include <SHT1x.h>
#include <MQTT.h>

//define Particle ID
#define id "P01"


//MQTT
#define host "192.168.1.253" //ktph_pi
#define port 1883

MQTT client(host, port, callback);
IPAddress remoteIP(8, 8, 8, 8);; 

void callback(char* topic, byte* payload, unsigned int length){
    //
}    
//********************************************

StaticJsonBuffer<600> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();

//Pin config **********
int led = D3;
int pirPin = D4;
int ldrPin = A2;
int noisePin = A3;
SHT1x sht1x(A0, A1);
//*********************

//Variable decleration//
int noise_count, p_count=0;
int SAMPLES = 3000;
int sam_count=0;
int light_value;
float temp_c;
float humidity;
float temperature;
long int noisesum=0;

// PIR
int m_count=0;
int motionValue=0;                        //Motion value
int count_motion;                         //Count motion

 
// Noise Sensor
int A,B,C,D,E,F,G,H=0;
//const int noisePin=A1;
const int noiseMargin=6;                   //Normal noise level
//float noiseT=0; 
int noise_state=0;
char sound[50];


String data;
int time_now;

//**********************************

void setup() {
    //Configuring pins
    pinMode(led, OUTPUT);
    pinMode(pirPin, INPUT);
    pinMode(ldrPin, INPUT);
    pinMode(noisePin, INPUT);
    WiFi.on();
    WiFi.connect();
    if(WiFi.ping(remoteIP, 10) > 5){
        Particle.connect();
    }
    time_now=0;
    //
   
}
bool led_status = false;
void loop() {
    data = getData();
    time_now = Time.now();
    digitalWrite(led, HIGH);led_status=true;
    client.connect(id);
    while(!client.isConnected()){
        client.connect(id);
        if(time_now+1>Time.now()){
            if(led_status) { digitalWrite(led, LOW); led_status = false; }
            else { digitalWrite(led, HIGH); led_status = true; }
        }
        if(time_now+60>Time.now()){
            digitalWrite(led, LOW);
            led_status = false;
            return;
        }
    }
    client.publish("sutd/2.601", data);
    client.disconnect();
    if(!Particle.connected()){    
        if(WiFi.ping(remoteIP, 10) > 5){
            Particle.connect();
        }
    }
}

String getData() {
    
    A=0;
    B=0;
    C=0;
    D=0;
    E=0;
    F=0;
    G=0;
    H=0;
    m_count=0;
    while (sam_count < SAMPLES) {
        // Sound Sensor
        int valN=analogRead(noisePin);  // Reading Noise Pin A1
        noise_count++;
       
        if(valN>=0){
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
        m_count++;
     
             
        if(m_count>29)
        {
          // PIR Motion Sensor
          motionValue = digitalRead(pirPin);
          p_count++;
               if(motionValue==1)
               {
                 count_motion++;             // Sending count Motion i.e Total count in 5 min
               }
          m_count=0;
        } 
        sam_count++;
        
        if(time_now+3 < Time.now() && time_now!=0){  //to reduce time !=0
            digitalWrite(led, LOW);
            time_now=0;
        }
        delay(100);
     
    }
    
    //LDR light
    light_value = analogRead(ldrPin);
    
    // Read values from the Temperature and Humidity sensor
    temp_c = sht1x.readTemperatureC();
    humidity = sht1x.readHumidity();
   
    
    //MQTT publishing
    
    sprintf(sound, "%d_%d_%d_%d_%d_%d_%d_%d", A,B,C,D,E,F,G,H);
    root["id"]=id;
    root["t"]=temp_c;
    root["l"]=light_value;
    root["h"]=humidity;
    root["m"]=count_motion;
    root["n_h"]=sound;
    root["n_t"]=noisesum;
        
    char jsonChar[100];
    root.printTo((char*) jsonChar, sizeof(jsonChar));
    
    count_motion=0;
    sam_count=0;
    noise_count=0;
    noisesum=0;
    p_count=0;
    return String(jsonChar);
}