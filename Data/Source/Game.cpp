#include "Game.h"
#include "GameState_MainMenu.h"
#include <iostream>

const sf::Time Game::timePerFrame = sf::seconds(1.0f / 60.0f);
Game *Game::instance = nullptr;

Game::Game(unsigned int w, unsigned int h, bool Vsync)
	: mWidth(1344),
	mHeight(768),
	mWindow(sf::VideoMode(1344, 768), "Thunder Tanks", sf::Style::Default) {

	mWindow.setVerticalSyncEnabled(Vsync);
	mWindow.setKeyRepeatEnabled(false);
	
	instance = this;
	loadTextures();
	mBackground.setTexture(mTextures.get(Texture::background_MainMenu));

	sf::Vector2f mWindowPosition = sf::Vector2f(mWindow.getSize());
	mView.setSize(mWindowPosition);
	mView.setCenter(mWindowPosition * 0.5f);
	mWindow.setView(mView);
	mBackground.setScale(mWindow.getSize().x / 1920.0f, mWindow.getSize().y / 1080.0f);


	changeState(new GameState_MainMenu());
	mClock.restart();
}

Game::Game(unsigned int w, unsigned int h, bool vSync, bool fullscreen)
	: mWidth(1344),
	mHeight(768),
	mWindow(sf::VideoMode(1344, 768), "Thunder Tanks", sf::Style::Fullscreen)
{
	mWindow.setVerticalSyncEnabled(vSync);


	instance = this;
	loadTextures();
	mBackground.setTexture(mTextures.get(Texture::background_MainMenu));

	sf::Vector2f mWindowPosition = sf::Vector2f(mWindow.getSize());
	mView.setSize(mWindowPosition);
	mView.setCenter(mWindowPosition * 0.5f);
	mWindow.setView(mView);
	mBackground.setScale(mWindow.getSize().x / 1920.0f, mWindow.getSize().y / 1080.0f);

	changeState(new GameState_MainMenu());
	/*std::string mapObjectsPath("Assets/Maps/map1_Objects.tmap");
	std::string mapCharactersPath("Assets/Maps/map1_Characters.tmap");
	mMap = new Map(mapObjectsPath, mapCharactersPath);*/

	mClock.restart();
}

Game::~Game()
{

}

void Game::loadTextures() {
	std::ifstream in("Assets/Textures/asset_path.txt");
	std::string path;
	int i = 0;

	while (!in.eof()) {
		in >> path;
		mTextures.load(static_cast<Texture>(i++), path);
	}
	in.close();

	mFonts.load(Font::VanillaExtractRegular, "Assets/Fonts/VanillaExtractRegular.ttf");
}

Map* Game::getMap() {
	return mMap;
}

void Game::update(sf::Time deltaTime)
{	

	GameState *currentState = getActiveState();
//	handleInput();
	if (currentState != nullptr)
	{
		currentState->update(deltaTime);
	}

	

	//mMap->update(deltaTime);
}

void Game::render()
{
	//mMap->draw(&mWindow);
	GameState *currentState = getActiveState();
	if (currentState != nullptr)
	{
		mWindow.clear();
		currentState->draw();
	}

	mWindow.display();
}

void Game::processEvents()
{
	sf::Event eventToBeHandled;

	while (mWindow.pollEvent(eventToBeHandled))
	{
		switch (eventToBeHandled.type)
		{
			case sf::Event::Closed:
			{
				mWindow.close();
				break;
			}

			case sf::Event::KeyPressed:
			{
				switch (eventToBeHandled.key.code)
				{
					default:break;
				}

				break;
			}

			
			
			default:break;
		}
	}
}

void Game::handleInput()
{
	processEvents();
}

void Game::run()
{
	
	sf::Clock gameClock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (mWindow.isOpen())
	{
		sf::Time elapsedTime = gameClock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			update(timePerFrame);
		}
		render();
	}
}

void Game::pushState(GameState *state)
{
	this->stateStack.push(state);
}

void Game::popState()
{
	//delete stateStack.top();
	stateStack.pop();
}

void Game::deleteTopState()
{
	delete stateStack.top();
	stateStack.pop();
}

void Game::changeState(GameState *state)
{
	if (!stateStack.empty())
		popState();
	pushState(state);
}

GameState* Game::getActiveState()
{
	if (stateStack.empty())
		return nullptr;

	return stateStack.top();
}