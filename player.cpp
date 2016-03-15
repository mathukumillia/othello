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

/**
*
* @brief sets the player's board to the given board
*
* @param board - the board we want to set as the current board
**/
void Player::setBoard(Board * b)
{
    board = b;
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
		chosenMove = minimax(possibleMoves);
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
    int minimaxScore = -500;
    int * moveScores = (int *)malloc(numPossibleMoves * sizeof(int));
    Move * bestMove;
    
    for(int i = 0; i < numPossibleMoves; i++)
    {
        Board * firstGen = board->copy();
        firstGen->doMove(possibleMoves[i], playerSide);
        
        std::vector<Move *> possibleMoves2;
        
        // get possible moves after the first player side move
		for(int x = 0;x < 8; x++)
		{
			for(int y = 0; y < 8; y++)
			{
				Move * current = new Move(x,y);
				if(firstGen->checkMove(current, opponentSide))
				{
					possibleMoves2.push_back(current);
				}
			}
		}
		
        // make a second level move and calculate the minimum board score that could result
        int minSecondGenScore = 100;
		for(unsigned int f = 0; f < possibleMoves2.size(); f++){			 
            int score = getLocScore(possibleMoves2[f], firstGen);    
			if(score < minSecondGenScore)
            {
                minSecondGenScore = score;
            }
		}
        moveScores[i] = minSecondGenScore;	
    }

    // getting minimax score and the corresponding best move
    int minimaxIndex;
    for(int i = 0; i < numPossibleMoves; i++)
    {
        if(moveScores[i] >= minimaxScore)
        {
            minimaxScore = moveScores[i];
            minimaxIndex = i;
        }
    }

    bestMove = possibleMoves[minimaxIndex];
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

/*
* @brief given a move, finds its heuristic score with a given board
*
* @param move - the move you want to get the score of
* @param customBoard - the board you want to make the move on
*
*/
int Player::getLocScore(Move * move, Board * customBoard)
{
    Board * newBoard = customBoard->copy();
    newBoard->doMove(move, opponentSide);
    int score;
    
    if (newBoard->countBlack() + newBoard->countWhite() < 13){
		score = newBoard->earlyBoardScore(playerSide, opponentSide);
	}else if (newBoard->countBlack() + newBoard->countWhite() > 54){
		score = newBoard->lateBoardScore(playerSide, opponentSide);
	}else{
		score = newBoard->getBoardScore(playerSide, opponentSide);
	}

    return score;
}


