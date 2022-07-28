// Network Stuff
#include <thread>
#include <string>
#include "Network.hpp"
#include "BasicConnection.hpp"
#include "Array.hpp"
#include "Sendable.hpp"
#include "RequestConnection.hpp"

//#define SERVER
#define SENDING_TRANSMISSION


#include <iostream>
#include <functional>
#include "GameEngine.hpp"
#include "GraphicsEngine.hpp"
#include "InputRegistry.hpp"

#include "SpaceStyleControlledObject.hpp"
#include "SpaceStyleFixedCannon.hpp"
#include "CarStyleFixedCannon.hpp"
#include "CarControlledObject.hpp"
#include "ScreenRectangleTank.hpp"

#include "ScreenCircleTank.hpp"
#include "ScreenRectangle.hpp"
#include "GameObject.hpp"
#include "Util.hpp"
#include "ArtilleryGun.hpp"
#include "CircleTarget.hpp"
#include "RectangleTarget.hpp"
#include "GreaseCannon.hpp"

int main() {


	Network::InitializeNetwork();

	BasicConnection* connection;

#ifdef SERVER
	std::cout << "Waiting For Connection" << std::endl;
	connection = Network::WaitForConnection(AF_INET, SOCK_STREAM, IPPROTO::IPPROTO_TCP, AI_PASSIVE, std::string("50800"));
#else
	std::cout << "Requesting Connection" << std::endl;
	connection = Network::TransmissionConnection(AF_UNSPEC, SOCK_STREAM, IPPROTO::IPPROTO_TCP, 0, std::string("50801"), std::string("159.118.186.175"));
#endif

	RequestConnection* requestConnection = new RequestConnection(connection);





    GameEngine* gameEngine = new GameEngine();

	//RectangleTarget* target = new RectangleTarget();
	//target->getMotionObject()->setPosition(400, 300);
	//gameEngine->addObject(target);



	//CarStyleFixedCannon* car = new CarStyleFixedCannon();
	//car->getCarControlledObject()->registerKeys(gameEngine->getInputRegistry(), true);

	//gameEngine->addObject(car);
	//ArtilleryGun* gun = new ArtilleryGun(gameEngine, car);
	//gun->registerKeys(gameEngine->getInputRegistry(), VK_ENTER);



	SpaceStyleFixedCannon* space = new SpaceStyleFixedCannon();
	space->getSpaceStyleControlledObject()->registerKeys(gameEngine->getInputRegistry(), true);

	gameEngine->addObject(space);
	GreaseCannon* cannon = new GreaseCannon(gameEngine, space);
	cannon->registerKeys(gameEngine->getInputRegistry(), VK_SHIFT);


	//std::function<void(bool)>* grease = new std::function<void(bool)>();
	//*grease = [&car](bool pressed) {
	//	if (!pressed) {
	//		return;
	//	}

	//	car->getMotionObject()->startGrease(2000);
	//};
	//gameEngine->getInputRegistry()->addListener(grease, VK_G);


	//std::function<void(bool)>* reset = new std::function<void(bool)>();
	//*reset = [&car](bool pressed) {
	//	if (!pressed) {
	//		return;
	//	}

	//	car->getMotionObject()->getPosition()->setXY(600, 300);
	//};
	//gameEngine->getInputRegistry()->addListener(reset, VK_R);

	gameEngine->getGraphicsEngine()->createWindow(L"Shooter Game");

	MSG messages;
	while (GetMessage(&messages, NULL, 0, 0) > 0) {
		TranslateMessage(&messages);
		DispatchMessage(&messages);
		// Do Stuff
		gameEngine->getInputRegistry()->handleAllKeyInput();
		gameEngine->update();

#ifdef SENDING_TRANSMISSION

#else

#endif

	}
    
}
