#include <iostream>
#include "GraphicsObserver.h"

using namespace std;

GraphicsObserver::GraphicsObserver() {
    w.fillRectangle(padding, padding, windowWidth, windowHeight, Xwindow::Black);
    w.drawTitle(cardWidth * 2 + padding * 7, cardHeight * 2 + padding * 15, "S O R C E R Y");
}

// Draw description with indent to allow space for top left num>>>>>>> 3e2b9eacf072e1aeb60a4d564dd110c25b1b9ce0
void GraphicsObserver::drawDescYesTL(int xOnWindow, int yOnWindow, string desc) {
    int ind = 0;
    int n = desc.length();
    while ((ind * descWidthYesTR) <= n) {
        // Check for last row
        if (n < (ind + 1) * descWidthYesTR) {
            w.drawString(xOnWindow * cardWidth + padding * 4 + (padding * 5), yOnWindow * cardHeight + padding * 6 + lineHeight * 2 + 2 * padding * ind, desc.substr(ind * descWidthYesTR));
        } else {
            w.drawString(xOnWindow * cardWidth + padding * 4 + (padding * 5), yOnWindow * cardHeight + padding * 6 + lineHeight * 2 + 2 * padding * ind, desc.substr(ind * descWidthYesTR,  descWidthYesTR));
        }
        ++ind;
    }
}

// Draw description without indent
void GraphicsObserver::drawDescNoTL(int xOnWindow, int yOnWindow, string desc) {
    int ind = 0;
    int n = desc.length();
    while ((ind * descWidthNoTR) <= n) {
        // Check for last row
        if (n <= (ind + 1) * descWidthNoTR) {
            w.drawString(xOnWindow * cardWidth + padding * 4, yOnWindow * cardHeight + padding * 6 + lineHeight * 2 + 2 * padding * ind, desc.substr(ind * descWidthNoTR));
        } else {
            w.drawString(xOnWindow * cardWidth + padding * 4, yOnWindow * cardHeight + padding * 6 + lineHeight * 2 + 2 * padding * ind, desc.substr(ind * descWidthNoTR,  descWidthNoTR));
        }
        ++ind;
    }
}

