#include "Game.hpp"
#include "GUI.hpp"
#include "Powerups.hpp"
#include <fstream>
#include <iostream>

void Game::InitTextures()
{
	Powerup::loadTextures();
    Tile::loadTextures();
    player.loadTextures();

    std::string fileName;
    for (int i = 0; i < 10; i++) {
        fileName = std::string("assets/images/clock/digit") + char(i + 48) + std::string(".png");
        clockDigit_t[i].loadFromFile(fileName);
    }
    clockDigit_t[10].loadFromFile("assets/images/clock/colon.png");

    for (int i = 0; i < 11; i++) {
        clockDigits[i].setTexture(clockDigit_t[i]);
        clockDigits[i].setPosition(38 * i, 0);
    }
}

void Game::InitWindow()
{
    sf::RenderTexture falseWin;
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
	window.create(500, 600, settings);
}

void Game::InitVariables()
{
    mapSize[0] = sf::Vector2i(23, 23); mapSize[1] = sf::Vector2i(37, 37); mapSize[2] = sf::Vector2i(53, 53); mapSize[3] = sf::Vector2i(61, 43); mapSize[4] = sf::Vector2i(353, 96);
	currentLvl = 1;
    levels = 4;
    visiblePath = false;
    hasGameStarted = false;
    setMap();
    pathFind(currentLvl);
}

Game::Game():
    player(this), tileSize(50, 50), cam(sf::Vector2f(500, 600))
{
	this->font.loadFromFile("assets/fonts/Amel Brush Font.ttf");
	this->InitTextures();
	this->InitWindow();
	this->InitVariables();
}

Game::~Game()
{
}

void Game::setMap()
{
    Powerup::Group.clear(); Powerup::ActiveGroup.clear();
	path.clear(); wall.clear(); shortestPath.clear(); nonWall.clear(); pathXY.clear(); 

    std::string line;
    std::ifstream file;
    std::string fileName(std::string("assets/maps/map (") + char(currentLvl + 48) + ").txt");
    file.open(fileName, std::ios::in);

    for (int i = 0; i < mapSize[currentLvl - 1].y; i++) {
        std::getline(file, line);
        for (int j = 0; j < mapSize[currentLvl - 1].x; j++) {
            if (line[j] == 'P') {
                player.set(sf::Vector2f((j + 0.5) * tileSize.x, (i + 0.5) * tileSize.y));
                nonWall.push_back(Tile(sf::Vector2f(j * tileSize.x, i * tileSize.y), tileSize, Tile::allTextures["path"]));
            }
            if (line[j] == '@') {
                wall.push_back(Tile(sf::Vector2f(j * tileSize.x, i * tileSize.y), tileSize, Tile::allTextures["wall"]));
            }
            else if (line[j] == 'F') {
                nonWall.push_back(Tile(sf::Vector2f(j * tileSize.x, i * tileSize.y), tileSize, Tile::allTextures["path"]));
                endTile = Tile(sf::Vector2f(j * tileSize.x, i * tileSize.y), tileSize, Tile::allTextures["end"]);
            }
            else if (line[j] == ' ') {
                path.push_back(Tile(sf::Vector2f(j * tileSize.x, i * tileSize.y), tileSize, Tile::allTextures["path"]));
                pathXY.push_back(sf::Vector2i(j, i));
            }
        }
    }
    file.close();

    int power = 0;
    srand(time(0));
    int nums[13];
    for (; power < 13; power++) {
        bool running = true;
        while (running) {
            running = false;
            nums[power] = rand() % path.size();
            for (int j = 0; j < power; j++)
                if (nums[j] == nums[power])running = true;
        }
    }
    for (int i = 0; i < 3; i++) {
        Powerup::Group.push_back(Powerup(0, path[nums[i]].hitbox));
    }
    for (int i = 0; i < 2; i++) {
        Powerup::Group.push_back(Powerup(1, path[nums[i + 3]].hitbox));
    }
    for (int i = 0; i < 1; i++) {
        Powerup::Group.push_back(Powerup(7, path[nums[i + 5]].hitbox));
    }
    for (int i = 0; i < 3; i++) {
        Powerup::Group.push_back(Powerup(2, path[nums[i + 6]].hitbox));
    }
    for (int i = 0; i < 1; i++) {
        Powerup::Group.push_back(Powerup(3, path[nums[i + 9]].hitbox));
    }
    for (int i = 0; i < 1; i++) {
        Powerup::Group.push_back(Powerup(4, path[nums[i + 10]].hitbox));
    }
    for (int i = 0; i < 1; i++) {
        Powerup::Group.push_back(Powerup(5, path[nums[i + 11]].hitbox));
    }
    for (int i = 0; i < 1; i++) {
        Powerup::Group.push_back(Powerup(6, path[nums[i + 12]].hitbox));
    }
}

