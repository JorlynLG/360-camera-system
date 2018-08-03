/*
This is teensy 1. It works on a teeny 3.6. It operates entirely to send information to the BMC.
There are 6 cameras, each recieving separate information from the hardware serial ports. Therefore
all 6 hardware serial ports are dedicated to sending infomation to cameras. The teensy receives 
and sends out information via two i2c communcation lines with teensy #2. Teensy #1 is a slave on 
Wire1. This means that the code will be interrupted every time a command is received from teensy #2. 
Teensy #1 is a master on Wire2. The mean that it will interrupt teensy #2 every time it has 
completed a command.

Channel setup per mode - map BMMCC camera functions to sbus channels as below
  ++ means that the SBUS commands work by moving the associated value either up or dodwn
  -- means that the SBUS commands work by moving the associated value directly to the value you input

ch01: Iris    (controlled by discrete values) --
ch02: Focus   (controlled by discrete values) --
ch03: Zoom    (controlled by discrete values) --
ch04: ISO     (controlled by up/down)         ++ 
ch05: Exposure(controlled by up/down)         ++
ch06: WB      (controlled by up/down)         ++
ch07: Levels  (controlled by discrete values) --
ch08: REC     (controlled by up/down)         --
*/

#include "./BMC_SBUS.h"
#include <i2c_t3.h>
BMC_SBUS mySBUS; 

//Variables for messages to cameras
char command;    
String payload;
String data;

void setup() {
  //Initialise Serial and SBUS
  Serial.begin(9600);
  mySBUS.begin();

  //Initialise i2c communication between teensys
  Wire1.begin(I2C_SLAVE, 9, I2C_PINS_37_38, I2C_PULLUP_EXT, 400000);                     //Receives information from teensy2 
  Wire2.begin(I2C_MASTER, 0x00, I2C_PINS_3_4, I2C_PULLUP_EXT, 400000, I2C_OP_MODE_IMM);  //Sends information to teensy2
  Wire1.onReceive(receiveEvent);                                                         //Function that is called when interrupt from teeny 2 received
}

