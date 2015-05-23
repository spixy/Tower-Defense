#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include <fstream>
#include <vector>
#include <windows.h>

#include "Unit.h"
#include "UI.h"
#include "GameData.h"
#include "ImageProcessing.h"
#include "BattleTower.h"
#include "ClickText.h"

void Draw(sf::RenderWindow *window);
void GenerateLevels(bool custom);
void SpawnTower(sf::RenderWindow *window, sf::Font *font);

std::vector<Unit> Enemies;
std::vector<BattleTower> Towers[TOWERTYPES];
std::vector<std::vector<int>> DMGlines;
std::vector<LevelInfo> Levels;

int ImageMap[800][500];
bool DrawRadiusCircle;
int TowerRadius;
sf::Vector2i Start;
sf::Vector2f TowerPosition;
sf::Sprite BackGround;

TowerInfo towerinfo[TOWERTYPES];
GameInfo gameinfo;
GameSettings gameSettings;
Castle MainCastle;
UI ui;

//int main(int argc, char* argv[])
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR, int nShowCmd )
{
	sf::Clock clock;
	sf::Clock gametime;
	sf::Font font;
	sf::Text text;
	sf::Texture texture;
    sf::Event event;
	sf::SoundBuffer laserbuffer, musicbuffer;
	sf::Sound laser, music;
	sf::ContextSettings *settings = new sf::ContextSettings;
	
	std::ifstream infile("config.txt");
	std::string line;
	float vsync=1, volume=100, FPS;
	bool AudioLoaded;

	/* Load config */
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		std::string a;
		float b;
		if (!(iss >> a >> b)) continue;

		if (a == "antialiasing")
		{
				gameSettings.AA = b;
				settings->antialiasingLevel = gameSettings.AA;
		}
		else if (a == "vsync") gameSettings.vsync = b;
		else if (a == "speed") gameSettings.speed = b;
		else if (a == "sounds") gameSettings.soundVolume = b;
		else if (a == "music") gameSettings.musicVolume = b;
	}

	sf::RenderWindow window(sf::VideoMode(800, 580), "Tower Defense", sf::Style::Close, *settings);
	window.setVerticalSyncEnabled(gameSettings.vsync);
	window.setFramerateLimit(30*gameSettings.speed);
	free(settings);

	gameSettings.menu = true;
	gameSettings.pause = false;
	gameSettings.sound = false;
	gameinfo.EnemyCount =0;
	gameinfo.Level = -1;
	gameinfo.Points = 300;
	int Frames = 0;
	int FrameTime = 0;
	int UnitsToSpawn = 0;
	int ActSpawnTime = 0;
	int FreeTowers = 0;
	int MapIndex = 0;
	int LevelIndex = 0;
	std::ostringstream UnitTexture;

    AudioLoaded = laserbuffer.loadFromFile("Audio\\laser.wav") &&
			musicbuffer.loadFromFile("Audio\\music.wav");

	if (!font.loadFromFile("Data\\font.ttf"))
	{
		throw new std::exception("Font is missing");
		return 255;
	}

	laser.setBuffer(laserbuffer);
	laser.setVolume(gameSettings.soundVolume);

	music.setBuffer(musicbuffer);
	music.setVolume(gameSettings.musicVolume);
	music.setLoop(true);

	ui = UI(&window, &font, &gameSettings);
	srand(time(NULL));
		
	while (window.isOpen())
    {
		clock.restart();

        while (window.pollEvent(event))
        {
			if (event.type == sf::Event::Closed)
				window.close();
        }

		if (gameSettings.pause)
		{
			Draw(&window);
			sf::sleep(sf::milliseconds(100));
			continue;
		}
		if (gameSettings.menu)
		{
			music.stop();
			ui.StartMenu(&MapIndex, &LevelIndex);

			Enemies.clear();
			for (int i=0; i< TOWERTYPES; i++)
				Towers[i].clear();

			GenerateLevels((LevelIndex==1));

			if (!texture.loadFromFile(Append("Maps\\map", MapIndex, ".bmp"), sf::IntRect(0, 0, 800, 500)))
			{
				throw new std::exception("Map texture is missing");
			}
			BackGround.setTexture(texture);
			BackGround.setScale(MAPSCALE,MAPSCALE);

			ImageProcessing::ImageProcess(ImageMap, &BackGround);
			Start = ImageProcessing::FindStart(ImageMap);

			MainCastle.Position = ImageProcessing::FindCastle(ImageMap);
			MainCastle.Health = 20;
			gameSettings.menu = false;
			gameSettings.pause = false;
			gameinfo.EnemyCount =0;
			gameinfo.Level = -1;
			gameinfo.Points = 300;
			Frames = 0;
			UnitsToSpawn = 0;
			ActSpawnTime = 0;
			FreeTowers = 0;
		}

		SpawnTower(&window, &font);
		
		if (gameinfo.EnemyCount == 0 && UnitsToSpawn == 0 && Frames > 100)
		{
			gameinfo.Level++;
			UnitsToSpawn = Levels [ gameinfo.Level ].Count;

			UnitTexture.str("");
			ActSpawnTime = 0;
			UnitTexture << "Units\\Unit" << Levels [ gameinfo.Level ].UnitIndex << ".png";
		}

		if (UnitsToSpawn > 0 && ActSpawnTime >= 500)
		{
			gameinfo.EnemyCount++;
			Enemies.push_back(Unit(Start.x, Start.y, Levels[gameinfo.Level].HP, Levels[gameinfo.Level].Speed, &window, UnitTexture.str(), &MainCastle, &gameinfo));
			UnitsToSpawn --;
			ActSpawnTime = 0;
		}
		
		for (int i=0; i< TOWERTYPES; i++)
			for (int j=0; j< Towers[i].size(); j++)
				Towers[i][j].Update(&Enemies, &DMGlines);

		for (int i=0; i< Enemies.size(); i++)
		{
			Enemies[i].Update(ImageMap);
			if (!Enemies[i].Living) Enemies.erase(Enemies.begin()+i);
		}

		if (gameSettings.sound && DMGlines.size() > 0 && AudioLoaded) laser.play();
		if (gameSettings.music && music.getStatus() == sf::Music::Status::Stopped) music.play();
		else if (!gameSettings.music && music.getStatus() == sf::Music::Status::Playing) music.stop();

		Draw(&window);

		if (MainCastle.Health <= 0)
		{
			ClickText ct(sf::IntRect(320, 250, 0, 0), "GAME OVER!", 40, &window, &font);
			ct.SetColor(sf::Color::Black, sf::Color::Black);
			ct.Draw();
			window.display();
			sf::sleep(sf::milliseconds(2000));
			gameSettings.menu = true;
		}
		if (gameinfo.Level == Levels.size())
		{
			ClickText ct(sf::IntRect(320, 250, 0, 0), "YOU WON!", 40, &window, &font);
			ct.SetColor(sf::Color::Black, sf::Color::Black);
			ct.Draw();
			window.display();
			sf::sleep(sf::milliseconds(2000));
			gameSettings.menu = true;
		}

		Frames++;
		FrameTime = clock.getElapsedTime().asMilliseconds();
		ActSpawnTime += FrameTime;
		FPS = 1000 / (float)FrameTime;
	}

    return 0;
}

