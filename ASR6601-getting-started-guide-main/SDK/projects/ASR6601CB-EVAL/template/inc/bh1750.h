#ifndef _BH1750_H_
#define _BH1750_H_

#include "iic.h"
#include "gwlog.h"

u8 bh1750_Start(void);
float bh1750_Get(void);

#endif