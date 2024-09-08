#include "Engine/Doom.h"

namespace Engine {

Engine::Doom::Doom()
    : m_sAppName("Doom"), m_bIsOver(false), m_iRenderWidth(320 * 2), m_iRenderHeight(200 * 2), m_pMap(nullptr), m_pPlayer(nullptr) {}
Engine::Doom::~Doom() {}

bool Engine::Doom::init() {
	m_pDisplayManager = std::unique_ptr<DisplayManager>(new DisplayManager(m_iRenderWidth, m_iRenderHeight));
	m_pDisplayManager->init(getAppName());

	m_pViewRenderer = std::unique_ptr<ViewRenderer>(new ViewRenderer());
	m_pThings = std::unique_ptr<Things>(new Things());
	m_pPlayer = std::unique_ptr<Player>(new Player(m_pViewRenderer.get(), 1));
	m_pMap = std::unique_ptr<Map>(new Map(m_pViewRenderer.get(), "E1M1", m_pPlayer.get(), m_pThings.get()));

	m_pViewRenderer->init(m_pMap.get(), m_pPlayer.get());

	loadWAD();

    m_pPlayer->init((m_pMap->getThings())->getThingByID(m_pPlayer->getID()));
    m_pMap->init();

	return true;
}

void Engine::Doom::loadWAD() {
	m_WADLoader.setFilePath(getWADFileName());
	m_WADLoader.loadToMemory();
	m_WADLoader.loadPalette(m_pDisplayManager.get());
	m_WADLoader.loadMapData(m_pMap.get());
}

std::string Engine::Doom::getWADFileName() const { return "assets/WAD/DOOM.WAD"; }

void Engine::Doom::render() {
	uint8_t *pScreenBuffer = m_pDisplayManager->getScreenBuffer();

	m_pDisplayManager->initFrame();
	m_pViewRenderer->render(pScreenBuffer, m_iRenderWidth);

	m_pDisplayManager->render();
}

void Engine::Doom::updateKeyStatus(const Uint8 *KeyStates) {
	if (KeyStates[SDL_SCANCODE_UP]) {
		m_pPlayer->moveForward();
	}

	if (KeyStates[SDL_SCANCODE_DOWN]) {
		m_pPlayer->moveBackward();
	}

	if (KeyStates[SDL_SCANCODE_LEFT]) {
		m_pPlayer->rotateLeft();
	}

	if (KeyStates[SDL_SCANCODE_RIGHT]) {
		m_pPlayer->rotateRight();
	}

	if (KeyStates[SDL_SCANCODE_Z]) {
		m_pPlayer->fly();
	}

	if (KeyStates[SDL_SCANCODE_X]) {
		m_pPlayer->sink();
	}
}

void Engine::Doom::keyPressed(SDL_Event &event) {
	switch (event.key.keysym.sym) {
	case SDLK_TAB:
		// m_bRenderAutoMap = true;
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
		// m_bRenderAutoMap = false;
		break;

	default:
		break;
	}
}

void Engine::Doom::quit() { m_bIsOver = true; }

void Engine::Doom::update() { m_pPlayer->think(m_pMap->getPlayerSubSectorHeight()); }

bool Engine::Doom::isOver() { return m_bIsOver; }

int Engine::Doom::getRenderWidth() const { return m_iRenderWidth; }

int Engine::Doom::getRenderHeight() const { return m_iRenderHeight; }

std::string Engine::Doom::getAppName() const { return "DOOM"; }

int Engine::Doom::getTimePerFrame() const { return 1000 / 60; }

} // namespace Engine