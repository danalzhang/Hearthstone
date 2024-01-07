#ifndef _GAMEBOARD_H_
#define _GAMEBOARD_H_
#include <string>
#include <vector>

using std::string;

class Player;
class Card;

extern Player *activePlayer;
extern Player *inactivePlayer;
extern Player player1;
extern Player player2;
extern int triggerP;
extern int triggerT;
extern bool isTesting;

void help();
void boardPrint();
void endTurn();
extern void activateTrigger(int trigger);
extern vector<string> printMinion(shared_ptr<Card> card);
extern vector<string> printRitual(shared_ptr<Ritual> card);
extern void winner(string name);
extern void endGame();
#endif
