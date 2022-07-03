#include "ArtilleryGun.hpp"
#include "BulletObject.hpp"

ArtilleryGun::ArtilleryGun(GameEngine* gameEngine, ShootCapable* shootCapable) : StandardCannon(gameEngine, shootCapable) {

}

GameObject* ArtilleryGun::getProjectile(Vector2D* position, double angle, Vector2D* addedVel) {
	return new BulletObject(position, angle, addedVel);
}
