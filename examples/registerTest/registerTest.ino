/*
 * file registerTest.ino
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
 * test register
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
uint8_t       pin_cs = 4, pin_dataRdy = 2, pin_accelInt = 3;
#else
uint8_t       pin_cs = D4, pin_dataRdy = D2, pin_accelInt = D3;
#endif

//create dof object, give cs pin
DFRobot_10DOF_SPI dof(pin_cs);

void setup()
{
  Serial.begin(115200);
  while(! Serial);

  SPI.begin();
  Serial.println();
  Serial.println("10dof register test");
  dof.weakUp();  //weak up
  
  while(dof.begin() != 0) {  //spend time 500ms
    Serial.println("dof begin faild !");
    delay(2000);
  } 
  Serial.println("dof begin successful !");
  dof.setSeaLevel(1015.0f);  //set sea level pressure
  delay(2000);
}

void loop()
{
  Serial.print("test read enviroment");
  int16_t temp = 0, humi = 0;
  float   alt;
  dof.readEnvironment(&temp, &alt);  //read enviroment data, include temperature and altitude
  Serial.print("temp: ");
  Serial.print(((float)temp) / 100);
  Serial.print(" ");
  Serial.print("altitude: ");
  Serial.print(alt);
  Serial.println();
  delay(2000);

  Serial.println("disable LED");
  dof.disableLED();  //disable LED
  delay(4000);
  dof.enableLED();  //enable LED

  Serial.println("enter stop mode");
  dof.setMode(e10DOF_MODE_STOP);  //set mode to stop
  delay(4000);
  dof.weakUp();  //weak up
  delay(1000);
}
