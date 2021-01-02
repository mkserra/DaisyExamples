
#include "daisysp.h"
#include "daisy_patch.h"

using namespace daisy;
using namespace daisysp;

DaisyPatch patch;

const int XRES   = 128;
const int YRES   = 64;
const int MARGIN = 96;  // hide CA edges
const int XBUF   = XRES + 2 * MARGIN;
const int YBUF   = 280;

bool ca[XBUF][YBUF];  // elementary cellular automaton

bool neighborhood(int x, int y, int rule)
{
	uint8_t a = ca[x - 1][y - 1];
	uint8_t b = ca[x - 0][y - 1];
	uint8_t c = ca[x + 1][y - 1];

	uint8_t n = (a << 2) | (b << 1) | c;

	switch (rule)
	{
		case 0:  return 0 < n && n < 7;  // 126
		case 1:  return n == 0 || n == 3 || n == 6;  // 73
		case 2:  return n == 0 || n == 3 || n == 5 || n == 6;  // 105
		case 3:  return n == 1 || n == 3 || n == 5 || n == 6;  // 106
		case 4:  return n == 1 || n == 2 || n == 4 || n == 7;  // 150
		default: return false;
	}
}

void populate(int rule)
{
	for (int y = 1; y < YBUF; y++)
	{
		for (int x = 1; x < XBUF - 1; x++)
		{
			ca[x][y] = neighborhood(x, y, rule);
		}
	}
}

void initBuffer()
{
	for (int x = 0; x < XBUF; x++)
	{
		for (int y = 0; y < YBUF; y++)
		{
			ca[x][y] = y == 0 && (rand() % 2);
		}
	}
}

void drawBufferSlice(int top)
{
	patch.display.Fill(false);

	for (int y = 0; y < YRES; y++)
	{
		for (int x = MARGIN; x < XBUF - MARGIN; x++)
		{
			patch.display.DrawPixel(x - MARGIN, y, !ca[x][y + top]);
		}
	}
}

void updateOled()
{
	for (int y = 0; y < YBUF - YRES; y++)
	{
		drawBufferSlice(y);
		patch.display.Update();
		patch.DelayMs(60);
	}
	patch.DelayMs(2500);  // pause before restarting
}

int main()
{
	patch.Init();

	while (true)
	{
		initBuffer();
		populate(rand() % 5);
		updateOled();
	}
}
