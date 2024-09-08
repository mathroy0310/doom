#pragma once
#include <vector>

#include "DataTypes.h"

class Things {
  public:
	Things();
	~Things();

	void  addThing(Thing &thing);
	Thing getThingByID(int iID);

  private:
	std::vector<Thing> m_Things;
};