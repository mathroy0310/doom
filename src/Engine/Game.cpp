#include "Engine/Game.h"

#include <iostream>
#include <string>

namespace Engine {
Engine::Game::Game() : m_iWindowWidth(1280), m_iWindowHeight(800) {
	m_pDoomEngine = new Engine::Doom();
}

Engine::Game::~Game() {
	if (m_pDoomEngine) {
		delete m_pDoomEngine;
	}
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	SDL_Quit();
}

bool Engine::Game::init() {
	// init SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
		return false;
	}

	m_pWindow = SDL_CreateWindow(m_pDoomEngine->getName().c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_iWindowWidth, m_iWindowHeight, SDL_WINDOW_SHOWN);
	if (m_pWindow == nullptr) {
		std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
		return false;
	}

	m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_SOFTWARE);
	if (m_pRenderer == nullptr) {
		std::cerr << "SDL failed to create renderer! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	SDL_SetRenderDrawColor(m_pRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	if (!m_pDoomEngine->init()) {
		std::cerr << m_pDoomEngine->getName() << " failed to initialize!" << std::endl;
		return false;
	}

	if (SDL_RenderSetLogicalSize(m_pRenderer, m_pDoomEngine->getRenderWidth(), m_pDoomEngine->getRenderHeight()) != 0) {
		std::cerr << "SDL failed to set logical size! SDL_Error: " << SDL_GetError() << std::endl;
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
}

void Engine::Game::render() {
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 0xFF);
	SDL_RenderClear(m_pRenderer);

	m_pDoomEngine->render(m_pRenderer);

	SDL_RenderPresent(m_pRenderer);
}

void Engine::Game::update() { m_pDoomEngine->update(); }

bool Engine::Game::isOver() { return m_pDoomEngine->isOver(); }

void Engine::Game::delay() { SDL_Delay(m_pDoomEngine->getTimePerFrame()); }

} // namespace Engine