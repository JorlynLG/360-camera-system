//This creates the physical interface for the arduino code CameraControl_interface
//THe code will create a folder that outputs IMU readings with TC
//Overall this should be abstracted out except that you need to change the COM dependent
//On what input it is on your computer.

import controlP5.*;
import processing.serial.*;

ControlP5 cp5;
Serial serial;
PrintWriter output;

//Variables for physical characteristics of the interface
int backcolor = 100;
int myColor = color(0,0,0);

void setup(){
  size(700,450);
  noStroke();
  //Here is the location where you should change coms(usually informatio "COM" on PC)
  serial = new Serial(this, "COM10", 9600);
  cp5 = new ControlP5(this);
  String filename = Integer.toString(month())+"-"+Integer.toString(day())+"-"+Integer.toString(year())+"_"+Integer.toString(hour())+"-"+Integer.toString(minute())+"-"+Integer.toString(second())+".txt";
  output = createWriter(filename);
  output.print("YAW");
  output.print("\t");
  output.print("PITCH");
  output.print("\t");
  output.print("ROLL");
  output.print("\t");
  output.print("HOUR");
  output.print("\t");
  output.print("MINUTE");
  output.print("\t");
  output.print("SECOND");
  output.print("\t");
  output.print("FRAME");
  output.print("\t");
  output.println("RETURN");

  cp5.addButton("ExposureUp")                //Exposure up
    .setPosition(575,125)
    .setSize(50,50)
    ;
  cp5.addButton("ExposureDown")              //Exposure down
    .setPosition(275,125)
    .setSize(50,50)
    ;
  cp5.addButton("GainUp")                    //Gain up
    .setPosition(575,225)
    .setSize(50,50)
    ;
  cp5.addButton("GainDown")                  //Gain down
    .setPosition(275,225)
    .setSize(50,50)
    ;
  cp5.addButton("WbUp")                      //White Balance up
    .setPosition(575,325)
    .setSize(50,50)
    ;
  cp5.addButton("WbDown")                    //White Balance down
    .setPosition(275,325)
    .setSize(50,50)
    ;
   cp5.addToggle("Camera1")                  //Button for camera 1
    .setPosition(50,25)
    .setSize(50,50)
    ;
   cp5.addToggle("Camera2")                  //Button for camera 2
    .setPosition(125,25)
    .setSize(50,50)
    ;
   cp5.addToggle("Camera3")                  //Button for camera 3
    .setPosition(200,25)
    .setSize(50,50)
    ;
   cp5.addToggle("Camera4")                  //Button for camera 4
    .setPosition(275,25)
    .setSize(50,50)
    ;
   cp5.addToggle("Camera5")                  //Button for camera 5
    .setPosition(350,25)
    .setSize(50,50)
    ;
   cp5.addToggle("Camera6")                  //Button for camera 6
    .setPosition(425,25)
    .setSize(50,50)
    ;
  loop();
}

void draw(){
  background(backcolor);
  if (serial.available()>0){
    char test = serial.lastChar();
    if (test == '1'){
    //println(serial.readString());
    //if (serial.peek()
      output.print(serial.readString());
      output.println();
      println("yes");
    }
    else if(test =='0'){
      output.print(serial.readString());
      output.print("\t");
      output.println();
      println("no");
    }
    else{
      serial.clear();
    }
  }
}

void ExposureUp(int value){
  serial.clear();
  serial.write("a");
}
void ExposureDown(int value){
  serial.clear();
  serial.write("b");
}
void GainUp(int value){
  serial.clear();
  serial.write("c");
}
void GainDown(int value){
  serial.clear();
  serial.write("d");
}
void WbUp(int value){
  serial.clear();
  serial.write("e");
}
void WbDown(int value){
  serial.clear();
  serial.write("f");
}

void Camera1(boolean status){
    serial.clear();
    if (status){serial.write("1");}
    else{serial.write("A");}
}
void Camera2(boolean status){
    serial.clear();
    if (status){serial.write("2");}
    else{serial.write("B");} 
}
void Camera3(boolean status){
    serial.clear();
    if (status){serial.write("3");}
    else{serial.write("C");} 
}
void Camera4(boolean status){
    serial.clear();
    if (status){serial.write("4");}
    else{serial.write("D");}
}
void Camera5(boolean status){
    serial.clear();
    if (status){serial.write("5");}
    else{serial.write("E");}
}
void Camera6(boolean status){
    serial.clear();
    if (status){serial.write("6");}
    else{serial.write("F");}
}