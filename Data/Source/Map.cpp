#include "Map.h"
#include "Tank.h"
#include "Player.h"
#include "Game.h"
#include "AI.h"
#include "Obstacle.h"
#include <fstream>
#include <vector>
#include <iostream>

Map::Map(std::string& path){
	mEntities.resize(NUMBER);
	loadFromFile(path);
}

void Map::loadFromFile(std::string& path) {
	std::ifstream in(path);

	int bground;
	in >> bground;

	mBackground.setTexture(Game::get()->mTextures.get((Texture)bground));

	std::string obj;
	size_t		noObjects;

	in >> noObjects;

	for (size_t i = 0; i < noObjects; ++i) {
		in >> obj;
		insertObject(obj, in);
	}
	in.close();
}

void Map::insertObject(std::string& obj, std::ifstream& stream) {
	if (obj == "player1") {
		sf::Vector2f* position, *velocity;
		position = new sf::Vector2f;
		velocity = new sf::Vector2f;

		std::string tankType;
		float health, damage;

		stream >> tankType;
		stream >> position->x >> position->y;
		stream >> velocity->x >> velocity->y;
		stream >> health >> damage;

		insertPlayerOne(tankType, position, velocity, health, damage);
	}
	if (obj == "player2") {
		
		sf::Vector2f* position, *velocity;
		position = new sf::Vector2f;
		velocity = new sf::Vector2f;

		std::string tankType;
		float health, damage;

		stream >> tankType;
		stream >> position->x >> position->y;
		stream >> velocity->x >> velocity->y;
		stream >> health >> damage;

		insertPlayerTwo(tankType, position, velocity, health, damage);
	}

	if (obj == "static_obstacle") {
		std::string	textureStr;
		Texture texture;
		sf::Vector2f* position = new sf::Vector2f;
		
		stream >> textureStr;
		stream >> position->x >> position->y;
	
		insertStaticObject(textureStr, position);
	}
}

void Map::update(sf::Time dt) {

	for (auto it1 = mEntities.begin(); it1 != mEntities.end(); ++it1) {
		for (auto it2 = (*it1).begin(); it2 != (*it1).end(); ++it2) {
			(*it2)->update(dt);
		}
	}

	for (auto it = mEffects.begin(); it != mEffects.end(); ++it) {
		(*it)->update(dt);
	}

	for (auto it1 = mEntities.begin(); it1 != mEntities.end(); ++it1) {
		for (auto it2 = (*it1).begin(); it2 != (*it1).end(); ++it2) {
			if ((*it2)->mDelete == true) {
				Entity* temp = (*it2);
				(*it1).erase(it2);
				delete temp;
			}
		}
	}

	for (auto it = mEffects.begin(); it != mEffects.end(); ++it) {
		if ((*it)->bFinished == true) {
			Animation* temp = (*it);
			mEffects.erase(it);
			delete temp;
		}

	}
}

void Map::draw(sf::RenderWindow* window) {
	
	window->draw(mBackground);
	for (auto it1 = mEntities.begin(); it1 != mEntities.end(); ++it1) {
		for (auto it2 = (*it1).begin(); it2 != (*it1).end(); ++it2) {
			(*it2)->draw(window);
		}
	}

	for (auto it = mEffects.begin(); it != mEffects.end(); ++it) {
		(*it)->draw(window);
	}
}

void Map::insertPlayerOne(const std::string& tankType, sf::Vector2f* position, sf::Vector2f* velocity, float health, float damage) {
	std::vector<sf::Keyboard::Key> keys;
	keys.resize(20);

	keys[(int)Tank::Command::DOWN] = sf::Keyboard::S;
	keys[(int)Tank::Command::UP] = sf::Keyboard::W;
	keys[(int)Tank::Command::LEFT] = sf::Keyboard::A;
	keys[(int)Tank::Command::RIGHT] = sf::Keyboard::D;

	keys[(int)Tank::Command::ROT_LEFT] = sf::Keyboard::C;
	keys[(int)Tank::Command::ROT_RIGHT] = sf::Keyboard::V;

	keys[(int)Tank::Command::SHOOT] = sf::Keyboard::B;

	Texture baseTexture, topTexture;
	getTankTextureIds(tankType, baseTexture, topTexture);

	mEntities[TANK].insert(mEntities[TANK].begin(), new Player(
		new sf::Sprite(Game::get()->mTextures.get(baseTexture)),
		new sf::Sprite(Game::get()->mTextures.get(topTexture)),
		position,
		velocity,
		health,
		damage,
		keys));
}

