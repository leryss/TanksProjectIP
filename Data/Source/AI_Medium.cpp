#include "AI_Medium.h"

AI_Medium::AI_Medium(sf::Sprite* base, sf::Sprite* top, sf::Vector2f* pos, sf::Vector2f* vel, float health, float damage, size_t team) :
	AI(base, top, pos, vel, health, damage, team) {

}

void AI_Medium::update(sf::Time dt) {
	if (mPathFindMap == nullptr) {
		assignNewPoint();
	}

	float distToNext = sqrt((mBase->getPosition().x - mNextPoint.x)*(mBase->getPosition().x - mNextPoint.x) + (mBase->getPosition().y - mNextPoint.y)*(mBase->getPosition().y - mNextPoint.y));
	if (distToNext < 5.f) {
		setPosition(mNextPoint);
		assignNewPoint();
	}
	else {

		float currentRotation = mBase->getRotation();

		if (currentRotation == mRotationAngle) {

			if (!MoveX(dt.asSeconds())) {
				std::stack<sf::Vector2f> clear;
				mCurrentPath = clear;

				assignNewPoint();
			}
		}
		else {
			if (mRotationAngle == 0.f) {
				if (currentRotation >= 270.f)
					mBase->setRotation(currentRotation + 6.f);
				else
					mBase->setRotation(currentRotation - 6.f);
			}
			else {
				if (currentRotation > mRotationAngle)
					mBase->setRotation(currentRotation - 6.f);
				else
					mBase->setRotation(currentRotation + 6.f);
			}
		}

		if (mTarget == nullptr) {
			findNewTarget();
			return;
		}

		sf::Vector2f turretProjection(mBase->getPosition());
		sf::Vector2f playerPosition(mTarget->mBase->getPosition());

		float distanceToPlayer = sqrt((turretProjection.x - playerPosition.x)*(turretProjection.x - playerPosition.x) + (turretProjection.y - playerPosition.y)*(turretProjection.y - playerPosition.y));

		turretProjection.x += sin(mTop->getRotation()*3.14f / 180.f)* distanceToPlayer;
		turretProjection.y -= cos(mTop->getRotation()*3.14f / 180.f)* distanceToPlayer;

		if (abs(turretProjection.y - playerPosition.y) > 5.f && abs(turretProjection.x - playerPosition.x) > 5.f) {

			float distProjPlayer = sqrt((turretProjection.x - playerPosition.x)*(turretProjection.x - playerPosition.x) + (turretProjection.y - playerPosition.y)*(turretProjection.y - playerPosition.y));
			float angle = acos(1.f - distProjPlayer / (2 * distanceToPlayer));

			if (playerPosition.x < mBase->getPosition().x) {
				if (360.f - angle > 180.f)
					if (turretProjection.y > playerPosition.y)
						rotateTurret(dt.asSeconds());
					else
						rotateTurret(-dt.asSeconds());
				else {
					if (turretProjection.y > playerPosition.y)
						rotateTurret(-dt.asSeconds());
					else
						rotateTurret(dt.asSeconds());
				}
			}
			else {
				if (360.f - angle > 180.f)
					if (turretProjection.y > playerPosition.y)
						rotateTurret(-dt.asSeconds());
					else
						rotateTurret(dt.asSeconds());
				else {
					if (turretProjection.y > playerPosition.y)
						rotateTurret(dt.asSeconds());
					else
						rotateTurret(-dt.asSeconds());
				}
			}


		}
		else
			shoot();

	}

	AI::update(dt);
}