void GraphicsObserver::drawCard(int xOnWindow, int yOnWindow, shared_ptr<Card> cPtr, int pNum) {
    if (!cPtr) {
        return;
    }
    
    // Draw card box
    w.fillRectangle(xOnWindow * cardWidth + padding, yOnWindow * cardHeight + padding, cardWidth, cardHeight, Xwindow::White);

    // Draw borders for player1, player2, and dead minion
    if (pNum == 1) {
        w.fillRectangle(xOnWindow * cardWidth + padding, yOnWindow * cardHeight + padding, cardP, cardHeight, Xwindow::Blue);
        w.fillRectangle(xOnWindow * cardWidth + padding, yOnWindow * cardHeight + padding, cardWidth, cardP, Xwindow::Blue);
        w.fillRectangle(xOnWindow * cardWidth + padding + cardWidth - cardP, yOnWindow * cardHeight + padding, cardP, cardHeight, Xwindow::Blue);
        w.fillRectangle(xOnWindow * cardWidth + padding, yOnWindow * cardHeight + padding + cardHeight - cardP, cardWidth, cardP, Xwindow::Blue);
    } else if (pNum == 2) {
        w.fillRectangle(xOnWindow * cardWidth + padding, yOnWindow * cardHeight + padding, cardP, cardHeight, Xwindow::Red);
        w.fillRectangle(xOnWindow * cardWidth + padding, yOnWindow * cardHeight + padding, cardWidth, cardP, Xwindow::Red);
        w.fillRectangle(xOnWindow * cardWidth + padding + cardWidth - cardP, yOnWindow * cardHeight + padding, cardP, cardHeight, Xwindow::Red);
        w.fillRectangle(xOnWindow * cardWidth + padding, yOnWindow * cardHeight + padding + cardHeight - cardP, cardWidth, cardP, Xwindow::Red);
    } else {
        w.fillRectangle(xOnWindow * cardWidth + padding, yOnWindow * cardHeight + padding, cardP, cardHeight, Xwindow::Black);
        w.fillRectangle(xOnWindow * cardWidth + padding, yOnWindow * cardHeight + padding, cardWidth, cardP, Xwindow::Black);
        w.fillRectangle(xOnWindow * cardWidth + padding + cardWidth - cardP, yOnWindow * cardHeight + padding, cardP, cardHeight, Xwindow::Black);
        w.fillRectangle(xOnWindow * cardWidth + padding, yOnWindow * cardHeight + padding + cardHeight - cardP, cardWidth, cardP, Xwindow::Black);
    }

    // Draw card name + cost + type
    w.drawString(xOnWindow * cardWidth + padding * 4, yOnWindow * cardHeight + padding * 6, cPtr->getName());
    w.drawString(xOnWindow * cardWidth + cardWidth - padding * 3, yOnWindow * cardHeight + padding * 6, to_string(cPtr->getCost()));
    w.drawString(xOnWindow * cardWidth + padding * 4, yOnWindow * cardHeight + padding * 6 + lineHeight, cPtr->getType());

    string desc = cPtr->getDescription();
    
    if (cPtr->getType() == "Ritual") {
        std::shared_ptr<Ritual> ritualPtr = std::dynamic_pointer_cast<Ritual>(cPtr);

        // Draw activation cost + charges left
        w.drawString(xOnWindow * cardWidth + padding * 4 + padding, yOnWindow * cardHeight + padding * 6 + lineHeight * 2, to_string(ritualPtr->getActivationCost()));
        w.drawString(xOnWindow * cardWidth + cardWidth - padding * 3, yOnWindow * cardHeight + cardHeight - padding * 3, to_string(ritualPtr->getChargesLeft()));
        drawDescYesTL(xOnWindow, yOnWindow, desc);

    } else if (cPtr->getType() == "Minion") {
        std::shared_ptr<Minion> minionPtr = std::dynamic_pointer_cast<Minion>(cPtr);
        
        // Draw att and def
        w.drawString(xOnWindow * cardWidth + padding * 4 + padding, yOnWindow * cardHeight + cardHeight - padding * 3, to_string(minionPtr->getAttack()));
        w.drawString(xOnWindow * cardWidth + cardWidth - padding * 3, yOnWindow * cardHeight + cardHeight - padding * 3, to_string(minionPtr->getDefence()));
        
        // Draw desc for minion with activated ability and then without
        if (minionPtr->getTriggerType() == 5) {
            drawDescYesTL(xOnWindow, yOnWindow, desc);
            w.drawString(xOnWindow * cardWidth + padding * 4 + padding, yOnWindow * cardHeight + padding * 6 + lineHeight * 2, to_string(minionPtr->getAbilityCost()));
        } else {
            drawDescNoTL(xOnWindow, yOnWindow, desc);
        }
    } else if (cPtr->getType() == "Enchantment") {
        std::shared_ptr<Enchantment> enchPtr = std::dynamic_pointer_cast<Enchantment>(cPtr);
        if (enchPtr->getName() == "Giant Strength" || enchPtr->getName() == "Enrage") {
            w.drawString(xOnWindow * cardWidth + padding * 4 + padding, yOnWindow * cardHeight + cardHeight - padding * 3, enchPtr->getAttMultiplier());
            w.drawString(xOnWindow * cardWidth + cardWidth - padding * 3, yOnWindow * cardHeight + cardHeight - padding * 3, enchPtr->getDefMultiplier());
            drawDescYesTL(xOnWindow, yOnWindow, desc);
        } else {
            drawDescNoTL(xOnWindow, yOnWindow, desc);
        }
    } else {
        drawDescNoTL(xOnWindow, yOnWindow, desc);
    }
}

