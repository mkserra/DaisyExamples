
#include "daisysp.h"
#include "daisy_patch.h"

#include "modes/Automata.cpp"
#include "modes/Mandelbrot.cpp"
#include "modes/Serviettes.cpp"
#include "modes/Gnarl.cpp"

using namespace daisy;
using namespace daisysp;

DaisyPatch patch;

int main()
{
	Automata* automata;
	Mandelbrot* mandelbrot;
	Serviettes* serviettes;
	Gnarl* gnarl;

	patch.Init();
	srand(71);

	int mode;

	while (true)
	{
		mode = rand() % 4;

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
		else
		{
			gnarl = new Gnarl();
			gnarl->run(&patch);
			delete gnarl;
		}
		patch.DelayMs(2000);
	}
}

