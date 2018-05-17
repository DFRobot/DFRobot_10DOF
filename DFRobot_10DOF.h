#ifndef DFRobot_10DOF_H
#define DFRobot_10DOF_H

#include "Arduino.h"

#define _DBG        0

#if _DBG
  #define _DBG_CODE(x)        x
#else
  #define _DBG_CODE(x)
#endif

#define DOF_STATUS   0x0f

#define DOF_STATUS_NEWMPU        0x80
#define DOF_STATUS_NEWATTI        0x40
#define DOF_STATUS_NEWBME        0x20
#define DOF_STATUS_NEWMAG        0x10
#define DOF_STATUS_COREND        0x08
#define DOF_STATUS_CONUSB        0x02

#define DOF_MPUDATA_BASE        0x10
#define DOF_BMEDATA_BASE        0x30
#define DOF_BMEDATA_TEMP        DOF_BMEDATA_BASE
#define DOF_BMEDATA_ALT         (DOF_BMEDATA_BASE + 8)

#define DOF_CONTROL_BASE         0x40

#define DOF_POWERCON_MODE        0x03
#define DOF_POWERCON_MODE_STANDERD       0x00
#define DOF_POWERCON_MODE_STANDERD_RATE  75
#define DOF_POWERCON_MODE_FAST           0x01
#define DOF_POWERCON_MODE_FAST_RATE      1000
#define DOF_POWERCON_MODE_STOP           0x02
#define DOF_POWERCON_MODE_RESTART        0x03

#define DOF_POWERCON_COR         0x80

#define DOF_EXTEND_LED           0x01
#define DOF_EXTEND_PEDOMETER     0x02
#define DOF_EXTEND_DATARDY       0x04

#define DOF_CONTROL_POWER        (0x00 + DOF_CONTROL_BASE)
#define DOF_CONTROL_EXTEND       (0x01 + DOF_CONTROL_BASE)
#define DOF_CONTROL_ID           (0x02 + DOF_CONTROL_BASE)
#define DOF_CONTROL_PEDOMETERH   (0x03 + DOF_CONTROL_BASE)
#define DOF_CONTROL_PEDOMETERL   (0x05 + DOF_CONTROL_BASE)
#define DOF_CONTROL_TEMPUH       (0x07 + DOF_CONTROL_BASE)
#define DOF_CONTROL_TEMPUL       (0x08 + DOF_CONTROL_BASE)
#define DOF_CONTROL_TEMPLH       (0x09 + DOF_CONTROL_BASE)
#define DOF_CONTROL_TEMPLL       (0x0a + DOF_CONTROL_BASE)

#define DOF_BME_BASE             0x60
#define DOF_BME_SEALEVEL         (0x00 + DOF_BME_BASE)

#define DOF_ACCEL_LSB        (2.0f * 16.0f / 65535.0f)
#define DOF_GYRO_LSB         (2.0f * 2000.0f / 65536.0f)
#define DOF_MAG_LSB          (2.0f * 8.1f / 4096)

typedef enum {
  e10DOF_MODE_STANDERD,
  e10DOF_MODE_FAST,
  e10DOF_MODE_STOP,
  e10DOF_MODE_RESTART
} e10DOF_mode_t;

class DFRobot_10DOF {
  public:
    DFRobot_10DOF() {}
    
    int16_t     begin();
    void        readAttitude(float* pitch, float* roll, float* yaw);
    void        readAccelerometer(int16_t* x, int16_t* y, int16_t* z);
    void        readGyro(int16_t* x, int16_t* y, int16_t* z);
    void        readMagnetic(int16_t* x, int16_t* y, int16_t* z);
    void        readEnvironment(int16_t* temp, float* alt);
    void        setMode(e10DOF_mode_t eMode);
    void        setSeaLevel(float seaLevel);
    void        setAccelInt(int16_t up, int16_t low);
    void        setAccelInt(float up, float low);
    void        enableAccelInt();
    void        disableAccelInt();
    void        enableLED();
    void        disableLED();
    void        enableDataInt();
    void        disableDataInt();
    void        weakUp();
    uint8_t     readStatus();
    
  protected:
    uint8_t       _weak;
    
    virtual void  read(uint8_t reg, uint8_t* pBuf, uint8_t len);
    virtual void  write(uint8_t reg, uint8_t* pBuf, uint8_t len);
    void          writeBit(uint8_t reg, uint8_t bitAddr, uint8_t dat);
    void          setWeak(uint8_t weak);
};

#include "SPI.h"

#define DOF_SPI_BEGIN()        SPI.begin();SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0))

class DFRobot_10DOF_SPI : public DFRobot_10DOF {
  
  public:
    DFRobot_10DOF_SPI(uint8_t cs);
  
  private:
    uint8_t       _cs;
  
    void  read(uint8_t reg, uint8_t* pBuf, uint8_t len);
    void  write(uint8_t reg, uint8_t* pBuf, uint8_t len);
};

#endif
