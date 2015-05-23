#pragma once

#include <SFML/Graphics.hpp>
#include <sstream>

#define C_PATH 0
#define C_CASTLE 1
#define C_MAP 2

#define TOWERTYPES 6
#define MAPSCALE 10

struct Castle
{
	sf::Vector2i Position;
	int Health;
};

struct LevelInfo
{
	int UnitIndex;
	int HP;
	int Count;
	float Speed;
};

struct GameInfo
{
	int Points;
	int EnemyCount;
	int Level;
};

struct TowerInfo
{
	int Damage;
	int Range;
	float Reload;
	int Value;
};

struct GameSettings
{
	bool AA;
	bool vsync;
	bool pause;
	float speed;
	bool grid;
	bool menu;
	bool sound;
	bool music;
	float soundVolume;
	float musicVolume;
};

std::string Append(std::string str1, int n, std::string str2);