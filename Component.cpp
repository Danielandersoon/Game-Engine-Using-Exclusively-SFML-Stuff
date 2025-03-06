#include "./Component.h"
#include "./GameObject.h"

void GUESS::core::Component::setOwner(GameObject* newOwner) { owner = newOwner->getGUID(); }

