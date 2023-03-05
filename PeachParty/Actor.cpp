#include "Actor.h"
#include "GameConstants.h"
#include "StudentWorld.h"
#include "GameWorld.h"
#include "Board.h"
#include "GraphObject.h"

#include <string>
#include <queue>
#include <set>
#include <iostream>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// =====================/* PLAYER AVATAR METHODS */==============================================

void PlayerAvatar::setStars(int newAmount)
{
    this->stars = newAmount;
    return;
}

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

void PlayerAvatar::teleportToRandomSquare(StudentWorld* world)
{
    Board board;
    std::string board_file = get_thisWorld()->assetPath() + "board0" + std::to_string(get_thisWorld()->getBoardNumber()) + ".txt";
    board.loadBoard(board_file);
    while(true) // keep a loop running so that you can keep finding new random squares until it meets the requirements
    {
        int randX_board = randInt(1, 16);
        int randY_board = randInt(1, 16);
        // make sure the new square is not the same one that player is currently on and not empty either
        if((board.getContentsOf(randX_board, randY_board) != Board::empty)
           && (randX_board*SPRITE_WIDTH != this->getX() && randY_board*SPRITE_HEIGHT != this->getY()))
        {
            // move the player to this new square
            this->moveTo(randX_board*SPRITE_WIDTH, randY_board*SPRITE_HEIGHT);
            
            // UPDATE THE JUST LANDED STATUS SO THE PLAYER ACTIVATES WHATEVER SQUARE IT IS ON
            this->didJustLand = true;
            
            return; // exit the loop & function once the teleportation is complete
        }
    }
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

bool PlayerAvatar::isFork(Board b, std::set<int>& validDirections)
{
    // check if the player is currently at a fork and store valid directions to move into the set
    int nextX = 0, nextY = 0;
    // check right
    getPositionInThisDirection(right, SPRITE_WIDTH, nextX, nextY);
    if (b.getContentsOf(nextX/SPRITE_WIDTH, nextY/SPRITE_HEIGHT) != Board::empty)
    {
        validDirections.insert((int)right);
    }
    // check left
    getPositionInThisDirection(left, SPRITE_WIDTH, nextX, nextY);
    if (b.getContentsOf(nextX/SPRITE_WIDTH, nextY/SPRITE_HEIGHT) != Board::empty)
    {
        validDirections.insert((int)left);
    }
    // check up
    getPositionInThisDirection(up, SPRITE_WIDTH, nextX, nextY);
    if (b.getContentsOf(nextX/SPRITE_WIDTH, nextY/SPRITE_HEIGHT) != Board::empty)
    {
        validDirections.insert((int)up);
    }
    // check down
    getPositionInThisDirection(down, SPRITE_WIDTH, nextX, nextY);
    if (b.getContentsOf(nextX/SPRITE_WIDTH, nextY/SPRITE_HEIGHT) != Board::empty)
    {
        validDirections.insert((int)down);
    }
    
    // make sure to exclude the walking direction that player came from (opposite of the walk direction)
    if (this->get_walkDirection() == GraphObject::right)
        validDirections.erase((int)left);
    if (this->get_walkDirection() == GraphObject::left)
        validDirections.erase((int)right);
    if (this->get_walkDirection() == GraphObject::up)
        validDirections.erase((int)down);
    if (this->get_walkDirection() == GraphObject::down)
        validDirections.erase((int)up);
    
    
    if (validDirections.size() >= 2) return true;
    else return false;
}


void PlayerAvatar::doSomething()
{
    // load the board
    Board board;
    std::string board_file = get_thisWorld()->assetPath() + "board0" + std::to_string(get_thisWorld()->getBoardNumber()) + ".txt";
    board.loadBoard(board_file);
    
    this->didJustLand = false;
    
    if (this->getState() == "waiting_to_roll")
    {
        // do a check for if the player has an invalid walking direction (can occur after being teleported)
        if (this->getX()%16==0 && this->getY()%16==0)
        {
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
        std::set<int> validDirections;
        validDirections.clear();
        // functionality for if the character is at a fork in the road (only check when an actor is directly on a square) && not still in starting square && the square currently on is not a directional square
        if (this->getX()%16==0 && this->getY()%16==0 && this->isFork(board, validDirections) && !this->stillInStart()
            && board.getContentsOf(this->getX()/SPRITE_WIDTH, this->getY()/SPRITE_HEIGHT) != Board::up_dir_square
            && board.getContentsOf(this->getX()/SPRITE_WIDTH, this->getY()/SPRITE_HEIGHT) != Board::down_dir_square
            && board.getContentsOf(this->getX()/SPRITE_WIDTH, this->getY()/SPRITE_HEIGHT) != Board::left_dir_square
            && board.getContentsOf(this->getX()/SPRITE_WIDTH, this->getY()/SPRITE_HEIGHT) != Board::right_dir_square)
        {
            int action_forFork = get_thisWorld()->getAction(this->getPlayer());
            switch (action_forFork)
            {
                case ACTION_UP:
                    if (validDirections.find((int)up) != validDirections.end()) // if the user input is a valid dir (it is contained in the set of validDirections)
                    {
                        // update the player walk direction
                        this->set_walkDirection(GraphObject::up);
                        this->setDirection(right);
                    }
                    break;
                case ACTION_DOWN:
                    if (validDirections.find((int)down) != validDirections.end())
                    {
                        this->set_walkDirection(GraphObject::down);
                        this->setDirection(right);
                    }
                    break;
                case ACTION_LEFT:
                    if (validDirections.find((int)left) != validDirections.end())
                    {
                        this->set_walkDirection(GraphObject::left);
                        
                        // change the sprite direction to face left as well
                        this->setDirection(GraphObject::left);
                    }
                    break;
                case ACTION_RIGHT:
                    if (validDirections.find((int)right) != validDirections.end())
                    {
                        this->set_walkDirection(GraphObject::right);
                        this->setDirection(right);
                    }
                    break;
                default: // if the user does not press any of these then we return (prevents avatar from moving on)
                    return;
            }
            
        }

        // if the avatar can not keep moving forward in the current direction, only do this check if the actor is directly on a square (16a, 16b)
        if (this->getX()%16==0 && this->getY()%16==0)
        {
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
        
        still_InStart = false; // after the first move the player is no longer still in start mode
        // this feature is so that the forking check doesn't activate on the starting position
        
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

void CoinSquare::coinSquareFunctionality(StudentWorld* world, PlayerAvatar* player)
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
        coinSquareFunctionality(world, peachPlayer);
    }
        
    // check if a new yoshi landed on the square
    PlayerAvatar* yoshiPlayer = world->getPlayer(2);
    int yoshiX = yoshiPlayer->getX();
    int yoshiY = yoshiPlayer->getY();
    if ((yoshiX == this->getX() && yoshiY == this->getY()) && yoshiPlayer->justLanded()) // the yoshi player is exactly on top of the coinsquare and it just landed on the coin square
    {
        coinSquareFunctionality(world, yoshiPlayer);
    }
}
        
        
// =====================/* STARSQUARE METHODS */==============================================

        
void StarSquare::starSquareFunctionality(StudentWorld* world, PlayerAvatar* player)
{
    if (player->getCoins() < 20)
        return;
    else
    {
        // deduct 20 coins from the player
        int newAmount = player->getCoins() - 20;
        player->setCoins(newAmount);
        
        // give 1 star to the player
        newAmount = player->getStars() + 1;
        player->setStars(newAmount);
        
        // play the sound
        world->playSound(SOUND_GIVE_STAR);
        
    }
}

void StarSquare::doSomething()
{
    StudentWorld* world = this->get_thisWorld();
    
    // check if a new peach has landed on the square OR passed over the square
    PlayerAvatar* peachPlayer = world->getPlayer(1);
    int peachX = peachPlayer->getX();
    int peachY = peachPlayer->getY();
    if ((peachX == this->getX() && peachY == this->getY()) && peachPlayer->justLanded()) // if peach just LANDED on star square
    {
        starSquareFunctionality(world, peachPlayer);
    }
    // peach is moving over the square (currently in the walking state) star square activates
    if ((peachX == this->getX() && peachY == this->getY()) && peachPlayer->getState() == "walking")
    {
        starSquareFunctionality(world, peachPlayer);
    }
    
    
    // check if a new yoshi has landed on the square OR passed over the square
    PlayerAvatar* yoshiPlayer = world->getPlayer(2);
    int yoshiX = yoshiPlayer->getX();
    int yoshiY = yoshiPlayer->getY();
    if ((yoshiX == this->getX() && yoshiY == this->getY()) && yoshiPlayer->justLanded()) // if yoshi just LANDED on star square
    {
        starSquareFunctionality(world, yoshiPlayer);
    }
    
    // yoshi is moving over the square (currently in the walking state) star square activates
    if ((yoshiX == this->getX() && yoshiY == this->getY()) && yoshiPlayer->getState() == "walking")
    {
        starSquareFunctionality(world, yoshiPlayer);
    }
}

// =====================/* DIRECTIONAL SQUARE METHODS */==============================================

void DirectionalSquare::directionSquareFunctionality(PlayerAvatar *player)
{
    int newDirection = this->get_directionOfSquare();
    
    // update sprite directions
    if (newDirection == GraphObject::left) player->setDirection(left);
    else player->setDirection(right);
    
    // update walk directions
    player->set_walkDirection(newDirection);
}

void DirectionalSquare::doSomething()
{
    StudentWorld* world = this->get_thisWorld();
    
    
    // check if a new peach has landed on the square OR passed over the square
    PlayerAvatar* peachPlayer = world->getPlayer(1);
    int peachX = peachPlayer->getX();
    int peachY = peachPlayer->getY();
    if ((peachX == this->getX() && peachY == this->getY()) && peachPlayer->justLanded()) // if peach just LANDED on direction square
    {
        directionSquareFunctionality(peachPlayer);
    }
    // peach is moving over the square (currently in the walking state) direction square activates
    if ((peachX == this->getX() && peachY == this->getY()) && peachPlayer->getState() == "walking")
    {
        directionSquareFunctionality(peachPlayer);
    }
    

    // check if a new yoshi has landed on the square OR passed over the square
    PlayerAvatar* yoshiPlayer = world->getPlayer(2);
    int yoshiX = yoshiPlayer->getX();
    int yoshiY = yoshiPlayer->getY();
    if ((yoshiX == this->getX() && yoshiY == this->getY()) && yoshiPlayer->justLanded()) // if yoshi just LANDED on direction square
    {
        directionSquareFunctionality(yoshiPlayer);
    }
    // yoshi is moving over the square (currently in the walking state) direction square activates
    if ((yoshiX == this->getX() && yoshiY == this->getY()) && yoshiPlayer->getState() == "walking")
    {
        directionSquareFunctionality(yoshiPlayer);
    }
}


// =====================/* BANK SQUARE METHODS */==============================================


void BankSquare::bankSquareFunctionalityLands(StudentWorld *world, PlayerAvatar *player)
{
    int amountToGivePlayer = world->get_bank_coins();
    
    // give the player the amount of coins that is in the bank
    int newAmount = player->getCoins() + amountToGivePlayer;
    player->setCoins(newAmount);
    
    // reset the central bank balance to zero
    world->reset_bank_coins();
    
    // play sound
    world->playSound(SOUND_WITHDRAW_BANK);
}

void BankSquare::bankSquareFunctionalityPasses(StudentWorld *world, PlayerAvatar *player)
{
    if (player->getCoins() < 5) // if the player has less than 5 coins then take what you can and deposit into the bank
    {
        // deposit into the bank how ever many coins
        world->deposit_bank_coins(player->getCoins());
        
        // take those coins away from the player
        player->setCoins(0);
        
        // play sound
        world->playSound(SOUND_DEPOSIT_BANK);
    }
    else // the player has more than 5 coins
    {
        // deposit 5 coins into the bank
        world->deposit_bank_coins(5);
        
        // take 5 coins away from the player
        int newAmount = player->getCoins() - 5;
        player->setCoins(newAmount);
        
        // play sound
        world->playSound(SOUND_DEPOSIT_BANK);
    }
}

void BankSquare::doSomething()
{
    StudentWorld* world = this->get_thisWorld();
    
    
    // check if a new peach has landed on the square OR passed over the square
    PlayerAvatar* peachPlayer = world->getPlayer(1);
    int peachX = peachPlayer->getX();
    int peachY = peachPlayer->getY();
    if ((peachX == this->getX() && peachY == this->getY()) && peachPlayer->justLanded()) // if peach just LANDED on bank square
    {
        bankSquareFunctionalityLands(world, peachPlayer);
    }
    // peach is moving over the square (currently in the walking state) bank square activates
    if ((peachX == this->getX() && peachY == this->getY()) && peachPlayer->getState() == "walking")
    {
        bankSquareFunctionalityPasses(world, peachPlayer);
    }
    
    
    // check if a new yoshi has landed on the square OR passed over the square
    PlayerAvatar* yoshiPlayer = world->getPlayer(2);
    int yoshiX = yoshiPlayer->getX();
    int yoshiY = yoshiPlayer->getY();
    if ((yoshiX == this->getX() && yoshiY == this->getY()) && yoshiPlayer->justLanded()) // if yoshi just LANDED on bank square
    {
        bankSquareFunctionalityLands(world, yoshiPlayer);
    }
    // yoshi is moving over the square (currently in the walking state) bank square activates
    if ((yoshiX == this->getX() && yoshiY == this->getY()) && yoshiPlayer->getState() == "walking")
    {
        bankSquareFunctionalityLands(world, yoshiPlayer);
    }
    
}


// =====================/* EVENT SQUARE METHODS */==============================================

void EventSquare::eventSquareFunctionalityTeleportation(StudentWorld* world, PlayerAvatar* player)
{
    // teleport the player to a random square
    player->teleportToRandomSquare(world);
    
    // play sound
    world->playSound(SOUND_PLAYER_TELEPORT);
}

void EventSquare::eventSquareFunctionalitySwap(StudentWorld* world, PlayerAvatar* player)
{
    
}

void EventSquare::eventSquareFunctionalityVortex(StudentWorld* world, PlayerAvatar* player)
{
    
}

void EventSquare::doSomething()
{
    StudentWorld* world = this->get_thisWorld();
    
    // check if a new peach has landed on the square
    PlayerAvatar* peachPlayer = world->getPlayer(1);
    int peachX = peachPlayer->getX();
    int peachY = peachPlayer->getY();
    if ((peachX == this->getX() && peachY == this->getY()) && peachPlayer->justLanded()) // if peach just LANDED on event square
    {
        int rand = randInt(1, 3);
        switch (rand)
        {
            case 1:
                eventSquareFunctionalityTeleportation(world, peachPlayer);
                break;
            case 2:
                eventSquareFunctionalitySwap(world, peachPlayer);
                break;
            case 3:
                eventSquareFunctionalityVortex(world, peachPlayer);
        }
    }
    
    // check if a new yoshi has landed on the square
    PlayerAvatar* yoshiPlayer = world->getPlayer(2);
    int yoshiX = yoshiPlayer->getX();
    int yoshiY = yoshiPlayer->getY();
    if ((yoshiX == this->getX() && yoshiY == this->getY()) && yoshiPlayer->justLanded()) // if yoshi just LANDED on event square
    {
        int rand = randInt(1, 3);
        switch (rand)
        {
            case 1:
                eventSquareFunctionalityTeleportation(world, yoshiPlayer);
                break;
            case 2:
                eventSquareFunctionalitySwap(world, yoshiPlayer);
                break;
            case 3:
                eventSquareFunctionalityVortex(world, yoshiPlayer);
        }
    }
}


// =====================/* DROPPING SQUARE METHODS */==============================================

void DroppingSquare::droppingSquareFunctionalityDeductCoins(StudentWorld *world, PlayerAvatar *player)
{
    
}

void DroppingSquare::droppingSquareFunctionalityDeductStars(StudentWorld *world, PlayerAvatar *player)
{
    
}

void DroppingSquare::doSomething()
{
    
}
