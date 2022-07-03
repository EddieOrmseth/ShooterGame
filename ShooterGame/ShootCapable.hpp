#pragma once

class Vector2D;

class ShootCapable {

public:

	virtual Vector2D* getShotPosition() = 0;
	virtual double getShotAngle() = 0;
	virtual Vector2D* getShotAddedVelocity() = 0;
	virtual bool canShoot();

};
