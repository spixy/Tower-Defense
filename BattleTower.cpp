#include "BattleTower.h"

BattleTower::BattleTower(int X, int Y, float DMG, float RAN, float REL, int Tvalue, sf::RenderWindow *renderWindow, std::string textureName, GameInfo *GamePoints)
{
	if (!texture.loadFromFile(textureName))
	{
		throw new std::exception("Tower texture is missing");
	}
	sprite.setTexture(texture);
	sprite.setPosition(X,Y);
	sprite.setScale(0.625f,0.625f);

	gi = GamePoints;
	Position = sf::Vector2f(X,Y);
	Damage = DMG;
	Range = RAN;
	ReloadTime = REL;
	ActReload = 0;
	window = renderWindow;
	Static = false;
	dragging = false;
	Value = Tvalue;
}

BattleTower::~BattleTower(void)
{
}

void BattleTower::Update(std::vector<Unit> *EnemiesList, std::vector<std::vector<int>> *DMGlinesList)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
	sf::IntRect towerRect(Position.x, Position.y, 20, 20);
	sf::IntRect mouseRect(mousePos.x, mousePos.y, 1, 1);

	if (towerRect.intersects(mouseRect))
	{
		//text.setPosition(Position.x, Position.y - 20);
		DrawText = true;
	}
	else DrawText = false;

	if (!Static)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && gi->Points >= Value)
		{
			DrawText = false;
			if (towerRect.intersects(mouseRect) || dragging)
			{
				Position.x = mousePos.x - 10;
				Position.y = mousePos.y - 10;
				sprite.setPosition(Position);
				dragging = true;
			}
		}
		else if (dragging)
		{
			dragging = false;
			if (Position.y > 480)
			{
				Static = true;
			}
			else if (gi->Points - Value >= 0)
			{
				Static = true;
				gi->Points -= Value;
			}
			else
			{
				Static = false;
				Position.y = 550;
			}
		}
		return;
	}

	if (ActReload >= ReloadTime)
	{
		sf::Vector2f MyPos = sf::Vector2f(Position.x + 10, Position.y + 10);
		for (int i=0; i< EnemiesList->size(); i++)
		{
			sf::Vector2f EnemyPos = sf::Vector2f((*EnemiesList)[i].Position.x, (*EnemiesList)[i].Position.y);

			if ( sqrt( pow(abs(EnemyPos.x - MyPos.x),2) + pow(abs(EnemyPos.y - MyPos.y),2) ) <= Range + UNITSIZE )
			{
				std::vector<int> data;
				data.push_back((int)MyPos.x);
				data.push_back((int)MyPos.y);
				data.push_back((int)EnemyPos.x);
				data.push_back((int)EnemyPos.y);

				(*EnemiesList)[i].Health -= Damage;
				(*DMGlinesList).push_back(data);
				ActReload = 0;

				if ((*EnemiesList)[i].Health <= 0)
				{
					gi->Points += (*EnemiesList)[i].MaxHealth;
					gi->EnemyCount --;
					(*EnemiesList).erase((*EnemiesList).begin() + i);
				}
				return;
			}
		}
	}
	else ActReload += 0.1f;
}

void BattleTower::SetText(sf::Font *textFont)
{
	text.setFont(*textFont);
	text.setCharacterSize(18);
	text.setColor(sf::Color::Black);
	
	std::ostringstream oss;
	oss << "Damage: " << Damage << "\t  Range: " << Range << "\nReload: " << ReloadTime << "\t  Value: " << Value;
	text.setString(oss.str());
	text.setPosition(200, 510);
}

void BattleTower::Draw()
{
	sprite.setTexture(texture);
	window->draw(sprite);
	if (DrawText) window->draw(text);
}

void BattleTower::Draw(sf::Shader *shader)
{
	sprite.setTexture(texture);
	window->draw(sprite, shader);
	if (DrawText) window->draw(text);
}