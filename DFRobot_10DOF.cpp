#include "DFRobot_10DOF.h"

int16_t DFRobot_10DOF::begin()
{
  uint8_t       id = 0;
  setMode(e10DOF_MODE_RESTART);
  delay(500);
  read(DOF_CONTROL_ID, &id, 1);
  if(id == 0x68) {
    setMode(e10DOF_MODE_STANDERD);
    return 0;
  }
  return -1;
}

void DFRobot_10DOF::readAttitude(float* pitch, float* roll, float* yaw)
{
  uint8_t       buf[12] = {0};
  read(DOF_MPUDATA_BASE, buf, 12);
  *pitch = *(float*)((uint8_t*)&buf[0]);
  *roll = *(float*)((uint8_t*)&buf[4]);
  *yaw = *(float*)((uint8_t*)&buf[8]);
  
//  read(DOF_CONTROL_BASE, buf, 2);
//  if((!(buf[1] & 0x01)) && (!(buf[0] & 0x01)))
//    while(!Serial.available());
}

void DFRobot_10DOF::readAccelerometer(int16_t* x, int16_t* y, int16_t* z)
{
  uint8_t       buf[6] = {0};
  read(DOF_MPUDATA_BASE + 18, buf, 6);
  *x = (float)(((int16_t)buf[1] << 8) | (int16_t)buf[0]);
  *y = (float)(((int16_t)buf[3] << 8) | (int16_t)buf[2]);
  *z = (float)(((int16_t)buf[5] << 8) | (int16_t)buf[4]);
}

void DFRobot_10DOF::readGyro(int16_t* x, int16_t* y, int16_t* z)
{
  uint8_t       buf[6] = {0};
  read(DOF_MPUDATA_BASE + 12, buf, 6);
  *x = (float)(((int16_t)buf[1] << 8) | (int16_t)buf[0]);
  *y = (float)(((int16_t)buf[3] << 8) | (int16_t)buf[2]);
  *z = (float)(((int16_t)buf[5] << 8) | (int16_t)buf[4]);
}

void DFRobot_10DOF::readMagnetic(int16_t* x, int16_t* y, int16_t* z)
{
  uint8_t       buf[6] = {0};
  read(DOF_MPUDATA_BASE + 24, buf, 6);
  *x = (float)(((int16_t)buf[1] << 8) | (int16_t)buf[0]);
  *y = (float)(((int16_t)buf[3] << 8) | (int16_t)buf[2]);
  *z = (float)(((int16_t)buf[5] << 8) | (int16_t)buf[4]);
}

void DFRobot_10DOF::readEnvironment(int16_t* temp, float* alt)
{
  uint8_t       buf[4] = {0};
  read(DOF_BMEDATA_TEMP, buf, 2);
  *temp = (int16_t)(((uint16_t)buf[1] << 8) | (uint16_t)buf[0]);
  read(DOF_BMEDATA_ALT, buf, 5);
  *alt = *(float*)buf;
}

void DFRobot_10DOF::setMode(e10DOF_mode_t eMode)
{
  switch(eMode) {
    case e10DOF_MODE_STANDERD: writeBit(DOF_CONTROL_POWER, DOF_POWERCON_MODE, DOF_POWERCON_MODE_STANDERD); break;
    case e10DOF_MODE_FAST: writeBit(DOF_CONTROL_POWER, DOF_POWERCON_MODE, DOF_POWERCON_MODE_FAST); break;
    case e10DOF_MODE_STOP: writeBit(DOF_CONTROL_POWER, DOF_POWERCON_MODE, DOF_POWERCON_MODE_STOP); break;
    case e10DOF_MODE_RESTART: writeBit(DOF_CONTROL_POWER, DOF_POWERCON_MODE, DOF_POWERCON_MODE_RESTART); break;
  }
}

void DFRobot_10DOF::setSeaLevel(float seaLevel)
{
  uint8_t*        pBuf = (uint8_t*)((float*)&seaLevel);
  write(DOF_BME_SEALEVEL, pBuf, 4);
}

