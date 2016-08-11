int sensorPin = 0;

int ledPin = 13;      // The LED is connected to digital pin 13

unsigned int average=0, average_10=0;
unsigned int signal_counter=0, null_counter=0, count_ausloeser;
unsigned char event_counter=0;
unsigned long startzeit, cachezeit;
bool flag, flag_ausloeser;
void setup() // this function runs once when the sketch starts up
{

flag=0;
flag_ausloeser=0;
count_ausloeser=0;
startzeit=0;
average=analogRead(sensorPin);
pinMode(ledPin, OUTPUT);
Serial.begin(57600);

}

void loop() // this function runs repeatedly after setup() finishes
{

int sensorValue;
unsigned char a = 0, b=0;

double differenz=0.0, bruch=0.0;
unsigned int prozent=0;

for(a=1;a<20;a++){
{
sensorValue = analogRead(sensorPin);
average=average+sensorValue;
average=average/2;
average_10=average_10+sensorValue;
average_10=average_10/a;
if(sensorValue >= 0){flag=1;}
}
//Serial.println((average_10/average));

} if(average_10>=average){differenz=average_10-average;};
if(average_10<average){differenz=average-average_10;};
prozent=differenz/average;
//Serial.println(prozent);
if(prozent>0.2 && flag){
//Serial.println(“Signal!”);
if(null_counter==0){
null_counter++;
signal_counter++;}
else if(null_counter>=16){
null_counter++;
signal_counter++;
bruch=(signal_counter/null_counter);
//Serial.println(bruch);
if((bruch>=0.70) && (signal_counter >= 6)){
signal_counter=0;
null_counter=0;
Serial.println("Ausloeser");}
}

else {
null_counter++;
signal_counter++;
bruch=(signal_counter/null_counter);
//Serial.println(bruch);
if((bruch>=0.70) && (signal_counter >= 6)){
signal_counter=0;
null_counter=0;
// 2 Serial.println("Ausloeser");
if(flag_ausloeser==0){
if(count_ausloeser==0){startzeit=millis();}
count_ausloeser++;
if(count_ausloeser==3)
{
cachezeit=millis();
if((cachezeit-startzeit)<1501)
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
delay(500);
}
}
}
}
else{
if(null_counter==0){}
else if(null_counter>=16){null_counter=0; signal_counter=0;}
else {
null_counter++;
bruch=(signal_counter/null_counter);
if((bruch>=0.70) && (signal_counter >= 6)){
signal_counter=0;
null_counter=0;
// 2    Serial.println(“Ausloeser”);
if(flag_ausloeser==0){
if(count_ausloeser==0){startzeit=millis();}
count_ausloeser++;
if(count_ausloeser==3)
{
cachezeit=millis();
if((cachezeit-startzeit)<1501)
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
delay(500);

}

}
}
if(startzeit!=0)
{
cachezeit=millis();
if((cachezeit-startzeit)>7500)
{
flag_ausloeser=0;
count_ausloeser=0;
startzeit=0;
Serial.println("Reset");
}
}
}

}
