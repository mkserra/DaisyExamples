
#include "daisysp.h"
#include "daisy_patch.h"

#include "modes/Automata.cpp"
#include "modes/Mandelbrot.cpp"

using namespace daisy;
using namespace daisysp;

DaisyPatch patch;

int main()
{
	Automata* automata;
	Mandelbrot* mandelbrot;

	patch.Init();
	srand(71);

	int mode;

	while (true)
	{
		mode = rand() % 2;

		if (mode == 0)
		{
			automata = new Automata();
			automata->run(&patch);
			delete automata;
		}
		else
		{
			mandelbrot = new Mandelbrot();
			mandelbrot->run(&patch);
			delete mandelbrot;
		}
		patch.DelayMs(2000);
	}
}

