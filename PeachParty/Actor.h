#ifndef ACTOR_H_
#define ACTOR_H_

#include <string>
#include <iostream>
#include <set>
#include "GraphObject.h"

class StudentWorld;
class Board;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

// =====================/* ACTOR BASE CLASS */==========================

class Actor : public GraphObject
{
public:
    // constructor for an actor
    Actor(StudentWorld *world, int imageID, int startX, int startY, int dir = right, int depth = 0, double size = 1.0) :
    GraphObject(imageID, startX, startY, dir, depth, size), this_world(world) {};
    
    // public methods
    virtual void doSomething() = 0;
    virtual bool isAlive() const = 0;
    virtual bool is_a_square() const = 0;
    virtual bool can_be_hit_by_vortex() const = 0;
    
    // public getter functions
    StudentWorld* get_thisWorld() const {return this_world;}
    
    
    // destructor
    virtual ~Actor() {}
    
private:
    StudentWorld *this_world; // variable to store pointer to which world this actor will belong to
};


// =====================/* PLAYER AVATAR (PEACH OR YOSHI) */=================================

class PlayerAvatar : public Actor
{
public:
    // constructor for a new Player Avatar
    PlayerAvatar(StudentWorld *world, int p, int imageID, int startX, int startY, int dir = 0, int depth = 0, double size = 1.0)
    : Actor(world, imageID, startX, startY, dir, depth, size), state("waiting_to_roll"), player(p), walkDirection(right), ticks_to_move(0), coins(0), stars(0), didJustLand(false), still_InStart(true), hasVortex(false), die_roll(0) {}
    
    // public methods
    virtual void doSomething();
    virtual bool isAlive() const {return true;} // players are always alive
    virtual bool is_a_square() const {return false;} // players are not squares
    virtual bool can_be_hit_by_vortex() const {return false;} // players can not be hit by a vortex
    bool hasAVortex() const {return hasVortex;}
    int findValidWalkingDirection(int currentWalkingDirection, Board b);
    bool justLanded() const {return didJustLand;}
    bool stillInStart() const {return still_InStart;}
    void teleportToRandomSquare(StudentWorld* world);
    bool isFork(Board b, std::set<int>& validDirections); // returns true if the player has reached a fork, stores valid directions to continue moving in a set passed in
    void swap_positions(PlayerAvatar* other);  // with other player
    void swap_stars(PlayerAvatar* other);  // with other player
    void swap_coins(PlayerAvatar* other);  // with other player
    void equip_with_vortex_projectile(); 
    
    // public getter functions
    std::string getState() const {return state;}
    int getPlayer() const {return player;} // 1 represents Peach and 2 represents Yoshi
    int get_walkDirection() const {return walkDirection;}
    int get_ticksToMove() const {return ticks_to_move;}
    
    int get_dice() const {return die_roll;} // used to display on status line
    int getCoins() const {return coins;} // used to display on status line
    int getStars() const {return stars;} // used to display on status line
    
    // public setter functions
    void setState(std::string newState);
    void set_walkDirection(int newDirection);
    void set_ticksToMove(int newTicksToMove);
    void setCoins(int newAmount);
    void setStars(int newAmount);
    void set_hasAVortex(int newStatus);
    
    
    // destructor
    virtual ~PlayerAvatar() {}
    
private:
    std::string state;
    int walkDirection;
    int player;
    int ticks_to_move;
    int coins;
    int stars;
    bool didJustLand;
    bool still_InStart;
    bool hasVortex;
    int die_roll;
        
};


// ==================================================================================================================
// ===============/* ACTIVATING OBJECT (EVERY OTHER ACTOR IN THE GAME BESIDES PLAYER) */==========================

class ActivatingObject : public Actor
{
public:
    // constructor for an ActivatingObject Object
    ActivatingObject(StudentWorld* world, int imageID, int startX, int startY, int dir, int depth, double size)
    : Actor(world, imageID, startX, startY, dir, depth, size) {}
    
    // public methods
    virtual void doSomething() {}
};


// ===============/* ACTIVATES WHEN COMES IN CONTACT WITH PLAYER (All Squares and Baddies) */==========================

class ActivateOnPlayer : public ActivatingObject
{
public:
    // constructor for an ActivateOnPlayer Object
    ActivateOnPlayer(StudentWorld* world, int imageID, int startX, int startY, bool activate_when_lands, int dir, int depth, double size) : ActivatingObject(world, imageID, startX, startY, dir, depth, size), activateWhenLands(activate_when_lands) {}
    
    // public methods
    bool activate_WhenLands() const {return activateWhenLands;}
    
private:
    bool activateWhenLands;
};


// ========================================/* SQUARES */========================================================

class CoinSquare : public ActivateOnPlayer
{
public:
    // constructor for a CoinSquare
    CoinSquare(StudentWorld *world, int imageID, int startX, int startY, bool activate_when_lands, std::string coinType, int dir, int depth, double size) : ActivateOnPlayer(world, imageID, startX, startY, activate_when_lands, dir, depth, size), type(coinType), active(true) {}
    
    // public methods
    virtual void doSomething();
    void coinSquareFunctionality(StudentWorld* world, PlayerAvatar* player);
    virtual bool isAlive() const {return active;} // coinsquare is NOT always alive, can be destroyed by bowser
    virtual bool is_a_square() const {return true;} // coinsquare is a type of square
    virtual bool can_be_hit_by_vortex() const {return false;} // coinsquares can not be hit by a vortex
    
