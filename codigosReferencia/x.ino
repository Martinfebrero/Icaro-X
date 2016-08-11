//############### small sample of larger code included here, does not include void loop() nor variable establishment.     
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
