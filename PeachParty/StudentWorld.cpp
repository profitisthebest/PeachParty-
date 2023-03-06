#include "StudentWorld.h"
#include "GameConstants.h"
#include "GameWorld.h"
#include "Board.h"
#include <string>
#include <list>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

PlayerAvatar* StudentWorld::getPlayer(int playerID) const
{
    if (playerID == 1) return my_peach;
    if (playerID == 2) return my_yoshi;
    else return nullptr;
}

PlayerAvatar* StudentWorld::get_other_player(PlayerAvatar* currentPlayer) const
{
    if (currentPlayer == my_peach) return my_yoshi;
    if (currentPlayer == my_yoshi) return my_peach;
    else return nullptr;
}

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    my_assetPath = assetPath;
    my_numberOfActors = 0;
    my_peach = nullptr;
    my_yoshi = nullptr;
    centralBankOfCoins = 0;
}

void StudentWorld::delete_square_at(int toEraseX, int toEraseY)
{
    // want to check if there is a square at the position passed in and then delete it
    for(int i = 0; i<my_actors.size(); i++)
    {
        if (my_actors[i]->getX() == toEraseX && my_actors[i]->getY() == toEraseY && my_actors[i]->is_a_square())
        {
            my_actors[i]->setInactive();
        }
    }
}

void StudentWorld::insert_dropping_square_at(int toInsertX, int toInsertY)
{
    // want to insert a dropping square at the passed in x and y
    my_actors.push_back(new DroppingSquare(this, IID_DROPPING_SQUARE, toInsertX, toInsertY, true, 0, 1, 1));
    my_numberOfActors++;
}

int StudentWorld::init()
{
    // load board data
    Board bd;
    string board_file = assetPath() + "board0" + to_string(getBoardNumber()) + ".txt";
    Board::LoadResult result = bd.loadBoard(board_file);
    if (result == Board::load_fail_file_not_found || result == Board::load_fail_bad_format)
    {
        return GWSTATUS_BOARD_ERROR;
    }
    else if (result == Board::load_success)
    {
        // populate the board
        my_numberOfActors = 0;
        
        for(int i = 0; i < BOARD_WIDTH; i++)
        {
            for(int j = 0; j < BOARD_HEIGHT; j++)
            {
                Board::GridEntry ge = bd.getContentsOf(i, j);
                switch (ge)
                {
                    case Board::empty:
                    {
                        break;
                    }
                    case Board::player:
                    {
                        // add a Blue Coin Square under the PlayerAvatars
                        // a Blue coin activates when player lands on, not when player moves over
                        my_actors.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, true, "blue", 0, 1, 1));
                        my_numberOfActors++;
                        
                        // add a Peach
                        my_peach = new PlayerAvatar(this, 1, IID_PEACH, i*SPRITE_WIDTH, j*SPRITE_HEIGHT);
                        my_numberOfActors++;
                        
                        // add a Yoshi to the world
                        my_yoshi = new PlayerAvatar(this, 2, IID_YOSHI, i*SPRITE_WIDTH, j*SPRITE_HEIGHT);
                        my_numberOfActors++;
                        
                    }
                    case Board::blue_coin_square:
                    {
                        // PART 1
                        // a blue coin activates when player lands on, not when player moves over
                        my_actors.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, true, "blue", 0, 1, 1));
                        my_numberOfActors++;
                        break;
                    }
                    case Board::red_coin_square:
                    {
                        // PART 2
                        // a red coin activates when player lands on, not when player moves over
                        my_actors.push_back(new CoinSquare(this, IID_RED_COIN_SQUARE, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, true, "red", 0, 1, 1));
                        my_numberOfActors++;
                        break;
                    }
                    case Board::left_dir_square:
                    {
                        // PART 2
                        // left directional square activates when a player lands on, OR passes over
                        my_actors.push_back(new DirectionalSquare(this, IID_DIR_SQUARE, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, false, GraphObject::left, 1, 1));
                        my_numberOfActors++;
                        break;
                    }
                    case Board::right_dir_square:
                    {
                        // PART 2
                        my_actors.push_back(new DirectionalSquare(this, IID_DIR_SQUARE, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, false, GraphObject::right, 1, 1));
                        my_numberOfActors++;
                        break;
                    }
                    case Board::up_dir_square:
                    {
                        // PART 2
                        my_actors.push_back(new DirectionalSquare(this, IID_DIR_SQUARE, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, false, GraphObject::up, 1, 1));
                        my_numberOfActors++;
                        break;
                    }
                    case Board::down_dir_square:
                    {
                        // PART 2
                        my_actors.push_back(new DirectionalSquare(this, IID_DIR_SQUARE, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, false, GraphObject::down, 1, 1));
                        my_numberOfActors++;
                        break;
                    }
                    case Board::event_square:
                    {
                        // PART 2
                        my_actors.push_back(new EventSquare(this, IID_EVENT_SQUARE, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, true, 0, 1, 1));
                        my_numberOfActors++;
                        break;
                    }
                    case Board::bank_square:
                    {
                        // PART 2
                        my_actors.push_back(new BankSquare(this, IID_BANK_SQUARE, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, false, 0, 1, 1));
                        my_numberOfActors++;
                        break;
                    }
                    case Board::star_square:
                    {
                        // PART 2
                        my_actors.push_back(new StarSquare(this, IID_STAR_SQUARE, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, false, 0, 1, 1));
                        my_numberOfActors++;
                        break;
                    }
                    case Board::bowser:
                    {
                        // PART 2
                        my_actors.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, true, "blue", 0, 1, 1));
                        
                        my_actors.push_back(new Bowser(this, IID_BOWSER, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, true, 0, 180, 0, 0, 1));
                        my_numberOfActors += 2;
                        break;
                    }
                    case Board::boo:
                    {
                        // PART 2
                        my_actors.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, true, "blue", 0, 1, 1));
                        
                        // add a boo
                        
                        my_numberOfActors+=2;
                        break;
                    }
                }
            }
        }
        // start the countdown timer for 99 seconds
        startCountdownTimer(99);
    }
    return GWSTATUS_CONTINUE_GAME;
}


