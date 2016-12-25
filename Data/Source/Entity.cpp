#include "Entity.h"
#include "Game.h"

void Entity::draw(sf::RenderWindow* window) {
	window->draw(*mSprite);
	mDelete = false;
}

sf::Sprite*	Entity::getSprite() {
	return mSprite;
}

Entity::Entity(sf::Vector2f* pos, sf::Sprite* sprite)
: mSprite(sprite)
{
	mSprite->setPosition(*pos);
}

Entity::~Entity() {
	delete mSprite;
}