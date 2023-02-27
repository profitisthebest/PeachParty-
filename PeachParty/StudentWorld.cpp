#include "StudentWorld.h"
#include "GameConstants.h"
#include "GameWorld.h"
#include "Board.h"
#include <string>
#include <list>
using namespace std;

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
                        my_actors.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, i*SPRITE_WIDTH, j*SPRITE_HEIGHT));
                        my_numberOfActors++;
                        
                        // add a Peach
                        my_peach = new PlayerAvatar(this, 1, IID_PEACH, i*SPRITE_WIDTH, j*SPRITE_HEIGHT);
                        my_numberOfActors++;
                        
                        // for part two come back and add a Yoshi
                    }
                    case Board::blue_coin_square:
                    {
                        // PART 1
                        my_actors.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, i*SPRITE_WIDTH, j*SPRITE_HEIGHT));
                        my_numberOfActors++;
                        break;
                    }
                    case Board::red_coin_square:
                    {
                        // PART 2
                        break;
                    }
                    case Board::left_dir_square:
                    {
                        // PART 2
                        break;
                    }
                    case Board::right_dir_square:
                    {
                        // PART 2
                        break;
                    }
                    case Board::up_dir_square:
                    {
                        // PART 2
                        break;
                    }
                    case Board::down_dir_square:
                    {
                        // PART 2
                        break;
                    }
                    case Board::event_square:
                    {
                        // PART 2
                        break;
                    }
                    case Board::bank_square:
                    {
                        // PART 2
                        break;
                    }
                    case Board::star_square:
                    {
                        // PART 2
                        break;
                    }
                    case Board::bowser:
                    {
                        // PART 2
                        break;
                    }
                    case Board::boo:
                    {
                        // PART 2
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

    
    if (timeRemaining() <= 0)
    {
        playSound(SOUND_GAME_FINISHED);
        return GWSTATUS_PEACH_WON;
        // PART 2: add logic for winner between yoshi and peach
    }
    // the game isn't over yet so continue playing
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for(int i = 0; i < my_actors.size(); i++)
    {
        delete my_actors[i];
    }
    if (my_peach != nullptr)
    {
        delete my_peach;
    }
    if (my_yoshi != nullptr)
    {
        delete my_yoshi;
    }
    my_actors.clear();
}