void Game::drawMap(sf::RenderTarget& win) {
    if (player.getPosition().x < 250 && currentLvl == 2) {
        //std::cput 
    }
    for (unsigned int i = 0; i < wall.size(); i++) {
        cam.apply(wall[i].hitbox);
        wall[i].update();
        win.draw(wall[i].image);
    }
    for (unsigned int i = 0; i < path.size(); i++) {
        cam.apply(path[i].hitbox);
        path[i].update();
        win.draw(path[i].image);
    }
    for (unsigned int i = 0; i < nonWall.size(); i++) {
        cam.apply(nonWall[i].hitbox);
        nonWall[i].update();
        win.draw(nonWall[i].image);
    }
    for (unsigned int i = 0; i < shortestPath.size(); i++) {
        cam.apply(shortestPath[i].hitbox);
        shortestPath[i].update();
        if (visiblePath)win.draw(shortestPath[i].image);
    }
    for (unsigned int i = 0; i < Powerup::Group.size(); i++) {
        //if(!powerups[i].isAvailable)continue;
        cam.apply(Powerup::Group[i].hitbox);
        Powerup::Group[i].update();
        win.draw(Powerup::Group[i].image);
    }
    cam.apply(endTile.hitbox);
    endTile.update();
    win.draw(endTile.image);
}

void Game::loadNextLevel() {
    if (currentLvl == levels)currentLvl = 1;
    else currentLvl++;
    setMap();
    pathFind(currentLvl);
    visiblePath = false;
    hasGameStarted = false;
    resetClock();
}

void Game::pathFind(int lvl) {
    shortestPath.clear();
    sf::Vector3i startPos;
    std::vector<sf::Vector3i> mainList;
    std::vector<std::vector<char>> maze(mapSize[lvl - 1].y, std::vector<char>(mapSize[lvl - 1].x, NULL));
    std::string line;
    std::ifstream file;
    std::string fileName(std::string("assets/maps/map (") + char(currentLvl + 48) + ").txt");
    file.open(fileName, std::ios::in);
    for (int i = 0; i < mapSize[lvl - 1].y; i++) {
        std::getline(file, line);
        for (int j = 0; j < mapSize[lvl - 1].x; j++) {
            maze[i][j] = line[j];
            if (maze[i][j] == 'F')mainList.push_back(sf::Vector3i(j, i, 0));
            if (maze[i][j] == 'P')startPos = sf::Vector3i(j, i, 0);
        }
    }
    file.close();
    bool done = false;
    std::vector<sf::Vector3i> subList;
    int k = 0;
    while (!done) {
        subList.clear();
        int i = mainList[k].y; int j = mainList[k].x;
        if (maze[i - 1][j] != 'Q' && maze[i - 1][j] != '@')subList.push_back(sf::Vector3i(j, i - 1, mainList[k].z + 1));
        if (maze[i + 1][j] != 'Q' && maze[i + 1][j] != '@')subList.push_back(sf::Vector3i(j, i + 1, mainList[k].z + 1));
        if (maze[i][j - 1] != 'Q' && maze[i][j - 1] != '@')subList.push_back(sf::Vector3i(j - 1, i, mainList[k].z + 1));
        if (maze[i][j + 1] != 'Q' && maze[i][j + 1] != '@')subList.push_back(sf::Vector3i(j + 1, i, mainList[k].z + 1));
        for (unsigned int h = 0; h < mainList.size(); h++) {
            for (unsigned int s = 0; s < subList.size(); s++) {
                if (mainList[h].x == subList[s].x && mainList[h].y == subList[s].y && mainList[h].z <= subList[s].z)
                    subList.erase(subList.begin() + s);
            }
        }
        for (unsigned int s = 0; s < subList.size(); s++) {
            mainList.push_back(subList[s]);
        }
        for (unsigned int h = 0; h < mainList.size(); h++) {
            if (mainList[h].x == startPos.x && mainList[h].y == startPos.y) {
                done = true; startPos.z = mainList[h].z; break;
            }
        }
        k++;
    }
    std::vector<std::vector<int>> nMaze(mapSize[lvl - 1].y, std::vector<int>(mapSize[lvl - 1].x, -1));
    for (unsigned int i = 0; i < mainList.size(); i++) {
        nMaze[mainList[i].y][mainList[i].x] = mainList[i].z;
    }
    std::vector<sf::Vector3i> pathList;
    pathList.push_back(startPos);
    done = false;
    k = 0;
    while (!done) {
        bool isAdded = false;
        int i = pathList[k].y; int j = pathList[k].x;
        if (nMaze[i - 1][j] != -1) { isAdded = true; pathList.push_back(sf::Vector3i(j, i - 1, nMaze[i - 1][j])); }
        if (nMaze[i + 1][j] != -1 && nMaze[i + 1][j] < pathList.back().z) {
            if (isAdded)pathList.pop_back(); pathList.push_back(sf::Vector3i(j, i + 1, nMaze[i + 1][j]));
        }
        if (nMaze[i][j - 1] != -1 && nMaze[i][j - 1] < pathList.back().z) {
            if (isAdded)pathList.pop_back(); pathList.push_back(sf::Vector3i(j - 1, i, nMaze[i][j - 1]));
        }
        if (nMaze[i][j + 1] != -1 && nMaze[i][j + 1] < pathList.back().z) {
            if (isAdded)pathList.pop_back(); pathList.push_back(sf::Vector3i(j + 1, i, nMaze[i][j + 1]));
        }

        k++;
        if (pathList.back().z == 0)done = true;
    }

    sf::Vector2f pos;
    for (unsigned int i = 0; i < pathList.size(); i++) {
        for (unsigned int k = 0; k < pathXY.size(); k++) {
            if (pathXY[k].x == pathList[i].x && pathXY[k].y == pathList[i].y)
            {
                pos = path[k].hitbox.getPosition(); break;
            }
        }
        shortestPath.push_back(Tile(pos, tileSize, Tile::allTextures["find"]));
        cam.apply(shortestPath.back().hitbox);
        shortestPath.back().update();
    }
}

