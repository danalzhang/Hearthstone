#ifndef __SPELL_H__
#define __SPELL_H__

#include <memory>
#include "Card.h"

class Player;

using namespace std;

class Spell : public Card {

  public:
    Spell();

    Spell(string name, int cost, string description);
    Spell(std::shared_ptr<Spell> s);
    
    bool play() override;
    bool play(int p, int t);
};

#endif
