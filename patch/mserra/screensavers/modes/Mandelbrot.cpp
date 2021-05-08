
#include "daisysp.h"
#include "daisy_patch.h"
#include <time.h>
#include <math.h>

using namespace daisy;
using namespace daisysp;

class Mandelbrot
{
	private:

	static const int XRES = 128;
	static const int YRES = 64;
	static const int DIM  = XRES * YRES;

	static const int MAX_INCR = 20;

	struct cmplx
	{
		double r;
		double i;
	};

	struct rect
	{
		int x;
		int y;
		int w;
		int h;
	};

	struct cmplx* plane;
	int* pixels;

	double threshold = 17.0;
	const int FRAMES = 200;

	public: Mandelbrot()
	{
		plane  = (struct cmplx*) aligned_alloc(32, DIM * sizeof (struct cmplx));
		pixels = (int*) aligned_alloc(32, DIM * sizeof (int));
	}

	public: ~Mandelbrot()
	{
		free(plane);
		free(pixels);
	}

	private:

	double cmag(struct cmplx z)
	{
		return sqrt(z.r * z.r + z.i * z.i);
	}

	struct cmplx cadd(struct cmplx a, struct cmplx b)
	{
		struct cmplx z;

		z.r = a.r + b.r;
		z.i = a.i + b.i;
		return z;
	}

	struct cmplx csub(struct cmplx a, struct cmplx b)
	{
		struct cmplx z;

		z.r = a.r - b.r;
		z.i = a.i - b.i;
		return z;
	}

	struct cmplx cmult(struct cmplx a, struct cmplx b)
	{
		struct cmplx z;

		z.r = a.r * b.r - a.i * b.i;
		z.i = a.i * b.r + a.r * b.i;

		return z;
	}

	struct cmplx xy(int x, int y)
	{
		return plane[x * YRES + y];
	}

	int pxl(int x, int y)
	{
		return pixels[x * YRES + y];
	}

	void setPxl(int x, int y, int c)
	{
		pixels[x * YRES + y] = c;
	}

	int getPxl(int x, int y)
	{
		return pixels[x * YRES + y];
	}

	void initPlane(double minX, double maxX, double minY, double maxY)
	{
		double x     = minX;
		double y     = minY;
		double incrX = (maxX - x) / XRES;
		double incrY = (maxY - y) / YRES;

		for (int i = 0; i < XRES; i++)
		{
			y = minY;

			for (int j = 0; j < YRES; j++)
			{
				struct cmplx z;

				z.r = x;
				z.i = y;

				plane[i * YRES + j] = z;

				y += incrY;
			}
			x += incrX;
		}
	}

	void paint(DaisyPatch* patch)
	{
		patch->display.Fill(false);

		for (int i = 0; i < XRES; i++)
		{
			for (int j = 0; j < YRES; j++)
			{
				if (pxl(i, j) > floor(threshold))
				{
					patch->display.DrawPixel(i, j, true);
				}
			}
		}
		patch->display.Update();
	}

	void draw(DaisyPatch* patch, int zoom)
	{
		int k;
		struct cmplx z;
		struct cmplx c;

		for (int i = 0; i < XRES; i++)
		{
			for (int j = 0; j < YRES; j++)
			{
				z.r = 0;
				z.i = 0;
				c   = xy(i,j);
				z   = cadd(z, c);

				for (k = 0; k < zoom * MAX_INCR; k++)
				{
					z = cmult(z, z);
					z = cadd(z, c);

					if (cmag(z) >= 2)
					{
						setPxl(i, j, k);
						break;
					}
				}
				setPxl(i, j, k);
			}
		}
		paint(patch);
	}

	void animate(DaisyPatch* patch, struct rect r, int* zoom)
	{
		double minX = xy(r.x, r.y).r;
		double minY = xy(r.x, r.y).i;
		double maxX = xy(r.x + r.w, r.y + r.h).r;
		double maxY = xy(r.x + r.w, r.y + r.h).i;

		double minXb = xy(0,0).r;
		double minYb = xy(0,0).i;
		double maxXb = xy(XRES - 1, YRES - 1).r;
		double maxYb = xy(XRES - 1, YRES - 1).i;

		double dxMin = (minXb - minX)  / FRAMES;
		double dyMin = (minYb - minY)  / FRAMES;
		double dxMax = (maxX  - maxXb) / FRAMES;
		double dyMax = (maxY  - maxYb) / FRAMES;

		double z  = (double) *zoom;
		double dz = 2 * ((double) *zoom) / FRAMES;

		for (int i = 0; i < FRAMES; i++)
		{
			initPlane(minXb, maxXb, minYb, maxYb);
			draw(patch, *zoom);

			minXb -= dxMin;
			minYb -= dyMin;
			maxXb += dxMax;
			maxYb += dyMax;

			z += dz;
			*zoom = round(z);
			threshold += 5.0 / FRAMES;
		}
		patch->DelayMs(2000);
	}

	public: void run(DaisyPatch* patch)
	{
		int zoom;
		struct rect r;
	
		threshold = 17.0;
		zoom = 1;
		r.w  = 12;
		r.h  = 6;
		initPlane(-2, 0.5, -1, 1);
		draw(patch, zoom);

		if (rand() % 2)  // explore X axis
		{
			r.x = 26;
			r.y = 28;
			animate(patch, r, &zoom);
			r.x = 2;
			r.y = 29;
			animate(patch, r, &zoom);
			r.x = 53;
			r.y = 30;
			animate(patch, r, &zoom);
			r.x = 49;
			r.y = 36;
			animate(patch, r, &zoom);
			r.x = 55;
			r.y = 15;
			animate(patch, r, &zoom);
			r.x = 58;
			r.y = 42;
			animate(patch, r, &zoom);
			r.x = 25;
			r.y = 30;
			animate(patch, r, &zoom);
		}
		else  // seahorse valley
		{
			r.x = 57;
			r.y = 23;
			animate(patch, r, &zoom);
			r.x = 9;
			r.y = 4;
			animate(patch, r, &zoom);
			r.x = 74;
			r.y = 25;
			animate(patch, r, &zoom);
			r.x = 94;
			r.y = 45;
			animate(patch, r, &zoom);
			r.x = 78;
			r.y = 24;
			animate(patch, r, &zoom);
			r.x = 62;
			r.y = 7;
			animate(patch, r, &zoom);
			r.x = 7;
			r.y = 28;
			animate(patch, r, &zoom);
		}
	}
};

