#ifndef __GRAPHICSDISPLAY_H__
#define __GRAPHICSDISPLAY_H__
#include <iostream>
#include <vector>
#include <string>
#include "Window.h"
#include "Player.h"

class GraphicsObserver {
  int windowWidth = 1200;
  int windowHeight = 750;
  int padding = 5;
  int cardP = 3;
  int lineHeight = 15;
  int descWidthNoTR = 35;
  int descWidthYesTR = 30;
  Xwindow w = Xwindow(windowWidth + 2 * padding, windowHeight + 2 * padding);
  int cardWidth = windowWidth / 5;
  int cardHeight = windowHeight / 6;

  void drawDescYesTL(int x, int y, string desc);
  void drawDescNoTL(int x, int y, string desc);

 public:
  GraphicsObserver();
  void drawCard(int x, int y, shared_ptr<Card> cPtr, int pNum);
  void drawPlayerCard(int xOnWindow, int yOnWindow, string name, int life, int magic, int playerNum);
  void drawBlackBox(int x, int y);
  void notify(Player &p1, Player &p2, Player &active);
};

#endif
