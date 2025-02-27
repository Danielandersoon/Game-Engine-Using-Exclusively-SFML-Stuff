#include "./InstanceManager.h"

int main() {

    GUESS::core::InstanceManager instanceManager;
    auto sceneManager = instanceManager.getInstanceByID(0)->getSceneManager();

	return -1;
}