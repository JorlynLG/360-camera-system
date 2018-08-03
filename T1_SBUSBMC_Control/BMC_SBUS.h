
#ifndef BMC_SBUS_h
#define BMC_SBUS_h

#include <Arduino.h>

#define BAUDRATE 100000
#define SERIAL1 Serial1
#define SERIAL2 Serial2
#define SERIAL3 Serial3
#define SERIAL4 Serial4
#define SERIAL5 Serial5
#define SERIAL6 Serial6
#define wbLength 18
#define gainLength 4
#define exposureLength 15
#define irisLength 22
#define sbusMID 1024   //neutral val
#define sbusLOW 0      //low switch val
#define sbusHIGH 2000   //high switch val
#define sbusWAIT 4      //frame timing delay in msecs

//#define SERIALPORT SERIAL1  - uncomment this line if using an arduino based board with more than one HW serial port

class BMC_SBUS
{
	public:
		uint8_t packet[25] = {0x0f,0x01,0x04,0x20,0x00,0xff,0x07,0x40,0x00,0x02,0x10,0x80,0x2c,0x64,0x21,0x0b,0x59,0x08,0x40,0x00,0x02,0x10,0x80,0x00,0x00};
		bool camStatus[6] = {true,false,false,false,false,false};

		int16_t channels[6][16] = {
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		};
		//int channels[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
		int exposureValues[15] = {60,60,75,90,100,120,150,180,250,360,500,725,1000,1450,2000};
		int exposureCurrent = 2;
		int gainValues[4] = {0,6,12,18};
		int gainCurrent = 1;
		int wbValues[18] = {2500,2800,3000,3200,3400,3600,4000,4500,4800,5000,5200,5400,5600,6000,6500,7000,7500,8000};
		int wbCurrent = 1;
		int irisValues[22] = {400,464,528,592,656,720,784,848,912,976,1040,1104,1168,1232,1296,1360,1424,1488,1552,1616,1680,1744};
		//float irisValues[22] = {2.0,2.2,2.4,2.6,2.8,3.2,3.5,3.7,4.0,4.5,4.8,5.2,5.6,6.2,6.7,7.3,8.0,8.7,9.5,10.0,11.0,12.0};
		int irisCurrent = 1;
		bool camRec[6] = {false,false,false,false,false,false};
		int iris = 1;
		int focus = 2;
		int zoom = 3;
		int gain = 4;
		int exposure = 5;
		int wb = 6;
		int levels = 7;
		int rec = 8;
		void begin(void);
		void Servo(uint8_t ch, int16_t position);
		void Send(int value);
		void Update(int value);
		void setHigh(int setting);
		void setLow(int setting);

	private:
		const uint8_t _sbusHeader = 0x0F;
  		const uint8_t _sbusFooter = 0x00;
		uint8_t ch;
};

#endif