#include "player.h"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    playerSide = side;
    if(playerSide == WHITE)
    {
        opponentSide = BLACK;
    }
    else
    {
        opponentSide = WHITE;
    }
    board = new Board();
}

/*
 * Destructor for the player.
 */
Player::~Player() {
    delete board;
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    
    board->doMove(opponentsMove, opponentSide);

    if(!board->hasMoves(playerSide))
    {
        return NULL;
    }

    std::vector<Move *> possibleMoves;
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            Move * current = new Move(i,j);
            if(board->checkMove(current, playerSide))
            {
                possibleMoves.push_back(current);
            }
        }
    }

    Move * chosenMove = getBestMove(possibleMoves);
    board->doMove(chosenMove, playerSide);
    return chosenMove;
}

/*
* @brief returns the best move heuristically out of a selection of moves
*
* @param possibleMoves - a list of possible moves
*
*/
Move *Player::getBestMove(std::vector<Move *> possibleMoves)
{
    int numPossibleMoves = possibleMoves.size();
    
    return possibleMoves[rand() % numPossibleMoves];
}