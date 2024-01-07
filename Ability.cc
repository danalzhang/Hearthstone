#include <string>
#include "Ability.h"

using namespace std;

Ability::Ability(string name, int abilityCost, string description, bool triggered) : triggered(triggered) {
  ability = std::make_shared<Spell>(name, abilityCost, description);
}

Ability::Ability(shared_ptr<Ability> other) {
  if (other->ability) {
    ability = make_shared<Spell>(other->ability);
  }
  triggered = other->triggered;
}

Ability& Ability::operator=(const Ability& other) {
  if (this != &other) {
    ability = other.ability;
    triggered = other.triggered;
  }
  return *this;
}

int Ability::getCost() {
  return ability->getCost();
}

void Ability::setCost(int val) {
  ability->setCost(val);
}

string Ability::getDescription() {
  return ability->getDescription();
}

bool Ability::useSpell() {
  return ability->play();
}
bool Ability::useSpell(int p, int t) {
  return ability->play(p, t);
}

string Ability::getName() {
  return ability->getName();
}
void Ability::setOwner(int owner) {
  ability->setOwner(owner);
}
