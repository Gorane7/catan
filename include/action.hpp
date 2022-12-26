#ifndef ACTION_HPP
#define ACTION_HPP

const int END_TURN = 0;
const int BUILD_ROAD = 1;

struct Action {
  int actionType;
  int actionLocation;
};

#endif
