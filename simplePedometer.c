#include "simplePedometer.h"

void pedometerInit(sPedometer_t* psP, float threshold)
{
  psP->threshold = threshold;
}

int calcPedometer(sPedometer_t* psP, int timeInterval, float accelX, float accelY, float accelZ)
{
  float deltaX = accelX - psP->lastAccelX;
  float deltaY = accelY - psP->lastAccelY;
  float deltaZ = accelZ - psP->lastAccelZ;
  psP->lastAccelX = accelX;
  psP->lastAccelY = accelY;
  psP->lastAccelZ = accelZ;

  float speed = sqrtf(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ) / timeInterval;
  if(speed >= psP->threshold) {
    return 1;
  }
  return 0;
}
