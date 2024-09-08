#include "Engine/Doom.h"

namespace Engine {

Engine::Doom::Doom(SDL_Renderer *pRenderer)
    : m_pRenderer(pRenderer), m_bIsOver(false), m_iRenderWidth(320), m_iRenderHeight(200), m_pMap(nullptr), m_bRenderAutoMap(false) {}

Engine::Doom::~Doom() {
	delete m_pMap;
	delete m_pPlayer;
	delete m_pViewRenderer;
	delete m_pThings;
}

bool Engine::Doom::init() {
	m_pViewRenderer = new ViewRenderer(m_pRenderer);

	m_pPlayer = new Player(m_pViewRenderer, 1);
	m_pThings = new Things();

	m_pMap = new Map(m_pViewRenderer, "E1M1", m_pPlayer, m_pThings);

	m_pViewRenderer->init(m_pMap, m_pPlayer);

	m_WADLoader.setWADFilePath(getWADFileName());
	m_WADLoader.loadWAD();
	m_WADLoader.loadMapData(m_pMap);

	Thing thing = (m_pMap->getThings())->getThingByID(m_pPlayer->getID());

	m_pPlayer->init(thing);
	m_pMap->init();

	return true;
}

std::string Engine::Doom::getWADFileName() const { return "assets/WAD/DOOM.WAD"; }

void Engine::Doom::render() {
	m_pViewRenderer->initFrame();
	m_pViewRenderer->render(m_bRenderAutoMap);
}

void Engine::Doom::keyPressed(SDL_Event &event) {
	switch (event.key.keysym.sym) {
	case SDLK_UP:
		m_pPlayer->moveForward();
		break;
	case SDLK_DOWN:
		m_pPlayer->moveBackward();
		break;
	case SDLK_LEFT:
		m_pPlayer->rotateLeft();
		break;
	case SDLK_RIGHT:
		m_pPlayer->rotateRight();
		break;
	case SDLK_TAB:
		m_bRenderAutoMap = true;
		break;
	case SDLK_ESCAPE:
		quit();
		break;

	default:
		break;
	}
}

void Engine::Doom::keyReleased(SDL_Event &event) {
	switch (event.key.keysym.sym) {
	case SDLK_TAB:
		m_bRenderAutoMap = false;
		break;

	default:
		break;
	}
}

void Engine::Doom::quit() { m_bIsOver = true; }

void Engine::Doom::update() {}

bool Engine::Doom::isOver() { return m_bIsOver; }

int Engine::Doom::getRenderWidth() const { return m_iRenderWidth; }

int Engine::Doom::getRenderHeight() const { return m_iRenderHeight; }

std::string Engine::Doom::getName() const { return "DOOM"; }

int Engine::Doom::getTimePerFrame() const { return 1000 / 60; }

} // namespace Engine