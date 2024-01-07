#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include "Player.h"
#include "Minion.h"
#include "Spell.h"
#include "Ritual.h"
#include "Enchantments.h"
#include "GameBoard.h"
#include "ascii_graphics.h"

using namespace std;

Player::Player() {}

Player::Player(const string name, int playerNumber) : name{name}, playerNumber{playerNumber} {}


string parseForNextPhrase(string delimiter, string& s) {
  size_t pos = 0;
  if ((pos = s.find(delimiter)) != std::string::npos) {
    string token = s.substr(0, pos);
    s.erase(0, pos + delimiter.length());
    return token;
  } else {
    return "";
  }
}

void Player::populateMapToCards() {
  ifstream file{"cards.data"};
  string line;
  while(getline(file, line)) {
    string name = parseForNextPhrase(":", line);
    string type = parseForNextPhrase(";", line);
    int cost = stoi(parseForNextPhrase(";", line));
    string description = parseForNextPhrase(";", line);
    if (type == "Minion") {
      int attack = stoi(parseForNextPhrase(";", line));
      int defence = stoi(parseForNextPhrase(";", line));
      mapToCards[name] = std::make_shared<Minion>(name, cost, description, attack, defence);
      mapToCards[name]->setOwner(playerNumber);
      string abilityCostStr = parseForNextPhrase(";", line);
      if (abilityCostStr != "") {
        int abilityCost = stoi(abilityCostStr);
        int triggerType = stoi(parseForNextPhrase(":", line));
        shared_ptr<Minion> minionPtr = std::dynamic_pointer_cast<Minion>(mapToCards[name]);
        minionPtr->setAbility(make_shared<Ability>(name, abilityCost, description, false), triggerType, playerNumber);
      }
      else if (description != "") {
        int triggerType = stoi(parseForNextPhrase(":", line));
        shared_ptr<Minion> minionPtr = std::dynamic_pointer_cast<Minion>(mapToCards[name]);
        minionPtr->setAbility(make_shared<Ability>(name, 0, description, true), triggerType, playerNumber);
      }

    } else if (type == "Spell") {
      mapToCards[name] = std::make_shared<Spell>(name, cost, description);
    } else if (type == "Ritual") {
      string tmp = parseForNextPhrase(";", line);
      int chargesLeft = stoi(parseForNextPhrase(";", line));
      int activationCost = stoi(parseForNextPhrase(";", line));
      int triggerType = stoi(parseForNextPhrase(":", line));
      mapToCards[name] = std::make_shared<Ritual>(name, cost, description, activationCost, chargesLeft, triggerType, playerNumber);
      mapToCards[name]->setOwner(playerNumber);
    } else if (type == "Enchantment") {
      if (name == "Giant Strength") {
        string field1 = parseForNextPhrase(";", line);
        string field2 = parseForNextPhrase(";", line);
        mapToCards[name] = std::make_shared<Enchantment>(name, cost, description, field1, field2);
      }
      else if (name == "Enrage") {
        string field1 = parseForNextPhrase(";", line);
        string field2 = parseForNextPhrase(";", line);
        mapToCards[name] = std::make_shared<Enchantment>(name, cost, description, field1, field2);
      }
      else if (name == "Haste") {
        mapToCards[name] = std::make_shared<Enchantment>(name, cost, description, "","");
      }
      else if (name == "Magic Fatigue") {
        mapToCards[name] = std::make_shared<Enchantment>(name, cost, description, "","");
      }
      else if (name == "Silence") {
        mapToCards[name] = std::make_shared<Enchantment>(name, cost, description, "","");
      }
    }
  }
}

void Player::loadDeck(string f) {
  ifstream file{f};
  if (file.fail()) {
    file.close();
    file.open("default.deck");
  }
  string name;
  while (getline(file, name)) {
    auto cardPtr = mapToCards[name]; 
    auto it = mapToCards.find(name);
    if (it != mapToCards.end()) {
      string type = cardPtr->getType();
      if (type == "Minion") {
        std::shared_ptr<Minion> minionPtr = std::dynamic_pointer_cast<Minion>(cardPtr);
        auto newMinion = std::make_shared<Minion>(minionPtr);
        deck.push_back(newMinion);
      }
      else {
        deck.push_back(cardPtr);
      }
    }
    else {
      cout << "There was an invalid card name in deck file!" << endl;
    }
  }
}

void Player::shuffleDeck() {
  // shuffle.cc code for randomization

	// use a time-based seed for the default seed value
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

	std::default_random_engine rng{seed};

	for ( int i = 0; i < 1000; i++ ) {
		std::shuffle( deck.begin(), deck.end(), rng );	
	}
}

