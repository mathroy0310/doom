#include "Things.h"

Things::Things() {}

Things::~Things() {}

void Things::addThing(Thing &thing) { m_Things.push_back(thing); }

Thing Things::getThingByID(int iID) {
	Thing thing;
	for (std::vector<Thing>::iterator itr = m_Things.begin(); itr != m_Things.end(); ++itr) {
		if (itr->Type == iID) {
			thing = (*itr);
			break;
		}
	}
	return thing;
}