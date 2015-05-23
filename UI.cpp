#include "UI.h"

UI::UI(void)
{
	window = NULL;
}

UI::UI(sf::RenderWindow *renderWindow, sf::Font *renderFont, GameSettings *settings)
{
	window = renderWindow;
	font = renderFont;
	gameSettings = settings;

	text.setFont(*font);
}

UI::~UI(void)
{
}

void UI::Sachovnica()
{
	sf::Vertex vertices[2];
	vertices[0].color = sf::Color::Black;
	vertices[1].color = sf::Color::Black;

	for (int i=0; i< 500; i+=20)
	{
		vertices[0].position = sf::Vector2f(0,i);
		vertices[1].position = sf::Vector2f(800,i);

		window->draw(vertices, 2, sf::Lines);
	}
	
	for (int i=0; i< 800; i+=20) 
	{
		vertices[0].position = sf::Vector2f(i,0);
		vertices[1].position = sf::Vector2f(i,500);

		window->draw(vertices, 2, sf::Lines);
	}
}

void UI::TopInfo(int hp, int pts, int lvl)
{
	text.setColor(sf::Color::Black);
	text.setCharacterSize(20);
	
	text.setPosition(0, 0);
	text.setString(Append("Health ",hp,""));
	window->draw(text);
	
	text.setPosition(700, 0);
	text.setString(Append("Points ",pts,""));
	window->draw(text);
	
	text.setPosition(706, 22);
	text.setString(Append("Level ",lvl+1,""));
	window->draw(text);
}

void UI::BottomMenu()
{
	ClickText GridSelect(sf::IntRect(660, 540, 75, 22), "Grid", window, font);
	ClickText SoundSelect(sf::IntRect(590, 510, 75, 22), "Sound", window, font);
	ClickText MusicSelect(sf::IntRect(590, 540, 75, 22), "Music", window, font);
	ClickText PauseSelect(sf::IntRect(660, 510, 75, 22), "Pause", window, font);
	ClickText MenuSelect(sf::IntRect(730, 510, 75, 22), "Menu", window, font);
	ClickText ExitSelect(sf::IntRect(730, 540, 75, 22), "Exit", window, font);
	ClickText speed1(sf::IntRect(500, 540, 15, 22), "1", window, font);
	ClickText speed2(sf::IntRect(525, 540, 15, 22), "2", window, font);
	ClickText speed4(sf::IntRect(550, 540, 15, 22), "4", window, font);

	if (gameSettings->speed == 1.0f) speed1.Selected = true;
	else if (gameSettings->speed == 2.0f) speed2.Selected = true;
	else if (gameSettings->speed == 4.0f) speed4.Selected = true;

	PauseSelect.SetColor(sf::Color::Black, sf::Color::Blue);
	GridSelect.SetColor(sf::Color::Black, sf::Color::Blue);
	SoundSelect.SetColor(sf::Color::Black, sf::Color::Blue);
	MusicSelect.SetColor(sf::Color::Black, sf::Color::Blue);
	MenuSelect.SetColor(sf::Color::Black, sf::Color::Blue);
	ExitSelect.SetColor(sf::Color::Black, sf::Color::Blue);
	speed1.SetColor(sf::Color::Black, sf::Color::Blue);
	speed2.SetColor(sf::Color::Black, sf::Color::Blue);
	speed4.SetColor(sf::Color::Black, sf::Color::Blue);

	window->clear(sf::Color(220,220,220));

	text.setColor(sf::Color::Black);
	text.setCharacterSize(22);
	text.setPosition(500, 510);
	text.setString("Speed");
	window->draw(text);

	if (PauseSelect.Clicked())
	{
		gameSettings->pause = !gameSettings->pause;
		sf::sleep(sf::milliseconds(150));
	}
	if (GridSelect.Clicked())
	{
		gameSettings->grid = !gameSettings->grid;
		sf::sleep(sf::milliseconds(150));
	}
	if (MenuSelect.Clicked())
	{
		gameSettings->menu = true;
	}
	if (SoundSelect.Clicked())
	{
		gameSettings->sound = !gameSettings->sound;
		sf::sleep(sf::milliseconds(150));
	}
	if (MusicSelect.Clicked())
	{
		gameSettings->music = !gameSettings->music;
		sf::sleep(sf::milliseconds(150));
	}
	if (ExitSelect.Clicked())
	{
		window->close();
	}
	
	if (speed1.Clicked())
	{
		gameSettings->speed = 1;
		window->setFramerateLimit(30);
		window->setVerticalSyncEnabled(gameSettings->vsync);
		speed1.Selected = true;
		speed2.Selected = false;
		speed4.Selected = false;
	}
	if (speed2.Clicked())
	{
		gameSettings->speed = 2;
		window->setFramerateLimit(60);
		window->setVerticalSyncEnabled(gameSettings->vsync);
		speed1.Selected = false;
		speed2.Selected = true;
		speed4.Selected = false;
	}
	if (speed4.Clicked())
	{
		gameSettings->speed = 4;
		window->setFramerateLimit(120);
		window->setVerticalSyncEnabled(false);
		speed1.Selected = false;
		speed2.Selected = false;
		speed4.Selected = true;
	}

	speed1.Draw();
	speed2.Draw();
	speed4.Draw();		
	PauseSelect.Draw();
	MenuSelect.Draw();
	MusicSelect.Draw();
	SoundSelect.Draw();
	GridSelect.Draw();
	ExitSelect.Draw();
}

