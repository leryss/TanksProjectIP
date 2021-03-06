#pragma once
#include "Tank.h"
#include <vector>
#include <string>

class Player : public Tank {
public:
	Player(sf::Sprite* base, sf::Sprite* top, sf::Vector2f* pos, sf::Vector2f* vel, float health, float damage, std::vector<sf::Keyboard::Key> keys, size_t team);

	void	handleInput(sf::Time dt);
	void	update(sf::Time dt);
public:
	std::vector<sf::Keyboard::Key> mKeys;
	std::string					   mName;
};