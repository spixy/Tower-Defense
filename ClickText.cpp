#include "ClickText.h"


ClickText::ClickText(void)
{
}


ClickText::ClickText(sf::IntRect position, std::string textString, int Size, sf::RenderWindow *renderWindow, sf::Font *renderFont)
{
	Text = textString;
	Position = position;
	window = renderWindow;
	font = renderFont;

	text.setFont(*font);
	text.setString(textString);
	text.setPosition(position.left, position.top);
	text.setCharacterSize(Size);
	text.setColor(sf::Color::White);
	Selected = false;
}

ClickText::ClickText(sf::IntRect position, std::string textString, sf::RenderWindow *renderWindow, sf::Font *renderFont)
{
	Text = textString;
	Position = position;
	window = renderWindow;
	font = renderFont;

	text.setFont(*font);
	text.setString(textString);
	text.setPosition(position.left, position.top);
	text.setCharacterSize(position.height);
	text.setColor(sf::Color::White);
	Selected = false;
}


ClickText::~ClickText(void)
{
}

void ClickText::SetColor(sf::Color normalColor, sf::Color highlightColor)
{
	NormalColor = normalColor;
	HighlightColor = highlightColor;

	text.setColor(normalColor);
}

bool ClickText::Clicked()
{
	if (Selected)
	{
		text.setColor(HighlightColor);
		return false;
	}

	sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
	sf::IntRect mouseRect(mousePos.x, mousePos.y, 1, 1);
	sf::IntRect textRect(Position.left, Position.top, Position.width, Position.height);

	if (textRect.intersects(mouseRect))
	{
		text.setColor(HighlightColor);
		return (sf::Mouse::isButtonPressed(sf::Mouse::Left));
	}
	else
	{
		text.setColor(NormalColor);
		return false;
	}
}

void ClickText::Draw()
{
	window->draw(text);
}


void ClickText::SetString(std::string textString)
{
	Text = textString;
	text.setString(textString);
}