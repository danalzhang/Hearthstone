#ifndef MINION_H
#define MINION_H

#include <vector>
#include <memory>
#include "Card.h"
#include "Enchantments.h"
#include "Ability.h"

using namespace std;

class Minion : public Card, public enable_shared_from_this<Minion> {
  int att;
  int def;
  int defaultAtt;
  int defaultDef;
  int actions;
  int actionsPerTurn = 1;
  bool silenced = false;

  int triggerType = 0;

  shared_ptr<Ability> ability;
  vector <shared_ptr<Enchantment>> enchantmentList;
  

  bool justPlaced = true; // for fire elemental (minion enters effect)

  public:
    Minion();
    Minion(string name, int cost, string description, int attack, int defence);
    Minion(shared_ptr<Minion> m);
    
    bool play() override;
    bool attack(shared_ptr<Minion> m);
    bool attack(int playerNumber);

    void restoreAction();
    void useAction();
    bool hasActions();
    bool hasNoLife();

    virtual int getDefence();
    virtual int getAttack();
    virtual int getActions();

    int getDefaultAtt();
    int getDefaultDef();

    void revive(int def = 1);
    bool decreaseLife(int amount = 1); // return false if minion dies
    void increaseLife(int amount = 1);
    void decreaseAttack(int amount = 1);
    void increaseAttack(int amount = 1);
    void setAbility(shared_ptr<Ability> newAbility, int triggerType, int owner);
    bool useAbility();
    bool useAbility(int p, int t);
    int getTriggerType();
    int getAbilityCost();
    void setAbilityCost(int val);
    bool getSilenced();
    void setSilenced(bool val);
    vector<shared_ptr<Enchantment>> getEnchantmentList();
    shared_ptr<Enchantment> getEnchantment(int i);
    void removeTopEnchantment();
    void attachEnchantment(shared_ptr<Enchantment> e);
   
    bool isJustPlaced();
    void notJustPlaced();

};


#endif
