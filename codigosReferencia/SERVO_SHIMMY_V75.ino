
//  SERVO_SHIMMY_V72
// BY BRIAN MATTHEWS flappingsprocket.com 
// built by example from arduino servo code examples 
// This software allows for 2 servos to follow a sine function between any two positions
// with a changeable ramp up start speed, ramp down end speed and different start times.   
// input:  one push button to start sequence, same button pushed to return to original position 
// setup:  microsecond range on each servo for desired begining and end position
// the speed of the servo starting it's movement is established at main void loop located at end, with descriptions.  

//Setup:  
//servo power needs 4.8 to 6 vown battery with common ground with board, 9v to arduino, all grounds are shared. 



#include <Servo.h> 
 
Servo servo1;   
Servo servo2;    

//shared variables 

float pie=3.14159; 

int sinsize=1700; //can not be <400 or min servo resolution default sin wave size (trough to peak or pie in length)
float event=sinsize * 15;  //cycles count limiter not really important as long as speed of servo isn't super slow  


//specific variables 
// servo default positions 
// WARNING:  when a higher or lower number is input than what the servo can handle, 
//           the servo may rapid to some manufacturer safe value 
// WARNING: refer to technical documentation for each servo to stay within its range. 
//  ccw   \->/   cw
//   600- 1200- 2400   =180 sweep servo  type 1800 point resolution
//  1050- 1550 - 1950   =90 sweep servo  type  900 point resolution
//  1500- 1700 - 1900   =1260 sweep winch type 400 point resolution 

//servo1 left shoulder 1500-1900 decrease to open
int svo1c   =2200;  //servo# closed position
int svo1o   =800;   //servo# open position 

//servo2 right shoulder 1500-1900 increase to open 
int svo2c   =800;  //servo# closed position
int svo2o   =2200; //servo# open position 


//variables for use in all servo functions per servo

float count1 =0; // sine wave x variable counts from 1 to 1700 (servo resolution) only starts counting after wait# reaches its activation value.                 
float speed1=0;  //a value that consists of small increment values that change in magnitude depending on if the wave starts slow and ends fast or vise versa.  
float speedtick1=0; //ticks off how long the hold position for the servo is in very small ms increments giving the illusion of a slower or faster moving servo 

                 // y= a*sin(bx+c)+d 
                 // yvar# = a#(sin (b#*count#+c#)+d#  
float yvar1=0;   // actual ms value thrown at servo ranged, paused, speed shifted etc.  
float a1=0;     //a# amplitude higher value taller wave shorter value shorter wave by magnitude:   a=(highest # - lowest #)/2
float b1=0;     //b# lower value = longer wave or higher value=shorter wave this is phase shift or stretch of function b=2pi/(blabla*2) where blabla are desired wave size 
float c1=0;     //c# is x frequency offset = what part of curve want to look at 
float d1=0;     //d# is y offset  = 0.5*amplitude shifts the curve so it is wholey in 1st quadrant 
int per1=0;     //trigger value either 0 or 1 to declare that that servo has reached its final position and so servo movement sequence of all servos (once all report per#=1)can end.

float count2 =0; 
float speed2=0;
float speedtick2=0; 
float yvar2=0;
float a2=0; 
float b2=0; 
float c2=0; 
float d2=0; 
int per2=0;


