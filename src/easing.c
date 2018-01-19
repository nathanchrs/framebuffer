#include "easing.h"
#include <math.h>

double linearTween(double time, double start, double end, double duration) {
	if (start > end) {
		return start - (end + (start * time / duration));
	} else {
		return start + (end * time / duration);
	}
}

double easeInQuadratic(double time, double start, double end, double duration) {
	time /= duration;
	if (start > end) {
		return start - (end + (start * time * time));
	} else {
		return start + (end * time * time);
	}
}

double easeOutQuadratic(double time, double start, double end, double duration) {
	time /= duration;
	if (start > end) {
		return start - (end + (-start * time * (time - 2)));
	} else {
		return start + (-end * time * (time - 2));
	}
}


double easeOutQuartic(double time, double start, double end, double duration) {
	time /= duration;
	time--;
	if (start > end) {
		return start - (end + (-start * (time * time * time * time - 1)));
	} else {
		return start + (-end * (time * time * time * time - 1));
	}
}
