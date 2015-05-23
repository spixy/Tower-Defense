#pragma once

#include <cstdlib>
#include <SFML/Graphics.hpp>
#include "GameData.h"

#define UNITSIZE 10
#define STEP 5

class Unit
{
private:
	sf::Texture texture;
	sf::Sprite sprite;
	GameInfo *gi;
	Castle *Target;
	sf::RenderWindow *window;
	bool LastUp, LastDown, LastRight, LastLeft;
	int smer;
	
	void MoveUp();
	void MoveDown();
	void MoveRight();
	void MoveLeft();

	/*
	bool FcUp(int x, int y, int ImageMap[800][500]);
	bool FcDown(int x, int y, int ImageMap[800][500]);
	bool FcRight(int x, int y, int ImageMap[800][500]);
	bool FcLeft(int x, int y, int ImageMap[800][500]);
	*/

public:
	sf::Vector2f Position;
	float Health;
	float MaxHealth;
	float Speed;
	bool Living;
	
	Unit(int X, int Y, float HP, float Speed, sf::RenderWindow *renderWindow, std::string textureName, Castle *castle, GameInfo *info);
	~Unit(void);

	void Update(int ImageMap[800][500]);
	void Draw();
	void Draw(sf::Shader *shader);
};
