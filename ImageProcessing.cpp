#include "ImageProcessing.h"


ImageProcessing::ImageProcessing(void)
{
}


ImageProcessing::~ImageProcessing(void)
{
}

void ImageProcessing::ImageProcess(int data[800][500], sf::Sprite *BackGround)
{
	sf::Image BackGroundImage = BackGround->getTexture()->copyToImage();
	sf::Vector2u size = BackGround->getTexture()->getSize();

	for (int x = 0; x < size.x; x++)
	{
		for (int y = 0; y < size.y; y++)
		{
			if (BackGroundImage.getPixel(x,y) == sf::Color(240,240,240)) 
			{
				FillPixels(data, x, y, C_PATH);
				//data[x][y] = C_PATH;
			}
			else if (BackGroundImage.getPixel(x,y) == sf::Color::Black)
			{
				FillPixels(data, x, y, C_CASTLE);
				//data[x][y] = C_CASTLE;
			}
			else
			{
				FillPixels(data, x, y, C_MAP);
				//data[x][y] = C_MAP;
			}
		}
	}
}

void ImageProcessing::FillPixels(int data[800][500], int X, int Y, int number)
{
	X = MAPSCALE*X;
	Y = MAPSCALE*Y;

	for (int x = X; x < X+MAPSCALE; x++)
	{
		for (int y = Y; y < Y+MAPSCALE; y++)
		{
			data[x][y] = number;
		}
	}
}

sf::Vector2i ImageProcessing::FindStart(int ImageMap[800][500])
{
	for (int x = 0; x< 800; x++)
		if (ImageMap[x][0] == C_PATH) return sf::Vector2i(x, 0);

	for (int x = 0; x< 800; x++)
		if (ImageMap[x][499] == C_PATH) return sf::Vector2i(x, 499);
	
	for (int y = 0; y< 500; y++)
		if (ImageMap[0][y] == C_PATH) return sf::Vector2i(0, y);

	for (int y = 0; y< 500; y++)
		if (ImageMap[799][y] == C_PATH) return sf::Vector2i(799, y);

	return sf::Vector2i(0,0);
}

sf::Vector2i ImageProcessing::FindCastle(int ImageMap[800][500])
{
	for (int x = 0; x< 800; x++)
	{
		for (int y=0; y < 500; y++)
		{
			if (ImageMap[x][y] == C_CASTLE) return sf::Vector2i(x, y);
		}
	}

	return sf::Vector2i(0,0);
}