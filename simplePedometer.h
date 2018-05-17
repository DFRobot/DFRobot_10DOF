#ifndef __SIMPLEP_PEDOMETER_H
#define __SIMPLEP_PEDOMETER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

typedef struct {
  float       lastAccelX;
  float       lastAccelY;
  float       lastAccelZ;

  float       threshold;
} sPedometer_t;

void      pedometerInit(sPedometer_t* psP, float threshold);
int       calcPedometer(sPedometer_t* psP, int timeInterval, float accelX, float accelY, float accelZ);

#ifdef __cplusplus
}
#endif

#endif