void Draw(sf::RenderWindow *window)
{
	std::vector<int> output;
	sf::Vertex vertices[2];
	vertices[0].color = sf::Color::Blue;
	vertices[1].color = sf::Color::Blue;

	ui.BottomMenu();
	
	window->draw(BackGround);
	if (gameSettings.grid) ui.Sachovnica();
	
	for (int i=0; i< DMGlines.size(); i++)
	{
		output = DMGlines[i];

		vertices[0].position = sf::Vector2f(output[0],output[1]);
		vertices[1].position = sf::Vector2f(output[2],output[3]);

		window->draw(vertices, 2, sf::Lines);
	}

	for (int i=0; i< TOWERTYPES; i++)
		for (int j=0; j< Towers[i].size(); j++)
			Towers[i][j].Draw();

	for (int i=0; i< Enemies.size(); i++)
		Enemies[i].Draw();

	if (DrawRadiusCircle)
	{
		sf::CircleShape shape(TowerRadius);
		shape.setPosition(TowerPosition);
		if (TowerPosition.y + TowerRadius < 490)
			shape.setFillColor(sf::Color(128, 200, 128, 80));
		else
			shape.setFillColor(sf::Color(255, 0, 0, 80));
		window->draw(shape);
	}
	
	ui.TopInfo(MainCastle.Health, gameinfo.Points, gameinfo.Level); 
	
    window->display();
	
	DMGlines.clear();
}

