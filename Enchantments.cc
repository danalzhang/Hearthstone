#include "Enchantments.h"
#include <iostream>

using namespace std;

Enchantment::Enchantment(string name, int cost, string description, string field1, string field2) : Card(name, "Enchantment", cost, description), attMultiplier(field1), defMultiplier(field2) {}

string Enchantment::getAttMultiplier() {
    return attMultiplier;
}

string Enchantment::getDefMultiplier() {
    return defMultiplier;
}
