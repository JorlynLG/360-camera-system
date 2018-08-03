//This code controls the intermediate teensy 3.6. This teensy receives 2 interupt signals.
//The first interrupt comes from the tri-level sync signal, 30 times a second. It triggers
//All the information to be captured and send at the exact same time as the frames. 
//The second interrupt comes from a third teensy which works exclusively to capture timecode
//information and triggers the interrupt when a new timecode has been received. 
//finally 

#include <i2c_t3.h> //This library is specific to the teensy and allows for robust I2C comms
//#include <Wire.h>
#include "./NXPMotionSense.h"
#include <EEPROM.h>

//NXPMotionSense imu;
//NXPSensorFusion filter;

int x = 0;
int led = 13;
const byte interruptPin = 2; //This is the Sync interrupt pin
volatile byte state = LOW;
int value = 0;
//IMU Values
float ax, ay, az;
float gx, gy, gz;
float mx, my, mz;
float roll, pitch, heading;
volatile char timeCode[11];
boolean syncTriggered = false;

void setup() {
  // Start the I2C Bus as Master
  //Wire.begin();
  //These wires create communication channels between teensys. Wire1 creates a master interface with teensy1
  //Wire2 creates a slave interface with teensy1 and Wire3 creates a slave interface with teensy3
  Wire1.begin(I2C_MASTER, 0x00, I2C_PINS_37_38, I2C_PULLUP_EXT, 400000, I2C_OP_MODE_IMM);
  Wire2.begin(I2C_SLAVE, 8, I2C_PINS_3_4, I2C_PULLUP_EXT, 400000);
  Wire3.begin(I2C_SLAVE, 7, I2C_PINS_56_57, I2C_PULLUP_EXT, 400000);
  //Wire1.begin();
  //Wire2.begin(8);
  Wire2.onReceive(receiveEvent);//This means that we have received an event (or command) to change camera status
  Wire3.onReceive(tcEvent);
  Serial.begin(9600);
  //Interrupt
  pinMode(led, OUTPUT); 
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, RISING);
  //IMU initiation
  //imu.begin();
  //filter.begin(100);
}
void loop() {
  //When the sync triggers an interrupt, syncTriggered is turned to true which in turn sends all the 
  //relevant information up to the computer
  if (syncTriggered){
    Serial.print(heading);
    Serial.print("\t");
    Serial.print(pitch);
    Serial.print("\t");
    Serial.print(roll);
    Serial.print("\t");
    //Serial.println((char*)timeCode);
    Serial.print(timeCode[0]);
    Serial.print(timeCode[1]);
    Serial.print("\t");
    Serial.print(timeCode[2]);
    Serial.print(timeCode[3]);
    Serial.print("\t");
    Serial.print(timeCode[4]);
    Serial.print(timeCode[5]);
    Serial.print("\t");
    Serial.print(timeCode[6]);
    Serial.print(timeCode[7]);
    Serial.print('1');
    syncTriggered = false;
  }
  //checks to see if any additional information has 
  if (Serial.available()){
    Wire1.beginTransmission(9); // transmit to device #9
    while (Serial.available()>0){
      //Serial.println(Serial.peek());
      Wire1.write(Serial.read());  // sends x  
    }
    Wire1.endTransmission();    // stop transmitting
  }
  /*
  if (imu.available()) {
    // Read the motion sensors
    imu.readMotionSensor(ax, ay, az, gx, gy, gz, mx, my, mz);

    // Update the SensorFusion filter
    filter.update(gx, gy, gz, ax, ay, az, mx, my, mz);

    // print the heading, pitch and roll
    roll = filter.getRoll();
    pitch = filter.getPitch();
    heading = filter.getYaw();
  }
  */
}
//This triggers when the computer sends a command for the cameras
void receiveEvent(size_t bytes) {
  int counter = 0;
  char info[20];
  while(Wire2.available()){
    info[counter] = Wire2.read();
    //Serial.println(Wire2.readString());
    counter += 1;
  }
  Serial.print((char*)info);
  Serial.print('0');
}
//This is triggered when the tc completes a reading
void tcEvent(size_t bytes){
  int count = 0;
  while(Wire3.available()){
    timeCode[count] = Wire3.read(); 
    count += 1;  
  }
  //Serial.println((char*)timeCode);
}
//When sync interrupts the controller it sets syncTriggered to true which then triggers future events
void blink() {
  syncTriggered = true;
}

