
#include "daisysp.h"
#include "daisy_patch.h"
#include <time.h>
#include <math.h>

using namespace daisy;
using namespace daisysp;

class Gnarl
{
	private:

	static const uint8_t XRES = 128;
	static const uint8_t YRES = 64;
	static const int ARR_SIZE = XRES * YRES;

	static const int GENERATIONS = 26;

	uint8_t* plane0;
	uint8_t* plane1;

	public: Gnarl()
	{
		plane0 = (uint8_t*) aligned_alloc(32, ARR_SIZE * sizeof (uint8_t));
		plane1 = (uint8_t*) aligned_alloc(32, ARR_SIZE * sizeof (uint8_t));
	}

	public: ~Gnarl()
	{
		free(plane0);
		free(plane1);
	}

	private:

	uint8_t xy(uint8_t* arr, uint8_t x, uint8_t y)
	{
		return arr[x * YRES + y];
	}

	void set(uint8_t* arr, uint8_t x, uint8_t y, uint8_t v)
	{
		arr[x * YRES + y] = v;
	}

	void initPlane()
	{
		for (int i = 0; i < XRES; i++)
		{
			for (int j = 0; j < YRES; j++)
			{
				set(plane0, i, j, 0);
				set(plane1, i, j, 0);
			}
		}

		for (int i = 0; i < 5; i++)   // random central pattern
		{
			for (int j = 0; j < 5; j++)
			{
				uint8_t b = rand() % 2;

				set(plane0, XRES / 2 - i, YRES / 2 - j, b);
				set(plane0, XRES / 2 + i, YRES / 2 - j, b);
				set(plane0, XRES / 2 - i, YRES / 2 + j, b);
				set(plane0, XRES / 2 + i, YRES / 2 + j, b);
			}
		}
	}

	void paint(DaisyPatch* patch, uint8_t* arr)
	{
		patch->display.Fill(false);

		for (int i = 0; i < XRES; i++)
		{
			for (int j = 0; j < YRES; j++)
			{
				patch->display.DrawPixel(i, j, xy(arr, i, j));
			}
		}
		patch->display.Update();
	}

	uint8_t moore(uint8_t* arr, uint8_t x, uint8_t y)
	{
		int n = 0;

		for (int i = x - 1; i < x + 2; i++)
		{
			for (int j = y - 1; j < y + 2; j++)
			{
				n += xy(arr, i, j) > 0;
			}
		}
		return n - xy(arr, x, y);
	}

	void update(uint8_t* src, uint8_t* dst)
	{
		for (int i = 1; i < XRES - 1; i++)
		{
			for (int j = 1; j < YRES - 1; j++)
			{
				set(dst, i, j, moore(src, i, j) == 1);
			}
		}
	}

	public: void run(DaisyPatch* patch)
	{
		initPlane();

		bool flag = true;

		for (int i = 0; i < GENERATIONS; i++)
		{
			if (flag)
			{
				paint(patch, plane0);
				update(plane0, plane1);
			}
			else  // toggle buffer source and destination
			{
				paint(patch, plane1);
				update(plane1, plane0);
			}
			flag = !flag;
			patch->DelayMs(500);
		}
	}
};

