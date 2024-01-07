#ifndef _ABILITY_H_
#define _ABILITY_H_

#include <string>
#include <memory>
#include <vector>
#include "Spell.h"

using namespace std;

class Ability {
  shared_ptr<Spell> ability;
  bool triggered;

  public:
    Ability(string name, int abilityCost, string description, bool triggered);
    Ability(shared_ptr<Ability> other);  // Copy constructor
    Ability& operator=(const Ability& other);
    int getCost();
    void setCost(int val);
    string getDescription();
    bool useSpell();
    bool useSpell(int p, int t);
    string getName();
    void setOwner(int owner);
};

#endif
