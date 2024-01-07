#ifndef __RITUAL_H__
#define __RITUAL_H__

#include <string>
#include <memory>
#include <vector>
#include "Ability.h"

using namespace std;

class Ritual : public Card, public enable_shared_from_this<Ritual>{
  shared_ptr<Ability> ritual;
  int triggerType;
  int activationCost;
  int chargesLeft;


  public:
    Ritual(string name, int ritualCost, string description, int activationCost, int chargesLeft, int triggerType, int owner);
    Ritual(shared_ptr<Ritual> m);
    Ritual& operator=(const Ritual& other);

    bool chargeForActivation();
    bool activate();
    bool activate(int p, int t);
    int getTriggerType();
    void setOwner(int owner);
    void increaseCharge(int n);
    void banish();
    int getActivationCost();
    int getChargesLeft();  
    void setActivationCost(int val);
    void setChargesLeft(int val);
};

#endif
