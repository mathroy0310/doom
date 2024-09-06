#include "WAD/Loader.h"
#include <iostream>

int main() {
	WAD::Loader wadloader("assets/DOOM.WAD");
	wadloader.LoadWAD();
	return 0;
}