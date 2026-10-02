//kpm.h
#ifndef KPM_H
#define KPM_H

#include "../common/types.h"

void Init_KPM(void);

u32 ColScan(void);
u32 RowCheck(void);
u32 ColCheck(void);
u32 KeyScan(void);

u32 ReadNum(void);

#endif
