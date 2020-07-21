/**
 * Copyright (C), 2018, alumy info&tech Co., Ltd
 *
 * This file include wireless quality apis
 *
 * @brief Wireless quality utils
 * @file quality.c
 * @author chenyj
 * @version 0.0.1
 * @date 2019/07/18
 * @note Get more about algorithm at
 *  	 https://docs.microsoft.com/zh-cn/windows/win32/api/wlanapi/ns-wlanapi-wlan_association_attributes
 * @since 0.0.1 Initial version
 */
#include "alumy/types.h"
#include "alumy/base.h"

__BEGIN_DECLS

#define RSSI_DBM_MAX		(-50)
#define RSSI_DBM_MIN		(-100)

/**
 * @brief rssi dBm to percent value
 *
 *
 * @author chenyj (2019/7/18)
 *
 * @param dbm The dBm value
 *
 * @return int32_t the percent quality value
 */
int32_t al_rssi_dbm2quality(int32_t dbm)
{
	if (dbm <= RSSI_DBM_MIN) {
		return 0;
	}

	if (dbm >= RSSI_DBM_MAX) {
		return 100;
	}

	return (2 * (dbm + 100));
}

int32_t al_rssi_quality2dbm(int32_t quality)
{
	if (quality <= 0) {
		return RSSI_DBM_MIN;
	}

	if (quality >= 100) {
		return RSSI_DBM_MAX;
	}

	return (quality / 2) - 100;
}

__END_DECLS

