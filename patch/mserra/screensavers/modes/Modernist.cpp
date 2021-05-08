
#include "daisysp.h"
#include "daisy_patch.h"
#include <time.h>

using namespace daisy;
using namespace daisysp;

#define MAX(a, b)		(((a) > (b)) ? (a) : (b))

typedef struct
{
	int x, y;
} point;

typedef struct
{
	point p, q;
} rect;

class Modernist
{
	private: static const int XRES = 128;
	private: static const int YRES = 64;
	private: static const int XBUF = 512;
	private: static const int YBUF = 256;

	private: static const int X_MARGIN = XBUF - XRES / 2;
	private: static const int Y_MARGIN = YBUF - YRES / 2;

	private: static const int INIT_POP_MIN = 5;
	private: static const int ITERATIONS = 30;

	private: uint8_t (*arr0)[YBUF];
	private: uint8_t (*arr1)[YBUF];

	public: Modernist()
	{
		arr0 = new uint8_t[XBUF][YBUF];
		arr1 = new uint8_t[XBUF][YBUF];
	}

	public: ~Modernist()
	{
		delete arr0;
		delete arr1;
	}

	//private: int moore(uint8_t** arr, int x, int y)
	private: int moore(uint8_t arr[][YBUF], int x, int y)
	{
		int n = 0;

		for (int i = -4; i < 5; i++)
		{
			for (int j = -4; j < 5; j++)
			{
				n += arr[x+i][y+j] > 0;
			}
		}
		return n;
	}

	//private: void update(uint8_t** a, uint8_t** b)
	private: void update(uint8_t a[][YBUF], uint8_t b[][YBUF])
	{
		for (int x = 5; x < XBUF - 5; x++)
		{
			for (int y = 5; y < YBUF - 5; y++)
			{
				int m = moore(a, x, y);

				if (a[x][y] > 0)  // cell is alive
				{
					if (!(2 <= m && m <= 3))
					{
						b[x][y] = MAX(0, b[x][y] - 1);
					}
				}
				else if (a[x][y] < 1)
				{
					b[x][y] = m == 3;
				}
			}
		}
	}

	private: bool withinRect(int x, int y, rect r)
	{
		return r.p.x < x && y > r.p.y && r.q.x > x && y < r.q.y;
	}

	private: void populate(uint8_t a[][YBUF], uint8_t b[][YBUF])
	{
		for (int i = 0; i < XBUF; i++)
		{
			for (int j = 0; j < YBUF; j++)
			{
				a[i][j] = 0;
				b[i][j] = 0;
			}
		}
		rect seedUpperLeft;   // northwest quadrant of central seed pattern

		seedUpperLeft.p.x = XBUF / 2 - 4;
		seedUpperLeft.p.y = YBUF / 2 - 4;
		seedUpperLeft.q.x = XBUF / 2;
		seedUpperLeft.q.y = YBUF / 2;

		int n = INIT_POP_MIN + (rand() % (15 - INIT_POP_MIN));

		while (--n > 0)
		{
			int x = rand() % XBUF;
			int y = rand() % YBUF;

			while (!withinRect(x, y, seedUpperLeft))
			{
				x = rand() % XBUF;
				y = rand() % YBUF;
			}
			int rnd = rand() % 256;
			a[x][y] = rnd;
			b[x][y] = rnd;

			// mirror the quadrant to other quadrants
			a[XBUF - x][y] = rnd;
			b[XBUF - x][y] = rnd;

			a[x][YBUF - y] = rnd;
			b[x][YBUF - y] = rnd;

			a[XBUF - x][YBUF - y] = rnd;
			b[XBUF - x][YBUF - y] = rnd;
		}
	}

	private: void drawBuffer(DaisyPatch* patch, uint8_t arr[][YBUF])
	{
		patch->display.Fill(false);

		for (int x = 0; x < XRES; x++)
		{
			for (int y = 0; y < YRES; y++)
			{
				if (arr[x + X_MARGIN][y + Y_MARGIN] > 0)
				{
					patch->display.DrawPixel(x, y, true);
				}
			}
		}
	}

	public: void run(DaisyPatch* patch)
	{
		//auto arr0 = new uint8_t[XBUF][YBUF];
		//auto arr1 = new uint8_t[XBUF][YBUF];

		populate(arr0, arr1);
		bool flag = 0;

		for (int i = 0; i < ITERATIONS; i++)
		{
			flag = !flag;

			if (flag)
			{
				update(arr0, arr1);
			}
			else
			{
				update(arr1, arr0);
			}
			drawBuffer(patch, arr0);
			patch->display.Update();
			patch->DelayMs(400);
		}
		patch->DelayMs(2500);  // pause before restarting
	}
};

