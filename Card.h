#ifndef _CARD_H_
#define _CARD_H_

#include <string>

using namespace std;

class Card {
  protected:
    string name;
    string type;
    int cost;
    string description;
    int owner;

  public:
    Card();
    Card(string name, string type, int cost, string description);
    Card(const Card &o);

    Card &operator=(const Card &o);
    void setOwner(int playerNumber);
    int getOwner();
    string getName() const;
    string getDescription() const;
    string getType() const;
    int getCost() const;
    void setCost(int val);

    virtual bool play();
    
};

#endif
