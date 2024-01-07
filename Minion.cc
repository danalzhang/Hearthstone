#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "Minion.h"
#include "Player.h"
#include "GameBoard.h"

using namespace std;

Minion::Minion(){}
Minion::Minion(string name, int cost, string description, int att, int def) : Card(name, "Minion", cost, description), att(att), def(def), defaultAtt(att), defaultDef(def), actions{0} {}

Minion::Minion(std::shared_ptr<Minion> m) 
  : Card(m->name, "Minion", m->cost, m->description), 
    att(m->att),
    def(m->def),
    defaultAtt(m->defaultAtt),
    defaultDef(m->defaultDef),
    actions(m->actions),
    triggerType(m->triggerType) {
  if (m->ability) {
    ability = make_shared<Ability>(m->ability);
    owner = m->getOwner();
    if (ability) {
      ability->setOwner(owner);
    }
  }
}

bool Minion::play() {
  shared_ptr<Minion> self = shared_from_this();
  activePlayer->summonMinion(self);
  return true;
}

bool Minion::attack(shared_ptr<Minion> m) {
  if (hasActions()) {
    useAction();
    m->decreaseLife(att);
    this->decreaseLife(m->getAttack());
    return true;
  } else {
    cout << "Minion does not have any actions left!" << endl;
    return false;
  }
}

bool Minion::attack(int playerNumber) {
  if (hasActions()) {
    useAction();
    if (playerNumber == 1) {
      player1.decreaseLife(att);
    } else {
      player2.decreaseLife(att);
    }
    return true;
  } else {
    cout << "Minion does not have any actions left!" << endl;
    return false;
  }
}

void Minion::restoreAction() { 
  actions = actionsPerTurn; 
}
void Minion::useAction() { 
  actions--; 
}
bool Minion::hasActions() { 
  return actions > 0; 
}
int Minion::getActions() {
  return actions;
}

int Minion::getDefaultAtt() {
  return defaultAtt;
}
int Minion::getDefaultDef() {
  return defaultDef;
}

void Minion::revive(int def) {
  this->def = def;
}

bool Minion::hasNoLife() {
  return def <= 0;
}

bool Minion::decreaseLife(int amount) {
  def -= amount;
  if (hasNoLife()) {
    if (owner == 1) {
      player1.moveToGraveyard();
    }
    else {
      player2.moveToGraveyard();
    } 
    activateTrigger(3);	
    return false;
  }
  return true;
}

void Minion::increaseLife(int amount) {
  def += amount;
}
void Minion::decreaseAttack(int amount) {
  att -= amount;
}
void Minion::increaseAttack(int amount) {
  att += amount;
}
int Minion::getDefence() {
  return def;
}
int Minion::getAttack() {
  return att;
}

vector<shared_ptr<Enchantment>> Minion::getEnchantmentList() {
  return enchantmentList;
}

shared_ptr<Enchantment> Minion::getEnchantment(int i) {
  return enchantmentList[i];
}

void Minion::removeTopEnchantment() {
  if (enchantmentList.size() > 0) {
    shared_ptr<Enchantment> lastEnchant = enchantmentList.back();
    enchantmentList.pop_back();
    if (lastEnchant->getName() == "Giant Strength") {
      decreaseAttack(lastEnchant->getAttMultiplier()[1] - '0');
      decreaseLife(lastEnchant->getDefMultiplier()[1] - '0');
    }
    else if (lastEnchant->getName() == "Enrage") {
      att /= (lastEnchant->getAttMultiplier()[1] - '0');
      def /= (lastEnchant->getAttMultiplier()[1] - '0');
    }
    else if (lastEnchant->getName() == "Haste") {
      actions -= 1;
      actionsPerTurn -= 1;
    }
    else if (lastEnchant->getName() == "Magic Fatigue") {
      if (triggerType == 5) {
        setAbilityCost(getAbilityCost() - 2);
      }
    }
    else if (lastEnchant->getName() == "Silence") {
      setSilenced(false);
    }
  }
}

void Minion::attachEnchantment(shared_ptr<Enchantment> e) {
  enchantmentList.emplace_back(e);
  if (e->getName() == "Giant Strength") {
    increaseAttack(e->getAttMultiplier()[1] - '0');
    increaseLife(e->getDefMultiplier()[1] - '0');
  }
  else if (e->getName() == "Enrage") {
    att *= (e->getAttMultiplier()[1] - '0');
    def *= (e->getAttMultiplier()[1] - '0');
  }
  else if (e->getName() == "Haste") {
    actions += 1;
    actionsPerTurn += 1;
  }
  else if (e->getName() == "Magic Fatigue") {
    if (triggerType == 5) {
      setAbilityCost(getAbilityCost() + 2);
    }
  }
  else if (e->getName() == "Silence") {
    setSilenced(true);
  }
}

void Minion::setAbility(shared_ptr<Ability> newAbility, int trigger, int owner) {
  ability = newAbility;
  triggerType = trigger;
  ability->setOwner(owner);
}

bool Minion::useAbility() {
  if(this->getSilenced()) {
    return false;
  }
  if (ability) {
    if (ability->useSpell()) {
      return true;
    }
  }
  return false;
}

bool Minion::useAbility(int p, int t) {
  if(this->getSilenced()) {
    return false;
  }
  if (ability) {
    if (ability->useSpell(p, t)) {
      return true;
    }
  }
  return false;
}

int Minion::getTriggerType() {
  return triggerType;
}
int Minion::getAbilityCost() {
  return ability->getCost();
}

void Minion::setAbilityCost(int val) {
  ability->setCost(val);
}

bool Minion::getSilenced() {
  return silenced;
}

void Minion::setSilenced(bool val) {
  silenced = val;
}

bool Minion::isJustPlaced() {
  return justPlaced;
}

void Minion::notJustPlaced() {
  justPlaced = false;
}
