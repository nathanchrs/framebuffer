#ifndef EASING_H
#define EASING_H

/* Robert Penner's easing equations */

double linearTween(double start, double end, double time, double duration);

double easeInQuadratic(double start, double end, double time, double duration);
double easeOutQuadratic(double start, double end, double time, double duration);
double easeInOutQuadratic(double start, double end, double time, double duration);

double easeInCubic(double start, double end, double time, double duration);
double easeOutCubic(double start, double end, double time, double duration);
double easeInOutCubic(double start, double end, double time, double duration);

double easeInQuartic(double start, double end, double time, double duration);
double easeOutQuartic(double start, double end, double time, double duration);
double easeInOutQuartic(double start, double end, double time, double duration);

double easeInQuintic(double start, double end, double time, double duration);
double easeOutQuintic(double start, double end, double time, double duration);
double easeInOutQuintic(double start, double end, double time, double duration);

/*
double easeInSine(double start, double end, double time, double duration);
double easeOutSine(double start, double end, double time, double duration);
double easeInOutSine(double start, double end, double time, double duration);

double easeInExpo(double start, double end, double time, double duration);
double easeOutExpo(double start, double end, double time, double duration);
double easeInOutExpo(double start, double end, double time, double duration);

double easeInCircular(double start, double end, double time, double duration);
double easeOutCircular(double start, double end, double time, double duration);
double easeInOutCircular(double start, double end, double time, double duration);
*/

#endif
