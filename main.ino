// The SdFat library supports FAT16, FAT32, and exFAT file systems on Standard SD, SDHC, and SDXC cards.
// * SD library does not support exFAT. *
#include "SdFat.h"

#define INT_T 2                // Interrupt time [ms] 


IntervalTimer intTimer;


SdExFat sd;
ExFile fp;

// constants won't change. Used here to set a pin number:
const int ledPin =  LED_BUILTIN;// the number of the LED pin


int global_time_ms = 0;         // time [ms]




void setup() {
  // put your setup code here, to run once:
  // set the digital pin as output:
  pinMode(ledPin, OUTPUT);

  // USB serial (to PC)
  Serial.begin( 921600); 

  // SD access
  sd.begin( SdioConfig( FIFO_SDIO));
  sd.remove( "data.log"); // delete if "data.log" file is aleady exist 
  fp = sd.open( "data.log", FILE_WRITE);

  // timer interrupt
  //FlexiTimer2::set( INT_T, flip); 
  //FlexiTimer2::start();  
  // for Teensy 4.x, FlexiTimer2 doesn't exactly interrupt.  
  intTimer.begin( flip, INT_T*1000); // [us]  
}




//--------------------------------------------------------------
//---------------------- timer interrupt function ----------------------
//--------------------------------------------------------------
void flip(){

  float out[3] = { 0 };
  short w_data[5] = { 0 };          // logging data(2byte)
  char w_cdata[5*2] = { 0 };        // logging data(1byte)
  int idx = 0;
  

  //[*] ------------- time [ms] -------------------------
  global_time_ms += INT_T;
  global_time_ms = (global_time_ms < 1000*1000) ? global_time_ms : 0; 

  //[*] ------------- data [-] -------------------------
  out[0] = 2*sin( 2*PI*(float)global_time_ms/1000.0);
  out[1] = cos( 2*PI*(float)global_time_ms/1000.0);
  out[2] = 0.5*sin( 2*PI*2.0*(float)global_time_ms/1000.0);
  
  //[*] -------- short int(2bytes) convert ---------------
  w_data[0] = 0xAAAA;
  w_data[1] = (short)( global_time_ms );
  for(idx=0;idx<3;idx++) 
    w_data[2+idx] = (short)( 100*out[idx] );

  //[*] ------------- char(1bytes) convert --------------- 
  // 2byte -> 1byte (Little Endian)
  for(idx=0;idx<5;idx++){
    w_cdata[2*idx+1] = (char)( (w_data[idx] >> 8) & 0x00FF );
    w_cdata[2*idx] = (char)( w_data[idx] & 0x00FF );
  }
   
  //[*] ----------- Write to binary file --------------
  if( global_time_ms <= 60*1000 ){    
    
    // write  
    if( fp ) 
      fp.write( w_cdata, 5*2); // Write 1 bytes at a time.
    
  }else{
    if( fp )
      fp.close();
  }
     
  //[*] ---------- send to PC -----------------------------       
  Serial.printf( "AA,%f[s],%f[-],%f[-],%f[-]\r\n", (float)global_time_ms/1000.0, out[0], out[1], out[2]);  
}





void loop() {
  // put your main code here, to run repeatedly:
  // set the LED with the ledState of the variable:
  digitalWrite(ledPin, 1);
  delay(100); // [ms]
  digitalWrite(ledPin, 0);
  delay(100); // [ms]
}
