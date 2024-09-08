#include "Engine/Game.h"

#include <iostream>
#include <string>

namespace Engine {
Engine::Game::Game() {}

Engine::Game::~Game() {}

bool Engine::Game::init() {
	m_pDoomEngine = std::unique_ptr<Engine::Doom>(new Engine::Doom());

	if (!m_pDoomEngine || !m_pDoomEngine->init()) {
		std::cout << m_pDoomEngine->getAppName() << " failed to initialize!" << std::endl;
		return false;
	}
	return true;
}

void Engine::Game::processInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		{
			switch (event.type) {
			case SDL_KEYDOWN:
				m_pDoomEngine->keyPressed(event);
				break;
			case SDL_KEYUP:
				m_pDoomEngine->keyReleased(event);
				break;
			case SDL_QUIT:
				m_pDoomEngine->quit();
				break;
			}
		}
	}
	m_pDoomEngine->updateKeyStatus(SDL_GetKeyboardState(NULL));
}

void Engine::Game::render() { m_pDoomEngine->render(); }

void Engine::Game::update() { m_pDoomEngine->update(); }

bool Engine::Game::isOver() const { return m_pDoomEngine->isOver(); }

void Engine::Game::delay() { SDL_Delay(m_pDoomEngine->getTimePerFrame()); }

} // namespace Engine