#ifndef ENCHANTMENTS_H
#define ENCHANTMENTS_H

#include <string>
#include <memory>
#include "Card.h"

using namespace std;

class Enchantment: public Card, public enable_shared_from_this<Enchantment> {
    string attMultiplier;
    string defMultiplier;
    public:
        Enchantment(string name, int cost, string description, string field1, string field2);
        Enchantment(shared_ptr<Enchantment> e);
        string getAttMultiplier();
        string getDefMultiplier();
};

#endif