//Called when teensy #2 sends teensy #1 information for the cameras
void receiveEvent(size_t bytes) {
  char val = Wire1.read();  //This value is the command that tells the camera which case it is
  Wire1.flush();            //Clear the buffer for future commands

  
  /*
  //This code was used when I tried to make the system more robust by sending commands like <command,val>
  //instead of just command,val.
  while(Wire1.available()){
    char val = Wire1.read();
    data.append(val);    
  }
  
  Serial.println(data);
  int commandindex = data.lastIndexOf('<');           //Parses out the beginning of the command
    int commandend = data.lastIndexOf('>');           //Parses out the end of the command
    int camstop = data.lastIndexOf(',');              //Parses out the spacer
    if (commandindex != -1 && commandend != -1){
      command = data.charAt(commandindex+1);          //Format of the input over serial is in the form:
    }
    if (camstop != -1 && commandend != -1){
      payload = data.substring(camstop+1, commandend);//<commandpayload,cameras>
    }
    */
    //This will switch between all test cases. Camera status is stored (commands on or off) are stored on this teensy
    switch (val){
      case '1':                                       //Turns on Camera 1
        mySBUS.camStatus[0] = true;
        Wire2.beginTransmission(8);                   //Transmit to teensy #2
        Wire2.write("Camera 1 is on");                //Send info
        //Wire2.write(mySBUS.camStatus[0]);           //Send info
        Wire2.endTransmission();                      //Stop transmitting
        break;
      case 'A':                                       //Turns off Camera 1
        mySBUS.camStatus[0] = false;
        Wire2.beginTransmission(8);
        Wire2.write("Camera 1 is off");                 
        //Wire2.write(mySBUS.camStatus[0]);           
        Wire2.endTransmission();                      
        break;
      case '2':                                       //Turns on Camera 2
        mySBUS.camStatus[1] = true;
        Wire2.beginTransmission(8);                  
        Wire2.write("Camera 2 is on");
        //Wire2.write(mySBUS.camStatus[1]);            
        Wire2.endTransmission();                     
        break;
      case 'B':                                       //Turns off Camera 2
        mySBUS.camStatus[1] = false;
        Wire2.beginTransmission(8); 
        Wire2.write("Camera 2 is off");
        //Wire2.write(mySBUS.camStatus[1]);   
        Wire2.endTransmission();     
        break;  
      case '3':                                       //Turns on Camera 3
        mySBUS.camStatus[2] = true;
        Wire2.beginTransmission(8); 
        Wire2.write("Camera 3 is on");
        //Wire2.write(mySBUS.camStatus[2]);  
        Wire2.endTransmission(); 
        break;
      case 'C':                                       //Turns off Camera 3
        mySBUS.camStatus[2] = false;
        Wire2.beginTransmission(8); 
        Wire2.write("Camera 3 is off");
        //Wire2.write(mySBUS.camStatus[2]);
        Wire2.endTransmission();
        break;   
      case '4':                                       //Turns on Camera 4
        mySBUS.camStatus[3] = true;
        Wire2.beginTransmission(8);
        Wire2.write("Camera 4 is on");
        //Wire2.write(mySBUS.camStatus[3]);
        Wire2.endTransmission();
        break;
      case 'D':                                       //Turns off Camera 4
        mySBUS.camStatus[3] = false;
        Wire2.beginTransmission(8);
        Wire2.write("Camera 4 is off");
        //Wire2.write(mySBUS.camStatus[3]);
        Wire2.endTransmission();
        break;    
      case '5':                                       //Turns on Camera 5
        mySBUS.camStatus[4] = true;
        Wire2.beginTransmission(8);
        Wire2.write("Camera 5 is on");
        //Wire2.write(mySBUS.camStatus[4]);
        Wire2.endTransmission();
        break;
      case 'E':                                       //Turns off Camera 5
        mySBUS.camStatus[4] = false;
        Wire2.beginTransmission(8);
        Wire2.write("Camera 5 is off");
        //Wire2.write(mySBUS.camStatus[4]);
        Wire2.endTransmission();
        break; 
      case '6':                                       //Turns on Camera 6
        mySBUS.camStatus[5] = true;
        Wire2.beginTransmission(8);
        Wire2.write("Camera 6 is on");
        //Wire2.write(mySBUS.camStatus[5]);
        Wire2.endTransmission();
        break;
      case 'F':                                       //Turns off Camera 6
        mySBUS.camStatus[5] = false;
        Wire2.beginTransmission(8); 
        Wire2.write("Camera 6 is off");
        //Wire2.write(mySBUS.camStatus[5]);
        Wire2.endTransmission();
        break;    
      case 'a':                                       //Sets the exposure up one
        mySBUS.setHigh(mySBUS.exposure);
        Wire2.beginTransmission(8);
        Wire2.write("Exposure up");
        //Wire2.write(mySBUS.exposureCurrent);
        Wire2.endTransmission();
        break;
      case 'b':                                       //Sets the exposure down one
        mySBUS.setLow(mySBUS.exposure);
        Wire2.beginTransmission(8);
        Wire2.write("Exposure down");
        //Wire2.write(mySBUS.exposureCurrent);
        Wire2.endTransmission();
        break;
      case 'c':                                       //Sets the gain up one
        mySBUS.setHigh(mySBUS.gain);
        Wire2.beginTransmission(8);
        Wire2.write("Gain up");
        Wire2.write(mySBUS.gainCurrent);
        Wire2.endTransmission();
        break;
      case 'd':                                       //Sets the gain down one
        mySBUS.setLow(mySBUS.gain);
        Wire2.beginTransmission(8); 
        Wire2.write("Gain down");
        //Wire2.write(mySBUS.gainCurrent);  
        Wire2.endTransmission();    
        break;
      case 'e':                                       //Sets the white balance
        mySBUS.setHigh(mySBUS.wb);
        Wire2.beginTransmission(8);
        Wire2.write("White Balance up");
        //Wire2.write(mySBUS.wbCurrent); 
        Wire2.endTransmission();    
        break;
      case 'f':                                       //Sets the white balance
        mySBUS.setLow(mySBUS.wb);
        Wire2.beginTransmission(8); 
        Wire2.write("White Balance down");
        //Wire2.write(mySBUS.wbCurrent); 
        Wire2.endTransmission();
        break;
      /*
      case 'g':                                       //Sets the aperture
        mySBUS.irisCurrent = payload.toInt();
        mySBUS.Servo(mySBUS.iris, mySBUS.irisValues[mySBUS.irisCurrent]);
        //Serial.print(mySBUS.irisCurrent);
        Wire2.beginTransmission(8); 
        Wire2.write(mySBUS.irisCurrent); 
        Wire2.endTransmission();    
        break;
      case 'h':                                       //Sets the focus
        mySBUS.focusCurrent = payload.toInt();
        mySBUS.Servo(mySBUS.focus, payload.toInt());
        //Serial.print(payload.toInt());
        Wire1.print(payload.toInt());
        break;
        */
      default:                                        //If the command is none of these options  
        break;
    }
  data = "";
  payload = "";
  command = '\0';
}

void loop(){
}
