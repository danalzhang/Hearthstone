#include <string>
#include <iostream>
#include "Ritual.h"

using namespace std;

Ritual::Ritual(string name, int ritualCost, string description, int activationCost, int chargesLeft, int triggerType, int owner) : Card(name, "Ritual", ritualCost, description), triggerType(triggerType) {
  ritual = std::make_shared<Ability>(name, ritualCost, description, true);
  this->activationCost = activationCost;
  this->chargesLeft = chargesLeft;
  ritual->setOwner(owner);
}

Ritual::Ritual(std::shared_ptr<Ritual> m) : Card(m->name, "Ritual", m->cost, m->description), triggerType(m->triggerType) {
  ritual = m->ritual;
  this->activationCost = m->activationCost;
  this->chargesLeft = m->chargesLeft;
}

Ritual& Ritual::operator=(const Ritual& other) {
    if (this != &other) {
        ritual = other.ritual;
        activationCost = activationCost;
        chargesLeft = other.chargesLeft;
    }
    return *this;
}

bool Ritual::chargeForActivation() {
  if (chargesLeft < activationCost) {
    return false;
  }
  chargesLeft -= activationCost;
  return true;
}

bool Ritual::activate() {
  return ritual->useSpell();
}

bool Ritual::activate(int p, int t) {
  return ritual->useSpell(p, t);
}

void Ritual::setOwner(int owner) {
  ritual->setOwner(owner);
}

int Ritual::getTriggerType() {
  return triggerType;
}

void Ritual::increaseCharge(int n) {
  chargesLeft += n;
}

void Ritual::banish() {
  chargesLeft = 0;
}

int Ritual::getActivationCost() {
  return activationCost;
}
int Ritual::getChargesLeft() {
  return chargesLeft;
}
void Ritual::setActivationCost(int val) {
  activationCost = val;
}
void Ritual::setChargesLeft(int val) {
  chargesLeft = val;
}

