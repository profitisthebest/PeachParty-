#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include "Actor.h"
#include <string>
#include <vector>


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    virtual ~StudentWorld() {cleanUp();}
    
    // dropping square functionality
    void delete_square_at(int toEraseX, int toEraseY);
    void insert_dropping_square_at(int toInsertX, int toInsertY);
    
    // controlling the central bank
    int get_bank_coins() const {return centralBankOfCoins;}
    void deposit_bank_coins(int amountToDeposit) {centralBankOfCoins += amountToDeposit;}
    void reset_bank_coins() {centralBankOfCoins = 0;}
    PlayerAvatar* get_other_player(PlayerAvatar* currentPlayer) const;
    
    
    std::string getAssetPath() const {return my_assetPath;}
    PlayerAvatar* getPlayer(int playerID) const;
    
private:
    std::vector<Actor*> my_actors;
    PlayerAvatar* my_peach;
    PlayerAvatar* my_yoshi;
    std::string my_assetPath;
    int my_numberOfActors;
    int centralBankOfCoins;
    
};

#endif // STUDENTWORLD_H_
