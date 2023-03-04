#include "Actor.h"
#include "GameConstants.h"
#include "StudentWorld.h"
#include "GameWorld.h"
#include "Board.h"
#include "GraphObject.h"

#include <string>
#include <queue>
#include <iostream>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// =====================/* PLAYER AVATAR METHODS */==============================================

void PlayerAvatar::setCoins(int newAmount)
{
    this->coins = newAmount;
    return;
}

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
    if (currentWalkingDirection == up || currentWalkingDirection == down)
    {
        getPositionInThisDirection(right, SPRITE_WIDTH, m_x, m_y);
        if (b.getContentsOf(m_x/SPRITE_WIDTH, m_y/SPRITE_HEIGHT) != Board::empty)
            return right;
        else
        {
            getPositionInThisDirection(left, SPRITE_WIDTH, m_x, m_y);
            if (b.getContentsOf(m_x/SPRITE_WIDTH, m_y/SPRITE_HEIGHT) != Board::empty)
                return left;
        }
    }
    
    // in this case we need to find if either up or down is available
    if (currentWalkingDirection == right || currentWalkingDirection == left)
    {
        getPositionInThisDirection(up, SPRITE_WIDTH, m_x, m_y);
        if (b.getContentsOf(m_x/SPRITE_WIDTH, m_y/SPRITE_HEIGHT) != Board::empty)
            return up;
        else
        {
            getPositionInThisDirection(down, SPRITE_WIDTH, m_x, m_y);
            if (b.getContentsOf(m_x/SPRITE_WIDTH, m_y/SPRITE_HEIGHT) != Board::empty)
                return down;
        }
    }
    
    // function will never get to this spot because there will always be somewhere to move
    return 0;
}

void PlayerAvatar::doSomething()
{
    this->didJustLand = false;
    if (this->getState() == "waiting_to_roll")
    {
        int action = get_thisWorld()->getAction(this->getPlayer());
        
        if (action == ACTION_ROLL)
        {
            int die_roll = randInt(1, 10);
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
        
        
        // if the avatar can not keep moving forward in the current direction, only do this check if the actor is directly on a square (16a, 16b)
        if (this->getX()%16==0 && this->getY()%16==0)
        {
            Board board;
            std::string board_file = get_thisWorld()->assetPath() + "board0" + std::to_string(get_thisWorld()->getBoardNumber()) + ".txt";
            board.loadBoard(board_file);
            int nextX = 0, nextY = 0;
            getPositionInThisDirection(get_walkDirection(), SPRITE_WIDTH, nextX, nextY);
            if (board.getContentsOf(nextX/SPRITE_WIDTH, nextY/SPRITE_HEIGHT) == Board::empty)
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
        }
        
        // move two pixels in the walk direction
        this->moveAtAngle(get_walkDirection(), 2);
        
        // decrement ticks_to_move by 1
        int z = get_ticksToMove() - 1;
        set_ticksToMove(z);
        
        // check if you should change state
        if (get_ticksToMove() == 0)
        {
            this->didJustLand = true; // there are no more ticks for the player to move so it just landed on a square
            this->setState("waiting_to_roll");
        }
    }
}


// =====================/* COINSQUARE METHODS */==============================================

void CoinSquare::set_status_isAlive(bool status)
{
    this->active = status;
    return;
}

void CoinSquare::changePlayerCoins(StudentWorld* world, PlayerAvatar* player)
{
    if (this->get_coinType() == "blue") // if the coin is a blue coin square we want to give the peach 3 coins
    {
        int newAmount = player->getCoins() + 3;
        player->setCoins(newAmount);
        world->playSound(SOUND_GIVE_COIN);
    }
    if (this->get_coinType() == "red") // if the coin is a red coin square we want to take 3 coins from peach
    {
        if (player->getCoins() <= 3) // if peach has less than or three coins we just set coins to zero
        {
            player->setCoins(0);
            world->playSound(SOUND_TAKE_COIN);
        }
        else // other wise just subtract 3 coins from the amount
        {
            int newAmount = player->getCoins() - 3;
            player->setCoins(newAmount);
            world->playSound(SOUND_TAKE_COIN);
        }
    }
}

void CoinSquare::doSomething()
{
    // store a pointer to the world that the square is in
    StudentWorld* world = this->get_thisWorld();

    if (!this->isAlive()) // if the CoinSquare is no longer active/alive return immediately
        return;
        
    // check if a new peach landed on the square
    PlayerAvatar* peachPlayer = world->getPlayer(1);
    int peachX = peachPlayer->getX();
    int peachY = peachPlayer->getY();
    if ((peachX == this->getX() && peachY == this->getY()) && peachPlayer->justLanded()) // the peach player is exactly on top of the coinsquare and it just landed on the coin square
    {
        changePlayerCoins(world, peachPlayer);
    }
        
    // check if a new yoshi landed on the square
    PlayerAvatar* yoshiPlayer = world->getPlayer(2);
    int yoshiX = yoshiPlayer->getX();
    int yoshiY = yoshiPlayer->getY();
    if ((yoshiX == this->getX() && yoshiY == this->getY()) && yoshiPlayer->justLanded()) // the yoshi player is exactly on top of the coinsquare and it just landed on the coin square
    {
        changePlayerCoins(world, yoshiPlayer);
    }
}
        
        
        
        
        





