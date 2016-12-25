#ifndef GAMESTATE_H

#define GAMESTATE_H


//CUSTOM LIBRARIES AND OTHER HELPERS



//SFML LIBRARIES
#include <SFML\System\Time.hpp>


class GameState
{

public:

public:
	virtual void									draw() = 0;
	virtual void									update(const sf::Time deltaTime) = 0;
	virtual void									handleInput() = 0;
	virtual void									handleEvents() = 0;
	virtual void									handleRealTimeInput() = 0;

private:


private:


};


#endif // !GAMESTATE_H