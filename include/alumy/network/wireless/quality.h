#ifndef __AL_WIRESS_QUALITY_H
#define __AL_WIRESS_QUALITY_H 1

#include "alumy/types.h"
#include "alumy/base.h"

__BEGIN_DECLS

int32_t al_rssi_dbm2quality(int32_t dbm);

int32_t al_rssi_quality2dbm(int32_t quality);

__END_DECLS

#endif
