#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.h"
#include "board.h"
#include <cstdlib>
#include <vector>
#include <algorithm>

using namespace std;

class Player {

private:
	Board *board;
	Side playerSide;
	Side opponentSide;

public:
    Player(Side side);
    ~Player();
    
    Move *doMove(Move *opponentsMove, int msLeft);
    Move *getBestMove(std::vector<Move *> possibleMoves);
    int getLocScore(Move * move);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
