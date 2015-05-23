#pragma once

#include <sstream>
#include <SFML/Graphics.hpp>
#include "Unit.h"

class BattleTower
{
private:
	sf::Texture texture;
	sf::Sprite sprite;
	sf::RenderWindow *window;
	sf::Text text;
	GameInfo *gi;

	float Damage;
	float ReloadTime;
	float ActReload;

	bool DrawText;

public:
	BattleTower(int X, int Y, float DMG, float RAN, float REL, int Tvalue, sf::RenderWindow *renderWindow, std::string textureName, GameInfo *GamePoints);
	~BattleTower(void);
	
	sf::Vector2f Position;

	int Value;
	float Range;

	bool Static;
	bool dragging;
	void Update(std::vector<Unit> *EnemiesList, std::vector<std::vector<int>> *DMGlinesList);
	void Draw();
	void Draw(sf::Shader *shader);
	void SetText(sf::Font *textFont);
};
