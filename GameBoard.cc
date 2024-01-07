#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "ascii_graphics.h"
#include "GraphicsObserver.h"
#include "GameBoard.h"

using namespace std;

Player *activePlayer;
Player *inactivePlayer;
Player player1;
Player player2;
bool firstRound = true;
int triggerP = 0;
int triggerT = 0;
bool gameOver = false;
bool isTesting;

int main(int argc, char* argv[]) {
  unique_ptr<GraphicsObserver> GO = nullptr;
  string name, initName;
  bool init = false;
  bool isDisplayingGraphics = false;
  isTesting = false;
  ifstream initFile;
  string deck1 = "default.deck";
  string deck2 = "default.deck";

  // initialize active/inactive players
  activePlayer = &player1;
  inactivePlayer = &player2;

  for (int i = 1; i < argc; ++i) {	
		string arg = argv[i];
		if (arg == "-init") {
			++i;
			init = true;
			arg = argv[i];
			initName = arg;
		} else if (arg == "-testing") {
			isTesting = true;
		} else if (arg == "-graphics") {
      GO = std::make_unique<GraphicsObserver>();
      isDisplayingGraphics = true;
    } else if (arg == "-deck1") {
			++i;
			arg = argv[i];
			deck1 = arg;
		} else if (arg == "-deck2") {
			++i;
			arg = argv[i];
			deck2 = arg;
    }
	}

  if (init) {
    initFile.open(initName);
		getline(initFile, name);
		player1.setName(name);
		getline(initFile, name);
		player2.setName(name);
	} else {
    cout << "Enter first player's name: ";
		getline(cin, name);
		player1.setName(name);
		cout << "Enter second player's name: ";
		getline(cin, name);
		player2.setName(name);
  }
  player1.setPlayerNumber(1);
  player2.setPlayerNumber(2);
  player1.populateMapToCards();
  player2.populateMapToCards();
  player1.loadDeck(deck1);
  player2.loadDeck(deck2);

  string line;

  if (!isTesting) {
    player1.shuffleDeck();
    player2.shuffleDeck();
  }
  for (int i = 0; i < 5; ++i) {
		player1.drawCard();
		player2.drawCard();
	}
  if (isDisplayingGraphics) {
    GO->notify(player1, player2, *activePlayer);
  }
  boardPrint();
  cout << "It's " << activePlayer->getName() << " turn!" << endl;
  while (!gameOver) {
    if (init) {
      getline(initFile, line);
      if (!initFile.good()) {
        init = false;
        getline(cin, line);
      }
    } else {
      getline(cin, line);
      if (!cin.good()) {
        break;
      }
	  }
    stringstream ss (line);
		string command;
		ss >> command;
    if (command == "help") {
      help();
    } else if (command == "end") {
      endTurn();
      boardPrint();
      cout << "It's " << activePlayer->getName() << " turn!" << endl;
      if (isDisplayingGraphics) {
        GO->notify(player1, player2, *activePlayer);
      }
    } else if (command == "quit") {
		  break;
		} else if (command == "draw") {
      if (isTesting) {
        if ((activePlayer->getHand()).size() < 5) {
          activePlayer->drawCard();
          cout << "A card was drawn!" << endl;
          activePlayer->handPrint();
          if (isDisplayingGraphics) {
            GO->notify(player1, player2, *activePlayer);
          }
        }
        else {
          cout << "Hand is full!" << endl;
        }
      } else {
        cout << "This command is only avilable in testing mode!" << endl;
      }
    } else if (command == "discard") {
      if (isTesting) {
				int card;
				ss >> card;
				if (activePlayer->discard(card)) {
          cout << "A card was discarded!" << endl;
          activePlayer->handPrint();
          if (isDisplayingGraphics) {
            GO->notify(player1, player2, *activePlayer);
          }
        }
			} else {
				cout << "This command is only avilable in testing mode!" << endl;
			}
    } else if (command == "attack") {
      int i;
			ss >> i;
			if (ss.good()) {
				int j;
				ss >> j;
				if (activePlayer->attack(i, j)) {
          if (isDisplayingGraphics) {
            GO->notify(player1, player2, *activePlayer);
          }
        }
			} else {
				if (activePlayer->attack(i)) {
          if (isDisplayingGraphics) {
            GO->notify(player1, player2, *activePlayer);
          }
        }
			}
    } else if (command == "play") {
      int card;
			ss >> card;
      if (ss.good()) {
        int p, target;
				char t;
				ss >> p >> t;
        if (t == 'r') target = 6;
        else if (t == '1') target = 1;
        else if (t == '2') target = 2;
        else if (t == '3') target = 3;
				else if (t == '4') target = 4;
        else if (t == '5') target = 5;
        else {
          cout << "Invalid target." << endl;
          continue;
        }
      
        if (target >= 1 && target <= 6 && activePlayer->play(card, p, target)) {
          if (isDisplayingGraphics) {
            GO->notify(player1, player2, *activePlayer);
          }
        }
      } else {
        if (activePlayer->play(card)) {
          if (isDisplayingGraphics) {
            GO->notify(player1, player2, *activePlayer);
          }
        }
      }
    } else if (command == "use") {
      int minion;
			ss >> minion;
      if (minion < 1 || minion > activePlayer->minionCount()) {
        cout << "Invalid minion." << endl;
        continue;
      }
      if (activePlayer->getMinion(minion)->getTriggerType() != 5) {
        cout << "Minion does not have an activated ability." << endl;
        continue;
      }
      if (activePlayer->getMinion(minion)->getActions() <= 0) {
        cout << "Minion does not have any actions left!" << endl;
        continue;
      }
      if (activePlayer->getMinion(minion)->getSilenced()) {
        cout << "Cannot use activated ability. Minion is silenced." << endl;
        continue;
      }
      if (ss.good()) {
        int p, target;
				char t;
				ss >> p >> t;
        if (t == 'r') target = 6;
        else if (t == '1') target = 1;
        else if (t == '2') target = 2;
        else if (t == '3') target = 3;
				else if (t == '4') target = 4;
        else if (t == '5') target = 5;
        else {
          cout << "Invalid target." << endl;
          continue;
        }
        if (target >= 1 && target <= 6 && activePlayer->useAbility(minion, p, target)) {
          if (isDisplayingGraphics) {
            GO->notify(player1, player2, *activePlayer);
          }
        }
      } else {
        if (activePlayer->useAbility(minion)) {
          if (isDisplayingGraphics) {
            GO->notify(player1, player2, *activePlayer);
          }
        }
      }
    } else if (command == "inspect") {
      int minion;
			ss >> minion;
      activePlayer->inspectMinion(minion);
    } else if (command == "hand") {
      activePlayer->handPrint();
    } else if (command == "board") {
      boardPrint();
    } else {
			cout << "Invalid command. Type 'help' for a list of commands." << endl;
		}
  }

}

