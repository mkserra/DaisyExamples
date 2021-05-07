
#include "daisysp.h"
#include "daisy_patch.h"

#include "automata/Automata.cpp"
#include "mandelbrot/Mandelbrot.cpp"

using namespace daisy;
using namespace daisysp;

DaisyPatch patch;

int main()
{
	Automata* automata = new Automata();
	Mandelbrot* mandelbrot = new Mandelbrot();

	patch.Init();
	srand(31);

	while (true)
	{
		if (rand() % 2)
		{
			automata->run(&patch);
		}
		else
		{
			mandelbrot->run(&patch);
		}
		patch.DelayMs(2000);
	}
}