void Game::draw(sf::RenderWindow& window)
{
    if (state == MENU || state == LEVEL)
    {
        window.draw(menuImg);
    }

    else if (state == RUNNING)
    {
        player.lightUp(this->window);
        player.showLight(window);

        nSeconds = clockTime.asSeconds();
        nMinutes = nSeconds / 60;
        nSeconds = nSeconds % 60;

        int dx = window.getSize().x - 167, dy = 0;
        clockDigits[int(nMinutes / 10)].setPosition(0 + dx, 0 + dy);
        window.draw(clockDigits[int(nMinutes / 10)]);
        clockDigits[int(nMinutes % 10)].setPosition(38 + dx, 0 + dy);
        window.draw(clockDigits[int(nMinutes % 10)]);
        clockDigits[10].setPosition(76 + dx, 0 + dy);
        window.draw(clockDigits[10]);
        clockDigits[int(nSeconds / 10)].setPosition(87 + dx, 0 + dy);
        window.draw(clockDigits[int(nSeconds / 10)]);
        clockDigits[int(nSeconds % 10)].setPosition(125 + dx, 0 + dy);
        window.draw(clockDigits[int(nSeconds % 10)]);
    }
    else if (state == PAUSED)
    {
        player.goalPointer.scale(1.2, 1.2);
        sf::CircleShape circle(player.goalPointer.getRadius());
        circle.setOrigin(circle.getRadius(), circle.getRadius());
        circle.setPosition(player.goalPointer.getPosition());
        circle.setFillColor(sf::Color(141, 35, 166));

        window.draw(player.goalPointer);
        window.draw(circle);

        player.goalPointer.scale(1 / 1.2, 1 / 1.2);
    }
}

void Game::update(sf::Vector2f mousePos)
{
    if (state == RUNNING) {
        if (mousePos.x > 0 && mousePos.y > 0 && mousePos.x < window.getSize().x && mousePos.y < window.getSize().y) {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))player.move(player.speed);
            sf::Vector2f d = sf::Vector2f(mousePos) - sf::Vector2f(250, 300);
            if (d.y != 0) {
                float angle = 0;
                if (d.y > 0 && d.x > 0)angle = -180;
                if (d.y > 0 && d.x < 0)angle = 180;
                if (d.x != 0)player.rotate((180 / 3.14) * atan(-d.x / d.y) + angle);
            }
        }
        player.checkPowerup();

        if (mainClock.getElapsedTime().asSeconds() >= 1) {
            clockTime += sf::seconds(1);
            mainClock.restart();
        }

        if (player.checkWin())loadNextLevel();
    }
}

void Game::resetClock()
{
    mainClock.restart();
    clockTime = sf::seconds(0);
    nSeconds = 0; nMinutes = 0;
}

