#pragma once

#include <SDL.h>

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
    void renderPresent();
    void renderClear();

	bool isOver();
	bool init();

  protected:
	int m_iWindowWidth;
	int m_iWindowHeight;

	SDL_Window   *m_pWindow;
	SDL_Renderer *m_pRenderer;
	Engine::Doom *m_pDoomEngine;
};
} // namespace Engine
