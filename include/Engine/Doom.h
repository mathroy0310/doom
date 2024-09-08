#pragma once

#include <SDL.h>
#include <string>

#include "Map.h"
#include "Player.h"
#include "ViewRenderer.h"
#include "WAD/Loader.h"

namespace Engine {
class Doom {
  public:
	Doom(SDL_Renderer *pRenderer);
	~Doom();

	virtual void render();
	virtual void updateKeyStatus(const Uint8 *KeyStates);
	virtual void keyPressed(SDL_Event &event);
	virtual void keyReleased(SDL_Event &event);
	virtual void quit();
	virtual void update();

	virtual bool isOver();
	virtual bool init();

	virtual int getRenderWidth() const;
	virtual int getRenderHeight() const;
	virtual int getTimePerFrame() const;

	virtual std::string getName() const;
	virtual std::string getWADFileName() const;

  protected:
	int m_iRenderWidth;
	int m_iRenderHeight;

	bool m_bIsOver;
	bool m_bRenderAutoMap;

	SDL_Renderer *m_pRenderer;
	WAD::Loader   m_WADLoader;
	Map          *m_pMap;
	Player       *m_pPlayer;
	Things       *m_pThings;
	ViewRenderer *m_pViewRenderer;
};

} // namespace Engine