void Map::insertPlayerTwo(const std::string& tankType, sf::Vector2f* position, sf::Vector2f* velocity, float health, float damage) {
	std::vector<sf::Keyboard::Key> keys;
	keys.resize(20);

	keys[(int)Tank::Command::DOWN] = sf::Keyboard::Numpad5;
	keys[(int)Tank::Command::UP] = sf::Keyboard::Numpad8;
	keys[(int)Tank::Command::LEFT] = sf::Keyboard::Numpad4;
	keys[(int)Tank::Command::RIGHT] = sf::Keyboard::Numpad6;

	keys[(int)Tank::Command::ROT_LEFT] = sf::Keyboard::Numpad7;
	keys[(int)Tank::Command::ROT_RIGHT] = sf::Keyboard::Numpad9;

	keys[(int)Tank::Command::SHOOT] = sf::Keyboard::P;

	Texture baseTexture, topTexture;
	getTankTextureIds(tankType, baseTexture, topTexture);

	mEntities[TANK].insert(mEntities[TANK].begin(), new Player(
		new sf::Sprite(Game::get()->mTextures.get(baseTexture)),
		new sf::Sprite(Game::get()->mTextures.get(topTexture)),
		position,
		velocity,
		health,
		damage,
		keys));
}

void Map::insertStaticObject(const std::string& textureString, sf::Vector2f* position) {

	Texture texture;
	getStaticObjectTextureId(textureString, texture);

	mEntities[OBSTACLE].insert(mEntities[OBSTACLE].begin(), new Obstacle(position, new sf::Sprite(Game::get()->mTextures.get(texture))));
}

void Map::getTankTextureIds(const std::string& textureString, Texture& baseTexture, Texture& topTexture) {
	if (textureString == "tank1") {
		baseTexture = Texture::tank1_body;
		topTexture = Texture::tank1_gun;
	}
	else if (textureString == "tank1b") {
		baseTexture = Texture::tank1b_body;
		topTexture = Texture::tank1b_gun;
	}
	else if (textureString == "tank2") {
		baseTexture = Texture::tank2_body;
		topTexture = Texture::tank2_gun;
	}
	else if (textureString == "tank2b") {
		baseTexture = Texture::tank2b_body;
		topTexture = Texture::tank2b_gun;
	}
	else if (textureString == "tank3") {
		baseTexture = Texture::tank3_body;
		topTexture = Texture::tank3_gun;
	}
	else if (textureString == "tank3b") {
		baseTexture = Texture::tank3b_body;
		topTexture = Texture::tank3b_gun;
	}
	else if (textureString == "tank3c") {
		baseTexture = Texture::tank3c_body;
		topTexture = Texture::tank3c_gun;
	}
	else if (textureString == "tank3d") {
		baseTexture = Texture::tank3d_body;
		topTexture = Texture::tank3d_gun;
	}
	else
		throw std::runtime_error("Map->getTankTextureIds: The specified texture doesn't exist");
}

void Map::getStaticObjectTextureId(const std::string& textureString, Texture& texture) {
	if (textureString == "box")
		texture = Texture::box;
	else if (textureString == "box_2x1")
		texture = Texture::box_2x1;
	else if (textureString == "box_2x2")
		texture = Texture::box_2x2;
	else if (textureString == "box_small")
		texture = Texture::box_small;
	else
		throw std::runtime_error("Map->getStaticObjectTextureId: The specified texture doesn't exist");
}