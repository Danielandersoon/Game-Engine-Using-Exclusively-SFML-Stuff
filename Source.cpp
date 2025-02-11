
#include "InstanceManager.h"
#include "InputSystem.h"

int main() {

	InputSystem inSys;
	inSys.BindAction("left", sf::Keyboard::Left);
	inSys.BindAction("right", sf::Keyboard::Right);
	inSys.BindAction("jump", sf::Keyboard::Up);
	inSys.BindAction("duck", sf::Keyboard::Down);

	InstanceManager iManager = InstanceManager();

	bool exit = false;


	while (!exit) {
		
		inSys.Update();

	}

	return -1;
}