void Player::drawCard() {
  if (deck.size() != 0) {
    shared_ptr<Card> newCard = deck.back();
    deck.pop_back();
    hand.push_back(newCard);
  }
}

string Player::getName() const {
	return name;
}

void Player::setName(string name) {
  this->name = name;
}

void Player::setPlayerNumber(int num) {
  this->playerNumber = num;
}

int Player::getPlayerNumber() {
  return playerNumber;
}

int Player::getLife() const {
	return life;
}

const vector<shared_ptr<Card>>& Player::getHand() const {
  return hand;
}

bool Player::decreaseLife(int amount) {
  if (life > amount) {
    life -= amount;
    return true;
  }
  else {
    life = 0;
    winner(name);
    return false;
  }
}
int Player::getMagic() const {
  return magic;
}
void Player::increaseLife(int amount) {
  life += amount;
}
void Player::increaseMagic(int amount) {
  magic += amount;
}
bool Player::decreaseMagic(int amount) {
	if (!isTesting && magic < amount) {
  	return false;
  } 
  magic -= amount;
  if(magic < 0) {
    magic = 0;
  }
  return true;	
}


bool Player::play(int i) {
  if (handSize() < i) {
	 	cout << "Invalid card number" << endl;
	 	return false;
 	}
  string type = hand.at(i-1)->getType();
  if (type == "Minion" && isMinionBoardFull()) {
    cout << "Unable to summon. Slots are full!" << endl;
    return false;
	}
  if (!isTesting && activePlayer->getMagic() < hand.at(i-1)->getCost()) {
		cout << "Insufficient magic." << endl;
		return false;
	}

	if (type == "Minion" || type == "Spell") {
    // play and remove from hand
		if (hand.at(i - 1)->play()) {
      activePlayer->decreaseMagic(hand.at(i-1)->getCost());
      hand.erase(hand.begin() + i - 1);
    }
	} 
  else if (type == "Ritual") {
    auto rit = std::dynamic_pointer_cast<Ritual>(hand.at(i - 1));
    ritual = make_shared<Ritual>(rit);    
    activePlayer->decreaseMagic(hand.at(i-1)->getCost());
    hand.erase(hand.begin() + i - 1);
  } 
  else if (type == "Enchantment") {
    cout << "Invalid command. Enchantment must have a target minion." << endl;
  }
  return true;
}

bool Player::play(int i, int p, int t) {
  if (handSize() < i) {
	 	cout << "Invalid card number" << endl;
	 	return false;
 	}
  if (p != 1 && p != 2) {
    cout << "Invalid player number" << endl;
    return false;
  }
  if ((p == 1) && (t != 6) && (t > player1.minionCount())) {
    cout << "Invalid target!" << endl;
    return false;
  }

  if ((p == 2) && (t != 6) && (t > player2.minionCount())) {
    cout << "Invalid target!" << endl;
    return false;
  }

  string type = hand.at(i-1)->getType();
  if (type=="Minion"){
 		cout << "Minions don't require targets." << endl;
 		return false;
	}
	if (!isTesting && activePlayer->getMagic() < hand.at(i-1)->getCost()) {
		cout << "Insufficient magic." << endl;
		return false;
	}

  if (type == "Spell") {
  auto spell = std::dynamic_pointer_cast<Spell>(hand.at(i - 1));
    if (spell) { 
      if (spell->play(p, t)) {
        activePlayer->decreaseMagic(hand.at(i-1)->getCost());
        hand.erase(hand.begin() + i - 1);
      } else {
        return false;
      }
    } else {
      return false;
    }
  }
  if (type == "Enchantment") {
    shared_ptr<Minion> m;
    if (t == 6) {
      cout << "Enchantments can't be played on rituals or non-minions." << endl;
      return false;
    }
    if (p == 1) m = player1.getMinion(t);
    else m = player2.getMinion(t);
    auto enchantment = std::dynamic_pointer_cast<Enchantment>(hand.at(i-1));
    m->attachEnchantment(enchantment);
    activePlayer->decreaseMagic(hand.at(i-1)->getCost());
    hand.erase(hand.begin() + i - 1);
  }
  return true;
}

bool Player::doRitual(int p, int t) {
  if (!ritual) {
    return false;
  } else if (!ritual->chargeForActivation()) {
    return false;
  }
  return ritual->activate(p, t);
} 
bool Player::doRitual() {
  if (!ritual) {
    return false;
  } else if (!ritual->chargeForActivation()) {
    return false;
  }
  return ritual->activate();
} 

