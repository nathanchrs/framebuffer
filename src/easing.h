#ifndef EASING_H
#define EASING_H

/* Robert Penner's easing equations */

double linearTween(double time, double start, double end, double duration);
double easeInQuadratic(double time, double start, double end, double duration);
double easeOutQuadratic(double time, double start, double end, double duration);
double easeOutQuartic(double time, double start, double end, double duration);

#endif
