#define FPS 60

#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include "easing.h"
#include "framebuffer.h"

#define TOTAL_DURATION 10000

FrameBuffer fb;

int dataWidth = 31;
int dataHeight = 7;

char data[7][31] = {"         KELOMPOK EVOS         ",
                    "                               ",
                    "JONATHAN CHRISTOPHER - 13515001",
                    "  ROBBY SYAIFULLAH - 13515013  ",
                    "   KEVIN JONATHAN - 13515052   ",
                    "   AFIF BAMBANG P - 13515058   ",
                    "  LAZUARDI FIRDAUS - 13515136  "};


void drawPulse(long elapsedMillis, Point center, double radius, Color color) {
	long duration = 700;
	long time = elapsedMillis % duration;
	double animRadius = easeOutQuadratic(time, 0, radius, duration);

	long colorTime = time > duration * 0.2 ? (time - duration*0.2) * 1.2 : 0;
	Color animColor = c_make(
		easeOutQuadratic(colorTime, color.r, 0, duration),
		easeOutQuadratic(colorTime, color.g, 0, duration),
		easeOutQuadratic(colorTime, color.b, 0, duration)
	);
	fb_drawCircleOutline(&fb, center, animRadius, 1.0, animColor);
}

void drawSquarePixel(int x, int y, int size, int color) {
  switch (color) {
    case 0:
	    fb_drawRectangle(&fb, p_make(x, y), p_make(x+size, y+size), c_make(0xff, 0xff, 0xff));
      break;
    case 1:
	    fb_drawRectangle(&fb, p_make(x, y), p_make(x+size, y+size), c_make(0xff, 0x00, 0x00));
      break;
    case 2:
	    fb_drawRectangle(&fb, p_make(x, y), p_make(x+size, y+size), c_make(0xff, 0x88, 0x00));
      break;
    case 3:
	    fb_drawRectangle(&fb, p_make(x, y), p_make(x+size, y+size), c_make(0xff, 0xff, 0x00));
      break;
    case 4:
	    fb_drawRectangle(&fb, p_make(x, y), p_make(x+size, y+size), c_make(0x00, 0xff, 0x00));
      break;
    case 5:
	    fb_drawRectangle(&fb, p_make(x, y), p_make(x+size, y+size), c_make(0x00, 0x00, 0xff));
      break;
    case 6:
	    fb_drawRectangle(&fb, p_make(x, y), p_make(x+size, y+size), c_make(0xff, 0x00, 0xff));
      break;
    default:
	    fb_drawRectangle(&fb, p_make(x, y), p_make(x+size, y+size), c_make(0xff, 0xff, 0xff));
  }
}

void drawLine(int x0, int y0, int x1, int y1) {
  int dx =  (x1 - x0 > 0 ? x1 - x0 : x0 - x1), sx = x0 < x1 ? 1 : -1;
  int dy = -(y1 - y0 > 0 ? y1 - y0 : y0 - y1), sy = y0 < y1 ? 1 : -1; 
  int err = dx + dy, e2; /* error value e_xy */
 
  for (;;){  /* loop */
    fb_drawPixel(&fb, p_make(x0, y0), c_make(0xff, 0xff, 0xff));
    if (x0 == x1 && y0 == y1) break;
    e2 = 2 * err;
    if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
    if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
  }
}

void drawSquareLine(int x0, int y0, int x1, int y1, int size, int color) {
  int dx =  (x1 - x0 > 0 ? x1 - x0 : x0 - x1), sx = x0 < x1 ? 1 : -1;
  int dy = -(y1 - y0 > 0 ? y1 - y0 : y0 - y1), sy = y0 < y1 ? 1 : -1; 
  int err = dx + dy, e2; /* error value e_xy */
 
  for (;;){  /* loop */
    drawSquarePixel(x0, y0, size, color);
    if (x0 == x1 && y0 == y1) break;
    e2 = 2 * err;
    if (e2 >= dy) { err += dy; x0 += size*sx; } /* e_xy+e_x > 0 */
    if (e2 <= dx) { err += dx; y0 += size*sy; } /* e_xy+e_y < 0 */
  }
}

