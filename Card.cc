#include "Card.h"

using namespace std;

Card::Card() : name{""}, type{""}, cost{0}, description{""} {
    this->owner = 0;
}

Card::Card(string name, string type, int cost, string description) : name{name}, type{type}, cost{cost}, description{description} {}

Card::Card(const Card &o) : name{o.name}, type{o.type}, cost{o.cost}, description{o.description} {
    this->owner = o.owner;
}
Card &Card::operator=(const Card &o) {
    if (this != &o) {
        name = o.name;
        type = o.type;
        cost = o.cost;
        description = o.description;
        owner = o.owner;
    }
    return *this;
}

void Card::setOwner(int playerNumber) {
    owner = playerNumber;
}

int Card::getOwner() {
    return owner;
}

string Card::getName() const {
    return name;
}

string Card::getDescription() const {
    return description;
}

string Card::getType() const {
    return type;
}

int Card::getCost() const {
    return cost;
}

void Card::setCost(int val) {
    cost = val;
}

bool Card::play() {return true;}