int StudentWorld::move()
{
    // Give each actor a chance to do something, incl. Peach and Yoshi
    if (my_peach->isAlive()) {
        my_peach->doSomething();
    }
    
     if (my_yoshi->isAlive()) {
        my_yoshi->doSomething();
    }

    for(int i = 0; i < my_actors.size(); i++)
    {
         if (my_actors[i]->isAlive()) // if the actor is still alive let it do something
         {
             // tell that actor to do something
             my_actors[i]->doSomething();
         }
        
        if (!my_actors[i]->isAlive()) // if the actor is dead, remove the actor after each tick
        {
            delete my_actors[i];
            my_actors.erase(my_actors.begin() + i);
            my_numberOfActors--;
        }
    }
    
    // Update the Game Status Line at the top of the screen (PART 2)
    ostringstream oss;
    oss.fill(' ');
    oss << "P1 Roll:";
    if (my_peach->get_dice() == 10)
        oss << setw(3) << my_peach->get_dice();
    else
        oss << setw(2) << my_peach->get_dice();
    
    oss << " Stars:";
    if (my_peach->getStars() >= 10)
        oss << setw(3) << my_peach->getStars();
    else
        oss << setw(2) << my_peach->getStars();
    
    oss << " $$:";
    if (my_peach->getCoins() >= 100)
        oss << setw(4) << my_peach->getCoins();
    else if (my_peach->getCoins() >= 10)
        oss << setw(3) << my_peach->getCoins();
    else // get coins less than 10
        oss << setw(2) << my_peach->getCoins();
    
    if (my_peach->hasAVortex())
        oss << setw(4) << "VOR";
    
    oss << " | Time:";
    if (timeRemaining() >= 10)
        oss << setw(3) << timeRemaining();
    else
        oss << setw(2) << timeRemaining();
    
    oss << " | Bank:";
    if (get_bank_coins() >= 100)
        oss << setw(4) << get_bank_coins();
    else if (get_bank_coins() >= 10)
        oss << setw(3) << get_bank_coins();
    else // get coins less than 10
        oss << setw(2) << get_bank_coins();
    
    oss << " | P2 Roll:";
    if (my_yoshi->get_dice() == 10)
        oss << setw(3) << my_yoshi->get_dice();
    else
        oss << setw(2) << my_yoshi->get_dice();
    
    oss << " Stars:";
    if (my_yoshi->getStars() >= 10)
        oss << setw(3) << my_yoshi->getStars();
    else
        oss << setw(2) << my_yoshi->getStars();
    
    oss << " $$:";
    if (my_yoshi->getCoins() >= 100)
        oss << setw(4) << my_yoshi->getCoins();
    else if (my_yoshi->getCoins() >= 10)
        oss << setw(3) << my_yoshi->getCoins();
    else // get coins less than 10
        oss << setw(2) << my_yoshi->getCoins();
    
    if (my_yoshi->hasAVortex())
        oss << setw(4) << "VOR";
    
    
    std::string text = oss.str();
    setGameStatText(text);
    
    if (timeRemaining() <= 0)
    {
        playSound(SOUND_GAME_FINISHED);
        // PART 2: add logic for winner between yoshi and peach
        if (my_peach->getStars() > my_yoshi->getStars()) // peach won
        {
            setFinalScore(my_peach->getStars(), my_peach->getCoins());
            return GWSTATUS_PEACH_WON;
        }
        else if (my_yoshi->getStars() > my_peach->getStars()) // yoshi won
        {
            setFinalScore(my_yoshi->getStars(), my_yoshi->getStars());
            return GWSTATUS_YOSHI_WON;
        }
        else // tie of stars
        {
            if (my_peach->getCoins() > my_yoshi->getCoins()) // peach won
            {
                setFinalScore(my_peach->getStars(), my_peach->getCoins());
                return GWSTATUS_PEACH_WON;
            }
            else if (my_yoshi->getCoins() > my_peach->getCoins()) // yoshi won
            {
                setFinalScore(my_yoshi->getStars(), my_yoshi->getStars());
                return GWSTATUS_YOSHI_WON;
            }
            else // both stars and coins are tied so pick random winner
            {
                int r = randInt(1, 2);
                if (r == 1) // peach wins
                {
                    setFinalScore(my_peach->getStars(), my_peach->getCoins());
                    return GWSTATUS_PEACH_WON;
                }
                else // yoshi wins
                {
                    setFinalScore(my_yoshi->getStars(), my_yoshi->getStars());
                    return GWSTATUS_YOSHI_WON;
                }
            }
        }
    }
    // the game isn't over yet so continue playing
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for(int i = 0; i < my_actors.size(); i++)
    {
        if (my_actors[i] != nullptr)
            delete my_actors[i];
    }
    delete my_peach;
    delete my_yoshi;
    my_actors.clear();
}
