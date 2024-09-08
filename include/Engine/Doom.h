#pragma once

#include <SDL.h>
#include <memory>
#include <string>

#include "DisplayManager.h"
#include "Map.h"
#include "Player.h"
#include "ViewRenderer.h"
#include "WAD/Loader.h"

namespace Engine {
class Doom {
  public:
	Doom();
	~Doom();

	virtual void render();
	virtual void updateKeyStatus(const Uint8 *KeyStates);
	virtual void keyPressed(SDL_Event &event);
	virtual void keyReleased(SDL_Event &event);
	virtual void quit();
	virtual void update();

	virtual bool isOver();
	virtual bool init();

	void loadWAD();

	virtual int getRenderWidth() const;
	virtual int getRenderHeight() const;
	virtual int getTimePerFrame() const;

	virtual std::string getAppName() const;
	virtual std::string getWADFileName() const;

  protected:
	int m_iRenderWidth;
	int m_iRenderHeight;

	bool m_bIsOver;

	std::string m_sAppName;

	WAD::Loader m_WADLoader;

	std::unique_ptr<Map>            m_pMap;
	std::unique_ptr<Player>         m_pPlayer;
	std::unique_ptr<Things>         m_pThings;
	std::unique_ptr<DisplayManager> m_pDisplayManager;
	std::unique_ptr<ViewRenderer>   m_pViewRenderer;
};

} // namespace Engine
