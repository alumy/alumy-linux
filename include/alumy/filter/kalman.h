#ifndef __AL_KALMAN_H
#define __AL_KALMAN_H

#include "alumy/types.h"
#include "alumy/base.h"

__BEGIN_DECLS

#define KALMAN_Q        0.01
#define KALMAN_R        10.00

typedef struct kalman_filter_s{
	double x_last;
	double p_last;
} kalman_filter_t;

/**
 * @brief Kalman filter init
 * 
 * @author chenyj (2019/11/13)
 * 
 * @param kf 
 * @param x_last Last x value
 * @param p_last Last p value
 */
void kalman_init(kalman_filter_t *kf, double x_last, double p_last);

/**
 * @brief Kalman filter
 * 
 * @author chenyj (2019/11/13)
 *
 * @param src Source value
 * @param q Process noise Q, Q increases, faster dynamic
 *  		response, bad convergence stability
 * @param r Measure noise R, R increases, slow dynamic response,
 *  		good convergence stability
 *
 * @return double
 */
double kalman_filter(kalman_filter_t *kf, double src, double q, double r);

__END_DECLS

#endif