//start of primary move function that includes all servos and is called up and activated per each event
void movef(float ecycle,float w81,float spa1,float spb1,float yprev1,float ynext1,float w82,float spa2,float spb2,float yprev2,float ynext2)
{
         
         delay(1);   //master delay 
         count1 = 1;   //reset of count# 
         speedtick1 = 1;  //reset of speedtick#
         b1=(2*pie/(sinsize*2));  //coefficient of sine math function 
         
         count2 = 1; 
         speedtick2 = 1;    
         b2=(2*pie/(sinsize*2));     
          
            if(ynext1 > yprev1)  //sets sine wave coefficients depending on positions
            {
              a1= (ynext1-yprev1)/2;  //coefficient of sine math function 
              c1= (1.5)*pie;           //coefficient of sine math function 
              d1= yprev1+a1;            //coefficient of sine math function 
            }
            else  //(ynext# < yprev#)
            {
              a1= (yprev1-ynext1)/2;
              c1= (0.5)*pie;  
              d1= yprev1-a1; 
            }
              if(ynext2 > yprev2)
            {
              a2= (ynext2-yprev2)/2;  
              c2= (1.5)*pie; 
              d2= yprev2+a2; 
            }
            else  //(ynext# < yprev#)
            {
              a2= (yprev2-ynext2)/2;
              c2= (0.5)*pie;  
              d2= yprev2-a2; 
            }
            
              //###coefficient establishment for all sine waves end ###
            
            int per1=0; //reset of all trigger values in case there is carry over from a previous run
            int per2=0; 
           
              
       //global loop for all servos cycles by each servo and sees if it has an tiny move.                             
       for (float count = 0; count < ecycle; count +=1)  
       {  
         //traditional speed values start off as spa# and end up as spb# as count# ticks away on the fly as curve is being drawn. 
        // result is a sine curve that is compressed in the x axis on one end (spa#=large number) and stretched on other end (spb#=small number).   
         
         if (spa1 > spb1) {speed1=((count1+1)/sinsize)*(spa1-spb1) + spb1;} //start fast end slow 
         else {speed1= ((count1+1)/sinsize)*(spb1-spa1)+ spa1;} // start slow end fast 
         
         if (spa2 > spb2) {speed2=((count2+1)/sinsize)*(spa2-spb2) + spb2;} //start fast end slow 
         else  {speed2= ((count2+1)/sinsize)*(spb2-spa2)+ spa2;} // start slow end fast    
         
       
        
  // servo #1   3 states or cases 
        if (count < w81) //case 1 servo not ready to move yet      
            {
            servo1.writeMicroseconds(yprev1);  
            
            }
        
         else if (count>w81 && count1 > sinsize) //case 3 motion is done and position is held 
            {
            servo1.writeMicroseconds(ynext1); 
            
            per1=1; //declares this servo is finished with its movement 
            }
         
         else if (count > w81)   //case 2 sin wave function active with optional hold position while big loop asks other servos for their turn  
            {
              
              if (count1 < sinsize && speedtick1 == 1)  //new position of servo is written 
                {   
                   yvar1= a1*sin((count1)*b1+c1)+d1;  //the math function
                   servo1.writeMicroseconds(yvar1);   //throws a command at the servo 
                   speedtick1 += 1; // start of increment to count for possible pauses at this position to simulate slow 
                   count1 += 1; //increments sine wave operator x in y=f(x) 
                   
                }
                else if (speedtick1 > 1 && speedtick1 < speed1)  //sine wave is sustained at old value for 1 to speed# as counted by speedtick# 
                {
                  servo1.writeMicroseconds(yvar1); 
                  speedtick1 += 1;  //increments speedtick1 to delay the servo at one position along its travel to simulate a speed
                  
                }
                else //sine wave is now permitted to continue by having speedtick# reset 
                {
                  count1+=1; //lock out the sine function from going past sinsize by ever increasing count# 
                 speedtick1 = 1; //reset for next fly through of sine function 
                 
                }  
            }  //end if statement for case 2
 
       //servo #2   
          if (count < w82)     //notes same as servo #1 above 
            {
            servo2.writeMicroseconds(yprev2);  
           
            }
         
         else if (count>w82 && count2 > sinsize) 
            {
            servo2.writeMicroseconds(ynext2); 
            
            per2=1; 
            }
         
         else if (count > w82)    
            {
               
              if (count2 < sinsize && speedtick2 == 1)
                {   
                   yvar2= a2*sin((count2)*b2+c2)+d2;  
                   servo2.writeMicroseconds(yvar2);
                   speedtick2 += 1;   
                   count2 += 1; 
                 }
                else if (speedtick2 > 1 && speedtick2 < speed2)  
                {
                  servo2.writeMicroseconds(yvar2); 
                  speedtick2 += 1;
                  
                }
                else 
                {
                  count2+=1; 
                 speedtick2 = 1; 
                }  
            }    
         
       if(per1 == 1 && per2 == 1)//breaks FOR loop out of further un necessary cycles as all servos report their movement complete
         {
          break; 
         }
    
   }  //end of for loop statement for all servo steps  
 
} //end of void subroutine function for entire move function 


// start of program meat 
void setup() 
{   
  servo1.attach(5);     //signal wire of servo#1 to pin 3
  servo2.attach(3);     //signal wire of servo#2 to pin 5
  
  
 
} 
void loop() 
{  
  //servo lockdown when power is on
  servo1.writeMicroseconds(svo1c);  
  servo2.writeMicroseconds(svo2c);
  

  //notes: 
  
//void movef(float ecycle,float w81,float spa1,float spb1,float yprev1,float ynext1.....
// ecycle = time acts as the cut off when all servos have completed their one way motion 
// w8#= each servo wait time after new movef function starts. higher longer til servo moves start, lower, shorter til servo moves start 
// spa# = start speed, range: 1-25 or 1 fast, 25 slow   
// spb# = end speed, range: 1-25 or 1 fast, 25 slow   
// yprev# = previous servo position 
// ynext# = next position 
// 
//Servo servo1;   
//Servo servo2;   

 //notes end 
 
  //#########open 
  
  movef(event,1100,25,25,svo1c,svo1o,  7000,5,25,svo2c,svo2o);
  delay(2000); 
  movef(event,5000,25,25,svo1o,svo1c,  1100,1,15,svo2o,svo2c);
  delay(2000); 


} //end void loop      
  