void UI::StartMenu(int *maps, int *levels)
{
	bool menu = true;
	int mapCount = LoadMaps();
	int frames = 0;
	bool UsingShader = false;

	sf::Texture mapTexture[10];
	sf::Sprite mapSprite[10];
	sf::Shader shader;
    sf::Event event;
	
	sf::Vector2i mouse;

	if (sf::Shader::isAvailable())
		UsingShader = shader.loadFromFile("Data\\menu.frag", sf::Shader::Fragment);
	if (UsingShader) shader.setParameter("resolution", sf::Vector2f(800,580));

	sf::RenderTexture renderTexture;
	renderTexture.create(800, 580);
	renderTexture.clear(sf::Color::White);
	renderTexture.display();

	sf::Texture texture = renderTexture.getTexture();
	sf::Sprite sprite(texture);

	ClickText PlaySelect(sf::IntRect(5, 20, 100, 22), "Play", window, font);
	PlaySelect.SetColor(sf::Color::White, sf::Color::Red);

	ClickText ExitSelect(sf::IntRect(5, 380, 100, 22), "Exit", window, font);
	ExitSelect.SetColor(sf::Color::White, sf::Color::Red);

	ClickText mapSelect[10];

	ClickText levelSelect1(sf::IntRect(80, 200, 80, 22), "Default", window, font); 
	levelSelect1.SetColor(sf::Color::White, sf::Color::Red);
	if (*levels == 0 ) levelSelect1.Selected = true;

	ClickText levelSelect2(sf::IntRect(160, 200, 80, 22), "Custom", window, font); 
	levelSelect2.SetColor(sf::Color::White, sf::Color::Red);
	if (*levels == 1 ) levelSelect2.Selected = true;

	ClickText audioON(sf::IntRect(80, 260, 35, 22), "ON", window, font); 
	audioON.SetColor(sf::Color::White, sf::Color::Red);

	ClickText audioOFF(sf::IntRect(160, 260, 50, 22), "OFF", window, font); 
	audioOFF.SetColor(sf::Color::White, sf::Color::Red);
	audioOFF.Selected = true;

	ClickText musicON(sf::IntRect(80, 320, 35, 22), "ON", window, font); 
	musicON.SetColor(sf::Color::White, sf::Color::Red);

	ClickText musicOFF(sf::IntRect(160, 320, 50, 22), "OFF", window, font); 
	musicOFF.SetColor(sf::Color::White, sf::Color::Red);
	musicOFF.Selected = true;

	for (int i = 0; i < mapCount; i++)
	{
		mapSelect[i] = ClickText(sf::IntRect(80 + 85*i, 80, 70, 100), Append("Map",i+1,""), 22, window, font);
		mapSelect[i].SetColor(sf::Color::White, sf::Color::Red);
		mapTexture[i].loadFromFile(Append("Maps\\Map",i,".bmp"));
		mapSprite[i].setTexture(mapTexture[i]);
		mapSprite[i].setPosition(79+85*i, 120);
		//mapSprite[i].setScale(0.9f, 0.9f);
	}
	if (mapCount > 0) mapSelect[*maps].Selected = true;
	
	sf::Text text1, text2, text3, text4;

	text1.setFont(*font);
	text1.setCharacterSize(22);
	text1.setPosition(5, 80);
	text1.setString("Map: ");

	text2.setFont(*font);
	text2.setCharacterSize(22);
	text2.setPosition(5, 200);
	text2.setString("Levels: ");

	text3.setFont(*font);
	text3.setCharacterSize(22);
	text3.setPosition(5, 260);
	text3.setString("Sound: ");

	text4.setFont(*font);
	text4.setCharacterSize(22);
	text4.setPosition(5, 320);
	text4.setString("Music: ");

	while (window->isOpen() && menu)
    {
        while (window->pollEvent(event))
        {
			if (event.type == sf::Event::Closed) window->close();
        }

		if (UsingShader)
		{
			mouse = sf::Mouse::getPosition(*window);
			shader.setParameter("mouse", 800-mouse.x, mouse.y);
			shader.setParameter("time", frames/100.0f);
			window->draw(sprite, &shader);
		}
		else window->clear();

		if (PlaySelect.Clicked())
		{
			menu = false;
		}
		if (ExitSelect.Clicked())
		{
			window->close();
		}
		if (levelSelect1.Clicked())
		{
			levelSelect1.Selected = true;
			levelSelect2.Selected = false;
			*levels = 0;
		}
		if (levelSelect2.Clicked())
		{
			levelSelect1.Selected = false;
			levelSelect2.Selected = true;
			*levels = 1;
		}
		if (audioON.Clicked())
		{
			audioON.Selected = true;
			audioOFF.Selected = false;
			gameSettings->sound = true;
		}
		if (audioOFF.Clicked())
		{
			audioON.Selected = false;
			audioOFF.Selected = true;
			gameSettings->sound = false;
		}
		if (musicON.Clicked())
		{
			musicON.Selected = true;
			musicOFF.Selected = false;
			gameSettings->music = true;
		}
		if (musicOFF.Clicked())
		{
			musicON.Selected = false;
			musicOFF.Selected = true;
			gameSettings->music = false;
		}
		for (int i = 0; i < mapCount; i++)
		{
			if (mapSelect[i].Clicked())
			{
				for (int j = 0; j < mapCount; j++)
				{
					mapSelect[j].Selected = false;
				}
				mapSelect[i].Selected = true;
				*maps = i;
			}
			window->draw(mapSprite[i]);
			mapSelect[i].Draw();
		}
			
		window->draw(text1);
		window->draw(text2);
		window->draw(text3);
		window->draw(text4);

		PlaySelect.Draw();
		levelSelect1.Draw();
		levelSelect2.Draw();
		audioON.Draw();
		audioOFF.Draw();
		musicON.Draw();
		musicOFF.Draw();
		ExitSelect.Draw();
		
		window->display();
		frames++;
	}
}

int UI::LoadMaps()
{
	char filename[16];
	for (int i=0; i<10; i++)
	{
		strcpy(filename, "Maps\\map");
		filename[8] = i + (char)'0';
		filename[9] = '\0';
		strcat(filename, ".bmp");

		if (!exists(filename)) return i;
	}
	return -1;
}

bool UI::exists (const char *fname)
{
    FILE *file;
    if (file = fopen(fname, "r"))
    {
        fclose(file);
        return true;
    }
    return false;
}