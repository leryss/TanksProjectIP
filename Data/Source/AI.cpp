#include "AI.h"
#include "Game.h"
#include "Map.h"
#include "Obstacle.h"
#include "VectorUtility.h"
#include <cstdlib>
#include <ctime>
#include <queue>
#include <iostream>
#include "GameState_Play.h"

AI::AI(sf::Sprite* base, sf::Sprite* top, sf::Vector2f* pos, sf::Vector2f* vel, float health, float damage, size_t team):
Tank(base, top, pos, vel, health, damage, team),
mTileLength(64.f),
mTarget(nullptr){

	srand(time(0));

	mWidth = (int)(Game::get()->mWidth / mTileLength);
	mHeight = (int)(Game::get()->mHeight / mTileLength);

	mNextPoint.x = -1.f;
	mNextPoint.y = -1.f;
}

void AI::initMap() {
	mPathFindMap = new int*[mHeight];
	for (int i = 0; i < mHeight; ++i) {
		mPathFindMap[i] = new int[mWidth]();
	}

	auto it1 = GameState_Play::getStatePointer()->mMap->mEntities[0].begin();
	auto it2 = GameState_Play::getStatePointer()->mMap->mEntities[0].end();
	for (; it1 != it2; ++it1) {

		sf::FloatRect bounds = (*it1)->mSprite->getGlobalBounds();

		int left = (int)(bounds.left / mTileLength);
		int top = (int)(bounds.top / mTileLength);
		int leftPlus = (int)((bounds.left + bounds.width) / mTileLength);
		int topPlus = (int)((bounds.top + bounds.height) / mTileLength);

		for (int i = left; i <= leftPlus; ++i) {
			mPathFindMap[top][i] = -1;
			mPathFindMap[topPlus][i] = -1;
		}		
		for (int i = top; i <= topPlus; ++i) {
			mPathFindMap[i][left] = -1;
			mPathFindMap[i][leftPlus] = -1;
		}

	}

	findNewTarget();

	

	float edgeX, edgeY;
	for (int i = 0; i < mHeight; ++i) {
		for (int j = 0; j < mWidth; ++j) {
			edgeY = i * mTileLength;
			edgeX = j * mTileLength;
			if (edgeX < 64.f || edgeY < 64.f || edgeX >= Game::get()->mWidth - 64.f || edgeY >= Game::get()->mHeight - 64.f)
				mPathFindMap[i][j] = -1;
		}
	}
}

void AI::calculateRotation() {

	if (mNextPoint.x > mBase->getPosition().x)
		mRotationAngle = 90.f;
	else if(mNextPoint.x < mBase->getPosition().x)
		mRotationAngle = 270.f;
	else if (mNextPoint.y > mBase->getPosition().y)
		mRotationAngle = 180.f;
	else if (mNextPoint.y < mBase->getPosition().y)
		mRotationAngle = 0.f;
}

void AI::assignNewPoint() {
	if (!mCurrentPath.empty()) {
		mNextPoint = mCurrentPath.top();
		mCurrentPath.pop();

		calculateRotation();
		mDistToNextPoint = Vector2f::distance(mNextPoint, mBase->getPosition());
	}
	else {
		calculatePathMap();
		calculateRandomPath();

		setPosition(mCurrentPath.top());
		mCurrentPath.pop();

		assignNewPoint();
	}
}

void AI::update(sf::Time dt) {

	Tank::update(dt);
}

