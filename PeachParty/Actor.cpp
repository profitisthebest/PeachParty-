#include "Actor.h"
#include "GameConstants.h"
#include "StudentWorld.h"
#include "GameWorld.h"
#include "Board.h"
#include "GraphObject.h"

#include <string>
#include <queue>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// constructor for Actor and PlayerAvatar is implemented in class declaration

void PlayerAvatar::setState(std::string newState)
{
    this->state = newState;
    return;
}

void PlayerAvatar::set_walkDirection(int newDirection)
{
    this->walkDirection = newDirection;
    return;
}

void PlayerAvatar::set_ticksToMove(int newTicksToMove)
{
    this->ticks_to_move = newTicksToMove;
    return;
}

int PlayerAvatar::findValidWalkingDirection(int currentWalkingDirection, Board b)
{
    int m_x, m_y;
    
    // in this case we need to find if either right or left is available
    if (currentWalkingDirection == up || currentWalkingDirection == GraphObject::down)
    {
        std::queue<int> directionQueue;
        directionQueue.push((int)right);
        directionQueue.push((int)left);
        for(int i = 0; i < directionQueue.size(); i++)
        {
            getPositionInThisDirection(directionQueue.front(), SPRITE_WIDTH, m_x, m_y);
            if (b.getContentsOf(m_x, m_y) != Board::empty)
            {
                return directionQueue.front();
            }
            directionQueue.pop();
        }
    }
    
    // in this case we need to find if either up or down is available
    if (currentWalkingDirection == right || currentWalkingDirection == left)
    {
        std::queue<int> directionQueue;
        directionQueue.push((int)up);
        directionQueue.push((int)down);
        for(int i = 0; i < directionQueue.size(); i++)
        {
            getPositionInThisDirection(directionQueue.front(), SPRITE_WIDTH, m_x, m_y);
            if (b.getContentsOf(m_x, m_y) != Board::empty)
            {
                return directionQueue.front();
            }
            directionQueue.pop();
        }
    }
    
    // function will never get to this spot because there will always be somewhere to move
    return 0;
}

void PlayerAvatar::doSomething()
{
    if (this->getState() == "waiting_to_roll")
    {
        int action = get_thisWorld()->getAction(this->getPlayer());
        if (action == ACTION_ROLL)
        {
            int die_roll = randInt(1, 10);
            // ticks_to_move = die_roll * 8;
            set_ticksToMove(die_roll*8);
            setState("walking");
        }
        if (action == ACTION_FIRE)
        {
            // add for part 2
        }
        else // action is not ACTION_ROLL or ACTION_FIRE
        {
            return;
        }
    }
    
    if(this->getState() == "walking")
    {
        /* part 2 stuff goes here */
        
        
        // if the avatar can not keep moving forward in the current direction
        Board board;
        std::string board_file = get_thisWorld()->assetPath() + "board0" + std::to_string(get_thisWorld()->getBoardNumber()) + ".txt";
        board.loadBoard(board_file);
        int nextX, nextY;
        getPositionInThisDirection(get_walkDirection(), SPRITE_WIDTH, nextX, nextY);
        if (board.getContentsOf(nextX, nextY) == Board::empty)
        {
            // want to find a valid walking direction: findValidWalkingDirection(get_WalkDirection(), board);
            // set the walk direction to this new valid walking direction
            // then check if the new valid walking direction is left, if it is update the sprite direction to left
            // in all other cases the sprite direction should be right
            
            int m_newDirection = findValidWalkingDirection(get_walkDirection(), board);
            set_walkDirection(m_newDirection);
            if (m_newDirection == left)
            {
                this->setDirection(left);
            }
            else
            {
                this->setDirection(right);
            }
        }
        
        
        // move two pixels in the walk direction
        this->moveAtAngle(get_walkDirection(), 2);
        
        // decrement ticks_to_move by 1
        int z = get_ticksToMove();
        set_ticksToMove(z--);
        
        // check if you should change state
        if (get_ticksToMove() == 0)
        {
            this->setState("waiting_to_roll");
        }
    }
}




