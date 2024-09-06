#include "Map.h"
#include "WAD/Loader.h"
#include <iostream>

int main() {
	WAD::Loader wadloader("assets/WAD/Doom1.WAD");
	wadloader.loadWAD();

	Map map("E1M1");
	wadloader.loadMapData(map);

	return 0;
}