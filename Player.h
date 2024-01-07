#ifndef PLAYER_H
#define PlAYER_H

#include <string>
#include <memory>
#include <map>
#include <vector>
#include "Card.h"
#include "Minion.h"
#include "Ability.h"
#include "Ritual.h"

using namespace std;

class Player {
    string name;
    int playerNumber;
    int life = 20;
    int magic = 3;
    map<string, shared_ptr<Card>> mapToCards;

    vector<shared_ptr<Card>> deck;
    vector<shared_ptr<Card>> hand;
    vector<shared_ptr<Minion>> minions;
    vector<shared_ptr<Minion>> graveyard;
    shared_ptr<Ritual> ritual = nullptr;

  public:
    Player();
    Player(const string name, int playerNumber);

    void populateMapToCards();
    void loadDeck(string f);
    void shuffleDeck();
    void drawCard();
    string getName() const;
    void setName(string name);
    void setPlayerNumber(int num);
    int getPlayerNumber();
    int getLife() const;
    int getMagic() const;
    const vector<shared_ptr<Card>>& getHand() const;
    void increaseLife(int amount = 1);
    bool decreaseLife(int amount = 1);
    void increaseMagic(int amount = 1);
    bool decreaseMagic(int amount = 1);
    int handSize();
    int minionCount();
    bool play(int i);
    bool play(int i, int p, int t);
    bool doRitual(int p, int t);
    bool doRitual();
    void summonMinion(std::shared_ptr<Minion> minion);
    void inspectMinion(int i);
    shared_ptr<Minion> getMinion(int i);
    int getMinionIndex(shared_ptr<Minion> minion);
    bool isMinionBoardFull();
    void moveToGraveyard();
    shared_ptr<Minion> getTopGraveyard();
    shared_ptr<Minion> popTopGraveyard();
    void destroyMinion(int i);
    shared_ptr<Ritual> getRitual();
    shared_ptr<Card> getCardAtHand(int i);
    void handPrint();
    bool attack(int i);
    bool attack(int i, int j);
    void resetActions();
    void returnToHand(int i);
    
    bool discard(size_t i);
    bool useAbility(int minion);
    bool useAbility(int minion, int p, int t);
};

#endif