void Player::summonMinion(std::shared_ptr<Minion> minion) {
  minion->setOwner(playerNumber);
  minions.push_back(minion);
  triggerP = playerNumber;
  triggerT = minionCount();
  activateTrigger(2);	
}

void Player::inspectMinion(int i) {
  if (minionCount() < i) {
    cout << "Invalid minion number." << endl;
  } else {
    vector<vector<string>> printInspect;
    vector<string> element;
    shared_ptr<Minion> m = this->getMinion(i);
    // read and display the minion card
    if (m->getTriggerType() == 0) element = display_minion_no_ability(m->getName(), m->getCost(), m->getAttack(), m->getDefence());
    else if (m->getTriggerType() >= 1 && m->getTriggerType() <= 4) element = display_minion_triggered_ability(m->getName(), m->getCost(), m->getAttack(), m->getDefence(), m->getDescription());
    else element = display_minion_activated_ability(m->getName(), m->getCost(), m->getAttack(), m->getDefence(), m->getAbilityCost(), m->getDescription());
    printInspect.emplace_back(element);
    for (int i = 0; i < int(element.size()); ++i) {
      for (int k = 0; k < 1; ++k) {
        cout << printInspect[k][i];
      }
      cout << endl;
    }
    printInspect.pop_back();

    // read enchantments of the minion
    for (int i = 0; i < int(m->getEnchantmentList().size()); i++) {
      shared_ptr<Enchantment> e = m->getEnchantment(i);
      if (e->getName() == "Giant Strength" || e->getName() == "Enrage") element = display_enchantment_attack_defence(e->getName(), e->getCost(), e->getDescription(), e->getAttMultiplier(), e->getDefMultiplier());
      else element = display_enchantment(e->getName(), e->getCost(), e->getDescription());
      printInspect.emplace_back(element);
    } 
    // display enchantment cards
    for (int k = 0; k < int(m->getEnchantmentList().size()); k += 5) {
      for (int j = 0; j < int(printInspect[0].size()); j++) {
          for (int i = k; i < k + 5 && i < int(m->getEnchantmentList().size()); i++) {
              cout << printInspect[i][j];
          }
          cout << endl;
      }
    }
  }
}

int Player::handSize(){
	return hand.size();
}

shared_ptr<Minion> Player::getMinion(int m) {
  if (m > minionCount()) {
    return nullptr;
  }
	return minions.at(m - 1);
}
int Player::getMinionIndex(shared_ptr<Minion> minion) {
  for (int i = 0; i < minionCount(); ++i) {
    if (minions[i] == minion) {
      return i+1;
    }
  }
  return -1;
}

int Player::minionCount() {
  return minions.size();
}

bool Player::isMinionBoardFull() {
  return minions.size() == 5;
}

void Player::moveToGraveyard() {
  for (size_t i = 0; i < minions.size(); ++i) {
    if (minions[i]->hasNoLife()) {
      graveyard.push_back(minions[i]);
      minions.erase(minions.begin() + i);
      break;
    }
  }
}

std::shared_ptr<Minion> Player::getTopGraveyard() {
  if (!graveyard.empty()) {
    std::shared_ptr<Minion> topMinion = graveyard.back();
    return topMinion;
  } else {
    return nullptr;
  }
}

shared_ptr<Minion> Player::popTopGraveyard() {
  if (!graveyard.empty()) {
    std::shared_ptr<Minion> topMinion = graveyard.back();
    graveyard.pop_back();
    return topMinion;
  } else {
    return nullptr;
  }
}

void Player::destroyMinion(int i) {
  minions.erase(minions.begin() + i);
}

shared_ptr<Ritual> Player::getRitual() {
  return ritual;
}

shared_ptr<Card> Player::getCardAtHand(int i) {
  return hand[i];
}