void draw(char c, int x, int y, int size, int color) {
  switch (c) {
    case 'A':
      drawSquareLine(x, y+size, x, y+4*size, size, color);
      drawSquareLine(x+size, y, x+3*size, y, size, color);
      drawSquareLine(x+4*size, y+size, x+4*size, y+4*size, size, color);
      drawSquareLine(x+size, y+3*size, x+3*size, y+3*size, size, color);
      break;
    case 'B':
      drawSquareLine(x, y, x, y+4*size, size, color);
      drawSquareLine(x+size, y, x+3*size, y, size, color);
      drawSquareLine(x+size, y+2*size, x+3*size, y+2*size, size, color);
      drawSquareLine(x+size, y+4*size, x+3*size, y+4*size, size, color);
      drawSquarePixel(x+4*size, y+size, size, color);
      drawSquarePixel(x+4*size, y+3*size, size, color);
      break;
    case 'C':
      drawSquareLine(x+size, y, x+4*size, y, size, color);
      drawSquareLine(x, y+size, x, y+3*size, size, color);
      drawSquareLine(x+size, y+4*size, x+4*size, y+4*size, size, color);
      break;
    case 'D':
      drawSquareLine(x, y, x, y+4*size, size, color);
      drawSquareLine(x+size, y, x+3*size, y, size, color);
      drawSquareLine(x+4*size, y+size, x+4*size, y+3*size, size, color);
      drawSquareLine(x+size, y+4*size, x+3*size, y+4*size, size, color);
      break;
    case 'E':
      drawSquareLine(x, y, x, y+4*size, size, color);
      drawSquareLine(x+size, y, x+4*size, y, size, color);
      drawSquareLine(x+size, y+2*size, x+2*size, y+2*size, size, color);
      drawSquareLine(x+size, y+4*size, x+4*size, y+4*size, size, color);
      break;
    case 'F':
      drawSquareLine(x, y, x, y+4*size, size, color);
      drawSquareLine(x+size, y, x+4*size, y, size, color);
      drawSquareLine(x+size, y+2*size, x+2*size, y+2*size, size, color);
      break;
    case 'G':
      drawSquareLine(x+size, y, x+4*size, y, size, color);
      drawSquareLine(x, y+size, x, y+3*size, size, color);
      drawSquareLine(x+size, y+4*size, x+4*size, y+4*size, size, color);
      drawSquareLine(x+2*size, y+2*size, x+4*size, y+2*size, size, color);
      drawSquarePixel(x+4*size, y+3*size, size, color);
      break;
    case 'H':
      drawSquareLine(x, y, x, y+4*size, size, color);
      drawSquareLine(x+size, y+2*size, x+3*size, y+2*size, size, color);
      drawSquareLine(x+4*size, y, x+4*size, y+4*size, size, color);
      break;
    case 'I':
      drawSquareLine(x, y, x+4*size, y, size, color);
      drawSquareLine(x, y+4*size, x+4*size, y+4*size, size, color);
      drawSquareLine(x+2*size, y+size, x+2*size, y+3*size, size, color);
      break;
    case 'J':
      drawSquareLine(x+size, y, x+4*size, y, size, color);
      drawSquareLine(x+4*size, y+size, x+4*size, y+3*size, size, color);
      drawSquareLine(x+size, y+4*size, x+3*size, y+4*size, size, color);
      drawSquarePixel(x, y+3*size, size, color);
      break;
    case 'K':
      drawSquareLine(x, y, x, y+4*size, size, color);
      drawSquareLine(x+size, y+2*size, x+2*size, y+2*size, size, color);
      drawSquareLine(x+2*size, y+2*size, x+4*size, y, size, color);
      drawSquareLine(x+2*size, y+2*size, x+4*size, y+4*size, size, color);
      break;
    case 'L':
      drawSquareLine(x, y, x, y+4*size, size, color);
      drawSquareLine(x+size, y+4*size, x+4*size, y+4*size, size, color);
      break;
    case 'M':
      drawSquareLine(x, y, x, y+4*size, size, color);
      drawSquareLine(x+size, y+size, x+2*size, y+2*size, size, color);
      drawSquarePixel(x+3*size, y+size, size, color);
      drawSquareLine(x+4*size, y, x+4*size, y+4*size, size, color);
      break;
    case 'N':
      drawSquareLine(x, y, x, y+4*size, size, color);
      drawSquareLine(x+size, y+size, x+3*size, y+3*size, size, color);
      drawSquareLine(x+4*size, y, x+4*size, y+4*size, size, color);
      break;
    case 'O':
      drawSquareLine(x+size, y, x+3*size, y, size, color);
      drawSquareLine(x, y+size, x, y+3*size, size, color);
      drawSquareLine(x+size, y+4*size, x+3*size, y+4*size, size, color);
      drawSquareLine(x+4*size, y+size, x+4*size, y+3*size, size, color);
      break;
    case 'P':
      drawSquareLine(x, y, x, y+4*size, size, color);
      drawSquareLine(x+size, y, x+3*size, y, size, color);
      drawSquareLine(x+size, y+3*size, x+3*size, y+3*size, size, color);
      drawSquareLine(x+4*size, y+size, x+4*size, y+2*size, size, color);
      break;
    case 'Q':
      break;
    case 'R':
      drawSquareLine(x, y, x, y+4*size, size, color);
      drawSquareLine(x+size, y, x+3*size, y, size, color);
      drawSquareLine(x+size, y+3*size, x+3*size, y+3*size, size, color);
      drawSquareLine(x+4*size, y+size, x+4*size, y+2*size, size, color);
      drawSquarePixel(x+4*size, y+4*size, size, color);
      break;
    case 'S':
      drawSquareLine(x+size, y, x+4*size, y, size, color);
      drawSquareLine(x+size, y+2*size, x+3*size, y+2*size, size, color);
      drawSquareLine(x, y+4*size, x+3*size, y+4*size, size, color);
      drawSquarePixel(x, y+size, size, color);
      drawSquarePixel(x+4*size, y+3*size, size, color);
      break;
    case 'T':
      drawSquareLine(x, y, x+4*size, y, size, color);
      drawSquareLine(x+2*size, y+size, x+2*size, y+4*size, size, color);
      break;
    case 'U':
      drawSquareLine(x, y, x, y+3*size, size, color);
      drawSquareLine(x+size, y+4*size, x+3*size, y+4*size, size, color);
      drawSquareLine(x+4*size, y, x+4*size, y+3*size, size, color);
      break;
    case 'V':
      drawSquareLine(x, y, x+2*size, y+4*size, size, color);
      drawSquareLine(x+4*size, y, x+2*size, y+4*size, size, color);
      break;
    case 'W':
      break;
    case 'X':
      break;
    case 'Y':
      drawSquareLine(x, y, x+2*size, y+2*size, size, color);
      drawSquareLine(x+4*size, y, x+2*size, y+2*size, size, color);
      drawSquareLine(x+2*size, y+2*size, x+2*size, y+4*size, size, color);
      break;
    case 'Z':
      drawSquareLine(x, y, x+4*size, y, size, color);
      drawSquareLine(x+3*size, y+size, x+size, y+3*size, size, color);
      drawSquareLine(x, y+4*size, x+4*size, y+4*size, size, color);
      break;
    case '1':
      drawSquareLine(x+2*size, y, x+2*size, y+3*size, size, color);
      drawSquareLine(x, y+4*size, x+4*size, y+4*size, size, color);
      drawSquarePixel(x+size, y+size, size, color);
      break;
    case '2':
      drawSquareLine(x, y, x+3*size, y, size, color);
      drawSquareLine(x+size, y+2*size, x+3*size, y+2*size, size, color);
      drawSquareLine(x, y+4*size, x+4*size, y+4*size, size, color);
      drawSquarePixel(x+4*size, y+size, size, color);
      drawSquarePixel(x, y+3*size, size, color);
      break;
    case '3':
      drawSquareLine(x+size, y, x+3*size, y, size, color);
      drawSquareLine(x+2*size, y+2*size, x+3*size, y+2*size, size, color);
      drawSquareLine(x+size, y+4*size, x+3*size, y+4*size, size, color);
      drawSquarePixel(x, y+size, size, color);
      drawSquarePixel(x, y+3*size, size, color);
      drawSquarePixel(x+4*size, y+size, size, color);
      drawSquarePixel(x+4*size, y+3*size, size, color);
      break;
    case '4':
      break;
    case '5':
      drawSquareLine(x+size, y, x+4*size, y, size, color);
      drawSquareLine(x, y, x, y+2*size, size, color);
      drawSquareLine(x+size, y+2*size, x+3*size, y+2*size, size, color);
      drawSquareLine(x, y+4*size, x+3*size, y+4*size, size, color);
      drawSquarePixel(x+4*size, y+3*size, size, color);
      break;
    case '6':
      drawSquareLine(x+size, y, x+4*size, y, size, color);
      drawSquareLine(x, y+size, x, y+3*size, size, color);
      drawSquareLine(x+size, y+2*size, x+3*size, y+2*size, size, color);
      drawSquareLine(x+size, y+4*size, x+3*size, y+4*size, size, color);
      break;
    case '7':
      break;
    case '8':
      drawSquareLine(x+size, y, x+3*size, y, size, color);
      drawSquareLine(x+size, y+2*size, x+3*size, y+2*size, size, color);
      drawSquareLine(x+size, y+4*size, x+3*size, y+4*size, size, color);
      drawSquarePixel(x, y+size, size, color);
      drawSquarePixel(x, y+3*size, size, color);
      drawSquarePixel(x+4*size, y+size, size, color);
      drawSquarePixel(x+4*size, y+3*size, size, color);
      break;
    case '9':
      break;
    case '0':
      drawSquareLine(x+size, y, x+3*size, y, size, color);
      drawSquareLine(x, y+size, x, y+3*size, size, color);
      drawSquareLine(x+size, y+4*size, x+3*size, y+4*size, size, color);
      drawSquareLine(x+4*size, y+size, x+4*size, y+3*size, size, color);
      drawSquareLine(x+3*size, y+size, x+size, y+3*size, size, color);
      break;
    case '-':
      drawSquareLine(x, y+2*size, x+4*size, y+2*size, size, color);
      break;
  }
}

