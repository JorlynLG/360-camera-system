/*
 *** This code has been modified from the original FUTABA_SBUS by Jorlyn Le Garre***
Origianl code found here:
BMC_SBUS version 0.1.0 May 14th 2016 Stu Aitken
A simplified library for writing out serial servo data in Futaba SBUS format 

explictly written to control Blackmagic Design micro cameras, but shoud work generally

note for RC use various important flags such as failsafe have been omiited here
please refer to original source Libray in this case

based on FUTABA_SBUS library written by Michael Baker 
see: https://github.com/mikeshub/FUTABA_SBUS
also see : http://forum.arduino.cc/index.php/topic,99708.0.html

initial reverse engineering of sbus protocol by Uwe Gartmann 
see: https://developer.mbed.org/users/Digixx/notebook/futaba-s-bus-controlled-by-mbed/

SBUS ©FUTABA CORPORATION 2016. 
*/

#include "BMC_SBUS.h"

void BMC_SBUS::begin()
{
  //intialise private data arrays
  //sbus_data is formatted for correct serial output
  //note that the actual 11bit sbus data for each channel is embedded across multiple data bytes in a very stange order
  //byte 1 and bytes 24 and 25 should be left as is 
  //the first is a start byte, the last is a stop byte and the second last holds various flags
  //servos is the internal per channel position and is more straightforward - one int_16 per channel
  //setup serial port to transmit at 100k baud and use 1 parity and 2 stop bits

  SERIAL1.begin(BAUDRATE, SERIAL_8E2_RXINV_TXINV);
  SERIAL2.begin(BAUDRATE, SERIAL_8E2_RXINV_TXINV);
  SERIAL3.begin(BAUDRATE, SERIAL_8E2_RXINV_TXINV);
  SERIAL4.begin(BAUDRATE, SERIAL_8E2_RXINV_TXINV);
  SERIAL5.begin(BAUDRATE, SERIAL_8E2_RXINV_TXINV);
  SERIAL6.begin(BAUDRATE, SERIAL_8E2_RXINV_TXINV);

  //setup public data arrays
}

void BMC_SBUS::Servo(uint8_t ch, int16_t position) 
{
  //set servo position on single channel
  if ((ch>0)&&(ch<=16)) 
  {
	constrain (position, 0, 2048); //keep within min/max values
	for (int x=0; x<6; x++){
		if (camStatus[x]){
			channels[x][ch-1] = position; //expects a non zero starting index to the channel
			Update(x);
			Send(x);
		}
	}
    
  }
}
void BMC_SBUS::Send(int value){
	if(value == 0){
  		SERIAL1.write(packet, 25);
  	}
	if(value == 1){
  		SERIAL2.write(packet, 25);
  	}
  	if(value == 2){
  		SERIAL3.write(packet, 25);
  	}
  	if(value == 3){
  		SERIAL4.write(packet, 25);
  	}
  	if(value == 4){
  		SERIAL5.write(packet, 25);
  	}
  	if(value == 5){
  		SERIAL6.write(packet, 25);
  	}
}
void BMC_SBUS::setHigh(int setting){
  Servo(setting, sbusMID);
  delay(sbusWAIT); //frame timing 
  Servo(setting, sbusHIGH);
  delay(sbusWAIT); //frame timing 
}
void BMC_SBUS::setLow(int setting){
  Servo(setting, sbusMID);
  delay(sbusWAIT); //frame timing 
  Servo(setting, sbusLOW);
  delay(sbusWAIT); //frame timing 
}

void BMC_SBUS::Update(int value) 
{
  //update positions for all servo channels within the SBUS data frame
  //ignores digital servos and any failsafe mode stuff that was originally written

  //clear out existing sbus data for all channel data bytes
  //ignores first and last bytes in the array (start and stop bytes)
  //mapping loop relies on initial 0 values - do not omit this step!
  uint8_t i;
  for (i=1; i<24; i++) 
  {
    packet[i] = 0;
  }

  packet[0] = _sbusHeader; 

  // 16 channels of 11 bit data
    packet[1] = (uint8_t) ((channels[value][0] & 0x07FF));
    packet[2] = (uint8_t) ((channels[value][0] & 0x07FF)>>8 | (channels[value][1] & 0x07FF)<<3);
    packet[3] = (uint8_t) ((channels[value][1] & 0x07FF)>>5 | (channels[value][2] & 0x07FF)<<6);
    packet[4] = (uint8_t) ((channels[value][2] & 0x07FF)>>2);
    packet[5] = (uint8_t) ((channels[value][2] & 0x07FF)>>10 | (channels[value][3] & 0x07FF)<<1);
    packet[6] = (uint8_t) ((channels[value][3] & 0x07FF)>>7 | (channels[value][4] & 0x07FF)<<4);
    packet[7] = (uint8_t) ((channels[value][4] & 0x07FF)>>4 | (channels[value][5] & 0x07FF)<<7);
    packet[8] = (uint8_t) ((channels[value][5] & 0x07FF)>>1);
    packet[9] = (uint8_t) ((channels[value][5] & 0x07FF)>>9 | (channels[value][6] & 0x07FF)<<2);
    packet[10] = (uint8_t) ((channels[value][6] & 0x07FF)>>6 | (channels[value][7] & 0x07FF)<<5);
    packet[11] = (uint8_t) ((channels[value][7] & 0x07FF)>>3);
    packet[12] = (uint8_t) ((channels[value][8] & 0x07FF));
    packet[13] = (uint8_t) ((channels[value][8] & 0x07FF)>>8 | (channels[value][9] & 0x07FF)<<3);
    packet[14] = (uint8_t) ((channels[value][9] & 0x07FF)>>5 | (channels[value][10] & 0x07FF)<<6);  
    packet[15] = (uint8_t) ((channels[value][10] & 0x07FF)>>2);
    packet[16] = (uint8_t) ((channels[value][10] & 0x07FF)>>10 | (channels[value][11] & 0x07FF)<<1);
    packet[17] = (uint8_t) ((channels[value][11] & 0x07FF)>>7 | (channels[value][12] & 0x07FF)<<4);
    packet[18] = (uint8_t) ((channels[value][12] & 0x07FF)>>4 | (channels[value][13] & 0x07FF)<<7);
    packet[19] = (uint8_t) ((channels[value][13] & 0x07FF)>>1);
    packet[20] = (uint8_t) ((channels[value][13] & 0x07FF)>>9 | (channels[value][14] & 0x07FF)<<2);
    packet[21] = (uint8_t) ((channels[value][14] & 0x07FF)>>6 | (channels[value][15] & 0x07FF)<<5);
    packet[22] = (uint8_t) ((channels[value][15] & 0x07FF)>>3);

  // flags
  packet[23] = 0x00;

  // footer
  packet[24] = _sbusFooter; 
}
