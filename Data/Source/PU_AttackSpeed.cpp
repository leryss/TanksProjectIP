#include "PU_AttackSpeed.h"

PU_AttackSpeed::PU_AttackSpeed(sf::Vector2f* position, sf::Sprite* sprite) :
	PowerUp(position, sprite),
	mAttackSpeedMultiplier(5.f){

}

void PU_AttackSpeed::update(sf::Time dt) {
	PowerUp::update(dt);
}

void PU_AttackSpeed::onTrigger(Tank* target) {

	PowerUp::onTrigger(target);

	target->mHitCooldown /= mAttackSpeedMultiplier;
	target->mHitCooldownClock.restart();

	checkForDuplicate();
}

void PU_AttackSpeed::onDurationEnd() {

	PowerUp::onDurationEnd();
	mTarget->mHitCooldown *= mAttackSpeedMultiplier;
	mTarget->mHitCooldownClock.restart();
}

void PU_AttackSpeed::checkForDuplicate() {
	bool found = false;
	auto it = mTarget->mPowerUpList.begin();

	for (; it != mTarget->mPowerUpList.end(); ++it) {
		if (PU_AttackSpeed* obj = dynamic_cast<PU_AttackSpeed*>((*it))) {
			obj->mDurationClock->restart();
			found = true;

			onDurationEnd();
		}
	}

	if (!found)
		mTarget->mPowerUpList.insert(mTarget->mPowerUpList.begin(), this);
}