void GraphicsObserver::drawBlackBox(int x, int y) {
    w.fillRectangle(x * cardWidth + padding, y * cardHeight + padding, cardWidth, cardHeight, Xwindow::Black);
}

void GraphicsObserver::drawPlayerCard(int xOnWindow, int yOnWindow, string name, int life, int magic, int pNum) {
    // Draw card box
    w.fillRectangle(xOnWindow * cardWidth + padding, yOnWindow * cardHeight + padding, cardWidth, cardHeight, Xwindow::White);

    // Draw borders
    if (pNum == 1) {
        w.fillRectangle(xOnWindow * cardWidth + padding, yOnWindow * cardHeight + padding, cardP, cardHeight, Xwindow::Blue);
        w.fillRectangle(xOnWindow * cardWidth + padding, yOnWindow * cardHeight + padding, cardWidth, cardP, Xwindow::Blue);
        w.fillRectangle(xOnWindow * cardWidth + padding + cardWidth - cardP, yOnWindow * cardHeight + padding, cardP, cardHeight, Xwindow::Blue);
        w.fillRectangle(xOnWindow * cardWidth + padding, yOnWindow * cardHeight + padding + cardHeight - cardP, cardWidth, cardP, Xwindow::Blue);
    } else if (pNum == 2) {
        w.fillRectangle(xOnWindow * cardWidth + padding, yOnWindow * cardHeight + padding, cardP, cardHeight, Xwindow::Red);
        w.fillRectangle(xOnWindow * cardWidth + padding, yOnWindow * cardHeight + padding, cardWidth, cardP, Xwindow::Red);
        w.fillRectangle(xOnWindow * cardWidth + padding + cardWidth - cardP, yOnWindow * cardHeight + padding, cardP, cardHeight, Xwindow::Red);
        w.fillRectangle(xOnWindow * cardWidth + padding, yOnWindow * cardHeight + padding + cardHeight - cardP, cardWidth, cardP, Xwindow::Red);
    }

    // Draw player card info
    w.drawString(xOnWindow * cardWidth + padding * 4, yOnWindow * cardHeight + padding * 6, "Player: " + name);
    w.drawString(xOnWindow * cardWidth + padding * 4 + padding, yOnWindow * cardHeight + cardHeight - padding * 3, to_string(life));
    w.drawString(xOnWindow * cardWidth + cardWidth - padding * 3, yOnWindow * cardHeight + cardHeight - padding * 3, to_string(magic));  
}

// Draw the entire board
void GraphicsObserver::notify(Player &p1, Player &p2, Player &active) {
    // Rituals, graveyards, player cards
    drawPlayerCard(2, 0, p1.getName(), p1.getLife(), p1.getMagic(), 1);
    drawPlayerCard(2, 4, p2.getName(), p2.getLife(), p2.getMagic(), 2);
    drawCard(0, 0, p1.getRitual(), 1);
    drawCard(0, 4, p2.getRitual(), 2);
    drawCard(4, 0, p1.getTopGraveyard(), 0);
    drawCard(4, 4, p2.getTopGraveyard(), 0);

    // Minions on board
    for (int i = 0; i < p1.minionCount(); ++i) {
        drawCard(i, 1, p1.getMinion(i + 1), 1);
    }
    for (int i = p1.minionCount(); i < 5; ++i) {
        drawBlackBox(i, 1);
    }

    for (int i = 0; i < p2.minionCount(); ++i) {
        drawCard(i, 3, p2.getMinion(i + 1), 2);
    }
    for (int i = p2.minionCount(); i < 5; ++i) {
        drawBlackBox(i, 3);
    }

    // Active hand
    for (int i = 0; i < active.handSize(); ++i) {
        drawCard(i, 5, active.getCardAtHand(i), active.getPlayerNumber());
    }
    for (int i = active.handSize(); i < 5; ++i) {
        drawBlackBox(i, 5);
    }
}
