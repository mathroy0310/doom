#pragma once
#include <SDL.h>
#include <string>
#include <vector>

#include "DataTypes.h"

class DisplayManager {
  public:
	DisplayManager(int m_iWindowWidth, int iWindowHeight);
	~DisplayManager();

	void initFrame();
	void render();
	void addColorPalette(WADPalette &palette);

	uint8_t      *getScreenBuffer();
	SDL_Renderer *init(const std::string &sWindowTitle);

  protected:
	int m_iScreenWidth;
	int m_iScreenHeight;

	std::vector<WADPalette> m_ColorPallettes;

	SDL_Window   *m_pWindow;
	SDL_Renderer *m_pRenderer;
	SDL_Texture  *m_pTexture;
	SDL_Surface  *m_pScreenBuffer;
	SDL_Surface  *m_pRGBBuffer;
};
