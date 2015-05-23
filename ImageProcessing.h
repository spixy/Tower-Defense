#pragma once

#include <SFML/Graphics.hpp>
#include "GameData.h"

#define C_PATH 0
#define C_CASTLE 1
#define C_MAP 2

static class ImageProcessing
{
private:
	static void FillPixels(int data[800][500], int X, int Y, int number);

public:
	ImageProcessing(void);
	~ImageProcessing(void);
	
	static void ImageProcess(int data[800][500], sf::Sprite *BackGround);
	static sf::Vector2i FindStart(int ImageMap[800][500]);
	static sf::Vector2i FindCastle(int ImageMap[800][500]);
};
