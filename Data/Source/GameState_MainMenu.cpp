#include "GameState_MainMenu.h"



GameState_MainMenu::GameState_MainMenu()
	: game(Game::get())
	, mSelector(game->mTextures.get(Texture::button_Selector), sf::Vector2f(20.0f, game->mWindow.getSize().y - 270.0f), 50.0f, mButtons)
{
	buildGUI();
	mSelector.mNumberOfButtons = mButtons.size();
	mSelector.mButtons = mButtons;
	mSelector.mSelectedButton = &mButtons[0];
	mSelector.mSelectedButton->getText().setFillColor(sf::Color::Red);
};

void GameState_MainMenu::buildGUI()
{
	TextButton playButton(sf::Vector2f(75.0f, game->mWindow.getSize().y - 250.0f), "Play", 20, TextButton::Action::buildGamePlay);
	TextButton optionsButton(sf::Vector2f(75.0f, game->mWindow.getSize().y - 200.0f), "Options", 20, TextButton::Action::buildOptions);
	TextButton scoreboardButton(sf::Vector2f(75.0f, game->mWindow.getSize().y - 150.0f), "Scoreboard", 20, TextButton::Action::buildScoreboard);
	TextButton exitButton(sf::Vector2f(75.0f, game->mWindow.getSize().y - 100.0f), "Exit Game", 20, TextButton::Action::exit);

	mButtons.push_back(playButton);
	mButtons.push_back(optionsButton);
	mButtons.push_back(scoreboardButton);
	mButtons.push_back(exitButton);
}

void GameState_MainMenu::update(const sf::Time deltaTime)
{
	handleInput();
}

void GameState_MainMenu::draw()
{
	game->mWindow.draw(game->mBackground);
	//game->mWindow.draw(mSelector.mSprite);
	for each (TextButton button in mButtons)
	{
		game->mWindow.draw(button.getText());
	}
}

void GameState_MainMenu::handleInput()
{
	handleEvents();
	handleRealTimeInput();
	
}

void GameState_MainMenu::handleEvents()
{
	sf::Event eventToBeHandled;
	while (this->game->mWindow.pollEvent(eventToBeHandled))
	{
		switch (eventToBeHandled.type)
		{
			case sf::Event::Closed:
			{
				game->mWindow.close();
				break;
			}

			case sf::Event::MouseButtonPressed:
			{
				switch (eventToBeHandled.mouseButton.button)
				{
					case sf::Mouse::Left:
					{
						if (mSelector.mSelectedButton->isSpriteSelected())
						{
							mSelector.mSelectedButton->triggerAction();
							sf::Clock timer;
							while (timer.getElapsedTime() < sf::seconds(0.15f))
							{

							}
						}
					}

					default:break;
				}

			}

			case sf::Event::KeyPressed:
			{
				switch (eventToBeHandled.key.code)
				{
					case sf::Keyboard::Return:
					{
						mSelector.mSelectedButton->triggerAction();
						break;
					}

					case sf::Keyboard::Space:
					{
						mSelector.mSelectedButton->triggerAction();
						break;
					}

					case sf::Keyboard::Down:
					{
						mSelector.mSelectedButton->getText().setFillColor(sf::Color::White);
						mSelector.move(Movement::down);
						mSelector.mSelectedButton->getText().setFillColor(sf::Color::Red);
						break;
					}

					case sf::Keyboard::Up:
					{
						mSelector.mSelectedButton->getText().setFillColor(sf::Color::White);
						mSelector.move(Movement::up);
						mSelector.mSelectedButton->getText().setFillColor(sf::Color::Red);
						break;
					}
				}
			}

			default:
				break;
		}
	}
}

void GameState_MainMenu::handleRealTimeInput()
{
	for (unsigned int i = 0; i < mButtons.size(); i++)
	{
		if (mButtons[i].isSpriteSelected())
			if (mButtons[i].getText().getFillColor() == sf::Color::White)
			{
				mSelector.mSelectedButton->getText().setFillColor(sf::Color::White);
				mButtons[i].getText().setFillColor(sf::Color::Red);
				mSelector.mSelectedButton = &mButtons[i];
			}
	}
}

void GameState_MainMenu::rePositionButtons(sf::Vector2u & currentSize, sf::Vector2u & newSize)
{
	
}