void boardPrint() {
  // Top Border
  cout << EXTERNAL_BORDER_CHAR_TOP_LEFT;
  for (int i = 0; i < 165; ++i) {
    cout << EXTERNAL_BORDER_CHAR_LEFT_RIGHT;
  }
  cout << EXTERNAL_BORDER_CHAR_TOP_RIGHT << endl;

  // Player 1's Row (Top)
  vector <vector<string>> Player1Row;
    // Ritual
  vector<string> ritual1 = printRitual(player1.getRitual());
  Player1Row.emplace_back(ritual1);
  Player1Row.emplace_back(CARD_TEMPLATE_EMPTY);  // Divider
  Player1Row.emplace_back(display_player_card(1, player1.getName(), player1.getLife(), player1.getMagic()));
  Player1Row.emplace_back(CARD_TEMPLATE_EMPTY);  // Divider
    // Graveyard
  shared_ptr<Card> player1GraveYard = dynamic_pointer_cast<Card>(player1.getTopGraveyard());
	if (player1GraveYard == nullptr) {
    Player1Row.emplace_back(CARD_TEMPLATE_BORDER);
  }
	else {
		Player1Row.emplace_back(printMinion(player1GraveYard));
	}
  for (size_t i = 0; i < Player1Row[0].size(); ++i) {
 	 	cout << EXTERNAL_BORDER_CHAR_UP_DOWN;
 	 	for (int k = 0; k < 5; ++k) {
      cout << Player1Row[k][i];
    }
		cout << EXTERNAL_BORDER_CHAR_UP_DOWN << endl;
  }

  // Player 1's Minions
  int oneMinionCount = player1.minionCount();
  vector<vector<string>> Player1Minions;
  for (int i = 1; i <= oneMinionCount; ++i) {
    shared_ptr<Minion> m = player1.getMinion(i);
    // Card *c = (Card *) m;
    vector<string> minion;
    if (m->getTriggerType() == 0) minion = display_minion_no_ability(m->getName(), m->getCost(), m->getAttack(), m->getDefence());
		else if (m->getTriggerType() == 5) minion = display_minion_activated_ability(m->getName(), m->getCost(), m->getAttack(), m->getDefence(), m->getAbilityCost(), m->getDescription());
		else minion = display_minion_triggered_ability(m->getName(), m->getCost(), m->getAttack(), m->getDefence(), m->getDescription());
    Player1Minions.emplace_back(minion);
  }
  for (int i = oneMinionCount; i < 5; ++i) {
    Player1Minions.emplace_back(CARD_TEMPLATE_BORDER);
  }
  for (size_t i = 0; i < Player1Minions[0].size(); ++i) {
    cout << EXTERNAL_BORDER_CHAR_UP_DOWN;
    for (int k = 0; k < 5; ++k) {
      cout << Player1Minions[k][i];
    }
    cout << EXTERNAL_BORDER_CHAR_UP_DOWN;
    cout << endl;
  }

  // Logo - Centre Row
  vector<string> logo = CENTRE_GRAPHIC;
  for (size_t i = 0; i < logo.size(); ++i) {
    cout << logo[i] << endl;
  }

  // Player 2's Minions
  int twoMinionCount = player2.minionCount();
  vector<vector<string>> Player2Minions;
  for (int i = 1; i <= twoMinionCount; ++i) {
    shared_ptr<Minion> m = player2.getMinion(i);
    vector<string> minion;
    if (m->getTriggerType() == 0) minion = display_minion_no_ability(m->getName(), m->getCost(), m->getAttack(), m->getDefence());
		else if (m->getTriggerType() == 5) minion = display_minion_activated_ability(m->getName(), m->getCost(), m->getAttack(), m->getDefence(), m->getAbilityCost(), m->getDescription());
		else minion = display_minion_triggered_ability(m->getName(), m->getCost(), m->getAttack(), m->getDefence(), m->getDescription());
    Player2Minions.emplace_back(minion);
  }
  for (int i = twoMinionCount; i < 5; ++i) {
    Player2Minions.emplace_back(CARD_TEMPLATE_BORDER);
  }
  for (size_t i = 0; i < Player2Minions[0].size(); ++i) {
    cout << EXTERNAL_BORDER_CHAR_UP_DOWN;
    for (int k = 0; k < 5; ++k) {
      cout  << Player2Minions[k][i];
    }
    cout << EXTERNAL_BORDER_CHAR_UP_DOWN;
    cout << endl;
  }

  // Player 2's Row (Bottom)
  vector <vector<string>> Player2Row;
    // Ritual
  vector<string> ritual2 = printRitual(player2.getRitual());
  Player2Row.emplace_back(ritual2);
  Player2Row.emplace_back(CARD_TEMPLATE_EMPTY);  // Divider
  Player2Row.emplace_back(display_player_card(1, player2.getName(), player2.getLife(), player2.getMagic()));
  Player2Row.emplace_back(CARD_TEMPLATE_EMPTY);  // Divider
    // Graveyard
  shared_ptr<Card> player2GraveYard = dynamic_pointer_cast<Card>(player2.getTopGraveyard());
	if (player2GraveYard == nullptr) {
    Player2Row.emplace_back(CARD_TEMPLATE_BORDER);
  }
	else {
		Player2Row.emplace_back(printMinion(player2GraveYard));
	}
  for (size_t i = 0; i < Player2Row[0].size(); ++i) {
    cout << EXTERNAL_BORDER_CHAR_UP_DOWN;
    for (int k = 0; k < 5; ++k) {
      cout << Player2Row[k][i];
    }
    cout << EXTERNAL_BORDER_CHAR_UP_DOWN;
    cout << endl;
  }

  // Bottom Border
  cout << EXTERNAL_BORDER_CHAR_BOTTOM_LEFT;
  for (int i = 0; i < 165; ++i) {
    cout << EXTERNAL_BORDER_CHAR_LEFT_RIGHT;
  }
  cout << EXTERNAL_BORDER_CHAR_BOTTOM_RIGHT << endl;
}

