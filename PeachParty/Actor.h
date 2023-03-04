#ifndef ACTOR_H_
#define ACTOR_H_

#include <string>
#include <iostream>
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
    StudentWorld* get_thisWorld() const {return this_world;}
    
    
    // destructor
    virtual ~Actor() {}
    
private:
    StudentWorld *this_world; // variable to store pointer to which world this actor will belong to
};




// =====================/* LIVING ACTORS (PEACH, YOSHI, BOWSER, BOO) */==========================

class PlayerAvatar : public Actor
{
public:
    // constructor for a new Player Avatar
    PlayerAvatar(StudentWorld *world, int p, int imageID, int startX, int startY) :
    Actor(world, imageID, startX, startY), state("waiting_to_roll"), player(p), walkDirection(right), ticks_to_move(0) {}
    
    // public methods
    virtual void doSomething();
    virtual bool isAlive() const {return true;} // players are always alive
    std::string getState() const {return state;}
    void setState(std::string newState);
    
    
    int findValidWalkingDirection(int currentWalkingDirection, Board b);
    int getPlayer() const {return player;} // 1 represents Peach and 2 represents Yoshi
    int get_walkDirection() const {return walkDirection;} //
    void set_walkDirection(int newDirection);
    
    int get_ticksToMove() const {return ticks_to_move;}
    void set_ticksToMove(int newTicksToMove);
    
    // destructor
    virtual ~PlayerAvatar() {}
    
private:
    std::string state;
    int walkDirection;
    int player;
    int ticks_to_move;
    
    // add private data memebers as needed and make sure to correctly add public methods to acess them
    
};

class Baddies : public Actor
{
    // PART 2
};




// =====================/* COINSQUARES, STAR SQUARES, BANK SQUARES */==========================


class CoinSquare : public Actor
{
public:
    // constructor for a CoinSquare
    CoinSquare(StudentWorld *world, int imageID, int startX, int startY) :
    Actor(world, imageID, startX, startY, right, 1) {};
    
    // public methods
    virtual void doSomething() {}
    virtual bool isAlive() const {return true;} // coinsquare is always alive
    
    // destructor
    virtual ~CoinSquare() {}
    
private:
    // add private data members as needed
    
};





#endif // ACTOR_H_

