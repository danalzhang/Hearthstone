#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "Spell.h"
#include "Player.h"
#include "Card.h"
#include "GameBoard.h"
#include "Minion.h"

using namespace std;

Spell::Spell(){}
Spell::Spell(string name, int cost, string description) : Card(name, "Spell", cost, description) {}
Spell::Spell(std::shared_ptr<Spell> s) : Card(s->name, "Spell", s->cost, s->description) {}

bool Spell::play() {
  string spell = this->getName();
  if (spell == "Blizzard") {
    int counter = 1;
    while (counter <= 5) {
      shared_ptr<Minion> currMinion1 = activePlayer->getMinion(counter);
      if (currMinion1 != nullptr) {
        if (currMinion1->decreaseLife(2)) {
          ++counter;
        }
      } else {
        ++counter;
      }
    }
    counter = 1;
    while (counter <= 5) {
      shared_ptr<Minion> currMinion2 = inactivePlayer->getMinion(counter);
      if (currMinion2 != nullptr) {
        if(currMinion2->decreaseLife(2)) {
          ++counter;
        }
      } else {
        ++counter;
      }
    }
    cout << "Blizzard!" << endl;
    return true;
  }
  else if (spell == "Raise Dead") {
    if(activePlayer->isMinionBoardFull()) {
      cout << "Minion board is full! Raise Dead was not played" << endl;
      return false;
    }
    shared_ptr<Minion> undead = activePlayer->popTopGraveyard();
    if (undead != nullptr) {
      undead->revive();
      activePlayer->summonMinion(undead);
      cout << "A minion has risen from the dead!" << endl;
      return true;
    }
    else {
      cout << "No minions in graveyard! Raise Dead was not played." << endl;
      return false;
    }
  }
  else if (spell == "Recharge") {
    shared_ptr<Ritual> ritual = activePlayer->getRitual();
    if (ritual) {
      ritual->increaseCharge(3);
      return true;
    }
    else {
      cout << "You don't have a ritual in play!" << endl;
      return false;
    }
  }
  else if (spell == "Apprentice Summoner") {
    if (activePlayer->isMinionBoardFull()) {
      cout << "Too many minions on the board!" << endl;
      return false;
    }
    activePlayer->summonMinion(make_shared<Minion>("Air Elemental", 0, "", 1, 1));
    return true;
  }
  else if (spell == "Master Summoner") {
    if (activePlayer->isMinionBoardFull()) {
      cout << "Too many minions on the board!" << endl;
      return false;
    }
    int counter = 0;
    for (int i = activePlayer->minionCount(); i <= 5; ++i) {
      activePlayer->summonMinion(make_shared<Minion>("Air Elemental", 0, "", 1, 1));
      counter++;
      if (counter == 3) {
        break;
      }
    }
    return true;
  }
  else if (spell == "Potion Seller") {
    int counter = 1;
    while (counter <= 5) {
      shared_ptr<Minion> currMinion;
      if (this->getOwner() == 1) {
        currMinion = player1.getMinion(counter);
      } else {
        currMinion = player2.getMinion(counter);
      }
      if (currMinion != nullptr) {
        currMinion->increaseLife(1);
      } else {
        break;
      }
      ++counter; 
    }
    return true;
  } else if (spell == "Dark Ritual") {
    if (this->owner == 1) {
      player1.increaseMagic(1);
    }
    else if (this->owner == 2){
      player2.increaseMagic(1);
    }
    return true;
  } 
  cout << "Spell or ability requires a target!" << endl;
  return false;

}

bool Spell::play(int p, int t) {
  string spell = this->getName();
  if (spell == "Banish") {
    if (p == 1) {
      if (t == 6) {
        shared_ptr<Ritual> ritual = player1.getRitual();
        if(ritual) {
          ritual->banish();
          return true;
        }
        else {
          cout << "Player 1 does not have a ritual." << endl;
          return false;
        }
      }
      else {
        player1.getMinion(t)->decreaseLife(player1.getMinion(t)->getDefence());
        return true;
      }
    }
    else {
      if (t == 6) {
        shared_ptr<Ritual> ritual = player2.getRitual();
        if(ritual) {
          ritual->banish();
          return true;
        }
        else {
          cout << "Player 2 does not have a ritual." << endl;
          return false;
        }
      }
      else {
        player2.getMinion(t)->decreaseLife(player2.getMinion(t)->getDefence());
        return true;
      }
    }
    return true;
  }
  else if (spell == "Unsummon") {
    if (t == 6) {
      cout << "Rituals cannot be unsummoned." << endl;
      return false;
    }
    if (p == 1) {
      player1.returnToHand(t);
    }
    else {
      player2.returnToHand(t);
    }
    return true;
  }
  else if (spell == "Disenchant") {
    if (t == 6) {
      cout << "Rituals cannot be disenchanted." << endl;
      return false;
    }
    if (p == 1) {
      player1.getMinion(t)->removeTopEnchantment();
    }
    else if (p == 2) {
      player2.getMinion(t)->removeTopEnchantment();
    }
    return true;
  }
  else if(spell == "Novice Pyromancer") {
    if (t == 6) {
      cout << "Rituals cannot be attacked." << endl;
      return false;
    }
    if (p == 1) {
      player1.getMinion(t)->decreaseLife(1);
    }
    else {
      player2.getMinion(t)->decreaseLife(1);
    }
    return true;
  }
  else if (spell == "Bone Golem") {
    if (p == 1) {
      player1.getMinion(t)->increaseLife(1);
      player1.getMinion(t)->increaseAttack(1);
    }
    else {
      player2.getMinion(t)->increaseLife(1);
      player2.getMinion(t)->increaseAttack(1);
    }
    return true;
  }
  else if (spell == "Fire Elemental") {
    if (this->getOwner() == 1) {
      shared_ptr<Minion> temp = player2.getMinion(t);
      if (temp) {
        temp->decreaseLife(1);
      }
    }
    else if (this->getOwner() == 2) {
      shared_ptr<Minion> temp = player1.getMinion(t);
      if (temp) {
        temp->decreaseLife(1);
      }
    }
    return true;
  }


  else if (spell == "Aura of Power") {
    if (this->getOwner() == 1 && p == 1) {
      player1.getMinion(t)->increaseAttack(1);
      player1.getMinion(t)->increaseLife(1);
    }
    else if (this->getOwner() == 2 && p == 2) {
      player2.getMinion(t)->increaseAttack(1);
      player2.getMinion(t)->increaseLife(1);
    }
    return true;
  }
  else if (spell == "Standstill") {
    if (p == 1) {
      player1.getMinion(t)->decreaseLife(player1.getMinion(t)->getDefence());
    }
    else if (p == 2) {
      player2.getMinion(t)->decreaseLife(player2.getMinion(t)->getDefence());
    }
    return true;
  }
  cout << "Spell or ability doesn't require a target!" << endl;
  return false; 
}
