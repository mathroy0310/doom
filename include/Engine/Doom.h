#pragma once

#include <SDL.h>
#include <string>

#include <Map.h>
#include <WAD/Loader.h>

namespace Engine {
class Doom {
  public:
	Doom();
	~Doom();

	virtual void render(SDL_Renderer *pRenderer);
	virtual void keyPressed(SDL_Event &event);
	virtual void keyReleased(SDL_Event &event);
	virtual void quit();
	virtual void update();

	virtual bool isOver();
	virtual bool init();

	virtual int getRenderWidth();
	virtual int getRenderHeight();
	virtual int getTimePerFrame();

	virtual std::string getName();
	virtual std::string getWADFileName();

  protected:
	int m_iRenderWidth;
	int m_iRenderHeight;

	bool m_bIsOver;

	WAD::Loader m_WADLoader;
	Map        *m_pMap;
	Player     *m_pPlayer;
};

} // namespace Engine
