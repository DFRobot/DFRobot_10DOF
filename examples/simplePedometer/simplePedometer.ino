/*
 * file simplePedometer.ino
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
 * pedometer example, shake device and print count
 *
 * Copyright   [DFRobot](http://www.dfrobot.com), 2016
 * Copyright   GNU Lesser General Public License
 *
 * version  V1.0
 * date  2018-05-16
 */

#include "DFRobot_10DOF.h"
#include <SPI.h>

#include "simplePedometer.h"

/*
 * pin_dataRdy  ----> int1
 * pin_accelInt ----> int2
 */
#ifdef __AVR__
uint8_t       pin_cs = 4, pin_dataRdy = 2, pin_accelInt = 3;
#else
uint8_t       pin_cs = D4, pin_dataRdy = D2, pin_accelInt = D3;
#endif

//change variable here to get a suitable test
#define TIME_INTERVAL       8
#define SAMPLE_LEN          8
#define SAMPLE_THRESHOLD    0.35f

//create dof object, give cs pin
DFRobot_10DOF_SPI dof(pin_cs);

sPedometer_t        sPedometer;
uint16_t            pedometerCount;
float               sampleX[SAMPLE_LEN], sampleY[SAMPLE_LEN], sampleZ[SAMPLE_LEN];
int                 sampleCount = 0;

//get max in array ant its subscript
#define MAX_IN_ARRAY(array, len, result, sub) \
          result = array[0]; \
          for(int var_i = 1; var_i < len; var_i ++) { \
            if(result < array[var_i]) { \
              result = array[var_i]; \
              sub = var_i; \
            } \
          }

//get min in array ant its subscript
#define MIN_IN_ARRAY(array, len, result, sub) \
          result = array[0]; \
          for(int var_i = 1; var_i < len; var_i ++) { \
            if(result > array[var_i]) { \
              result = array[var_i]; \
              sub = var_i; \
            } \
          }

//full array with data
#define FULL_ARRAY(array, len, dat) \
          for(int var_i = 0; var_i < len; var_i ++) { \
            array[var_i] = dat; \
          }

void setup()
{
  Serial.begin(115200);
  while(! Serial);

  SPI.begin();
  Serial.println();
  Serial.println("10dof pedometer example");
  dof.weakUp();  //weak up
  
  while(dof.begin() != 0) {  //spend time 500ms
    Serial.println("dof begin faild !");
    delay(2000);
  }
  Serial.println("dof begin successful !");
  dof.setMode(e10DOF_MODE_FAST);  //set mode to fast
  pedometerInit(&sPedometer, SAMPLE_THRESHOLD);  //init pedometer threshold
  delay(2000);
}

void loop()
{
  static unsigned long        lastMillis = 0;
  if((millis() - lastMillis) > TIME_INTERVAL) {  //TIME_INTERVAL ms circle
    lastMillis = millis();

    int16_t       accelX, accelY, accelZ;
    dof.readAccelerometer(&accelX, &accelY, &accelZ);  //get accelerometer data
    //convert adc data to analog data
    float         analogX = accelX * DOF_ACCEL_LSB;
    float         analogY = accelY * DOF_ACCEL_LSB;
    float         analogZ = accelZ * DOF_ACCEL_LSB;

    sampleX[sampleCount] = analogX;
    sampleY[sampleCount] = analogY;
    sampleZ[sampleCount] = analogZ;

    float       maxSampleX, maxSampleY, maxSampleZ;
    float       minSampleX, minSampleY, minSampleZ;
    int         maxXSub, maxYSub, maxZSub;
    int         minXSub, minYSub, minZSub;
    //get max and min data in sample
    MAX_IN_ARRAY(sampleX, SAMPLE_LEN, maxSampleX, maxXSub);
    MAX_IN_ARRAY(sampleY, SAMPLE_LEN, maxSampleY, maxYSub);
    MAX_IN_ARRAY(sampleZ, SAMPLE_LEN, maxSampleZ, maxZSub);
    MIN_IN_ARRAY(sampleX, SAMPLE_LEN, minSampleX, minXSub);
    MIN_IN_ARRAY(sampleY, SAMPLE_LEN, minSampleY, minYSub);
    MIN_IN_ARRAY(sampleZ, SAMPLE_LEN, minSampleZ, minZSub);

    int         timeInterval = 0;
    //if accelerometer value overflow, get time interval
    if((maxSampleX - minSampleX) >= SAMPLE_THRESHOLD) {
      timeInterval = ((sampleCount + SAMPLE_LEN - minXSub) % SAMPLE_LEN) * TIME_INTERVAL;
    } else if((maxSampleY - minSampleY) >= SAMPLE_THRESHOLD) {
      timeInterval = ((sampleCount + SAMPLE_LEN - minYSub) % SAMPLE_LEN) * TIME_INTERVAL;
    } else if((maxSampleZ - minSampleZ) >= SAMPLE_THRESHOLD) {
      timeInterval = ((sampleCount + SAMPLE_LEN - minZSub) % SAMPLE_LEN) * TIME_INTERVAL;
    }

    if(timeInterval) {
      if(calcPedometer(&sPedometer, timeInterval, analogX, analogY, analogZ)) {  //return 1 if pedometer action
        Serial.print("pedometer to ");
        Serial.println(++ pedometerCount);
        //clear sample
        FULL_ARRAY(sampleX, SAMPLE_LEN, analogX);
        FULL_ARRAY(sampleY, SAMPLE_LEN, analogY);
        FULL_ARRAY(sampleZ, SAMPLE_LEN, analogZ);
      }
    }

    sampleCount ++;
    if(sampleCount == SAMPLE_LEN) {
      sampleCount = 0;
    }
  }
}
