//Sourcecode to be used on biceps
//Difference to version for facial muscles: only 4 signals in a row needed and emg signal needs to deviate 0.6 from average and not 0.7 from average to be counted
//and cachezeit-startzeit<2001
// also: both relais on high at startup so connected device stays off on power cut
// Newest Version: Now uses Arduino statistics library for input detection
#include "Statistic.h"
Statistic EMGStats, EMGStats_Shorttime; 

int sensorPin = 0;    

int state=0;
int state2=0;  
int ledPin = 13;      
int switch1_pin=2;
int relais = 13;
int relais2 = 12;
int hold_or_contact = 1; //contact=default=1;
int detectioncycles = 3; // defaultvalue = 3
float sd_detection =2.7; // defaultvalue =2.7 
char a;


unsigned int count_ausloeser;
unsigned long startzeit, cachezeit; 
bool flag, flag_ausloeser;	

void switch_state(int relaisnr=-1)
{
  if(hold_or_contact==0)
  {
    if(relaisnr !=-1)
    {
      int state_cache=0;
      state_cache=digitalRead(relaisnr);

      if(state_cache==LOW){    
        digitalWrite(relaisnr, HIGH);
      };
      if(state_cache==HIGH){    
        digitalWrite(relaisnr, LOW);
      };
    };
  }
  if(hold_or_contact==1)
    if(relaisnr !=-1)
    {
      int state_cache=0;
      state_cache=digitalRead(relaisnr);

      digitalWrite(relaisnr, LOW);
      delay(500);
      digitalWrite(relaisnr, HIGH);
    };    

}

void setup() // this function runs once when the sketch starts up
{

  flag_ausloeser=0;
  count_ausloeser=0;
  startzeit=0;
  pinMode(relais, OUTPUT);
  pinMode(relais2, OUTPUT);
  pinMode(switch1_pin, INPUT);
  Serial.begin(57600);
  digitalWrite(relais, HIGH);
  digitalWrite(relais2, HIGH); 
  EMGStats.clear();
  EMGStats_Shorttime.clear();

}


void loop() // this function runs repeatedly after setup() finishes
{

  int sensorValue;
  hold_or_contact = digitalRead(switch1_pin);
  if(hold_or_contact==1) //If board is set to contact mode both relais are set of (this is HIGH with the relais card I used)
  {  
    digitalWrite(relais, HIGH);
    digitalWrite(relais2, HIGH);
  }
  //   Serial.println(hold_or_contact);


  for(a=0;a<6;a++){
    sensorPin=a;
    sensorValue = analogRead(sensorPin);
    EMGStats.add((float)sensorValue);
    EMGStats_Shorttime.add((float)sensorValue);    

  } 

  if(EMGStats_Shorttime.count()==6*detectioncycles)
  {
    float diff, sdev;    

    diff= EMGStats.average() - EMGStats_Shorttime.average();
    sdev=EMGStats.unbiased_stdev();

    /*  Serial.print("Average:   ");
     Serial.print(EMGStats.average());
     Serial.print("    Standarddeviation:   ");
     Serial.print(EMGStats.unbiased_stdev());
     Serial.print("    Measured Average Deviation   ");
     Serial.print(EMGStats_Shorttime.average());
     Serial.print("    Measured Maximum    ");
     Serial.print(EMGStats.maximum());
     Serial.println("");*/
    if(diff>(sd_detection*sdev)){
      if(flag_ausloeser==0){
        if(count_ausloeser==0){
          startzeit=millis();
        }
        count_ausloeser++;
        if(count_ausloeser==3)
        {
          cachezeit=millis();
          if((cachezeit-startzeit)<2001)
          {
            count_ausloeser=0;
            flag_ausloeser=1;
            Serial.println("Scharf");
            delay(500);
          }
          else
          {
            count_ausloeser=0;
            flag_ausloeser=0;
            Serial.println("Zu langsam");
            delay(500);
          }
        }
      }//flag ausloeser==0
      if(flag_ausloeser==1)
      {
        Serial.println("Fire");
        switch_state(relais);
        delay(1000);
      }
    }

    EMGStats_Shorttime.clear();        
  }

}


// EMG Relais Driver for Arduino With Olimex Shield EMG 
// By Otto Schwarzwälder
// Serial Console Version (Relais Toggle)

