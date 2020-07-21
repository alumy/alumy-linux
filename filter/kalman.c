#include "alumy/types.h"
#include "alumy/base.h"
#include "alumy/filter/kalman.h"

__BEGIN_DECLS

void kalman_init(kalman_filter_t *kf, double x_last, double p_last)
{
	kf->p_last = p_last;
	kf->x_last = x_last;
}

double kalman_filter(kalman_filter_t *kf, double src, double q, double r)
{
	double x_mid, x_now;
	double p_mid, p_now;
	double kg;

	/* x_last = x(k-1|k-1), x_mid = x(k|k-1) */
	x_mid = kf->x_last;
	/* p_mid = p(k|k-1), p_last = p(k-1|k-1), Q is the noise */
	p_mid = kf->p_last + q;
	/* kg is kalman filter，R is the noise */
	kg = p_mid / (p_mid + r);
	/* Estimated the optimal value */
	x_now = x_mid + kg * (src - x_mid);
	/* The covariance to the optimal value */
	p_now = (1 - kg) * p_mid;
	/* Update covariance */
	kf->p_last = p_now;
	/* Update system status value */
	kf->x_last = x_now;

	return x_now;
}

__END_DECLS

