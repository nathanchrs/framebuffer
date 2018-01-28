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
