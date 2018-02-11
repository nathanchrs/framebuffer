#include "easing.h"
#include <math.h>

double linearTween(double start, double end, double time, double duration) {
	if (start > end) {
		return start - (end + (start * time / duration));
	} else {
		return start + (end * time / duration);
	}
}

double easeInQuadratic(double start, double end, double time, double duration) {
	time /= duration;
	if (start > end) {
		return start - (end + (start * time * time));
	} else {
		return start + (end * time * time);
	}
}

double easeOutQuadratic(double start, double end, double time, double duration) {
	time /= duration;
	if (start > end) {
		return start - (end + (-start * time * (time - 2)));
	} else {
		return start + (-end * time * (time - 2));
	}
}

double easeInOutQuadratic(double start, double end, double time, double duration) {
	time /= duration / 2;
	if (start > end) {
		if (time < 1) return start - (start/2*time*time + end);
		time--;
		return start - (-start/2 * (time*(time-2) - 1) + end);
	} else {
		if (time < 1) return end/2*time*time + start;
		time--;
		return -end/2 * (time*(time-2) - 1) + start;
	}
}

double easeInCubic(double start, double end, double time, double duration) {
	time /= duration;
	if (start > end) {
		return start - (start*time*time*time + end);
	} else {
		return end*time*time*time + start;
	}
}

double easeOutCubic(double start, double end, double time, double duration) {
	time /= duration;
	time--;
	if (start > end) {
		return start - (start*(time*time*time + 1) + end);
	} else {
		return end*(time*time*time + 1) + start;
	}
}

double easeInOutCubic(double start, double end, double time, double duration) {
	time /= duration/2;
	if (start > end) {
		if (time < 1) return start - (start/2*time*time*time + end);
		time -= 2;
		return start - (start/2*(time*time*time + 2) + end);
	} else {
		if (time < 1) return end/2*time*time*time + start;
		time -= 2;
		return end/2*(time*time*time + 2) + start;
	}
}

double easeInQuartic(double start, double end, double time, double duration) {
	time /= duration;
	if (start > end) {
		return start - (start*time*time*time*time + end);
	} else {
		return end*time*time*time*time + start;
	}
}

double easeOutQuartic(double start, double end, double time, double duration) {
	time /= duration;
	time--;
	if (start > end) {
		return start - (-start*(time*time*time*time - 1) + end);
	} else {
		return -end*(time*time*time*time - 1) + start;
	}
}

double easeInOutQuartic(double start, double end, double time, double duration) {
	time /= duration/2;
	if (start > end) {
		if (time < 1) return start - (start/2*time*time*time*time + end);
		time -= 2;
		return start - (-start/2*(time*time*time*time - 2) + end);
	} else {
		if (time < 1) return end/2*time*time*time*time + start;
		time -= 2;
		return -end/2*(time*time*time*time - 2) + start;
	}
}

double easeInQuintic(double start, double end, double time, double duration) {
	time /= duration;
	if (start > end) {
		return start - (start*time*time*time*time*time + end);
	} else {
		return end*time*time*time*time*time + start;
	}
}

double easeOutQuintic(double start, double end, double time, double duration) {
	time /= duration;
	time--;
	if (start > end) {
		return start - (start*(time*time*time*time*time + 1) + end);
	} else {
		return end*(time*time*time*time*time + 1) + start;
	}
}

double easeInOutQuintic(double start, double end, double time, double duration) {
	time /= duration/2;
	if (start > end) {
		if (time < 1) return start - (start/2*time*time*time*time*time + end);
		time -= 2;
		return start - (start/2*(time*time*time*time*time + 2) + end);
	} else {
		if (time < 1) return end/2*time*time*time*time*time + start;
		time -= 2;
		return end/2*(time*time*time*time*time + 2) + start;
	}
}

double easeInBounce(double start, double end, double time, double duration){
	if(start > end){
		return start - (start - easeOutBounce (duration-time, 0, start, duration) + end);
	}else{
		return end - easeOutBounce (duration-time, 0, end, duration) + start;
	}
}

double easeOutBounce(double start, double end, double time, double duration){
	time/=duration;
	if (start > end){
		if ((time) < (1/2.75f)) {
			return start - (end + start*(7.5625f*time*time));
		} else if (time < (2/2.75f)) {
			float postFix = time-=(1.5f/2.75f);
			return start - (end + start*(7.5625f*(postFix)*time + .75f));
		} else if (time < (2.5/2.75)) {
			float postFix = time-=(2.25f/2.75f);
			return start - (end + start*(7.5625f*(postFix)*time + .9375f));
		} else {
			float postFix = time-=(2.625f/2.75f);
			return start - (end + start*(7.5625f*(postFix)*time + .984375f));
		}
	} else {
		if ((time) < (1/2.75f)) {
			return start + end*(7.5625f*time*time);
		} else if (time < (2/2.75f)) {
			float postFix = time-=(1.5f/2.75f);
			return start + end*(7.5625f*(postFix)*time + .75f);
		} else if (time < (2.5/2.75)) {
			float postFix = time-=(2.25f/2.75f);
			return start + end*(7.5625f*(postFix)*time + .9375f);
		} else {
			float postFix = time-=(2.625f/2.75f);
			return start + end*(7.5625f*(postFix)*time + .984375f);
		}
	}
}

double easeInOutBounce(double start, double end, double time, double duration){
	if(start > end){
		if (time < duration/2) {
			return start - (easeInBounce (time*2, 0, start, duration) * .5f + end);
		}
		else {
			return start - (easeOutBounce (time*2-duration, 0, start, duration) * .5f + start*.5f + end);
		}
	}
	else{
		if (time < duration/2) {
			return easeInBounce (time*2, 0, end, duration) * .5f + start;
		}
		else {
			return easeOutBounce (time*2-duration, 0, end, duration) * .5f + end*.5f + start;
		}
	}
}
