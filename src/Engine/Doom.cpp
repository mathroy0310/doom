#include "Engine/Doom.h"

namespace Engine {

Engine::Doom::Doom() : m_bIsOver(false), m_iRenderWidth(320), m_iRenderHeight(200) {
	m_pMap = new Map("E1M1");
}

Engine::Doom::~Doom() { delete m_pMap; }

bool Engine::Doom::init() {
	m_WADLoader.setWADFilePath(getWADFileName());
	m_WADLoader.loadWAD();

	m_WADLoader.loadMapData(m_pMap);
	return true;
}

std::string Engine::Doom::getWADFileName() { return "assets/WAD/Doom1.WAD"; }

void Engine::Doom::render(SDL_Renderer *pRenderer) {
	SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 0);
	SDL_RenderClear(pRenderer);
	m_pMap->renderAutoMap(pRenderer);
}

void Engine::Doom::keyPressed(SDL_Event &event) {
	switch (event.key.keysym.sym) {
	case SDLK_UP:
		break;

	case SDLK_DOWN:
		break;

	case SDLK_LEFT:
		break;

	case SDLK_RIGHT:
		break;

	case SDLK_ESCAPE:
		quit();
		break;

	default:
		break;
	}
}

void Engine::Doom::keyReleased(SDL_Event &event) {}

void Engine::Doom::quit() { m_bIsOver = true; }

void Engine::Doom::update() {}

bool Engine::Doom::isOver() { return m_bIsOver; }

int Engine::Doom::getRenderWidth() { return m_iRenderWidth; }

int Engine::Doom::getRenderHeight() { return m_iRenderHeight; }

std::string Engine::Doom::getName() { return "DOOM"; }

int Engine::Doom::getTimePerFrame() { return 1000 / 60; }

} // namespace Engine