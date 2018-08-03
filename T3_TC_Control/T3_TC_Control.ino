/*
Teensy 3.2, teensy #3 on the electronics control. This teensy will exclusively be used for
timecode. The timecode which is a SMPTE signal will interrupt the teensy every change in 
voltage level. Then teensy then processes the time since the last interrupt and determines 
if the received bit is a 1 or a 0. Once a completed timecode has been processed it will 
interrupt teeny #2 to relay the information. 
*/

#include <i2c_t3.h>

//Specifications for time between bits
//This will change if you change FPS or other parameters
#define one_time_max          275     //these values are setup for NTSC video
#define one_time_min          150     //PAL would be around 1000 for 0 and 500 for 1
#define zero_time_max         475     //80bits times 29.97 frames per sec
#define zero_time_min         350     //equals 833 (divide by 8 clock pulses)

//Defines the positioning of relevant timecode (SMPTE) data                                  
#define end_data_position     63      //End of data 
#define end_sync_position     77      //Beginning of 12 bits (1) to sync the signal
#define end_smpte_position    80      //End of 80 bits of SMPTE signal

const byte                    tcInterruptPin = 24;
volatile unsigned int         bit_time;
volatile unsigned int         bit_current = 0;
volatile boolean              valid_tc_word;
volatile boolean              ones_bit_count;
volatile boolean              tc_sync;
volatile boolean              write_tc_out;
volatile boolean              test1;
volatile boolean              drop_frame_flag;
volatile byte                 total_bits;
volatile byte                 current_bit;
volatile byte                 sync_count;
volatile byte                 tc[8];
//volatile char                 timeCode[8];
volatile char                 timeCode[9];

void setup() {
  //Start the I2C Bus as Master so that it can interrupt teensy #2
  Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_EXT, 400000);
  Serial.begin(115200);

  bit_time       = 0;
  valid_tc_word  = false;
  ones_bit_count = false;
  tc_sync        = false;
  write_tc_out   = false;
  drop_frame_flag= false;
  total_bits     = 0;
  current_bit    = 0;
  sync_count     = 0;

  //Define the function that is called each time the interrupt is triggered
  //pinMode(tcInterruptPin, INPUT);
  //attachInterrupt(digitalPinToInterrupt(tcInterruptPin), tcode, CHANGE);
  pinMode(tcInterruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(tcInterruptPin), tcode, CHANGE);
}

void loop() {
  //This waits until write_tc_out is true. This means that a complete timecode has been processed
  if (write_tc_out){
    write_tc_out = false;
    Serial.println((char*)timeCode);
    Wire.beginTransmission(7); // transmit to device #
    Wire.write((char*)timeCode);  // sends x  
    Wire.endTransmission();    // stop transmitting
  }
}

//When the interrupt is triggered this function is called  
void tcode(){
 bit_time = micros()-bit_current;
 bit_current = micros();
 //Serial.println(bit_time);

 if ((bit_time < one_time_min) || (bit_time > zero_time_max)) // get rid of anything way outside the norm
 {
   //Serial.println(bit_time, DEC);
   total_bits = 0;
 }
 else
 {
   if (ones_bit_count == true) // only count the second ones pluse
     ones_bit_count = false;
   else
   {    
     if (bit_time > zero_time_min)
     {
       current_bit = 0;
       sync_count = 0;
     }
     else //if (bit_time < one_time_max)
     {
       ones_bit_count = true;
       current_bit = 1;
       sync_count++;
       if (sync_count == 12) // part of the last two bytes of a timecode word
       {
         sync_count = 0;
         tc_sync = true;
         total_bits = end_sync_position;
       }
     }

     if (total_bits <= end_data_position) // timecode runs least to most so we need
     {                                    // to shift things around
       tc[0] = tc[0] >> 1;
 
       for(int n=1;n<8;n++)
       {
         if(tc[n] & 1)
           tc[n-1] |= 0x80;
       
         tc[n] = tc[n] >> 1;
       }
 
       if(current_bit == 1)
         tc[7] |= 0x80;
     }
     total_bits++;
   }

   if (total_bits == end_smpte_position) // we have the 80th bit
   {
     total_bits = 0;
     if (tc_sync)
     {
       tc_sync = false;
       valid_tc_word = true;
     }
   }
   
   if (valid_tc_word)
   {
     valid_tc_word = false;

     timeCode[7] = (tc[0]&0x0F)+0x30;     // frames
     timeCode[6] = (tc[1]&0x03)+0x30;      // 10's of frames
     //timeCode[8] =  '.';
     timeCode[5] = (tc[2]&0x0F)+0x30;      // seconds
     timeCode[4] = (tc[3]&0x07)+0x30;      // 10's of seconds
     //timeCode[5] =  ':';
     timeCode[3] = (tc[4]&0x0F)+0x30;      // minutes
     timeCode[2] = (tc[5]&0x07)+0x30;      // 10's of minutes
     //timeCode[2] = ':';
     timeCode[1] = (tc[6]&0x0F)+0x30;      // hours
     timeCode[0] = (tc[7]&0x03)+0x30;      // 10's of hours

/*
     timeCode[7] = (tc[0]&0x0F)+0x30;     // frames
     timeCode[6] = (tc[1]&0x03)+0x30;      // 10's of frames
     timeCode[5] = (tc[2]&0x0F)+0x30;      // seconds
     timeCode[4] = (tc[3]&0x07)+0x30;      // 10's of seconds
     timeCode[3] = (tc[4]&0x0F)+0x30;      // minutes
     timeCode[2] = (tc[5]&0x07)+0x30;      // 10's of minutes
     timeCode[1] = (tc[6]&0x0F)+0x30;      // hours
     timeCode[0] = (tc[7]&0x03)+0x30;      // 10's of hours    
     //drop_frame_flag = bit_is_set(tc[1], 2);
*/
     write_tc_out = true;
   }
 }
}
