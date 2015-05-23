#pragma once

#include <SFML/Graphics.hpp>
#include <sstream>

class ClickText
{
private:
	sf::RenderWindow *window;
	sf::Font *font;
	sf::Text text;
	sf::IntRect Position;
	sf::Color NormalColor;
	sf::Color HighlightColor;

public:
	ClickText(void);
	ClickText(sf::IntRect position, std::string textString, sf::RenderWindow *renderWindow, sf::Font *renderFont);
	ClickText(sf::IntRect position, std::string textString, int Size, sf::RenderWindow *renderWindow, sf::Font *renderFont);
	~ClickText(void);

	std::string Text;

	bool Selected;
	bool Clicked();
	void Draw();
	void SetString(std::string text);
	void SetColor(sf::Color normalColor, sf::Color highlightColor);
};