void AI::calculatePathMap() {
	if (mPathFindMap == nullptr)
		initMap();
	else {
		for (int i = 0; i < mHeight; ++i) {
			for (int j = 0; j < mWidth; ++j) {
				if (mPathFindMap[i][j] > 0)
					mPathFindMap[i][j] = 0;
			}
		}
	}

	std::queue<LeePoint> que;
	LeePoint startPoint;
	LeePoint currentPoint;
	startPoint.x = (int)(mBase->getPosition().y / mTileLength);
	startPoint.y = (int)(mBase->getPosition().x / mTileLength);

	if(mPathFindMap[startPoint.x][startPoint.y] != -1)
		mPathFindMap[startPoint.x][startPoint.y] = 1;

	que.push(startPoint);
	while (!que.empty()) {
		currentPoint = que.front();
		que.pop();

		for (int i = 0; i < 4; ++i) {
			currentPoint.x += dx[i];
			currentPoint.y += dy[i];

			if (mPathFindMap[currentPoint.x][currentPoint.y] == 0) {
				currentPoint.step++;
				que.push(currentPoint);

				mPathFindMap[currentPoint.x][currentPoint.y] = currentPoint.step;
				currentPoint.step--;
			}

			currentPoint.x -= dx[i];
			currentPoint.y -= dy[i];
		}
	}
 
}

void AI::calculateRandomPath() {
	sf::Vector2f startPoint;
	sf::Vector2f toInsert;

	do {
		startPoint.x = (float)(rand() % mHeight);
		startPoint.y = (float)(rand() % mWidth);
	} while (mPathFindMap[(int)(startPoint.x)][(int)(startPoint.y)] <= 0);

	calculatePath(startPoint);
}

void AI::calculatePath(sf::Vector2f& startPoint) {

	sf::Vector2f toInsert;

	int cost = mPathFindMap[(int)(startPoint.x)][(int)(startPoint.y)];
	toInsert.y = (startPoint.x * mTileLength) + mTileLength / 2.f;
	toInsert.x = (startPoint.y * mTileLength) + mTileLength / 2.f;

	mCurrentPath.push(toInsert);

	while (cost > 0) {
		cost--;

		for (int i = 0; i < 4; ++i) {
			startPoint.x += dx[i];
			startPoint.y += dy[i];

			if (mPathFindMap[(int)(startPoint.x)][(int)(startPoint.y)] == cost) {
				toInsert.y = (startPoint.x * mTileLength) + mTileLength / 2.f;
				toInsert.x = (startPoint.y * mTileLength) + mTileLength / 2.f;
				mCurrentPath.push(toInsert);

				break;
			}

			startPoint.x -= dx[i];
			startPoint.y -= dy[i];
		}
	}
}

void AI::findNewTarget() {


	mTarget = nullptr;
	auto it = GameState_Play::getStatePointer()->mMap->mEntities[2].begin();
	for (; it != GameState_Play::getStatePointer()->mMap->mEntities[2].end(); ++it) {
		if (Tank* enemy = dynamic_cast<Tank*>(*it)) {

			if(enemy->mTeam != mTeam && enemy->mHealth > 0.f && rand() % 2)
				mTarget = enemy;
		}
	}
}

bool AI::isProjectilePathClear() {

	if (mIsProjectileHollow)
		return true;

	float xRot = sin(mTop->getRotation()*3.14f / 180.f);
	float yRot = -cos(mTop->getRotation()*3.14f / 180.f);
	float increment = 10.f;

	sf::Vector2f currentPos = mTop->getPosition();

	float boundWidth = Game::get()->mWidth - 64.f;
	float boundHeight = Game::get()->mHeight - 64.f;

	while (currentPos.x > 64.f && currentPos.x < boundWidth && currentPos.y > 64.f && currentPos.y < boundHeight) {

		auto it1 = GameState_Play::getStatePointer()->mMap->mEntities[0].begin();
		auto it2 = GameState_Play::getStatePointer()->mMap->mEntities[0].end();

		for (; it1 != it2; ++it1) {
			sf::FloatRect obstacleBound = (*it1)->mSprite->getGlobalBounds();

			if (currentPos.x > obstacleBound.left && currentPos.x < (obstacleBound.left + obstacleBound.width) &&
				currentPos.y > obstacleBound.top && currentPos.y < (obstacleBound.top + obstacleBound.height))
				return false;
		}

		currentPos.x += xRot*increment;
		currentPos.y += yRot*increment;
	}

	return true;

}

AI::~AI(){
	for (int i = 0; i < mHeight; ++i)
		delete[] mPathFindMap[i];
	mPathFindMap = nullptr;
}