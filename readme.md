# 10DOF

10-DOF inertial guidance sensor, providing accurate Euler angle information: When the USB access, <br>
can cooperate with our software Imu_show(https://github.com/DFRobot/DFRobot_IMU_Show) real-time transmission display current posture (in the default standard mode), <br>
and other MCU through the SPI interface Communication transmission attitude information and temperature, altitude information. <br>

## DFRobot_10DOF Library for Arduino
---------------------------------------------------------
Provide a library faciltates operations in the 10dof modules.

## Table of Contents

* [Summary](#summary)
* [Feature](#feature)
* [Installation](#installation)
* [Methods](#methods)

* [Compatibility](#compatibility)
* [Credits](#credits)
<snippet>
<content>

## Summary

Input commands and read data from 10dof modules

## Feature

1. Use SPI interface to communicate with other MCU, rate 8MHz <br>
2. Standard mode (enable geomagnetic sensor) data output frequency of 75hz <br>
3. Fast mode (no geomagnetic sensor is enabled) data output frequency 500hz, but yaw angle data does not have reference value. <br>
4. Get 10-axis data: 16-bit 2000-degree/sec three-axis gyroscope raw data, <br>
16-digit positive and negative 16g/second triaxial accelerometer raw data, <br>
13-bit positive and negative 8.1g/second three-axis geomagnetic compass original data. <br>
5. The onboard LED blink is usable via SPI control. <br>
6. You can enter and exit low power mode, reboot the system initialization 10-axis sensor when exiting low power mode, 
set parameters before entering low power mode to reset. <br>
7. Interrupt signal can be generated when data preparation is complete (attitude data only) <br>
8. You can set the upper and lower limits of the acceleration and interrupt signal when any of the upper or lower limits overflow. <br>

## Installation

Download the library ZIP file and unzip it to the Arduino folder of the library.<br>

## Methods

```C++

#include "DFRobot_10DOF.h"

/*
 * @brief 10DOF object
 *
 * @param pin_cs        10DOF cs pin
 */
DFRobot_10DOF_SPI(uint8_t pin_cs);

/*
 * @brief lib begin
 *
 * @return 0        successed
 *         other    error
 */
int16_t begin(void);

/*
 * @brief read attitude data
 *
 * @param *pitch        pitch data container
 *        *roll         roll data container
 *        *yaw          yaw data container
 */
void readAttitude(float* pitch, float* roll, float* yaw);

/*
 * @brief read accelerometer data
 *
 * @param *x        raw x-axis accelerometer data container
 *        *y        raw y-axis accelerometer data container
 *        *z        raw z-axis accelerometer data container
 */
void readAccelerometer(int16_t* x, int16_t* y, int16_t* z);

/*
 * @brief read gyro data
 *
 * @param *x        raw x-axis gyro data container
 *        *y        raw y-axis gyro data container
 *        *z        raw z-axis gyro data container
 */
void readGyro(int16_t* x, int16_t* y, int16_t* z);

/*
 * @brief read magnetic data
 *
 * @param *x        raw x-axis magnetic data container
 *        *y        raw y-axis magnetic data container
 *        *z        raw z-axis magnetic data container
 */
void readMagnetic(int16_t* x, int16_t* y, int16_t* z);

/*
 * @brief read environment data
 *
 * @param *temp        temperature data container
 *        *alt         altitude data container
 */
void readEnvironment(int16_t* temp, float* alt);

typedef enum {
  e10DOF_MODE_STANDERD,
  e10DOF_MODE_FAST,
  e10DOF_MODE_STOP,
  e10DOF_MODE_RESTART
} e10DOF_mode_t;

/*
 * @brief set modules mode
 *
 * @param eMode       mode
 */
void setMode(e10DOF_mode_t eMode);

/*
 * @brief set modules sea level pressure
 *
 * @param seaLevel        sea level pressure
 */
void setSeaLevel(float seaLevel);

/*
 * @brief set accelerometer interrupt overflow
 *
 * @param up        upper limit
 *        low       lower limit
 */
void setAccelInt(int16_t up, int16_t low);

/*
 * @brief enable accelerometer interrupt
 */
void enableAccelInt();

/*
 * @brief disable accelerometer interrupt
 */
void disableAccelInt();

/*
 * @brief enable led blink
 */
void enableLED();

/*
 * @brief disable led blink
 */
void disableLED();

/*
 * @brief enable attitude data ready interrupt
 */
void enableDataInt();

/*
 * @brief disable attitude data ready interrupt
 */
void disableDataInt();

/*
 * @brief weak up modules
 */
void weakUp();

```

## Compatibility

MCU                | Work Well | Work Wrong | Untested  | Remarks
------------------ | :----------: | :----------: | :---------: | -----
FireBeetle-ESP32  |      √       |             |            | 
FireBeetle-ESP8266  |      √       |             |            | 
FireBeetle-BLE4.1 |       √      |             |            | 
Arduino uno |       √      |             |            | 
Arduino leonardo |      √       |             |            | 

## Credits

Written by DFRobot_xiaowo, 2018. (Welcome to our [website](https://www.dfrobot.com/))
