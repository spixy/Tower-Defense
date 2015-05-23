#pragma once

#include "ClickText.h"
#include "GameData.h"

class UI
{
private:	
	sf::RenderWindow *window;
	sf::Font *font;
	sf::Text text;
	GameSettings *gameSettings;

	int LoadMaps();
	bool exists (const char *fname);

public:
	UI(void);
	UI(sf::RenderWindow *renderWindow, sf::Font *renderFont, GameSettings *settings);
	~UI(void);
	
	void StartMenu(int *maps, int *levels);
	void BottomMenu();
	void TopInfo(int hp, int pts, int lvl);
	void Sachovnica();
};