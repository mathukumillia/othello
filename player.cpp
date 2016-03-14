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
    Move * chosenMove;
	if (testingMinimax == true){
		chosenMove = minimax(possibleMoves);
	}else{
		chosenMove = getBestMove(possibleMoves);
	}
    board->doMove(chosenMove, playerSide);
    return chosenMove;
}

/*
* @brief returns the best move minimax out of a selection of moves
*
* @param returns best move
*
*/
Move *Player::minimax(std::vector<Move *> possibleMoves)
{
    int numPossibleMoves = possibleMoves.size();
    int minimaxscore;
    int * moveScores = (int *)malloc(numPossibleMoves * sizeof(int));
    Move * bestMove;
    
    for(int i = 0; i < numPossibleMoves; i++)
    {
		int flum;
        Board * foo = board->copy();
        foo->doMove(possibleMoves[i], playerSide);
        
        std::vector<Move *> possiblemoves;
        
		for(int p = 0;p < 8; i++)
		{
			for(int j = 0; j < 8; j++)
			{
				Move * current = new Move(p,j);
				if(foo->checkMove(current, opponentSide))
				{
					possiblemoves.push_back(current);
				}
			}
		}
		
		for(unsigned int f = 0; f < possiblemoves.size(); f++){
			Board * goo = foo->copy();
			goo->doMove(possiblemoves[f], opponentSide);
			
			std::vector<Move *> possiblemuves;
        
			for(int p = 0;p < 8; i++)
			{
				for(int j = 0; j < 8; j++)
				{
					Move * current = new Move(p,j);
					if(goo->checkMove(current, playerSide))
					{
						possiblemuves.push_back(current);
					}
				}
			}
			
			for(unsigned int m = 0; m < possiblemuves.size(); m++){
				Board * moo = goo->copy();
				moo->doMove(possiblemuves[m], playerSide);
				
				int gum = moo->count(playerSide);
				flum = min(flum, gum);
			}
		}
		if ( flum > minimaxscore ){
			minimaxscore = flum;
			bestMove = possibleMoves[i];
		}
    }

    std::free(moveScores);

    return bestMove;
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
    int * moveScores = (int *)malloc(numPossibleMoves * sizeof(int));
    int max = -5;
    int maxIndex;
    for(int i = 0; i < numPossibleMoves; i++)
    {
        moveScores[i] = getLocScore(possibleMoves[i]);
        if(moveScores[i] > max)
        {
            maxIndex = i;
            max = moveScores[i];
        }
    }

    Move * bestMove = possibleMoves[maxIndex];
    std::free(moveScores);

    return bestMove;
}

/*
* @brief given a move, finds its heuristic score
*
* @param move - the move you want to get the score of
*
*/
int Player::getLocScore(Move * move)
{
    Board * newBoard = board->copy();
    newBoard->doMove(move, playerSide);
    int score = newBoard->getBoardScore(playerSide, opponentSide);

    return score;
}

