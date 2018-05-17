/*
 * file testAll.ino
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
 * Copyright   [DFRobot](http://www.dfrobot.com), 2016
 * Copyright   GNU Lesser General Public License
 *
 * version  V1.0
 * date  2018-04-23
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

//create dof object, give cs pin
DFRobot_10DOF_SPI dof(pin_cs);

uint8_t       dataRdyFlag = 0, accelIntFlag = 0;

//print attitude data (pitch, roll, yaw, gyro x, gyro y, gyro z, accelerometer x, accelerometer y, accelerometer z)
static inline void readAttitude(void)
{
  Serial.println();
  float pitch = 0, roll = 0, yaw = 0;
  dof.readAttitude(&pitch, &roll, &yaw);  //get attitude data
  Serial.println("attitude: ");
  Serial.print("pitch :");
  Serial.print(((float)pitch));
  Serial.print(" ");
  Serial.print("roll :");
  Serial.print(((float)roll));
  Serial.print(" ");
  Serial.print("yaw :");
  Serial.print(((float)yaw));
  Serial.print(" ");
  
  Serial.println();
  int16_t GX = 0, GY = 0, GZ = 0;
  dof.readGyro(&GX, &GY, &GZ);  //get gyro data
  Serial.println("gyroscope: ");
  Serial.print("GX :");
  Serial.print(GX);
  Serial.print(" ");
  Serial.print("GY :");
  Serial.print(GY);
  Serial.print(" ");
  Serial.print("GZ :");
  Serial.print(GZ);
  Serial.print(" ");
  
  Serial.println();
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
  Serial.print(" ");
}

//print enviroment data (temperature in Celsius, altitude in meter)
static inline void readEnvironment(void)
{
  Serial.println();
  int16_t temp = 0, humi = 0;
  float   alt;
  dof.readEnvironment(&temp, &alt);  //read enviroment data, include temperature and altitude
  Serial.print("temp: ");
  Serial.print(((float)temp) / 100);
  Serial.print(" ");
  Serial.print("altitude: ");
  Serial.print(alt);
  Serial.print(" ");
}

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
  delay(500);
  //set up interrupt pin
  pinMode(pin_dataRdy, INPUT_PULLUP);
  pinMode(pin_accelInt, INPUT_PULLUP);
  attachInterrupt(pin_dataRdy, dataRdyInterrupt, FALLING);
  attachInterrupt(pin_accelInt, accelInterrupt, FALLING);
  while(!Serial);  //wait serial open
  SPI.begin();
  Serial.println();
  Serial.println("10dof test");
  dof.weakUp();  //weak up
  
  while(dof.begin() != 0) {  //spend time 500ms
    Serial.println("dof begin faild !");
    delay(2000);
  }
  Serial.println("dof begin successful !");
  delay(2000);
}

void loop()
{
  unsigned long       lastMillis = 0;
  Serial.println();

  Serial.println();
  Serial.println("test mode standerd");  //output rate 75hz in standerd mode
  delay(1000);
  dof.setMode(e10DOF_MODE_STANDERD);  //set mode to standerd
  lastMillis = millis();
  while((millis() - lastMillis) < 1000) {  //test 1s
    readAttitude();
    readEnvironment();
    delay(1000 / 10);
  }
  delay(1000);
  Serial.println();

  Serial.println();
  Serial.println("test mode fast");  //output rate 500hz in fast mode and do not reference yaw data
  delay(1000);
  dof.setMode(e10DOF_MODE_FAST);  //set mode to fast
  lastMillis = millis();
  while((millis() - lastMillis) < 1000) { //test 1s
    readAttitude();
    readEnvironment();
    delay(1000 / 10);
  }
  delay(1000);
  Serial.println();

  Serial.println();
  Serial.println("test data ready interrupt");  //when attitude data is ready, low level on pin 'int1'
  delay(1000);
  dof.enableDataInt();  //enable data ready interrupt
  lastMillis = millis();
  while((millis() - lastMillis) < 1000) {  //test 1s
    if(dataRdyFlag) {
      dataRdyFlag = 0;
      Serial.println();
      float pitch = 0, roll = 0, yaw = 0;
      dof.readAttitude(&pitch, &roll, &yaw);  //read attitude
      Serial.println("attitude: ");
      Serial.print("pitch :");
      Serial.print(((float)pitch));
      Serial.print(" ");
      Serial.print("roll :");
      Serial.print(((float)roll));
      Serial.print(" ");
      Serial.print("yaw :");
      Serial.print(((float)yaw));
      Serial.print(" ");
    }
  }
  dof.disableDataInt();  //disable data ready interrupt
  delay(1000);
  Serial.println();

  Serial.println();
  Serial.println("test accelerometer interrupt");  //when accelerometer is overflow, low level on pin 'int2'
  delay(1000);
  dof.setAccelInt(0, 0);  //set accelerometer data overflow threshold, first is upper value, second is lower value
  dof.enableAccelInt();  //enable accelerometer data overflow interrupt
  lastMillis = millis();
  while((millis() - lastMillis) < 1000) {
    if(accelIntFlag) {
      accelIntFlag = 0;
      Serial.println();
      int16_t AX = 0, AY = 0, AZ = 0;
      dof.readAccelerometer(&AX, &AY, &AZ);
      Serial.println("accelerometer: ");
      Serial.print("AX :");
      Serial.print(AX);
      Serial.print(" ");
      Serial.print("AY :");
      Serial.print(AY);
      Serial.print(" ");
      Serial.print("AZ :");
      Serial.print(AZ);
      Serial.print(" ");
    }
  }
  dof.disableAccelInt();  //disable accelerometer data overflow interrupt
  delay(1000);
  Serial.println();

  Serial.println();
  Serial.println("test LED control");  //test LED conrol on board
  dof.disableLED();  //disable LED
  delay(4000);
  dof.enableLED();  //enable LED
  delay(2000);
  Serial.println();

  Serial.println();
  Serial.println("test stop mode");  //test stop mode
  delay(1000);
  dof.setMode(e10DOF_MODE_STOP);  //set mode to stop
  delay(4000);
  dof.weakUp();  //spend time 500ms
  delay(500);
  Serial.println();

  Serial.println("--------  test end  --------");
  delay(1000);
}
