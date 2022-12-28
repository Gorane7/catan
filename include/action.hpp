#ifndef ACTION_HPP
#define ACTION_HPP

const int END_TURN = 0;
const int BUILD_ROAD = 1;
const int TRADE_RESOURCES = 2;
const int BUILD_VILLAGE = 3;
const int BUILD_CITY = 4;
const int BUY_DEVELOPMENT_CARD = 5;
const int PLAY_DEVELOPMENT_CARD = 6;

struct Action {
  int actionType;
  int actionLocation;
  int secondaryActionLocation;
  int tertiaryActionLocation;
  int resourceFrom;
  int resourceTo;
};

#endif