void Game::setup(Game::State state)
{
	gui::deleteAllGuiGroups();
	if (state == MENU)
	{
        window.clear();
        drawMap(window);
        window.display();
        menuImg.setTexture(window.getTexture());

		gui::Button::Group["START"] = new gui::Button(sf::RectangleShape({ 200, 80 }), sf::Text("START", this->font, 50));
		gui::Button::Group["START"]->setPosition(sf::Vector2f(window.getSize()) * 0.5f - sf::Vector2f(100, 40));
        gui::Button::Group["START"]->setBackgroundColor(sf::Color::Transparent);
		gui::Button::Group["START"]->updateTheme();
		gui::Button::Group["START"]->setHighlight(sf::Color(23, 23, 23, 50), sf::Color(141, 35, 166), 5, sf::Color::White);
		gui::Button::Group["START"]->setAction([this] {this->setup(RUNNING); });

        gui::Button::Group["level"] = new gui::Button(*gui::Button::Group["START"]);
        gui::Button::Group["level"]->setPosition(sf::Vector2f(window.getSize()) * 0.5f - sf::Vector2f(100, -50));
        gui::Button::Group["level"]->setString("LEVEL");
        gui::Button::Group["level"]->setAction([this] {this->setup(LEVEL); });
	}

	else if (state == PAUSED)
	{
        gui::Textbox::Group["paused"] = new gui::Textbox(sf::RectangleShape({ player.goalPointer.getRadius() * 2, 30 }), sf::Text("Paused", this->font, 50));
        gui::Textbox::Group["paused"]->setBackgroundColor(sf::Color::Transparent);
        gui::Textbox::Group["paused"]->setOutlineColor(sf::Color::Transparent);
        gui::Textbox::Group["paused"]->setPosition(sf::Vector2f(window.getSize()) * 0.5f - sf::Vector2f(175, 100));

        gui::Button::Group["restart"] = new gui::Button(sf::RectangleShape({ 200, 80 }), sf::Text("RESTART", this->font, 50));
        gui::Button::Group["restart"]->setPosition(sf::Vector2f(window.getSize()) * 0.5f - sf::Vector2f(90, 0));
        gui::Button::Group["restart"]->setBackgroundColor(sf::Color::Transparent);
        gui::Button::Group["restart"]->setOutlineColor(sf::Color::Transparent);
        gui::Button::Group["restart"]->setTextFillColor(sf::Color(161, 55, 186));
        gui::Button::Group["restart"]->updateTheme();
        gui::Button::Group["restart"]->setHighlight(sf::Color::Transparent, sf::Color::Transparent, 5, sf::Color::White);
        gui::Button::Group["restart"]->setAction([this] {this->currentLvl--; this->loadNextLevel(); this->setup(RUNNING); });

        gui::Button::Group["exit"] = new gui::Button(*gui::Button::Group["restart"]);
        gui::Button::Group["exit"]->setString("EXIT");
        gui::Button::Group["exit"]->setPosition(sf::Vector2f(window.getSize()) * 0.5f - sf::Vector2f(90, -50));
        gui::Button::Group["exit"]->setAction([this] {this->setup(END); });

        gui::Button::Group["menu"] = new gui::Button(*gui::Button::Group["restart"]);
        gui::Button::Group["menu"]->setString("MENU");
        gui::Button::Group["menu"]->setPosition(sf::Vector2f(window.getSize()) * 0.5f - sf::Vector2f(90, 50));
        gui::Button::Group["menu"]->setAction([this] {this->currentLvl = 0; this->loadNextLevel(); this->setup(MENU); });

	}

    else if (state == LEVEL)
    {
        gui::Button sample(sf::RectangleShape({ 100,100 }), sf::Text("", this->font, 60));
        sample.setPosition({ 100, 100 });
        sample.setBackgroundColor(sf::Color::Transparent);
        sample.setOutlineColor(sf::Color::Red);
        sample.updateTheme();
        sample.setHighlight(sf::Color::Transparent, sf::Color::Green, 3, sf::Color::White);

        for (int i = 0; i < levels; i++)
        {
            std::string key(std::string() + char(i + 1 + 48));
            gui::Button::Group[key] = new gui::Button(sample);
            gui::Button::Group[key]->setString(key);
            gui::Button::Group[key]->setPosition(sample.getPosition() + sf::Vector2f((i % 2) * 200, (i / 2) * 150));
            gui::Button::Group[key]->setAction([this, i] {this->currentLvl = i; loadNextLevel(); this->setup(RUNNING); });
        }
    }

    this->state = state;
}

