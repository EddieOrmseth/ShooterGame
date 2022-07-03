#pragma once

class AfterburnCapable {

public:

	enum class AfterburnStage {
		Off,
		On,
		Cooldown
	};

	virtual void startAfterburn() = 0;
	virtual void cooldownAfterburn() = 0;
	virtual void endAfterburn() = 0;

	virtual void setMaxVelocity() = 0;
	virtual void setAcceleration() = 0;

private:

	AfterburnStage afterburnStage;

};
