
#include "daisysp.h"
#include "daisy_patch.h"

#include "modes/Automata.cpp"
#include "modes/Mandelbrot.cpp"
#include "modes/Serviettes.cpp"
#include "modes/Gnarl.cpp"
#include "modes/Flakes.cpp"

using namespace daisy;
using namespace daisysp;

DaisyPatch patch;

int main()
{
	Automata* automata;
	Mandelbrot* mandelbrot;
	Serviettes* serviettes;
	Gnarl* gnarl;
	Flakes* flakes;

	patch.Init();
	srand(71);

	int mode;

	while (true)
	{
		mode = rand() % 5;

		if (mode == 0)
		{
			automata = new Automata();
			automata->run(&patch);
			delete automata;
		}
		else if (mode == 1)
		{
			mandelbrot = new Mandelbrot();
			mandelbrot->run(&patch);
			delete mandelbrot;
		}
		else if (mode == 2)
		{
			serviettes = new Serviettes();
			serviettes->run(&patch);
			delete serviettes;
		}
		else if (mode == 3)
		{
			flakes = new Flakes();
			flakes->run(&patch);
			delete flakes;
		}
		else
		{
			gnarl = new Gnarl();
			gnarl->run(&patch);
			delete gnarl;
		}
		patch.DelayMs(2000);
	}
}

