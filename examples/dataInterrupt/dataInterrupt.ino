/*
 * file dataInterrupt.ino
 *
 * @ https://github.com/DFRobot/DFRobot_10DOF
 *
 * connect with your board (please reference board compatibility)
 *
 * 10-DOF inertial guidance sensor, providing accurate Euler angle information: When the USB access,
 * can cooperate with our software Imu_show(https://github.com/DFRobot/DFRobot_IMU_Show) 
 * real-time transmission display current posture (in the default standard mode),
 * and other MCU through the SPI interface Communication transmission attitude information and temperature, altitude information.
 * 
 * test data interrupt
 *
 * Copyright   [DFRobot](http://www.dfrobot.com), 2016
 * Copyright   GNU Lesser General Public License
 *
 * version  V1.0
 * date  2018-05-16
 */

#include "DFRobot_10DOF.h"
#include <SPI.h>

/*
 * pin_dataRdy  ----> int1
 * pin_accelInt ----> int2
 */
#ifdef __AVR__
uint8_t       pin_cs = 4, pin_dataRdy = 0, pin_accelInt = 1;
#else
uint8_t       pin_cs = D4, pin_dataRdy = D2, pin_accelInt = D3;
#endif

DFRobot_10DOF_SPI dof(pin_cs);

uint8_t       dataRdyFlag = 0, accelIntFlag = 0;

//test data ready interrupt
void dataRdyInterrupt()
{
  dataRdyFlag = 1;
}

//test accelerometer overflow interrupt
void accelInterrupt()
{
  accelIntFlag = 1;
}

void setup()
{
  Serial.begin(115200);
  //setup interrupt pin
  pinMode(pin_dataRdy, INPUT_PULLUP);
  pinMode(pin_accelInt, INPUT_PULLUP);
  attachInterrupt(pin_dataRdy, dataRdyInterrupt, FALLING);
  attachInterrupt(pin_accelInt, accelInterrupt, FALLING);
  while(! Serial);

  SPI.begin();
  Serial.println();
  Serial.println("10dof interrupt test");
  dof.weakUp();  //weak up
  
  while(dof.begin() != 0) {  //spend time 500ms
    Serial.println("dof begin faild !");
    delay(2000);
  } 
  Serial.println("dof begin successful !");
  dof.enableDataInt();  //enable data ready interrupt
  dof.setAccelInt(1.0f, -1.0f);  //set accelerometer overflow interrupt threshold to 1g/s and -1g/s
  dof.enableAccelInt();  //enable acccelerometer overflow interrupt
  delay(2000);
}

void loop()
{
  if(dataRdyFlag == 1) { //data ready interrupt
    dataRdyFlag = 0;

    float pitch, roll, yaw;
    dof.readAttitude(&pitch, &roll, &yaw);  //get attitude data
    Serial.println("attitude: ");
    Serial.print("pitch:");
    Serial.print(pitch);
    Serial.print(" roll:");
    Serial.print(roll);
    Serial.print(" yaw:");
    Serial.print(yaw);
    Serial.print(" ");
    Serial.println();
  }
  
  if(accelIntFlag == 1) { //accelerometer overflow interrupt
    accelIntFlag = 0;

    int16_t AX = 0, AY = 0, AZ = 0;
    dof.readAccelerometer(&AX, &AY, &AZ);  //get accelerometer data
    Serial.println("accelerometer: ");
    Serial.print("AX :");
    Serial.print(AX);
    Serial.print(" ");
    Serial.print("AY :");
    Serial.print(AY);
    Serial.print(" ");
    Serial.print("AZ :");
    Serial.print(AZ);
    Serial.println();
    delay(500);
  }
}