void DFRobot_10DOF::setAccelInt(int16_t up, int16_t low)
{
  uint8_t       buf[4] = {(uint8_t)(up & 0x00ff), (uint8_t)(up >> 8), (uint8_t)(low & 0x00ff), (uint8_t)(low >> 8)};
  write(DOF_CONTROL_PEDOMETERH, buf, 4);
}

void DFRobot_10DOF::setAccelInt(float up, float low)
{
  setAccelInt((int16_t)(up / DOF_ACCEL_LSB), (int16_t)(low / DOF_ACCEL_LSB));
}

void DFRobot_10DOF::enableAccelInt()
{
  writeBit(DOF_CONTROL_EXTEND, 0x02, 0x02);
}

void DFRobot_10DOF::disableAccelInt()
{
  writeBit(DOF_CONTROL_EXTEND, 0x02, 0x00);
}

void DFRobot_10DOF::enableLED()
{
  writeBit(DOF_CONTROL_EXTEND, 0x01, 0x01);
}

void DFRobot_10DOF::disableLED()
{
  writeBit(DOF_CONTROL_EXTEND, 0x01, 0x00);
}

void DFRobot_10DOF::enableDataInt()
{
  writeBit(DOF_CONTROL_EXTEND, 0x04, 0x04);
}

void DFRobot_10DOF::disableDataInt()
{
  writeBit(DOF_CONTROL_EXTEND, 0x04, 0x00);
}

void DFRobot_10DOF::weakUp()
{
  digitalWrite(_weak, 0);
  delayMicroseconds(20);
  digitalWrite(_weak, 1);
}

uint8_t DFRobot_10DOF::readStatus()
{
  uint8_t       buf = 0;
  read(DOF_STATUS, &buf, 1);
  return buf;
}

void DFRobot_10DOF::writeBit(uint8_t reg, uint8_t bitAddr, uint8_t dat)
{
  uint8_t       buf = 0;
  read(reg, &buf, 1);
  buf &= ~bitAddr;
  buf |= dat;
  write(reg, &buf, 1);
}

void DFRobot_10DOF::setWeak(uint8_t weak)
{
  _weak = weak;
  pinMode(weak, OUTPUT);
  digitalWrite(weak, 1);
}

DFRobot_10DOF_SPI::DFRobot_10DOF_SPI(uint8_t cs)
{
  _cs = cs;
  setWeak(cs);
}

/*
 *
 * DFRobot_10DOF_SPI
 *
 *
*/

void DFRobot_10DOF_SPI::read(uint8_t reg, uint8_t* pBuf, uint8_t len)
{
  if(!len)
    return;
  DOF_SPI_BEGIN();
  digitalWrite(_cs, 0);
  delayMicroseconds(16);
  SPI.transfer(reg | 0x80);
  digitalWrite(_cs, 1);
  delayMicroseconds(32);
  digitalWrite(_cs, 0);
  delayMicroseconds(16);
  _DBG_CODE(Serial.println());
  *pBuf = SPI.transfer(reg | 0x80);
  _DBG_CODE(Serial.print(*pBuf, HEX));
  pBuf ++;
  len --;
  while(len --) {
    *pBuf = SPI.transfer(0x00);
    _DBG_CODE(
      Serial.print(" ");
      Serial.print(*pBuf, HEX);
    );
    pBuf ++;
  }
  _DBG_CODE(Serial.println());
  digitalWrite(_cs, 1);
  delayMicroseconds(16);
  SPI.endTransaction();
}

void DFRobot_10DOF_SPI::write(uint8_t reg, uint8_t* pBuf, uint8_t len)
{
  if(!len)
    return;
  DOF_SPI_BEGIN();
  digitalWrite(_cs, 0);
  delayMicroseconds(16);
  SPI.transfer(reg);
  SPI.transfer(len);
  while(len --) {
    SPI.transfer(*pBuf);
    pBuf ++;
  }
  digitalWrite(_cs, 1);
  delayMicroseconds(128);
  SPI.endTransaction();
}