void Player::handPrint() {
  if (hand.size() == 0) {
    cout << "Hand is empty!" << endl;
  }
  vector<vector<string>> printedHand;
  int height;
  for (size_t i = 0; i < hand.size(); ++i) {
    vector<string> card;
    string name = hand[i]->getName();
    int cost = hand[i]->getCost();
    string description = hand[i]->getDescription();
    if (hand[i]->getType() == "Minion") {
      std::shared_ptr<Card> cardPtr = hand[i];
      std::shared_ptr<Minion> minionPtr = std::dynamic_pointer_cast<Minion>(cardPtr);
      Minion* minion = nullptr;
      if (minionPtr) {
        minion = minionPtr.get(); 
      }
      int att = minion->getAttack();
      int defence = minion->getDefence();
      card = display_minion_no_ability(name, cost, att, defence);
      if (minion->getTriggerType() == 0) card = display_minion_no_ability(name, cost, att, defence);
      else if (minion->getTriggerType() == 5) card = display_minion_activated_ability(name, cost, att, defence, minion->getAbilityCost(), minion->getDescription());
      else card = display_minion_triggered_ability(name, cost, att, defence, minion->getDescription());
    }
    else if (hand[i]->getType() == "Spell") {
      card = display_spell(name, cost, description);
    }
    else if (hand[i]->getType() == "Ritual") {
      std::shared_ptr<Card> cardPtr = hand[i];
      std::shared_ptr<Ritual> ritualPtr = std::dynamic_pointer_cast<Ritual>(cardPtr);
      card = printRitual(ritualPtr);
    }
    else if (hand[i]->getType() == "Enchantment") {
      if (name == "Giant Strength") {
        auto e = std::dynamic_pointer_cast<Enchantment>(hand.at(i));
        card = display_enchantment_attack_defence(name, cost, description, e->getAttMultiplier(), e->getDefMultiplier());
      }
      else if (name == "Enrage") {
        auto e = std::dynamic_pointer_cast<Enchantment>(hand.at(i));
        card = display_enchantment_attack_defence(name, cost, description, e->getAttMultiplier(), e->getDefMultiplier());
      }
      else if (name == "Haste" || name == "Magic Fatigue" || name == "Silence") {
        card = display_enchantment(name, cost, description);
      }
    }
    printedHand.emplace_back(card);
    height = card.size();
  }
  int width = hand.size();
  for (int i = 0; i < height; ++i) {
    for (int k = 0; k < width; ++k) {
      cout << printedHand[k][i];
    }
		cout << endl;
  }

}

bool Player::attack(int i) {
  if (i > minionCount()) {
		cout << "Invalid minion" << endl;
		return false;
	}
	shared_ptr<Minion> attacker = getMinion(i);
	return attacker->attack(inactivePlayer->getPlayerNumber());
}

bool Player::attack(int i, int j) {
  if (i > minionCount() || j > inactivePlayer->minionCount()) {
		cout << "Invalid minion" << endl;
		return false;
	}
	shared_ptr<Minion> attacker = getMinion(i);
  shared_ptr<Minion> target = inactivePlayer->getMinion(j);
	attacker->attack(target);
  return true;
}

void Player::resetActions() {
  for (int i = 1; i <= minionCount(); ++i) {
    getMinion(i)->restoreAction();
  }
}

void Player::returnToHand(int i) {
  if (hand.size() > 5) {
    minions.erase(minions.begin() + i - 1);
    cout << "Hand is full. Card removed." << endl;
  }

  shared_ptr<Minion> minionToReturn = minions[i - 1];
  hand.push_back(minionToReturn);
  minions.erase(minions.begin() + i - 1);
}

bool Player::useAbility(int minion) {
  shared_ptr<Minion> m = getMinion(minion);
  if(m->getSilenced()) {
    return false;
  }
  if (!isTesting && activePlayer->getMagic() < m->getAbilityCost()) {
    cout << "Insufficient magic!" << endl;
    return false;
  }
  if (m->useAbility()) {
    activePlayer->decreaseMagic(m->getAbilityCost());
    return true;
  }
  return false;
}

bool Player::useAbility(int minion, int p, int t) {
  shared_ptr<Minion> m = getMinion(minion);
  if(m->getSilenced()) {
    return false;
  }
  if (m->getTriggerType() == 5) {
    if (p != 1 && p != 2) {
      cout << "Invalid player number" << endl;
      return false;
    }
    if (p == 1 && t > player1.minionCount()) {
      cout << "Invalid target!" << endl;
      return false;
    }
    if (p == 2 && t > player2.minionCount()) {
      cout << "Invalid target!" << endl;
      return false;
    }
  }
  if (!isTesting && m->getTriggerType() == 5 && activePlayer->getMagic() < m->getAbilityCost()) {
    cout << "Insufficient magic!" << endl;
    return false;
  }
  if (m->useAbility(p, t)) {
    activePlayer->decreaseMagic(m->getAbilityCost());
    return true;
  }
  return false;
}

bool Player::discard(size_t i){
  if (hand.size() == 0) {
    cout << "No cards to discard!" << endl;
    return false;
  }
	if (i > hand.size()){
		cout << to_string(i) + " is not a valid card in your hand." << endl;
    return false;
	}
	else {
		hand.erase(hand.begin() + i - 1);
    return true;
	}
}
