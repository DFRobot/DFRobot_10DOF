/*
 * file 10DOF.ino
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
#include "SPI.h"

/*
 * pin_dataRdy  ----> int1
 * pin_accelInt ----> int2
 */
#ifdef __AVR__
uint8_t       pin_cs = 2, pin_dataRdy = 3, pin_accelInt = 4;
#else
uint8_t       pin_cs = D2, pin_dataRdy = D3, pin_accelInt = D4;
#endif

DFRobot_10DOF_SPI dof(pin_cs);
uint8_t       dataRdyFlag = 0, accelIntFlag = 0;

static inline void readAttitude(void)
{
  Serial.println();
  float pitch = 0, roll = 0, yaw = 0;
  dof.readAttitude(&pitch, &roll, &yaw);
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
  dof.readGyro(&GX, &GY, &GZ);
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

static inline void readEnvironment(void)
{
  Serial.println();
  int16_t temp = 0, humi = 0;
  int32_t pre;
  float   alt;
  dof.readEnvironment(&temp, &alt);
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
  pinMode(pin_dataRdy, INPUT_PULLUP);
  pinMode(pin_accelInt, INPUT_PULLUP);
  attachInterrupt(pin_dataRdy, dataRdyInterrupt, FALLING);
  attachInterrupt(pin_accelInt, accelInterrupt, FALLING);
  while(!Serial);
  SPI.begin();
  Serial.println();
  Serial.println("10dof test");
  
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
  dof.setMode(e10DOF_MODE_STANDERD);
  lastMillis = millis();
  while((millis() - lastMillis) < 1000) {
    readAttitude();
    readEnvironment();
    delay(1000 / 10);
  }
  delay(1000);
  Serial.println();

  Serial.println();
  Serial.println("test mode fast");  //output rate 500hz in fast mode and do not reference yaw data
  delay(1000);
  dof.setMode(e10DOF_MODE_FAST);
  lastMillis = millis();
  while((millis() - lastMillis) < 1000) {
    readAttitude();
    readEnvironment();
    delay(1000 / 10);
  }
  delay(1000);
  Serial.println();

  Serial.println();
  Serial.println("test data ready interrupt");  //when attitude data is ready, low level on pin 'int1'
  delay(1000);
  dof.enableDataInt();
  lastMillis = millis();
  while((millis() - lastMillis) < 1000) {
    if(dataRdyFlag) {
      dataRdyFlag = 0;
      Serial.println();
      float pitch = 0, roll = 0, yaw = 0;
      dof.readAttitude(&pitch, &roll, &yaw);
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
  dof.disableDataInt();
  delay(1000);
  Serial.println();

  Serial.println();
  Serial.println("test accelerometer interrupt");  //when accelerometer is overflow, low level on pin 'int2'
  delay(1000);
  dof.setAccelInt(0, 0);
  dof.enableAccelInt();
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
  dof.disableAccelInt();
  delay(1000);
  Serial.println();

  Serial.println();
  Serial.println("test LED control");  //test LED conrol on board
  dof.disableLED();
  delay(4000);
  dof.enableLED();
  delay(2000);
  Serial.println();

  Serial.println();
  Serial.println("test stop mode");  //test stop mode
  delay(1000);
  dof.setMode(e10DOF_MODE_STOP);
  delay(4000);
  dof.weakUp();  //spend time 500ms
  delay(500);
  Serial.println();

  Serial.println("--------  test end  --------");
  delay(1000);
}