    // public getter functions
    std::string get_coinType() const {return this->type;}
    
    // public setter functions
    void set_status_isAlive(bool status);
    
    // destructor
    virtual ~CoinSquare() {}
    
private:
    // add private data members as needed
    std::string type;
    bool active;
    
};

class BankSquare : public ActivateOnPlayer
{
public:
    // constructor for a Banksquare
    BankSquare(StudentWorld* world, int imageID, int startX, int startY, bool activate_when_lands, int dir, int depth, double size) : ActivateOnPlayer(world, imageID, startX, startY, activate_when_lands, dir, depth, size) {}
    
    // public methods
    virtual void doSomething();
    void bankSquareFunctionalityLands(StudentWorld* world, PlayerAvatar* player); // bank gives coins to player
    void bankSquareFunctionalityPasses(StudentWorld* world, PlayerAvatar* player); // bank takes coins from player
    virtual bool isAlive() const {return true;} // bank square is always alive
    virtual bool is_a_square() const {return true;} // bank square is a type of square
    virtual bool can_be_hit_by_vortex() const {return false;} // bank squares can not be hit by a vortex
    
    // destructor for a BankSquare
    virtual ~BankSquare() {}
    
};

class DirectionalSquare : public ActivateOnPlayer
{
public:
    // constructor for a DirectionalSquare
    DirectionalSquare(StudentWorld* world, int imageID, int startX, int startY, bool activate_when_lands, int dir, int depth, double size) : ActivateOnPlayer(world, imageID, startX, startY, activate_when_lands, dir, depth, size), directionOfSquare(dir) {}
    
    // public methods
    virtual void doSomething();
    void directionSquareFunctionality(PlayerAvatar* player);
    virtual bool isAlive() const {return true;} // direction square is always alive
    virtual bool is_a_square() const {return true;} // direction square is a type of square
    virtual bool can_be_hit_by_vortex() const {return false;} // direction squares can not be hit by a vortex
    
    // public getter functions
    int get_directionOfSquare() const {return directionOfSquare;}
    
    // destructor for a directional Square
    virtual ~DirectionalSquare() {}
private:
    int directionOfSquare;
};

class DroppingSquare : public ActivateOnPlayer
{
public:
    // constructor for a Dropping Square
    DroppingSquare(StudentWorld* world, int imageID, int startX, int startY, bool activate_when_lands, int dir, int depth, double size) : ActivateOnPlayer(world, imageID, startX, startY, activate_when_lands, dir, depth, size) {}
    
    // public methods
    virtual void doSomething();
    void droppingSquareFunctionalityDeductCoins(StudentWorld* world, PlayerAvatar* player);
    void droppingSquareFunctionalityDeductStars(StudentWorld* world, PlayerAvatar* player);
    virtual bool isAlive() const {return true;} // direction square is always alive
    virtual bool is_a_square() const {return true;} // direction square is a type of square
    virtual bool can_be_hit_by_vortex() const {return false;} // direction squares can not be hit by a vortex
    
    // destructor for a dropping square
    virtual ~DroppingSquare() {}
};

class EventSquare : public ActivateOnPlayer
{
public:
    // constructor for an Event Square
    EventSquare(StudentWorld* world, int imageID, int startX, int startY, bool activate_when_lands, int dir, int depth, double size) : ActivateOnPlayer(world, imageID, startX, startY, activate_when_lands, dir, depth, size) {}
    
    // public methods
    virtual void doSomething();
    void eventSquareFunctionalityTeleportation(StudentWorld* world, PlayerAvatar* player);
    void eventSquareFunctionalitySwap(StudentWorld* world, PlayerAvatar* player);
    void eventSquareFunctionalityVortex(StudentWorld* world, PlayerAvatar* player);
    virtual bool isAlive() const {return true;} //  event squares are always active
    virtual bool is_a_square() const {return true;} // event square is a type of square
    virtual bool can_be_hit_by_vortex() const {return false;} // event squares can not be hit by a vortex
    
    // destructor for an Event Square
    virtual ~EventSquare() {}
};

class StarSquare : public ActivateOnPlayer
{
public:
    // constructor for a Star Square
    StarSquare(StudentWorld *world, int imageID, int startX, int startY, bool activate_when_lands, int dir, int depth, double size) : ActivateOnPlayer(world, imageID, startX, startY, activate_when_lands, dir, depth, size) {}
    
    // public methods
    virtual void doSomething();
    void starSquareFunctionality(StudentWorld* world, PlayerAvatar* player);
    virtual bool isAlive() const {return true;} //  star squares are always active
    virtual bool is_a_square() const {return true;} // star square is a type of square
    virtual bool can_be_hit_by_vortex() const {return false;} // star squares can not be hit by a vortex
    
    // destructor for a Star Square
    virtual ~StarSquare() {}

};


// ========================================/* BADDIES */========================================================

class Baddies : public ActivateOnPlayer
{
    
};

class Bowser : public Baddies
{
    
};

class Boo : public Baddies
{
    
};


// ========================================/* VORTEX PROJECTILE */========================================================

class Vortex : public ActivatingObject
{
    
};


#endif // ACTOR_H_