extern vector<string> printMinion(shared_ptr<Card> card) {
	vector<string> minionString;
	string name = card->getName();
	int cost = card->getCost();
	std::shared_ptr<Minion> m = dynamic_pointer_cast<Minion>(card);
	int attack = m->getAttack();
	int defence = m->getDefence();
  
  if (m->getTriggerType() == 0) minionString = display_minion_no_ability(name, cost, attack, defence);
  else if (m->getTriggerType() == 5) minionString = display_minion_activated_ability(name, cost, attack, defence, m->getAbilityCost(), m->getDescription());
  else minionString = display_minion_triggered_ability(name, cost, attack, defence, m->getDescription());
  return minionString;
}

extern vector<string> printRitual(shared_ptr<Ritual> card) {
  if (!card) {
    return CARD_TEMPLATE_BORDER;
  }
  
  return display_ritual(card->getName(), card->getCost(), card->getActivationCost(), card->getDescription(), card->getChargesLeft());
}


void help() {
  cout << "help -- Display this message." << endl;
  cout << "end -- End the current player's turn." << endl;
  cout << "quit -- End the game." << endl;
  cout << "attack minion other-minion -- Orders minion to attack other-minion." << endl;
  cout << "attack minion -- Orders minion to attack opponent." << endl; 
  cout << "play card [target-player target-card] -- Play card, optionally targeting target-card owned by target-player." << endl;
  cout << "use minion [target-player target-card] -- Use minion's special ability, optionally targeting target-card owned by target-player." << endl;
  cout << "inspect minion -- View a minion's card and all enchantments on that minion." << endl;
  cout << "hand -- Describe all cards in your hand." << endl;
  cout << "board -- Describe all cards on the board." << endl;
}

