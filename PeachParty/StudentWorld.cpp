#include "StudentWorld.h"
#include "GameConstants.h"
#include "GameWorld.h"
#include "Board.h"
#include <string>
#include <list>

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
