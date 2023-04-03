#ifndef _SGP30_H_
#define _SGP30_H_

#include "iic.h"

void sgp30_Init(void);
void sgp30_SetHumi(void);
u8 sgp30_Get(int *co2, int *tvoc);

#endif