#ifndef ACTION_HPP
#define ACTION_HPP

const int END_TURN = 0;
const int BUILD_ROAD = 1;
const int TRADE_RESOURCES = 2;
const int BUILD_VILLAGE = 3;

struct Action {
  int actionType;
  int actionLocation;
  int resourceFrom;
  int resourceTo;
};

#endif