void drawData(int x, int y) {
  for (int i = 0; i < dataHeight; i++) {
    for (int j = 0; j < dataWidth; j++) {
      draw(data[i][j], x + 30*j, y + 30*i, 5, i == 0 ? 1 : i);
    }
  }
}

void render(long elapsedMillis) {
	// Clear screen from previous frame
	fb_clear(&fb, COLOR_BLACK);

	// Draw
	
	long width = fb_getWidth(&fb);
	long height = fb_getHeight(&fb);

	// Pulse effect
	int i, j;
	for (i = 0; i < width; i += 60) {
		for (j = 0; j < height; j += 60) {
			drawPulse(elapsedMillis - sqrt((i-width/2)*(i-width/2) + (j-height/2)*(j-height/2)) - 40, p_make(i, j), 20, c_make(0x01, 0x22, 0x55));
			drawPulse(elapsedMillis - sqrt((i-width/2)*(i-width/2) + (j-height/2)*(j-height/2)) + 40, p_make(i, j), 20, c_make(0x01, 0x22, 0x55));
			drawPulse(elapsedMillis - sqrt((i-width/2)*(i-width/2) + (j-height/2)*(j-height/2)), p_make(i, j), 20, c_make(0x05, 0xcb, 0xff));
		}
	}

	// Progress rectangle
	fb_drawRectangle(&fb, p_make(0, 0), p_make(elapsedMillis * width / TOTAL_DURATION, 10), c_make(0xff, 0xff, 0xff));
	
	int x = (width - (dataWidth*30 - 5))/2;
	int y = height - (elapsedMillis * (height + (dataHeight*40 - 15)) / TOTAL_DURATION);
	drawData(x, y);
 
	// Output drawn pixels to screen
	fb_output(&fb);
}

int main() {

	// Initialize
	fb_init(&fb, "/dev/fb0");

	// Main loop
	long nanoSecondsPerFrame = 1000000 / FPS;
	long elapsedMillis = 0;
	unsigned char isRunning = 1;
	while (isRunning) {
		render(elapsedMillis);
		usleep(nanoSecondsPerFrame);
		elapsedMillis += nanoSecondsPerFrame / 1000;
		if (elapsedMillis > TOTAL_DURATION) isRunning = 0;
	}

	// Clean up
	fb_clear(&fb, COLOR_BLACK);
	fb_output(&fb);
	return 0;
}
