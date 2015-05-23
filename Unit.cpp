#include "Unit.h"
#include <iostream>

Unit::Unit(int X, int Y, float HP, float SPD, sf::RenderWindow *renderWindow, std::string textureName, Castle *castle, GameInfo *info)
{
	if (!texture.loadFromFile(textureName))
	{
		throw new std::exception("Enemy texture is missing");
	}
	sprite.setTexture(texture);
	sprite.setPosition(X,Y);
	sprite.setOrigin(10,10);
	sprite.setScale(0.625f,0.625f);
	
	smer = 0;
	window = renderWindow;
	gi = info;
	Target = castle;
	Position = sf::Vector2f(X,Y);
	MaxHealth = HP;
	Health = MaxHealth;
	Speed = SPD;
	Living = true;
	LastRight = LastLeft = LastUp = LastDown = false;
}

void Unit::MoveUp()
{
	LastDown = true;
	LastUp = LastRight = LastLeft = false;
	Position = sf::Vector2f(Unit::Position.x, Unit::Position.y - Unit::Speed);
	sprite.setPosition(Position);
}
void Unit::MoveDown()
{
	LastUp = true;
	LastDown = LastRight = LastLeft = false;
	Position = sf::Vector2f(Unit::Position.x, Unit::Position.y + Unit::Speed);
	sprite.setPosition(Position);
}
void Unit::MoveRight()
{
	LastLeft = true;
	LastUp = LastDown = LastRight = false;
	Position = sf::Vector2f(Unit::Position.x + Unit::Speed, Unit::Position.y);
	sprite.setPosition(Position);
}
void Unit::MoveLeft()
{
	LastRight = true;
	LastUp = LastDown = LastLeft = false;
	Position = sf::Vector2f(Unit::Position.x - Unit::Speed, Unit::Position.y);
	sprite.setPosition(Position);
}

void Unit::Update(int ImageMap[800][500])
{
	if (Health <= 0 || !Living)
	{
		Living = false;
		return;
	}

	int x = Position.x;
	int y = Position.y;
	int Tx = Target->Position.x+10;
	int Ty = Target->Position.y+10;

	if ( ImageMap[(int)(x+UNITSIZE+Speed)][y] == C_CASTLE || ImageMap[(int)(x-UNITSIZE-Speed)][y] == C_CASTLE ||
		 ImageMap[x][(int)(y+UNITSIZE+Speed)] == C_CASTLE || ImageMap[x][(int)(y-UNITSIZE-Speed)] == C_CASTLE )
	{
		Target->Health--;
		gi->EnemyCount --;
		Living = false;
		return;
	}

	bool ChangeDirection = false;

	switch (smer)
	{
	case 0:
			ChangeDirection = true;
			break;
	case 1: 
			if (ImageMap[x][(int)(y-UNITSIZE-Speed-STEP)] != C_PATH)
			{
				ChangeDirection = true;
			}
			break;
	case 2: 
			if (ImageMap[x][(int)(y+UNITSIZE+Speed+STEP)] != C_PATH)
			{
				ChangeDirection = true;
			}
			break;
	case 3: 
			if (ImageMap[(int)(x-UNITSIZE-Speed-STEP)][y] != C_PATH)
			{
				ChangeDirection = true;
			}
			break;
	case 4: 
			if (ImageMap[(int)(x+UNITSIZE+Speed+STEP)][y] != C_PATH)			
			{
				ChangeDirection = true;
			}
			break;
	}

	if (ChangeDirection)
	{

	bool exitStatus = false;
	std::vector<int> myvector;
    for (int i=0; i<4; ++i) myvector.push_back(i);
    std::random_shuffle(myvector.begin(), myvector.end());

	for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    {
		if (exitStatus) break;
		switch (*it)
		{
		case 0:
			if (ImageMap[x][(int)(y-UNITSIZE-Speed-STEP)] == C_PATH && !LastUp && y-UNITSIZE-Speed-STEP>0) // Up
			{
				sprite.setRotation(0);
				smer = 1;
				exitStatus = true;
			}
			break;
		case 1:
			if (ImageMap[x][(int)(y+UNITSIZE+Speed+STEP)] == C_PATH && !LastDown && y+UNITSIZE+Speed+STEP<window->getSize().y) // Down
			{
				sprite.setRotation(180);
				smer = 2;
				exitStatus = true;
			}
			break;
		case 2:
			if (ImageMap[(int)(x-UNITSIZE-Speed-STEP)][y] == C_PATH && !LastLeft && x-UNITSIZE-Speed-STEP>0) // Left
			{
				sprite.setRotation(270);
				smer = 3;
				exitStatus = true;
			}
			break;
		case 3:
			if (ImageMap[(int)(x+UNITSIZE+Speed+STEP)][y] == C_PATH && !LastRight && x+UNITSIZE+Speed+STEP<window->getSize().x) // Right
			{
				sprite.setRotation(90);
				smer = 4;
				exitStatus = true;
			}
			break;
		}
	}
	}

	switch (smer)
	{
	case 1: 
			MoveUp();
			break;
	case 2: 
			MoveDown();
			break;
	case 3: 
			MoveLeft();
			break;
	case 4: 
			MoveRight();
			break;
	}
}

void Unit::Draw()
{
	if (Living)
	{
		sprite.setTexture(texture);	
		window->draw(sprite);
	}
}

void Unit::Draw(sf::Shader *shader)
{
	if (Living)
	{
		sprite.setTexture(texture);	
		window->draw(sprite, shader);
	}
}


Unit::~Unit(void)
{
}
