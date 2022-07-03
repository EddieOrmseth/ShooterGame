#pragma once

class MotionObject;
class ScreenObject;

class GameObject {

public:

	enum class ObjectType {
		Player,
		Projectile
	};

	GameObject(MotionObject* motionObject, ScreenObject* screenObject);
	~GameObject();

	MotionObject* getMotionObject();
	ScreenObject* getScreenObject();

	virtual bool shouldBeRemoved();
	virtual void notifyCollision(GameObject::ObjectType objectType, GameObject* object);

	ObjectType getObjectType();

	// Functions For Object Type
	virtual bool isGreasable();

protected:

	MotionObject* motionObject;
	ScreenObject* screenObject;

	ObjectType objectType;

};