void endTurn() {
  activateTrigger(4);

  if (activePlayer == &player1) {
    activePlayer = &player2;
    inactivePlayer = &player1;
  } else if (activePlayer == &player2) {
    activePlayer = &player1;
    inactivePlayer = &player2;
  }
  // draw card
  if ((activePlayer->getHand()).size() < 5) {
    activePlayer->drawCard();
  }

  // implement triggers, reset fields, e.t.c
  activePlayer->resetActions();
  if (!firstRound) {
    activePlayer->increaseMagic(1);
  } else {
    firstRound = false;
  }

  activateTrigger(1);
}

// trigger - 1: start of turn
// trigger - 2: minion enters
// trigger - 3: minion leaves (dies)
// trigger 4: end of turn
extern void activateTrigger(int trigger) {
  shared_ptr<Ritual> ritual1 = activePlayer->getRitual();
  shared_ptr<Ritual> ritual2 = inactivePlayer->getRitual();

  for (int i = 1; i <= activePlayer->minionCount(); ++i) {
    shared_ptr<Minion> minion1 = activePlayer->getMinion(i);
    if (!minion1) {
      break;
    }
    if (minion1->getTriggerType() == trigger) {
      if (trigger == 1 || trigger == 4) {
				minion1->useAbility();
      } else if (trigger == 2 && !minion1->isJustPlaced()) {
        shared_ptr<Minion> triggerMinion;
        if (triggerP == 1) {
          triggerMinion = player1.getMinion(triggerT);
        } else {
          triggerMinion = player2.getMinion(triggerT);
        }
        minion1->useAbility(triggerP, triggerT);
        if (triggerMinion->getDefence() <= 0) {
          return;
        }
      } else if (trigger == 3) {
				minion1->useAbility(minion1->getOwner(), activePlayer->getMinionIndex(minion1));
			}
      if (minion1->isJustPlaced()) {
        minion1->notJustPlaced();
      }
    }
  }

  // Active Player's Rituals
  if (ritual1 && trigger == ritual1->getTriggerType()) {
    if (trigger == 1) {
			activePlayer->doRitual();
		} else if (trigger == 2) {
      shared_ptr<Minion> triggerMinion;
      if (triggerP == 1) {
        triggerMinion = player1.getMinion(triggerT);
      } else {
        triggerMinion = player2.getMinion(triggerT);
      }
			activePlayer->doRitual(triggerP, triggerT);
      if (triggerMinion->getDefence() <= 0) {
        return;
      }
	  }
  }

  for (int i = 1; i <= inactivePlayer->minionCount(); ++i) {
    shared_ptr<Minion> minion2 = inactivePlayer->getMinion(i);
    if (!minion2) {
      break;
    }
    if (minion2->getTriggerType() == trigger) {
      if (trigger == 2 && !minion2->isJustPlaced()) {
        shared_ptr<Minion> triggerMinion;
        if (triggerP == 1) {
          triggerMinion = player1.getMinion(triggerT);
        } else {
          triggerMinion = player2.getMinion(triggerT);
        }
        minion2->useAbility(triggerP, triggerT);
        if (triggerMinion->getDefence() <= 0) {
          return;
        }
      } else if (trigger == 3) {
				minion2->useAbility(minion2->getOwner(), inactivePlayer->getMinionIndex(minion2));
			}

      if (minion2->isJustPlaced()) {
        minion2->notJustPlaced();
      }
    }
  }

  // Inactive Player's Rituals
  if (ritual2 && trigger == ritual2->getTriggerType()) {
    if (trigger == 2) {
      shared_ptr<Minion> triggerMinion;
      if (triggerP == 1) {
        triggerMinion = player1.getMinion(triggerT);
      }  else {
        triggerMinion = player2.getMinion(triggerT);
      }
			inactivePlayer->doRitual(triggerP, triggerT);
      if (triggerMinion->getDefence() <= 0) {
        return;
      }
	  }
  }
}

extern void winner(string name) {
	cout << name << " won the game!" << endl;
	gameOver = true;
}

extern void endGame() {
	cout << "An error has occured" << endl;
	gameOver = true;
}