void GenerateLevels(bool custom)
{
	LevelInfo LI;
	Levels.clear();

	if (custom)
	{
		std::ifstream infile("customlevels.txt");
		std::string line;
		while (std::getline(infile, line))
		{
			std::istringstream iss(line);
			int a, b, d;
			float c;

			if (!(iss >> a >> b >> c >> d))
				continue;

			LI.Count = a;
			LI.HP = b;
			LI.Speed = c;
			LI.UnitIndex = d;
			Levels.push_back(LI);
		}
	}
	else
	{
		for (int i=0; i < 100; i++)
		{
			LI.Count = 1 + i*2;
			LI.HP = 2*(10 - (i % 10)); // 2-20
			LI.Speed = 1 + (i % 10);  // 1-10
			LI.UnitIndex = 1 + (i % 8); // 1-8
			Levels.push_back(LI);
		}
	}

	towerinfo[0].Damage = 1;
	towerinfo[0].Range = 100;
	towerinfo[0].Reload = 0.5f;
	towerinfo[0].Value = 100;

	towerinfo[1].Damage = 3;
	towerinfo[1].Range = 60;
	towerinfo[1].Reload = 1;
	towerinfo[1].Value = 120;

	towerinfo[2].Damage = 3;
	towerinfo[2].Range = 100;
	towerinfo[2].Reload = 0.8f;
	towerinfo[2].Value = 200;

	towerinfo[3].Damage = 4;
	towerinfo[3].Range = 150;
	towerinfo[3].Reload = 1;
	towerinfo[3].Value = 300;

	towerinfo[4].Damage = 8;
	towerinfo[4].Range = 50;
	towerinfo[4].Reload = 1.2f;
	towerinfo[4].Value = 450;

	towerinfo[5].Damage = 10;
	towerinfo[5].Range = 100;
	towerinfo[5].Reload = 1;
	towerinfo[5].Value = 600;
}

void SpawnTower(sf::RenderWindow *window, sf::Font *font)
{
	DrawRadiusCircle = false;
	int FreeTowers;

	for (int i=0; i<TOWERTYPES; i++)
	{
		FreeTowers = 0;
		for (int j=0; j< Towers[i].size(); j++)
		{
			if (!Towers[i][j].Static) // nestriela
			{
				if (Towers[i][j].dragging)
				{
					TowerRadius = (int)Towers[i][j].Range;
					TowerPosition = sf::Vector2f(Towers[i][j].Position.x + UNITSIZE - TowerRadius, Towers[i][j].Position.y + UNITSIZE - TowerRadius);
					DrawRadiusCircle = true;
				}
				else FreeTowers++;
			}
			else if (Towers[i][j].Position.y >= 480) // "striela"
			{
				Towers[i].erase( Towers[i].begin() + j);
			}
		}
		if (FreeTowers == 0)
		{
			BattleTower bt(BattleTower(10+30*i,520, towerinfo[i].Damage, towerinfo[i].Range, towerinfo[i].Reload, towerinfo[i].Value, window, Append("Units\\Tower",i+1,".png"), &gameinfo));
			bt.SetText(font);
			Towers[i].push_back(bt);
		}
	}
}