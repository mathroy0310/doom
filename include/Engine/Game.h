#pragma once

#include <SDL.h>
#include <memory>

#include "DisplayManager.h"
#include <Engine/Doom.h>

namespace Engine {
class Game {
  public:
	Game();
	virtual ~Game();

	void processInput();
	void render();
	void update();
	void delay();

	bool isOver() const;
	bool init();

  protected:
	std::unique_ptr<Engine::Doom> m_pDoomEngine;
};
} // namespace Engine
